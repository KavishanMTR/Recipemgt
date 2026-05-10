#include "recipedetailwindow.h"

#include "styles.h"
#include "uihelpers.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QSizePolicy>
#include <QVBoxLayout>

RecipeDetailWindow::RecipeDetailWindow(const Recipe& recipe, bool darkMode, QWidget* parent)
    : QDialog(parent),
      recipe(recipe),
      darkMode(darkMode) {
    setObjectName("RecipeDetailWindow");
    setWindowTitle("Recipe Details");
    resize(980, 760);
    setMinimumSize(860, 700);
    setStyleSheet(Styles::recipeDetail(darkMode));
    setupUI();
}

void RecipeDetailWindow::setupUI() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(16, 16, 16, 16);

    QFrame* card = new QFrame(this);
    card->setObjectName("DetailShell");
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(24, 24, 24, 24);
    cardLayout->setSpacing(16);

    QHBoxLayout* topActions = new QHBoxLayout();
    topActions->setSpacing(10);
    QPushButton* backButton = new QPushButton("Back", card);
    backButton->setObjectName("SecondaryButton");
    backButton->setMinimumSize(78, 42);
    connect(backButton, &QPushButton::clicked, this, &QDialog::accept);

    favouriteButton = new QPushButton(card);
    favouriteButton->setObjectName("GhostButton");
    favouriteButton->setMinimumHeight(42);
    connect(favouriteButton, &QPushButton::clicked, [this]() {
        emit favouriteToggled(recipe.getId());
        accept();
    });

    editButton = new QPushButton("Edit Recipe", card);
    editButton->setObjectName("PrimaryButton");
    editButton->setMinimumHeight(42);
    connect(editButton, &QPushButton::clicked, [this]() {
        emit editRequested(recipe.getId());
        accept();
    });

    topActions->addWidget(backButton);
    topActions->addStretch();
    topActions->addWidget(favouriteButton);
    topActions->addWidget(editButton);

    imageLabel = new QLabel(card);
    imageLabel->setPixmap(UiHelpers::recipePixmap(recipe, QSize(900, 220), 20));
    imageLabel->setFixedHeight(220);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

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
    chipRow->setSpacing(10);
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
    scrollArea->setObjectName("DetailScrollArea");
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->viewport()->setObjectName("DetailViewport");

    QWidget* content = new QWidget(scrollArea);
    content->setObjectName("DetailScrollContent");
    QVBoxLayout* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(14);

    if (!recipe.getDescription().trimmed().isEmpty()) {
        QLabel* descriptionLabel = new QLabel(recipe.getDescription(), content);
        descriptionLabel->setObjectName("DetailDescription");
        descriptionLabel->setWordWrap(true);
        contentLayout->addWidget(descriptionLabel);
    }

    QFrame* ingredientsCard = new QFrame(content);
    ingredientsCard->setObjectName("DetailSection");
    QVBoxLayout* ingredientsLayout = new QVBoxLayout(ingredientsCard);
    ingredientsLayout->setContentsMargins(20, 18, 20, 18);
    ingredientsLayout->setSpacing(8);

    QLabel* ingredientsTitle = new QLabel("Ingredients", ingredientsCard);
    ingredientsTitle->setObjectName("SectionTitle");
    ingredientsLabel = new QLabel("- " + recipe.getIngredients().join("\n- "), ingredientsCard);
    ingredientsLabel->setObjectName("DetailBody");
    ingredientsLabel->setWordWrap(true);
    ingredientsLayout->addWidget(ingredientsTitle);
    ingredientsLayout->addWidget(ingredientsLabel);

    QFrame* instructionsCard = new QFrame(content);
    instructionsCard->setObjectName("DetailSection");
    QVBoxLayout* instructionsLayout = new QVBoxLayout(instructionsCard);
    instructionsLayout->setContentsMargins(20, 18, 20, 18);
    instructionsLayout->setSpacing(8);

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
    imageLabel->setPixmap(UiHelpers::recipePixmap(recipe, QSize(900, 220), 20));
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
