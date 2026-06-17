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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Domyślnie ustawiamy pierwszą stronę (Logowanie) przy starcie aplikacji
    ui->stackedWidget->setCurrentIndex(0);

    connect(&authManager, &AuthManager::loginFailed, this, [this](const QString& errorMsg){
        ui->errorLabel->setText(errorMsg);
        ui->loginButton->setEnabled(true);
    });

    connect(&authManager, &AuthManager::loginSuccess, this, [this](const QString& id, const QString& name, const QString& role){
        qDebug() << "Logowanie udane! Użytkownik:" << name;
        ui->errorLabel->setStyleSheet("color: green;");
        ui->errorLabel->setText("Zalogowano pomyślnie!");

        // Prawidłowe logowanie z serwera przełącza na Panel Główny (strona index 1)
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(&chatTool, &P2PChatTool::messageReceived, this, [this](const QString& msg){
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

    // --- MECHANIZM BYPASS (DO CELÓW DEWELOPERSKICH) ---
    if (username == "admin" && password == "admin") {
        qDebug() << "[TRYB DEWELOPERSKI] Bypass logowania. Przełączanie ekranu...";
        ui->errorLabel->clear();
        ui->loginLineEdit->clear();
        ui->passwordLineEdit->clear();

        populateCalendar(); // <--- ZASILAMY KALENDARZ TUTAJ

        ui->stackedWidget->setCurrentIndex(1);
        return;
    }
    // --------------------------------------------------

    ui->errorLabel->clear();
    ui->loginButton->setEnabled(false);
    authManager.login(username, password);
}

void MainWindow::on_logoutButton_clicked()
{
    qDebug() << "Wylogowano użytkownika. Powrót do ekranu logowania.";

    // Zmieniamy stronę w QStackedWidget z powrotem na pierwszą (Logowanie)
    ui->stackedWidget->setCurrentIndex(0);

    // Opcjonalnie: czyścimy kalendarz, żeby po ponownym zalogowaniu dane były świeże
    ui->listWidget->clear();
}

void MainWindow::populateCalendar()
{
    // Czyścimy listę, aby po wylogowaniu i ponownym zalogowaniu wydarzenia się nie dublowały
    ui->listWidget->clear();

    // --- TWORZENIE KARTY WYDARZENIA 1 ---
    // 1. Tworzymy pusty kontener na naszą kartę
    QWidget* cardWidget = new QWidget();
    QHBoxLayout* cardLayout = new QHBoxLayout(cardWidget);

    // 2. Tworzymy tekst z informacjami o spotkaniu (Tytuł, Uczestnik, Czas)
    QLabel* infoLabel = new QLabel("Przedmiot: Programowanie C++\nUczestnik: Jan Kowalski\nCzas: Dzisiaj, 18:00 - 19:00");

    // 3. Tworzymy przycisk dołączania
    QPushButton* joinButton = new QPushButton("Dołącz do spotkania");
    joinButton->setStyleSheet("background-color: #4CAF50; color: white; font-weight: bold; padding: 5px 15px;");

    // 4. Układamy to w naszej karcie (tekst po lewej, sprężyna rozpychająca, przycisk po prawej)
    cardLayout->addWidget(infoLabel);
    cardLayout->addStretch();
    cardLayout->addWidget(joinButton);

    // 5. Umieszczamy kartę w kalendarzu
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(cardWidget->sizeHint()); // Mówimy liście, jak duża ma być ta karta
    ui->listWidget->setItemWidget(item, cardWidget); // Zastępujemy zwykły tekst naszą piękną kartą

    connect(joinButton, &QPushButton::clicked, this, [this]() {
        qDebug() << ">>> INICJALIZACJA SPOTKANIA: Łączenie P2P...";
        chatTool.start();

        // 1. PORTY
        int vIn = chatTool.isServerInstance() ? 5002 : 5000;
        int vOut = chatTool.isServerInstance() ? 5000 : 5002;
        int aIn = chatTool.isServerInstance() ? 5003 : 5001;
        int aOut = chatTool.isServerInstance() ? 5001 : 5003;

        // 2. DEFINICJE
        QString videoSource = chatTool.isServerInstance() ? "videotestsrc pattern=smpte is-live=true" : "videotestsrc pattern=ball is-live=true";

        // Odbiornik Wideo
        QString vRecv = QString("udpsrc port=%1 caps=\"application/x-rtp,media=video,clock-rate=90000,encoding-name=JPEG,payload=26\" ! rtpjpegdepay ! jpegdec ! videoconvert ! d3dvideosink name=mysink sync=false").arg(vIn);

        // NADAJNIK Z TEE I WYMUSZONYM FORMATEM (Dodano format=I420)
        QString vSend = QString(
                            "%1 ! videoconvert ! video/x-raw,width=640,height=480,format=I420 ! tee name=t "
                            "t. ! queue ! jpegenc ! rtpjpegpay ! udpsink host=127.0.0.1 port=%2 sync=false "
                            "t. ! queue ! videoscale ! videoconvert ! d3dvideosink name=localsink sync=false"
                            ).arg(videoSource).arg(vOut);

        // Odbiornik i Nadajnik Audio
        QString aRecv = QString("udpsrc port=%1 caps=\"application/x-rtp,media=audio,clock-rate=48000,encoding-name=OPUS,payload=96\" ! rtpopusdepay ! opusdec ! audioconvert ! autoaudiosink sync=false").arg(aIn);
        QString aSend = QString("audiotestsrc wave=sine freq=440 volume=0.05 is-live=true ! volume name=mutesink ! audioconvert ! audioresample ! opusenc ! rtpopuspay ! udpsink host=127.0.0.1 port=%1 sync=false").arg(aOut);

        // 3. URUCHOMIENIE (Tylko po jednym razie na potok!)
        videoPipeline = gst_parse_launch(vRecv.toUtf8().constData(), nullptr);
        videoTransPipeline = gst_parse_launch(vSend.toUtf8().constData(), nullptr);
        audioPipeline = gst_parse_launch(aRecv.toUtf8().constData(), nullptr);
        audioTransPipeline = gst_parse_launch(aSend.toUtf8().constData(), nullptr);

        // 4. INTEGRACJA Z INTERFEJSEM (UI)
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
                // Przypinamy lokalny podgląd z trójnika do małego szarego ekranu
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


void MainWindow::on_sendMessageButton_clicked()
{
    // 1. Pobieramy tekst wpisany przez użytkownika
    QString message = ui->chatInputLineEdit->text();

    // Zabezpieczenie: jeśli pole jest puste, ignorujemy kliknięcie
    if (message.isEmpty()) {
        return;
    }

    // 2. Logika UI: Wyświetlamy naszą wiadomość w oknie historii
    ui->chatHistoryTextEdit->appendPlainText("Ja: " + message);

    // 3. Logika Backend: Przekazujemy tekst do naszego narzędzia
    // (na razie wyświetli się w konsoli qDebug, w Etapie 3 pójdzie w sieć TCP)
    chatTool.sendMessage(message);

    // 4. Logika UI: Czyścimy pole wprowadzania po udanym wysłaniu
    ui->chatInputLineEdit->clear();
}

void MainWindow::on_endCallButton_clicked()
{
    qDebug() << "Zakończono spotkanie. Powrót do kalendarza.";
    chatTool.stop();

    // Bezpieczne zatrzymanie i zwolnienie zasobów rurociągu wideo
    if (videoPipeline) {
        qDebug() << "[GStreamer] Zatrzymywanie rurociągu...";
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
    // Wyłączenie zewnętrznego skanera wtyczek oraz włączenie logów (DEBUG)
    qputenv("GST_REGISTRY_FORK", "no");
    qputenv("GST_DEBUG", "3");

    qDebug() << "[GStreamer] Inicjalizacja bibliotek...";
    gst_init(nullptr, nullptr);

    ui->videoStreamLabel->setText("Strumień wideo zarządzany przez GStreamer");
}


void MainWindow::on_cameraButton_clicked()
{
    // Używamy potoku NADAWCZEGO
    if (!videoTransPipeline) return;

    GstState currentState, pendingState;
    gst_element_get_state(videoTransPipeline, &currentState, &pendingState, 0);

    if (currentState == GST_STATE_PLAYING) {
        gst_element_set_state(videoTransPipeline, GST_STATE_PAUSED);
        ui->cameraButton->setText("Włącz kamerę");
        qDebug() << "[GStreamer] Nadawanie wstrzymane (PAUSED)";
    } else {
        gst_element_set_state(videoTransPipeline, GST_STATE_PLAYING);
        ui->cameraButton->setText("Wyłącz kamerę");
        qDebug() << "[GStreamer] Nadawanie wznowione (PLAYING)";
    }
}


void MainWindow::on_muteButton_clicked() // Upewnij się, że masz podpięty ten slot!
{
    if (!audioTransPipeline) return;

    GstElement *vol = gst_bin_get_by_name(GST_BIN(audioTransPipeline), "mutesink");
    if (vol) {
        gboolean isMuted;
        g_object_get(G_OBJECT(vol), "mute", &isMuted, NULL);

        g_object_set(G_OBJECT(vol), "mute", !isMuted, NULL);

        ui->muteButton->setText(isMuted ? "Wycisz" : "Włącz mikrofon");
        qDebug() << "[Audio] Stan wyciszenia:" << !isMuted;

        gst_object_unref(vol);
    }
}

