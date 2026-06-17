#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "authmanager.h"
#include "communicationtool.h" // Załączamy interfejsy narzędzi komunikacyjnych

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();
    void on_logoutButton_clicked();
    void on_endCallButton_clicked();
    // Tutaj zaraz wygenerujesz slot dla przycisku wysyłania

    void on_sendMessageButton_clicked();

private:
    Ui::MainWindow *ui;
    AuthManager authManager;
    P2PChatTool chatTool; // Instancja naszego narzędzia do czatu

    void populateCalendar();
};
#endif // MAINWINDOW_H