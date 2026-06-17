#include "mainwindow.h"
#include "communicationtool.h"
#include <QApplication>
#include <QList>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "--- ROZPOCZĘCIE TESTÓW NARZĘDZI SPOTKANIA ---";

    // Inicjalizacja narzędzi w trakcie dołączania do spotkania
    ICommunicationTool* chat = new P2PChatTool();
    ICommunicationTool* video = new VideoStreamTool();

    // Pokój spotkania zarządza wszystkimi narzędziami poprzez ustandaryzowany interfejs
    QList<ICommunicationTool*> meetingTools = { chat, video };

    qDebug() << "\n>>> Uruchamianie spotkania...";
    for (ICommunicationTool* tool : meetingTools) {
        qDebug() << "Startowanie modułu:" << tool->getToolName();
        tool->start();
    }

    qDebug() << "\n>>> W trakcie spotkania...";
    // Bezpieczne rzutowanie (downcasting), aby użyć funkcji specyficznej dla czatu
    if (P2PChatTool* p2pChat = dynamic_cast<P2PChatTool*>(chat)) {
        p2pChat->sendMessage("Dzień dobry, czy dobrze mnie słychać?");
    }

    qDebug() << "\n>>> Zakończenie spotkania...";
    for (ICommunicationTool* tool : meetingTools) {
        tool->stop();
    }

    // Sprzątanie pamięci
    delete chat;
    delete video;

    qDebug() << "--- ZAKOŃCZENIE TESTÓW ---";

    return 0;
}