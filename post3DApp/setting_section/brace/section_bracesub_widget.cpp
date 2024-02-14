#include "section_bracesub_widget.h"

#include <QtGui>
#include <QDebug>

#include "define_section_struct.h"
#include "section_tableview.h"
#include "unified_inputdata.h"

namespace post3dapp{
void SectionBraceSubWidget::createViews()
{
    SectionBraceWidget::createViews();
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    tableView_1->setColumnWidth(coltag["iSHAPELISTNAME"], 150);
    tableView_1->setColumnWidth(coltag["iSTANDARDSHAPESTEEL"], 150);
    tableView_2->setColumnWidth(coltag["mSHAPELISTNAME"], 150);
    tableView_2->setColumnWidth(coltag["mSTANDARDSHAPESTEEL"], 150);
    tableView_3->setColumnWidth(coltag["jSHAPELISTNAME"], 150);
    tableView_3->setColumnWidth(coltag["jSTANDARDSHAPESTEEL"], 150);
}

void SectionBraceSubWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPBRACE)){
        if (sv.sectionType != STRUCTTYPE::STP_S)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(QString::number(sv.line_param.finishLoad)));
        data.append(QVariant(QString::number(sv.line_param.hasCompResistance)));
        data.append(QVariant(QString::number(sv.line_param.i_length)));
        data.append(QVariant(QString::number(sv.line_param.j_length)));
        data.append(QVariant(QString::number(sv.line_param.direct_area)));
        data.append(QVariant(QString::number(sv.line_param.direct_effectiveArea)));
        data.append(QVariant(QString::number(sv.line_param.direct_slenderRatio)));
        data.append(QVariant(QString::number(sv.line_param.direct_unitmass)));
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

QList<bool> SectionBraceSubWidget::getDefaultFilters() const{
    QList<bool> sDefault;
    sDefault << true; // NO
    sDefault << true; // LABEL
    sDefault << true; // FINISHLOAD
    sDefault << true; // COMPRESSIVERESISTANCE
    sDefault << false; // iLENGTH
    sDefault << false; // jLENGTH
    sDefault << true; // AREA
    sDefault << true; // EFFECTIVEAREA
    sDefault << true; // SLENDERNESSRATIO
    sDefault << true; // UNITMASS
    sDefault << true; // DISPLAYHEIGHT
    sDefault << true; // DISPLAYWIDTH
    sDefault << true; // iSTEEL
    sDefault << true; // iSHAPELISTNAME
    sDefault << true; // iSTANDARDSHAPESTEEL
    sDefault << true; // iH
    sDefault << true; // iB
    sDefault << true; // iT1
    sDefault << true; // iT2
    sDefault << false; // mSTEEL
    sDefault << false; // mSHAPELISTNAME
    sDefault << false; // mSTANDARDSHAPESTEEL
    sDefault << false; // mH
    sDefault << false; // mB
    sDefault << false; // mT1
    sDefault << false; // mT2
    sDefault << false; // jSTEEL
    sDefault << false; // jSHAPELISTNAME
    sDefault << false; // jSTANDARDSHAPESTEEL
    sDefault << false; // jH
    sDefault << false; // jB
    sDefault << false; // jT1
    sDefault << false; // jT2
    return sDefault;
}
} // namespace post3dapp
