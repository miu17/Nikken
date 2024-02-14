#include "outputtext_plaintextedit.h"

#include <QBoxLayout>
#include <QPainter>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextBlock>

#include "outputtext_reader.h"

namespace post3dapp{

OutputTextPlainTextEdit::OutputTextPlainTextEdit(QWidget *parent)
    :CustomPlainTextEdit(parent)
{
    this->setFont(QFont("Courier", 8));
    fontWidth = fontMetrics().horizontalAdvance('8');
    fontHeight = fontMetrics().height();
    rowCountWidth = fontWidth*6;
    rowCountAreaWidth = fontWidth*8;
    setViewportMargins(rowCountAreaWidth,0,0,0);
}

void OutputTextPlainTextEdit::decorateCells() const
{
    QTextBlock block = this->document()->firstBlock();
    OutputTextStatus type = OutputTextStatus::None;
    while(block.isValid()){
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        if (top >= viewport()->rect().bottom()){
            break;
        }

        QString errorMessage;
        LineOutputText* condition = OutputTextReader::MatchLineCondition(block.text(), type, &errorMessage);
        if (condition != nullptr){
            if (condition->getStatus() != OutputTextStatus::Skip)
                type = condition->getStatus();

            if (top >= 0 && errorMessage == QString{}){
                Q_FOREACH(auto unit, condition->getUnits()){
                    decorateUnit(block, unit);
                }
            }
        }

        if (type == OutputTextStatus::FinalEnd)
            return;

        block = block.next();
    }
}

bool OutputTextPlainTextEdit::eventFilter(QObject *obj,QEvent *event)
{
    if(obj == rowCounter && (event->type() == QEvent::Paint)){
        QPainter painter(rowCounter);
        painter.fillRect(rowCounter->rect(), Qt::lightGray);

        QTextBlock block = this->document()->firstBlock();
        OutputTextStatus type = OutputTextStatus::None;
        bool skipCheck = false;
        while(block.isValid()){
            int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
            if( top >= viewport()->rect().bottom() ){
                break;
            }

            QString errorMessage;
            LineOutputText* condition = OutputTextReader::CheckLineCondition(block.text(), type, &errorMessage);
            if (condition != nullptr){
                if (condition->getStatus() != OutputTextStatus::Skip)
                    type = condition->getStatus();
            }

            if (type == OutputTextStatus::FinalEnd)
                skipCheck = true;
            if (top >= 0){
                drawRowNumber(block, &painter);
                if (errorMessage != QString{} && !skipCheck){
                    drawRedCircle(block, &painter);
                }
            }
            block = block.next();
        }

        return true;
    }
    return false;
}


bool OutputTextPlainTextEdit::hasFormatError() const{
    return OutputTextReader::hasFormatError(toPlainText());
}

QString OutputTextPlainTextEdit::getStatusMessage() const{
    int col = this->textCursor().positionInBlock() + 1;
    QString column = "col:" + QString::number(col).rightJustified(4) + " ";

    QTextBlock current = this->textCursor().block();
    QTextBlock block = this->document()->firstBlock();
    OutputTextStatus type = OutputTextStatus::None;
    QString errorMessage;
    while(block.isValid()){
        LineOutputText* condition = OutputTextReader::MatchLineCondition(block.text(), type, &errorMessage);
        if (block.blockNumber() >= current.blockNumber() && errorMessage == QString{}){
            if (condition == nullptr)
                return column;
            return column + condition->getStatusBar(col);
        }
        if (condition != nullptr){
            if (condition->getStatus() != OutputTextStatus::Skip)
                type = condition->getStatus();
        }

        if (type == OutputTextStatus::FinalEnd)
            break;

        block = block.next();
    }

    return column;
}

} // namespace post3dapp
