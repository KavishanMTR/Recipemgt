#pragma once

#include "recipe.h"

#include <QFileInfo>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

namespace UiHelpers {

inline QColor categoryColor(const QString& category) {
    const QString key = category.trimmed().toLower();
    if (key == "breakfast") return QColor("#F6AD55");
    if (key == "lunch") return QColor("#68D391");
    if (key == "dinner") return QColor("#F56565");
    if (key == "dessert") return QColor("#D53F8C");
    if (key == "snack") return QColor("#4299E1");
    if (key == "beverage") return QColor("#38B2AC");
    if (key == "soup") return QColor("#DD6B20");
    if (key == "salad") return QColor("#48BB78");
    if (key == "baking") return QColor("#9F7AEA");
    return QColor("#718096");
}

inline QString recipeInitials(const QString& name) {
    const QStringList parts = name.simplified().split(' ', Qt::SkipEmptyParts);
    QString initials;
    for (int index = 0; index < qMin(parts.size(), 2); ++index) {
        initials += parts[index].left(1).toUpper();
    }
    return initials.isEmpty() ? QString("RM") : initials;
}

inline QPixmap roundedPixmap(const QPixmap& source, const QSize& size, int radius) {
    if (source.isNull()) {
        return QPixmap();
    }

    QPixmap target(size);
    target.fill(Qt::transparent);

    QPainter painter(&target);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    QPainterPath clipPath;
    clipPath.addRoundedRect(target.rect(), radius, radius);
    painter.setClipPath(clipPath);

    const QPixmap scaled = source.scaled(size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    const QPoint offset((scaled.width() - size.width()) / 2, (scaled.height() - size.height()) / 2);
    painter.drawPixmap(0, 0, scaled, offset.x(), offset.y(), size.width(), size.height());
    return target;
}

inline QPixmap makePlaceholderPixmap(const Recipe& recipe, const QSize& size, int radius = 24) {
    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    const QColor accent = categoryColor(recipe.getCategory());
    const QColor accentDark = accent.darker(145);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    QPainterPath clipPath;
    clipPath.addRoundedRect(pixmap.rect(), radius, radius);
    painter.fillPath(clipPath, QLinearGradient(0, 0, size.width(), size.height()));

    QLinearGradient background(0, 0, size.width(), size.height());
    background.setColorAt(0.0, accent);
    background.setColorAt(1.0, accentDark);
    painter.fillPath(clipPath, background);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(255, 255, 255, 32));
    painter.drawEllipse(QRect(size.width() - 110, -10, 120, 120));
    painter.drawEllipse(QRect(-20, size.height() - 80, 120, 120));

    painter.setBrush(QColor(255, 255, 255, 46));
    painter.drawRoundedRect(QRect(18, 18, 68, 28), 14, 14);

    QFont labelFont("Segoe UI", 9, QFont::DemiBold);
    labelFont.setCapitalization(QFont::AllUppercase);
    painter.setFont(labelFont);
    painter.setPen(Qt::white);
    painter.drawText(QRect(18, 18, 68, 28), Qt::AlignCenter, recipe.getCategoryLabel());

    painter.setBrush(QColor(255, 255, 255, 220));
    painter.drawEllipse(QRect(size.width() / 2 - 42, size.height() / 2 - 24, 84, 84));
    painter.setBrush(accentDark);
    painter.drawEllipse(QRect(size.width() / 2 - 28, size.height() / 2 - 10, 56, 56));

    painter.setPen(QPen(QColor(255, 255, 255, 210), 4, Qt::SolidLine, Qt::RoundCap));
    painter.drawArc(QRect(size.width() / 2 - 34, size.height() / 2 - 16, 68, 68), 35 * 16, 110 * 16);
    painter.drawLine(size.width() / 2 - 52, size.height() / 2 + 18, size.width() / 2 - 68, size.height() / 2 + 40);
    painter.drawLine(size.width() / 2 + 52, size.height() / 2 + 18, size.width() / 2 + 68, size.height() / 2 + 40);

    return pixmap;
}

inline QPixmap recipePixmap(const Recipe& recipe, const QSize& size, int radius = 24) {
    if (!recipe.getImagePath().trimmed().isEmpty()) {
        QFileInfo info(recipe.getImagePath());
        if (info.exists() && info.isFile()) {
            const QPixmap source(recipe.getImagePath());
            if (!source.isNull()) {
                return roundedPixmap(source, size, radius);
            }
        }
    }

    return makePlaceholderPixmap(recipe, size, radius);
}

}
