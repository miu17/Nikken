#ifndef CALCULATIONMAKERCONFIGPARSER_H
#define CALCULATIONMAKERCONFIGPARSER_H

#include "calculationmakercommandinterface.h"


#include <QMap>
#include <QVariantList>
#include <QStringList>

namespace CalculationMakerLib{
struct CalculationMakerConfig;


/**
 * @brief The CalculationMakerConfigParser class
 *
 * \common{} コマンドの各項目の処理用クラス。
 */
class CalculationMakerConfigParser : public CalculationMakerCommandInterface
{
public:
    enum ArgumentType { Empty, Integer, Double, String, QuotedString, Alignment, Boolean, Orientation, PaperSize, Unknown };

    CalculationMakerConfigParser(const QString &type);
    QStringList supportedConfigs() const;
    virtual QVariant configValue(const QVariantList &args);
    bool parseConfig(CalculationMakerConfig *config, const QString &key, const QString &arg);
    static QVariant parseArg(const QString &arg, ArgumentType types, const QVariant &defaultValue, bool *ok = 0);
    static QVariantList parseArgs(const QString &args, const QList<ArgumentType> &types, const QVariantList &defaultValues, bool *ok = 0);
    static QVariantList parseArgs(const QStringList &args, const QList<ArgumentType> &types, const QVariantList &defaultValues, bool *ok = 0);
protected:
    void setSupportedConfigs(const QStringList &list);
    QList<ArgumentType> format() const;

    QStringList m_configs;
    QList<ArgumentType> m_formats;
    QMap<QString, QVariant> m_defaults;
};

#define SIMPLE_CONFIG_PARSER(classname) \
    class classname : public CalculationMakerConfigParser \
    { \
    public: \
        classname(); \
    };

SIMPLE_CONFIG_PARSER(CalculationMakerPageSizeConfig)
SIMPLE_CONFIG_PARSER(CalculationMakerOrientationConfig)
SIMPLE_CONFIG_PARSER(CalculationMakerRealConfig)
SIMPLE_CONFIG_PARSER(CalculationMakerTextConfig)

class CalculationMakerMarginConfig : public CalculationMakerConfigParser
{
public:
    CalculationMakerMarginConfig();
    QVariant configValue(const QVariantList &args);
};

class CalculationMakerTextStyleConfig : public CalculationMakerConfigParser
{
public:
    CalculationMakerTextStyleConfig();
    QVariant configValue(const QVariantList &args);
};

class CalculationMakerTitleStyleConfig : public CalculationMakerConfigParser
{
public:
    CalculationMakerTitleStyleConfig();
    QVariant configValue(const QVariantList &args);
};

class CalculationMakerContentsStyleConfig : public CalculationMakerConfigParser
{
public:
    CalculationMakerContentsStyleConfig();
    QVariant configValue(const QVariantList &args);
};

class CalculationMakerFormatConfig : public CalculationMakerConfigParser
{
public:
    CalculationMakerFormatConfig();
    QVariant configValue(const QVariantList &args);
};
} // namespace CalculationMakerLib

#endif // CALCULATIONMAKERCONFIGPARSER_H
