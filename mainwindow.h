#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "authmanager.h"
#include "communicationtool.h"
#include "databasemanager.h"

// Definiujemy strukturę GStreamera bez dołączania całego nagłówka C do C++ (forward declaration)
struct _GstElement;
typedef _GstElement GstElement;

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
    void on_sendMessageButton_clicked();
    void on_cameraButton_clicked();

    void on_muteButton_clicked();

    void on_searchButton_clicked();

    void on_Zarejestruj_clicked();

    void on_removeTimeButton_clicked();
    void on_addTimeButton_clicked();

private:
    Ui::MainWindow *ui;
    //AuthManager authManager;
    DatabaseManager dbManager;
    P2PChatTool chatTool;

    // Wskaźnik na nasz rurociąg wideo GStreamer
    GstElement *videoPipeline = nullptr;
    GstElement *audioPipeline = nullptr;
    GstElement *audioTransPipeline = nullptr; // Dodaj to
    GstElement *videoTransPipeline = nullptr; // <--- Dodajemy potok nadawczy wideo

    void populateCalendar();
    void initVideo();
};
#endif // MAINWINDOW_H
