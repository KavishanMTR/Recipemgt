#pragma once

#include <QVariant>
#include <QString>
#include <QStringList>

class FileHandler {
private:
    static QString appDataPath;

public:
    static void initialize();
    static QString getDataPath();
    static QString getImagesPath();

    static bool writeFile(const QString& filename, const QString& content);
    static QString readFile(const QString& filename);
    static bool appendLine(const QString& filename, const QString& line);
    static QStringList readLines(const QString& filename);

    static bool fileExists(const QString& filename);
    static bool deleteFile(const QString& filename);
    static bool createDirectory(const QString& path);
    static QString importImage(const QString& sourcePath);

    static QVariant readSetting(const QString& key, const QVariant& defaultValue = QVariant());
    static void writeSetting(const QString& key, const QVariant& value);

    static QString getRecipesFilePath();
    static QString getUsersFilePath();
    static QString getSettingsFilePath();
};
