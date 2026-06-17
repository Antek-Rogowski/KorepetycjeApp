#ifndef COMMUNICATIONTOOL_H
#define COMMUNICATIONTOOL_H

#include <QString>
#include <QDebug>

// Interfejs narzędzia komunikacyjnego (czysta abstrakcja)
class ICommunicationTool {
public:
    virtual ~ICommunicationTool() = default;

    // Czysto wirtualne metody, które narzucają kontrakt dla klas pochodnych
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isConnected() const = 0;
    virtual QString getToolName() const = 0;
};

// Implementacja: Czat tekstowy P2P
class P2PChatTool : public ICommunicationTool {
private:
    bool connectedStatus;

public:
    P2PChatTool();

    void start() override;
    void stop() override;
    bool isConnected() const override;
    QString getToolName() const override;

    // Metoda specyficzna tylko dla czatu
    void sendMessage(const QString& message);
};

// Implementacja: Strumieniowanie Wideo/Audio
class VideoStreamTool : public ICommunicationTool {
private:
    bool streamingStatus;

public:
    VideoStreamTool();

    void start() override;
    void stop() override;
    bool isConnected() const override;
    QString getToolName() const override;
};

#endif // COMMUNICATIONTOOL_H