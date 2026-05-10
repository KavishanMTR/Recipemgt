#include "loginwindow.h"

#include "styles.h"
#include "uihelpers.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QResizeEvent>
#include <QStringList>
#include <QVBoxLayout>

LoginWindow::LoginWindow(Authentication* auth, QWidget* parent)
    : QDialog(parent),
      auth(auth),
      signupMode(false),
      darkMode(false) {
    setObjectName("LoginWindow");
    setWindowTitle("Recipe Management System");
    resize(860, 540);
    setMinimumSize(760, 500);
    setModal(true);
    setStyleSheet(Styles::login());
    setupUI();
    setupConnections();
    loadRememberedState();
    updateMode();
}

void LoginWindow::setupUI() {
    QHBoxLayout* rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(28, 28, 28, 28);
    rootLayout->setSpacing(20);

    QFrame* leftPanel = new QFrame(this);
    leftPanel->setObjectName("LoginShell");
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(28, 28, 28, 28);
    leftLayout->setSpacing(14);

    QLabel* projectLabel = new QLabel("RECIPE MANAGEMENT SYSTEM", leftPanel);
    projectLabel->setObjectName("FieldLabel");

    QLabel* headingLabel = new QLabel("Organize recipes, ingredients, and cooking steps in one smart kitchen workspace.", leftPanel);
    headingLabel->setObjectName("LoginTitle");
    headingLabel->setWordWrap(true);

    QLabel* bodyLabel = new QLabel(
        "",
        leftPanel
    );
    bodyLabel->setObjectName("LoginSubtitle");
    bodyLabel->setWordWrap(true);

    heroImageLabel = new QLabel(leftPanel);
    heroImageLabel->setObjectName("LoginHeroImage");
    heroImageLabel->setFixedHeight(170);
    heroImageLabel->setMinimumWidth(360);
    heroImageLabel->setAlignment(Qt::AlignCenter);
    heroImageLabel->setText("Recipe Collection");

    QLabel* featuresLabel = new QLabel(
        "What you can do:\n"
        "- Save complete recipes with ingredients and method\n"
        "- Organize dishes by category such as breakfast, dinner, and dessert\n"
        "- Search meals quickly and keep favourite dishes in one place\n"
        "- Maintain a neat digital recipe collection for daily cooking",
        leftPanel
    );
    featuresLabel->setObjectName("LoginSubtitle");
    featuresLabel->setWordWrap(true);

    leftLayout->addWidget(projectLabel);
    leftLayout->addWidget(headingLabel);
    leftLayout->addWidget(bodyLabel);
    leftLayout->addWidget(heroImageLabel);
    leftLayout->addStretch();
    leftLayout->addWidget(featuresLabel);
    loadHeroImage();

    QFrame* card = new QFrame(this);
    card->setObjectName("LoginCard");
    card->setMaximumWidth(420);
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(26, 26, 26, 26);
    cardLayout->setSpacing(10);

    titleLabel = new QLabel(card);
    titleLabel->setObjectName("LoginTitle");

    subtitleLabel = new QLabel(card);
    subtitleLabel->setObjectName("LoginSubtitle");
    subtitleLabel->setWordWrap(true);

    QLabel* usernameLabel = new QLabel("USERNAME", card);
    usernameLabel->setObjectName("FieldLabel");
    usernameEdit = new QLineEdit(card);
    usernameEdit->setPlaceholderText("Enter username");
    usernameEdit->setFixedHeight(42);

    emailFieldLabel = new QLabel("EMAIL", card);
    emailFieldLabel->setObjectName("FieldLabel");
    emailEdit = new QLineEdit(card);
    emailEdit->setPlaceholderText("Enter email");
    emailEdit->setFixedHeight(42);

    QLabel* passwordLabel = new QLabel("PASSWORD", card);
    passwordLabel->setObjectName("FieldLabel");
    passwordEdit = new QLineEdit(card);
    passwordEdit->setPlaceholderText("Enter password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setFixedHeight(42);

    confirmFieldLabel = new QLabel("CONFIRM PASSWORD", card);
    confirmFieldLabel->setObjectName("FieldLabel");
    confirmPasswordEdit = new QLineEdit(card);
    confirmPasswordEdit->setPlaceholderText("Confirm password");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setFixedHeight(42);

    rememberMeCheck = new QCheckBox("Remember username", card);

    forgotButton = new QPushButton("Forgot password?", card);
    forgotButton->setObjectName("LinkButton");
    forgotButton->setCursor(Qt::PointingHandCursor);

    QHBoxLayout* helperLayout = new QHBoxLayout();
    helperLayout->addWidget(rememberMeCheck);
    helperLayout->addStretch();
    helperLayout->addWidget(forgotButton);

    statusLabel = new QLabel(card);
    statusLabel->setObjectName("StatusLabel");
    statusLabel->setWordWrap(true);
    statusLabel->hide();

    primaryButton = new QPushButton(card);
    primaryButton->setObjectName("PrimaryButton");
    primaryButton->setCursor(Qt::PointingHandCursor);
    primaryButton->setFixedHeight(44);

    secondaryButton = new QPushButton(card);
    secondaryButton->setObjectName("SecondaryButton");
    secondaryButton->setCursor(Qt::PointingHandCursor);
    secondaryButton->setFixedHeight(44);

    hintLabel = new QLabel("Tip: keep your most-used dishes in favourites for quick access.", card);
    hintLabel->setObjectName("HintLabel");
    hintLabel->setWordWrap(true);

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addWidget(usernameLabel);
    cardLayout->addWidget(usernameEdit);
    cardLayout->addWidget(emailFieldLabel);
    cardLayout->addWidget(emailEdit);
    cardLayout->addWidget(passwordLabel);
    cardLayout->addWidget(passwordEdit);
    cardLayout->addWidget(confirmFieldLabel);
    cardLayout->addWidget(confirmPasswordEdit);
    cardLayout->addLayout(helperLayout);
    cardLayout->addWidget(statusLabel);
    cardLayout->addWidget(primaryButton);
    cardLayout->addWidget(secondaryButton);
    cardLayout->addWidget(hintLabel);
    cardLayout->addStretch();

    rootLayout->addWidget(leftPanel, 1);
    rootLayout->addWidget(card);
}

void LoginWindow::setupConnections() {
    connect(primaryButton, &QPushButton::clicked, this, &LoginWindow::onPrimaryClicked);
    connect(secondaryButton, &QPushButton::clicked, this, &LoginWindow::onSecondaryClicked);
    connect(forgotButton, &QPushButton::clicked, this, &LoginWindow::onForgotClicked);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onPrimaryClicked);
    connect(confirmPasswordEdit, &QLineEdit::returnPressed, this, &LoginWindow::onPrimaryClicked);
}

void LoginWindow::updateMode() {
    titleLabel->setText(signupMode ? "Create Account" : "Login");
    subtitleLabel->setText(signupMode
        ? "Create an account to start saving your personal recipes."
        : "Sign in to access your recipe dashboard, saved dishes, and cooking records.");

    primaryButton->setText(signupMode ? "Create Account" : "Login");
    secondaryButton->setText(signupMode ? "Back to Login" : "Create New Account");

    emailFieldLabel->setVisible(signupMode);
    emailEdit->setVisible(signupMode);
    confirmFieldLabel->setVisible(signupMode);
    confirmPasswordEdit->setVisible(signupMode);
    rememberMeCheck->setVisible(!signupMode);
    forgotButton->setVisible(!signupMode);
    hintLabel->setText(signupMode
        ? "Password must be at least 6 characters."
        : "");
}

void LoginWindow::loadRememberedState() {
    const QString savedUsername = auth->getSavedUsername();
    if (!savedUsername.isEmpty()) {
        usernameEdit->setText(savedUsername);
        rememberMeCheck->setChecked(true);
    }
}

void LoginWindow::loadHeroImage() {
    const QString appDir = QCoreApplication::applicationDirPath();
    const QString appParentDir = QDir(appDir).absoluteFilePath("..");
    const QStringList baseDirs = {
        QDir::currentPath(),
        appDir,
        appParentDir
    };
    const QStringList assetFolders = {"assets", "assests"};
    const QStringList fileNames = {
        "login-hero.jpg",
        "login-hero.jpeg",
        "login-hero.png",
        "login-hero.webp"
    };

    heroImagePath.clear();
    for (const QString& baseDir : baseDirs) {
        for (const QString& folder : assetFolders) {
            for (const QString& fileName : fileNames) {
                QFileInfo info(QDir(baseDir).filePath(folder + "/" + fileName));
                if (info.exists() && info.isFile()) {
                    heroImagePath = info.absoluteFilePath();
                    break;
                }
            }

            if (!heroImagePath.isEmpty()) {
                break;
            }
        }

        if (!heroImagePath.isEmpty()) {
            break;
        }
    }

    if (heroImagePath.isEmpty()) {
        heroImageLabel->setPixmap(QPixmap());
        heroImageLabel->setText("Recipe Collection");
        return;
    }

    const QPixmap source(heroImagePath);
    if (source.isNull()) {
        heroImageLabel->setPixmap(QPixmap());
        heroImageLabel->setText("Recipe Collection");
        return;
    }

    heroImageLabel->setText(QString());
    const QSize imageSize(qMax(1, heroImageLabel->width()), heroImageLabel->height());
    heroImageLabel->setPixmap(UiHelpers::roundedPixmap(source, imageSize, 18));
}

void LoginWindow::showStatus(const QString& message, bool error) {
    statusLabel->setStyleSheet(QString(
        "color: %1; background: %2; border-radius: 10px; padding: 8px 10px; font-size: 12px;")
        .arg(error ? "#C53030" : "#2F855A")
        .arg(error ? "rgba(197,48,48,0.12)" : "rgba(47,133,90,0.12)"));
    statusLabel->setText(message);
    statusLabel->show();
}

void LoginWindow::onPrimaryClicked() {
    statusLabel->hide();

    const QString username = usernameEdit->text().trimmed();
    const QString password = passwordEdit->text();
    const QString email = emailEdit->text().trimmed();
    const QString confirm = confirmPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        showStatus("Username and password are required.", true);
        return;
    }

    if (signupMode) {
        if (!email.isEmpty() && !email.contains('@')) {
            showStatus("Please enter a valid email.", true);
            return;
        }
        if (password.length() < 6) {
            showStatus("Password must be at least 6 characters.", true);
            return;
        }
        if (password != confirm) {
            showStatus("Passwords do not match.", true);
            return;
        }
        if (!auth->signup(username, password, email)) {
            showStatus("Unable to create account. Username may already exist.", true);
            return;
        }

        showStatus("Account created successfully. Please log in.");
        signupMode = false;
        passwordEdit->clear();
        confirmPasswordEdit->clear();
        updateMode();
        return;
    }

    if (!auth->login(username, password, rememberMeCheck->isChecked())) {
        showStatus("Invalid username or password.", true);
        return;
    }

    emit loginSuccess(username);
    accept();
}

void LoginWindow::onSecondaryClicked() {
    signupMode = !signupMode;
    statusLabel->hide();
    passwordEdit->clear();
    confirmPasswordEdit->clear();
    updateMode();
}

void LoginWindow::onForgotClicked() {
    QMessageBox::information(
        this,
        "Password Help",
        "Use the default account for testing:\n\nUsername: admin\nPassword: admin123"
    );
}

QString LoginWindow::getLoggedInUser() const {
    return auth->getCurrentUser() ? auth->getCurrentUser()->getUsername() : QString();
}

void LoginWindow::resizeEvent(QResizeEvent* event) {
    QDialog::resizeEvent(event);
    if (heroImageLabel) {
        loadHeroImage();
    }
}
