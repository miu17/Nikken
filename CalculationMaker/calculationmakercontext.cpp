#include "calculationmakercontext.h"

#include "calculationmakerutils.h"

#include "calculationmakerdrawcommands.h"
#include "calculationmakersnippet.h"
#include "calculationmakerconfigparser.h"

#include "figureinterface.h"
#include "tableinterface.h"

#include <QRegExp>

#include <QPrinter>
#include <QPainter>
//#include <QPrinterBookmark>

#include <QFontMetrics>
#include <QTextLayout>

#include <QDebug>

Q_DECLARE_METATYPE(QMargins)
Q_DECLARE_METATYPE(CalculationMakerLib::TextStyle)
Q_DECLARE_METATYPE(CalculationMakerLib::HeaderFooterFormat)

using namespace CalculationMakerUtils;
namespace CalculationMakerLib{


QMargins CalculationMakerConfig::margin() const
{
    return configValues.value("margin").value<QMargins>();
}

TextStyle CalculationMakerConfig::style(const QString &style) const
{
    return configValues.value(style).value<TextStyle>();
}

HeaderFooterFormat CalculationMakerConfig::format(const QString &style) const
{
    return configValues.value(style).value<HeaderFooterFormat>();
}

/**
 * @brief CalculationMakerContext::CalculationMakerContext
 * @param maker
 *
 * コンストラクタ。
 *
 * 各コマンドの初期化など。
 */
CalculationMakerContext::CalculationMakerContext(CalculationMakerLib::CalculationMaker *maker, const post3dapp::OutputPaperSetting& setting) :
    m_maker(maker)
{
    initContext();

    addDrawCommand(new CalculationMakerCommonCommand);
    addDrawCommand(new CalculationMakerBeginCommand);
    addDrawCommand(new CalculationMakerEndCommand);

    addDrawCommand(new CalculationMakerTextCommand);
    addDrawCommand(new CalculationMakerItemCommand);
    addDrawCommand(new CalculationMakerNewPageCommand);

    addDrawCommand(new CalculationMakerFontCommand);
    addDrawCommand(new CalculationMakerAlignmentCommand);
    addDrawCommand(new CalculationMakerIndentLCommand);
    addDrawCommand(new CalculationMakerIndentRCommand);
    addDrawCommand(new CalculationMakerLineSpacingCommand);
    addDrawCommand(new CalculationMakerHeaderCommand);
    addDrawCommand(new CalculationMakerFooterCommand);

    addDrawCommand(new CalculationMakerSetPenCommand);
    addDrawCommand(new CalculationMakerRectCommand);
    addDrawCommand(new CalculationMakerLineCommand);

    addDrawCommand(new CalculationMakerNewLineCommand);
    addDrawCommand(new CalculationMakerVSpaceCommand);
    addDrawCommand(new CalculationMakerHSpaceCommand);
    addDrawCommand(new CalculationMakerMoveXYCommand);
    addDrawCommand(new CalculationMakerMoveRXYCommand);

    addDrawCommand(new CalculationMakerUnderLineCommand);
    addDrawCommand(new CalculationMakerFboxCommand);

    addDrawCommand(new CalculationMakerSuperScriptCommand);
    addDrawCommand(new CalculationMakerSubScriptCommand);

    addDrawCommand(new CalculationMakerStyledTextCommand("\\normal"));
    addDrawCommand(new CalculationMakerStyledTextCommand("\\title"));
    addDrawCommand(new CalculationMakerStyledTextCommand("\\subtitle"));
    addDrawCommand(new CalculationMakerStyledTextCommand("\\chapter"));
    addDrawCommand(new CalculationMakerStyledTextCommand("\\section"));
    addDrawCommand(new CalculationMakerStyledTextCommand("\\subsection"));

    addDrawCommand(new CalculationMakerIncludeFigureCommand);
    addDrawCommand(new CalculationMakerIncludeTableCommand);
    addDrawCommand(new CalculationMakerIncludeTextCommand);
    addDrawCommand(new CalculationMakerIncludeLineTextCommand);
    addDrawCommand(new CalculationMakerPictureCommand);

    addDrawCommand(new CalculationMakerContentsCommand);

    addSnippetCommand(new CalculationMakerPgSnippet);
    addSnippetCommand(new CalculationMakerDateSnippet);
    addSnippetCommand(new CalculationMakerProjectNameSnippet);
    addSnippetCommand(new CalculationMakerStructureTypeSnippet);

    addConfigCommand(new CalculationMakerPageSizeConfig);
    addConfigCommand(new CalculationMakerOrientationConfig);
    addConfigCommand(new CalculationMakerMarginConfig);
    addConfigCommand(new CalculationMakerRealConfig);
    addConfigCommand(new CalculationMakerTextStyleConfig);
    addConfigCommand(new CalculationMakerTitleStyleConfig);
    addConfigCommand(new CalculationMakerContentsStyleConfig);
    addConfigCommand(new CalculationMakerTextConfig);
    addConfigCommand(new CalculationMakerFormatConfig);

}

void CalculationMakerContext::initContext()
{
    m_painter = 0;
    m_printer = 0;
    //m_bookmark = 0;

    m_pos = QPointF(0, 0);
    m_pageno = -1;
    m_enabled = true;

    m_chapter = 0;
    m_section = 0;
    m_subsection = 0;
}

CalculationMakerContext::~CalculationMakerContext()
{
    qDeleteAll(m_drawCommands);
    qDeleteAll(m_snippetCommands);
    qDeleteAll(m_configCommands);
    qDeleteAll(m_figureCommands);
    //delete m_bookmark;
}

bool CalculationMakerContext::hasCommonConfig() const
{
    return !m_config.configValues.isEmpty();
}

bool CalculationMakerContext::isPrinting() const
{
    return m_printer != 0 && m_printer->printerState() != QPrinter::Error;
}

bool CalculationMakerContext::inBegin() const
{
    return m_stateStack.count() > 1;
}

bool CalculationMakerContext::hasContents() const
{
    return m_contentsNodes.count() > 0;
}

void CalculationMakerContext::addDrawCommand(CalculationMakerDrawCommand *cmd)
{
    m_drawCommands.insert(cmd->command(), cmd);
}

void CalculationMakerContext::addSnippetCommand(CalculationMakerSnippet *cmd)
{
    m_snippetCommands.insert(cmd->command(), cmd);
}

void CalculationMakerContext::addConfigCommand(CalculationMakerConfigParser *cmd)
{
    m_configCommands.insert(cmd->command(), cmd);
}

void CalculationMakerContext::setFigureCommand(FigureInterface *cmd)
{
    m_figureCommands.insert(cmd->command(), cmd);
}

void CalculationMakerContext::setTableCommand(TableInterface *cmd)
{
    m_tableCommands.insert(cmd->command(), cmd);
}

/**
 * @brief CalculationMakerContext::setConfig
 * @param config
 * @param key
 * @param value
 * @return
 *
 * \common{} コマンド用。
 *
 */
bool CalculationMakerContext::setConfig(CalculationMakerConfig *config, const QString &key, const QString &value)
{
    QMap<QString, CalculationMakerConfigParser *>::const_iterator i = m_configCommands.constBegin();
    while (i != m_configCommands.constEnd()) {
        CalculationMakerConfigParser *cmd = i.value();
        if (cmd->supportedConfigs().contains(key)) {
            bool result = cmd->parseConfig(config, key, value);
            if (!result) {
                qWarning("\\common: Invalid config: %s: %s\n%s", key.toUtf8().constData(), value.toUtf8().constData(), cmd->errorString().toUtf8().constData());
            }
            return result;
        }
        ++i;
    }
    qWarning("No such config: '%s'", qPrintable(key));
    return false;
}

/**
 * @brief CalculationMakerContext::expandSnippet
 * @param text
 * @return
 *
 * テキストの展開を行う。
 * \projectname{} や \pg{} などの処理を行う。
 *
 */
QString CalculationMakerContext::expandSnippet(const QString &text)
{
    QRegExp regexp("(\\\\[\\w][\\w\\d]*)\\{([^\\}]*)\\}");
    QString str = text;
    str.replace(u8"\\\n", u8"");
    int pos = 0;
    while ((pos = regexp.indexIn(str, pos)) != -1) {
//        qDebug() << "Match:" << pos;
        Q_ASSERT(regexp.captureCount() == 2);
        QString command = regexp.cap(1);
        QString arg = regexp.cap(2);
        CalculationMakerSnippet *cmd = m_snippetCommands.value(command, 0);
        if (!cmd) {
            qWarning("No such command: %s", qPrintable(command));
            break;
        }
        QString buf = cmd->expandText(m_maker, arg);
        str.replace(pos, regexp.matchedLength(), buf);
    }
    return str;
}

/**
 * @brief CalculationMakerContext::draw
 * @param command
 * @param arg
 * @param lineno
 * @return
 *
 * 各コマンドの実行を行う。
 *
 */
bool CalculationMakerContext::draw(const QString &command, const QString &arg, int lineno)
{
    CalculationMakerDrawCommand *cmd = m_drawCommands.value(command, 0);
    if (!cmd) {
        qWarning("No such command: %s at line %d", qPrintable(command), lineno);
        return false;
    }
    bool result = cmd->draw(m_maker, arg);
    if (!result) {
        qWarning("Failed: %s{%s}: %s at line %d", command.toUtf8().constData(),
               arg.toUtf8().constData(), cmd->errorString().toUtf8().constData(), lineno);
    }
    if (m_printer && isPrinting()) {
        QPrinter::PrinterState state = m_printer->printerState();
        if (state == QPrinter::Aborted || state == QPrinter::Error) {
            qWarning("Printer Error");
            return false;
        }
    }
    return result;
}

void CalculationMakerContext::beginPrinting()
{
    m_printer = new QPrinter(QPrinter::HighResolution);
    m_printer->setResolution(300);
    m_printer->setOutputFormat(QPrinter::PdfFormat);
    m_printer->setOutputFileName(m_filename);
    m_printer->setFullPage(true);

    //m_bookmark = new QPrinterBookmark;
    //m_printer->setBookmark(m_bookmark);

    m_currentPaper = configPaper();
    m_printer->setPaperSize(m_currentPaper.paper.PaperSize());
    m_printer->setPageOrientation(m_currentPaper.paper.Orientation());

    m_painter = new QPainter(m_printer);
}

void CalculationMakerContext::endPrinting()
{
    m_painter->end();

    delete m_painter;
    delete m_printer;
    //delete m_bookmark;

    m_painter = 0;
    m_printer = 0;
    //m_bookmark = 0;
}

void CalculationMakerContext::initState()
{
    applyTextConfig(u8"normal");
    setLineSpacing(m_config.configValues.value(u8"linespacing").toDouble());
    setHeader(true);
    setFooter(true);
    setPen(u8"NORMALPEN");
    //m_state.node = 0;
}

/**
 * @brief CalculationMakerContext::markToBookmark
 * @param title
 *
 * PDFしおりの作成
 *
 */
void CalculationMakerContext::markToBookmark(const QString &title)
{
    //if (m_state.node)
    //    return;
    const CalculationMakerState &prev = m_stateStack.top();
    //m_state.node = m_printer->createBookmarkNode(title);
    //if (prev.node)
    //    prev.node->append(m_state.node);
    //else
    //    m_bookmark->append(m_state.node);
}

/**
 * @brief CalculationMakerContext::markToContents
 * @param title
 *
 * 目次用データの作成
 *
 */
void CalculationMakerContext::markToContents(const QString &title)
{
    CalculationMakerContentsNode node;
    node.level = m_stateStack.count();
    node.title = title;
    node.pageno = m_pageno;
    m_contentsNodes.append(node);
}

int CalculationMakerContext::currentPage()
{
    return m_pageno;
}

/**
 * @brief CalculationMakerContext::newPage
 * @param paper
 *
 * \newpage{} コマンド用のメソッド。
 *
 */
void CalculationMakerContext::newPage(const post3dapp::PAPERSETTING &paper)
{
    m_currentPaper = paper;
    if (m_printer->paperSize() != m_currentPaper.paper.PaperSize())
        m_printer->setPaperSize(m_currentPaper.paper.PaperSize());
    if (m_printer->pageLayout().orientation() != m_currentPaper.paper.Orientation())
        m_printer->setPageOrientation(m_currentPaper.paper.Orientation());


    if (m_pageno >= 0) {
        m_printer->newPage();
    }
    ++m_pageno;

    QMargins margin = m_config.margin();
    m_pos = QPointF(-1, margin.top());

    m_currentPaperRectPixel = m_printer->paperRect();
    m_currentPaperRectMiliMeter = m_printer->paperRect(QPrinter::Millimeter);

    drawHeaderFooter();
}

void CalculationMakerContext::newPage()
{
    newPage(configPaper());
}

/**
 * @brief CalculationMakerContext::currentLineHeight
 * @return
 *
 * 一行の高さの計算。
 * 単位はミリメートル。
 */
qreal CalculationMakerContext::lineHeight(const TextStyle &style)
{
    m_painter->save();
    QFont font(style.family);
    font.setPointSizeF(style.fontSize);
    m_painter->setFont(font);

    QFontMetrics fm = m_painter->fontMetrics();
    int height = fm.height();
    qreal lh = height * m_currentPaperRectMiliMeter.height() / m_currentPaperRectPixel.height();
    m_painter->restore();
    return lh;
}

qreal CalculationMakerContext::currentLineHeight()
{
    return lineHeight(m_state.style);
}

/**
 * @brief CalculationMakerContext::newLine
 *
 * \\ コマンド用のメソッド。
 *
 */
void CalculationMakerContext::newLine()
{
    newLine(currentLineHeight());
}

/**
 * @brief CalculationMakerContext::newLine
 * @param height
 *
 * \\{height} コマンド用の処理。
 *
 */
void CalculationMakerContext::newLine(qreal height)
{
    QPointF pos = currentPos();
    setPos(QPointF(-1, pos.y() + height));
}

bool CalculationMakerContext::currentBlockEnabled() const
{
    return m_enabled;
}

QString CalculationMakerContext::outputFilename() const
{
    return m_filename;
}

void CalculationMakerContext::setOutputFilename(const QString &name)
{
    m_filename = name;
}

/**
 * @brief CalculationMakerContext::beginBlock
 * @param name
 * @return
 *
 * \begin{} コマンド用の処理。
 *
 */
bool CalculationMakerContext::beginBlock(const QString &name)
{
    bool isFirst = m_stateStack.isEmpty() && !isPrinting();
    if (isFirst) {
        beginPrinting();
        if (m_printer->printerState() == QPrinter::Error || !m_painter->isActive())
            return false;
        initState();
    }
    m_stateStack.push(m_state);
    m_state.blockName = name;
    if (!m_enabled)
        return true;
    if (name.isEmpty() || !m_maker->sectionSettings.contains(name))
        m_enabled = true;
    else
        m_enabled = m_maker->sectionSettings.value(name);
    qDebug() << "beginBlock" << name << m_enabled;
    initState();
    return true;
}

/**
 * @brief CalculationMakerContext::endBlock
 * @param name
 * @return
 *
 * \end{} コマンド用の処理。
 *
 */
bool CalculationMakerContext::endBlock(const QString &name)
{
    if (m_state.blockName != name || m_stateStack.isEmpty())
        return false;
    m_state = m_stateStack.pop();
    QString newBlock = m_state.blockName;
    m_enabled = m_maker->sectionSettings.value(newBlock, m_enabled);
    qDebug() << "endBlock" << name << m_enabled;
    return true;
}

/**
 * @brief CalculationMakerContext::setFont
 * @param font
 * @param size
 *
 * \font{} コマンド用の処理。
 *
 */
void CalculationMakerContext::setFont(const QString &font, qreal size)
{
    m_state.style.family = font;
    m_state.style.fontSize = size;
}

/**
 * @brief CalculationMakerContext::setAlignment
 * @param align
 *
 * \alignment{} コマンド用の処理。
 *
 */
void CalculationMakerContext::setAlignment(Qt::Alignment align)
{
    m_state.style.align = align;
}

/**
 * @brief CalculationMakerContext::setLeftIndent
 * @param value
 *
 * \indentL{} コマンド用の処理。
 *
 */
void CalculationMakerContext::setLeftIndent(qreal value)
{
    m_state.style.leftindent = value;
}

/**
 * @brief CalculationMakerContext::setRightIndent
 * @param value
 *
 * \indentR{} コマンド用の処理。
 *
 */
void CalculationMakerContext::setRightIndent(qreal value)
{
    m_state.style.rightindent = value;
}

void CalculationMakerContext::setLineSpacing(qreal value)
{
    m_state.linespacing = value;
}

void CalculationMakerContext::setHeader(bool value)
{
    m_state.drawHeader = value;
}

void CalculationMakerContext::setFooter(bool value)
{
    m_state.drawFooter = value;
}

void CalculationMakerContext::setPen(const QString &pen)
{
    QString p = pen.toLower();
    if (m_config.configValues.contains(p)) {
        QVariant val = m_config.configValues.value(p);
        if (val.canConvert(QVariant::Double))
            m_state.m_penwidth = val.toDouble();
    }
}

CalculationMakerConfig *CalculationMakerContext::config()
{
    return &m_config;
}

post3dapp::PAPERSETTING CalculationMakerContext::configPaper() const
{
    return post3dapp::PAPERSETTING(
                post3dapp::PAPER(
                static_cast<QPagedPaintDevice::PageSize>(m_config.configValues.value("pagesize").toInt()),
                static_cast<QPageLayout::Orientation>(m_config.configValues.value("pageorientation").toInt())));
}

QPointF CalculationMakerContext::pos() const
{
    return m_pos;
}

QPointF CalculationMakerContext::currentPos() const
{
    QPointF p(m_pos);
    if (p.x() < 0) {
        QMargins margin = m_config.margin();
        TextStyle style = currentStyle();
        p.setX(margin.left() + style.leftindent);
    }
    return p;
}

/**
 * @brief CalculationMakerContext::painterPos
 * @param src
 * @return
 *
 * ミリメートル座標をピクセル座標に変換する。
 *
 */
QPoint CalculationMakerContext::painterPos(const QPointF &src) const
{
    QRect prect = m_currentPaperRectPixel;
    QRectF mrect = m_currentPaperRectMiliMeter;
    double x = src.x() / mrect.width() * prect.width();
    double y = src.y() / mrect.height() * prect.height();
    return QPoint(qRound(x), qRound(y));
}

/**
 * @brief CalculationMakerContext::paperPos
 * @param src
 * @return
 *
 * ピクセル座標をミリメートル座標に変換する。
 *
 */
QPointF CalculationMakerContext::paperPos(const QPoint &src) const
{
    QRect prect = m_currentPaperRectPixel;
    QRectF mrect = m_currentPaperRectMiliMeter;
    double x = src.x() * mrect.width() / prect.width();
    double y = src.y() * mrect.height() / prect.height();
    return QPointF(x, y);
}

void CalculationMakerContext::applyTextConfig(const QString &type)
{
    if (!config()->configValues.contains(type))
        return;
    QVariant val = config()->configValues.value(type);
    if (!val.canConvert<TextStyle>())
        return;
    m_state.style = val.value<TextStyle>();
}

const TextStyle &CalculationMakerContext::currentStyle() const
{
    return m_state.style;
}

/**
 * @brief CalculationMakerContext::textWidth
 * @param style
 * @param text
 * @return
 *
 * 与えられた文字列 @a text の幅を計算する。
 * 値はミリメートル単位で返す。
 */
qreal CalculationMakerContext::textWidth(const TextStyle &style, const QString &text)
{
    if (!m_painter)
        return 0;

    m_painter->save();

    QString str = expandSnippet(text);

    QFont font(style.family);
    font.setPointSizeF(style.fontSize);
    m_painter->setFont(font);

    QFontMetrics fm = m_painter->fontMetrics();

    qreal w = fm.width(str);
    qreal width = w * m_currentPaperRectMiliMeter.width() / m_currentPaperRectPixel.width();

    m_painter->restore();

    return width;
}

/**
 * @brief CalculationMakerContext::drawText
 * @param style
 * @param pos
 * @param text
 * @param opt
 * @param nextPos
 * @return
 *
 * @internal
 *
 * 与えられた文字列 @a text を描画する。
 */
QString CalculationMakerContext::drawText(const TextStyle &style, const QPointF &pos, const QString &text, TextOptions opt, QPointF *nextPos)
{
    if (!m_painter)
        return QString();

    QString str = text;
    if (!(opt & IsNextPage))
        str = expandSnippet(text);

    QFont font(style.family);
    font.setPointSizeF(style.fontSize);

    QFontMetrics fm = m_painter->fontMetrics();
    QRect prect = m_currentPaperRectPixel;
    QRectF mrect = m_currentPaperRectMiliMeter;

    QMargins margin = config()->margin();

    QPointF topleftF(pos);
    if (topleftF.x() < 0)
        topleftF.setX(margin.left() + style.leftindent);
    QPointF rightlsF(mrect.width() - style.rightindent - margin.right(), m_state.linespacing);

    if (opt & OneLine) {
        if (style.align & Qt::AlignRight) {
            topleftF.setX(0);
        } else if (style.align & Qt::AlignLeft) {
            rightlsF.setX(mrect.right());
        }
    }
    QPoint topleft = painterPos(topleftF);
    QPoint rightls = painterPos(rightlsF);

    QPointF paperBottomF = mrect.bottomLeft();
    paperBottomF.setY(paperBottomF.y() - margin.bottom());
    QPoint paperBottom = painterPos(paperBottomF);

    qreal left = topleft.x();
    qreal width = rightls.x() - left;
    qreal top = topleft.y();
    qreal ls = rightls.y();
    int leading = fm.leading();

    qreal y = top;

    qreal itemizeindent = 0;
    if (opt & Itemize) {
        if (!(opt & IsNextPage)) {
            QString istr = str.at(0);
            QTextLayout layout(istr, font, m_printer);
            layout.beginLayout();
            QTextLine line = layout.createLine();
            line.setLineWidth(width);
            qreal bottom = y + leading + line.height();
            if (bottom > paperBottom.y()) {
                if (nextPos)
                    *nextPos = QPoint(-1, -1);
                return str;
            }
            line.setPosition(QPointF(left, y + leading));
            line.draw(m_painter, QPointF(0, 0));
            itemizeindent = line.horizontalAdvance();
            left += itemizeindent;
            width = rightls.x() - left;
            layout.endLayout();
            str.remove(0, 1);
        } else {
            // for next page.
            itemizeindent = left - painterPos(QPointF(margin.left() + style.leftindent, 0)).x();
        }
    }

    QTextLayout layout(str, font, m_printer);
    QTextOption lopt;
    lopt.setAlignment(style.align);
    layout.setTextOption(lopt);

    if (opt & SuperScript || opt & SubScript) {
        QTextLayout::FormatRange range;
        range.start = 0;
        range.length = str.length();
        QTextCharFormat format;
        if (opt & SuperScript)
            format.setVerticalAlignment(QTextCharFormat::AlignSuperScript);
        else
            format.setVerticalAlignment(QTextCharFormat::AlignSubScript);
        range.format = format;
        QList<QTextLayout::FormatRange> ranges;
        ranges << range;
        layout.setAdditionalFormats(ranges);
    }

    layout.beginLayout();
    qreal maxw = 0;
    int linecount = 0;
    QPoint np;
    while (true) {
        QTextLine line = layout.createLine();
        if (!line.isValid())
            break;

        line.setLineWidth(width);
        y += leading;
        qreal bottom = y + line.height();
        if ((bottom > paperBottom.y()) && !(opt & AllowInMarginArea) && !(opt & OneLine)) {
            if (opt & WithBox) {
                // TODO: write box;
            } else if (opt & Itemize) {
                if (nextPos)
                    *nextPos = paperPos(QPoint(left, 0));
            }
            QString remain = str.mid(line.textStart());
            return remain;
        }

        line.setPosition(QPointF(left, y));
        line.draw(m_painter, QPointF(0, 0));
        maxw = qMax(maxw, line.naturalTextWidth());

        if (linecount == 0 && pos.x() >= 0) {
            topleftF.setX(margin.left() + style.leftindent);
            topleft = painterPos(topleftF);
            left = topleft.x() + itemizeindent;
            width = rightls.x() - left;
        }

        np = line.naturalTextRect().topRight().toPoint();

        y += line.height() + ls;

        if (opt & OneLine)
            break;
    }
    layout.endLayout();

    qreal dy = y - top;

    if (opt & WithBox) {
        // TODO: check IsNextPage
        qreal boxdy = (dy - ls) / prect.height() * mrect.height();
        qreal boxdx = maxw / prect.width() * mrect.width();
        QPointF bottomrightF(topleftF.x() + boxdx, topleftF.y() + boxdy);
        QRectF rect;
        rect.setTopLeft(topleftF);
        rect.setBottomRight(bottomrightF);
        drawRect(rect);
    }

    if (nextPos) {
        QPointF p;
        if (opt & WithoutCarriageReturn) {
            p = paperPos(np);
        } else {
            np = QPoint(0, y);
            p = paperPos(np);
            p.setX(margin.left() + style.leftindent);
        }
        *nextPos = p;
    }

    return QString();
}

/**
 * @brief CalculationMakerContext::drawText
 * @param style
 * @param text
 * @param opt
 *
 * 文字列を描画する。
 *
 */
void CalculationMakerContext::drawText(const TextStyle &style, const QString &text, TextOptions opt)
{
    QPointF np;
    QPointF pos = m_pos;
    QString str = drawText(style, pos, text, opt, &np);
    while (!str.isEmpty() && !(opt & OneLine)) {
        newPage();
        pos = m_pos;
        if (opt & Itemize && np.x() >= 0)
            pos.setX(np.x());
        if (opt & Itemize && np.x() < 0)
            str = drawText(style, pos, str, opt, &np);
        else
            str = drawText(style, pos, str, opt | IsNextPage, &np);
    }
    if (opt & WithoutCarriageReturn)
        setPos(np);
    else
        setPos(QPointF(-1, np.y()));
}

/**
 * @brief CalculationMakerContext::drawRect
 * @param rect
 *
 * 矩形を描画する。
 * \rect{} コマンド用の処理。
 *
 */
void CalculationMakerContext::drawRect(const QRectF &rect)
{
    if (!m_painter)
        return;

    QRect drect;
    drect.setTopLeft(painterPos(rect.topLeft()));
    drect.setBottomRight(painterPos(rect.bottomRight()));

    QRect prect = m_currentPaperRectPixel;
    QRectF mrect = m_currentPaperRectMiliMeter;

    QPen pen;
    pen.setWidthF(m_state.m_penwidth * prect.width() / mrect.width());
    m_painter->setPen(pen);
    m_painter->drawRect(drect);
}

/**
 * @brief CalculationMakerContext::drawLine
 * @param p0
 * @param p1
 *
 * 直線を描画する。
 * \line{} コマンド用の処理。
 *
 */
void CalculationMakerContext::drawLine(const QPointF &p0, const QPointF &p1)
{
    if (!m_painter)
        return;

    QRect prect = m_currentPaperRectPixel;
    QRectF mrect = m_currentPaperRectMiliMeter;

    QPen pen;
    pen.setWidthF(m_state.m_penwidth * prect.width() / mrect.width());
    m_painter->setPen(pen);
    m_painter->drawLine(painterPos(p0), painterPos(p1));
}

void CalculationMakerContext::drawImage(const QRectF &rect, const QImage &image)
{
    if (!m_painter)
        return;

    QRect r(painterPos(rect.topLeft()), painterPos(rect.bottomRight()));
    QImage scaledimage = image.scaled(r.size(), Qt::KeepAspectRatio);
    m_painter->drawImage(r.topLeft(), scaledimage);
}

/**
 * @brief CalculationMakerContext::drawHeaderFooter
 *
 * ヘッダー、フッターの描画。
 * \newpage{} 時に呼び出される。
 */
void CalculationMakerContext::drawHeaderFooter()
{
    if (m_state.drawHeader) {
        HeaderFooterFormat format = config()->format(u8"headerformat");
        if (!format.family.isEmpty()) {
            TextStyle headerStyle;
            headerStyle.family = format.family;
            headerStyle.fontSize = format.fontSize;
            headerStyle.leftindent = headerStyle.rightindent = 0;
            headerStyle.align = Qt::AlignLeft | Qt::AlignTop;
            headerStyle.bookmark = headerStyle.contents = false;
            QPointF pos(-1, format.pos);
            QString text = trimQuote(config()->configValues.value("headerL").toString());
            drawText(headerStyle, pos, text, AllowInMarginArea);
            headerStyle.align = Qt::AlignRight | Qt::AlignTop;
            text = trimQuote(config()->configValues.value("headerR").toString());
            drawText(headerStyle, pos, text, AllowInMarginArea);
        }
    }
    if (m_state.drawFooter) {
        HeaderFooterFormat format = config()->format(u8"footerformat");
        if (!format.family.isEmpty()) {
            TextStyle footerStyle;
            footerStyle.family = format.family;
            footerStyle.fontSize = format.fontSize;
            footerStyle.leftindent = footerStyle.rightindent = 0;
            footerStyle.align = Qt::AlignLeft | Qt::AlignTop;
            footerStyle.bookmark = footerStyle.contents = false;
            QRectF r = m_currentPaperRectMiliMeter;
            QPointF pos(-1, r.bottom() - format.pos);
            QString text = trimQuote(config()->configValues.value("footerL").toString());
            drawText(footerStyle, pos, text, AllowInMarginArea);
            footerStyle.align = Qt::AlignRight | Qt::AlignTop;
            text = trimQuote(config()->configValues.value("footerR").toString());
            drawText(footerStyle, pos, text, AllowInMarginArea);
        }
    }
}

void CalculationMakerContext::setPos(const QPointF &pos)
{
    m_pos = pos;
}

/**
 * @brief CalculationMakerContext::drawFigure
 * @param pos
 * @param figure
 *
 * 図の描画。
 * \includefigure{} コマンド用の処理。
 *
 */
void CalculationMakerContext::drawFigure(const QString &figure, const QPointF &pos, qreal width, qreal height)
{
    FigureInterface *figureCmd = m_figureCommands.value(figure);
    if (!figureCmd) {
        qWarning("FigureInterface fot %s is not found.", figure.toUtf8().constData());
        return;
    }

    QString group = m_maker->figureToGroup.value(figure);
    if (group.isEmpty()) {
        QRectF rect = m_currentPaperRectMiliMeter;
        QMargins margin = config()->margin();
        rect.setTopLeft(pos);
        if (width > 0)
            rect.setWidth(width);
        else
            rect.setRight(rect.right() - margin.right());
        if (height > 0)
            rect.setHeight(height);
        else
            rect.setBottom(rect.bottom() - margin.bottom());
        // QRect r(painterPos(rect.topLeft()), painterPos(rect.bottomRight()));
        QRect r(rect.x(), rect.y(), rect.width(), rect.height());
        setPos(QPointF(-1, rect.bottom()));
        m_painter->save();
        figureCmd->draw(m_painter, r, QString(), post3dapp::PAPERDIVIDE(1, 1), 0);
        m_painter->restore();
        TextStyle style = config()->style(u8"normal");
        style.align = Qt::AlignCenter;
        drawText(style, group);
    } else {
        Q_ASSERT(m_maker->outGroups.contains(group));
        Q_ASSERT(m_maker->groupFormats.contains(group));
        QStringList floors = m_maker->outGroups.value(group);
        post3dapp::PAPERSETTING paper = m_maker->groupFormats.value(group);

        int sum = paper.separate.figureCount();
        Q_ASSERT(sum > 0);

        foreach (const QString &floor, floors) {
            for (int i = 0; i < sum; ++i) {
                if (m_pos.y() > pos.y())
                    newPage(paper);
                QRectF rect = m_currentPaperRectMiliMeter;
                QMargins margin = config()->margin();
                rect.setTopLeft(pos);
                if (width > 0)
                    rect.setWidth(width);
                else
                    rect.setRight(rect.right() - margin.right());
                if (height > 0)
                    rect.setHeight(height);
                else
                    rect.setBottom(rect.bottom() - margin.bottom());
                //QRect r(painterPos(rect.topLeft()), painterPos(rect.bottomRight()));
                QRect r(rect.x(), rect.y(), rect.width(), rect.height());
                m_painter->save();
                figureCmd->draw(m_painter, r, floor, paper.separate, i);
                m_painter->restore();
                TextStyle style = config()->style(u8"normal");
                style.align = Qt::AlignLeft;
                if (figureCmd->isPlan()){
                    drawText(style, floor + u8"階床梁伏図");
                }
                else{
                    drawText(style, floor + u8"通軸組図");
                }
                setPos(QPointF(-1, rect.bottom()));
            }
        }
    }

}

/**
 * @brief CalculationMakerContext::drawTable
 * @param width
 * @param align
 * @param table
 *
 * テーブルの描画。
 * \includetable{} コマンド用の処理。
 *
 */
void CalculationMakerContext::drawTable(const QString &table, Qt::Alignment align, qreal width)
{
    TableInterface *tableCmd = m_tableCommands.value(table, 0);
    if (!tableCmd)
        return;

    int headers = tableCmd->headerCount();
    int rows = tableCmd->tableCount();

    qreal rowh = config()->configValues.value(u8"tablerowheight").toDouble();
    qreal ls = config()->configValues.value(u8"linespacing").toDouble();
    QMargins margin = config()->margin();

    int w = qRound(width * m_currentPaperRectPixel.width() / m_currentPaperRectMiliMeter.width());
    int rh = qRound(rowh * m_currentPaperRectPixel.height() / m_currentPaperRectMiliMeter.height());

    m_painter->save();

    int r = 0;
    while (r < rows) {
        QPointF pF = currentPos();
        if (align & Qt::AlignHCenter) {
            qreal x0 = pF.x();
            qreal pw = m_currentPaperRectMiliMeter.right() - margin.right() - x0;
            pF.setX(x0 + (pw - width) / 2.0);
        } else if (align & Qt::AlignRight) {
            pF.setX(m_currentPaperRectMiliMeter.right() - margin.right() - width);
        }
        qreal y = pF.y();
        qreal h = m_currentPaperRectMiliMeter.bottom() - margin.bottom() - y;
        QPoint p = painterPos(pF);

        int n = int(h / rowh) - headers;
        int e = qMin(rows - 1, r + n - 1);
        if (n > 0) {
            tableCmd->draw(m_painter, p, w, rh, r, e);
            m_pos.setY(y + (e - r + 1 + headers) * rowh + ls);
            r = e + 1;
        }
        if (r < rows)
            newPage();
    }
    m_painter->restore();
}

/**
 * @brief CalculationMakerContext::drawContents
 *
 * 目次を描画する。
 * \contents{} コマンド用の処理。
 */
void CalculationMakerContext::drawContents()
{
    if (!hasContents())
        return;

    newPage();

    TextStyle style = config()->style(u8"chapter");
    style.align = Qt::AlignCenter | Qt::AlignTop;

    newLine();
    drawText(style, QString::fromLocal8Bit("目　次"));
    newLine();

    style = config()->style(u8"contents");
    QString buf;
    // Draw title and page number.
    for (int i = 0; i< m_contentsNodes.count(); i++){
        QPointF pos;
        TextStyle s = style;
        s.align = Qt::AlignLeft | Qt::AlignTop;
        s.leftindent += 1.0 * (m_contentsNodes.at(i).level - 1);
        buf = drawText(s, m_pos, m_contentsNodes.at(i).title, OneLine);
        Q_ASSERT(buf.isEmpty());

        QString str;
        str.setNum(m_contentsNodes.at(i).pageno);
        s.align = Qt::AlignRight | Qt::AlignTop;
        drawText(s, m_pos, str, OneLine);

        // ("・・・ ")
        s.rightindent = 5.0;
        s.align = Qt::AlignRight | Qt::AlignTop;
        drawText(s, m_pos, u8"・・・ ", OneLine, &pos);

        newLine(8.0);
        // newpage判定
        if (i == m_contentsNodes.count() - 1)
            return;

        if (m_pos.y() > m_currentPaperRectMiliMeter.bottom() - config()->margin().bottom()){
            newPage();
            style = config()->style(u8"chapter");
            style.align = Qt::AlignCenter | Qt::AlignTop;

            newLine();
            drawText(style, QString::fromLocal8Bit("目　次"));
            newLine();

            style = config()->style(u8"contents");
        }
    }
}

int CalculationMakerContext::currentChapter() const
{
    return m_chapter;
}

int CalculationMakerContext::currentSection() const
{
    return m_section;
}

int CalculationMakerContext::currentSubSection() const
{
    return m_subsection;
}

void CalculationMakerContext::nextChapter()
{
    ++m_chapter;
    m_section = 0;
    m_subsection = 0;
}

void CalculationMakerContext::nextSection()
{
    ++m_section;
    m_subsection = 0;
}

void CalculationMakerContext::nextSubSection()
{
    ++m_subsection;
}
} // namespace CalculationMakerLib
