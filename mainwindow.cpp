#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

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

    // 6. Logika kliknięcia w przycisk - w Etapie 3 przeniesie nas to do pokoju spotkań
    connect(joinButton, &QPushButton::clicked, this, [this]() {
        qDebug() << ">>> INICJALIZACJA SPOTKANIA: Połączenie z Janem Kowalskim...";

        chatTool.start(); // Otwieramy gniazda komunikacyjne

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

    chatTool.stop(); // Zamykamy bezpiecznie gniazda komunikacyjne

    // Czyścimy historię na ekranie, by nie została do następnego spotkania
    ui->chatHistoryTextEdit->clear();

    ui->stackedWidget->setCurrentIndex(1);
}

