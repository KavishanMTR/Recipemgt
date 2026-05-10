#include "splashscreen.h"

#include "styles.h"

#include <QApplication>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>

SplashScreen::SplashScreen(QWidget* parent)
    : QWidget(parent),
      progressValue(0) {
    setObjectName("SplashScreen");
    setFixedSize(560, 360);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(Styles::splash());
    setupUI();
    setupAnimations();
}

void SplashScreen::setupUI() {
    QVBoxLayout* hostLayout = new QVBoxLayout(this);
    hostLayout->setContentsMargins(22, 22, 22, 22);

    QWidget* panel = new QWidget(this);
    panel->setObjectName("SplashScreen");

    auto* shadow = new QGraphicsDropShadowEffect(panel);
    shadow->setBlurRadius(34);
    shadow->setOffset(0, 18);
    shadow->setColor(QColor(0, 0, 0, 90));
    panel->setGraphicsEffect(shadow);

    QVBoxLayout* layout = new QVBoxLayout(panel);
    layout->setContentsMargins(54, 48, 54, 48);
    layout->setSpacing(16);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* title = new QLabel("RECIPE STUDIO", panel);
    title->setObjectName("AppTitle");
    title->setAlignment(Qt::AlignCenter);

    QLabel* subtitle = new QLabel("PROFESSIONAL MANAGEMENT SYSTEM", panel);
    subtitle->setObjectName("AppSubtitle");
    subtitle->setAlignment(Qt::AlignCenter);

    statusLabel = new QLabel("Preparing your workspace...", panel);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: #C3CDD7; font-size: 13px;");

    progressBar = new QProgressBar(panel);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);
    progressBar->setFixedHeight(10);

    QLabel* footer = new QLabel("Recipe planning, favourites, search, and visual dashboards", panel);
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("color: rgba(255,255,255,0.65); font-size: 11px;");

    layout->addStretch();
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addSpacing(18);
    layout->addWidget(progressBar);
    layout->addWidget(statusLabel);
    layout->addStretch();
    layout->addWidget(footer);

    hostLayout->addWidget(panel);
}

void SplashScreen::setupAnimations() {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SplashScreen::updateProgress);
    timer->start(28);
}

void SplashScreen::updateProgress() {
    progressValue = qMin(100, progressValue + 2);
    progressBar->setValue(progressValue);

    if (progressValue < 28) {
        statusLabel->setText("Loading interface components...");
    } else if (progressValue < 56) {
        statusLabel->setText("Connecting recipe storage...");
    } else if (progressValue < 82) {
        statusLabel->setText("Preparing dashboard experience...");
    } else {
        statusLabel->setText("Launching Recipe Studio...");
    }

    if (progressValue >= 100) {
        timer->stop();
        emit loadingComplete();
    }
}
