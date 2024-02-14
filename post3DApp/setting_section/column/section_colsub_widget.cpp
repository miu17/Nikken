#include <QtGui>
#include <QDebug>

#include "section_colsub_widget.h"
#include "unified_inputdata.h"
#include "define_section_struct.h"
#include "section_tableview.h"

namespace post3dapp{
/* 柱断面：RC */
void SectionColumnRCWidget::recalcDefaultValue()
{
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    for (int row = 0; row < myTableModel->rowCount(); row++) {
        inputStandardValuesRC(myTableModel->index(row, coltag["iMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["jMAINBARPHI"]));
    }
}

void SectionColumnRCWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPCOLUMN)){
        if (sv.sectionType != STRUCTTYPE::STP_RC)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.startFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.endFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(QString::number(sv.line_param.finishLoad)));
        data.append(QVariant(sv.line_param.concMaterial));
        data.append(QVariant(QString::number(sv.i_param.Dy)));
        data.append(QVariant(QString::number(sv.i_param.Dz)));
        data.append(QVariant(sv.i_param.mainBarArrange1.reflectColumnText()));
        data.append(QVariant(sv.i_param.mainBarArrange2.reflectColumnText()));
        data.append(QVariant(sv.i_param.mainBar1));
        data.append(QVariant(QString::number(sv.i_param.axialCenterY)));
        data.append(QVariant(QString::number(sv.i_param.axialCenterZ)));
        data.append(QVariant(QString::number(sv.i_param.axialDy)));
        data.append(QVariant(QString::number(sv.i_param.axialDz)));
        data.append(QVariant(sv.i_param.axialBarArrange1.reflectColumnText()));
        data.append(QVariant(sv.i_param.axialBarArrange2.reflectColumnText()));
        data.append(QVariant(sv.i_param.axialBar));
        data.append(QVariant(QString::number(sv.i_param.hoopY)));
        data.append(QVariant(QString::number(sv.i_param.hoopZ)));
        data.append(QVariant(sv.i_param.hoopBar));
        data.append(QVariant(QString::number(sv.i_param.hoopPitch)));
        data.append(QVariant(QString::number(sv.i_param.RK)));
        data.append(QVariant(QString::number(sv.i_param.RY)));
        data.append(QVariant(QString::number(sv.j_param.Dy)));
        data.append(QVariant(QString::number(sv.j_param.Dz)));
        data.append(QVariant(sv.j_param.mainBarArrange1.reflectColumnText()));
        data.append(QVariant(sv.j_param.mainBarArrange2.reflectColumnText()));
        data.append(QVariant(sv.j_param.mainBar1));
        data.append(QVariant(QString::number(sv.j_param.axialCenterY)));
        data.append(QVariant(QString::number(sv.j_param.axialCenterZ)));
        data.append(QVariant(QString::number(sv.j_param.axialDy)));
        data.append(QVariant(QString::number(sv.j_param.axialDz)));
        data.append(QVariant(sv.j_param.axialBarArrange1.reflectColumnText()));
        data.append(QVariant(sv.j_param.axialBarArrange2.reflectColumnText()));
        data.append(QVariant(sv.j_param.axialBar));
        data.append(QVariant(QString::number(sv.j_param.hoopY)));
        data.append(QVariant(QString::number(sv.j_param.hoopZ)));
        data.append(QVariant(sv.j_param.hoopBar));
        data.append(QVariant(QString::number(sv.j_param.hoopPitch)));
        data.append(QVariant(QString::number(sv.j_param.RK)));
        data.append(QVariant(QString::number(sv.j_param.RY)));
        temp.appendRowData(data);
        i++;
    }

    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionColumnRCWidget::getDefaultFilters() const{
    QList<bool> rcDefault;
    rcDefault << true; // NO
    rcDefault << true; // LABEL
    rcDefault << true; // FLOORFROM
    rcDefault << true; // FLOORTO
    rcDefault << true; // FINISHLOAD
    rcDefault << true; // CONC
    rcDefault << true; // iDX
    rcDefault << true; // iDY
    rcDefault << true; // iMAINBAR1
    rcDefault << true; // iMAINBAR2
    rcDefault << true; // iMAINBARPHI
    rcDefault << false; // iAXIALCENTERX
    rcDefault << false; // iAXIALCENTERY
    rcDefault << false; // iAXIALDX
    rcDefault << false; // iAXIALDY
    rcDefault << false; // iAXIALBAR1
    rcDefault << false; // iAXIALBAR2
    rcDefault << false; // iAXIALBARPHI
    rcDefault << true; // iHOOPX
    rcDefault << true; // iHOOPY
    rcDefault << true; // iHOOPPHI
    rcDefault << true; // iHOOPPITCH
    rcDefault << false; // iRK
    rcDefault << false; // iRY
    rcDefault << true; // jDX
    rcDefault << true; // jDY
    rcDefault << true; // jMAINBAR1
    rcDefault << true; // jMAINBAR2
    rcDefault << true; // jMAINBARPHI
    rcDefault << false; // jAXIALCENTERX
    rcDefault << false; // jAXIALCENTERY
    rcDefault << false; // jAXIALDX
    rcDefault << false; // jAXIALDY
    rcDefault << false; // jAXIALBAR1
    rcDefault << false; // jAXIALBAR2
    rcDefault << false; // jAXIALBARPHI
    rcDefault << true; // jHOOPX
    rcDefault << true; // jHOOPY
    rcDefault << true; // jHOOPPHI
    rcDefault << true; // jHOOPPITCH
    rcDefault << false; // jRK
    rcDefault << false; // jRY
    return rcDefault;
}

/* 柱断面：S */

void SectionColumnSWidget::createViews()
{
    SectionColumnWidget::createViews();
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL"], 150);
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME_T"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL_T"], 150);
    tableView_2->setColumnWidth(coltag["jSHAPELISTNAME"], 150);
    tableView_2->setColumnWidth(coltag["jSTANDARDSHAPESTEEL"], 150);
    tableView_2->setColumnWidth(coltag["jSHAPELISTNAME_T"], 150);
    tableView_2->setColumnWidth(coltag["jSTANDARDSHAPESTEEL_T"], 150);
}

void SectionColumnSWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPCOLUMN)){
        if (sv.sectionType != STRUCTTYPE::STP_S)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.startFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.endFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(QString::number(sv.line_param.finishLoad)));
        data.append(QVariant(QString::number(sv.line_param.i_length)));
        data.append(QVariant(sv.line_param.basePlate));
        data.append(QVariant(sv.i_param.steelMaterial));
        data.append(QVariant(SteelType::enumToString(sv.i_param.steelType)));
        data.append(QVariant(sv.i_param.standardName));
        data.append(QVariant(QString::number(sv.i_param.sizeH)));
        data.append(QVariant(QString::number(sv.i_param.sizeB)));
        data.append(QVariant(QString::number(sv.i_param.t1)));
        data.append(QVariant(QString::number(sv.i_param.t2)));
        data.append(QVariant(QString::number(sv.i_param.H_T)));
        data.append(QVariant(QString::number(sv.i_param.B_T)));
        data.append(QVariant(QString::number(sv.i_param.t1_T)));
        data.append(QVariant(QString::number(sv.i_param.t2_T)));
        data.append(QVariant(sv.j_param.steelMaterial));
        data.append(QVariant(SteelType::enumToString(sv.j_param.steelType)));
        data.append(QVariant(sv.j_param.standardName));
        data.append(QVariant(QString::number(sv.j_param.sizeH)));
        data.append(QVariant(QString::number(sv.j_param.sizeB)));
        data.append(QVariant(QString::number(sv.j_param.t1)));
        data.append(QVariant(QString::number(sv.j_param.t2)));
        data.append(QVariant(QString::number(sv.j_param.H_T)));
        data.append(QVariant(QString::number(sv.j_param.B_T)));
        data.append(QVariant(QString::number(sv.j_param.t1_T)));
        data.append(QVariant(QString::number(sv.j_param.t2_T)));
        temp.appendRowData(data);
        i++;
    }

    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionColumnSWidget::getDefaultFilters() const{
    QList<bool> sDefault;
    sDefault << true; // NO
    sDefault << true; // LABEL
    sDefault << true; // FLOORFROM
    sDefault << true; // FLOORTO
    sDefault << true; // FINISHLOAD
    sDefault << false; // iLENGTH
    sDefault << true; // BASELIST
    sDefault << true; // iSTEEL
    sDefault << true; // iSHAPELISTNAME
    sDefault << true; // iSTANDARDSHAPESTEEL
    sDefault << true; // iH
    sDefault << true; // iB
    sDefault << true; // iT1
    sDefault << true; // iT2
    sDefault << true; // iH_T
    sDefault << true; // iB_T
    sDefault << true; // iT1_T
    sDefault << true; // iT2_T
    sDefault << true; // jSTEEL
    sDefault << true; // jSHAPELISTNAME
    sDefault << true; // jSTANDARDSHAPESTEEL
    sDefault << true; // jH
    sDefault << true; // jB
    sDefault << true; // jT1
    sDefault << true; // jT2
    sDefault << true; // jH_T
    sDefault << true; // jB_T
    sDefault << true; // jT1_T
    sDefault << true; // jT2_T
    return sDefault;
}

/* 柱断面：SRC */

void SectionColumnSRCWidget::createViews()
{
    SectionColumnWidget::createViews();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL"], 150);
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME_T"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL_T"], 150);
    tableView_2->setColumnWidth(coltag["jSHAPELISTNAME"], 150);
    tableView_2->setColumnWidth(coltag["jSTANDARDSHAPESTEEL"], 150);
    tableView_2->setColumnWidth(coltag["jSHAPELISTNAME_T"], 150);
    tableView_2->setColumnWidth(coltag["jSTANDARDSHAPESTEEL_T"], 150);
}

void SectionColumnSRCWidget::recalcDefaultValue()
{
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    for (int row = 0; row < myTableModel->rowCount(); row++) {
        inputStandardValuesRC(myTableModel->index(row, coltag["iMAINBARPHI"]));
        inputStandardValuesRC(myTableModel->index(row, coltag["jMAINBARPHI"]));
    }
}

void SectionColumnSRCWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPCOLUMN)){
        if (sv.sectionType != STRUCTTYPE::STP_SRC)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.startFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.endFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(QString::number(sv.line_param.finishLoad)));
        data.append(QVariant(sv.line_param.concMaterial));
        data.append(QVariant(sv.line_param.filledConcMaterial));
        data.append(QVariant(QString::number(sv.line_param.i_length)));
        data.append(QVariant(sv.line_param.basePlate));
        data.append(QVariant(QString::number(sv.i_param.Dy)));
        data.append(QVariant(QString::number(sv.i_param.Dz)));
        data.append(QVariant(sv.i_param.mainBarArrange1.reflectColumnText()));
        data.append(QVariant(sv.i_param.mainBarArrange2.reflectColumnText()));
        data.append(QVariant(sv.i_param.mainBar1));
        data.append(QVariant(QString::number(sv.i_param.axialCenterY)));
        data.append(QVariant(QString::number(sv.i_param.axialCenterZ)));
        data.append(QVariant(QString::number(sv.i_param.axialDy)));
        data.append(QVariant(QString::number(sv.i_param.axialDz)));
        data.append(QVariant(sv.i_param.axialBarArrange1.reflectColumnText()));
        data.append(QVariant(sv.i_param.axialBarArrange2.reflectColumnText()));
        data.append(QVariant(sv.i_param.axialBar));
        data.append(QVariant(QString::number(sv.i_param.hoopY)));
        data.append(QVariant(QString::number(sv.i_param.hoopZ)));
        data.append(QVariant(sv.i_param.hoopBar));
        data.append(QVariant(QString::number(sv.i_param.hoopPitch)));
        data.append(QVariant(QString::number(sv.i_param.RK)));
        data.append(QVariant(QString::number(sv.i_param.RY)));
        data.append(QVariant(sv.i_param.steelMaterial));
        data.append(QVariant(SteelType::enumToString(sv.i_param.steelType)));
        data.append(QVariant(sv.i_param.standardName));
        data.append(QVariant(QString::number(sv.i_param.sizeH)));
        data.append(QVariant(QString::number(sv.i_param.sizeB)));
        data.append(QVariant(QString::number(sv.i_param.t1)));
        data.append(QVariant(QString::number(sv.i_param.t2)));
        data.append(QVariant(QString::number(sv.i_param.cY)));
        data.append(QVariant(QString::number(sv.i_param.cZ)));
        data.append(QVariant(SteelType::enumToString(sv.i_param.steelType_T)));
        data.append(QVariant(sv.i_param.standardName_T));
        data.append(QVariant(QString::number(sv.i_param.H_T)));
        data.append(QVariant(QString::number(sv.i_param.B_T)));
        data.append(QVariant(QString::number(sv.i_param.t1_T)));
        data.append(QVariant(QString::number(sv.i_param.t2_T)));
        data.append(QVariant(QString::number(sv.i_param.t2_T)));
        data.append(QVariant(QString::number(sv.i_param.cY_T)));
        data.append(QVariant(QString::number(sv.i_param.cZ_T)));
        data.append(QVariant(QString::number(sv.j_param.Dy)));
        data.append(QVariant(QString::number(sv.j_param.Dz)));
        data.append(QVariant(sv.j_param.mainBarArrange1.reflectColumnText()));
        data.append(QVariant(sv.j_param.mainBarArrange2.reflectColumnText()));
        data.append(QVariant(sv.j_param.mainBar1));
        data.append(QVariant(QString::number(sv.j_param.axialCenterY)));
        data.append(QVariant(QString::number(sv.j_param.axialCenterZ)));
        data.append(QVariant(QString::number(sv.j_param.axialDy)));
        data.append(QVariant(QString::number(sv.j_param.axialDz)));
        data.append(QVariant(sv.j_param.axialBarArrange1.reflectColumnText()));
        data.append(QVariant(sv.j_param.axialBarArrange2.reflectColumnText()));
        data.append(QVariant(sv.j_param.axialBar));
        data.append(QVariant(QString::number(sv.j_param.hoopY)));
        data.append(QVariant(QString::number(sv.j_param.hoopZ)));
        data.append(QVariant(sv.j_param.hoopBar));
        data.append(QVariant(QString::number(sv.j_param.hoopPitch)));
        data.append(QVariant(QString::number(sv.j_param.RK)));
        data.append(QVariant(QString::number(sv.j_param.RY)));
        data.append(QVariant(sv.j_param.steelMaterial));
        data.append(QVariant(SteelType::enumToString(sv.j_param.steelType)));
        data.append(QVariant(sv.j_param.standardName));
        data.append(QVariant(QString::number(sv.j_param.sizeH)));
        data.append(QVariant(QString::number(sv.j_param.sizeB)));
        data.append(QVariant(QString::number(sv.j_param.t1)));
        data.append(QVariant(QString::number(sv.j_param.t2)));
        data.append(QVariant(QString::number(sv.j_param.cY)));
        data.append(QVariant(QString::number(sv.j_param.cZ)));
        data.append(QVariant(SteelType::enumToString(sv.j_param.steelType_T)));
        data.append(QVariant(sv.j_param.standardName_T));
        data.append(QVariant(QString::number(sv.j_param.H_T)));
        data.append(QVariant(QString::number(sv.j_param.B_T)));
        data.append(QVariant(QString::number(sv.j_param.t1_T)));
        data.append(QVariant(QString::number(sv.j_param.t2_T)));
        data.append(QVariant(QString::number(sv.j_param.t2_T)));
        data.append(QVariant(QString::number(sv.j_param.cY_T)));
        data.append(QVariant(QString::number(sv.j_param.cZ_T)));
        temp.appendRowData(data);
        i++;
    }

    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();

}

QList<bool> SectionColumnSRCWidget::getDefaultFilters() const{
    QList<bool> srcDefault;
    srcDefault << true; // NO
    srcDefault << true; // LABEL
    srcDefault << true; // FLOORFROM
    srcDefault << true; // FLOORTO
    srcDefault << true; // FINISHLOAD
    srcDefault << true; // CONC
    srcDefault << true; // FILLEDCONC
    srcDefault << false; // iLENGTH
    srcDefault << true; // BASELIST
    srcDefault << true; // iDX
    srcDefault << true; // iDY
    srcDefault << true; // iMAINBAR1
    srcDefault << true; // iMAINBAR2
    srcDefault << true; // iMAINBARPHI
    srcDefault << false; // iAXIALCENTERX
    srcDefault << false; // iAXIALCENTERY
    srcDefault << false; // iAXIALDX
    srcDefault << false; // iAXIALDY
    srcDefault << false; // iAXIALBAR1
    srcDefault << false; // iAXIALBAR2
    srcDefault << false; // iAXIALBARPHI
    srcDefault << true; // iHOOPX
    srcDefault << true; // iHOOPY
    srcDefault << true; // iHOOPPHI
    srcDefault << true; // iHOOPPITCH
    srcDefault << false; // iRK
    srcDefault << false; // iRY
    srcDefault << true; // iSTEEL
    srcDefault << true; // iSHAPELISTNAME
    srcDefault << true; // iSTANDARDSHAPESTEEL
    srcDefault << true; // iH
    srcDefault << true; // iB
    srcDefault << true; // iT1
    srcDefault << true; // iT2
    srcDefault << true; // iCOODINATEX
    srcDefault << true; // iCOODINATEY
    srcDefault << true; // iSHAPELISTNAME_T
    srcDefault << true; // iSTANDARDSHAPESTEEL_T
    srcDefault << true; // iH_T
    srcDefault << true; // iB_T
    srcDefault << true; // iT1_T
    srcDefault << true; // iT2_T
    srcDefault << true; // iCOODINATEX_T
    srcDefault << true; // iCOODINATEY_T
    srcDefault << true; // jDX
    srcDefault << true; // jDY
    srcDefault << true; // jMAINBAR1
    srcDefault << true; // jMAINBAR2
    srcDefault << true; // jMAINBARPHI
    srcDefault << false; // jAXIALCENTERX
    srcDefault << false; // jAXIALCENTERY
    srcDefault << false; // jAXIALDX
    srcDefault << false; // jAXIALDY
    srcDefault << false; // jAXIALBAR1
    srcDefault << false; // jAXIALBAR2
    srcDefault << false; // jAXIALBARPHI
    srcDefault << true; // jHOOPX
    srcDefault << true; // jHOOPY
    srcDefault << true; // jHOOPPHI
    srcDefault << true; // jHOOPPITCH
    srcDefault << false; // jRK
    srcDefault << false; // jRY
    srcDefault << true; // jSTEEL
    srcDefault << true; // jSHAPELISTNAME
    srcDefault << true; // jSTANDARDSHAPESTEEL
    srcDefault << true; // jH
    srcDefault << true; // jB
    srcDefault << true; // jT1
    srcDefault << true; // jT2
    srcDefault << true; // jCOODINATEX
    srcDefault << true; // jCOODINATEY
    srcDefault << true; // jSHAPELISTNAME_T
    srcDefault << true; // jSTANDARDSHAPESTEEL_T
    srcDefault << true; // jH_T
    srcDefault << true; // jB_T
    srcDefault << true; // jT1_T
    srcDefault << true; // jT2_T
    srcDefault << true; // jCOODINATEX_T
    srcDefault << true; // jCOODINATEY_T
    return srcDefault;
}

/* 柱断面：CFT */

void SectionColumnCFTWidget::createViews()
{
    SectionColumnWidget::createViews();
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL"], 150);
    tableView_2->setColumnWidth(coltag["jSHAPELISTNAME"], 150);
    tableView_2->setColumnWidth(coltag["jSTANDARDSHAPESTEEL"], 150);
}

void SectionColumnCFTWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPCOLUMN)){
        if (sv.sectionType != STRUCTTYPE::STP_CFT)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.startFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(sv.endFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(QString::number(sv.line_param.finishLoad)));
        data.append(QVariant(sv.line_param.filledConcMaterial));
        data.append(QVariant(QString::number(sv.line_param.i_length)));
        data.append(QVariant(sv.line_param.basePlate));
        data.append(QVariant(sv.i_param.steelMaterial));
        data.append(QVariant(SteelType::enumToString(sv.i_param.steelType)));
        data.append(QVariant(sv.i_param.standardName));
        data.append(QVariant(QString::number(sv.i_param.sizeH)));
        data.append(QVariant(QString::number(sv.i_param.sizeB)));
        data.append(QVariant(QString::number(sv.i_param.t1)));
        data.append(QVariant(QString::number(sv.i_param.t2)));
        data.append(QVariant(sv.j_param.steelMaterial));
        data.append(QVariant(SteelType::enumToString(sv.j_param.steelType)));
        data.append(QVariant(sv.j_param.standardName));
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

QList<bool> SectionColumnCFTWidget::getDefaultFilters() const{
    QList<bool> cftDefault;
    cftDefault << true; // NO
    cftDefault << true; // LABEL
    cftDefault << true; // FLOORFROM
    cftDefault << true; // FLOORTO
    cftDefault << true; // FINISHLOAD
    cftDefault << true; // FILLEDCONC
    cftDefault << false; // iLENGTH
    cftDefault << true; // BASELIST
    cftDefault << true; // iSTEEL
    cftDefault << true; // iSHAPELISTNAME
    cftDefault << true; // iSTANDARDSHAPESTEEL
    cftDefault << true; // iH
    cftDefault << true; // iB
    cftDefault << true; // iT1
    cftDefault << true; // iT2
    cftDefault << true; // jSTEEL
    cftDefault << true; // jSHAPELISTNAME
    cftDefault << true; // jSTANDARDSHAPESTEEL
    cftDefault << true; // jH
    cftDefault << true; // jB
    cftDefault << true; // jT1
    cftDefault << true; // jT2
    return cftDefault;
}
} // namespace post3dapp
