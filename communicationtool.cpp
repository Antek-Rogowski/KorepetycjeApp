#include "communicationtool.h"

// --- Implementacja P2PChatTool ---
P2PChatTool::P2PChatTool() : connectedStatus(false) {}

void P2PChatTool::start() {
    qDebug() << "[P2PChatTool] Nawiązywanie połączenia peer-to-peer...";
    connectedStatus = true;
    qDebug() << "[P2PChatTool] Połączono.";
}

void P2PChatTool::stop() {
    qDebug() << "[P2PChatTool] Zamykanie gniazd sieciowych i czyszczenie historii.";
    connectedStatus = false;
}

bool P2PChatTool::isConnected() const {
    return connectedStatus;
}

QString P2PChatTool::getToolName() const {
    return "Czat P2P";
}

void P2PChatTool::sendMessage(const QString& message) {
    if (connectedStatus) {
        qDebug() << "[P2PChatTool] Wysyłanie wiadomości:" << message;
    } else {
        qDebug() << "[P2PChatTool] Błąd: Brak połączenia P2P!";
    }
}


// --- Implementacja VideoStreamTool ---
VideoStreamTool::VideoStreamTool() : streamingStatus(false) {}

void VideoStreamTool::start() {
    qDebug() << "[VideoStreamTool] Inicjalizacja biblioteki A/V (np. GStreamer)...";
    qDebug() << "[VideoStreamTool] Przechwytywanie obrazu z kamery i mikrofonu...";
    streamingStatus = true;
}

void VideoStreamTool::stop() {
    qDebug() << "[VideoStreamTool] Zatrzymywanie strumieniowania. Zwalnianie sprzętu.";
    streamingStatus = false;
}

bool VideoStreamTool::isConnected() const {
    return streamingStatus;
}

QString VideoStreamTool::getToolName() const {
    return "Strumień Audio/Wideo";
}