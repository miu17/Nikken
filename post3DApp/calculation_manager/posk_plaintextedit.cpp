#include "posk_plaintextedit.h"

#include <QPainter>
#include <QTextBlock>

#include "abstractwrite.h"
#include "calculationreader.h"
#include "linecalculation.h"
#include "textdataunit.h"

namespace post3dapp {

PoskPlainTextEdit::PoskPlainTextEdit(QWidget *parent)
    :CustomPlainTextEdit(parent)
{
    this->setFont(QFont("Courier", 8));
    fontWidth = fontMetrics().horizontalAdvance('8');
    fontHeight = fontMetrics().height();
    rowCountWidth = fontWidth*6;
    rowCountAreaWidth = fontWidth*8;
    setViewportMargins(rowCountAreaWidth,0,0,0);

    // temp
    QString text;
    text.append(u8"*COVER").append("\n");
    text.append(u8"*ORGANIZE").append("\n");
    text.append(u8"*OUTLINE").append("\n");
    text.append(u8"*PLAN").append("\n");
    text.append(u8"B1        1         PHR").append("\n");
    text.append(u8"*SECTION").append("\n");
    text.append(u8"X1        X2        X3").append("\n");
    text.append(u8"Y1        Y2        Y3").append("\n");
    text.append(u8"*LSGIR").append("\n");
    text.append(u8"*LSCOL").append("\n");
    text.append(u8"*LSWAL").append("\n");
    text.append(u8"*LSSWA").append("\n");
    text.append(u8"*LSFWA").append("\n");
    text.append(u8"*LSBRA").append("\n");
    text.append(u8"*LSBEAM").append("\n");
    text.append(u8"*LSSLAB").append("\n");
    text.append(u8"*LSOPEN").append("\n");
    text.append(u8"*LSBS").append("\n");
    text.append(u8"*LSBSRC").append("\n");
    text.append(u8"*LSDAMP").append("\n");
    text.append(u8"*LSMEN").append("\n");
    text.append(u8"*ALIST").append("\n");
    text.append(u8"*CALC").append("\n");
    text.append(u8"*STND").append("\n");
    text.append(u8"*POLICY").append("\n");
    text.append(u8"*STT").append("\n");
    text.append(u8"*MATC").append("\n");
    text.append(u8"*MATR").append("\n");
    text.append(u8"*MATS").append("\n");
    text.append(u8"*CCS").append("\n");
    text.append(u8"CVL       RSTDL     1.0       RSTLL1    1.0").append("\n");
    text.append(u8"CE1       RSTE1     1.0").append("\n");
    text.append(u8"CE2       RSTE2     1.0").append("\n");
    text.append(u8"*POLICYLD").append("\n");
    text.append(u8"*LDL").append("\n");
    text.append(u8"*LDF").append("\n");
    text.append(u8"*LPLAN").append("\n");
    text.append(u8"B1        1         PHR").append("\n");
    text.append(u8"*SEISLE").append("\n");
    text.append(u8"*LCMQP").append("\n");
    text.append(u8"CVL       B1        1         PHR").append("\n");
    text.append(u8"*LCMQS").append("\n");
    text.append(u8"CVL       X1        X2        X3").append("\n");
    text.append(u8"CVL       Y1        Y2        Y3").append("\n");
    text.append(u8"*JMASSP").append("\n");
    text.append(u8"B1        1         PHR").append("\n");
    text.append(u8"*JMASSS").append("\n");
    text.append(u8"X1        X2        X3").append("\n");
    text.append(u8"Y1        Y2        Y3").append("\n");
    text.append(u8"*POLICYEC").append("\n");
    text.append(u8"*CMODEL").append("\n");
    text.append(u8"*EMODEL").append("\n");
    text.append(u8"*MODELP").append("\n");
    text.append(u8"MODEL2    B1        1         PHR").append("\n");
    text.append(u8"*MODELS").append("\n");
    text.append(u8"MODEL2    X1        X2        X3        Y1        Y2        Y3").append("\n");
    text.append(u8"*MPHAIP").append("\n");
    text.append(u8"MODEL2    B1        1         PHR").append("\n");
    text.append(u8"*MPHAIS").append("\n");
    text.append(u8"MODEL2    X1        X2        X3        Y1        Y2        Y3").append("\n");
    text.append(u8"*MRLP").append("\n");
    text.append(u8"MODEL2    B1        1         PHR").append("\n");
    text.append(u8"*MRLS").append("\n");
    text.append(u8"MODEL2    X1        X2        X3        Y1        Y2        Y3").append("\n");
    text.append(u8"*MCLP").append("\n");
    text.append(u8"MODEL2    B1        1         PHR").append("\n");
    text.append(u8"*MCLS").append("\n");
    text.append(u8"MODEL2    X1        X2        X3        Y1        Y2        Y3").append("\n");
    text.append(u8"*MBLP").append("\n");
    text.append(u8"MODEL2    B1        1         PHR").append("\n");
    text.append(u8"*MBLS").append("\n");
    text.append(u8"MODEL2    X1        X2        X3        Y1        Y2        Y3").append("\n");
    text.append(u8"*ANACON").append("\n");
    text.append(u8"*RFLOOR").append("\n");
    text.append(u8"*JSPRING").append("\n");
    text.append(u8"*MSPRING").append("\n");
    text.append(u8"*MDETAIL").append("\n");
    text.append(u8"*AUTOPHI").append("\n");
    text.append(u8"*AUTOLK").append("\n");
    text.append(u8"*JNT").append("\n");
    text.append(u8"*INDEX").append("\n");
    text.append(u8"*FRAME").append("\n");
    text.append(u8"*PNLMDL").append("\n");
    text.append(u8"*OUTCS").append("\n");
    text.append(u8"*VJSTRP").append("\n");
    text.append(u8"CVL       B1        1         PHR").append("\n");
    text.append(u8"*VJSTRS").append("\n");
    text.append(u8"CVL       X1        X2        X3").append("\n");
    text.append(u8"CVL       Y1        Y2        Y3").append("\n");
    text.append(u8"*VFSTRP").append("\n");
    text.append(u8"CVL       B1        1         PHR").append("\n");
    text.append(u8"*VFSTRS").append("\n");
    text.append(u8"CVL       X1        X2        X3").append("\n");
    text.append(u8"CVL       Y1        Y2        Y3").append("\n");
    text.append(u8"*HRATIO").append("\n");
    text.append(u8"*HJSTRP").append("\n");
    text.append(u8"CE1       B1        1         PHR").append("\n");
    text.append(u8"CE2       B1        1         PHR").append("\n");
    text.append(u8"*HJSTRS").append("\n");
    text.append(u8"CE1       X1        X2        X3").append("\n");
    text.append(u8"CE2       X1        X2        X3").append("\n");
    text.append(u8"*HFSTRP").append("\n");
    text.append(u8"CE1       B1        1         PHR").append("\n");
    text.append(u8"CE2       B1        1         PHR").append("\n");
    text.append(u8"*HFSTRS").append("\n");
    text.append(u8"CE1       X1        X2        X3").append("\n");
    text.append(u8"CE2       X1        X2        X3").append("\n");
    text.append(u8"*CREZ").append("\n");
    text.append(u8"CVL       CE1       CE2       B1").append("\n");
    text.append(u8"*REACT").append("\n");
    text.append(u8"CVL       B1").append("\n");
    text.append(u8"CE1       B1").append("\n");
    text.append(u8"CE2       B1").append("\n");
    text.append(u8"*STSTIFF").append("\n");
    text.append(u8"*FRSTIFF").append("\n");
    text.append(u8"*FRDIST").append("\n");
    text.append(u8"*QMAP").append("\n");
    text.append(u8"CE1       B1        1").append("\n");
    text.append(u8"CE2       B1        1").append("\n");
    text.append(u8"*POLICYCH").append("\n");
    text.append(u8"*CHCASE").append("\n");
    text.append(u8"*CHVP").append("\n");
    text.append(u8"CVL       B1        1         PHR").append("\n");
    text.append(u8"*CHVS").append("\n");
    text.append(u8"CVL       X1        X2        X3").append("\n");
    text.append(u8"CVL       Y1        Y2        Y3").append("\n");
    text.append(u8"*CHHP").append("\n");
    text.append(u8"CE1       B1        1         PHR").append("\n");
    text.append(u8"CE2       B1        1         PHR").append("\n");
    text.append(u8"*CHLISTM").append("\n");
    text.append(u8"*CHLISTQ").append("\n");
    text.append(u8"*CHCOL").append("\n");
    text.append(u8"C                             VL        VL+E1     VL-E1     VL+E2     VL-E2").append("\n");
    text.append(u8"                    B1    1").append("\n");
    text.append(u8"*CHGIR").append("\n");
    text.append(u8"C                             VL        VL+E1     VL-E1").append("\n");
    text.append(u8"                    B1    1").append("\n");
    text.append(u8"*POLICYSTEC").append("\n");
    text.append(u8"*MXDFRM").append("\n");
    text.append(u8"*PRESTEC").append("\n");
    text.append(u8"*GCKCP").append("\n");
    text.append(u8"B1        1         PHR").append("\n");
    text.append(u8"*STECF").append("\n");
    text.append(u8"*CWAREA").append("\n");
    text.append(u8"*QUDS").append("\n");
    text.append(u8"CASE      XP").append("\n");
    text.append(u8"FLOOR     B1        1").append("\n");
    text.append(u8"GRID      X1        X2        X3").append("\n");
    text.append(u8"CASE      YP").append("\n");
    text.append(u8"FLOOR     B1        1").append("\n");
    text.append(u8"GRID      Y1        Y2        Y3").append("\n");
    text.append(u8"CASE      XM").append("\n");
    text.append(u8"FLOOR     B1        1").append("\n");
    text.append(u8"GRID      X1        X2        X3").append("\n");
    text.append(u8"CASE      YM").append("\n");
    text.append(u8"FLOOR     B1        1").append("\n");
    text.append(u8"GRID      Y1        Y2        Y3").append("\n");
    text.append(u8"*DSSTR").append("\n");
    text.append(u8"*MRNK").append("\n");
    text.append(u8"*QUN").append("\n");
    text.append(u8"*DSSH").append("\n");
    text.append(u8"*QUPUSH").append("\n");
    text.append(u8"*QUCURVE").append("\n");
    text.append(u8"*QUSTR").append("\n");
    text.append(u8"*QUSH").append("\n");
    text.append(u8"*DSQUCURVE").append("\n");
    text.append(u8"*QUNEEDF").append("\n");
    text.append(u8"*MESSAGE").append("\n");
    text.append(u8"*CONTENTS").append("\n");
    text.append(u8"*END").append("\n");
    this->setPlainText(text);
}

void PoskPlainTextEdit::decorateCells() const
{
    QTextBlock block = this->document()->firstBlock();
    CalculationStatus type = CalculationStatus::None;
    while(block.isValid()){
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        if (top >= viewport()->rect().bottom()){
            break;
        }

        bool isOk;
        LineCalculation* condition = CalculationReader::MatchLineCondition(block.text(), type, &isOk);
        if (condition != nullptr){
            if (condition->getStatus() != CalculationStatus::Skip)
                type = condition->getStatus();

            if (top >= 0 && isOk){
                Q_FOREACH(auto unit, condition->getUnits()){
                    decorateUnit(block, unit);
                }
            }
        }

        if (type == CalculationStatus::End)
            return;

        block = block.next();
    }
}

bool PoskPlainTextEdit::eventFilter(QObject *obj,QEvent *event)
{
    if(obj == rowCounter && (event->type() == QEvent::Paint)){
        QPainter painter(rowCounter);
        painter.fillRect(rowCounter->rect(), Qt::lightGray);

        QTextBlock block = this->document()->firstBlock();
        CalculationStatus type = CalculationStatus::None;
        bool skipCheck = false;
        while(block.isValid()){
            int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
            if( top >= viewport()->rect().bottom() ){
                break;
            }

            bool isOk = true;
            LineCalculation* condition = CalculationReader::CheckLineCondition(block.text(), type, &isOk);
            if (condition != nullptr){
                if (condition->getStatus() != CalculationStatus::Skip)
                    type = condition->getStatus();
            }

            if (type == CalculationStatus::End)
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


bool PoskPlainTextEdit::hasFormatError() const{
    return CalculationReader::hasFormatError(toPlainText());
}

QString PoskPlainTextEdit::getStatusMessage() const{
    int col = this->textCursor().positionInBlock() + 1;
    QString column = "col:" + QString::number(col).rightJustified(4) + " ";

    QTextBlock current = this->textCursor().block();
    QTextBlock block = this->document()->firstBlock();
    CalculationStatus type = CalculationStatus::None;
    bool isOk = true;
    while(block.isValid()){
        LineCalculation* condition = CalculationReader::MatchLineCondition(block.text(), type, &isOk);
        if (block.blockNumber() >= current.blockNumber() && isOk == true){
            if (condition == nullptr)
                return column;
            return column + condition->getStatusBar(col);
        }
        if (condition != nullptr){
            if (condition->getStatus() != CalculationStatus::Skip)
                type = condition->getStatus();
        }

        if (type == CalculationStatus::End)
            break;

        block = block.next();
    }

    return column;
}

} // namespace post3dapp
