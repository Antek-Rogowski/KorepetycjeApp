#ifndef COMMUNICATIONTOOL_H
#define COMMUNICATIONTOOL_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class P2PChatTool : public QObject {
    Q_OBJECT // Wymagane dla mechanizmu sygnałów i slotów
public:
    explicit P2PChatTool(QObject* parent = nullptr);
    ~P2PChatTool();

    void start();
    void stop();
    bool isServerInstance() const; // Zwraca true, jeśli zajęliśmy port główny (jesteśmy Serwerem)
    void sendMessage(const QString& message);
    void sendDiscoverPing(const QString& myName);

    void setMyName(const QString& name) { myIdentity = name; }
    QString getMyName() const { return myIdentity; }

    void sendInvite(const QString& targetName, const QString& subject, const QJsonArray& slots);
    void sendAccept(const QString& targetName, const QString& chosenTime);

    // Sygnały, które wyślemy do GUI (MainWindow), gdy coś przyjdzie z sieci
signals:
    void messageReceived(const QString& message);
    void connectionEstablished();
    void userDiscovered(const QString& name, const QHostAddress& ip, quint16 port);

    void inviteReceived(const QString& from, const QString& subject, const QJsonArray& timeSlots);
    void inviteAccepted(const QString& from, const QString& chosenTime);

private slots:
    void onNewConnection();
    void onReadyRead();
    void processDiscoveryDatagrams();

private:
    QTcpServer* tcpServer;
    QTcpSocket* tcpSocket;
    QUdpSocket *discoverySocket = nullptr;
    QString myIdentity = "Nieznajomy";
};

#endif // COMMUNICATIONTOOL_H