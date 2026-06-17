#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "authmanager.h"
#include "communicationtool.h"

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

private:
    Ui::MainWindow *ui;
    AuthManager authManager;
    P2PChatTool chatTool;

    // Wskaźnik na nasz rurociąg wideo GStreamer
    GstElement *videoPipeline = nullptr;

    void populateCalendar();
    void initVideo();
};
#endif // MAINWINDOW_H