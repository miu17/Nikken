#include "output_plane_item_model.h"

#include <QPainter>

#include "calc_vector2d.h"
#include "define_npstn.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "model_elevation_template.h"
#include "unified_analysisdata.h"
#include "unified_output.h"


namespace post3dapp{

OutputPlaneItemModel::OutputPlaneItemModel(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype,const QUuid &uid, const QList<QPointF> &plist, MRESULT *mresult)
    : OutputPlaneItem(fig, dtype,stype, uid,plist)
{
    qDebug() << "outplaneModel";
    myResult = mresult;
//    valueTextSize = myFigureTemplate->valueTextSize(dtype);

}

void  OutputPlaneItemModel::createPlaneShapePath()
{

    /* 時計回り・反時計回り、図心位置の算出 */
    qreal area = 0.0;
    for ( int i = 0; i < myPointList.count(); i++ ) {
        QPointF p1 = myPointList.at(i);
        QPointF p2 = ( i == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(i + 1) ;
        area += p1.x() * p2.y() - p2.x() * p1.y();
    }
    area = area / 2.0;


    /* 描画パスの作成 */
    drawShapePath = QPainterPath();

    qreal clen = 800.0;
    MemberData* md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    if(md->getPlaneSubModel()==PLANESUBMODEL::PLATE_MODEL
            || md->getPlaneSubModel()==PLANESUBMODEL::MEMBRANE_MODEL){
        clen = 200;
    }

    QList<QPointF> qq;
//    if(myPointList.count()<4)return;
    for ( int i = 0; i < myPointList.count(); i++ ) {

        QPointF p1 = ( i == 0 ) ? myPointList.last() : myPointList.at(i - 1);
        QPointF p2 = myPointList.at(i) ;
        QPointF p3 = ( i == myPointList.count() - 1 ) ? myPointList.first() : myPointList.at(i + 1) ;

        qreal th1 = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());
        qreal th2 = qAtan2(p3.y() - p2.y(), p3.x() - p2.x());


        qreal a0 = (M_PI+th1-th2)/2.0;
        qreal a1 = th2+a0;
        qreal len = clen / qSin(a0);
        QPointF q1 = p2 + QPointF(len * qCos(a1), len * qSin(a1));

        qq<<q1;

    }
    OFFSETPOINT off1 = OFFSETPOINT::OFFSET_CENTERTOP;
    OFFSETPOINT off2 = OFFSETPOINT::OFFSET_CENTERBOTTOM;
    if(md->getPlaneSubModel()==PLANESUBMODEL::PLATE_MODEL
            || md->getPlaneSubModel()==PLANESUBMODEL::MEMBRANE_MODEL){
        drawShapePath.moveTo(qq[0]);
        for(int i=1;i<myPointList.count();i++){
            drawShapePath.lineTo(qq[i]);
        }
        drawShapePath.lineTo(qq[0]);
    }else if(md->getPlaneSubModel()==PLANESUBMODEL::SI_MODEL
            || md->getPlaneSubModel()==PLANESUBMODEL::WI_MODEL){
        qreal wid = 300;
        qreal w1=qSqrt(QPointF::dotProduct((qq[1]-qq[0]),(qq[1]-qq[0])));
        qreal w2=qSqrt(QPointF::dotProduct((qq[3]-qq[0]),(qq[3]-qq[0])));
        QPointF u = wid * (qq[1]-qq[0])/w1;
        QPointF v = wid * (qq[3]-qq[0])/w2;

        QPainterPath path;
        path.moveTo(qq[0]);
        path.lineTo(qq[1]);
        path.lineTo(qq[1]+0.4*v);
        path.lineTo(qq[0]+0.4*v);
        path.lineTo(qq[0]);
        QPainterPath path2;
        path2.moveTo(qq[2]);
        path2.lineTo(qq[3]);
        path2.lineTo(qq[3]-0.4*v);
        path2.lineTo(qq[2]-0.4*v);
        path2.lineTo(qq[2]);
        OTpathes.append(PATH_AND_PEN(path,QPen(itemColor(ATTENTION_STATUS::STATUS_NORMAL),0),QBrush(itemColor(ATTENTION_STATUS::STATUS_NORMAL))));
        OTpathes.append(PATH_AND_PEN(path2,QPen(itemColor(ATTENTION_STATUS::STATUS_NORMAL),0),QBrush(itemColor(ATTENTION_STATUS::STATUS_NORMAL))));

        drawShapePath.moveTo(0.5*qq[0]+0.5*qq[1]+0.4*v+0.5*u);
        drawShapePath.lineTo(0.5*qq[2]+0.5*qq[3]-0.4*v+0.5*u);
        drawShapePath.lineTo(0.5*qq[2]+0.5*qq[3]-0.4*v-0.5*u);
        drawShapePath.lineTo(0.5*qq[0]+0.5*qq[1]+0.4*v-0.5*u);
        drawShapePath.lineTo(0.5*qq[0]+0.5*qq[1]+0.4*v+0.5*u);


        off1 = OFFSETPOINT::OFFSET_LEFTMIDDLE;
        off2 = OFFSETPOINT::OFFSET_RIGHTMIDDLE;
    }else if(md->getPlaneSubModel()==PLANESUBMODEL::SH_MODEL
             || md->getPlaneSubModel()==PLANESUBMODEL::WH_MODEL){
        qreal wid = 300;
        qreal w1=qSqrt(QPointF::dotProduct((qq[1]-qq[0]),(qq[1]-qq[0])));
        qreal w2=qSqrt(QPointF::dotProduct((qq[3]-qq[0]),(qq[3]-qq[0])));
        QPointF u = wid * (qq[1]-qq[0])/w1;
        QPointF v = wid * (qq[3]-qq[0])/w2;

        QPainterPath path;
        path.moveTo(qq[0]);
        path.lineTo(qq[3]);
        path.lineTo(qq[3]+0.4*u);
        path.lineTo(qq[0]+0.4*u);
        path.lineTo(qq[0]);
        QPainterPath path2;
        path2.moveTo(qq[2]);
        path2.lineTo(qq[1]);
        path2.lineTo(qq[1]-0.4*u);
        path2.lineTo(qq[2]-0.4*u);
        path2.lineTo(qq[2]);
        OTpathes.append(PATH_AND_PEN(path,QPen(itemColor(ATTENTION_STATUS::STATUS_NORMAL),0),QBrush(itemColor(ATTENTION_STATUS::STATUS_NORMAL))));
        OTpathes.append(PATH_AND_PEN(path2,QPen(itemColor(ATTENTION_STATUS::STATUS_NORMAL),0),QBrush(itemColor(ATTENTION_STATUS::STATUS_NORMAL))));

        drawShapePath.moveTo(0.5*qq[0]+0.5*qq[3]+0.4*u+0.5*v);
        drawShapePath.lineTo(0.5*qq[2]+0.5*qq[1]-0.4*u+0.5*v);
        drawShapePath.lineTo(0.5*qq[2]+0.5*qq[1]-0.4*u-0.5*v);
        drawShapePath.lineTo(0.5*qq[0]+0.5*qq[3]+0.4*u-0.5*v);
        drawShapePath.lineTo(0.5*qq[0]+0.5*qq[3]+0.4*u+0.5*v);

    }

    drawShapePath.closeSubpath();
    boundRect = drawShapePath.boundingRect();

    shapePath.clear();
    shapePath.moveTo(qq[0]);
    QPointF center(0.0,0.0);
    center += qq[0]/myPointList.count();
    for(int i=1;i<myPointList.count();i++){
        shapePath.lineTo(qq[i]);
        center += qq[i]/myPointList.count();
    }
    shapePath.lineTo(qq[0]);
    center.setY(-center.y());

    //部材番号・断面名
    ModelElevationTemplate* tp = static_cast<ModelElevationTemplate*>(myFigureTemplate);
    if( !tp->nameVisible(myDataType))return;
    QColor color = tp->nameColor(myDataType,STRUCTTYPE::NOSTRUCTTYPE,ATTENTION_STATUS::STATUS_NORMAL);
    int size = tp->nameTextSize(myDataType);

    QString name = md->sectionName();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);

    QPointF p1 = myPointList.at(0);
    QPointF p2 = myPointList.at(1) ;

    qreal th = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

//    QColor color = myFigureTemplate->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_NORMAL);;
    OTvaluetext.append( POINT_AND_TEXT(center, th, name,
                                       color, off1,9999999,size) );
    OTvaluetext.append( POINT_AND_TEXT(center, th, QString::number(numm+1),
                                       color, off2,9999999,size) );


}



} // namespace post3dapp
