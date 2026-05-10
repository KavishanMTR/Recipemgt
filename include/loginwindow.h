#pragma once

#include "authentication.h"

#include <QCheckBox>
#include <QDialog>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QResizeEvent>
#include <QString>

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
    QLabel* heroImageLabel;
    Authentication* auth;
    bool signupMode;
    bool darkMode;
    QString heroImagePath;

    void setupUI();
    void setupConnections();
    void updateMode();
    void loadRememberedState();
    void loadHeroImage();
    void showStatus(const QString& message, bool error = false);

public:
    explicit LoginWindow(Authentication* auth, QWidget* parent = nullptr);
    QString getLoggedInUser() const;

protected:
    void resizeEvent(QResizeEvent* event) override;

signals:
    void loginSuccess(const QString& username);

private slots:
    void onPrimaryClicked();
    void onSecondaryClicked();
    void onForgotClicked();
};
