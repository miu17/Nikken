#include "output_springitem.h"

#include <QPainter>

#include "calc_vector2d.h"
#include "fixed_data.h"
#include "manage_springdata.h"
#include "unified_analysisdata.h"

namespace post3dapp {

OutputSpringItem::OutputSpringItem(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid,
                                    const QList<QPointF> &plist, bool shape_on )
    : OutputGraphicsItem(fig, dtype,stype, uid)
{
    QPointF p0 = plist.first();
    myLenList.clear();
    myPointList.clear();
    myLenList.append(0.0);
    myPointList.append(QPointF(0.0, 0.0));
    for ( int i = 1; i < plist.count(); i++ ) {
        myLenList.append( myLenList.last() + CalcVector2D::length( plist.at(i) - plist.at(i - 1) ) );
        myPointList.append( plist.at(i) - p0 );
    }

    if ( shape_on ) createSpringShapePath();
//    createSectionName();
    SpringData *sd = UnifiedFixedData::getInstance()->getAnalysisData()->springIDToPointer(myUuid);
    if(sd->isISO())attentionStatus=ATTENTION_STATUS::STATUS_WARNING;

}

//QColor OutputSpringItem::itemColor(ATTENTION_STATUS st){
////    return Qt::red;
//    if(this->isSelected())return selectColor;

//    SpringData *sd = UnifiedFixedData::getInstance()->getAnalysisData()->springIDToPointer(myUuid);
//    if(st == ATTENTION_STATUS::STATUS_NORMAL){
//        return normalColor;
//    }else if(st == ATTENTION_STATUS::STATUS_WARNING){
//        return warningColor;
//    }else if(st == ATTENTION_STATUS::STATUS_FATAL){
//        return fatalColor;
//    }
//    return normalColor;
//}
void  OutputSpringItem::createSpringShapePath()
{

    qreal shapeWidth = 1000.0;
    drawShapePath = QPainterPath();
    shapePath = QPainterPath();

    for ( int i = 0; i < myPointList.count() - 1 ; i++ ) {

        QPointF q1, q2, q3, q4;
        QPointF p1 = myPointList.at(i);
        QPointF p2 = myPointList.at(i + 1);
        qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

        drawShapePath.moveTo(p1);
        drawShapePath.lineTo(p2);

        q1 = p1 + QPointF(shapeWidth / 2.0 * qCos(ag + M_PI / 2.0),
                          shapeWidth / 2.0 * qSin(ag + M_PI / 2.0));
        q2 = p2 + QPointF(shapeWidth / 2.0 * qCos(ag + M_PI / 2.0),
                          shapeWidth / 2.0 * qSin(ag + M_PI / 2.0));
        q3 = p2 + QPointF(shapeWidth / 2.0 * qCos(ag - M_PI / 2.0),
                          shapeWidth / 2.0 * qSin(ag - M_PI / 2.0));
        q4 = p1 + QPointF(shapeWidth / 2.0 * qCos(ag - M_PI / 2.0),
                          shapeWidth / 2.0 * qSin(ag - M_PI / 2.0));

        QPolygonF s_polygon;
        s_polygon << q1 << q2 << q3 << q4;
        shapePath.addPolygon(s_polygon);
    }

    drawShapePath.closeSubpath();
    shapePath.closeSubpath();
    boundRect = shapePath.boundingRect();

    //MSS せん断
    //BN
    //BQ
    //符号


}

} // namespace post3dapp
