#pragma once

#include <QString>

namespace Styles {

inline QString accent(bool darkMode) {
    return darkMode ? "#F59E0B" : "#2F855A";
}

inline QString accentSoft(bool darkMode) {
    return darkMode ? "rgba(245, 158, 11, 0.18)" : "rgba(47, 133, 90, 0.14)";
}

inline QString windowBackground(bool darkMode) {
    return darkMode ? "#10151D" : "#EEF2F6";
}

inline QString cardBackground(bool darkMode) {
    return darkMode ? "#18212B" : "#FFFFFF";
}

inline QString elevatedCard(bool darkMode) {
    return darkMode ? "#111923" : "#F7FAFC";
}

inline QString border(bool darkMode) {
    return darkMode ? "rgba(255,255,255,0.08)" : "#D9E2EC";
}

inline QString textPrimary(bool darkMode) {
    return darkMode ? "#F7FAFC" : "#17212B";
}

inline QString textMuted(bool darkMode) {
    return darkMode ? "#9AA5B1" : "#617182";
}

inline QString inputBackground(bool darkMode) {
    return darkMode ? "#0F1720" : "#F8FAFC";
}

inline QString splashScreen() {
    return R"(
        QWidget#SplashScreen {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #0F1720, stop:0.5 #18212B, stop:1 #2C3E50);
            border-radius: 26px;
        }
        QLabel#AppTitle {
            color: #F7FAFC;
            font-size: 40px;
            font-weight: 700;
            letter-spacing: 3px;
        }
        QLabel#AppSubtitle {
            color: #C3CDD7;
            font-size: 14px;
            letter-spacing: 5px;
        }
        QProgressBar {
            background: rgba(255,255,255,0.08);
            border: none;
            border-radius: 6px;
            height: 10px;
        }
        QProgressBar::chunk {
            border-radius: 6px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #F59E0B, stop:1 #F97316);
        }
    )";
}

inline QString splash() {
    return splashScreen();
}

inline QString loginWindow(bool darkMode) {
    return QString(R"(
        QDialog#LoginWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 %1, stop:0.55 %2, stop:1 %3);
        }
        QFrame#LoginShell {
            background: rgba(255,255,255,0.04);
            border: 1px solid rgba(255,255,255,0.08);
            border-radius: 30px;
        }
        QWidget#IllustrationPanel {
            background: transparent;
        }
        QFrame#LoginCard {
            background: %4;
            border: 1px solid %5;
            border-radius: 26px;
        }
        QLabel#LoginTitle {
            color: %6;
            font-size: 28px;
            font-weight: 700;
        }
        QLabel#LoginSubtitle {
            color: %7;
            font-size: 12px;
            line-height: 1.5;
        }
        QLabel#FieldLabel {
            color: %7;
            font-size: 11px;
            font-weight: 700;
            letter-spacing: 1.4px;
        }
        QLineEdit {
            background: %8;
            border: 1px solid %5;
            border-radius: 10px;
            padding: 10px 12px;
            color: %6;
            font-size: 13px;
        }
        QLineEdit:focus {
            border: 1px solid %9;
        }
        QPushButton#PrimaryButton {
            background: %9;
            color: white;
            border: none;
            border-radius: 10px;
            padding: 12px 16px;
            font-size: 13px;
            font-weight: 700;
        }
        QPushButton#PrimaryButton:hover {
            background: %10;
        }
        QPushButton#SecondaryButton {
            background: %4;
            color: %9;
            border: 1px solid %9;
            border-radius: 10px;
            padding: 11px 16px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#SecondaryButton:hover {
            background: %11;
        }
        QPushButton#LinkButton {
            background: transparent;
            border: none;
            color: %9;
            font-size: 12px;
            font-weight: 600;
            text-align: left;
        }
        QPushButton#LinkButton:hover {
            color: %10;
        }
        QPushButton#GhostButton {
            background: transparent;
            border: 1px solid %5;
            color: %7;
            border-radius: 12px;
            padding: 10px 12px;
        }
        QPushButton#GhostButton:hover {
            border: 1px solid %9;
            color: %6;
        }
        QCheckBox {
            color: %7;
            font-size: 12px;
        }
        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border-radius: 5px;
            border: 1px solid %5;
            background: %8;
        }
        QCheckBox::indicator:checked {
            background: %9;
            border-color: %9;
        }
        QLabel#StatusLabel {
            color: %9;
            background: %11;
            border-radius: 10px;
            padding: 8px 10px;
            font-size: 12px;
        }
        QLabel#HintLabel {
            color: %7;
            font-size: 12px;
        }
    )")
        .arg(darkMode ? "#0E1726" : "#F2F6FA")
        .arg(darkMode ? "#16202C" : "#E8EFF6")
        .arg(darkMode ? "#223144" : "#DDE8F2")
        .arg(cardBackground(darkMode))
        .arg(border(darkMode))
        .arg(textPrimary(darkMode))
        .arg(textMuted(darkMode))
        .arg(inputBackground(darkMode))
        .arg(accent(darkMode))
        .arg(darkMode ? "#F97316" : "#276749")
        .arg(accentSoft(darkMode));
}

inline QString login() {
    return loginWindow(false);
}

inline QString mainWindow(bool darkMode) {
    return QString(R"(
        QMainWindow#MainWindow {
            background: %1;
        }
        QWidget#Sidebar {
            background: %2;
            border-right: 1px solid %3;
        }
        QLabel#BrandTitle {
            color: %4;
            font-size: 24px;
            font-weight: 700;
            letter-spacing: 1.5px;
        }
        QLabel#BrandSubtitle {
            color: %5;
            font-size: 11px;
            letter-spacing: 2px;
        }
        QPushButton#SidebarButton,
        QPushButton#SidebarButtonActive {
            text-align: left;
            border-radius: 14px;
            padding: 12px 16px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#SidebarButton {
            border: 1px solid transparent;
            color: %5;
            background: transparent;
        }
        QPushButton#SidebarButton:hover {
            color: %6;
            background: %7;
        }
        QPushButton#SidebarButtonActive {
            color: %6;
            background: %7;
            border: 1px solid %3;
        }
        QPushButton#AccentButton {
            background: %6;
            color: white;
            border: none;
            border-radius: 14px;
            padding: 12px 16px;
            font-size: 13px;
            font-weight: 700;
        }
        QPushButton#AccentButton:hover {
            background: %8;
        }
        QPushButton#GhostButton {
            background: transparent;
            color: %5;
            border: 1px solid %3;
            border-radius: 14px;
            padding: 10px 14px;
            font-size: 12px;
            font-weight: 600;
        }
        QPushButton#GhostButton:hover {
            border-color: %6;
            color: %4;
        }
        QWidget#PageSurface {
            background: transparent;
        }
        QFrame#HeroCard,
        QFrame#SectionCard,
        QFrame#StatCard {
            background: %9;
            border: 1px solid %3;
            border-radius: 20px;
        }
        QLabel#PageTitle {
            color: %4;
            font-size: 26px;
            font-weight: 700;
        }
        QLabel#PageSubtitle,
        QLabel#MutedText {
            color: %5;
            font-size: 13px;
        }
        QLabel#HeroEyebrow {
            color: %6;
            font-size: 12px;
            font-weight: 700;
            letter-spacing: 1.4px;
        }
        QLabel#StatValue {
            color: %4;
            font-size: 22px;
            font-weight: 700;
        }
        QLabel#StatLabel,
        QLabel#SectionTitle {
            color: %4;
            font-size: 14px;
            font-weight: 600;
        }
        QLabel#StatNote {
            color: %5;
            font-size: 12px;
        }
        QLineEdit#SearchField {
            background: %10;
            border: 1px solid %3;
            border-radius: 12px;
            padding: 10px 12px;
            color: %4;
            font-size: 13px;
        }
        QLineEdit#SearchField:focus,
        QComboBox#FilterCombo:focus {
            border: 1px solid %6;
        }
        QComboBox#FilterCombo {
            background: %10;
            border: 1px solid %3;
            border-radius: 12px;
            padding: 8px 10px;
            color: %4;
            font-size: 13px;
        }
        QPushButton#CategoryChip,
        QPushButton#CategoryChipActive {
            border-radius: 18px;
            padding: 8px 14px;
            font-size: 12px;
            font-weight: 600;
        }
        QPushButton#CategoryChip {
            background: %10;
            color: %5;
            border: 1px solid %3;
        }
        QPushButton#CategoryChip:hover {
            color: %4;
            border-color: %6;
        }
        QPushButton#CategoryChipActive {
            background: %7;
            color: %6;
            border: 1px solid transparent;
        }
        QScrollArea {
            border: none;
            background: transparent;
        }
        QScrollBar:vertical {
            background: transparent;
            width: 8px;
        }
        QScrollBar::handle:vertical {
            background: %3;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical:hover {
            background: %6;
        }
        QStatusBar {
            background: %9;
            color: %5;
            border-top: 1px solid %3;
        }
        QMessageBox {
            background: %9;
        }
    )")
        .arg(windowBackground(darkMode))
        .arg(darkMode ? "#0D141C" : "#F8FBFD")
        .arg(border(darkMode))
        .arg(textPrimary(darkMode))
        .arg(textMuted(darkMode))
        .arg(accent(darkMode))
        .arg(accentSoft(darkMode))
        .arg(darkMode ? "#F97316" : "#276749")
        .arg(cardBackground(darkMode))
        .arg(inputBackground(darkMode));
}

inline QString recipeCard(bool darkMode) {
    return QString(R"(
        QWidget#RecipeCard {
            background: %1;
            border: 1px solid %2;
            border-radius: 18px;
        }
        QWidget#RecipeCard:hover {
            border: 1px solid %3;
        }
        QLabel#RecipeName {
            color: %4;
            font-size: 14px;
            font-weight: 700;
        }
        QLabel#RecipeMeta {
            color: %5;
            font-size: 11px;
        }
        QLabel#Badge {
            color: %3;
            background: %6;
            border-radius: 8px;
            padding: 3px 8px;
            font-size: 10px;
            font-weight: 700;
        }
        QPushButton#IconButton,
        QPushButton#TextButton,
        QPushButton#DangerButton {
            border-radius: 10px;
            font-size: 11px;
            font-weight: 600;
        }
        QPushButton#IconButton {
            background: %1;
            border: 1px solid %2;
            color: %5;
            padding: 6px;
        }
        QPushButton#IconButton:hover {
            border-color: %3;
            color: %4;
        }
        QPushButton#TextButton {
            background: %6;
            color: %3;
            border: none;
            padding: 8px 10px;
        }
        QPushButton#TextButton:hover {
            background: %3;
            color: white;
        }
        QPushButton#DangerButton {
            background: %1;
            border: 1px solid %2;
            color: #E53E3E;
            padding: 8px 10px;
        }
        QPushButton#DangerButton:hover {
            background: rgba(229, 62, 62, 0.12);
        }
    )")
        .arg(cardBackground(darkMode))
        .arg(border(darkMode))
        .arg(accent(darkMode))
        .arg(textPrimary(darkMode))
        .arg(textMuted(darkMode))
        .arg(accentSoft(darkMode));
}

inline QString dialog(bool darkMode) {
    return QString(R"(
        QDialog {
            background: %1;
        }
        QFrame#DialogCard {
            background: %2;
            border: 1px solid %3;
            border-radius: 26px;
        }
        QLabel#DialogTitle {
            color: %4;
            font-size: 24px;
            font-weight: 700;
        }
        QLabel#FieldLabel {
            color: %5;
            font-size: 11px;
            font-weight: 700;
            letter-spacing: 1.3px;
        }
        QLabel#MutedText,
        QLabel#PreviewHint {
            color: %5;
            font-size: 12px;
        }
        QLineEdit,
        QTextEdit,
        QComboBox,
        QSpinBox {
            background: %6;
            border: 1px solid %3;
            border-radius: 14px;
            padding: 10px 12px;
            color: %4;
            font-size: 13px;
        }
        QLineEdit:focus,
        QTextEdit:focus,
        QComboBox:focus,
        QSpinBox:focus {
            border: 1px solid %7;
        }
        QPushButton#PrimaryButton {
            background: %7;
            color: white;
            border: none;
            border-radius: 14px;
            padding: 12px 16px;
            font-size: 13px;
            font-weight: 700;
        }
        QPushButton#PrimaryButton:hover {
            background: %8;
        }
        QPushButton#SecondaryButton {
            background: transparent;
            color: %5;
            border: 1px solid %3;
            border-radius: 14px;
            padding: 12px 16px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#SecondaryButton:hover {
            color: %4;
            border-color: %7;
        }
        QPushButton#GhostButton {
            background: %9;
            color: %7;
            border: none;
            border-radius: 12px;
            padding: 10px 14px;
            font-size: 12px;
            font-weight: 700;
        }
        QPushButton#GhostButton:hover {
            background: %7;
            color: white;
        }
        QLabel#ValidationLabel {
            color: #E53E3E;
            font-size: 12px;
        }
    )")
        .arg(windowBackground(darkMode))
        .arg(cardBackground(darkMode))
        .arg(border(darkMode))
        .arg(textPrimary(darkMode))
        .arg(textMuted(darkMode))
        .arg(inputBackground(darkMode))
        .arg(accent(darkMode))
        .arg(darkMode ? "#F97316" : "#276749")
        .arg(accentSoft(darkMode));
}

inline QString recipeDetail(bool darkMode) {
    return QString(R"(
        QDialog#RecipeDetailWindow {
            background: %1;
        }
        QFrame#DetailCard {
            background: %2;
            border: 1px solid %3;
            border-radius: 28px;
        }
        QLabel#DetailTitle {
            color: %4;
            font-size: 30px;
            font-weight: 700;
        }
        QLabel#DetailSubtitle,
        QLabel#DetailBody {
            color: %5;
            font-size: 13px;
        }
        QLabel#SectionTitle {
            color: %4;
            font-size: 15px;
            font-weight: 700;
        }
        QLabel#PillLabel {
            color: %6;
            background: %7;
            border-radius: 12px;
            padding: 6px 10px;
            font-size: 11px;
            font-weight: 700;
        }
        QPushButton#PrimaryButton,
        QPushButton#SecondaryButton,
        QPushButton#GhostButton {
            border-radius: 14px;
            padding: 11px 16px;
            font-size: 13px;
            font-weight: 700;
        }
        QPushButton#PrimaryButton {
            background: %6;
            color: white;
            border: none;
        }
        QPushButton#PrimaryButton:hover {
            background: %8;
        }
        QPushButton#SecondaryButton {
            background: transparent;
            color: %5;
            border: 1px solid %3;
        }
        QPushButton#SecondaryButton:hover {
            color: %4;
            border-color: %6;
        }
        QPushButton#GhostButton {
            background: %7;
            color: %6;
            border: none;
        }
        QPushButton#GhostButton:hover {
            background: %6;
            color: white;
        }
    )")
        .arg(windowBackground(darkMode))
        .arg(cardBackground(darkMode))
        .arg(border(darkMode))
        .arg(textPrimary(darkMode))
        .arg(textMuted(darkMode))
        .arg(accent(darkMode))
        .arg(accentSoft(darkMode))
        .arg(darkMode ? "#F97316" : "#276749");
}

}
