#include "authmanager.h"
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>

AuthManager::AuthManager(QObject *parent) : QObject(parent) {
    // Inicjalizacja menedżera zapytań sieciowych
    networkManager = new QNetworkAccessManager(this);
}

void AuthManager::login(const QString& username, const QString& password) {
    // 1. Budowanie struktury JSON do wysłania
    QJsonObject jsonBody;
    jsonBody["login"] = username;
    jsonBody["password"] = password;
    QJsonDocument doc(jsonBody);
    QByteArray data = doc.toJson();

    // 2. Przygotowanie żądania sieciowego (Request)
    QNetworkRequest request((QUrl(API_URL)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 3. Wysłanie zapytania POST
    QNetworkReply* reply = networkManager->post(request, data);

    // 4. Obsługa odpowiedzi za pomocą lambdy (wywoła się, gdy serwer odpowie)
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // Sukces sieciowy - parsujemy odpowiedź JSON
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
            QJsonObject jsonObject = jsonResponse.object();

            if (jsonObject["status"].toString() == "success") {
                QJsonObject dataObject = jsonObject["data"].toObject();
                QString userId = dataObject["userId"].toString();
                QString name = dataObject["name"].toString();
                QString role = dataObject["role"].toString();

                // Informujemy UI o sukcesie
                emit loginSuccess(userId, name, role);
            } else {
                emit loginFailed("Nieprawidłowy login lub hasło.");
            }
        } else {
            // Błąd sieciowy (np. brak połączenia z serwerem)
            emit loginFailed("Błąd połączenia z serwerem: " + reply->errorString());
        }

        // Czyszczenie pamięci po obiekcie zapytania
        reply->deleteLater();
    });
}