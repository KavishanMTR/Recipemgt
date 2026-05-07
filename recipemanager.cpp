#include "recipemanager.h"

#include "filehandler.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>

#include <algorithm>

RecipeManager::RecipeManager() : nextId(1) {
    dataFilePath = FileHandler::getRecipesFilePath();
    loadFromFile();
}

RecipeManager::RecipeManager(const QString& filePath)
    : nextId(1),
      dataFilePath(filePath) {
    loadFromFile();
}

void RecipeManager::loadFromFile() {
    recipes.clear();

    const QString raw = FileHandler::readFile(dataFilePath).trimmed();
    int maxId = 0;

    if (raw.isEmpty()) {
        nextId = 1;
        return;
    }

    if (raw.startsWith("[") || raw.startsWith("{")) {
        QJsonParseError error;
        const QJsonDocument document = QJsonDocument::fromJson(raw.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError) {
            const QJsonArray array = document.isObject()
                ? document.object().value("recipes").toArray()
                : document.array();

            for (const QJsonValue& value : array) {
                Recipe recipe = Recipe::fromJson(value.toObject());
                if (recipe.isValid()) {
                    recipes.append(recipe);
                    maxId = qMax(maxId, recipe.getId());
                }
            }
        }
    } else {
        const QStringList lines = raw.split("\n", Qt::SkipEmptyParts);
        for (const QString& line : lines) {
            Recipe recipe = Recipe::fromCSV(line.trimmed());
            if (recipe.isValid()) {
                recipes.append(recipe);
                maxId = qMax(maxId, recipe.getId());
            }
        }

        saveToFile();
    }

    nextId = maxId + 1;
}

void RecipeManager::saveToFile() {
    QJsonArray array;
    for (const Recipe& recipe : recipes) {
        array.append(recipe.toJson());
    }

    const QJsonDocument document(array);
    FileHandler::writeFile(dataFilePath, QString::fromUtf8(document.toJson(QJsonDocument::Indented)));
}

bool RecipeManager::addRecipe(Recipe& recipe) {
    recipe.setId(nextId++);
    if (!recipe.getDateAdded().isValid()) {
        recipe.setDateAdded(QDateTime::currentDateTime());
    }

    recipes.append(recipe);
    saveToFile();
    return true;
}

bool RecipeManager::updateRecipe(const Recipe& recipe) {
    for (Recipe& current : recipes) {
        if (current.getId() == recipe.getId()) {
            current = recipe;
            saveToFile();
            return true;
        }
    }

    return false;
}

bool RecipeManager::deleteRecipe(int id) {
    for (int index = 0; index < recipes.size(); ++index) {
        if (recipes[index].getId() == id) {
            recipes.removeAt(index);
            saveToFile();
            return true;
        }
    }

    return false;
}

Recipe* RecipeManager::findById(int id) {
    for (Recipe& recipe : recipes) {
        if (recipe.getId() == id) {
            return &recipe;
        }
    }

    return nullptr;
}

Recipe* RecipeManager::findByName(const QString& name) {
    for (Recipe& recipe : recipes) {
        if (recipe.getName().compare(name.trimmed(), Qt::CaseInsensitive) == 0) {
            return &recipe;
        }
    }

    return nullptr;
}

QVector<Recipe> RecipeManager::getAllRecipes() const {
    return recipes;
}

QVector<Recipe> RecipeManager::searchRecipes(const QString& query) const {
    return filterRecipes(query, QString(), false);
}

QVector<Recipe> RecipeManager::getByCategory(const QString& category) const {
    return filterRecipes(QString(), category, false);
}

QVector<Recipe> RecipeManager::getFavourites() const {
    return filterRecipes(QString(), QString(), true);
}

QVector<Recipe> RecipeManager::getRecentRecipes(int count) const {
    QVector<Recipe> sortedRecipes = recipes;
    std::sort(sortedRecipes.begin(), sortedRecipes.end(), [](const Recipe& left, const Recipe& right) {
        return left.getDateAdded() > right.getDateAdded();
    });

    return sortedRecipes.mid(0, qMin(count, sortedRecipes.size()));
}

QVector<Recipe> RecipeManager::filterRecipes(const QString& query, const QString& category, bool favouritesOnly) const {
    QVector<Recipe> filteredRecipes;
    const QString normalizedCategory = category.trimmed();

    for (const Recipe& recipe : recipes) {
        if (favouritesOnly && !recipe.getFavourite()) {
            continue;
        }

        if (!normalizedCategory.isEmpty() &&
            normalizedCategory != "All" &&
            normalizedCategory != "All Categories" &&
            recipe.getCategory().compare(normalizedCategory, Qt::CaseInsensitive) != 0) {
            continue;
        }

        if (!recipe.matchesSearch(query)) {
            continue;
        }

        filteredRecipes.append(recipe);
    }

    return filteredRecipes;
}

bool RecipeManager::toggleFavourite(int id) {
    Recipe* recipe = findById(id);
    if (!recipe) {
        return false;
    }

    recipe->setFavourite(!recipe->getFavourite());
    saveToFile();
    return true;
}

bool RecipeManager::markFavourite(int id, bool favourite) {
    Recipe* recipe = findById(id);
    if (!recipe) {
        return false;
    }

    recipe->setFavourite(favourite);
    saveToFile();
    return true;
}

int RecipeManager::getTotalCount() const {
    return recipes.size();
}

int RecipeManager::getFavouriteCount() const {
    int favouriteCount = 0;
    for (const Recipe& recipe : recipes) {
        if (recipe.getFavourite()) {
            ++favouriteCount;
        }
    }
    return favouriteCount;
}

QStringList RecipeManager::getCategories() const {
    QStringList categories;
    for (const Recipe& recipe : recipes) {
        if (!recipe.getCategory().isEmpty() && !categories.contains(recipe.getCategory())) {
            categories.append(recipe.getCategory());
        }
    }

    std::sort(categories.begin(), categories.end(), [](const QString& left, const QString& right) {
        return left.toLower() < right.toLower();
    });

    return categories;
}

void RecipeManager::reload() {
    loadFromFile();
}

void RecipeManager::save() {
    saveToFile();
}
