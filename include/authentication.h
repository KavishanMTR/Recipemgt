#pragma once

#include "user.h"

#include <QString>
#include <QVector>

class Authentication {
private:
    QVector<User> users;
    User* currentUser;
    QString usersFilePath;

    void loadUsers();
    void saveUsers();
    QString hashPassword(const QString& password) const;
    bool verifyPassword(const QString& input, const QString& storedHash) const;

public:
    Authentication();
    explicit Authentication(const QString& filePath);
    ~Authentication();

    bool login(const QString& username, const QString& password, bool rememberUser = false);
    bool signup(const QString& username, const QString& password, const QString& email = QString());
    void logout();
    bool isLoggedIn() const;
    User* getCurrentUser() const;
    bool userExists(const QString& username) const;
    bool changePassword(const QString& oldPassword, const QString& newPassword);
    QString getSavedUsername() const;
    void clearSavedUsername();
};
