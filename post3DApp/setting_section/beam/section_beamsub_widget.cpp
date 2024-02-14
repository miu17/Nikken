#include "section_beamsub_widget.h"

#include <QtGui>
#include <QDebug>

#include "define_defaultvalue.h"
#include "define_section_struct.h"
#include "section_tableview.h"
#include "unified_inputdata.h"

namespace post3dapp{
void SectionBeamRCWidget::recalcDefaultValue()
{
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    for (int row = 0; row < myTableModel->rowCount(); row++) {
        inputStandardValuesRC(myTableModel->index(row, coltag["iMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["mMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["jMAINBARPHI"]));
    }
}

void SectionBeamRCWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPBEAM)){
        if (sv.sectionType != STRUCTTYPE::STP_RC)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(sv.line_param.isCantilever ? REGULATION::defaultGirderCantilever.at(2) : REGULATION::defaultGirderCantilever.at(1)));
        data.append(QVariant(QString::number(sv.line_param.finishLoad)));
        data.append(QVariant(sv.line_param.concMaterial));
        data.append(QVariant(QString::number(sv.i_param.Dy)));
        data.append(QVariant(QString::number(sv.i_param.Dz)));
        data.append(QVariant(sv.i_param.mainBarArrange1.text()));
        data.append(QVariant(sv.i_param.mainBarArrange2.text()));
        data.append(QVariant(sv.i_param.mainBar1));
        data.append(QVariant(sv.i_param.mainBar2));
        data.append(QVariant(sv.i_param.hoopZ));
        data.append(QVariant(sv.i_param.hoopBar));
        data.append(QVariant(QString::number(sv.i_param.hoopPitch)));
        data.append(QVariant(QString::number(sv.i_param.RK)));
        data.append(QVariant(QString::number(sv.i_param.RKu)));
        data.append(QVariant(QString::number(sv.i_param.RKb)));
        data.append(QVariant(QString::number(sv.i_param.RY)));
        data.append(QVariant(QString::number(sv.c_param.Dy)));
        data.append(QVariant(QString::number(sv.c_param.Dz)));
        data.append(QVariant(sv.c_param.mainBarArrange1.text()));
        data.append(QVariant(sv.c_param.mainBarArrange2.text()));
        data.append(QVariant(sv.c_param.mainBar1));
        data.append(QVariant(sv.c_param.mainBar2));
        data.append(QVariant(sv.c_param.hoopZ));
        data.append(QVariant(sv.c_param.hoopBar));
        data.append(QVariant(QString::number(sv.c_param.hoopPitch)));
        data.append(QVariant(QString::number(sv.c_param.RK)));
        data.append(QVariant(QString::number(sv.c_param.RKu)));
        data.append(QVariant(QString::number(sv.c_param.RKb)));
        data.append(QVariant(QString::number(sv.c_param.RY)));
        data.append(QVariant(QString::number(sv.j_param.Dy)));
        data.append(QVariant(QString::number(sv.j_param.Dz)));
        data.append(QVariant(sv.j_param.mainBarArrange1.text()));
        data.append(QVariant(sv.j_param.mainBarArrange2.text()));
        data.append(QVariant(sv.j_param.mainBar1));
        data.append(QVariant(sv.j_param.mainBar2));
        data.append(QVariant(sv.j_param.hoopZ));
        data.append(QVariant(sv.j_param.hoopBar));
        data.append(QVariant(QString::number(sv.j_param.hoopPitch)));
        data.append(QVariant(QString::number(sv.j_param.RK)));
        data.append(QVariant(QString::number(sv.j_param.RKu)));
        data.append(QVariant(QString::number(sv.j_param.RKb)));
        data.append(QVariant(QString::number(sv.j_param.RY)));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionBeamRCWidget::getDefaultFilters() const{
    QList<bool> rcDefault;
    rcDefault << true; // NO
    rcDefault << true; // LABEL
    rcDefault << true; // CANTILEVER
    rcDefault << true; // FINISHLOAD
    rcDefault << true; // CONC
    rcDefault << true; // iDX
    rcDefault << true; // iDY
    rcDefault << true; // iMAINBAR1
    rcDefault << true; // iMAINBAR2
    rcDefault << true; // iMAINBARPHI1
    rcDefault << false; // iMAINBARPHI2
    rcDefault << true; // iHOOPY
    rcDefault << true; // iHOOPPHI
    rcDefault << true; // iHOOPPITCH
    rcDefault << false; // iRK
    rcDefault << false; // iRKU
    rcDefault << false; // iRKB
    rcDefault << false; // iRY
    rcDefault << true; // mDX
    rcDefault << true; // mDY
    rcDefault << true; // mMAINBAR1
    rcDefault << true; // mMAINBAR2
    rcDefault << true; // mMAINBARPHI1
    rcDefault << false; // mMAINBARPHI2
    rcDefault << true; // mHOOPY
    rcDefault << true; // mHOOPPHI
    rcDefault << true; // mHOOPPITCH
    rcDefault << false; // mRK
    rcDefault << false; // mRKU
    rcDefault << false; // mRKB
    rcDefault << false; // mRY
    rcDefault << true; // jDX
    rcDefault << true; // jDY
    rcDefault << true; // jMAINBAR1
    rcDefault << true; // jMAINBAR2
    rcDefault << true; // jMAINBARPHI1
    rcDefault << false; // jMAINBARPHI2
    rcDefault << true; // jHOOPY
    rcDefault << true; // jHOOPPHI
    rcDefault << true; // jHOOPPITCH
    rcDefault << false; // jRK
    rcDefault << false; // jRKU
    rcDefault << false; // jRKB
    rcDefault << false; // jRY
    return rcDefault;
}

void SectionBeamSWidget::createViews()
{
    SectionBeamWidget::createViews();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL"], 150);
    tableView_2->setColumnWidth(coltag["mSHAPELISTNAME"], 150);
    tableView_2->setColumnWidth(coltag["mSTANDARDSHAPESTEEL"], 150);
    tableView_3->setColumnWidth(coltag["jSHAPELISTNAME"], 150);
    tableView_3->setColumnWidth(coltag["jSTANDARDSHAPESTEEL"], 150);
}

void SectionBeamSWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPBEAM)){
        if (sv.sectionType != STRUCTTYPE::STP_S)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(sv.line_param.isCantilever ? REGULATION::defaultGirderCantilever.at(2) : REGULATION::defaultGirderCantilever.at(1)));
        data.append(QVariant(QString::number(sv.line_param.finishLoad)));
        data.append(QVariant(QString::number(sv.line_param.i_length)));
        data.append(QVariant(QString::number(sv.line_param.j_length)));
        data.append(QVariant(sv.i_param.steelMaterial));
        data.append(QVariant(sv.i_param.standardName));
        data.append(QVariant(SteelType::enumToString(sv.i_param.steelType)));
        data.append(QVariant(QString::number(sv.i_param.sizeH)));
        data.append(QVariant(QString::number(sv.i_param.sizeB)));
        data.append(QVariant(QString::number(sv.i_param.t1)));
        data.append(QVariant(QString::number(sv.i_param.t2)));
        data.append(QVariant(sv.c_param.steelMaterial));
        data.append(QVariant(sv.c_param.standardName));
        data.append(QVariant(SteelType::enumToString(sv.c_param.steelType)));
        data.append(QVariant(QString::number(sv.c_param.sizeH)));
        data.append(QVariant(QString::number(sv.c_param.sizeB)));
        data.append(QVariant(QString::number(sv.c_param.t1)));
        data.append(QVariant(QString::number(sv.c_param.t2)));
        data.append(QVariant(sv.j_param.steelMaterial));
        data.append(QVariant(sv.j_param.standardName));
        data.append(QVariant(SteelType::enumToString(sv.j_param.steelType)));
        data.append(QVariant(QString::number(sv.j_param.sizeH)));
        data.append(QVariant(QString::number(sv.j_param.sizeB)));
        data.append(QVariant(QString::number(sv.j_param.t1)));
        data.append(QVariant(QString::number(sv.j_param.t2)));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionBeamSWidget::getDefaultFilters() const{
    QList<bool> sDefault;
    sDefault << true; // NO
    sDefault << true; // LABEL
    sDefault << true; // CANTILEVER
    sDefault << true; // FINISHLOAD
    sDefault << true; // iLENGTH
    sDefault << true; // jLENGTH
    sDefault << true; // iSTEEL
    sDefault << false; // iSTEELWEB
    sDefault << true; // iSHAPELISTNAME
    sDefault << true; // iSTANDARDSHAPESTEEL
    sDefault << true; // iH
    sDefault << true; // iB
    sDefault << true; // iT1
    sDefault << true; // iT2
    sDefault << false; // mSTEEL
    sDefault << false; // mSTEELWEB
    sDefault << false; // mSHAPELISTNAME
    sDefault << false; // mSTANDARDSHAPESTEEL
    sDefault << false; // mH
    sDefault << false; // mB
    sDefault << false; // mT1
    sDefault << false; // mT2
    sDefault << false; // jSTEEL
    sDefault << false; // jSTEELWEB
    sDefault << false; // jSHAPELISTNAME
    sDefault << false; // jSTANDARDSHAPESTEEL
    sDefault << false; // jH
    sDefault << false; // jB
    sDefault << false; // jT1
    sDefault << false; // jT2
    return sDefault;
}

void SectionBeamSRCWidget::createViews()
{
    SectionBeamWidget::createViews();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL"], 150);
    tableView_2->setColumnWidth(coltag["mSHAPELISTNAME"], 150);
    tableView_2->setColumnWidth(coltag["mSTANDARDSHAPESTEEL"], 150);
    tableView_3->setColumnWidth(coltag["jSHAPELISTNAME"], 150);
    tableView_3->setColumnWidth(coltag["jSTANDARDSHAPESTEEL"], 150);
}

void SectionBeamSRCWidget::recalcDefaultValue()
{
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    for (int row = 0; row < myTableModel->rowCount(); row++) {
        inputStandardValuesRC(myTableModel->index(row, coltag["iMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["mMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["jMAINBARPHI"]));
    }
}

void SectionBeamSRCWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPBEAM)){
        if (sv.sectionType != STRUCTTYPE::STP_SRC)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(sv.line_param.isCantilever ? REGULATION::defaultGirderCantilever.at(2) : REGULATION::defaultGirderCantilever.at(1)));
        data.append(QVariant(QString::number(sv.line_param.finishLoad)));
        data.append(QVariant(sv.line_param.concMaterial));
        data.append(QVariant(QString::number(sv.line_param.i_length)));
        data.append(QVariant(QString::number(sv.line_param.j_length)));
        data.append(QVariant(QString::number(sv.i_param.Dy)));
        data.append(QVariant(QString::number(sv.i_param.Dz)));
        data.append(QVariant(sv.i_param.mainBarArrange1.text()));
        data.append(QVariant(sv.i_param.mainBarArrange2.text()));
        data.append(QVariant(sv.i_param.mainBar1));
        data.append(QVariant(sv.i_param.mainBar2));
        data.append(QVariant(sv.i_param.hoopZ));
        data.append(QVariant(sv.i_param.hoopBar));
        data.append(QVariant(QString::number(sv.i_param.hoopPitch)));
        data.append(QVariant(QString::number(sv.i_param.RK)));
        data.append(QVariant(QString::number(sv.i_param.RKu)));
        data.append(QVariant(QString::number(sv.i_param.RKb)));
        data.append(QVariant(QString::number(sv.i_param.RY)));
        data.append(QVariant(QString::number(sv.i_param.RY1)));
        data.append(QVariant(sv.i_param.steelMaterial));
        data.append(QVariant(sv.i_param.standardName));
        data.append(QVariant(SteelType::enumToString(sv.i_param.steelType)));
        data.append(QVariant(QString::number(sv.i_param.sizeH)));
        data.append(QVariant(QString::number(sv.i_param.sizeB)));
        data.append(QVariant(QString::number(sv.i_param.t1)));
        data.append(QVariant(QString::number(sv.i_param.t2)));
        data.append(QVariant(QString::number(sv.i_param.cY)));
        data.append(QVariant(QString::number(sv.i_param.cZ)));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionBeamSRCWidget::getDefaultFilters() const{
    QList<bool> srcDefault;
    srcDefault << true; // NO
    srcDefault << true; // LABEL
    srcDefault << true; // CANTILEVER
    srcDefault << true; // FINISHLOAD
    srcDefault << true; // CONC
    srcDefault << true; // iLENGTH
    srcDefault << true; // jLENGTH
    srcDefault << true; // iDX
    srcDefault << true; // iDY
    srcDefault << true; // iMAINBAR1
    srcDefault << true; // iMAINBAR2
    srcDefault << true; // iMAINBARPHI1
    srcDefault << false; // iMAINBARPHI2
    srcDefault << true; // iHOOPY
    srcDefault << true; // iHOOPPHI
    srcDefault << true; // iHOOPPITCH
    srcDefault << true; // iRK
    srcDefault << true; // iRKU
    srcDefault << true; // iRKB
    srcDefault << true; // iRY
    srcDefault << true; // iRY1
    srcDefault << true; // iSTEEL
    srcDefault << false; // iSTEELWEB
    srcDefault << true; // iSHAPELISTNAME
    srcDefault << true; // iSTANDARDSHAPESTEEL
    srcDefault << true; // iH
    srcDefault << true; // iB
    srcDefault << true; // iT1
    srcDefault << true; // iT2
    srcDefault << true; // iCOODINATEX
    srcDefault << true; // iCOODINATEY
    srcDefault << true; // mDX
    srcDefault << true; // mDY
    srcDefault << true; // mMAINBAR1
    srcDefault << true; // mMAINBAR2
    srcDefault << true; // mMAINBARPHI1
    srcDefault << false; // mMAINBARPHI2
    srcDefault << true; // mHOOPY
    srcDefault << true; // mHOOPPHI
    srcDefault << true; // mHOOPPITCH
    srcDefault << false; // mRK
    srcDefault << false; // mRKU
    srcDefault << false; // mRKB
    srcDefault << false; // mRY
    srcDefault << false; // mRY1
    srcDefault << true; // mSTEEL
    srcDefault << false; // mSTEELWEB
    srcDefault << true; // mSHAPELISTNAME
    srcDefault << true; // mSTANDARDSHAPESTEEL
    srcDefault << true; // mH
    srcDefault << true; // mB
    srcDefault << true; // mT1
    srcDefault << true; // mT2
    srcDefault << true; // mCOODINATEX
    srcDefault << true; // mCOODINATEY
    srcDefault << true; // jDX
    srcDefault << true; // jDY
    srcDefault << true; // jMAINBAR1
    srcDefault << true; // jMAINBAR2
    srcDefault << true; // jMAINBARPHI1
    srcDefault << false; // jMAINBARPHI2
    srcDefault << true; // jHOOPY
    srcDefault << true; // jHOOPPHI
    srcDefault << true; // jHOOPPITCH
    srcDefault << false; // jRK
    srcDefault << false; // jRKU
    srcDefault << false; // jRKB
    srcDefault << false; // jRY
    srcDefault << false; // jRY1
    srcDefault << true; // jSTEEL
    srcDefault << false; // jSTEELWEB
    srcDefault << true; // jSHAPELISTNAME
    srcDefault << true; // jSTANDARDSHAPESTEEL
    srcDefault << true; // jH
    srcDefault << true; // jB
    srcDefault << true; // jT1
    srcDefault << true; // jT2
    srcDefault << true; // jCOODINATEX
    srcDefault << true; // jCOODINATEY
    return srcDefault;
}
} // namespace post3dapp
