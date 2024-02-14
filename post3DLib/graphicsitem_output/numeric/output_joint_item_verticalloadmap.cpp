#include "output_joint_item_verticalloadmap.h"

#include "define_npstn.h"
#include "fixed_data.h"
#include "unified_output.h"

namespace post3dapp{

void OutputJointItemVerticalLoadMap::setDrawingStatusDirect()
{
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
    DataNBLD3D_LOAD load = UnifiedFixedData::getInstance()->getPstnData()->getLoadData();
    DataNBLD3D_CMDL model= UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numj = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    int maxketa;

    qreal sem=load.semass.at(numj).at(5);
    for(int i=0;i<model.njmem.at(numj);i++){
        int im = model.jmem.at(numj).at(0)-1;
        int  msyu = model.imm1.at(im).at(1);
        int kk=0;
        if(model.indm[im][0]==numj)kk=model.indm[im][1]-1;
        if(model.indm[im][1]==numj)kk=model.indm[im][0]-1;
        if(msyu==2 && model.njmem.at(kk)==1){//片持ち梁のとき、他端分も集計
            sem += load.semass.at(kk).at(5);
        }
    }
    QVector<QVector<qreal>> zload;
    try {
        VerticalLoadMapPlanTemplate& myTemplate = dynamic_cast<VerticalLoadMapPlanTemplate&>(*myFigureTemplate);  // ダウンキャスト
        zload = myTemplate.getZload();
    } catch (const std::bad_cast&) {
        return;
    }

    qreal v1 = zload.at(numj).at(0)+zload.at(numj).at(1);
    qreal v2 = sem;
    qreal v3 = zload.at(numj).at(2);
    qreal axal = 0.0;
    //プルダウンで選んだケースに対する反力？
    qreal react_z = UnifiedFixedData::getInstance()->getPstnData()->getCombinationReaction(myUuid,type.stress.at(0)).at(2);
    qreal v4 = axal + react_z;
    qreal v5 = zload.at(numj).at(0);
    qreal v6 = zload.at(numj).at(1);

    maxketa = qMax(qMax(qMax(qMax(qMax((int)log10(qAbs(v1))+1,
                                       (int)log10(qAbs(v2))+1),
                                  (int)log10(qAbs(v3))+1),
                             (int)log10(qAbs(v4))+1),
                        (int)log10(qAbs(v5))+1),
                   (int)log10(qAbs(v6))+1);
    maxketa = qMax(maxketa,1);
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, QString::number(v1, 'f', 0).rightJustified(maxketa), normalTextColor,
                                       OFFSETPOINT::OFFSET_LEFTTOP, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, QString::number(v2, 'f', 0).rightJustified(maxketa), normalTextColor,
                                       OFFSETPOINT::OFFSET_RIGHTTOP, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, QString::number(v3, 'f', 0).rightJustified(maxketa), normalTextColor,
                                       OFFSETPOINT::OFFSET_LEFTBOTTOM, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, QString::number(v4, 'f', 0).rightJustified(maxketa), normalTextColor,
                                       OFFSETPOINT::OFFSET_LEFTBOTTOM2, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, QString::number(v5, 'f', 0).rightJustified(maxketa), normalTextColor,
                                       OFFSETPOINT::OFFSET_RIGHTBOTTOM, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, QString::number(v6, 'f', 0).rightJustified(maxketa), normalTextColor,
                                       OFFSETPOINT::OFFSET_RIGHTBOTTOM2, 999999,valueTextSize) );
    qDebug()<<"OutputJointItemVerticalLoadMap::setDrawingStatusDirect return";

}

}
