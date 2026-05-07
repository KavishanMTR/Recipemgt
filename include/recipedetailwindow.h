#pragma once

#include "recipe.h"

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class RecipeDetailWindow : public QDialog {
    Q_OBJECT

private:
    Recipe recipe;
    bool darkMode;
    QLabel* imageLabel;
    QLabel* titleLabel;
    QLabel* subtitleLabel;
    QLabel* ingredientsLabel;
    QLabel* instructionsLabel;
    QPushButton* favouriteButton;
    QPushButton* editButton;

    void setupUI();
    void updateFavouriteButton();

public:
    explicit RecipeDetailWindow(const Recipe& recipe, bool darkMode, QWidget* parent = nullptr);
    void updateRecipe(const Recipe& recipe);

signals:
    void favouriteToggled(int recipeId);
    void editRequested(int recipeId);
};
