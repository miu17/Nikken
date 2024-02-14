#include "analysis_plaintextedit.h"

#include <QDebug>
#include <QPainter>
#include <QTextBlock>

#include "analysisconditionreader.h"
#include "linecondition.h"
#include "textdataunit.h"

namespace post3dapp {

AnalysisPlainTextEdit::AnalysisPlainTextEdit(QWidget *parent)
    :CustomPlainTextEdit(parent)
{
    this->setFont(QFont("Courier", 8));
    fontWidth = fontMetrics().horizontalAdvance('8');
    fontHeight = fontMetrics().height();
    rowCountWidth = fontWidth*6;
    rowCountAreaWidth = fontWidth*8;
    setViewportMargins(rowCountAreaWidth,0,0,0);
}

void AnalysisPlainTextEdit::decorateCells() const
{
    QTextBlock block = this->document()->firstBlock();
    ANALYSISSTEP step = ANALYSISSTEP::STEP_NONE;
    LINESTATUSTYPE type = LINESTATUSTYPE::NONE;
    while(block.isValid()){
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        if (top >= viewport()->rect().bottom()){
            break;
        }

        bool isOk;
        QSharedPointer<LineCondition> condition = AnalysisConditionReader::MatchLineCondition(block.text(), step, type, &isOk);
        if (condition != nullptr){
            step = condition->getStep();
            if (condition->getStatus() != LINESTATUSTYPE::SKIP)
                type = condition->getStatus();

            if (top >= 0 && isOk){
                Q_FOREACH(auto unit, condition->getUnits()){
                    decorateUnit(block, unit);
                }
            }
        }

        if (type == LINESTATUSTYPE::FINALEND)
            return;

        block = block.next();
    }
}

bool AnalysisPlainTextEdit::eventFilter(QObject *obj,QEvent *event)
{
    if(obj == rowCounter && (event->type() == QEvent::Paint)){
        QPainter painter(rowCounter);
        painter.fillRect(rowCounter->rect(), Qt::lightGray);

        QTextBlock block = this->document()->firstBlock();
        ANALYSISSTEP step = ANALYSISSTEP::STEP_NONE;
        LINESTATUSTYPE type = LINESTATUSTYPE::NONE;
        bool skipCheck = false;
        while(block.isValid()){
            int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
            if( top >= viewport()->rect().bottom() ){
                break;
            }

            bool isOk = true;
            QSharedPointer<LineCondition> condition = AnalysisConditionReader::CheckLineCondition(block.text(), step, type, &isOk);
            if (condition != nullptr){
                step = condition->getStep();
                if (condition->getStatus() != LINESTATUSTYPE::SKIP)
                    type = condition->getStatus();
            }

            if (type == LINESTATUSTYPE::FINALEND)
                skipCheck = true;
            if (top >= 0){
                drawRowNumber(block, &painter);
                if (!isOk && !skipCheck){
                    drawRedCircle(block, &painter);
                }
            }
            block = block.next();
        }

        return true;
    }
    return false;
}

bool AnalysisPlainTextEdit::hasFormatError() const{
    return AnalysisConditionReader::hasFormatError(toPlainText());
}

QString AnalysisPlainTextEdit::getStatusMessage() const{
    int col = this->textCursor().positionInBlock() + 1;
    QString column = "col:" + QString::number(col).rightJustified(4) + " ";

    QTextBlock current = this->textCursor().block();
    QTextBlock block = this->document()->firstBlock();
    ANALYSISSTEP step = ANALYSISSTEP::STEP_NONE;
    LINESTATUSTYPE type = LINESTATUSTYPE::NONE;
    bool isOk = true;
    while(block.isValid()){
        QSharedPointer<LineCondition> condition = AnalysisConditionReader::MatchLineCondition(block.text(), step, type, &isOk);
        if (block.blockNumber() >= current.blockNumber() && isOk == true){
            if (condition == nullptr)
                return column;
            return column + condition->getStatusBar(col);
        }

        if (condition != nullptr){
            step = condition->getStep();
            if (condition->getStatus() != LINESTATUSTYPE::SKIP)
                type = condition->getStatus();
        }

        if (type == LINESTATUSTYPE::FINALEND)
            break;

        block = block.next();
    }

    return column;
}
} // namespace post3dapp
