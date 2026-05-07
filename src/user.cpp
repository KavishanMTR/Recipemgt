#include "user.h"

#include <QStringList>

namespace {
QString escapeCsvPart(const QString& value) {
    QString escaped = value;
    escaped.replace("\\", "\\\\");
    escaped.replace(",", "\\,");
    return escaped;
}

QStringList splitEscapedCsv(const QString& csv) {
    QStringList parts;
    QString current;
    bool escaping = false;

    for (const QChar ch : csv) {
        if (escaping) {
            current.append(ch);
            escaping = false;
            continue;
        }

        if (ch == '\\') {
            escaping = true;
            continue;
        }

        if (ch == ',') {
            parts.append(current);
            current.clear();
            continue;
        }

        current.append(ch);
    }

    parts.append(current);
    return parts;
}
}

User::User() : rememberMe(false) {}

User::User(const QString& username, const QString& passwordHash, const QString& email)
    : username(username.trimmed()),
      passwordHash(passwordHash),
      email(email.trimmed()),
      rememberMe(false) {}

QString User::getUsername() const { return username; }
QString User::getPassword() const { return passwordHash; }
QString User::getEmail() const { return email; }
bool User::getRememberMe() const { return rememberMe; }

void User::setUsername(const QString& value) { username = value.trimmed(); }
void User::setPassword(const QString& value) { passwordHash = value; }
void User::setEmail(const QString& value) { email = value.trimmed(); }
void User::setRememberMe(bool value) { rememberMe = value; }

bool User::isValid() const {
    return !username.trimmed().isEmpty() && !passwordHash.trimmed().isEmpty();
}

QJsonObject User::toJson() const {
    return {
        {"username", username},
        {"passwordHash", passwordHash},
        {"email", email},
        {"rememberMe", rememberMe}
    };
}

User User::fromJson(const QJsonObject& object) {
    User user(
        object.value("username").toString(),
        object.value("passwordHash").toString(),
        object.value("email").toString()
    );
    user.setRememberMe(object.value("rememberMe").toBool(false));
    return user;
}

QString User::toCSV() const {
    return escapeCsvPart(username) + "," +
           escapeCsvPart(passwordHash) + "," +
           escapeCsvPart(email);
}

User User::fromCSV(const QString& csv) {
    const QStringList parts = splitEscapedCsv(csv);
    if (parts.size() < 2) {
        return User();
    }

    return User(parts.value(0), parts.value(1), parts.value(2));
}
