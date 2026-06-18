#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QFile>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
             explicit DatabaseManager(QObject *parent = nullptr);

    // Główne operacje użytkownika
    bool registerUser(const QString& username, const QString& password, const QString& role = "student");
    void loginUser(const QString& username, const QString& password);

    // Metody dostępowe, które wykorzystamy w kolejnym etapie
    QJsonObject getUserData(const QString& username);
    bool updateUserData(const QString& username, const QJsonObject& userData);

signals:
    // Sygnały zgodne ze starym AuthManagerem
    void loginSuccess(const QString& name, const QString& role);
    void loginFailed(const QString& errorMsg);

private:
    QString dbFilePath;

    // Prywatne metody zarządzające plikiem
    QJsonObject loadDatabase();
    bool saveDatabase(const QJsonObject& dbObj);
};

#endif // DATABASEMANAGER_H