/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.11.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *loginLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QLabel *errorLabel;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *userProfileLabel;
    QSpacerItem *verticalSpacer;
    QPushButton *logoutButton;
    QListWidget *listWidget;
    QWidget *page_3;
    QHBoxLayout *horizontalLayout_2;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *videoStreamLabel;
    QWidget *widget_4;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *muteButton;
    QPushButton *cameraButton;
    QPushButton *endCallButton;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout_2;
    QPlainTextEdit *chatHistoryTextEdit;
    QLineEdit *chatInputLineEdit;
    QPushButton *sendMessageButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        page = new QWidget();
        page->setObjectName("page");
        label = new QLabel(page);
        label->setObjectName("label");
        label->setGeometry(QRect(350, 220, 81, 16));
        label_2 = new QLabel(page);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(360, 280, 71, 16));
        loginLineEdit = new QLineEdit(page);
        loginLineEdit->setObjectName("loginLineEdit");
        loginLineEdit->setGeometry(QRect(300, 180, 191, 31));
        passwordLineEdit = new QLineEdit(page);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setGeometry(QRect(300, 250, 191, 31));
        passwordLineEdit->setEchoMode(QLineEdit::EchoMode::Password);
        loginButton = new QPushButton(page);
        loginButton->setObjectName("loginButton");
        loginButton->setGeometry(QRect(530, 320, 80, 24));
        errorLabel = new QLabel(page);
        errorLabel->setObjectName("errorLabel");
        errorLabel->setGeometry(QRect(480, 350, 161, 20));
        errorLabel->setStyleSheet(QString::fromUtf8("color: red;"));
        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        horizontalLayout = new QHBoxLayout(page_2);
        horizontalLayout->setObjectName("horizontalLayout");
        widget = new QWidget(page_2);
        widget->setObjectName("widget");
        widget->setMaximumSize(QSize(1677215, 16777215));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        userProfileLabel = new QLabel(widget);
        userProfileLabel->setObjectName("userProfileLabel");

        verticalLayout->addWidget(userProfileLabel);

        verticalSpacer = new QSpacerItem(20, 448, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        logoutButton = new QPushButton(widget);
        logoutButton->setObjectName("logoutButton");

        verticalLayout->addWidget(logoutButton);


        horizontalLayout->addWidget(widget);

        listWidget = new QListWidget(page_2);
        listWidget->setObjectName("listWidget");
        listWidget->setMaximumSize(QSize(500, 16777215));

        horizontalLayout->addWidget(listWidget);

        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        horizontalLayout_2 = new QHBoxLayout(page_3);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        widget_3 = new QWidget(page_3);
        widget_3->setObjectName("widget_3");
        verticalLayout_3 = new QVBoxLayout(widget_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        videoStreamLabel = new QLabel(widget_3);
        videoStreamLabel->setObjectName("videoStreamLabel");
        videoStreamLabel->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        videoStreamLabel->setStyleSheet(QString::fromUtf8("background-color: black; color: white;"));

        verticalLayout_3->addWidget(videoStreamLabel);

        widget_4 = new QWidget(widget_3);
        widget_4->setObjectName("widget_4");
        horizontalLayout_3 = new QHBoxLayout(widget_4);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        muteButton = new QPushButton(widget_4);
        muteButton->setObjectName("muteButton");

        horizontalLayout_3->addWidget(muteButton);

        cameraButton = new QPushButton(widget_4);
        cameraButton->setObjectName("cameraButton");

        horizontalLayout_3->addWidget(cameraButton);

        endCallButton = new QPushButton(widget_4);
        endCallButton->setObjectName("endCallButton");
        endCallButton->setStyleSheet(QString::fromUtf8("color: red;"));

        horizontalLayout_3->addWidget(endCallButton);


        verticalLayout_3->addWidget(widget_4);


        horizontalLayout_2->addWidget(widget_3);

        widget_2 = new QWidget(page_3);
        widget_2->setObjectName("widget_2");
        widget_2->setMaximumSize(QSize(200, 16777215));
        verticalLayout_2 = new QVBoxLayout(widget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        chatHistoryTextEdit = new QPlainTextEdit(widget_2);
        chatHistoryTextEdit->setObjectName("chatHistoryTextEdit");
        chatHistoryTextEdit->setReadOnly(true);

        verticalLayout_2->addWidget(chatHistoryTextEdit);

        chatInputLineEdit = new QLineEdit(widget_2);
        chatInputLineEdit->setObjectName("chatInputLineEdit");

        verticalLayout_2->addWidget(chatInputLineEdit);

        sendMessageButton = new QPushButton(widget_2);
        sendMessageButton->setObjectName("sendMessageButton");

        verticalLayout_2->addWidget(sendMessageButton);


        horizontalLayout_2->addWidget(widget_2);

        stackedWidget->addWidget(page_3);

        gridLayout->addWidget(stackedWidget, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Login:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Has\305\202o:", nullptr));
        loginButton->setText(QCoreApplication::translate("MainWindow", "Zaloguj", nullptr));
        errorLabel->setText(QString());
        userProfileLabel->setText(QCoreApplication::translate("MainWindow", "Witaj, ", nullptr));
        logoutButton->setText(QCoreApplication::translate("MainWindow", "Wyloguj", nullptr));
        videoStreamLabel->setText(QCoreApplication::translate("MainWindow", "Oczekiwanie na po\305\202\304\205czenie wideo...", nullptr));
        muteButton->setText(QCoreApplication::translate("MainWindow", "Wycisz", nullptr));
        cameraButton->setText(QCoreApplication::translate("MainWindow", "Wy\305\202\304\205cz kamer\304\231", nullptr));
        endCallButton->setText(QCoreApplication::translate("MainWindow", "Zako\305\204cz po\305\202\304\205czenie", nullptr));
        sendMessageButton->setText(QCoreApplication::translate("MainWindow", "Wy\305\233lij", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
