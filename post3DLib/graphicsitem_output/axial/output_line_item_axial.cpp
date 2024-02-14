#include "output_line_item_axial.h"

#include "axial_elevation_template.h"
#include "calc_vector2d.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "output_graphicsitem.h"
#include "unified_output.h"

namespace post3dapp{

OutputLineItemAxial::OutputLineItemAxial(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid,
                                                const QList<QPointF> &plist, bool shape_on)
    : OutputLineItem(fig, dtype,stype, uid,plist,shape_on)
{
    axialForce = 0.0;
    shapeWidth = 200.0;

    createAxialShapePath();
}

QColor OutputLineItemAxial::itemColor(ATTENTION_STATUS){
    if(this->isSelected())
        return selectColor;

    if (axialForce >= 0)
        return static_cast<AxialElevationTemplate*>(myFigureTemplate)->getViewSettings()->getAxialPlusColor();
    else
        return static_cast<AxialElevationTemplate*>(myFigureTemplate)->getViewSettings()->getAxialMinusColor();
}

//void OutputLineItemAxial::drawOTShapePath(QPainter *painter){
//    QColor positiveColor= itemColor(ATTENTION_STATUS::STATUS_WARNING);
//    QColor negativeColor= itemColor(ATTENTION_STATUS::STATUS_NORMAL);
//    QColor color = negativeColor;
//    if (axialForce >= 0)color =positiveColor;

//        QPen tmppen=painter->pen();
//        QBrush tmpbrush=painter->brush();

//        painter->setPen(QPen(Qt::black,0));
//        painter->setBrush(QBrush(color,Qt::SolidPattern));
//        painter->drawPath(drawShapePath);

//        painter->setPen(tmppen);
//        painter->setBrush(tmpbrush);


//}
void  OutputLineItemAxial::createAxialShapePath()
{
//    QColor positiveColor= itemColor(ATTENTION_STATUS::STATUS_NORMAL);
//    QColor negativeColor= itemColor(ATTENTION_STATUS::STATUS_NORMAL);

    qreal eps = 1.0e-6;
    qreal len_mid = ( myLenList.first() + myLenList.last() ) / 2.0;

//    QColor color = negativeColor;
//    if (axialForce >= 0)color =positiveColor;
//    QColor color = itemColor(ATTENTION_STATUS::STATUS_NORMAL);

    shapeWidth = 1000. * qAbs(axialForce) / static_cast<AxialElevationTemplate*>(myFigureTemplate)->getDiagramLoadPerMeter();
    if (shapeWidth < 1)shapeWidth = 1;

    drawShapePath = QPainterPath();
//    shapePath = QPainterPath();

//    if (!isValue) {
        for ( int i = 0; i < myPointList.count() - 1 ; i++ ) {

            QPointF q1, q2, q3, q4;
            QPointF p1 = myPointList.at(i);
            QPointF p2 = myPointList.at(i + 1);
            qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());


            q1 = p1 + QPointF(shapeWidth / 2.0 * qCos(ag + M_TAU / 4.0),
                              shapeWidth / 2.0 * qSin(ag + M_TAU / 4.0));
            q2 = p2 + QPointF(shapeWidth / 2.0 * qCos(ag + M_TAU / 4.0),
                              shapeWidth / 2.0 * qSin(ag + M_TAU / 4.0));
            q3 = p2 + QPointF(shapeWidth / 2.0 * qCos(ag - M_TAU / 4.0),
                              shapeWidth / 2.0 * qSin(ag - M_TAU / 4.0));
            q4 = p1 + QPointF(shapeWidth / 2.0 * qCos(ag - M_TAU / 4.0),
                              shapeWidth / 2.0 * qSin(ag - M_TAU / 4.0));

            QPolygonF s_polygon;
            s_polygon << q1 << q2 << q3 << q4;
//            path.closeSubpath();
            drawShapePath.addPolygon(s_polygon);
//            drawShapePath.addPath(path);

            QPainterPath path;
            path.moveTo(p1);
            path.lineTo(p2);
            path.moveTo(q1);
            path.lineTo(q2);
            path.lineTo(q3);
            path.lineTo(q4);
            path.lineTo(q1);
            OTpathes.append(PATH_AND_PEN(path,QPen(Qt::black,0)));
        }

        drawShapePath.closeSubpath();
//        shapePath.closeSubpath();
        shapePath = QPainterPath(drawShapePath);
        boundRect = shapePath.boundingRect();


        int idx_mid = 0;
        for (int i = 0; i < myLenList.count() - 1; i++) {
            if ( myLenList.at(i) - eps < len_mid && len_mid < myLenList.at(i + 1) ) {
                idx_mid = i;
                break;
            }
        }
        textAngle = qAtan2(myPointList.at(idx_mid + 1).y() - myPointList.at(idx_mid).y(),
                           myPointList.at(idx_mid + 1).x() - myPointList.at(idx_mid).x()) * 360.0 / M_TAU;
        qreal rr = (len_mid - myLenList.at(idx_mid)) / (myLenList.at(idx_mid + 1) - myLenList.at(idx_mid));
        QPointF pos = CalcVector2D::interiorPoint(myPointList.at(idx_mid), myPointList.at(idx_mid + 1), rr);
        //posから軸力の幅の半分だけ、textAngle方向に平行移動

        pos.setX(pos.x() - ( shapeWidth / 2.0) * qSin(textAngle/360.0 * M_TAU));
        pos.setY(pos.y() + ( shapeWidth / 2.0) * qCos(textAngle/360.0 * M_TAU));
        pos.setY(-pos.y());

        if ( textAngle >= 92.0 || textAngle <= -88.0 ) textAngle += 180.0;
        textAngle = textAngle * M_TAU / 360.0;
        QColor vcolor = stringColor(ATTENTION_STATUS::STATUS_NORMAL);
        QString str = QString::number(axialForce, 'f', 0);

        if (myFigureTemplate->valueVisible(dataType())) {
            if(!isZero(axialForce)){
                OTvaluetext.append( POINT_AND_TEXT(pos, textAngle, str, vcolor, OFFSETPOINT::OFFSET_CENTERTOP, 999999,valueTextSize) );
            }
        }
//    }
}
void OutputLineItemAxial::setDrawingStatusDirect()
{
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
    MemberForceByIL force = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(myUuid,type.stress.at(0),2,0.5);
    axialForce = force.getN() + UnifiedFixedData::getInstance()->getPstnData()->getCombinationNodeMemberForce(myUuid, type.stress.at(0)).N;
    createAxialShapePath();

}

} // namespace post3dapp
