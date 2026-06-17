#include "communicationtool.h"
#include <QDebug>

P2PChatTool::P2PChatTool(QObject* parent)
    : QObject(parent), tcpServer(new QTcpServer(this)), tcpSocket(nullptr)
{
}

P2PChatTool::~P2PChatTool() {
    stop();
}

void P2PChatTool::start() {
    if (tcpServer->isListening()) return;

    // Logika MVP dla testów na jednym komputerze (localhost)
    if (tcpServer->listen(QHostAddress::Any, 12345)) {
        qDebug() << "[P2P] Instancja A (Serwer) nasłuchuje na porcie 12345";
        connect(tcpServer, &QTcpServer::newConnection, this, &P2PChatTool::onNewConnection);
    }
    else if (tcpServer->listen(QHostAddress::Any, 12346)) {
        qDebug() << "[P2P] Instancja B (Klient) nasłuchuje na porcie 12346";
        connect(tcpServer, &QTcpServer::newConnection, this, &P2PChatTool::onNewConnection);

        // Skoro jesteśmy drugą instancją, łączymy się z pierwszą
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, &QTcpSocket::readyRead, this, &P2PChatTool::onReadyRead);
        tcpSocket->connectToHost(QHostAddress::LocalHost, 12345);
    } else {
        qDebug() << "[P2P] Błąd krytyczny: Nie można otworzyć portów sieciowych.";
    }
}

void P2PChatTool::onNewConnection() {
    // Akceptujemy połączenie od drugiego użytkownika, o ile jeszcze go nie mamy
    if (!tcpSocket) {
        tcpSocket = tcpServer->nextPendingConnection();
        connect(tcpSocket, &QTcpSocket::readyRead, this, &P2PChatTool::onReadyRead);
        connect(tcpSocket, &QTcpSocket::disconnected, tcpSocket, &QTcpSocket::deleteLater);

        qDebug() << "[P2P] Zestawiono fizyczne połączenie z rozmówcą!";
        emit connectionEstablished();
    }
}

void P2PChatTool::onReadyRead() {
    // Odbieranie fizycznych bajtów i zamiana ich z powrotem na tekst
    if (tcpSocket) {
        QByteArray data = tcpSocket->readAll();
        QString message = QString::fromUtf8(data);
        qDebug() << "[P2P] Otrzymano z sieci:" << message;

        // Wysyłamy sygnał z tekstem do interfejsu graficznego
        emit messageReceived(message);
    }
}

void P2PChatTool::sendMessage(const QString& message) {
    // Wysyłamy dane tylko wtedy, gdy kanał TCP jest otwarty
    if (tcpSocket && tcpSocket->state() == QAbstractSocket::ConnectedState) {
        tcpSocket->write(message.toUtf8());
        tcpSocket->flush(); // Wymuszenie natychmiastowego wysłania bufora
        qDebug() << "[P2P] Wysłano w sieć:" << message;
    } else {
        qDebug() << "[P2P] Ostrzeżenie: Brak aktywnego połączenia TCP. Nie wysłano wiadomości.";
    }
}

void P2PChatTool::stop() {
    if (tcpSocket) {
        tcpSocket->disconnectFromHost();
        tcpSocket = nullptr;
    }
    if (tcpServer->isListening()) {
        tcpServer->close();
    }
    qDebug() << "[P2P] Gniazda sieciowe bezpiecznie zamknięte.";
}