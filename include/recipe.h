#pragma once

#include <QDateTime>
#include <QJsonObject>
#include <QString>
#include <QStringList>

class Recipe {
private:
    int id;
    QString name;
    QString category;
    QString description;
    QStringList ingredients;
    QString instructions;
    int prepTimeMinutes;
    int cookTimeMinutes;
    bool isFavourite;
    QString imagePath;
    QString addedBy;
    QDateTime dateAdded;

public:
    Recipe();
    Recipe(int id, const QString& name, const QString& category,
           const QString& description, const QStringList& ingredients,
           const QString& instructions, int prepTime, int cookTime,
           const QString& addedBy = QString());

    int getId() const;
    QString getName() const;
    QString getCategory() const;
    QString getDescription() const;
    QStringList getIngredients() const;
    QString getInstructions() const;
    int getPrepTime() const;
    int getCookTime() const;
    int getTotalTime() const;
    bool getFavourite() const;
    QString getImagePath() const;
    QString getAddedBy() const;
    QDateTime getDateAdded() const;
    QString getIngredientsText() const;

    void setId(int id);
    void setName(const QString& name);
    void setCategory(const QString& category);
    void setDescription(const QString& description);
    void setIngredients(const QStringList& ingredients);
    void setInstructions(const QString& instructions);
    void setPrepTime(int minutes);
    void setCookTime(int minutes);
    void setFavourite(bool favourite);
    void setImagePath(const QString& path);
    void setAddedBy(const QString& username);
    void setDateAdded(const QDateTime& addedAt);

    bool isValid() const;
    bool matchesSearch(const QString& query) const;
    QJsonObject toJson() const;
    static Recipe fromJson(const QJsonObject& object);
    QString toCSV() const;
    static Recipe fromCSV(const QString& csv);
    QString getCategoryLabel() const;
};

class FavouriteRecipe : public Recipe {
private:
    QDateTime favoritedAt;
    QString note;

public:
    FavouriteRecipe();
    FavouriteRecipe(const Recipe& recipe, const QString& note = QString());

    QDateTime getFavoritedAt() const;
    QString getNote() const;
    void setNote(const QString& note);
};
