#pragma once

#include "recipe.h"

#include <QLabel>
#include <QPushButton>
#include <QWidget>

class RecipeCard : public QWidget {
    Q_OBJECT

private:
    Recipe recipe;
    bool darkMode;
    QLabel* imageLabel;
    QLabel* nameLabel;
    QLabel* badgeLabel;
    QLabel* timeLabel;
    QLabel* authorLabel;
    QPushButton* favouriteButton;
    QPushButton* viewButton;
    QPushButton* editButton;
    QPushButton* deleteButton;

    void setupUI();
    void applyTheme();
    void updateFavouriteButton();

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;

public:
    explicit RecipeCard(const Recipe& recipe, bool darkMode, QWidget* parent = nullptr);

    void updateRecipe(const Recipe& recipe);

signals:
    void viewRequested(int recipeId);
    void editRequested(int recipeId);
    void deleteRequested(int recipeId);
    void favouriteToggled(int recipeId);

private slots:
    void onViewClicked();
    void onEditClicked();
    void onDeleteClicked();
    void onFavouriteClicked();
};
