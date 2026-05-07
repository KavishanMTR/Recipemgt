#pragma once

#include "authentication.h"

#include <QCheckBox>
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class LoginWindow : public QDialog {
    Q_OBJECT

private:
    QLabel* emailFieldLabel;
    QLabel* confirmFieldLabel;
    QLineEdit* usernameEdit;
    QLineEdit* emailEdit;
    QLineEdit* passwordEdit;
    QLineEdit* confirmPasswordEdit;
    QCheckBox* rememberMeCheck;
    QPushButton* primaryButton;
    QPushButton* secondaryButton;
    QPushButton* forgotButton;
    QLabel* titleLabel;
    QLabel* subtitleLabel;
    QLabel* statusLabel;
    QLabel* hintLabel;
    Authentication* auth;
    bool signupMode;
    bool darkMode;

    void setupUI();
    void setupConnections();
    void updateMode();
    void loadRememberedState();
    void showStatus(const QString& message, bool error = false);

public:
    explicit LoginWindow(Authentication* auth, QWidget* parent = nullptr);
    QString getLoggedInUser() const;

signals:
    void loginSuccess(const QString& username);

private slots:
    void onPrimaryClicked();
    void onSecondaryClicked();
    void onForgotClicked();
};
