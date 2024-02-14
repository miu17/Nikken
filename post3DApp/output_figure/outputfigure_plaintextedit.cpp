#include "outputfigure_plaintextedit.h"

#include <QBoxLayout>
#include <QPainter>
#include <QPlainTextEdit>
#include <QTextBlock>

#include "define_draw_figure.h"
#include "outputfigure_reader.h"

namespace post3dapp{

OutputFigurePlainTextEdit::OutputFigurePlainTextEdit(QWidget *parent)
    :CustomPlainTextEdit(parent)
{
    this->setFont(QFont("Courier", 8));
    fontWidth = fontMetrics().horizontalAdvance('8');
    fontHeight = fontMetrics().height();
    rowCountWidth = fontWidth*6;
    rowCountAreaWidth = fontWidth*8;
    setViewportMargins(rowCountAreaWidth,0,0,0);
}

void OutputFigurePlainTextEdit::decorateCells() const
{
    QTextBlock block = this->document()->firstBlock();
    OutputFigureStatus type = OutputFigureStatus::None;
    while(block.isValid()){
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        if (top >= viewport()->rect().bottom()){
            break;
        }

        QString errorMessage;
        QSharedPointer<LineOutputFigure> condition = OutputFigureReader::MatchLineCondition(block.text(), type, &errorMessage);
        if (condition != nullptr){
            if (condition->getStatus() != OutputFigureStatus::Skip)
                type = condition->getStatus();

            if (top >= 0 && errorMessage == QString{}){
                Q_FOREACH(auto unit, condition->getUnits()){
                    decorateUnit(block, unit);
                }
            }
        }

        if (type == OutputFigureStatus::FinalEnd)
            return;

        block = block.next();
    }
}

bool OutputFigurePlainTextEdit::eventFilter(QObject *obj,QEvent *event)
{
    if(obj == rowCounter && (event->type() == QEvent::Paint)){
        QPainter painter(rowCounter);
        painter.fillRect(rowCounter->rect(), Qt::lightGray);

        QTextBlock block = this->document()->firstBlock();
        OutputFigureStatus type = OutputFigureStatus::None;
        bool skipCheck = false;
        while(block.isValid()){
            int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
            if( top >= viewport()->rect().bottom() ){
                break;
            }

            QString errorMessage;
            QSharedPointer<LineOutputFigure> condition = OutputFigureReader::CheckLineCondition(block.text(), type, &errorMessage);
            if (condition != nullptr){
                if (condition->getStatus() != OutputFigureStatus::Skip)
                    type = condition->getStatus();
            }

            if (type == OutputFigureStatus::FinalEnd)
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


bool OutputFigurePlainTextEdit::hasFormatError() const{
    return OutputFigureReader::hasFormatError(toPlainText());
}

QString OutputFigurePlainTextEdit::getStatusMessage() const{
    int col = this->textCursor().positionInBlock() + 1;
    QString column = "col:" + QString::number(col).rightJustified(4) + " ";

    QTextBlock current = this->textCursor().block();
    QTextBlock block = this->document()->firstBlock();
    OutputFigureStatus type = OutputFigureStatus::None;
    QString errorMessage;
    while(block.isValid()){
        QSharedPointer<LineOutputFigure> condition = OutputFigureReader::MatchLineCondition(block.text(), type, &errorMessage);
        if (block.blockNumber() >= current.blockNumber() && errorMessage == QString{}){
            if (condition == nullptr)
                return column;
            return column + condition->getStatusBar(col);
        }
        if (condition != nullptr){
            if (condition->getStatus() != OutputFigureStatus::Skip)
                type = condition->getStatus();
        }

        if (type == OutputFigureStatus::FinalEnd)
            break;

        block = block.next();
    }

    return column;
}

QList<BASE_TYPE_PROPERTY*> OutputFigurePlainTextEdit::getTypeProperties(QString *errorMessage) const{
    auto lines = this->document()->toPlainText().split("\n");
    OutputFigureReader reader;
    return reader.appParse(lines, errorMessage);
}

} // namespace post3dapp

