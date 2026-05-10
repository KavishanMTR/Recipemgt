#include "recipecard.h"

#include "styles.h"
#include "uihelpers.h"

#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QSizePolicy>
#include <QStyle>
#include <QVBoxLayout>

RecipeCard::RecipeCard(const Recipe& recipe, bool darkMode, QWidget* parent)
    : QWidget(parent),
      recipe(recipe),
      darkMode(darkMode) {
    setObjectName("RecipeCard");
    setFixedSize(280, 270);
    setCursor(Qt::PointingHandCursor);
    setupUI();
    applyTheme();

    auto* shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(16);
    shadow->setOffset(0, 6);
    shadow->setColor(QColor(15, 23, 42, 20));
    setGraphicsEffect(shadow);
}

void RecipeCard::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(10);

    imageLabel = new QLabel(this);
    imageLabel->setFixedHeight(128);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout* infoRow = new QHBoxLayout();
    infoRow->setSpacing(8);

    badgeLabel = new QLabel(recipe.getCategoryLabel(), this);
    badgeLabel->setObjectName("Badge");
    badgeLabel->setMinimumHeight(26);

    favouriteButton = new QPushButton(this);
    favouriteButton->setObjectName("IconButton");
    favouriteButton->setFixedSize(58, 34);
    favouriteButton->setCursor(Qt::PointingHandCursor);

    infoRow->addWidget(badgeLabel);
    infoRow->addStretch();
    infoRow->addWidget(favouriteButton);

    nameLabel = new QLabel(recipe.getName(), this);
    nameLabel->setObjectName("RecipeName");
    nameLabel->setWordWrap(true);
    nameLabel->setFixedHeight(42);

    QHBoxLayout* metaRow = new QHBoxLayout();
    metaRow->setSpacing(8);

    timeLabel = new QLabel(this);
    timeLabel->setObjectName("RecipeMeta");

    authorLabel = new QLabel(this);
    authorLabel->setObjectName("RecipeMeta");
    authorLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    metaRow->addWidget(timeLabel);
    metaRow->addStretch();
    metaRow->addWidget(authorLabel);

    QHBoxLayout* actionRow = new QHBoxLayout();
    actionRow->setSpacing(8);

    viewButton = new QPushButton("View", this);
    viewButton->setObjectName("TextButton");
    viewButton->setCursor(Qt::PointingHandCursor);

    editButton = new QPushButton("Edit", this);
    editButton->setObjectName("IconButton");
    editButton->setCursor(Qt::PointingHandCursor);
    editButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    editButton->setMinimumSize(56, 34);

    deleteButton = new QPushButton("Delete", this);
    deleteButton->setObjectName("DangerButton");
    deleteButton->setCursor(Qt::PointingHandCursor);
    deleteButton->setMinimumSize(62, 34);
    viewButton->setMinimumHeight(34);

    actionRow->addWidget(viewButton, 1);
    actionRow->addWidget(editButton);
    actionRow->addWidget(deleteButton);

    layout->addWidget(imageLabel);
    layout->addLayout(infoRow);
    layout->addWidget(nameLabel);
    layout->addLayout(metaRow);
    layout->addStretch();
    layout->addLayout(actionRow);

    connect(viewButton, &QPushButton::clicked, this, &RecipeCard::onViewClicked);
    connect(editButton, &QPushButton::clicked, this, &RecipeCard::onEditClicked);
    connect(deleteButton, &QPushButton::clicked, this, &RecipeCard::onDeleteClicked);
    connect(favouriteButton, &QPushButton::clicked, this, &RecipeCard::onFavouriteClicked);
}

void RecipeCard::applyTheme() {
    setStyleSheet(Styles::recipeCard(darkMode));
    imageLabel->setPixmap(UiHelpers::recipePixmap(recipe, QSize(248, 128), 16));

    const QString author = recipe.getAddedBy().trimmed().isEmpty()
        ? QString("Community")
        : recipe.getAddedBy();
    timeLabel->setText(QString("%1 min total").arg(recipe.getTotalTime()));
    authorLabel->setText(author);
    updateFavouriteButton();
}

void RecipeCard::updateFavouriteButton() {
    favouriteButton->setText(recipe.getFavourite() ? "Saved" : "Save");
}

void RecipeCard::mouseDoubleClickEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit viewRequested(recipe.getId());
    }
    QWidget::mouseDoubleClickEvent(event);
}

void RecipeCard::updateRecipe(const Recipe& value) {
    recipe = value;
    nameLabel->setText(recipe.getName());
    badgeLabel->setText(recipe.getCategoryLabel());
    applyTheme();
}

void RecipeCard::onViewClicked() {
    emit viewRequested(recipe.getId());
}

void RecipeCard::onEditClicked() {
    emit editRequested(recipe.getId());
}

void RecipeCard::onDeleteClicked() {
    emit deleteRequested(recipe.getId());
}

void RecipeCard::onFavouriteClicked() {
    emit favouriteToggled(recipe.getId());
}
