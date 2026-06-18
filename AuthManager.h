#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class AuthManager : public QObject {
    Q_OBJECT
private:
    QNetworkAccessManager* networkManager;
    const QString API_URL = "http://localhost:8080/api/v1/auth/login"; // Przykładowy adres testowy

public:
    explicit AuthManager(QObject *parent = nullptr);
    void login(const QString& username, const QString& password);

    // Sygnały, które wyślemy do interfejsu graficznego (UI) po zakończeniu zapytania
signals:
    void loginSuccess(const QString& userId, const QString& name, const QString& role);
    void loginFailed(const QString& errorMessage);
};

#endif // AUTHMANAGER_H