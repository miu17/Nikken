#ifndef CALCULATIONMAKERUTILS_H
#define CALCULATIONMAKERUTILS_H

#include <QString>
#include <QStringList>
#include <QPrinter>
#include <QVariant>

#include "calculationmaker.h"

namespace CalculationMakerUtils {

extern QStringList splitString(const QString &orig);

extern bool isQuotedString(const QString &orig);
extern QString trimQuote(const QString &orig);

extern bool toBool(const QString &str, bool *ok = 0);
extern Qt::Alignment toAlignment(const QString &str, bool *ok = 0);
extern QPageLayout::Orientation toOrientation(const QString &str, bool *ok = 0);
extern QPagedPaintDevice::PageSize toPaperSize(const QString &str, bool *ok = 0);

extern bool isPenName(const QString &str);

} // namespace CalculationMakerUtils

#endif // CALCULATIONMAKERUTILS_H
