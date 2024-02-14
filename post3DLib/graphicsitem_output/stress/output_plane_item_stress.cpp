#include "output_plane_item_stress.h"

#include <QPainter>

#include "calc_vector2d.h"
#include "define_output.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "manage_memberdata.h"
#include "stress_elevation_template.h"
#include "unified_analysisdata.h"
#include "unified_output.h"
namespace post3dapp{

OutputPlaneItemStress::OutputPlaneItemStress(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype,const QUuid &uid, const QList<QPointF> &plist, MRESULT *mresult)
    : OutputPlaneItem(fig, dtype,stype, uid,plist)
{
    qDebug() << "outplanestress";
    myResult = mresult;
    valueTextSize = myFigureTemplate->valueTextSize(dtype);

}


void OutputPlaneItemStress::setDrawingStatusDirect()
{
    bool q_visible = static_cast<StressElevationTemplate*>(myFigureTemplate)->qVisible();
    bool m_visible = static_cast<StressElevationTemplate*>(myFigureTemplate)->mVisible();
    bool n_visible = static_cast<StressElevationTemplate*>(myFigureTemplate)->nVisible();

    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
    MemberForce mf = UnifiedFixedData::getInstance()->getPstnData()->getCombinationNodeMemberForce(myUuid, type.stress.at(0));
//MemberData* md = UnifiedFixedData::getInstance()->getAnalysisData();

    MemberData* md=UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    QPointF ps;
    QPointF pe;
    qreal angle = 0.0;
    if(md->getPlaneSubModel()==PLANESUBMODEL::WI_MODEL
            || md->getPlaneSubModel()==PLANESUBMODEL::SI_MODEL){
        ps = 0.5*myPointList.at(0)+0.5*myPointList.at(1); //i端
        pe = 0.5*myPointList.at(2)+0.5*myPointList.at(3); //j端


    }else if(md->getPlaneSubModel()==PLANESUBMODEL::WH_MODEL
             || md->getPlaneSubModel()==PLANESUBMODEL::SH_MODEL){
        ps = 0.5*myPointList.at(0)+0.5*myPointList.at(3); //i端
        pe = 0.5*myPointList.at(2)+0.5*myPointList.at(1); //j端
        angle = -M_PI_2;
    }else{
        return;
    }


    ps.setY(-ps.y());
    pe.setY(-pe.y());

    QColor color = myFigureTemplate->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_NORMAL);;
    if (myFigureTemplate->figureVisible(myDataType)) {

    if(!isZero(mf.N) && n_visible){
        OTvaluetext.append( POINT_AND_TEXT(0.7 * ps + 0.3 * pe, angle, QString::number(mf.N, 'f', 0),
                                           color, OFFSETPOINT::OFFSET_NONE,9999999,valueTextSize) );
    }
    if(!isZero(mf.iQy&& q_visible)){
        OTvaluetext.append( POINT_AND_TEXT(0.4 * ps + 0.6 * pe, angle, QString::number(-mf.iQy, 'f', 0),
                                           color, OFFSETPOINT::OFFSET_NONE,9999999,valueTextSize) );
    }
    if(!isZero(mf.iMz&& m_visible)){
        OTvaluetext.append( POINT_AND_TEXT(0.9 * ps + 0.1 * pe, angle, QString::number(-mf.iMz, 'f', 0),
                                           color, OFFSETPOINT::OFFSET_NONE,9999999,valueTextSize) );
    }
    if(!isZero(mf.jMz&& m_visible)){
        OTvaluetext.append( POINT_AND_TEXT(0.1 * ps + 0.9 * pe, angle, QString::number(mf.jMz, 'f', 0),
                                           color,OFFSETPOINT:: OFFSET_NONE,9999999,valueTextSize) );
    }
    }
}


void OutputPlaneItemStress::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                  QWidget *)
{
    setPainter(painter, option);
    drawOTShapePath(painter);
    drawOTText(painter);
    return;


}
} // namespace post3dapp
