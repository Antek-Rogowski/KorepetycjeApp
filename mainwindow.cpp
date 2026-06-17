#include <gst/gst.h>
#include <gst/video/videooverlay.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QComboBox>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Domyślnie ustawiamy pierwszą stronę (Logowanie) przy starcie aplikacji
    ui->stackedWidget->setCurrentIndex(0);

    // --- REJESTRACJA TESTOWA W BAZIE JSON ---
    // Przy pierwszym uruchomieniu tworzy plik i nadaje użytkownikom różne godziny
    dbManager.registerUser("Antoni", "admin", "tutor");
    dbManager.registerUser("Kama", "admin", "student");

    // ==========================================
    // 1. LOGIKA LOGOWANIA
    // ==========================================
    connect(&dbManager, &DatabaseManager::loginFailed, this, [this](const QString& errorMsg){
        ui->errorLabel->setStyleSheet("color: red;");
        ui->errorLabel->setText(errorMsg);
        ui->loginButton->setEnabled(true);
    });

    connect(&dbManager, &DatabaseManager::loginSuccess, this, [this](const QString& name, const QString& role){
        qDebug() << "[Baza Danych] Logowanie udane! Użytkownik:" << name << "Rola:" << role;
        ui->errorLabel->setStyleSheet("color: green;");
        ui->errorLabel->setText("Zalogowano pomyślnie!");

        // Wstrzykujemy tożsamość do silnika sieciowego
        chatTool.setMyName(name);

        populateCalendar(); // Rysuje kalendarz na podstawie JSON-a
        ui->stackedWidget->setCurrentIndex(1);
    });

    // ==========================================
    // 2. LOGIKA RADARU I KAFELKÓW DOSTĘPNOŚCI
    // ==========================================
    connect(&chatTool, &P2PChatTool::userDiscovered, this, [this](const QString& name, const QHostAddress& ip, quint16 port){
        // Zabezpieczenie przed dublowaniem (ukryty tag UserRole)
        for (int i = 0; i < ui->discoveredUsersList->count(); ++i) {
            if (ui->discoveredUsersList->item(i)->data(Qt::UserRole).toString() == name) {
                return;
            }
        }

        // Budowa kafelka
        QWidget* cardWidget = new QWidget();
        QHBoxLayout* cardLayout = new QHBoxLayout(cardWidget);
        cardLayout->setContentsMargins(5, 5, 5, 5);

        QLabel* infoLabel = new QLabel(QString("Dostępny: %1\nIP: %2").arg(name).arg(ip.toString()));
        QPushButton* inviteButton = new QPushButton("Zaproś");
        inviteButton->setStyleSheet("background-color: #2196F3; color: white; font-weight: bold; padding: 5px 15px; border-radius: 3px;");

        cardLayout->addWidget(infoLabel);
        cardLayout->addStretch();
        cardLayout->addWidget(inviteButton);

        QListWidgetItem* item = new QListWidgetItem(ui->discoveredUsersList);
        item->setData(Qt::UserRole, name);
        item->setSizeHint(cardWidget->sizeHint());
        ui->discoveredUsersList->setItemWidget(item, cardWidget);

        // Zmieniona lambda - nie przechwytujemy już ip i port!
        connect(inviteButton, &QPushButton::clicked, this, [this, name]() {
            QJsonObject myData = dbManager.getUserData(chatTool.getMyName());
            QJsonArray mySlots = myData["availability"].toArray();

            qDebug() << ">>> WYSYŁANIE ZAPROSZENIA [Z HARMONOGRAMEM] DO:" << name;
            // Używamy Imienia (name) zamiast IP
            chatTool.sendInvite(name, "Korepetycje C++ (GStreamer)", mySlots);

            QPushButton* btn = qobject_cast<QPushButton*>(sender());
            if (btn) {
                btn->setText("Wysłano");
                btn->setEnabled(false);
                btn->setStyleSheet("background-color: #9E9E9E; color: white; padding: 5px 15px; border-radius: 3px;");
            }
        });
    });

    // ==========================================
    // 3. ALGORYTM NEGOCJACJI TERMINÓW (A ∩ B)
    // ==========================================
    // Usunięto QHostAddress i quint16 z argumentów sygnału
    connect(&chatTool, &P2PChatTool::inviteReceived, this, [this](const QString& from, const QString& subject, const QJsonArray& remoteSlots){

        QJsonObject myData = dbManager.getUserData(chatTool.getMyName());
        QJsonArray mySlots = myData["availability"].toArray();

        QStringList commonSlots;
        for(const QJsonValue& rs : remoteSlots) {
            for(const QJsonValue& ms : mySlots) {
                if (rs.toString() == ms.toString()) {
                    commonSlots.append(rs.toString());
                }
            }
        }

        if (commonSlots.isEmpty()) {
            QMessageBox::warning(this, "Brak wspólnych terminów", QString("Użytkownik %1 zaprasza Cię na spotkanie, ale nie macie wspólnych wolnych godzin!").arg(from));
            return;
        }

        QDialog dialog(this);
        dialog.setWindowTitle("Nowe Zaproszenie");
        QVBoxLayout layout(&dialog);
        layout.addWidget(new QLabel(QString("%1 zaprasza Cię na spotkanie:\nTemat: %2\n\nWybierz pasujący termin:").arg(from, subject)));

        QComboBox* timeSelector = new QComboBox(&dialog);
        timeSelector->addItems(commonSlots);
        layout.addWidget(timeSelector);

        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        layout.addWidget(buttonBox);

        connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted) {
            QString chosenTime = timeSelector->currentText();

            QJsonArray myMeetings = myData["meetings"].toArray();
            QJsonObject newMeeting;
            newMeeting["with"] = from;
            newMeeting["subject"] = subject;
            newMeeting["time"] = chosenTime;
            myMeetings.append(newMeeting);
            myData["meetings"] = myMeetings;
            dbManager.updateUserData(chatTool.getMyName(), myData);

            // Odsyłamy używając Imienia zamiast IP/Port
            chatTool.sendAccept(from, chosenTime);

            populateCalendar();
            ui->stackedWidget->setCurrentIndex(1);
        }
    });
    // ==========================================
    // 4. POTWIERDZENIE ZAPROSZENIA U NADAWCY
    // ==========================================
    connect(&chatTool, &P2PChatTool::inviteAccepted, this, [this](const QString& from, const QString& chosenTime){

        // Zapis do naszej bazy
        QJsonObject myData = dbManager.getUserData(chatTool.getMyName());
        QJsonArray myMeetings = myData["meetings"].toArray();
        QJsonObject newMeeting;
        newMeeting["with"] = from;
        newMeeting["subject"] = "Korepetycje C++ (GStreamer)";
        newMeeting["time"] = chosenTime;
        myMeetings.append(newMeeting);
        myData["meetings"] = myMeetings;
        dbManager.updateUserData(chatTool.getMyName(), myData);

        QMessageBox::information(this, "Zaproszenie zaakceptowane",
                                 QString("Użytkownik %1 zaakceptował zaproszenie.\nWybrany termin: %2").arg(from, chosenTime));

        populateCalendar(); // Odświeżamy kalendarz, by spotkanie się pojawiło
    });

    // ==========================================
    // 5. OBSŁUGA CZATU TEKSTOWEGO (TCP)
    // ==========================================
    connect(&chatTool, &P2PChatTool::messageReceived, this, [this](const QString& msg){
        // Wyświetlamy otrzymaną wiadomość w oknie historii z dopiskiem
        ui->chatHistoryTextEdit->appendPlainText("Rozmówca: " + msg);
    });

    ui->stackedWidget->setCurrentIndex(0);
    initVideo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    QString username = ui->loginLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        ui->errorLabel->setText("Pola login i hasło nie mogą być puste!");
        return;
    }

    ui->errorLabel->clear();
    ui->loginButton->setEnabled(false);

    // Logowanie przez bazę danych
    dbManager.loginUser(username, password);
}

void MainWindow::on_logoutButton_clicked()
{
    qDebug() << "Wylogowano użytkownika. Powrót do ekranu logowania.";
    ui->loginLineEdit->clear();
    ui->passwordLineEdit->clear();
    ui->errorLabel->clear();
    ui->loginButton->setEnabled(true);
    ui->listWidget->clear();
    ui->discoveredUsersList->clear();
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::populateCalendar()
{
    ui->listWidget->clear();

    QJsonObject myData = dbManager.getUserData(chatTool.getMyName());
    QJsonArray myMeetings = myData["meetings"].toArray();

    // Rysujemy kafelki na podstawie danych z pliku JSON
    for (const QJsonValue& val : myMeetings) {
        QJsonObject meeting = val.toObject();
        QString withWho = meeting["with"].toString();
        QString subject = meeting["subject"].toString();
        QString time = meeting["time"].toString();

        QWidget* cardWidget = new QWidget();
        QHBoxLayout* cardLayout = new QHBoxLayout(cardWidget);

        QLabel* infoLabel = new QLabel(QString("Przedmiot: %1\nUczestnik: %2\nCzas: %3").arg(subject, withWho, time));

        QPushButton* joinButton = new QPushButton("Dołącz do spotkania");
        joinButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 5px 15px; border-radius: 3px;");

        cardLayout->addWidget(infoLabel);
        cardLayout->addStretch();
        cardLayout->addWidget(joinButton);

        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(cardWidget->sizeHint());
        ui->listWidget->setItemWidget(item, cardWidget);

        // Akcja wchodzenia do pokoju wideo P2P (GStreamer)
        connect(joinButton, &QPushButton::clicked, this, [this]() {
            qDebug() << ">>> INICJALIZACJA SPOTKANIA P2P Z GSTREAMEREM...";
            chatTool.start();

            int vIn = chatTool.isServerInstance() ? 5002 : 5000;
            int vOut = chatTool.isServerInstance() ? 5000 : 5002;
            int aIn = chatTool.isServerInstance() ? 5003 : 5001;
            int aOut = chatTool.isServerInstance() ? 5001 : 5003;

            QString videoSource = chatTool.isServerInstance() ? "ksvideosrc" : "videotestsrc pattern=ball is-live=true";
            QString audioSource = chatTool.isServerInstance() ? "autoaudiosrc" : "audiotestsrc wave=sine freq=440 volume=0.05 is-live=true";

            QString vRecv = QString("udpsrc port=%1 caps=\"application/x-rtp,media=video,clock-rate=90000,encoding-name=JPEG,payload=26\" ! rtpjpegdepay ! jpegdec ! videoconvert ! d3dvideosink name=mysink sync=false").arg(vIn);
            QString vSend = QString("%1 ! videoconvert ! video/x-raw,width=640,height=480,format=I420 ! tee name=t "
                                    "t. ! queue ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=%2 sync=false "
                                    "t. ! queue ! videoscale ! videoconvert ! d3dvideosink name=localsink sync=false").arg(videoSource).arg(vOut);

            QString aRecv = QString("udpsrc port=%1 caps=\"application/x-rtp,media=audio,clock-rate=48000,encoding-name=OPUS,payload=96\" ! rtpopusdepay ! opusdec ! audioconvert ! autoaudiosink sync=false").arg(aIn);
            QString aSend = QString("%1 ! volume name=mutesink ! audioconvert ! audioresample ! opusenc ! rtpopuspay ! udpsink host=127.0.0.1 port=%2 sync=false").arg(audioSource).arg(aOut);

            videoPipeline = gst_parse_launch(vRecv.toUtf8().constData(), nullptr);
            videoTransPipeline = gst_parse_launch(vSend.toUtf8().constData(), nullptr);
            audioPipeline = gst_parse_launch(aRecv.toUtf8().constData(), nullptr);
            audioTransPipeline = gst_parse_launch(aSend.toUtf8().constData(), nullptr);

            if (videoPipeline) {
                GstElement *sink = gst_bin_get_by_name(GST_BIN(videoPipeline), "mysink");
                if (sink) {
                    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(sink), (guintptr)ui->videoStreamLabel->winId());
                    gst_object_unref(sink);
                }
                gst_element_set_state(videoPipeline, GST_STATE_PLAYING);
            }

            if (videoTransPipeline) {
                GstElement *localSink = gst_bin_get_by_name(GST_BIN(videoTransPipeline), "localsink");
                if (localSink) {
                    gst_video_overlay_set_window_handle(GST_VIDEO_OVERLAY(localSink), (guintptr)ui->localVideoLabel->winId());
                    gst_object_unref(localSink);
                }
                gst_element_set_state(videoTransPipeline, GST_STATE_PLAYING);
            }

            if (audioPipeline) gst_element_set_state(audioPipeline, GST_STATE_PLAYING);
            if (audioTransPipeline) gst_element_set_state(audioTransPipeline, GST_STATE_PLAYING);

            ui->stackedWidget->setCurrentIndex(2);
        });
    }
}

void MainWindow::on_sendMessageButton_clicked()
{
    QString message = ui->chatInputLineEdit->text();
    if (message.isEmpty()) return;

    ui->chatHistoryTextEdit->appendPlainText("Ja: " + message);
    chatTool.sendMessage(message); // TCP Chat podczas wideo
    ui->chatInputLineEdit->clear();
}

void MainWindow::on_endCallButton_clicked()
{
    qDebug() << "Zakończono spotkanie. Powrót do kalendarza.";
    chatTool.stop();

    if (videoPipeline) {
        gst_element_set_state(videoPipeline, GST_STATE_NULL);
        gst_object_unref(videoPipeline);
        videoPipeline = nullptr;
    }

    if (audioPipeline) {
        gst_element_set_state(audioPipeline, GST_STATE_NULL);
        gst_object_unref(audioPipeline);
        audioPipeline = nullptr;
    }

    if (audioTransPipeline) {
        gst_element_set_state(audioTransPipeline, GST_STATE_NULL);
        gst_object_unref(audioTransPipeline);
        audioTransPipeline = nullptr;
    }

    if (videoTransPipeline) {
        gst_element_set_state(videoTransPipeline, GST_STATE_NULL);
        gst_object_unref(videoTransPipeline);
        videoTransPipeline = nullptr;
    }

    ui->chatHistoryTextEdit->clear();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::initVideo()
{
    qputenv("GST_REGISTRY_FORK", "no");
    qputenv("GST_DEBUG", "3");
    gst_init(nullptr, nullptr);
    ui->videoStreamLabel->setText("Strumień wideo zarządzany przez GStreamer");
}

void MainWindow::on_cameraButton_clicked()
{
    if (!videoTransPipeline) return;

    GstState currentState, pendingState;
    gst_element_get_state(videoTransPipeline, &currentState, &pendingState, 0);

    if (currentState == GST_STATE_PLAYING) {
        gst_element_set_state(videoTransPipeline, GST_STATE_PAUSED);
        ui->cameraButton->setText("Włącz kamerę");
    } else {
        gst_element_set_state(videoTransPipeline, GST_STATE_PLAYING);
        ui->cameraButton->setText("Wyłącz kamerę");
    }
}

void MainWindow::on_muteButton_clicked()
{
    if (!audioTransPipeline) return;

    GstElement *vol = gst_bin_get_by_name(GST_BIN(audioTransPipeline), "mutesink");
    if (vol) {
        gboolean isMuted;
        g_object_get(G_OBJECT(vol), "mute", &isMuted, NULL);
        g_object_set(G_OBJECT(vol), "mute", !isMuted, NULL);
        ui->muteButton->setText(isMuted ? "Wycisz" : "Włącz mikrofon");
        gst_object_unref(vol);
    }
}

void MainWindow::on_searchButton_clicked()
{
    ui->discoveredUsersList->clear();
    qDebug() << ">>> URUCHAMIANIE RADARU...";
    chatTool.sendDiscoverPing(chatTool.getMyName());
}