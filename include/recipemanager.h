#pragma once

#include "recipe.h"

#include <QString>
#include <QStringList>
#include <QVector>

class RecipeManager {
private:
    QVector<Recipe> recipes;
    int nextId;
    QString dataFilePath;

    void loadFromFile();
    void saveToFile();

public:
    RecipeManager();
    explicit RecipeManager(const QString& filePath);

    bool addRecipe(Recipe& recipe);
    bool updateRecipe(const Recipe& recipe);
    bool deleteRecipe(int id);
    Recipe* findById(int id);
    Recipe* findByName(const QString& name);

    QVector<Recipe> getAllRecipes() const;
    QVector<Recipe> searchRecipes(const QString& query) const;
    QVector<Recipe> getByCategory(const QString& category) const;
    QVector<Recipe> getFavourites() const;
    QVector<Recipe> getRecentRecipes(int count = 6) const;
    QVector<Recipe> filterRecipes(const QString& query, const QString& category, bool favouritesOnly = false) const;

    bool toggleFavourite(int id);
    bool markFavourite(int id, bool favourite);

    int getTotalCount() const;
    int getFavouriteCount() const;
    QStringList getCategories() const;

    void reload();
    void save();
};
