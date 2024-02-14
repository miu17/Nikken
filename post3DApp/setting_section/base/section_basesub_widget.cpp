#include "section_basesub_widget.h"

#include <QtGui>
#include <QDebug>

#include "customtable_view.h"
#include "define_material_struct.h"
#include "define_section_struct.h"
#include "unified_inputdata.h"


namespace post3dapp{
void SectionBaseSWidget::createViews()
{
    SectionBaseWidget::createViews();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    myTableView->setColumnWidth(coltag["STANDARDTYPE"], 250);
    myTableView->setColumnWidth(coltag["PRODUCTNAME"], 120);
}

void SectionBaseSWidget::recalcDefaultValue()
{
    QModelIndexList idxlist;
    QList<QVariant> varlist;
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    for (int row = 0; row < myTableModel->rowCount(); row++) {
        inputStandardValuesBASE(myTableModel->index(row, coltag["BOLTPHI"]), idxlist, varlist);
        inputStandardValuesBASE(myTableModel->index(row, coltag["BC"]), idxlist, varlist);
        inputStandardValuesBASE(myTableModel->index(row, coltag["DC"]), idxlist, varlist);
    }

}

void SectionBaseSWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPBASE)){
        if (sv.sectionType != STRUCTTYPE::STP_S){
            continue;
        }
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(QString::number(sv.base_param.isStandardType)));
        data.append(QVariant(sv.base_param.productName));
        data.append(QVariant(QString::number(sv.base_param.sizeB)));
        data.append(QVariant(QString::number(sv.base_param.sizeD)));
        data.append(QVariant(QString::number(sv.base_param.sizeT)));
        data.append(QVariant(sv.base_param.basePlateMaterial));
        data.append(QVariant(QString::number(sv.base_param.boltHolePhi)));
        data.append(QVariant(QString::number(sv.base_param.edgehy1)));
        data.append(QVariant(QString::number(sv.base_param.edgehz1)));
        data.append(QVariant(QString::number(sv.base_param.bolt_ny1)));
        data.append(QVariant(QString::number(sv.base_param.bolt_nz1)));
        data.append(QVariant(QString::number(sv.base_param.boltPhi)));
        data.append(QVariant(QString::number(sv.base_param.screwPhi)));
        data.append(QVariant(sv.base_param.boltMaterial));
        data.append(QVariant(QString::number(sv.base_param.anchorLength)));
        data.append(QVariant(QString::number(sv.base_param.hasElongationAbility)));
        data.append(QVariant(QString::number(sv.base_param.anchorArea)));
        data.append(QVariant(QString::number(sv.base_param.sizeBc)));
        data.append(QVariant(QString::number(sv.base_param.sizeDc)));
        data.append(QVariant(QString::number(sv.base_param.sizeH)));
        data.append(QVariant(QString::number(sv.base_param.projectedarea)));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionBaseSWidget::getDefaultFilters() const{
    QList<bool> sDefault;
    sDefault << true; // NO
    sDefault << true; // LABEL
    sDefault << true; // STANDARDTYPE
    sDefault << true; // PRODUCTNAME
    sDefault << true; // B
    sDefault << true; // D
    sDefault << true; // T
    sDefault << true; // BASEPLATEMATERIAL
    sDefault << true; // EDGEDISTANCEX1
    sDefault << true; // EDGEDISTANCEY1
    sDefault << true; // BOLTNX1
    sDefault << true; // BOLTNY1
    sDefault << true; // BOLTPHI
    sDefault << true; // BOLTMATERIAL
    sDefault << true; // BOLTLENGTH
    return sDefault;
}

void SectionBaseSRCWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPBASE)){
        if (sv.sectionType != STRUCTTYPE::STP_SRC){
            continue;
        }
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(QString::number(sv.base_param.B0)));
        data.append(QVariant(QString::number(sv.base_param.D0)));
        data.append(QVariant(sv.base_param.concMaterial));
        data.append(QVariant(QString::number(static_cast<int>(sv.base_param.basePlateType))));
        data.append(QVariant(QString::number(sv.base_param.sizeB)));
        data.append(QVariant(QString::number(sv.base_param.sizeD)));
        data.append(QVariant(QString::number(sv.base_param.sizeT)));
        data.append(QVariant(sv.base_param.SML));
        data.append(QVariant(QString::number(sv.base_param.edgehy1)));
        data.append(QVariant(QString::number(sv.base_param.edgehy2)));
        data.append(QVariant(QString::number(sv.base_param.edgehz1)));
        data.append(QVariant(QString::number(sv.base_param.edgehz2)));
        data.append(QVariant(QString::number(static_cast<int>(sv.base_param.boltType))));
        data.append(QVariant(QString::number(sv.base_param.bolt_ny1)));
        data.append(QVariant(QString::number(sv.base_param.bolt_ny2)));
        data.append(QVariant(QString::number(sv.base_param.bolt_nz1)));
        data.append(QVariant(QString::number(sv.base_param.bolt_nz2)));
        data.append(QVariant(QString::number(sv.base_param.boltPhi)));
        data.append(QVariant(QString::number(static_cast<int>(sv.base_param.boltType)))); // 要確認
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionBaseSRCWidget::getDefaultFilters() const{
    QList<bool> srcDefault;
    srcDefault << true; // NO
    srcDefault << true; // LABEL
    srcDefault << true; // B0
    srcDefault << true; // D0
    srcDefault << true; // CONC
    srcDefault << true; // BASEPLATETYPE
    srcDefault << true; // B
    srcDefault << true; // D
    srcDefault << true; // T
    srcDefault << true; // SML
    srcDefault << true; // EDGEDISTANCEX1
    srcDefault << true; // EDGEDISTANCEX2
    srcDefault << true; // EDGEDISTANCEY1
    srcDefault << true; // EDGEDISTANCEY2
    srcDefault << true; // BOLTARRANGEMENTTYPE
    srcDefault << true; // BOLTNX1
    srcDefault << true; // BOLTNX2
    srcDefault << true; // BOLTNY1
    srcDefault << true; // BOLTNY2
    srcDefault << true; // BOLTPHI
    srcDefault << true; // BOLTMATERIAL
    return srcDefault;
}
} // namespace post3dapp
