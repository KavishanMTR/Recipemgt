#pragma once

#include "authentication.h"
#include "recipemanager.h"

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QStackedWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(RecipeManager* manager, Authentication* auth,
                        const QString& username, QWidget* parent = nullptr);

private:
    RecipeManager* recipeManager;
    Authentication* auth;
    QString currentUser;
    const bool darkMode;

    QWidget* dashboardPage;
    QWidget* libraryPage;
    QWidget* favouritesPage;
    QStackedWidget* contentStack;

    QPushButton* dashboardNavButton;
    QPushButton* libraryNavButton;
    QPushButton* favouritesNavButton;
    QPushButton* addRecipeButton;
    QPushButton* logoutButton;

    QLabel* totalRecipesValue;
    QLabel* favouriteRecipesValue;
    QLabel* categoryCountValue;
    QLabel* libraryCountLabel;

    QLineEdit* librarySearchField;
    QComboBox* categoryFilter;

    QGridLayout* dashboardRecentGrid;
    QGridLayout* libraryGrid;
    QGridLayout* favouritesGrid;
    int lastGridColumnCount;

    void setupUI();
    QWidget* createSidebar();
    QWidget* createDashboardPage();
    QWidget* createLibraryPage();
    QWidget* createFavouritesPage();
    QWidget* createPageHeader(const QString& title, const QString& subtitle);
    QWidget* createGridSection(const QString& title, const QString& subtitle, QGridLayout** layout);
    QWidget* createEmptyState(const QString& title, const QString& subtitle) const;
    void clearGrid(QGridLayout* layout);
    void refreshDashboard();
    void refreshLibrary();
    void refreshFavourites();
    void refreshStats();
    void refreshAll();
    void setActiveNav(QPushButton* activeButton);
    void switchPage(int index, QPushButton* activeButton);
    int gridColumnCount(QGridLayout* layout) const;
    void populateGrid(QGridLayout* layout, const QVector<Recipe>& recipes, const QString& emptyTitle, const QString& emptySubtitle);
    QVector<Recipe> currentLibraryRecipes() const;
    void openRecipeDialog(int recipeId = 0);
    void openRecipeDetail(int recipeId);
    void seedSampleRecipes();

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onLibrarySearch();
    void onCategoryFilterChanged(const QString& text);
    void onViewRecipe(int recipeId);
    void onEditRecipe(int recipeId);
    void onDeleteRecipe(int recipeId);
    void onFavouriteToggled(int recipeId);
    void onLogout();

signals:
    void logoutRequested();
};
