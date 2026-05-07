#pragma once

#include "recipe.h"

#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>

class AddRecipeDialog : public QDialog {
    Q_OBJECT

private:
    bool darkMode;
    bool editMode;
    Recipe editingRecipe;
    QString selectedImagePath;

    QLineEdit* nameEdit;
    QComboBox* categoryCombo;
    QLineEdit* descriptionEdit;
    QSpinBox* prepTimeSpin;
    QSpinBox* cookTimeSpin;
    QTextEdit* ingredientsEdit;
    QTextEdit* instructionsEdit;
    QLineEdit* imagePathEdit;
    QLabel* imagePreviewLabel;
    QLabel* validationLabel;
    QPushButton* browseButton;
    QPushButton* saveButton;
    QPushButton* cancelButton;

    void setupUI();
    void setupConnections();
    void populateFields();
    void updatePreview();
    bool validateForm();

public:
    explicit AddRecipeDialog(bool darkMode, QWidget* parent = nullptr);
    AddRecipeDialog(const Recipe& recipe, bool darkMode, QWidget* parent = nullptr);

    Recipe getRecipe() const;

private slots:
    void onBrowseImage();
};
