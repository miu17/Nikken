#include "section_wallsub_widget.h"

#include <QtGui>
#include <QDebug>

#include "define_section_struct.h"
#include "section_tableview.h"
#include "unified_inputdata.h"

namespace post3dapp{
void SectionWallRegularWidget::createViews()
{
    SectionWallWidget::createViews();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    Q_FOREACH (int tag, coltag) {
        if (coltag.key(tag).contains("BAR")) {
            tableView_1->setColumnWidth(tag, 75);
            tableView_2->setColumnWidth(tag, 75);
            tableView_3->setColumnWidth(tag, 75);
        }
    }
}

void SectionWallRegularWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        if (sv.sectionType != STRUCTTYPE::STP_RC)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(sv.startFloorID.toString()));
        data.append(QVariant(sv.endFloorID.toString()));
        data.append(QVariant(PlaneType::enumToString(sv.plane_param.planeType)));
        data.append(QVariant(sv.plane_param.concMaterial));
        data.append(QVariant(QString::number(sv.plane_param.concThickness)));
        data.append(QVariant(sv.plane_param.steelMaterial));
        data.append(QVariant(QString::number(sv.plane_param.steelThickness)));
        data.append(QVariant(QString::number(sv.plane_param.finishLoad)));
        data.append(QVariant(QString::number(sv.plane_param.isDoubleBar)));
        data.append(QVariant(sv.plane_param.vBar.phi));
        data.append(QVariant(QString::number(sv.plane_param.vBar.pitch)));
        data.append(QVariant(sv.plane_param.hBar.phi));
        data.append(QVariant(QString::number(sv.plane_param.hBar.pitch)));
        data.append(QVariant(QString::number(sv.plane_param.vOpeningBar_small.num)));
        data.append(QVariant(sv.plane_param.vOpeningBar_small.phi));
        data.append(QVariant(QString::number(sv.plane_param.hOpeningBar_small.num)));
        data.append(QVariant(sv.plane_param.hOpeningBar_small.phi));
        data.append(QVariant(QString::number(sv.plane_param.dOpeningBar_small.num)));
        data.append(QVariant(sv.plane_param.dOpeningBar_small.phi));
        data.append(QVariant(QString::number(sv.plane_param.vOpeningBar_large.num)));
        data.append(QVariant(sv.plane_param.vOpeningBar_large.phi));
        data.append(QVariant(QString::number(sv.plane_param.hOpeningBar_large.num)));
        data.append(QVariant(sv.plane_param.hOpeningBar_large.phi));
        data.append(QVariant(QString::number(sv.plane_param.dOpeningBar_large.num)));
        data.append(QVariant(sv.plane_param.dOpeningBar_large.phi));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionWallRegularWidget::getDefaultFilters() const
{
    QList<bool> wallDefault;
    wallDefault << true; // NO
    wallDefault << true; // LABEL
    wallDefault << true; // FLOORFROM
    wallDefault << true; // FLOORTO
    wallDefault << true; // WALLTYPE
    wallDefault << true; // CONC
    wallDefault << true; // CONCTHICKNESS
    wallDefault << false; // STEEL
    wallDefault << false; // STEELTHICKNESS
    wallDefault << true; // FINISHLOAD
    wallDefault << true; // BARARRANGEMENTTYPE
    wallDefault << true; // vBARPHI
    wallDefault << true; // vBARPITCH
    wallDefault << true; // hBARPHI
    wallDefault << true; // hBARPITCH
    wallDefault << true; // vSMALLOPENINGBARNUM
    wallDefault << true; // vSMALLOPENINGBARPHI
    wallDefault << true; // hSMALLOPENINGBARNUM
    wallDefault << true; // hSMALLOPENINGBARPHI
    wallDefault << true; // dSMALLOPENINGBARNUM
    wallDefault << true; // dSMALLOPENINGBARPHI
    wallDefault << true; // vLARGEOPENINGBARNUM
    wallDefault << true; // vLARGEOPENINGBARPHI
    wallDefault << true; // hLARGEOPENINGBARNUM
    wallDefault << true; // hLARGEOPENINGBARPHI
    wallDefault << true; // dLARGEOPENINGBARNUM
    wallDefault << true; // dLARGEOPENINGBARPHI
    return wallDefault;
};

void SectionWallSteelWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        if (sv.sectionType != STRUCTTYPE::STP_S)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(sv.startFloorID.toString()));
        data.append(QVariant(sv.endFloorID.toString()));
        data.append(QVariant(PlaneType::enumToString(sv.plane_param.planeType)));
        data.append(QVariant(sv.plane_param.steelMaterial));
        data.append(QVariant(QString::number(sv.plane_param.steelThickness)));
        data.append(QVariant(QString::number(sv.plane_param.finishLoad)));
        data.append(QVariant(QString::number(0))); // 縦リブピッチ
        data.append(QVariant(u8"")); // 縦リブタイプ
        data.append(QVariant(QString::number(0))); // 縦リブ厚
        data.append(QVariant(QString::number(0))); // 縦リブ幅
        data.append(QVariant(QString::number(0))); // 横リブピッチ
        data.append(QVariant(u8"")); // 横リブタイプ
        data.append(QVariant(QString::number(0))); // 横リブ厚
        data.append(QVariant(QString::number(0))); // 横リブ幅
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionWallSteelWidget::getDefaultFilters() const
{
    QList<bool> wallDefault;
    wallDefault << true; // NO
    wallDefault << true; // LABEL
    wallDefault << true; // FLOORFROM
    wallDefault << true; // FLOORTO
    wallDefault << true; // WALLTYPE
    wallDefault << true; // STEEL
    wallDefault << true; // STEELTHICKNESS
    wallDefault << true; // FINISHLOAD
    wallDefault << false; // vRIBPITCH
    wallDefault << false; // vRIBTYPE
    wallDefault << false; // vRIBTHICKNESS
    wallDefault << false; // vRIBBREADTH
    wallDefault << false; // hRIBPITCH
    wallDefault << false; // hRIBTYPE
    wallDefault << false; // hRIBTHICKNESS
    wallDefault << false; // hRIBBREADTH
    return wallDefault;
};



} // namespace post3dapp
