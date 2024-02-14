#include "output_spring_item_model.h"

#include <QPainter>

#include "calc_vector2d.h"
#include "fixed_data.h"
#include "manage_springdata.h"
#include "model_elevation_template.h"
#include "unified_analysisdata.h"

namespace post3dapp {

OutputSpringItemModel::OutputSpringItemModel(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid,
                                    const QList<QPointF> &plist, bool shape_on )
    : OutputSpringItem(fig, dtype,stype, uid,plist,shape_on)
{
    createSectionName();
}

void  OutputSpringItemModel:: createSectionName(){
    ModelElevationTemplate* tp = static_cast<ModelElevationTemplate*>(myFigureTemplate);
//    if( !tp->nameVisible(myDataType))return;


    SpringData *sd = UnifiedFixedData::getInstance()->getAnalysisData()->springIDToPointer(myUuid);

    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();
    if (CalcVector2D::length(pe - ps) < 0.1)return;

    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));
    QPointF pos = QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                          -0.5 * ps.y() - 0.5 * pe.y());

    if(sd->isISO() ){
        if( !tp->nameVisible(DATATYPE::TPISO))return;
        QColor color = tp->nameColor(DATATYPE::TPISO,STRUCTTYPE::NOSTRUCTTYPE,ATTENTION_STATUS::STATUS_WARNING);
        int size = tp->nameTextSize(DATATYPE::TPISO);
        OFFSETPOINT op=OFFSETPOINT::OFFSET_CENTERTOP;
        if( sd->springType()==B3D_DATATYPE::B3D_TPSPRING_MSS){
            op=OFFSETPOINT::OFFSET_CENTERBOTTOM;
        }
        POINT_AND_TEXT pat(pos,p_ang,sd->springName().split('-').first(),color,op,999999.9,size);
        OTvaluetext.append(pat);
    }else{
        if( !tp->nameVisible(DATATYPE::TPSPRING))return;
        QColor color = tp->nameColor(DATATYPE::TPSPRING,STRUCTTYPE::NOSTRUCTTYPE,ATTENTION_STATUS::STATUS_NORMAL);
        int size = tp->nameTextSize(DATATYPE::TPSPRING);
        POINT_AND_TEXT pat(pos,p_ang,sd->springName().split('-').first(),color,OFFSETPOINT::OFFSET_CENTERTOP,999999.9,size);
        OTvaluetext.append(pat);
    }
}

} // namespace post3dapp
