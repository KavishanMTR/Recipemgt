#include "filehandler.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QStringConverter>
#endif
#include <QStandardPaths>
#include <QTextStream>
#include <QtGlobal>

namespace {
void setUtf8Encoding(QTextStream& stream) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    stream.setEncoding(QStringConverter::Utf8);
#else
    stream.setCodec("UTF-8");
#endif
}
}

QString FileHandler::appDataPath;

void FileHandler::initialize() {
    if (!appDataPath.isEmpty()) {
        return;
    }

    appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (appDataPath.isEmpty()) {
        appDataPath = QDir::homePath() + "/RecipeManagementSystem";
    }

    createDirectory(appDataPath);
    createDirectory(getImagesPath());
}

QString FileHandler::getDataPath() {
    if (appDataPath.isEmpty()) {
        initialize();
    }
    return appDataPath;
}

QString FileHandler::getImagesPath() {
    return getDataPath() + "/images";
}

bool FileHandler::writeFile(const QString& filename, const QString& content) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        return false;
    }

    QTextStream stream(&file);
    setUtf8Encoding(stream);
    stream << content;
    return true;
}

QString FileHandler::readFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QString();
    }

    QTextStream stream(&file);
    setUtf8Encoding(stream);
    return stream.readAll();
}

bool FileHandler::appendLine(const QString& filename, const QString& line) {
    QFile file(filename);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return false;
    }

    QTextStream stream(&file);
    setUtf8Encoding(stream);
    stream << line << "\n";
    return true;
}

QStringList FileHandler::readLines(const QString& filename) {
    const QString content = readFile(filename);
    if (content.isEmpty()) {
        return {};
    }

    return content.split("\n", Qt::SkipEmptyParts);
}

bool FileHandler::fileExists(const QString& filename) {
    return QFile::exists(filename);
}

bool FileHandler::deleteFile(const QString& filename) {
    return QFile::remove(filename);
}

bool FileHandler::createDirectory(const QString& path) {
    return QDir().mkpath(path);
}

QString FileHandler::importImage(const QString& sourcePath) {
    if (sourcePath.trimmed().isEmpty()) {
        return QString();
    }

    QFileInfo info(sourcePath);
    if (!info.exists() || !info.isFile()) {
        return QString();
    }

    createDirectory(getImagesPath());

    const QString extension = info.suffix().isEmpty() ? QString("png") : info.suffix().toLower();
    const QString targetName = info.completeBaseName().replace(" ", "_") + "_" +
                               QString::number(QDateTime::currentMSecsSinceEpoch()) + "." + extension;
    const QString targetPath = getImagesPath() + "/" + targetName;

    if (QFile::exists(targetPath)) {
        QFile::remove(targetPath);
    }

    if (QFile::copy(sourcePath, targetPath)) {
        return targetPath;
    }

    return sourcePath;
}

QVariant FileHandler::readSetting(const QString& key, const QVariant& defaultValue) {
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);
    return settings.value(key, defaultValue);
}

void FileHandler::writeSetting(const QString& key, const QVariant& value) {
    QSettings settings(getSettingsFilePath(), QSettings::IniFormat);
    settings.setValue(key, value);
    settings.sync();
}

QString FileHandler::getRecipesFilePath() {
    return getDataPath() + "/recipes.json";
}

QString FileHandler::getUsersFilePath() {
    return getDataPath() + "/users.json";
}

QString FileHandler::getSettingsFilePath() {
    return getDataPath() + "/settings.ini";
}
