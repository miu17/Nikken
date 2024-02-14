#include "output_lineitem.h"

#include <QBrush>
#include <QDebug>
#include <QPaintEngine>
#include <QPainter>
#include <QStyleOption>

#include "calc_vector2d.h"
#include "define_draw_figure.h"
#include "define_npstn.h"
#include "define_output.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "manage_memberdata.h"
//#include "output_control.h"
#include "unified_output.h"
#include "unified_analysisdata.h"

namespace post3dapp {

OutputLineItem::OutputLineItem( FigureTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid,
                                const QList<QPointF> &plist, bool shape_on )
    : OutputGraphicsItem(fig, dtype,stype, uid)
{

    setFlag(QGraphicsItem::ItemIsSelectable,true);
    QPointF p0 = plist.first();
    myLenList.clear();
    myPointList.clear();
    myLenList.append(0.0);
    myPointList.append(QPointF(0.0, 0.0));
    for ( int i = 1; i < plist.count(); i++ ) {
        myLenList.append( myLenList.last() + CalcVector2D::length( plist.at(i) - plist.at(i - 1) ) );
        myPointList.append( plist.at(i) - p0 );
    }

    if ( shape_on ) createLineShapePath();
}
void  OutputLineItem::createLineShapePath(){

    //    qreal eps = 1.0e-6;
    //    qreal len_mid = ( myLenList.first() + myLenList.last() ) / 2.0;

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



}
QVector<qreal> OutputLineItem::getOutputFaceRigidLength(){
//    QVector<qreal> uso;
//    uso<<400<<400<<100<<100;
//    return uso;
    //rigidpolicyに従ってfi,fj,ri,rjミリメートルのVectorを返す
    //データ未収の場合はreturn0;
//    try{
    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    QString modelName;
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);

    LISTTYPE type = myFigureTemplate->getFigureTypeProperty().listtype();
//    LISTTYPE type = OutputControl::listType(myFigureTemplate->getFigureTypeProperty().type);
    if(type==LISTTYPE::STRESS){
            QList<STRESS> st = myFigureTemplate->getFigureTypeProperty().stress.first();
            DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
            modelName = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstData().value(st.first().rst).nameam;
    }else if(type==LISTTYPE::MODEL){
        modelName = myFigureTemplate->getFigureTypeProperty().model;

    }else{
            QVector<qreal> dummy(4,0.0);
            return dummy;
    }

    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();

    QHash<QString, DataNBLD3D_PREA> preaRaw=UnifiedFixedData::getInstance()->getPstnData()->getPreaData();
    DataNBLD3D_PREA prea = preaRaw.value(modelName);

        qDebug()<<"2";
        qreal RL,RR,FL,FR;
        int gou_flag =prea.imm4.at(numm).at(0);
        qreal RY1=model.dmm1.at(numm).at(0);
        qreal RZ1=model.dmm1.at(numm).at(1);
        qreal RY2=model.dmm1.at(numm).at(3);
        qreal RZ2=model.dmm1.at(numm).at(4);
        qreal FY1 =model.dmm1.at(numm).at(30);
        qreal FZ1 =model.dmm1.at(numm).at(31);
        qreal FY2 =model.dmm1.at(numm).at(32);
        qreal FZ2 =model.dmm1.at(numm).at(33);

        if(gou_flag == 0){
            RL=qMax(RY1,RZ1);
            RR=qMax(RY2,RZ2);
            FL=qMax(FY1,FZ1);
            FR=qMax(FY2,FZ2);
        }else if(gou_flag==1){
            RL=RY1;
            RR=RY2;
            FL=FY1;
            FR=FY2;
        }else if(gou_flag==2){
            RL=RZ1;
            RR=RZ2;
            FL=FZ1;
            FR=FZ2;
        }else if(gou_flag==3){//X軸に近いほう
            if( md->getVVector().x > md->getWVector().x){
                RL=RY1;
                RR=RY2;
                FL=FY1;
                FR=FY2;
            }else{
                RL=RZ1;
                RR=RZ2;
                FL=FZ1;
                FR=FZ2;
            }
        }else if(gou_flag==4){//Y軸に近いほう
            if( md->getVVector().y > md->getWVector().y){
                RL=RY1;
                RR=RY2;
                FL=FY1;
                FR=FY2;
            }else{
                RL=RZ1;
                RR=RZ2;
                FL=FZ1;
                FR=FZ2;
            }
        }else{
            RL=qMax(RY1,RZ1);
            RR=qMax(RY2,RZ2);
            FL=qMax(FY1,FZ1);
            FR=qMax(FY2,FZ2);
        }
        QVector<qreal> result;
        result<<1000.0*FL<<1000.0*FR<<1000.0*RL<<1000.0*RR;
        qDebug()<<"3";
        return result;
//    }catch(...){
//        qDebug()<<"error occured! OutputLineItem::getOutputFaceRigidLength";
//        return QVector<qreal>(4,0.0);
//    }
}

} // namespace post3dapp

