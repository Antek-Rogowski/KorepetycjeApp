#include "communicationtool.h"
#include <QDebug>

P2PChatTool::P2PChatTool(QObject *parent) : QObject(parent) {
    // Inicjalizacja gniazda
    discoverySocket = new QUdpSocket(this);

    // --- WŁĄCZAMY RADAR W TLE OD RAZU PO URUCHOMIENIU ---
    // Wiążemy gniazdo z portem 45454 i pozwalamy innym instancjom go współdzielić
    discoverySocket->bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    // Zapisujemy się do wirtualnego pokoju Multicast
    discoverySocket->joinMulticastGroup(QHostAddress("239.255.43.21"));

    // Podpinamy sygnał odbioru pod naszą funkcję przetwarzającą
    connect(discoverySocket, &QUdpSocket::readyRead, this, &P2PChatTool::processDiscoveryDatagrams);

    qDebug() << "[Radar] Nasłuch Multicast uruchomiony w tle.";
}

P2PChatTool::~P2PChatTool() {
    stop();
}

void P2PChatTool::start() {
    if (tcpServer->isListening()) return;

    // ZESTAWIENIE "CIĘŻKIEGO" GNIAZDA TCP (Czat i Wideo)
    if (tcpServer->listen(QHostAddress::Any, 12345)) {
        qDebug() << "[P2P] Instancja A (Serwer) nasłuchuje na TCP 12345";
        connect(tcpServer, &QTcpServer::newConnection, this, &P2PChatTool::onNewConnection);
    }
    else if (tcpServer->listen(QHostAddress::Any, 12346)) {
        qDebug() << "[P2P] Instancja B (Klient) nasłuchuje na TCP 12346";
        connect(tcpServer, &QTcpServer::newConnection, this, &P2PChatTool::onNewConnection);

        // Skoro jesteśmy drugą instancją, łączymy się z pierwszą
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, &QTcpSocket::readyRead, this, &P2PChatTool::onReadyRead);
        tcpSocket->connectToHost(QHostAddress::LocalHost, 12345);
    } else {
        qDebug() << "[P2P] Błąd krytyczny: Nie można otworzyć portów sieciowych TCP.";
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

bool P2PChatTool::isServerInstance() const {
    // Jeśli nasz serwer nasłuchuje na porcie 12345, to my jesteśmy główną instancją (A)
    return (tcpServer->isListening() && tcpServer->serverPort() == 12345);
}

void P2PChatTool::sendDiscoverPing(const QString& myName) {
    if (!discoverySocket) return;

    QJsonObject json;
    json["type"] = "DISCOVER";
    json["name"] = myName;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson(QJsonDocument::Compact);

    discoverySocket->writeDatagram(data, QHostAddress("239.255.43.21"), 45454);
    qDebug() << "[Radar] Wysłano ping DISCOVER w sieć.";
}

void P2PChatTool::processDiscoveryDatagrams() {
    while (discoverySocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = discoverySocket->receiveDatagram();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(datagram.data(), &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject json = doc.object();
            QString type = json["type"].toString();
            QString name = json["name"].toString();

            if (type == "DISCOVER") {
                // 1. ZABEZPIECZENIE: Ignorujemy własne zapytania (Echo)
                if (name == myIdentity) return;

                qDebug() << "[Radar] Otrzymano zapytanie od:" << name;

                QJsonObject responseJson;
                responseJson["type"] = "PRESENCE";
                responseJson["name"] = myIdentity; // 2. Odpowiadamy NASZĄ tożsamością

                QByteArray responseData = QJsonDocument(responseJson).toJson(QJsonDocument::Compact);
                if (!datagram.senderAddress().isNull() && datagram.senderPort() > 0) {
                    discoverySocket->writeDatagram(responseData, QHostAddress("239.255.43.21"), 45454);
                }
            }
            else if (type == "PRESENCE") {
                // Zabezpieczenie: Ignorujemy własne odpowiedzi
                if (name == myIdentity) return;

                qDebug() << "[Radar] ZNALEZIONO UŻYTKOWNIKA:" << name;
                emit userDiscovered(name, datagram.senderAddress(), datagram.senderPort());
            }
        }
    }
}