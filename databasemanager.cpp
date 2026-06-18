#include "databasemanager.h"
#include <QCoreApplication>
#include <QDir>
#include <QDebug>

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent)
{
    // Ustalamy ścieżkę do pliku obok pliku wykonywalnego
    dbFilePath = QCoreApplication::applicationDirPath() + "/database.json";

    QFile file(dbFilePath);
    // Jeśli plik nie istnieje (pierwsze uruchomienie), tworzymy pusty szkielet
    if (!file.exists()) {
        QJsonObject root;
        root["users"] = QJsonObject(); // Pusty kontener na użytkowników
        saveDatabase(root);
        qDebug() << "[Baza Danych] Utworzono nową, pustą bazę danych JSON w:" << dbFilePath;
    }
}

QJsonObject DatabaseManager::loadDatabase()
{
    QFile file(dbFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "[Baza Danych] Błąd otwarcia pliku do odczytu.";
        return QJsonObject();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    return doc.object();
}

bool DatabaseManager::saveDatabase(const QJsonObject& dbObj)
{
    QFile file(dbFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "[Baza Danych] Błąd otwarcia pliku do zapisu.";
        return false;
    }

    QJsonDocument doc(dbObj);
    file.write(doc.toJson(QJsonDocument::Indented)); // Indented = ładnie sformatowany tekst
    file.close();
    return true;
}

bool DatabaseManager::registerUser(const QString& username, const QString& password, const QString& role)
{
    QJsonObject db = loadDatabase();
    QJsonObject users = db["users"].toObject();

    if (users.contains(username)) {
        return false; // Taki login już istnieje
    }

    QJsonObject newUser;
    newUser["password"] = password;
    newUser["role"] = role;

    // --- GENEROWANIE TESTOWEJ DOSTĘPNOŚCI ---
    QJsonArray availabilityArray;
    if (username == "Antoni") {
        // Antoni jako korepetytor ma szerokie okna
        availabilityArray.append("Poniedziałek 16:00");
        availabilityArray.append("Poniedziałek 17:00");
        availabilityArray.append("Poniedziałek 18:00");
        availabilityArray.append("Wtorek 18:00");
    } else if (username == "Kama") {
        // Kama ma tylko kilka wybranych godzin
        availabilityArray.append("Poniedziałek 18:00"); // <-- TO JEST CZĘŚĆ WSPÓLNA!
        availabilityArray.append("Poniedziałek 19:00");
        availabilityArray.append("Środa 16:00");
    } else {
        // Domyślnie dla reszty świata
        availabilityArray.append("Piątek 20:00");
    }

    newUser["availability"] = availabilityArray;
    newUser["meetings"] = QJsonArray();

    // Dodajemy użytkownika do drzewa i zapisujemy
    users[username] = newUser;
    db["users"] = users;

    qDebug() << "[Baza Danych] Zarejestrowano użytkownika z harmonogramem:" << username;
    return saveDatabase(db);
}

void DatabaseManager::loginUser(const QString& username, const QString& password)
{
    QJsonObject db = loadDatabase();
    QJsonObject users = db["users"].toObject();

    if (!users.contains(username)) {
        emit loginFailed("Użytkownik nie istnieje w bazie.");
        return;
    }

    QJsonObject userObj = users[username].toObject();
    if (userObj["password"].toString() != password) {
        emit loginFailed("Nieprawidłowe hasło.");
        return;
    }

    // Jeśli wszystko się zgadza, emitujemy sukces z imieniem i rolą
    emit loginSuccess(username, userObj["role"].toString());
}

QJsonObject DatabaseManager::getUserData(const QString& username)
{
    QJsonObject db = loadDatabase();
    QJsonObject users = db["users"].toObject();
    return users[username].toObject(); // Zwróci obiekt lub pusty JSON, jeśli nie ma
}

bool DatabaseManager::updateUserData(const QString& username, const QJsonObject& userData)
{
    QJsonObject db = loadDatabase();
    QJsonObject users = db["users"].toObject();

    if (!users.contains(username)) return false;

    users[username] = userData;
    db["users"] = users;
    return saveDatabase(db);
}