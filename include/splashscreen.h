#pragma once

#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QWidget>

class SplashScreen : public QWidget {
    Q_OBJECT

private:
    QProgressBar* progressBar;
    QLabel* statusLabel;
    QTimer* timer;
    int progressValue;

    void setupUI();
    void setupAnimations();

public:
    explicit SplashScreen(QWidget* parent = nullptr);

signals:
    void loadingComplete();

private slots:
    void updateProgress();
};
