
#include "calculationmakerutils.h"

namespace CalculationMakerUtils {

QString trimQuote(const QString &orig)
{
    if (!isQuotedString(orig))
        return orig;
    return orig.mid(1, orig.length() - 2);
}

bool isQuotedString(const QString &orig)
{
    if (orig.isEmpty() || orig.length() < 2)
        return false;
    QChar ch = orig.at(0);
    if (ch != QLatin1Char('\'') && ch != QLatin1Char('"'))
        return false;
    // TODO: check internal quote
    return ch == orig.at(orig.length() - 1);
}

QStringList splitString(const QString &orig)
{
    QStringList list;
    int i = 0;
    QChar quote;
    QChar ch;
    QString str;
    while (i < orig.length()) {
        str.clear();
        while (i < orig.length() && orig.at(i).isSpace())
            ++i;
        if (i >= orig.length())
            break;
        while (i < orig.length() && ((ch = orig.at(i)) != QLatin1Char(',') || !quote.isNull())) {
            str += ch;
            if (quote.isNull() && (ch == QLatin1Char('\'') || ch == QLatin1Char('"')))
                quote = ch;
            else if (!quote.isNull() && quote == ch)
                quote = QChar();
            ++i;
        }
        Q_ASSERT(i >= orig.length() || orig.at(i) == QLatin1Char(','));
        if (i < orig.length())
            ++i;
        list << str.trimmed();
    }
    return list;
}

bool toBool(const QString &str, bool *ok)
{
    bool value;
    bool result = true;
    if (str == u8"ON")
        value = true;
    else if (str == u8"OFF")
        value = false;
    else
        result = false;
    if (ok)
        *ok = result;
    return value;
}

Qt::Alignment toAlignment(const QString &str, bool *ok)
{
    Qt::Alignment align;
    align = Qt::AlignTop;
    bool result = true;
    if (str == u8"LEFT")
        align |= Qt::AlignLeft;
    else if (str == u8"RIGHT")
        align |= Qt::AlignRight;
    else if (str == u8"CENTER")
        align |= Qt::AlignCenter;
    else
        result = false;
    if (ok)
        *ok = result;
    return align;
}

QPageLayout::Orientation toOrientation(const QString &str, bool *ok)
{
    bool result = true;
    QPageLayout::Orientation orient;
    if (str == u8"PORTRAIT")
        orient = QPageLayout::Portrait;
    else if (str == u8"LANDSCAPE")
        orient = QPageLayout::Landscape;
    else
        result = false;
    if (ok)
        *ok = result;
    return orient;
}

QPagedPaintDevice::PageSize toPaperSize(const QString &str, bool *ok)
{
    static struct {
        const char *name;
        QPagedPaintDevice::PageSize value;
    } sizelist[] = {
        { "A0", QPagedPaintDevice::A0 },
        { "A1", QPagedPaintDevice::A1 },
        { "A2", QPagedPaintDevice::A2 },
        { "A3", QPagedPaintDevice::A3 },
        { "A4", QPagedPaintDevice::A4 },
        { "A5", QPagedPaintDevice::A5 },
        { "A6", QPagedPaintDevice::A6 },
        { "A7", QPagedPaintDevice::A7 },
        { "A8", QPagedPaintDevice::A8 },
        { "A9", QPagedPaintDevice::A9 },
        { "B0", QPagedPaintDevice::B0 },
        { "B1", QPagedPaintDevice::B1 },
        { "B2", QPagedPaintDevice::B2 },
        { "B3", QPagedPaintDevice::B3 },
        { "B4", QPagedPaintDevice::B4 },
        { "B5", QPagedPaintDevice::B5 },
        { "B6", QPagedPaintDevice::B6 },
        { "B7", QPagedPaintDevice::B7 },
        { "B8", QPagedPaintDevice::B8 },
        { "B9", QPagedPaintDevice::B9 },
        { "B10", QPagedPaintDevice::B10 },
        { "C5E", QPagedPaintDevice::C5E },
        { "Comm10E", QPagedPaintDevice::Comm10E },
        { "DLE", QPagedPaintDevice::DLE },
        { "Executive", QPagedPaintDevice::Executive },
        { "Folio", QPagedPaintDevice::Folio },
        { "Ledger", QPagedPaintDevice::Ledger },
        { "Legal", QPagedPaintDevice::Legal },
        { "Letter", QPagedPaintDevice::Letter },
    };
    static QMap<QString, QPagedPaintDevice::PageSize> sizeMap;

    if (sizeMap.isEmpty()) {
        for (size_t i = 0; i < sizeof(sizelist)/sizeof(sizelist[0]); ++i) {
            QString key = sizelist[i].name;
            sizeMap.insert(key, sizelist[i].value);
        }
    }

    bool result = sizeMap.contains(str);
    if (ok)
        *ok = result;
    return sizeMap.value(str, QPrinter::A4);
}

bool isPenName(const QString &str)
{
    if (str == u8"NORMALPEN" || str == u8"HEAVYPEN")
        return true;
    return false;
}

}
