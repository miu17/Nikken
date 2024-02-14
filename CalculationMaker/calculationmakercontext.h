#ifndef CALCULATIONMAKERCONTEXT_H
#define CALCULATIONMAKERCONTEXT_H

#include "calculationmaker.h"

#include <QString>
#include <QMap>
#include <QStack>
#include <QPointF>
#include <QMargins>

class QPaitner;

namespace CalculationMakerLib{

class CalculationMakerDrawCommand;
class CalculationMakerSnippet;
class CalculationMakerConfigParser;

struct HeaderFooterFormat {
    qreal pos;
    QString family;
    qreal fontSize;
};

struct TextStyle {
    QString family;
    qreal fontSize;
    qreal leftindent;
    qreal rightindent;
    Qt::Alignment align;
    bool bookmark;
    bool contents;
};

struct CalculationMakerConfig
{
public:
    QMap<QString, QVariant> configValues;
    QMargins margin() const;
    TextStyle style(const QString &style) const;
    HeaderFooterFormat format(const QString &style) const;
};

struct CalculationMakerState
{
    QString blockName;
    TextStyle style;
    qreal linespacing;
    bool drawHeader;
    bool drawFooter;
    qreal m_penwidth;

//    QPrinterBookmarkNode *node;
};

struct CalculationMakerContentsNode
{
    int level;
    QString title;
    int pageno;
};

/**
 * @brief The CalculationMakerContext class
 *
 * 描画のメインとなるクラス。
 */
class CalculationMakerContext
{
public:
    CalculationMakerContext(CalculationMakerLib::CalculationMaker *maker, const post3dapp::OutputPaperSetting& setting);
    virtual ~CalculationMakerContext();

    /**
     * @brief The TextOption enum
     *
     * テキスト描画時のフラグ。
     */
    enum TextOption {
        None = 0,
        AllowInMarginArea = 0x001,
        SuperScript = 0x002,
        SubScript = 0x004,
        WithUnderline = 0x008,
        WithBox = 0x010,
        WithoutCarriageReturn = 0x020,
        OneLine = 0x040,
        Itemize = 0x080,
        IsNextPage = 0x8000, /* for internal use */
    };
    Q_DECLARE_FLAGS(TextOptions, TextOption)
private:
    void initContext();

public:
    bool hasCommonConfig() const;
    bool isPrinting() const;
    bool inBegin() const;
    bool hasContents() const;

    bool setConfig(CalculationMakerConfig *config, const QString &key, const QString &value);
    QString expandSnippet(const QString &text);
    bool draw(const QString &command, const QString &arg, int lineno);

    void beginPrinting();
    void endPrinting();

    void initState();

    void markToBookmark(const QString &title);
    void markToContents(const QString &title);

    int currentPage();
    void newPage(const post3dapp::PAPERSETTING &paper);
    void newPage();

    qreal lineHeight(const TextStyle &style);
    qreal currentLineHeight();
    void newLine();
    void newLine(qreal height);

    bool currentBlockEnabled() const;

    QString outputFilename() const;
    void setOutputFilename(const QString &name);
    bool beginBlock(const QString &name);
    bool endBlock(const QString &name);

    void setFont(const QString &font, qreal size);
    void setAlignment(Qt::Alignment align);
    void setLeftIndent(qreal value);
    void setRightIndent(qreal value);
    void setLineSpacing(qreal value);
    void setHeader(bool value);
    void setFooter(bool value);

    void setPen(const QString &pen);

    CalculationMakerConfig *config();

    post3dapp::PAPERSETTING configPaper() const;

    QPointF pos() const;
    QPointF currentPos() const;
    QPoint painterPos(const QPointF &src) const;
    QPointF paperPos(const QPoint &src) const;

    void applyTextConfig(const QString &type);

    const TextStyle &currentStyle() const;
    qreal textWidth(const TextStyle &style, const QString &text);
    QString drawText(const TextStyle &style, const QPointF &pos, const QString &text, TextOptions opt, QPointF *nextPos = 0);
    void drawText(const TextStyle &style, const QString &text, TextOptions opt = None);

    void drawRect(const QRectF &rect);
    void drawLine(const QPointF &p0, const QPointF &p1);
    void drawImage(const QRectF &rect, const QImage &image);

    void drawHeaderFooter();

    void setPos(const QPointF &pos);

    void setFigureCommand(FigureInterface *cmd);
    void setTableCommand(TableInterface *cmd);

    void drawFigure(const QString &figure, const QPointF &pos, qreal width, qreal height);
    void drawTable(const QString &table, Qt::Alignment align, qreal width);

    void drawContents();

    int currentChapter() const;
    int currentSection() const;
    int currentSubSection() const;

    void nextChapter();
    void nextSection();
    void nextSubSection();

protected:
    void addDrawCommand(CalculationMakerDrawCommand *cmd);
    void addSnippetCommand(CalculationMakerSnippet *cmd);
    void addConfigCommand(CalculationMakerConfigParser *cmd);

protected:
    CalculationMakerState m_state;

    QPointF m_pos;
    int m_pageno;
    post3dapp::PAPERSETTING m_currentPaper;
    QRect m_currentPaperRectPixel;
    QRectF m_currentPaperRectMiliMeter;

    CalculationMaker *m_maker;
    QPainter *m_painter;
    QPrinter *m_printer;
//    QPrinterBookmark *m_bookmark;
    QString m_filename;

    CalculationMakerConfig m_config;
    QStack<CalculationMakerState> m_stateStack;
    bool m_enabled;

    int m_chapter;
    int m_section;
    int m_subsection;
    QList<CalculationMakerContentsNode> m_contentsNodes;

protected:
    QMap<QString, CalculationMakerDrawCommand *> m_drawCommands;
    QMap<QString, CalculationMakerSnippet *> m_snippetCommands;
    QMap<QString, CalculationMakerConfigParser *> m_configCommands;

    QMap<QString, FigureInterface *> m_figureCommands;
    QMap<QString, TableInterface *> m_tableCommands;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CalculationMakerContext::TextOptions)
} // namespace CalculationMakerLib

#endif // CALCULATIONMAKERCONTEXT_H
