#include "recipedetailwindow.h"

#include "styles.h"
#include "uihelpers.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QVBoxLayout>

RecipeDetailWindow::RecipeDetailWindow(const Recipe& recipe, bool darkMode, QWidget* parent)
    : QDialog(parent),
      recipe(recipe),
      darkMode(darkMode) {
    setObjectName("RecipeDetailWindow");
    setWindowTitle("Recipe Details");
    resize(860, 760);
    setMinimumSize(820, 720);
    setStyleSheet(Styles::recipeDetail(darkMode));
    setupUI();
}

void RecipeDetailWindow::setupUI() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(18, 18, 18, 18);

    QFrame* card = new QFrame(this);
    card->setObjectName("DetailCard");
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(22, 22, 22, 22);
    cardLayout->setSpacing(18);

    QHBoxLayout* topActions = new QHBoxLayout();
    QPushButton* backButton = new QPushButton("Back", card);
    backButton->setObjectName("SecondaryButton");
    connect(backButton, &QPushButton::clicked, this, &QDialog::accept);

    favouriteButton = new QPushButton(card);
    favouriteButton->setObjectName("GhostButton");
    connect(favouriteButton, &QPushButton::clicked, [this]() {
        emit favouriteToggled(recipe.getId());
        accept();
    });

    editButton = new QPushButton("Edit Recipe", card);
    editButton->setObjectName("PrimaryButton");
    connect(editButton, &QPushButton::clicked, [this]() {
        emit editRequested(recipe.getId());
        accept();
    });

    topActions->addWidget(backButton);
    topActions->addStretch();
    topActions->addWidget(favouriteButton);
    topActions->addWidget(editButton);

    imageLabel = new QLabel(card);
    imageLabel->setPixmap(UiHelpers::recipePixmap(recipe, QSize(780, 250), 24));
    imageLabel->setFixedHeight(250);
    imageLabel->setScaledContents(true);

    titleLabel = new QLabel(recipe.getName(), card);
    titleLabel->setObjectName("DetailTitle");
    titleLabel->setWordWrap(true);

    subtitleLabel = new QLabel(
        QString("%1  |  Prep %2 min  |  Cook %3 min  |  Total %4 min")
            .arg(recipe.getCategoryLabel())
            .arg(recipe.getPrepTime())
            .arg(recipe.getCookTime())
            .arg(recipe.getTotalTime()),
        card
    );
    subtitleLabel->setObjectName("DetailSubtitle");

    QHBoxLayout* chipRow = new QHBoxLayout();
    QLabel* categoryChip = new QLabel(recipe.getCategoryLabel(), card);
    categoryChip->setObjectName("PillLabel");

    QLabel* authorChip = new QLabel(
        recipe.getAddedBy().trimmed().isEmpty() ? "Recipe library" : QString("By %1").arg(recipe.getAddedBy()),
        card
    );
    authorChip->setObjectName("PillLabel");

    chipRow->addWidget(categoryChip);
    chipRow->addWidget(authorChip);
    chipRow->addStretch();

    QScrollArea* scrollArea = new QScrollArea(card);
    scrollArea->setWidgetResizable(true);

    QWidget* content = new QWidget(scrollArea);
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(18);

    if (!recipe.getDescription().trimmed().isEmpty()) {
        QLabel* descriptionLabel = new QLabel(recipe.getDescription(), content);
        descriptionLabel->setObjectName("DetailBody");
        descriptionLabel->setWordWrap(true);
        contentLayout->addWidget(descriptionLabel);
    }

    QFrame* ingredientsCard = new QFrame(content);
    ingredientsCard->setObjectName("DetailCard");
    QVBoxLayout* ingredientsLayout = new QVBoxLayout(ingredientsCard);
    ingredientsLayout->setContentsMargins(18, 18, 18, 18);
    ingredientsLayout->setSpacing(10);

    QLabel* ingredientsTitle = new QLabel("Ingredients", ingredientsCard);
    ingredientsTitle->setObjectName("SectionTitle");
    ingredientsLabel = new QLabel("- " + recipe.getIngredients().join("\n- "), ingredientsCard);
    ingredientsLabel->setObjectName("DetailBody");
    ingredientsLabel->setWordWrap(true);
    ingredientsLayout->addWidget(ingredientsTitle);
    ingredientsLayout->addWidget(ingredientsLabel);

    QFrame* instructionsCard = new QFrame(content);
    instructionsCard->setObjectName("DetailCard");
    QVBoxLayout* instructionsLayout = new QVBoxLayout(instructionsCard);
    instructionsLayout->setContentsMargins(18, 18, 18, 18);
    instructionsLayout->setSpacing(10);

    QLabel* instructionsTitle = new QLabel("Cooking Instructions", instructionsCard);
    instructionsTitle->setObjectName("SectionTitle");
    instructionsLabel = new QLabel(recipe.getInstructions(), instructionsCard);
    instructionsLabel->setObjectName("DetailBody");
    instructionsLabel->setWordWrap(true);
    instructionsLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    instructionsLayout->addWidget(instructionsTitle);
    instructionsLayout->addWidget(instructionsLabel);

    contentLayout->addWidget(ingredientsCard);
    contentLayout->addWidget(instructionsCard);
    contentLayout->addStretch();

    scrollArea->setWidget(content);

    cardLayout->addLayout(topActions);
    cardLayout->addWidget(imageLabel);
    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addLayout(chipRow);
    cardLayout->addWidget(scrollArea, 1);

    rootLayout->addWidget(card);
    updateFavouriteButton();
}

void RecipeDetailWindow::updateFavouriteButton() {
    favouriteButton->setText(recipe.getFavourite() ? "Remove Favourite" : "Add Favourite");
}

void RecipeDetailWindow::updateRecipe(const Recipe& value) {
    recipe = value;
    imageLabel->setPixmap(UiHelpers::recipePixmap(recipe, QSize(780, 250), 24));
    titleLabel->setText(recipe.getName());
    subtitleLabel->setText(
        QString("%1  |  Prep %2 min  |  Cook %3 min  |  Total %4 min")
            .arg(recipe.getCategoryLabel())
            .arg(recipe.getPrepTime())
            .arg(recipe.getCookTime())
            .arg(recipe.getTotalTime())
    );
    ingredientsLabel->setText("- " + recipe.getIngredients().join("\n- "));
    instructionsLabel->setText(recipe.getInstructions());
    updateFavouriteButton();
}
