#include "addrecipedialog.h"

#include "filehandler.h"
#include "styles.h"
#include "uihelpers.h"

#include <QFileDialog>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

AddRecipeDialog::AddRecipeDialog(bool darkMode, QWidget* parent)
    : QDialog(parent),
      darkMode(darkMode),
      editMode(false) {
    setWindowTitle("Add Recipe");
    resize(760, 760);
    setMinimumSize(720, 720);
    setStyleSheet(Styles::dialog(darkMode));
    setupUI();
    setupConnections();
}

AddRecipeDialog::AddRecipeDialog(const Recipe& recipe, bool darkMode, QWidget* parent)
    : QDialog(parent),
      darkMode(darkMode),
      editMode(true),
      editingRecipe(recipe),
      selectedImagePath(recipe.getImagePath()) {
    setWindowTitle("Edit Recipe");
    resize(760, 760);
    setMinimumSize(720, 720);
    setStyleSheet(Styles::dialog(darkMode));
    setupUI();
    setupConnections();
    populateFields();
}

void AddRecipeDialog::setupUI() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(18, 18, 18, 18);

    QFrame* card = new QFrame(this);
    card->setObjectName("DialogCard");
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(22, 22, 22, 22);
    cardLayout->setSpacing(16);

    QLabel* titleLabel = new QLabel(editMode ? "Edit Recipe" : "Create a New Recipe", card);
    titleLabel->setObjectName("DialogTitle");

    QLabel* subtitleLabel = new QLabel(
        editMode
            ? "Update the recipe details, image, timings, and instructions."
            : "Add a polished recipe card with ingredients, instructions, and food imagery.",
        card
    );
    subtitleLabel->setObjectName("MutedText");
    subtitleLabel->setWordWrap(true);

    QGridLayout* formLayout = new QGridLayout();
    formLayout->setHorizontalSpacing(14);
    formLayout->setVerticalSpacing(12);

    auto addFieldLabel = [&](const QString& text) {
        QLabel* label = new QLabel(text, card);
        label->setObjectName("FieldLabel");
        return label;
    };

    nameEdit = new QLineEdit(card);
    nameEdit->setPlaceholderText("Creamy Tuscan Pasta");

    categoryCombo = new QComboBox(card);
    categoryCombo->addItems({"Breakfast", "Lunch", "Dinner", "Dessert", "Snack", "Beverage", "Soup", "Salad", "Baking", "Other"});

    descriptionEdit = new QLineEdit(card);
    descriptionEdit->setPlaceholderText("Short description that appears in cards and the detail page");

    prepTimeSpin = new QSpinBox(card);
    prepTimeSpin->setRange(0, 600);
    prepTimeSpin->setSuffix(" min");
    prepTimeSpin->setValue(15);

    cookTimeSpin = new QSpinBox(card);
    cookTimeSpin->setRange(0, 600);
    cookTimeSpin->setSuffix(" min");
    cookTimeSpin->setValue(20);

    ingredientsEdit = new QTextEdit(card);
    ingredientsEdit->setMinimumHeight(150);
    ingredientsEdit->setPlaceholderText("Enter one ingredient per line");

    instructionsEdit = new QTextEdit(card);
    instructionsEdit->setMinimumHeight(170);
    instructionsEdit->setPlaceholderText("Write clear step-by-step cooking instructions");

    imagePathEdit = new QLineEdit(card);
    imagePathEdit->setReadOnly(true);
    imagePathEdit->setPlaceholderText("No image selected");

    browseButton = new QPushButton("Upload Image", card);
    browseButton->setObjectName("GhostButton");

    imagePreviewLabel = new QLabel(card);
    imagePreviewLabel->setFixedSize(210, 140);
    imagePreviewLabel->setAlignment(Qt::AlignCenter);

    validationLabel = new QLabel(card);
    validationLabel->setObjectName("ValidationLabel");
    validationLabel->hide();

    formLayout->addWidget(addFieldLabel("RECIPE NAME"), 0, 0);
    formLayout->addWidget(addFieldLabel("CATEGORY"), 0, 1);
    formLayout->addWidget(nameEdit, 1, 0);
    formLayout->addWidget(categoryCombo, 1, 1);

    formLayout->addWidget(addFieldLabel("DESCRIPTION"), 2, 0);
    formLayout->addWidget(addFieldLabel("PREPARATION TIME"), 2, 1);
    formLayout->addWidget(descriptionEdit, 3, 0);
    formLayout->addWidget(prepTimeSpin, 3, 1);

    formLayout->addWidget(addFieldLabel("COOKING TIME"), 4, 0);
    formLayout->addWidget(addFieldLabel("IMAGE"), 4, 1);
    formLayout->addWidget(cookTimeSpin, 5, 0);

    QVBoxLayout* imageLayout = new QVBoxLayout();
    imageLayout->setSpacing(8);
    imageLayout->addWidget(imagePathEdit);
    imageLayout->addWidget(browseButton, 0, Qt::AlignLeft);
    QWidget* imageFieldWidget = new QWidget(card);
    imageFieldWidget->setLayout(imageLayout);
    formLayout->addWidget(imageFieldWidget, 5, 1);

    QHBoxLayout* bodyLayout = new QHBoxLayout();
    bodyLayout->setSpacing(16);

    QVBoxLayout* textColumns = new QVBoxLayout();
    textColumns->setSpacing(12);
    textColumns->addWidget(addFieldLabel("INGREDIENTS"));
    textColumns->addWidget(ingredientsEdit);
    textColumns->addWidget(addFieldLabel("INSTRUCTIONS"));
    textColumns->addWidget(instructionsEdit);

    QVBoxLayout* previewColumn = new QVBoxLayout();
    previewColumn->setSpacing(10);
    QLabel* previewLabel = addFieldLabel("PREVIEW");
    QLabel* previewHint = new QLabel("Cards and detail pages will use the uploaded image. If no image is selected, the app generates a premium placeholder.", card);
    previewHint->setObjectName("PreviewHint");
    previewHint->setWordWrap(true);
    previewColumn->addWidget(previewLabel);
    previewColumn->addWidget(imagePreviewLabel);
    previewColumn->addWidget(previewHint);
    previewColumn->addStretch();

    bodyLayout->addLayout(textColumns, 3);
    bodyLayout->addLayout(previewColumn, 2);

    QHBoxLayout* buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(10);
    buttonRow->addStretch();

    cancelButton = new QPushButton("Cancel", card);
    cancelButton->setObjectName("SecondaryButton");

    saveButton = new QPushButton(editMode ? "Save Changes" : "Create Recipe", card);
    saveButton->setObjectName("PrimaryButton");

    buttonRow->addWidget(cancelButton);
    buttonRow->addWidget(saveButton);

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(subtitleLabel);
    cardLayout->addLayout(formLayout);
    cardLayout->addLayout(bodyLayout, 1);
    cardLayout->addWidget(validationLabel);
    cardLayout->addLayout(buttonRow);

    rootLayout->addWidget(card);
    updatePreview();
}

void AddRecipeDialog::setupConnections() {
    connect(browseButton, &QPushButton::clicked, this, &AddRecipeDialog::onBrowseImage);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveButton, &QPushButton::clicked, [this]() {
        if (validateForm()) {
            accept();
        }
    });
    connect(nameEdit, &QLineEdit::textChanged, this, [this]() { updatePreview(); });
    connect(categoryCombo, &QComboBox::currentTextChanged, this, [this]() { updatePreview(); });
    connect(prepTimeSpin, qOverload<int>(&QSpinBox::valueChanged), this, [this](int) { updatePreview(); });
    connect(cookTimeSpin, qOverload<int>(&QSpinBox::valueChanged), this, [this](int) { updatePreview(); });
}

void AddRecipeDialog::populateFields() {
    nameEdit->setText(editingRecipe.getName());
    descriptionEdit->setText(editingRecipe.getDescription());
    ingredientsEdit->setPlainText(editingRecipe.getIngredients().join("\n"));
    instructionsEdit->setPlainText(editingRecipe.getInstructions());
    prepTimeSpin->setValue(editingRecipe.getPrepTime());
    cookTimeSpin->setValue(editingRecipe.getCookTime());
    imagePathEdit->setText(editingRecipe.getImagePath());

    const int categoryIndex = categoryCombo->findText(editingRecipe.getCategory());
    if (categoryIndex >= 0) {
        categoryCombo->setCurrentIndex(categoryIndex);
    }

    updatePreview();
}

void AddRecipeDialog::updatePreview() {
    Recipe previewRecipe;
    previewRecipe.setName(nameEdit ? nameEdit->text().trimmed() : editingRecipe.getName());
    previewRecipe.setCategory(categoryCombo ? categoryCombo->currentText() : editingRecipe.getCategory());
    previewRecipe.setPrepTime(prepTimeSpin ? prepTimeSpin->value() : editingRecipe.getPrepTime());
    previewRecipe.setCookTime(cookTimeSpin ? cookTimeSpin->value() : editingRecipe.getCookTime());
    previewRecipe.setImagePath(selectedImagePath);
    imagePreviewLabel->setPixmap(UiHelpers::recipePixmap(previewRecipe, imagePreviewLabel->size(), 20));
}

bool AddRecipeDialog::validateForm() {
    validationLabel->hide();

    if (nameEdit->text().trimmed().isEmpty()) {
        validationLabel->setText("Recipe name is required.");
        validationLabel->show();
        return false;
    }

    if (ingredientsEdit->toPlainText().trimmed().isEmpty()) {
        validationLabel->setText("Add at least one ingredient.");
        validationLabel->show();
        return false;
    }

    if (instructionsEdit->toPlainText().trimmed().isEmpty()) {
        validationLabel->setText("Cooking instructions are required.");
        validationLabel->show();
        return false;
    }

    return true;
}

Recipe AddRecipeDialog::getRecipe() const {
    Recipe recipe;
    if (editMode) {
        recipe.setId(editingRecipe.getId());
        recipe.setFavourite(editingRecipe.getFavourite());
        recipe.setAddedBy(editingRecipe.getAddedBy());
        recipe.setDateAdded(editingRecipe.getDateAdded());
    }

    recipe.setName(nameEdit->text().trimmed());
    recipe.setCategory(categoryCombo->currentText());
    recipe.setDescription(descriptionEdit->text().trimmed());
    recipe.setPrepTime(prepTimeSpin->value());
    recipe.setCookTime(cookTimeSpin->value());
    recipe.setIngredients(ingredientsEdit->toPlainText().split("\n", Qt::SkipEmptyParts));
    recipe.setInstructions(instructionsEdit->toPlainText().trimmed());
    recipe.setImagePath(selectedImagePath);
    return recipe;
}

void AddRecipeDialog::onBrowseImage() {
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        "Select Recipe Image",
        QString(),
        "Images (*.png *.jpg *.jpeg *.bmp *.webp)"
    );

    if (filePath.isEmpty()) {
        return;
    }

    selectedImagePath = FileHandler::importImage(filePath);
    imagePathEdit->setText(selectedImagePath);
    updatePreview();
}
