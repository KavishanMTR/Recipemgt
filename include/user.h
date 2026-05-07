#pragma once

#include <QJsonObject>
#include <QString>

class User {
private:
    QString username;
    QString passwordHash;
    QString email;
    bool rememberMe;

public:
    User();
    User(const QString& username, const QString& passwordHash, const QString& email = QString());

    QString getUsername() const;
    QString getPassword() const;
    QString getEmail() const;
    bool getRememberMe() const;

    void setUsername(const QString& username);
    void setPassword(const QString& passwordHash);
    void setEmail(const QString& email);
    void setRememberMe(bool remember);

    bool isValid() const;
    QJsonObject toJson() const;
    static User fromJson(const QJsonObject& object);
    QString toCSV() const;
    static User fromCSV(const QString& csv);
};
