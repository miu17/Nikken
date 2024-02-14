#include "section_girsub_widget.h"

#include <QtGui>
#include <QDebug>

#include "define_defaultvalue.h"
#include "define_section_struct.h"
#include "section_tableview.h"
#include "unified_inputdata.h"

namespace post3dapp{
void SectionGirderRCWidget::recalcDefaultValue()
{
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    for (int row = 0; row < myTableModel->rowCount(); row++) {
        inputStandardValuesRC(myTableModel->index(row, coltag["iMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["mMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["jMAINBARPHI"]));
    }
}

void SectionGirderRCWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPGIRDER)){
        if (sv.sectionType != STRUCTTYPE::STP_RC)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.startFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.endFloorID, DATATYPE::TPFLOOR)));
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

QList<bool> SectionGirderRCWidget::getDefaultFilters() const{
    QList<bool> rcDefault;
    rcDefault << true; // NO
    rcDefault << true; // LABEL
    rcDefault << true; // FLOORFROM
    rcDefault << true; // FLOORTO
    rcDefault << true; // CANTILEVER
    rcDefault << true; // FINISHLOAD
    rcDefault << true; // CONC
    rcDefault << true; // iDX
    rcDefault << true; // iDY
    rcDefault << true; // iMAINBAR1
    rcDefault << true; // iMAINBARPHI1
    rcDefault << true; // iMAINBAR2
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
    rcDefault << true; // mMAINBARPHI1
    rcDefault << true; // mMAINBAR2
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
    rcDefault << true; // jMAINBARPHI1
    rcDefault << true; // jMAINBAR2
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

void SectionGirderSWidget::createViews()
{
    SectionGirderWidget::createViews();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL"], 150);
    tableView_2->setColumnWidth(coltag["mSHAPELISTNAME"], 150);
    tableView_2->setColumnWidth(coltag["mSTANDARDSHAPESTEEL"], 150);
    tableView_3->setColumnWidth(coltag["jSHAPELISTNAME"], 150);
    tableView_3->setColumnWidth(coltag["jSTANDARDSHAPESTEEL"], 150);
}

void SectionGirderSWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPGIRDER)){
        if (sv.sectionType != STRUCTTYPE::STP_S)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.startFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.endFloorID, DATATYPE::TPFLOOR)));
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

QList<bool> SectionGirderSWidget::getDefaultFilters() const{
    QList<bool> sDefault;
    sDefault << true; // NO
    sDefault << true; // LABEL
    sDefault << true; // FLOORFROM
    sDefault << true; // FLOORTO
    sDefault << true; // CANTILEVER
    sDefault << true; // FINISHLOAD
    sDefault << false; // iLENGTH
    sDefault << false; // jLENGTH
    sDefault << true; // iENDSTIFFENER
    sDefault << true; // jENDSTIFFENER
    sDefault << true; // iSTEEL
    sDefault << false; // iSTEELWEB
    sDefault << true; // iSHAPELISTNAME
    sDefault << true; // iSTANDARDSHAPESTEEL
    sDefault << true; // iH
    sDefault << true; // iB
    sDefault << true; // iT1
    sDefault << true; // iT2
    sDefault << true; // mSTEEL
    sDefault << false; // mSTEELWEB
    sDefault << true; // mSHAPELISTNAME
    sDefault << true; // mSTANDARDSHAPESTEEL
    sDefault << true; // mH
    sDefault << true; // mB
    sDefault << true; // mT1
    sDefault << true; // mT2
    sDefault << true; // jSTEEL
    sDefault << false; // jSTEELWEB
    sDefault << true; // jSHAPELISTNAME
    sDefault << true; // jSTANDARDSHAPESTEEL
    sDefault << true; // jH
    sDefault << true; // jB
    sDefault << true; // jT1
    sDefault << true; // jT2
    return sDefault;
}

void SectionGirderSRCWidget::createViews()
{
    SectionGirderWidget::createViews();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL"], 150);
    tableView_2->setColumnWidth(coltag["mSHAPELISTNAME"], 150);
    tableView_2->setColumnWidth(coltag["mSTANDARDSHAPESTEEL"], 150);
    tableView_3->setColumnWidth(coltag["jSHAPELISTNAME"], 150);
    tableView_3->setColumnWidth(coltag["jSTANDARDSHAPESTEEL"], 150);
}

void SectionGirderSRCWidget::recalcDefaultValue()
{
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    for (int row = 0; row < myTableModel->rowCount(); row++) {
        inputStandardValuesRC(myTableModel->index(row, coltag["iMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["mMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["jMAINBARPHI"]));
    }
}

void SectionGirderSRCWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPGIRDER)){
        if (sv.sectionType != STRUCTTYPE::STP_SRC)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.startFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.endFloorID, DATATYPE::TPFLOOR)));
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
        data.append(QVariant(static_cast<int>(sv.i_param.steelType)));
        data.append(QVariant(QString::number(sv.i_param.sizeH)));
        data.append(QVariant(QString::number(sv.i_param.sizeB)));
        data.append(QVariant(QString::number(sv.i_param.t1)));
        data.append(QVariant(QString::number(sv.i_param.t2)));
        data.append(QVariant(QString::number(sv.i_param.cY)));
        data.append(QVariant(QString::number(sv.i_param.cZ)));
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
        data.append(QVariant(QString::number(sv.c_param.RY1)));
        data.append(QVariant(sv.c_param.steelMaterial));
        data.append(QVariant(sv.c_param.standardName));
        data.append(QVariant(static_cast<int>(sv.c_param.steelType)));
        data.append(QVariant(QString::number(sv.c_param.sizeH)));
        data.append(QVariant(QString::number(sv.c_param.sizeB)));
        data.append(QVariant(QString::number(sv.c_param.t1)));
        data.append(QVariant(QString::number(sv.c_param.t2)));
        data.append(QVariant(QString::number(sv.c_param.cY)));
        data.append(QVariant(QString::number(sv.c_param.cZ)));
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
        data.append(QVariant(QString::number(sv.j_param.RY1)));
        data.append(QVariant(sv.j_param.steelMaterial));
        data.append(QVariant(sv.j_param.standardName));
        data.append(QVariant(static_cast<int>(sv.j_param.steelType)));
        data.append(QVariant(QString::number(sv.j_param.sizeH)));
        data.append(QVariant(QString::number(sv.j_param.sizeB)));
        data.append(QVariant(QString::number(sv.j_param.t1)));
        data.append(QVariant(QString::number(sv.j_param.t2)));
        data.append(QVariant(QString::number(sv.j_param.cY)));
        data.append(QVariant(QString::number(sv.j_param.cZ)));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionGirderSRCWidget::getDefaultFilters() const{
    QList<bool> srcDefault;
    srcDefault << true; // NO
    srcDefault << true; // LABEL
    srcDefault << true; // FLOORFROM
    srcDefault << true; // FLOORTO
    srcDefault << true; // CANTILEVER
    srcDefault << true; // FINISHLOAD
    srcDefault << true; // CONC
    srcDefault << false; // iLENGTH
    srcDefault << false; // jLENGTH
    srcDefault << true; // iDX
    srcDefault << true; // iDY
    srcDefault << true; // iMAINBAR1
    srcDefault << true; // iMAINBAR2
    srcDefault << true; // iMAINBARPHI1
    srcDefault << false; // iMAINBARPHI2
    srcDefault << true; // iHOOPY
    srcDefault << true; // iHOOPPHI
    srcDefault << true; // iHOOPPITCH
    srcDefault << false; // iRK
    srcDefault << false; // iRKU
    srcDefault << false; // iRKB
    srcDefault << false; // iRY
    srcDefault << false; // iRY1
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
    srcDefault << true; // mMAINBARPHI1
    srcDefault << true; // mMAINBAR2
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
    srcDefault << true; // jMAINBARPHI1
    srcDefault << true; // jMAINBAR2
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
