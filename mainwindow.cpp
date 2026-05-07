#include "mainwindow.h"

#include "addrecipedialog.h"
#include "recipedetailwindow.h"
#include "recipecard.h"
#include "styles.h"

#include <QDateTime>
#include <QFrame>
#include <QMessageBox>
#include <QScrollArea>
#include <QSizePolicy>
#include <QStatusBar>
#include <QVBoxLayout>

namespace {

QFrame* createCard(QWidget* parent = nullptr) {
    QFrame* frame = new QFrame(parent);
    frame->setObjectName("SectionCard");
    return frame;
}

}

MainWindow::MainWindow(RecipeManager* manager, Authentication* auth, const QString& username, QWidget* parent)
    : QMainWindow(parent),
      recipeManager(manager),
      auth(auth),
      currentUser(username),
      darkMode(false) {
    setObjectName("MainWindow");
    setWindowTitle(QString("Recipe Management System - %1").arg(username));
    resize(1260, 820);
    setMinimumSize(1100, 760);
    setupUI();
    seedSampleRecipes();
    refreshAll();
    switchPage(0, dashboardNavButton);
}

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* rootLayout = new QHBoxLayout(central);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    rootLayout->addWidget(createSidebar());

    contentStack = new QStackedWidget(this);
    dashboardPage = createDashboardPage();
    libraryPage = createLibraryPage();
    favouritesPage = createFavouritesPage();

    contentStack->addWidget(dashboardPage);
    contentStack->addWidget(libraryPage);
    contentStack->addWidget(favouritesPage);
    rootLayout->addWidget(contentStack, 1);

    setStyleSheet(Styles::mainWindow(darkMode));
    statusBar()->showMessage("Ready");
}

QWidget* MainWindow::createSidebar() {
    QWidget* sidebar = new QWidget(this);
    sidebar->setObjectName("Sidebar");
    sidebar->setFixedWidth(240);

    QVBoxLayout* layout = new QVBoxLayout(sidebar);
    layout->setContentsMargins(18, 24, 18, 24);
    layout->setSpacing(10);

    QLabel* title = new QLabel("Recipe Manager", sidebar);
    title->setObjectName("BrandTitle");

    QLabel* subtitle = new QLabel("Simple and Professional", sidebar);
    subtitle->setObjectName("BrandSubtitle");

    dashboardNavButton = new QPushButton("Dashboard", sidebar);
    libraryNavButton = new QPushButton("Recipes", sidebar);
    favouritesNavButton = new QPushButton("Favourites", sidebar);
    addRecipeButton = new QPushButton("Add Recipe", sidebar);
    logoutButton = new QPushButton("Logout", sidebar);

    for (QPushButton* button : {dashboardNavButton, libraryNavButton, favouritesNavButton}) {
        button->setObjectName("SidebarButton");
        button->setMinimumHeight(44);
        button->setCursor(Qt::PointingHandCursor);
    }

    addRecipeButton->setObjectName("AccentButton");
    addRecipeButton->setMinimumHeight(46);
    addRecipeButton->setCursor(Qt::PointingHandCursor);

    logoutButton->setObjectName("GhostButton");
    logoutButton->setMinimumHeight(42);
    logoutButton->setCursor(Qt::PointingHandCursor);

    QLabel* userLabel = new QLabel(QString("User: %1").arg(currentUser), sidebar);
    userLabel->setObjectName("MutedText");

    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addSpacing(20);
    layout->addWidget(dashboardNavButton);
    layout->addWidget(libraryNavButton);
    layout->addWidget(favouritesNavButton);
    layout->addSpacing(8);
    layout->addWidget(addRecipeButton);
    layout->addStretch();
    layout->addWidget(userLabel);
    layout->addWidget(logoutButton);

    connect(dashboardNavButton, &QPushButton::clicked, [this]() { switchPage(0, dashboardNavButton); });
    connect(libraryNavButton, &QPushButton::clicked, [this]() { switchPage(1, libraryNavButton); });
    connect(favouritesNavButton, &QPushButton::clicked, [this]() { switchPage(2, favouritesNavButton); });
    connect(addRecipeButton, &QPushButton::clicked, [this]() { openRecipeDialog(); });
    connect(logoutButton, &QPushButton::clicked, this, &MainWindow::onLogout);

    return sidebar;
}

QWidget* MainWindow::createDashboardPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* rootLayout = new QVBoxLayout(page);
    rootLayout->setContentsMargins(22, 22, 22, 22);
    rootLayout->setSpacing(16);

    rootLayout->addWidget(createPageHeader(
        "Dashboard",
        QString("Welcome, %1. Manage your recipe collection from one clean workspace.").arg(currentUser)
    ));

    QHBoxLayout* statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(14);

    auto createStat = [&](const QString& title, QLabel** valueLabel) {
        QFrame* card = createCard(page);
        card->setObjectName("StatCard");
        card->setFixedHeight(120);
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        QVBoxLayout* layout = new QVBoxLayout(card);
        layout->setContentsMargins(18, 16, 18, 16);
        layout->setSpacing(6);
        QLabel* titleLabel = new QLabel(title, card);
        titleLabel->setObjectName("SectionTitle");
        QLabel* value = new QLabel("0", card);
        value->setObjectName("StatValue");
        QLabel* noteLabel = new QLabel(card);
        noteLabel->setObjectName("StatNote");
        if (title == "Total Recipes") noteLabel->setText("Recipes in your collection");
        if (title == "Favourites") noteLabel->setText("Saved for quick access");
        if (title == "Categories") noteLabel->setText("Available recipe groups");
        layout->addWidget(titleLabel);
        layout->addWidget(value);
        layout->addWidget(noteLabel);
        *valueLabel = value;
        return card;
    };

    statsLayout->addWidget(createStat("Total Recipes", &totalRecipesValue), 1);
    statsLayout->addWidget(createStat("Favourites", &favouriteRecipesValue), 1);
    statsLayout->addWidget(createStat("Categories", &categoryCountValue), 1);
    rootLayout->addLayout(statsLayout);

    rootLayout->addWidget(createGridSection(
        "Recently Added",
        "Your latest recipes are shown here for quick access.",
        &dashboardRecentGrid
    ));

    return page;
}

QWidget* MainWindow::createLibraryPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* rootLayout = new QVBoxLayout(page);
    rootLayout->setContentsMargins(22, 22, 22, 22);
    rootLayout->setSpacing(16);

    rootLayout->addWidget(createPageHeader(
        "Recipe Library",
        "Search, filter, edit, and organize all recipes."
    ));

    QFrame* toolbar = createCard(page);
    QHBoxLayout* toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(18, 18, 18, 18);
    toolbarLayout->setSpacing(12);

    librarySearchField = new QLineEdit(toolbar);
    librarySearchField->setObjectName("SearchField");
    librarySearchField->setPlaceholderText("Search by recipe name, ingredient, or category...");

    categoryFilter = new QComboBox(toolbar);
    categoryFilter->setObjectName("FilterCombo");
    categoryFilter->setMinimumWidth(180);

    libraryCountLabel = new QLabel("0 recipes", toolbar);
    libraryCountLabel->setObjectName("MutedText");

    toolbarLayout->addWidget(librarySearchField, 1);
    toolbarLayout->addWidget(categoryFilter);
    toolbarLayout->addWidget(libraryCountLabel);

    rootLayout->addWidget(toolbar);
    rootLayout->addWidget(createGridSection(
        "All Recipes",
        "Use the buttons on each card to view, edit, favourite, or delete.",
        &libraryGrid
    ), 1);

    connect(librarySearchField, &QLineEdit::textChanged, this, &MainWindow::onLibrarySearch);
    connect(categoryFilter, &QComboBox::currentTextChanged, this, &MainWindow::onCategoryFilterChanged);

    return page;
}

QWidget* MainWindow::createFavouritesPage() {
    QWidget* page = new QWidget(this);
    QVBoxLayout* rootLayout = new QVBoxLayout(page);
    rootLayout->setContentsMargins(22, 22, 22, 22);
    rootLayout->setSpacing(16);

    rootLayout->addWidget(createPageHeader(
        "Favourite Recipes",
        "Recipes marked as favourite are listed here."
    ));

    rootLayout->addWidget(createGridSection(
        "Saved Favourites",
        "Quick access to the recipes you like most.",
        &favouritesGrid
    ), 1);

    return page;
}

QWidget* MainWindow::createPageHeader(const QString& title, const QString& subtitle) {
    QFrame* header = createCard(this);
    QVBoxLayout* layout = new QVBoxLayout(header);
    layout->setContentsMargins(20, 18, 20, 18);

    QLabel* titleLabel = new QLabel(title, header);
    titleLabel->setObjectName("PageTitle");

    QLabel* subtitleLabel = new QLabel(subtitle, header);
    subtitleLabel->setObjectName("PageSubtitle");
    subtitleLabel->setWordWrap(true);

    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    return header;
}

QWidget* MainWindow::createGridSection(const QString& title, const QString& subtitle, QGridLayout** layout) {
    QFrame* section = createCard(this);
    QVBoxLayout* sectionLayout = new QVBoxLayout(section);
    sectionLayout->setContentsMargins(20, 20, 20, 20);
    sectionLayout->setSpacing(12);

    QLabel* titleLabel = new QLabel(title, section);
    titleLabel->setObjectName("SectionTitle");

    QLabel* subtitleLabel = new QLabel(subtitle, section);
    subtitleLabel->setObjectName("MutedText");
    subtitleLabel->setWordWrap(true);

    QScrollArea* scrollArea = new QScrollArea(section);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QWidget* content = new QWidget(scrollArea);
    content->setStyleSheet("background: transparent;");
    *layout = new QGridLayout(content);
    (*layout)->setContentsMargins(0, 0, 0, 0);
    (*layout)->setHorizontalSpacing(14);
    (*layout)->setVerticalSpacing(14);
    (*layout)->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    scrollArea->setWidget(content);

    sectionLayout->addWidget(titleLabel);
    sectionLayout->addWidget(subtitleLabel);
    sectionLayout->addWidget(scrollArea, 1);
    return section;
}

QWidget* MainWindow::createEmptyState(const QString& title, const QString& subtitle) const {
    QFrame* empty = new QFrame();
    empty->setObjectName("SectionCard");
    QVBoxLayout* layout = new QVBoxLayout(empty);
    layout->setContentsMargins(32, 32, 32, 32);
    layout->setSpacing(8);
    layout->setAlignment(Qt::AlignCenter);

    QLabel* titleLabel = new QLabel(title, empty);
    titleLabel->setObjectName("SectionTitle");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel* subtitleLabel = new QLabel(subtitle, empty);
    subtitleLabel->setObjectName("MutedText");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setWordWrap(true);

    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    return empty;
}

void MainWindow::clearGrid(QGridLayout* layout) {
    while (layout && layout->count() > 0) {
        QLayoutItem* item = layout->takeAt(0);
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

void MainWindow::refreshDashboard() {
    populateGrid(
        dashboardRecentGrid,
        recipeManager->getRecentRecipes(6),
        "No recipes yet",
        "Add your first recipe to see it on the dashboard."
    );
}

void MainWindow::refreshLibrary() {
    const QVector<Recipe> recipes = currentLibraryRecipes();
    libraryCountLabel->setText(QString("%1 recipes").arg(recipes.size()));

    populateGrid(
        libraryGrid,
        recipes,
        "No recipes found",
        "Try another search term or change the category filter."
    );
}

void MainWindow::refreshFavourites() {
    populateGrid(
        favouritesGrid,
        recipeManager->getFavourites(),
        "No favourites yet",
        "Mark recipes as favourite to show them here."
    );
}

void MainWindow::refreshStats() {
    totalRecipesValue->setText(QString::number(recipeManager->getTotalCount()));
    favouriteRecipesValue->setText(QString::number(recipeManager->getFavouriteCount()));
    categoryCountValue->setText(QString::number(recipeManager->getCategories().size()));
}

void MainWindow::refreshAll() {
    categoryFilter->blockSignals(true);
    categoryFilter->clear();
    categoryFilter->addItem("All Categories");
    categoryFilter->addItems(recipeManager->getCategories());
    categoryFilter->blockSignals(false);

    refreshStats();
    refreshDashboard();
    refreshLibrary();
    refreshFavourites();
}

void MainWindow::setActiveNav(QPushButton* activeButton) {
    for (QPushButton* button : {dashboardNavButton, libraryNavButton, favouritesNavButton}) {
        button->setObjectName(button == activeButton ? "SidebarButtonActive" : "SidebarButton");
        style()->unpolish(button);
        style()->polish(button);
        button->update();
    }
}

void MainWindow::switchPage(int index, QPushButton* activeButton) {
    contentStack->setCurrentIndex(index);
    setActiveNav(activeButton);
}

void MainWindow::populateGrid(QGridLayout* layout, const QVector<Recipe>& recipes, const QString& emptyTitle, const QString& emptySubtitle) {
    clearGrid(layout);

    if (recipes.isEmpty()) {
        layout->addWidget(createEmptyState(emptyTitle, emptySubtitle), 0, 0);
        return;
    }

    int row = 0;
    int column = 0;
    for (const Recipe& recipe : recipes) {
        RecipeCard* card = new RecipeCard(recipe, darkMode);
        connect(card, &RecipeCard::viewRequested, this, &MainWindow::onViewRecipe);
        connect(card, &RecipeCard::editRequested, this, &MainWindow::onEditRecipe);
        connect(card, &RecipeCard::deleteRequested, this, &MainWindow::onDeleteRecipe);
        connect(card, &RecipeCard::favouriteToggled, this, &MainWindow::onFavouriteToggled);
        layout->addWidget(card, row, column);

        ++column;
        if (column == 3) {
            column = 0;
            ++row;
        }
    }
}

QVector<Recipe> MainWindow::currentLibraryRecipes() const {
    const QString category = categoryFilter->currentText() == "All Categories"
        ? QString()
        : categoryFilter->currentText();
    return recipeManager->filterRecipes(librarySearchField->text(), category, false);
}

void MainWindow::openRecipeDialog(int recipeId) {
    if (recipeId > 0) {
        Recipe* existingRecipe = recipeManager->findById(recipeId);
        if (!existingRecipe) {
            return;
        }

        AddRecipeDialog dialog(*existingRecipe, darkMode, this);
        if (dialog.exec() == QDialog::Accepted) {
            Recipe updatedRecipe = dialog.getRecipe();
            updatedRecipe.setAddedBy(existingRecipe->getAddedBy());
            updatedRecipe.setDateAdded(existingRecipe->getDateAdded());
            recipeManager->updateRecipe(updatedRecipe);
            refreshAll();
            statusBar()->showMessage("Recipe updated", 2500);
        }
        return;
    }

    AddRecipeDialog dialog(darkMode, this);
    if (dialog.exec() == QDialog::Accepted) {
        Recipe recipe = dialog.getRecipe();
        recipe.setAddedBy(currentUser);
        recipe.setDateAdded(QDateTime::currentDateTime());
        recipeManager->addRecipe(recipe);
        refreshAll();
        statusBar()->showMessage("Recipe added", 2500);
    }
}

void MainWindow::openRecipeDetail(int recipeId) {
    Recipe* recipe = recipeManager->findById(recipeId);
    if (!recipe) {
        return;
    }

    bool editAfterClose = false;
    RecipeDetailWindow detail(*recipe, darkMode, this);
    connect(&detail, &RecipeDetailWindow::favouriteToggled, this, &MainWindow::onFavouriteToggled);
    connect(&detail, &RecipeDetailWindow::editRequested, this, [&]() {
        editAfterClose = true;
    });
    detail.exec();

    if (editAfterClose) {
        openRecipeDialog(recipeId);
    }
}

void MainWindow::seedSampleRecipes() {
    if (recipeManager->getTotalCount() > 0) {
        return;
    }

    QVector<Recipe> samples = {
        Recipe(0, "Chicken Fried Rice", "Dinner",
               "Simple fried rice with chicken and vegetables.",
               {"2 cups cooked rice", "200g chicken", "1 carrot", "2 eggs", "2 tbsp soy sauce", "Spring onion"},
               "1. Cook the chicken.\n2. Scramble the eggs.\n3. Stir fry vegetables.\n4. Add rice, chicken, eggs, and soy sauce.\n5. Mix well and serve.",
               15, 15, currentUser),

        Recipe(0, "Fruit Salad", "Breakfast",
               "Fresh fruit salad for a healthy breakfast.",
               {"Apple", "Banana", "Papaya", "Orange", "Honey"},
               "1. Wash and cut the fruits.\n2. Add honey.\n3. Toss lightly and serve chilled.",
               10, 0, currentUser),

        Recipe(0, "Chocolate Cake", "Dessert",
               "Soft and rich homemade chocolate cake.",
               {"2 cups flour", "1 cup sugar", "1/2 cup cocoa", "2 eggs", "1 cup milk", "Butter"},
               "1. Mix dry ingredients.\n2. Add eggs and milk.\n3. Pour into a tray.\n4. Bake until done and cool before serving.",
               20, 35, currentUser)
    };

    for (Recipe& recipe : samples) {
        recipeManager->addRecipe(recipe);
    }

    recipeManager->markFavourite(1, true);
}

void MainWindow::onLibrarySearch() {
    refreshLibrary();
}

void MainWindow::onCategoryFilterChanged(const QString&) {
    refreshLibrary();
}

void MainWindow::onViewRecipe(int recipeId) {
    openRecipeDetail(recipeId);
}

void MainWindow::onEditRecipe(int recipeId) {
    openRecipeDialog(recipeId);
}

void MainWindow::onDeleteRecipe(int recipeId) {
    Recipe* recipe = recipeManager->findById(recipeId);
    if (!recipe) {
        return;
    }

    const QMessageBox::StandardButton choice = QMessageBox::question(
        this,
        "Delete Recipe",
        QString("Delete \"%1\"?").arg(recipe->getName()),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (choice == QMessageBox::Yes) {
        recipeManager->deleteRecipe(recipeId);
        refreshAll();
        statusBar()->showMessage("Recipe deleted", 2500);
    }
}

void MainWindow::onFavouriteToggled(int recipeId) {
    recipeManager->toggleFavourite(recipeId);
    refreshAll();
    statusBar()->showMessage("Favourite updated", 2000);
}

void MainWindow::onLogout() {
    auth->logout();
    emit logoutRequested();
    close();
}
