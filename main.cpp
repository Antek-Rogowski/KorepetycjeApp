#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Inicjalizacja środowiska graficznego i pętli zdarzeń
    QApplication a(argc, argv);

    // Utworzenie i wyświetlenie głównego okna
    MainWindow w;
    w.show();

    // Uruchomienie aplikacji - program działa i czeka na interakcję użytkownika
    return a.exec();
}