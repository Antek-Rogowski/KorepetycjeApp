#ifndef COMMUNICATIONTOOL_H
#define COMMUNICATIONTOOL_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

class P2PChatTool : public QObject {
    Q_OBJECT // Wymagane dla mechanizmu sygnałów i slotów
public:
    explicit P2PChatTool(QObject* parent = nullptr);
    ~P2PChatTool();

    void start();
    void stop();
    bool isServerInstance() const; // Zwraca true, jeśli zajęliśmy port główny (jesteśmy Serwerem)
    void sendMessage(const QString& message);

    // Sygnały, które wyślemy do GUI (MainWindow), gdy coś przyjdzie z sieci
signals:
    void messageReceived(const QString& message);
    void connectionEstablished();

    // Wewnętrzne sloty do obsługi zdarzeń gniazd TCP
private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer* tcpServer;
    QTcpSocket* tcpSocket;
};

#endif // COMMUNICATIONTOOL_H