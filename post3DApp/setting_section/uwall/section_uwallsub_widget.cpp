#include "section_uwallsub_widget.h"

#include <QtGui>
#include <QDebug>

#include "define_section_struct.h"
#include "section_tableview.h"
#include "unified_inputdata.h"

namespace post3dapp{


//void SectionUWallUnderGroundWidget::createViews()
//{
//    SectionUnderGroundWallWidget::createViews();

//    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
//    Q_FOREACH (int tag, coltag) {
//        if (coltag.key(tag).contains("BAR")) {
//            tableView_1->setColumnWidth(tag, 75);
//            tableView_2->setColumnWidth(tag, 75);
//        }
//    }
//}

//void SectionUWallUnderGroundWidget::reflectUnifiedData(){
//    DoubleList temp(myTableModel->columnCount());
//    int i = 0;
//    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPUWALL)){
//        QList<QVariant> data;
//        data.append(QVariant(i));
//        data.append(QVariant(sv.sectionName));
//        data.append(QVariant(sv.plane_param.concMaterial));
//        data.append(QVariant(QString::number(sv.plane_param.concThickness)));
//        data.append(QVariant(sv.plane_param.steelMaterial));
//        data.append(QVariant(QString::number(sv.plane_param.steelThickness)));
//        data.append(QVariant(QString::number(sv.plane_param.finishLoad)));
//        data.append(QVariant()); // 標準径D
//        data.append(QVariant()); // 標準ピッチ@
//        data.append(QVariant(sv.plane_param.vBar_outerTop.phi));
//        data.append(QVariant(QString::number(sv.plane_param.vBar_outerTop.pitch)));
//        data.append(QVariant(sv.plane_param.vBar_outerMiddle.phi));
//        data.append(QVariant(QString::number(sv.plane_param.vBar_outerMiddle.pitch)));
//        data.append(QVariant(sv.plane_param.vBar_outerBottom.phi));
//        data.append(QVariant(QString::number(sv.plane_param.vBar_outerBottom.pitch)));
//        data.append(QVariant(sv.plane_param.vBar_innerTop.phi));
//        data.append(QVariant(QString::number(sv.plane_param.vBar_innerTop.pitch)));
//        data.append(QVariant(sv.plane_param.hBar_outerEdge.phi));
//        data.append(QVariant(QString::number(sv.plane_param.hBar_outerEdge.pitch)));
//        data.append(QVariant(sv.plane_param.hBar_outerCenter.phi));
//        data.append(QVariant(QString::number(sv.plane_param.hBar_outerCenter.pitch)));
//        data.append(QVariant(sv.plane_param.vBar_innerMiddle.phi));
//        data.append(QVariant(QString::number(sv.plane_param.vBar_innerMiddle.pitch)));
//        data.append(QVariant(sv.plane_param.vBar_innerBottom.phi));
//        data.append(QVariant(QString::number(sv.plane_param.vBar_innerBottom.pitch)));
//        data.append(QVariant(sv.plane_param.hBar_innerEdge.phi));
//        data.append(QVariant(QString::number(sv.plane_param.hBar_innerEdge.pitch)));
//        data.append(QVariant(sv.plane_param.hBar_innerCenter.phi));
//        data.append(QVariant(QString::number(sv.plane_param.hBar_innerCenter.pitch)));
//        temp.appendRowData(data);
//        i++;
//    }
//    myTableModel->reflectUnifiedData(temp);
//    emit sectionItemChanged();
//    slotChangeCurrentRow(0);
//    undoStack->clear();
//}

//QList<bool> SectionUWallUnderGroundWidget::getDefaultFilters() const
//{
//    QList<bool> uwallDefault;
//    uwallDefault << true; // NO
//    uwallDefault << true; // LABEL
//    uwallDefault << true; // CONC
//    uwallDefault << true; // CONCTHICKNESS
//    uwallDefault << false; // STEEL
//    uwallDefault << false; // STEELTHICKNESS
//    uwallDefault << true; // FINISHLOAD
//    uwallDefault << true; // STANDARDBARPHI
//    uwallDefault << true; // STANDARDBARPITCH
//    uwallDefault << true; // vOUTERTOP_BARPHI
//    uwallDefault << true; // vOUTERTOP_BARPITCH
//    uwallDefault << true; // vOUTERMIDDLE_BARPHI
//    uwallDefault << true; // vOUTERMIDDLE_BARPITCH
//    uwallDefault << true; // vOUTERBOTTOM_BARPHI
//    uwallDefault << true; // vOUTERBOTTOM_BARPITCH
//    uwallDefault << true; // hOUTEREDGE_BARPHI
//    uwallDefault << true; // hOUTEREDGE_BARPITCH
//    uwallDefault << true; // hOUTERCENTER_BARPHI
//    uwallDefault << true; // hOUTERCENTER_BARPITCH
//    uwallDefault << true; // vINNERTOP_BARPHI
//    uwallDefault << true; // vINNERTOP_BARPITCH
//    uwallDefault << true; // vINNERMIDDLE_BARPHI
//    uwallDefault << true; // vINNERMIDDLE_BARPITCH
//    uwallDefault << true; // vINNERBOTTOM_BARPHI
//    uwallDefault << true; // vINNERBOTTOM_BARPITCH
//    uwallDefault << true; // hINNEREDGE_BARPHI
//    uwallDefault << true; // hINNEREDGE_BARPITCH
//    uwallDefault << true; // hINNERCENTER_BARPHI
//    uwallDefault << true; // hINNERCENTER_BARPITCH
//    return uwallDefault;
//};
} // namespace post3dapp
