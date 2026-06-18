#include "communicationtool.h"
#include <QDebug>

P2PChatTool::P2PChatTool(QObject *parent) : QObject(parent) {
    // 1. BEZPIECZNE WSKAŹNIKI TCP (Rozwiązanie Twojego błędu!)
    tcpServer = new QTcpServer(this);
    tcpSocket = nullptr;

    // 2. RADAR UDP (Praca w tle)
    discoverySocket = new QUdpSocket(this);
    discoverySocket->bind(QHostAddress::AnyIPv4, 45454, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    discoverySocket->joinMulticastGroup(QHostAddress("239.255.43.21"));
    connect(discoverySocket, &QUdpSocket::readyRead, this, &P2PChatTool::processDiscoveryDatagrams);

    qDebug() << "[Radar] Nasłuch Multicast uruchomiony w tle.";
}

P2PChatTool::~P2PChatTool() {
    stop();
}

void P2PChatTool::start() {
    // Dodatkowa tarcza ochronna, gdyby serwer został zniszczony podczas zamykania poprzedniej rozmowy
    if (!tcpServer) {
        tcpServer = new QTcpServer(this);
    }

    if (tcpServer->isListening()) return;

    // ZESTAWIENIE "CIĘŻKIEGO" GNIAZDA TCP (Czat i Wideo)
    if (tcpServer->listen(QHostAddress::Any, 12345)) {
        qDebug() << "[P2P] Instancja A (Serwer) nasłuchuje na TCP 12345";
        connect(tcpServer, &QTcpServer::newConnection, this, &P2PChatTool::onNewConnection);
    }
    else if (tcpServer->listen(QHostAddress::Any, 12346)) {
        qDebug() << "[P2P] Instancja B (Klient) nasłuchuje na TCP 12346";
        connect(tcpServer, &QTcpServer::newConnection, this, &P2PChatTool::onNewConnection);

        // Klient łączy się do Serwera
        tcpSocket = new QTcpSocket(this);
        connect(tcpSocket, &QTcpSocket::readyRead, this, &P2PChatTool::onReadyRead);
        tcpSocket->connectToHost(QHostAddress::LocalHost, 12345);
    } else {
        qDebug() << "[P2P] Błąd krytyczny: Nie można otworzyć portów sieciowych TCP.";
    }
}

void P2PChatTool::stop() {
    if (tcpServer && tcpServer->isListening()) {
        tcpServer->close();
    }
    if (tcpSocket) {
        tcpSocket->disconnectFromHost();
        tcpSocket->deleteLater();
        tcpSocket = nullptr;
    }
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

void P2PChatTool::sendInvite(const QString& targetName, const QString& subject, const QJsonArray& timeSlots) {
    if (!discoverySocket) return;

    QJsonObject json;
    json["type"] = "INVITE";
    json["from"] = myIdentity;
    json["to"] = targetName; // Wskazujemy do kogo ma trafić z powrotem
    json["subject"] = subject;
    json["slots"] = timeSlots;

    QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
    // Wysyłamy prosto w pokój Multicast (zamiast używać parametru ip i port)
    discoverySocket->writeDatagram(data, QHostAddress("239.255.43.21"), 45454);
}

void P2PChatTool::sendAccept(const QString& targetName, const QString& chosenTime) {
    if (!discoverySocket) return;

    QJsonObject json;
    json["type"] = "ACCEPT";
    json["from"] = targetName;
    json["by"] = myIdentity;
    json["time"] = chosenTime;

    QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
    // Wysyłamy prosto w pokój Multicast
    discoverySocket->writeDatagram(data, QHostAddress("239.255.43.21"), 45454);
}

void P2PChatTool::processDiscoveryDatagrams() {
    while (discoverySocket->hasPendingDatagrams()) {
        QNetworkDatagram datagram = discoverySocket->receiveDatagram();
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(datagram.data(), &error);

        if (error.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject json = doc.object();
            QString type = json["type"].toString();

            if (type == "DISCOVER") {
                QString name = json["name"].toString();
                if (name == myIdentity) return;

                QJsonObject responseJson;
                responseJson["type"] = "PRESENCE";
                responseJson["name"] = myIdentity;
                QByteArray responseData = QJsonDocument(responseJson).toJson(QJsonDocument::Compact);
                discoverySocket->writeDatagram(responseData, QHostAddress("239.255.43.21"), 45454);
            }
            else if (type == "PRESENCE") {
                QString name = json["name"].toString();
                if (name == myIdentity) return;

                emit userDiscovered(name, datagram.senderAddress(), datagram.senderPort());
            }
            else if (type == "INVITE") {
                QString to = json["to"].toString();
                // TARCZA 1: Ignorujemy, jeśli to zaproszenie nie jest do nas!
                if (to != myIdentity) return;

                QString from = json["from"].toString();
                QString subject = json["subject"].toString();
                QJsonArray timeSlots = json["slots"].toArray();

                emit inviteReceived(from, subject, timeSlots);
            }
            else if (type == "ACCEPT") {
                QString from = json["from"].toString();
                QString by = json["by"].toString();
                if (by == myIdentity) return;

                // TARCZA 2: Reagujemy tylko, jeśli akceptacja dotyczy naszego zaproszenia
                if (from == myIdentity) {
                    QString time = json["time"].toString();
                    emit inviteAccepted(by, time);
                }
            }
        }
    }
}

// 1. Zestawienie połączenia przez instancję Serwera
void P2PChatTool::onNewConnection() {
    if (tcpServer->hasPendingConnections()) {
        tcpSocket = tcpServer->nextPendingConnection();
        connect(tcpSocket, &QTcpSocket::readyRead, this, &P2PChatTool::onReadyRead);
        qDebug() << "[P2P TCP] Zestawiono połączenie czatu z rozmówcą!";
    }
}

// 2. Wysyłanie wiadomości na zewnątrz
void P2PChatTool::sendMessage(const QString& message) {
    if (tcpSocket && tcpSocket->state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["type"] = "CHAT";
        json["payload"] = message;

        QByteArray data = QJsonDocument(json).toJson(QJsonDocument::Compact);
        tcpSocket->write(data);
    } else {
        qDebug() << "[P2P TCP] Gniazdo nie jest połączone. Nie można wysłać wiadomości.";
    }
}

// 3. Odbieranie wiadomości z zewnątrz
void P2PChatTool::onReadyRead() {
    if (!tcpSocket) return;

    QByteArray data = tcpSocket->readAll();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error == QJsonParseError::NoError && doc.isObject()) {
        QJsonObject json = doc.object();
        if (json["type"].toString() == "CHAT") {
            QString payload = json["payload"].toString();
            // Wypuszczamy sygnał do interfejsu
            emit messageReceived(payload);
        }
    }
}