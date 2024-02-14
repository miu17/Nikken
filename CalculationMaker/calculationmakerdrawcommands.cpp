#include "calculationmakerdrawcommands.h"

#include "calculationmaker.h"
#include "calculationmakerconfigparser.h"
#include "calculationmakerutils.h"
#include "calculationmakercontext.h"

#include <QPainter>
#include <QFontMetrics>

#include <QStringList>
#include <QRegExp>
#include <QDebug>

#include <QCoreApplication>

#define DEBUG_DRAWCOMMAND 1
namespace CalculationMakerLib{

CalculationMakerDrawCommand::CalculationMakerDrawCommand(const QString &command) :
    CalculationMakerCommandInterface(command)
{
}

bool CalculationMakerDrawCommand::preCheck(CalculationMakerLib::CalculationMaker *maker)
{
    CalculationMakerContext *context = maker->context();
    if (!context->hasCommonConfig()) {
        setError(QCoreApplication::translate("CalculationMakerDrawCommand", "Command \\common is needed"));
        return false;
    }
    if (!context->isPrinting()) {
        QString err = QCoreApplication::translate("CalculationMakerDrawCommand", "%1: should be used between \\begin{} and \\end{}").arg(command());
        setError(err);
        return false;
    }
    return true;
}

bool CalculationMakerDrawCommand::isEnabled(CalculationMakerLib::CalculationMaker *maker)
{
    return maker->context()->currentBlockEnabled();
}

bool CalculationMakerDrawCommand::draw(CalculationMaker *maker, const QString &arg)
{
#ifdef DEBUG_DRAWCOMMAND
    qDebug("Command: %s", qPrintable(command()));
#endif
    if (!preCheck(maker))
        return false;
    if (!isEnabled(maker))
        return true;
    return drawContent(maker, arg);
}

CalculationMakerCommonCommand::CalculationMakerCommonCommand() :
    CalculationMakerDrawCommand(u8"\\common")
{
}

bool CalculationMakerCommonCommand::preCheck(CalculationMakerLib::CalculationMaker *maker)
{
    if (maker->context()->hasCommonConfig()) {
        setError(QCoreApplication::translate("CalculationMakerCommonCommand", "Command \\common should be first command"));
        return false;
    }
    return true;
}

bool CalculationMakerCommonCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    m_commonValues.clear();
    QRegExp regexp("^([\\w][\\w\\d]*)\\s*:\\s*([^\\s].*)$");
    QStringList arglist = arg.split(QLatin1Char('\n'));
    CalculationMakerConfig *config = maker->context()->config();
    foreach (const QString &buf, arglist) {
        QString s = buf.trimmed();
        if (s.isEmpty())
            continue;
        if (regexp.exactMatch(s)) {
            Q_ASSERT(regexp.captureCount() == 2);
            QString key = regexp.cap(1);
            QString value = regexp.cap(2);
            m_commonValues.insert(key, value);
            bool result = maker->context()->setConfig(config, key, value);
            if (!result) {
                setError(QCoreApplication::translate("CalculationMakerCommonCommand", "Configuration Error"));
                return false;
            }
        } else {
            setError(QCoreApplication::translate("CalculationMakerCommonCommand", "Invalid format: '%1'").arg(buf));
            return false;
        }
    }
    return true;
}

CalculationMakerBeginCommand::CalculationMakerBeginCommand() :
    CalculationMakerDrawCommand(u8"\\begin")
{
}

bool CalculationMakerBeginCommand::preCheck(CalculationMakerLib::CalculationMaker *maker)
{
    CalculationMakerContext *context = maker->context();
    if (!context->hasCommonConfig()) {
        setError(QCoreApplication::translate("CalculationMakerBeginCommand", "Command \\common is needed"));
        return false;
    }
    return true;
}

bool CalculationMakerBeginCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    return maker->context()->beginBlock(arg);
}

CalculationMakerEndCommand::CalculationMakerEndCommand() :
    CalculationMakerDrawCommand(u8"\\end")
{
}

bool CalculationMakerEndCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    return maker->context()->endBlock(arg);
}

CalculationMakerStyledTextCommand::CalculationMakerStyledTextCommand(const QString &command) :
    CalculationMakerDrawCommand(command)
{
    m_style = command.mid(1);
}

bool CalculationMakerStyledTextCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    CalculationMakerContext *context = maker->context();
    TextStyle style = context->config()->style(m_style);
    QString text = arg;
    if (m_style == u8"chapter") {
        context->nextChapter();
        text = QString::fromLocal8Bit("§%1 %2").arg(context->currentChapter()).arg(text);
    } else if (m_style == u8"section") {
        context->nextSection();
        text = QString::fromLocal8Bit("%1-%2 %3").arg(context->currentChapter()).arg(context->currentSection()).arg(text);
    } else if (m_style == u8"subsection") {
        context->nextSubSection();
        text = QString::fromLocal8Bit("%1-%2-%3 %4").arg(context->currentChapter()).arg(context->currentSection()).arg(context->currentSubSection()).arg(text);
    }
    context->drawText(style, text);
    if (style.bookmark) {
        context->markToBookmark(text);
    }
    if (style.contents) {
        context->markToContents(text);
    }
    return true;
}

bool CalculationMakerTextCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    maker->context()->drawText(maker->context()->currentStyle(), arg);
    return true;
}

bool CalculationMakerItemCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    maker->context()->drawText(maker->context()->currentStyle(), arg, CalculationMakerContext::Itemize);
    return true;
}

bool CalculationMakerNewPageCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    if (arg.isEmpty()) {
        maker->context()->newPage();
        return true;
    }
    QString group = maker->figureToGroup.value(arg);
    if (group.isEmpty()) {
        setError("Invalid argument");
        return false;
    }
    post3dapp::PAPERSETTING paper = maker->groupFormats.value(group);
    maker->context()->newPage(paper);
    return true;
}

bool CalculationMakerFontCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    CalculationMakerContext *context = maker->context();
    bool result = true;
    QList<CalculationMakerConfigParser::ArgumentType> types;
    QVariantList defaultValues;
    types << CalculationMakerConfigParser::QuotedString
          << CalculationMakerConfigParser::Double;
    TextStyle style = context->config()->style(u8"normal");
    defaultValues << style.family << style.fontSize;
    QVariantList values = CalculationMakerConfigParser::parseArgs(arg, types, defaultValues, &result);
    if (result)
        context->setFont(values.at(0).toString(), values.at(1).toDouble());
    else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerAlignmentCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result = true;
    Qt::Alignment align;
    if (arg.isEmpty())
        align = maker->context()->config()->style(u8"normal").align;
    else
        align = CalculationMakerUtils::toAlignment(arg, &result);
    if (result)
        maker->context()->setAlignment(align);
    else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerIndentLCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result = true;
    qreal indent;
    if (arg.isEmpty())
        indent = maker->context()->config()->style(u8"normal").leftindent;
    else
        indent = arg.toDouble(&result);
    if (result)
        maker->context()->setLeftIndent(indent);
    else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerIndentRCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result = true;
    qreal indent;
    if (arg.isEmpty())
        indent = maker->context()->config()->style(u8"normal").rightindent;
    else
        indent = arg.toDouble(&result);
    if (result)
        maker->context()->setRightIndent(indent);
    else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerLineSpacingCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result = true;
    qreal value;
    if (arg.isEmpty())
        value = maker->context()->config()->configValues.value(u8"linespacing").toDouble(&result);
    else
        value = arg.toDouble(&result);
    if (result)
        maker->context()->setLineSpacing(value);
    else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerHeaderCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result;
    bool value;
    if (arg.isEmpty())
        result = value = true;
    else
        value = CalculationMakerUtils::toBool(arg, &result);
    if (result)
        maker->context()->setHeader(value);
    else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerFooterCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result;
    bool value;
    if (arg.isEmpty())
        result = value = true;
    else
        value = CalculationMakerUtils::toBool(arg, &result);
    if (result)
        maker->context()->setFooter(value);
    else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerSetPenCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    QString pen;
    if (arg.isEmpty())
        pen = "NORMALPEN";
    else
        pen = arg;
    bool result = CalculationMakerUtils::isPenName(pen);
    if (result)
        maker->context()->setPen(pen);
    else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerRectCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    const int argCount = 4;
    bool result = true;
    QList<CalculationMakerConfigParser::ArgumentType> types;
    for (int i = 0; i < argCount; ++i)
        types << CalculationMakerConfigParser::Double;
    QVariantList values = CalculationMakerConfigParser::parseArgs(arg, types, QVariantList(), &result);
    if (result) {
        QRectF rect(values.at(0).toDouble(), values.at(1).toDouble(),
                    values.at(2).toDouble(), values.at(3).toDouble());
        maker->context()->drawRect(rect);
    } else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerLineCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result = true;
    QStringList list = CalculationMakerUtils::splitString(arg);
    QList<double> dlist;
    if (list.count() == 4) {
        foreach (const QString &s, list) {
            dlist << s.toDouble(&result);
            if (!result)
                break;
        }
    } else
        result = false;
    if (result) {
        QPointF p0(dlist.at(0), dlist.at(1));
        QPointF p1(dlist.at(2), dlist.at(3));
        maker->context()->drawLine(p0, p1);
    } else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerNewLineCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result = true;
    if (arg.isEmpty()) {
        maker->context()->newLine();
    } else {
        double height = arg.toDouble(&result);
        if (result)
            maker->context()->newLine(height);
        else
            setError("Invalid argument");
    }
    return result;
}

bool CalculationMakerVSpaceCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result = true;
    double vs = arg.toDouble(&result);
    if (result) {
        QPointF pos = maker->context()->pos();
        pos.setY(pos.y() + vs);
        maker->context()->setPos(pos);
    } else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerHSpaceCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    bool result = true;
    double hs = arg.toDouble(&result);
    if (result) {
        QPointF pos = maker->context()->currentPos();
        if (pos.x() >= 0)
            pos.setX(pos.x() + hs);
        else {
            qreal x = maker->context()->config()->margin().left() +
                    maker->context()->currentStyle().leftindent + hs;
            pos.setX(x);
        }
        maker->context()->setPos(pos);
    } else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerMoveXYCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    CalculationMakerContext *context = maker->context();
    bool result = true;
    QList<CalculationMakerConfigParser::ArgumentType> types;
    QVariantList defaultValues;
    types << CalculationMakerConfigParser::Double
          << CalculationMakerConfigParser::Double;
    QPointF pos = maker->context()->pos();
    defaultValues << pos.x() << pos.y();
    QVariantList values = CalculationMakerConfigParser::parseArgs(arg, types, defaultValues, &result);
    if (result){
        QPointF p(values.at(0).toDouble(), values.at(1).toDouble());
        context->setPos(p);
    } else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerMoveRXYCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    CalculationMakerContext *context = maker->context();
    bool result = true;
    QMargins margin = context->config()->margin();
    QList<CalculationMakerConfigParser::ArgumentType> types;
    QVariantList defaultValues;
    types << CalculationMakerConfigParser::Double
          << CalculationMakerConfigParser::Double;
    QPointF pos = maker->context()->pos();
    qreal x = pos.x();
    if (x >= 0)
        x -= margin.left();
    defaultValues << x << pos.y() - margin.top();
    QVariantList values = CalculationMakerConfigParser::parseArgs(arg, types, defaultValues, &result);
    if (result){
        x = values.at(0).toDouble();
        if (x >= 0)
            x += margin.left();
        QPointF p(x, values.at(1).toDouble() + margin.top());
        context->setPos(p);
    } else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerUnderLineCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    maker->context()->drawText(maker->context()->currentStyle(), arg, CalculationMakerContext::WithUnderline);
    return true;
}

bool CalculationMakerFboxCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    maker->context()->drawText(maker->context()->currentStyle(), arg, CalculationMakerContext::WithBox);
    return true;
}

bool CalculationMakerSuperScriptCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    maker->context()->drawText(maker->context()->currentStyle(), arg, CalculationMakerContext::SuperScript);
    return true;
}

bool CalculationMakerSubScriptCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    maker->context()->drawText(maker->context()->currentStyle(), arg, CalculationMakerContext::SubScript);
    return true;
}

bool CalculationMakerIncludeFigureCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    CalculationMakerContext *context = maker->context();
    bool result = true;
    QList<CalculationMakerConfigParser::ArgumentType> types;
    types << CalculationMakerConfigParser::String
          << CalculationMakerConfigParser::Double
          << CalculationMakerConfigParser::Double
          << CalculationMakerConfigParser::Double
          << CalculationMakerConfigParser::Double;
    QVariantList defaultValues;
    defaultValues << QVariant()
                  << QVariant()
                  << QVariant()
                  << 0
                  << 0;
    QVariantList values = CalculationMakerConfigParser::parseArgs(arg, types, defaultValues, &result);
    QPointF pos;
    QString key;
    qreal width;
    qreal height;
    if (result) {
        key = values.at(0).toString();
        pos = QPointF(values.at(1).toDouble(), values.at(2).toDouble());
        width = values.at(3).toDouble();
        height = values.at(4).toDouble();
        if (pos.x() < 0 || pos.y() < 0 || key.isEmpty() || !maker->figureToGroup.contains(key)) {
            result = false;
        }
    }
    if (result) {
        context->drawFigure(key, pos, width, height);
    } else {
        setError("Invalid argument");
    }
    return result;
}

bool CalculationMakerIncludeTableCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    CalculationMakerContext *context = maker->context();
    bool result = true;
    QList<CalculationMakerConfigParser::ArgumentType> types;
    types << CalculationMakerConfigParser::String
          << CalculationMakerConfigParser::Alignment
          << CalculationMakerConfigParser::Double;
    QVariantList values = CalculationMakerConfigParser::parseArgs(arg, types, QVariantList(), &result);
    if (result) {
        QString key = values.at(0).toString();
        Qt::Alignment align = static_cast<Qt::Alignment>(values.at(1).toInt());
        qreal width = values.at(2).toDouble();
        context->drawTable(key, align, width);
    } else {
        setError("Invalid argument");
    }
    return result;
}

bool CalculationMakerIncludeTextCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    maker->context()->drawText(maker->context()->currentStyle(), maker->textSettings.value(arg));
    return true;
}

bool CalculationMakerIncludeLineTextCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    CalculationMakerContext *context = maker->context();
    bool result = true;
    QList<CalculationMakerConfigParser::ArgumentType> types;
    QVariantList defaultValues;
    types << CalculationMakerConfigParser::String
          << CalculationMakerConfigParser::Double;
    defaultValues << QVariant() << -1;
    QVariantList values = CalculationMakerConfigParser::parseArgs(arg, types, defaultValues, &result);
    QString key = values.at(0).toString();
    if (key.isEmpty())
        result = false;
    if (result) {
        QString text = maker->textSettings.value(key);
        qreal width = values.at(1).toDouble();
        TextStyle style = context->currentStyle();
        if (width < 0) {
            context->drawText(style, text, CalculationMakerContext::OneLine);
        } else {
            qreal w = context->textWidth(style, text);
            int n = 0;
            while (w > width) {
                qreal f = style.fontSize * width / w;
                if (f == style.fontSize || n > 10 || style.fontSize < 1.0) {
                    setError("the width is too narrow!");
                    return false;
                }
                style.fontSize = f;
                w = context->textWidth(style, text);
                ++n;
            }
            context->drawText(style, text, CalculationMakerContext::OneLine);
        }
    } else
        setError("Invalid argument");
    return result;
}

bool CalculationMakerPictureCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    CalculationMakerContext *context = maker->context();
    bool result = true;
    QList<CalculationMakerConfigParser::ArgumentType> types;
    types << CalculationMakerConfigParser::Double
          << CalculationMakerConfigParser::Double
          << CalculationMakerConfigParser::Double
          << CalculationMakerConfigParser::Double
          << CalculationMakerConfigParser::QuotedString;
    QVariantList values = CalculationMakerConfigParser::parseArgs(arg, types, QVariantList(), &result);
    if (result) {
        QRectF rect(values.at(0).toDouble(), values.at(1).toDouble(),
                    values.at(2).toDouble(), values.at(3).toDouble());
        QString file = values.at(4).toString();
        QImage image(file);
        if (!image.isNull())
            context->drawImage(rect, image);
        else {
            setError("No such file or directory");
            result = false;
        }
    } else
        setError("Invalid argument");
    return result;
}

CalculationMakerContentsCommand::CalculationMakerContentsCommand() :
    CalculationMakerDrawCommand(u8"\\contents")
{
}

bool CalculationMakerContentsCommand::preCheck(CalculationMakerLib::CalculationMaker *maker, const post3dapp::OutputPaperSetting& setting)
{
    CalculationMakerContext *context = maker->context();
    return context->isPrinting();
}

bool CalculationMakerContentsCommand::drawContent(CalculationMaker *maker, const QString &arg)
{
    if (!arg.isEmpty()) {
        setError("Invalid argument");
        return false;
    }
    maker->context()->drawContents();
    return true;
}
} // namespace CalculationMakerLib
