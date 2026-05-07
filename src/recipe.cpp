#include "recipe.h"

#include <QJsonArray>
#include <QStringList>

namespace {
QString escapePipeField(const QString& value) {
    QString escaped = value;
    escaped.replace("\\", "\\\\");
    escaped.replace("|", "\\|");
    escaped.replace("\n", "\\n");
    return escaped;
}

QStringList splitEscapedPipe(const QString& text) {
    QStringList parts;
    QString current;
    bool escaping = false;

    for (const QChar ch : text) {
        if (escaping) {
            current.append(ch);
            escaping = false;
            continue;
        }

        if (ch == '\\') {
            escaping = true;
            continue;
        }

        if (ch == '|') {
            parts.append(current);
            current.clear();
            continue;
        }

        current.append(ch);
    }

    parts.append(current);
    return parts;
}

QString normalizeText(const QString& value) {
    QString cleaned = value;
    cleaned.replace("\r\n", "\n");
    return cleaned.trimmed();
}
}

Recipe::Recipe()
    : id(0),
      prepTimeMinutes(0),
      cookTimeMinutes(0),
      isFavourite(false),
      dateAdded(QDateTime::currentDateTime()) {}

Recipe::Recipe(int id, const QString& name, const QString& category,
               const QString& description, const QStringList& ingredients,
               const QString& instructions, int prepTime, int cookTime,
               const QString& addedBy)
    : id(id),
      name(name.trimmed()),
      category(category.trimmed()),
      description(normalizeText(description)),
      ingredients(ingredients),
      instructions(normalizeText(instructions)),
      prepTimeMinutes(prepTime),
      cookTimeMinutes(cookTime),
      isFavourite(false),
      addedBy(addedBy.trimmed()),
      dateAdded(QDateTime::currentDateTime()) {}

int Recipe::getId() const { return id; }
QString Recipe::getName() const { return name; }
QString Recipe::getCategory() const { return category; }
QString Recipe::getDescription() const { return description; }
QStringList Recipe::getIngredients() const { return ingredients; }
QString Recipe::getInstructions() const { return instructions; }
int Recipe::getPrepTime() const { return prepTimeMinutes; }
int Recipe::getCookTime() const { return cookTimeMinutes; }
int Recipe::getTotalTime() const { return prepTimeMinutes + cookTimeMinutes; }
bool Recipe::getFavourite() const { return isFavourite; }
QString Recipe::getImagePath() const { return imagePath; }
QString Recipe::getAddedBy() const { return addedBy; }
QDateTime Recipe::getDateAdded() const { return dateAdded; }

QString Recipe::getIngredientsText() const {
    return ingredients.join("\n");
}

void Recipe::setId(int value) { id = value; }
void Recipe::setName(const QString& value) { name = value.trimmed(); }
void Recipe::setCategory(const QString& value) { category = value.trimmed(); }
void Recipe::setDescription(const QString& value) { description = normalizeText(value); }
void Recipe::setIngredients(const QStringList& value) {
    QStringList cleaned;
    for (const QString& ingredient : value) {
        const QString trimmed = ingredient.trimmed();
        if (!trimmed.isEmpty()) {
            cleaned.append(trimmed);
        }
    }
    ingredients = cleaned;
}
void Recipe::setInstructions(const QString& value) { instructions = normalizeText(value); }
void Recipe::setPrepTime(int value) { prepTimeMinutes = qMax(0, value); }
void Recipe::setCookTime(int value) { cookTimeMinutes = qMax(0, value); }
void Recipe::setFavourite(bool value) { isFavourite = value; }
void Recipe::setImagePath(const QString& value) { imagePath = value.trimmed(); }
void Recipe::setAddedBy(const QString& value) { addedBy = value.trimmed(); }
void Recipe::setDateAdded(const QDateTime& value) { dateAdded = value.isValid() ? value : QDateTime::currentDateTime(); }

bool Recipe::isValid() const {
    return !name.isEmpty() &&
           !category.isEmpty() &&
           !ingredients.isEmpty() &&
           !instructions.isEmpty();
}

bool Recipe::matchesSearch(const QString& query) const {
    if (query.trimmed().isEmpty()) {
        return true;
    }

    const QString normalized = query.trimmed().toLower();
    return name.toLower().contains(normalized) ||
           category.toLower().contains(normalized) ||
           description.toLower().contains(normalized) ||
           instructions.toLower().contains(normalized) ||
           ingredients.join(" ").toLower().contains(normalized) ||
           addedBy.toLower().contains(normalized);
}

QJsonObject Recipe::toJson() const {
    return {
        {"id", id},
        {"name", name},
        {"category", category},
        {"description", description},
        {"ingredients", QJsonArray::fromStringList(ingredients)},
        {"instructions", instructions},
        {"prepTimeMinutes", prepTimeMinutes},
        {"cookTimeMinutes", cookTimeMinutes},
        {"isFavourite", isFavourite},
        {"imagePath", imagePath},
        {"addedBy", addedBy},
        {"dateAdded", dateAdded.toString(Qt::ISODate)}
    };
}

Recipe Recipe::fromJson(const QJsonObject& object) {
    Recipe recipe;
    recipe.setId(object.value("id").toInt());
    recipe.setName(object.value("name").toString());
    recipe.setCategory(object.value("category").toString());
    recipe.setDescription(object.value("description").toString());

    QStringList loadedIngredients;
    const QJsonArray ingredientArray = object.value("ingredients").toArray();
    for (const QJsonValue& value : ingredientArray) {
        loadedIngredients.append(value.toString());
    }

    recipe.setIngredients(loadedIngredients);
    recipe.setInstructions(object.value("instructions").toString());
    recipe.setPrepTime(object.value("prepTimeMinutes").toInt());
    recipe.setCookTime(object.value("cookTimeMinutes").toInt());
    recipe.setFavourite(object.value("isFavourite").toBool(false));
    recipe.setImagePath(object.value("imagePath").toString());
    recipe.setAddedBy(object.value("addedBy").toString());
    recipe.setDateAdded(QDateTime::fromString(object.value("dateAdded").toString(), Qt::ISODate));
    return recipe;
}

QString Recipe::toCSV() const {
    return QString::number(id) + "|" +
           escapePipeField(name) + "|" +
           escapePipeField(category) + "|" +
           escapePipeField(description) + "|" +
           escapePipeField(ingredients.join(";")) + "|" +
           escapePipeField(instructions) + "|" +
           QString::number(prepTimeMinutes) + "|" +
           QString::number(cookTimeMinutes) + "|" +
           (isFavourite ? "1" : "0") + "|" +
           escapePipeField(imagePath) + "|" +
           escapePipeField(addedBy) + "|" +
           dateAdded.toString(Qt::ISODate);
}

Recipe Recipe::fromCSV(const QString& csv) {
    const QStringList parts = splitEscapedPipe(csv);
    Recipe recipe;
    if (parts.size() < 11) {
        return recipe;
    }

    recipe.setId(parts.value(0).toInt());
    recipe.setName(parts.value(1));
    recipe.setCategory(parts.value(2));
    recipe.setDescription(parts.value(3).replace("\\n", "\n"));
    recipe.setIngredients(parts.value(4).split(";", Qt::SkipEmptyParts));
    recipe.setInstructions(parts.value(5).replace("\\n", "\n"));
    recipe.setPrepTime(parts.value(6).toInt());
    recipe.setCookTime(parts.value(7).toInt());
    recipe.setFavourite(parts.value(8) == "1");

    if (parts.size() >= 12) {
        recipe.setImagePath(parts.value(9));
        recipe.setAddedBy(parts.value(10));
        recipe.setDateAdded(QDateTime::fromString(parts.value(11), Qt::ISODate));
    } else {
        recipe.setAddedBy(parts.value(9));
        recipe.setDateAdded(QDateTime::fromString(parts.value(10), Qt::ISODate));
    }

    return recipe;
}

QString Recipe::getCategoryLabel() const {
    return category.isEmpty() ? QString("General") : category;
}

FavouriteRecipe::FavouriteRecipe()
    : Recipe(),
      favoritedAt(QDateTime::currentDateTime()) {}

FavouriteRecipe::FavouriteRecipe(const Recipe& recipe, const QString& note)
    : Recipe(recipe),
      favoritedAt(QDateTime::currentDateTime()),
      note(note.trimmed()) {
    setFavourite(true);
}

QDateTime FavouriteRecipe::getFavoritedAt() const { return favoritedAt; }
QString FavouriteRecipe::getNote() const { return note; }
void FavouriteRecipe::setNote(const QString& value) { note = value.trimmed(); }
