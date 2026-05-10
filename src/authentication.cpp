#include "authentication.h"

#include "filehandler.h"

#include <QCryptographicHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

Authentication::Authentication() : currentUser(nullptr) {
    usersFilePath = FileHandler::getUsersFilePath();
    loadUsers();

    if (users.isEmpty()) {
        signup("admin", "admin123", "admin@recipe.app");
    }
}

Authentication::Authentication(const QString& filePath)
    : currentUser(nullptr),
      usersFilePath(filePath) {
    loadUsers();
}

Authentication::~Authentication() {
    delete currentUser;
}

void Authentication::loadUsers() {
    users.clear();

    const QString raw = FileHandler::readFile(usersFilePath).trimmed();
    if (raw.isEmpty()) {
        return;
    }

    if (raw.startsWith("[")) {
        QJsonParseError error;
        const QJsonDocument document = QJsonDocument::fromJson(raw.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError && document.isArray()) {
            for (const QJsonValue& value : document.array()) {
                const User user = User::fromJson(value.toObject());
                if (user.isValid()) {
                    users.append(user);
                }
            }
        }
        return;
    }

    const QStringList lines = raw.split("\n", Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        const User user = User::fromCSV(line.trimmed());
        if (user.isValid()) {
            users.append(user);
        }
    }

    saveUsers();
}

void Authentication::saveUsers() {
    QJsonArray array;
    for (const User& user : users) {
        array.append(user.toJson());
    }

    const QJsonDocument document(array);
    FileHandler::writeFile(usersFilePath, QString::fromUtf8(document.toJson(QJsonDocument::Indented)));
}

QString Authentication::hashPassword(const QString& password) const {
    return QString::fromUtf8(
        QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex()
    );
}

bool Authentication::verifyPassword(const QString& input, const QString& storedHash) const {
    return hashPassword(input) == storedHash;
}

bool Authentication::login(const QString& username, const QString& password, bool rememberUser) {
    const QString normalizedUsername = username.trimmed();
    for (const User& user : users) {
        if (user.getUsername().compare(normalizedUsername, Qt::CaseInsensitive) == 0 &&
            verifyPassword(password, user.getPassword())) {
            delete currentUser;
            currentUser = new User(user);
            FileHandler::writeSetting("auth/rememberedUser", rememberUser ? user.getUsername() : QString());
            return true;
        }
    }

    return false;
}

bool Authentication::signup(const QString& username, const QString& password, const QString& email) {
    const QString normalizedUsername = username.trimmed();
    if (normalizedUsername.isEmpty() || password.trimmed().length() < 6 || userExists(normalizedUsername)) {
        return false;
    }

    User newUser(normalizedUsername, hashPassword(password), email.trimmed());
    users.append(newUser);
    saveUsers();
    return true;
}

void Authentication::logout() {
    delete currentUser;
    currentUser = nullptr;
}

bool Authentication::isLoggedIn() const {
    return currentUser != nullptr;
}

User* Authentication::getCurrentUser() const {
    return currentUser;
}

bool Authentication::userExists(const QString& username) const {
    for (const User& user : users) {
        if (user.getUsername().compare(username.trimmed(), Qt::CaseInsensitive) == 0) {
            return true;
        }
    }
    return false;
}

bool Authentication::changePassword(const QString& oldPassword, const QString& newPassword) {
    if (!currentUser || newPassword.trimmed().length() < 6) {
        return false;
    }

    if (!verifyPassword(oldPassword, currentUser->getPassword())) {
        return false;
    }

    for (User& user : users) {
        if (user.getUsername() == currentUser->getUsername()) {
            const QString hashedPassword = hashPassword(newPassword);
            user.setPassword(hashedPassword);
            currentUser->setPassword(hashedPassword);
            saveUsers();
            return true;
        }
    }

    return false;
}

QString Authentication::getSavedUsername() const {
    return FileHandler::readSetting("auth/rememberedUser", QString()).toString();
}

void Authentication::clearSavedUsername() {
    FileHandler::writeSetting("auth/rememberedUser", QString());
}
