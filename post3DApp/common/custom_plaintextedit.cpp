#include "custom_plaintextedit.h"

#include <QPainter>
#include <QTextBlock>
#include <QUuid>

#include "textdataunit.h"


namespace post3dapp {

CustomPlainTextEdit::CustomPlainTextEdit(QWidget *parent)
    :QPlainTextEdit(parent)
{
    this->setLineWrapMode(NoWrap);
    this->document()->setDocumentMargin(0.0);
    rowCounter = new QWidget(this);
    rowCounter->installEventFilter(this);
    connect(this,QOverload<const QRect&, int>::of(&QPlainTextEdit::updateRequest),viewport(),QOverload<const QRect&>::of(&QWidget::update));
    connect(this,QOverload<const QRect&, int>::of(&QPlainTextEdit::updateRequest),rowCounter,QOverload<const QRect&>::of(&QWidget::update));
}

void CustomPlainTextEdit::drawHorizontalRuledLine(QPainter *painter) const{
    painter->setPen(Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    while( block.isValid() ) {
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        if (top >= viewport()->rect().bottom()){
            break;
        }
        if (top < 0) continue;

        painter->drawLine(QPointF(0,top),QPointF(this->width(),top));
        block = block.next();
    }
}

void CustomPlainTextEdit::drawVerticalRuledLine(QPainter *painter){
    int x = contentOffset().x();
    while(true){
        if(x > this->width()){
            break;
        }
        painter->drawLine(QPointF(x,0),QPointF(x,this->height()));
        x += fontWidth * 10;
    }
}

void CustomPlainTextEdit::paintEvent(QPaintEvent *event){
    decorateCells(); // QPainterは色を重ねていくので、背景は先に書いておく
    QPlainTextEdit::paintEvent(event);

    QPainter painter(viewport());
    drawEOB(&painter);
    drawHorizontalRuledLine(&painter);
    drawVerticalRuledLine(&painter);

    QRect cr=this->cursorRect(this->textCursor());
    painter.setPen(Qt::red);
    painter.drawLine(QPointF(cr.left(),0),QPointF(cr.left(),this->height()));
    painter.drawLine(QPointF(0,cr.bottom()),QPointF(this->width(),cr.bottom()));
}

void CustomPlainTextEdit::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    rowCounter->setGeometry(QRect(rect().left(), rect().top(), rowCountAreaWidth, rect().height()));
}

void CustomPlainTextEdit::drawEOB(QPainter* painter) const
{
    painter->setPen(Qt::blue);

    QTextCursor cur = textCursor();
    cur.movePosition(QTextCursor::End);
    const int posEOF = cur.position();

    QTextBlock block = firstVisibleBlock();
    while( block.isValid() ) {
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        if (top >= viewport()->rect().bottom()){
            break;
        }
        if (top < 0) continue;

        cur.setPosition(block.position());
        cur.movePosition(QTextCursor::EndOfBlock);
        QRect r = cursorRect(cur);

        if( cur.position() == posEOF ){
            painter->drawText(QPointF(r.left(), r.bottom()), "[EOF]");
            break;
        }
        else{
            painter->drawText(QPointF(r.left(), r.bottom()), "^");
            block = block.next();
        }
    }
}

void CustomPlainTextEdit::decorateUnit(const QTextBlock& block, const TextDataUnit& unit) const{
    QColor color;
    int offset = contentOffset().x();
    switch(unit.type){
    case TEXTTYPE::TYPE_INT:
        color = QColor{200, 200, 255};
        break;
    case TEXTTYPE::TYPE_DOUBLE:
        color = QColor{255, 255, 200};
        break;
    case TEXTTYPE::TYPE_TEXT:
        color = QColor{255, 200, 200};
        break;
    case TEXTTYPE::TYPE_BLANK:
        color = QColor{100, 100, 100};
        break;
    }
    QPainter painter(viewport());
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = (int) blockBoundingGeometry(block).translated(contentOffset()).bottom();
    int ht=bottom-top;

    painter.fillRect(offset + fontWidth * (unit.startCol-1),
                     top,
                     fontWidth * (unit.endCol - unit.startCol + 1),
                     ht,
                     color);
    painter.setPen(Qt::darkGray);
    painter.drawRect(offset + fontWidth * (unit.startCol - 1),
                     top,
                     fontWidth * (unit.endCol - unit.startCol + 1),
                     ht);
    if(block.text().mid(unit.startCol-1,unit.endCol-unit.startCol+1).trimmed().isEmpty()){
       painter.setPen(QColor(150,150,150));
       painter.drawText(QPointF(offset + fontWidth * (unit.startCol - 1), bottom - 5.0),unit.defaultValue);
    }

}

void CustomPlainTextEdit::drawRedCircle(const QTextBlock& block, QPainter* painter) const{
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    painter->setPen(Qt::red);
    painter->drawText(0, top + 3.0, rowCountWidth, fontHeight, Qt::AlignLeft, "*");
}


void CustomPlainTextEdit::drawRowNumber(const QTextBlock& block, QPainter* painter) const
{
    if(block.blockNumber() == this->textCursor().block().blockNumber()){
        QFont boldfont=painter->font();
        boldfont.setBold(true);
        painter->setFont(boldfont);
        painter->setPen(Qt::red);
    }
    else{
        QFont orgfont= painter->font();
        orgfont.setBold(false);
        painter->setFont(orgfont);
        painter->setPen(Qt::black);
    }
    QString lineNumber = QString::number(block.blockNumber() + 1);
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    painter->drawText(0, top + 3.0, rowCountWidth, fontHeight, Qt::AlignRight, lineNumber);
}

QString CustomPlainTextEdit::write() const{
    return toPlainText();
}
} // namespace post3dapp
