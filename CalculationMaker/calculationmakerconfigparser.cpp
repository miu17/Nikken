#include "calculationmakerconfigparser.h"

#include <QPrinter>

#include <QDebug>

#include "calculationmaker.h"
#include "calculationmakerutils.h"
#include "calculationmakercontext.h"

Q_DECLARE_METATYPE(Qt::Alignment)
Q_DECLARE_METATYPE(QMargins)
Q_DECLARE_METATYPE(CalculationMakerLib::TextStyle)
Q_DECLARE_METATYPE(CalculationMakerLib::HeaderFooterFormat)

using namespace CalculationMakerUtils;
namespace CalculationMakerLib{

CalculationMakerConfigParser::CalculationMakerConfigParser(const QString &type) :
    CalculationMakerCommandInterface(type)
{
}

QStringList CalculationMakerConfigParser::supportedConfigs() const
{
    return m_configs;
}

QVariant CalculationMakerConfigParser::configValue(const QVariantList &args)
{
    if (args.count() == 1)
        return args.at(0);
    return args;
}

bool CalculationMakerConfigParser::parseConfig(CalculationMakerConfig *config, const QString &key, const QString &arg)
{
    Q_ASSERT(supportedConfigs().contains(key));
    Q_ASSERT(!m_formats.isEmpty());
    bool result = true;
    if (config->configValues.contains(key)) {
        setError(u8"Duplicated config");
        return false;
    }
    QVariantList args;
    if (m_formats.count() == 1)
        args << parseArg(arg, m_formats.at(0), m_defaults.value(key), &result);
    else
        args = parseArgs(arg, m_formats, m_defaults.value(key).toList(), &result);
    if (result) {
        QVariant value = configValue(args);
        if (value.isValid())
            config->configValues.insert(key, value);
        else
            result = false;
    } else
        setError(u8"Invalid Argument");
    Q_ASSERT(result && config->configValues.contains(key));
    return result;
}

QVariant CalculationMakerConfigParser::parseArg(const QString &arg, CalculationMakerConfigParser::ArgumentType type, const QVariant &defaultValue, bool *ok)
{
    bool result = true;
    if (arg.isEmpty() || arg == u8"*") {
        if (!defaultValue.isValid())
            result = false;
        if (ok)
            *ok = result;
        return defaultValue;
    }
    QVariant val;
    switch (type) {
    case String:
        val = arg;
        break;
    case Integer:
        val = arg.toInt(&result);
        break;
    case Double:
        val = arg.toDouble(&result);
        break;
    case QuotedString:
        result = isQuotedString(arg);
        val = trimQuote(arg);
        break;
    case Alignment:
        val = static_cast<int>(toAlignment(arg, &result));
        break;
    case Boolean:
        val = toBool(arg, &result);
        break;
    case Orientation:
        val = toOrientation(arg, &result);
        break;
    case PaperSize:
        val = toPaperSize(arg, &result);
        break;
    case Empty:
        result = arg.isEmpty();
        break;
    default:
        qWarning("Unknwon Type: %s", arg.toUtf8().constData());
    }
    if (ok)
        *ok = result;
    return val;
}

QVariantList CalculationMakerConfigParser::parseArgs(const QStringList &args, const QList<CalculationMakerConfigParser::ArgumentType> &types, const QVariantList &defaultValues, bool *ok)
{
    QVariantList values;
    bool result = true;
    Q_ASSERT(types.count() > 0);
    if (args.count() > types.count() || (args.count() < types.count() && defaultValues.count() < types.count())) {
        if (ok)
            *ok = false;
        return values;
    }
    for (int i = 0; i < types.count(); ++i) {
        QString a = args.value(i);
        ArgumentType t = types.at(i);
        QVariant d = defaultValues.value(i);
        QVariant v = parseArg(a, t, d, &result);
        if (!result)
            break;
        values << v;
    }
    if (ok)
        *ok = result;
    Q_ASSERT(result && types.count() == values.count());
    return values;
}

QVariantList CalculationMakerConfigParser::parseArgs(const QString &args, const QList<CalculationMakerConfigParser::ArgumentType> &types, const QVariantList &defaultValues, bool *ok)
{
    QStringList list = splitString(args);
    return parseArgs(list, types, defaultValues, ok);
}

void CalculationMakerConfigParser::setSupportedConfigs(const QStringList &list)
{
    m_configs = list;
}

CalculationMakerPageSizeConfig::CalculationMakerPageSizeConfig() :
    CalculationMakerConfigParser(u8"PaperSize")
{
    m_formats << PaperSize;
    m_defaults.insert(u8"pagesize", QPagedPaintDevice::A4);
    setSupportedConfigs(m_defaults.keys());
}

CalculationMakerOrientationConfig::CalculationMakerOrientationConfig() :
    CalculationMakerConfigParser(u8"Orientation")
{
    m_formats << Orientation;
    m_defaults.insert(u8"pageorientation", QPageLayout::Portrait);
    setSupportedConfigs(m_defaults.keys());
}

CalculationMakerMarginConfig::CalculationMakerMarginConfig() :
    CalculationMakerConfigParser(u8"Margin")
{
    const int argCount = 4;
    QVariantList defaultValues;
    for (int i = 0; i < argCount; ++i) {
        m_formats << Double;
        defaultValues << 10.0;
    }
    m_defaults.insert(u8"margin", defaultValues);
    setSupportedConfigs(m_defaults.keys());
}

QVariant CalculationMakerMarginConfig::configValue(const QVariantList &args)
{
    QMargins margin(args.at(0).toDouble(), args.at(1).toDouble(),
                    args.at(2).toDouble(), args.at(3).toDouble());
    return QVariant::fromValue<QMargins>(margin);
}

CalculationMakerRealConfig::CalculationMakerRealConfig() :
    CalculationMakerConfigParser(u8"Real")
{
    m_formats << Double;
    m_defaults.insert(u8"linespacing", 0.0);
    m_defaults.insert(u8"tablerowheight", 5.0);
    m_defaults.insert(u8"normalpen", 0.1);
    m_defaults.insert(u8"heavypen", 0.5);
    setSupportedConfigs(m_defaults.keys());
}

CalculationMakerTextConfig::CalculationMakerTextConfig() :
    CalculationMakerConfigParser(u8"Text")
{
    m_formats << String;
    setSupportedConfigs(QStringList()
                        << u8"headerL"
                        << u8"headerR"
                        << u8"footerL"
                        << u8"footerR"
                        );
}

CalculationMakerTextStyleConfig::CalculationMakerTextStyleConfig() :
    CalculationMakerConfigParser(u8"TextStyle")
{
    m_formats << QuotedString << Double << Double << Double << Alignment;
    QVariantList defaultValues;
    Qt::Alignment defaultAlignment = Qt::AlignLeft | Qt::AlignTop;
    defaultValues << u8"\"Times New Roman\""
                     << 10.0 << 0.0 << 0.0
                     << static_cast<int>(defaultAlignment);
    m_defaults.insert(u8"normal", defaultValues);
    defaultValues.replace(1, 16.0);
    m_defaults.insert(u8"title", defaultValues);
    defaultValues.replace(1, 12.0);
    m_defaults.insert(u8"subtitle", defaultValues);
    setSupportedConfigs(m_defaults.keys());
}

QVariant CalculationMakerTextStyleConfig::configValue(const QVariantList &args)
{
    int idx = 0;
    TextStyle style;
    style.family = args.at(idx++).toString();
    style.fontSize = args.at(idx++).toDouble();
    style.leftindent = args.at(idx++).toDouble();
    style.rightindent = args.at(idx++).toDouble();
    style.align = static_cast<Qt::Alignment>(args.at(idx++).toInt());
    style.bookmark = false;
    style.contents = false;
    return QVariant::fromValue<TextStyle>(style);
}

CalculationMakerTitleStyleConfig::CalculationMakerTitleStyleConfig() :
    CalculationMakerConfigParser(u8"TitleStyle")
{
    m_formats << QuotedString << Double << Double << Alignment << Boolean << Boolean;
    QVariantList defaultValues;
    Qt::Alignment defaultAlignment = Qt::AlignLeft | Qt::AlignTop;
    defaultValues << u8"\"Times New Roman\""
                     << 16.0 << 0.0
                     << static_cast<int>(defaultAlignment)
                     << true << true;
    m_defaults.insert(u8"chapter", defaultValues);
    defaultValues.replace(1, 14.0);
    m_defaults.insert(u8"section", defaultValues);
    defaultValues.replace(1, 12.0);
    m_defaults.insert(u8"subsection", defaultValues);
    setSupportedConfigs(m_defaults.keys());
}

QVariant CalculationMakerTitleStyleConfig::configValue(const QVariantList &args)
{
    int idx = 0;
    TextStyle style;
    style.family = args.at(idx++).toString();
    style.fontSize = args.at(idx++).toDouble();
    style.leftindent = args.at(idx++).toDouble();
    style.rightindent = 0;
    style.align = static_cast<Qt::Alignment>(args.at(idx++).toInt());
    style.bookmark = args.at(idx++).toBool();
    style.contents = args.at(idx++).toBool();
    return QVariant::fromValue<TextStyle>(style);
}

CalculationMakerContentsStyleConfig::CalculationMakerContentsStyleConfig() :
    CalculationMakerConfigParser(u8"ContentsStyle")
{
    m_formats << QuotedString << Double;
    QVariantList defaultValues;
    defaultValues << u8"\"Times New Roman\"" << 10.0;
    m_defaults.insert(u8"contents", defaultValues);
    setSupportedConfigs(m_defaults.keys());
}

QVariant CalculationMakerContentsStyleConfig::configValue(const QVariantList &args)
{
    int idx = 0;
    TextStyle style;
    style.family = args.at(idx++).toString();
    style.fontSize = args.at(idx++).toDouble();
    style.leftindent = 0;
    style.rightindent = 0;
    style.align = Qt::AlignLeft | Qt::AlignTop;
    style.bookmark = false;
    style.contents = false;
    return QVariant::fromValue<TextStyle>(style);
}

CalculationMakerFormatConfig::CalculationMakerFormatConfig() :
    CalculationMakerConfigParser("Format")
{
    m_formats << Double << QuotedString << Double;
    QVariantList defaultValues;
    defaultValues << 10.0 << u8"\"Times New Roman\"" << 8.0;
    m_defaults.insert(u8"headerformat", defaultValues);
    m_defaults.insert(u8"footerformat", defaultValues);
    setSupportedConfigs(m_defaults.keys());
}

QVariant CalculationMakerFormatConfig::configValue(const QVariantList &args)
{
    int idx = 0;
    HeaderFooterFormat format;
    format.pos = args.at(idx++).toDouble();
    format.family = args.at(idx++).toString();
    format.fontSize = args.at(idx++).toDouble();
    return QVariant::fromValue<HeaderFooterFormat>(format);
}
} // namespace CalculationMakerLib
