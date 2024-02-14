#include "custom_label.h"

#include <qglobal.h>
#include <QFontMetrics>
#include <QPainter>

namespace post3dapp{
class CustomLine
{
public:
    void clear(const QChar &ch = QChar());

    bool adjustLetterSpacing;
    qreal letterSpacing;
    QString text;
};

class CustomLabelPrivate
{
public:
    CustomLabelPrivate()
    {
        lines = 0;
        width = 0;
        height = 0;
        elide = false;
        lineSpacing = 10;
        autoLineSpacing = false;
        dirty = false;
        margin = QMargins(5, 5, 5, 5);
        sizehint = QSize(-1, -1);
    }

    virtual ~CustomLabelPrivate()
    {
    }

    void nextLine(const CustomLine &line);
    void updateLayout(const QRect &contentRect, const QFont &font);
    void updateSizeHint(const QFont &font);

    QString text;
    int lines;
    int width;
    int height;
    QSize sizehint;
    QVector<CustomLine> textlines;
    bool elide;
    bool autoLineSpacing;
    int lineSpacing;
    int effectiveLineSpace;
    bool dirty;
    QMargins margin;
};

void CustomLine::clear(const QChar &ch)
{
    if (ch.isNull())
        text.clear();
    else
        text = ch;
    adjustLetterSpacing = false;
    letterSpacing = 0;
}

void CustomLabelPrivate::nextLine(const CustomLine &line)
{
    ++lines;
    textlines.append(line);
}

void CustomLabelPrivate::updateLayout(const QRect &contentRect, const QFont &font)
{
    textlines.clear();
    lines = 0;
    elide = false;

    QRect drawrect(contentRect);
    drawrect.adjust(margin.left(), margin.top(), - margin.right(), - margin.bottom());
    width = drawrect.width();
    height = drawrect.height();
    // add 2013/08/22
#if 0
    if (!autoLineSpacing)
        height += margin.bottom();
#endif

    QString doNotStartLineChars = QString::fromUtf8(",.\xe3\x80\x81\xe3\x80\x82");
    QFontMetrics fm(font);

    CustomLine line;
    line.clear();
    int texth = fm.height();
    int lineh = fm.height() + (autoLineSpacing ? 0 : lineSpacing);
    foreach (const QChar ch, text) {
        if (ch == QLatin1Char('\n')) {
            nextLine(line);
            line.clear();
            texth += lineh;
        } else {
            // modified 2013/08/22
            bool check = !line.adjustLetterSpacing && doNotStartLineChars.contains(ch);
            QString tmp = line.text + ch;
            int w = fm.width(tmp);
            if (w > width && !check) {
                nextLine(line);
                line.clear(ch);
                if (fm.width(line.text) > width)
                    texth = height + lineh;
                else
                    texth += lineh;
            } else {
                line.text = tmp;
                if (check && w > width) {
                    line.adjustLetterSpacing = true;
                    line.letterSpacing = ((qreal) w - width) / (line.text.length() - 1);
                }
            }
        }
        if (texth > height) {
            line.clear();
            texth -= lineh;
            if (texth < 0)
                texth = 0;
            elide = true;
            break;
        }
    }
    if (!line.text.isEmpty())
        nextLine(line);
    if (autoLineSpacing && lines > 1) {
        effectiveLineSpace = (height - texth) / (lines - 1);
    } else
        effectiveLineSpace = lineSpacing;
    dirty = false;
}

void CustomLabelPrivate::updateSizeHint(const QFont &font)
{
    QFontMetrics fm(font);
    int w = text.isEmpty() ? fm.maxWidth() * 5 : fm.width(text);
    sizehint = QSize(margin.left() + w + margin.right() + 2,
                     margin.top() + fm.height() + margin.bottom() + 2 );
}

/**
 * @brief CustomLabel::CustomLabel
 * @param parent
 */
CustomLabel::CustomLabel(QWidget *parent) :
    QFrame(parent), d_ptr(new CustomLabelPrivate)
{
    Q_D(CustomLabel);
    d->updateSizeHint(font());
    setForegroundRole(QPalette::WindowText);
    // add 2013/08/22
    setAutoFillBackground(true);
}

/**
 * @brief CustomLabel::CustomLabel
 * @param text
 * @param lineSpacing
 * @param margin
 * @param parent
 */
CustomLabel::CustomLabel(const QString &text, int lineSpacing, const QMargins &margin,
                         QWidget *parent) :
    QFrame(parent), d_ptr(new CustomLabelPrivate)
{
    Q_D(CustomLabel);
    d->text = text;
    d->lineSpacing = lineSpacing;
    d->margin = margin;
    d->dirty = true;
    d->updateSizeHint(font());
    setForegroundRole(QPalette::WindowText);
    // add 2013/08/22
    setAutoFillBackground(true);
}

/**
 * @brief CustomLabel::~CustomLabel
 */
CustomLabel::~CustomLabel()
{
    delete d_ptr;
}

/**
 * @brief CustomLabel::autoLineSpacing
 * @return
 */
bool CustomLabel::autoLineSpacing() const
{
    Q_D(const CustomLabel);
    return d->autoLineSpacing;
}

/**
 * @brief CustomLabel::setAutoLineSpacing
 * @param flag
 */
void CustomLabel::setAutoLineSpacing(bool flag)
{
    Q_D(CustomLabel);
    if (d->autoLineSpacing != flag) {
        d->autoLineSpacing = flag;
        d->dirty = true;
        update();
    }
}

/**
 * @brief CustomLabel::lineSpacing
 * @return
 */
int CustomLabel::lineSpacing() const
{
    Q_D(const CustomLabel);
    return d->lineSpacing;
}

/**
 * @brief CustomLabel::setLineSpacing
 * @param space
 */
void CustomLabel::setLineSpacing(int space)
{
    Q_D(CustomLabel);
    if (d->lineSpacing != space) {
        d->lineSpacing = space;
        if (!d->autoLineSpacing) {
            d->dirty = true;
            update();
        }
    }
}

/**
 * @brief CustomLabel::lineCount
 * @return
 */
int CustomLabel::lineCount()
{
    Q_D(CustomLabel);
    if (d->dirty)
        d->updateLayout(contentsRect(), font());
    return d->lines;
}

/**
 * @brief CustomLabel::isElide
 * @return
 */
bool CustomLabel::isElide()
{
    Q_D(CustomLabel);
    if (d->dirty)
        d->updateLayout(contentsRect(), font());
    return d->elide;
}

/**
 * @brief CustomLabel::margins
 * @return
 */
QMargins CustomLabel::margins() const
{
    Q_D(const CustomLabel);
    return d->margin;
}

/**
 * @brief CustomLabel::setMargins
 * @param margin
 */
void CustomLabel::setMargins(const QMargins &margin)
{
    Q_D(CustomLabel);
    if (d->margin != margin) {
        d->margin = margin;
        d->dirty = true;
        d->updateSizeHint(font());
        update();
    }
}

/**
 * @brief CustomLabel::text
 * @return
 */
QString CustomLabel::text() const
{
    Q_D(const CustomLabel);
    return d->text;
}

QSize CustomLabel::sizeHint() const
{
    Q_D(const CustomLabel);
    return d->sizehint;
}

/**
 * @brief CustomLabel::setText
 * @param text
 */
void CustomLabel::setText(const QString &text)
{
    Q_D(CustomLabel);
    if (d->text != text) {
        d->text = text;
        d->dirty = true;
        d->updateSizeHint(font());
        update();
        emit textChanged();
    }
}

/**
 * @brief CustomLabel::clear
 */
void CustomLabel::clear()
{
    setText(QString());
}

/**
 * @brief CustomLabel::resizeEvent
 * @param ev
 */
void CustomLabel::resizeEvent(QResizeEvent *ev)
{
    Q_D(CustomLabel);
    d->dirty = true;
    QFrame::resizeEvent(ev);
    update();
}

/**
 * @brief CustomLabel::paintEvent
 * @param ev
 */
void CustomLabel::paintEvent(QPaintEvent *ev)
{
    Q_D(CustomLabel);
    QFrame::paintEvent(ev);
    QFont labelFont(font());
    if (d->dirty)
        d->updateLayout(contentsRect(), labelFont);
    QFontMetrics fm(labelFont);
    QPainter p(this);
    p.setFont(labelFont);
    p.setPen(palette().color(foregroundRole()));
    int y = contentsRect().top() + d->margin.top();
    QRect textArea(contentsRect().left() + d->margin.left(), y, d->width, fm.height());
    for (int i = 0; i < d->lines; ++i) {
        const CustomLine &line = d->textlines[i];
        if (!d->elide || i != d->lines - 1) {
            if (line.adjustLetterSpacing) {
                QFont f(labelFont);
                f.setLetterSpacing(QFont::AbsoluteSpacing, labelFont.letterSpacing() - line.letterSpacing);
                p.save();
                p.setFont(f);
            }
            p.drawText(textArea.left(), y + fm.ascent() + 1, line.text);
            if (line.adjustLetterSpacing) {
                p.restore();
            }
            y += fm.height() + d->effectiveLineSpace;
            textArea.moveTop(y);
        } else {
            int padding = d->width - fm.width(line.text);
            int spaceWidth = fm.width(QLatin1Char(' '));
            QString str = line.text + QString(padding / spaceWidth + 1, QLatin1Char(' '));
            QString edlidedText = fm.elidedText(str, Qt::ElideRight, d->width);
            p.drawText(textArea.left(), y + fm.ascent() + 1, edlidedText);
        }
    }
}

/**
 * @brief CustomLabel::fontChange
 */
void CustomLabel::fontChange(const QFont &)
{
    Q_D(CustomLabel);
    d->dirty = true;
    d->updateSizeHint(font());
    update();
}
} // namespace post3dapp
