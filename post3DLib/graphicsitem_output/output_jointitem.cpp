#include "output_jointitem.h"

#include <QBrush>
#include <QDebug>
#include <QPaintEngine>
#include <QPainter>
#include <QStyleOption>
#include "define_npstn.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp {

OutputJointItem::OutputJointItem( FigureTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid , VEC3D mypvec, VEC3D myxvec,bool _shape_on)
    : OutputGraphicsItem( fig, dtype,stype, uid)
{
    myPlaneVector = mypvec; //描画面の法線
    myPlaneXVector = myxvec; //描画面のx正方向

//    sectionText=QString("%1").arg(inum);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    createBoundaryPath();
    createSpringPath();
    if(_shape_on)createJointShapePath();
}

QPainterPath OutputJointItem::shape() const
{
    QPainterPath path;
    path.addRect(boundRect);
    return path;
}

void OutputJointItem::createJointShapePath(){

        QPainterPath circle;
        int sz = 125+(myFigureTemplate->jointSize()-3)*25;
        circle.addEllipse(-sz, -sz, 2*sz, 2*sz);
        drawShapePath = circle;
        boundRect = QRectF(-2*sz, -2*sz, 4*sz, 4*sz);

};

void OutputJointItem::createBoundaryPath()
{

    qDebug()<<"OutputJointItem::createBoundaryPath()";
     const QPen pen = QPen(QBrush(myFigureTemplate->supportColor()), 0);

     qreal factor = 25+(myFigureTemplate->jointSize()-3)*5;
     int fac250 = static_cast<int>(25*factor);
     int fac125 = static_cast<int>(12.5*factor);
     int fac150 = static_cast<int>(15*factor);
     int fac040 = static_cast<int>(4*factor);
     int fac290 = static_cast<int>(29*factor);


     QVector<int> boundary = UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(myUuid)->getBoundary();
     bool yokofix = false;
     bool tatefix = false;
     bool rollfix = false;
     bool isplan = false;
     bool absolute_posfix = false;
     bool absolute_rollfix = false;
     //固定については、全方向の回転自由度固定の時のみ
     if (boundary.at(0) == IFFIX && boundary.at(1) == IFFIX && boundary.at(2) == IFFIX)absolute_posfix = true;
     if (boundary.at(3) == IFFIX && boundary.at(4) == IFFIX && boundary.at(5) == IFFIX)absolute_rollfix = true;

     if(qAbs(CalcVector3D::dot(myPlaneVector, VEC3D(0, 0, 1))) > 0.9)//XY平面
     {
         if (boundary.at(0) == IFFIX)yokofix = true;
         if (boundary.at(1) == IFFIX)tatefix = true;
         if (boundary.at(5) == IFFIX)rollfix = true;
         isplan = true;
     }
     else if(qAbs(CalcVector3D::dot(myPlaneVector, VEC3D(0, 1, 0))) > 0.9)//XZ平面
     {
         if (boundary.at(0) == IFFIX)yokofix = true;
         if (boundary.at(2) == IFFIX)tatefix = true;
         if (boundary.at(4) == IFFIX)rollfix = true;
     }
     else if(qAbs(CalcVector3D::dot(myPlaneVector, VEC3D(1, 0, 0))) > 0.9)//YZ平面
     {
         if (boundary.at(1) == IFFIX)yokofix = true;
         if (boundary.at(2) == IFFIX)tatefix = true;
         if (boundary.at(3) == IFFIX)rollfix = true;
     }
     else {//斜め方向の通りの時、xyの両方向固定なら固定扱い
         if ((boundary.at(0) == IFFIX) && (boundary.at(1) == IFFIX))yokofix = true;
         if (boundary.at(2) == IFFIX)tatefix = true;
         if ((boundary.at(3) == IFFIX) && (boundary.at(4) == IFFIX))rollfix = true;
     }

//     QPainterPath path;
//     path.moveTo(-fac250,0);path.lineTo( fac250, 0);
//     path.moveTo(-fac250, -fac125);path.lineTo( -fac125, 0);
//     path.moveTo(-fac250, -fac250);path.lineTo(0, 0);
//     path.moveTo(-fac125, -fac250);path.lineTo( fac125, 0);
//     path.moveTo(0, -fac250);path.lineTo( fac250, 0);
//     path.moveTo(fac125, -fac250);path.lineTo( fac250, -fac125);
//     OTpathes.append(PATH_AND_PEN(path,pen));
//     return;

    if (!yokofix && !tatefix && !rollfix)return;

    QPainterPath path;
     if (yokofix && tatefix && absolute_rollfix) { //固定
         path.moveTo(-fac250,0);path.lineTo( fac250, 0);
         path.moveTo(-fac250, -fac125);path.lineTo( -fac125, 0);
         path.moveTo(-fac250, -fac250);path.lineTo(0, 0);
         path.moveTo(-fac125, -fac250);path.lineTo( fac125, 0);
         path.moveTo(0, -fac250);path.lineTo( fac250, 0);
         path.moveTo(fac125, -fac250);path.lineTo( fac250, -fac125);
     } else if (yokofix && tatefix && !rollfix) { //ピン
         path.moveTo(-fac150, -fac250);path.lineTo( fac150, -fac250);
         path.moveTo(-fac150, -fac250);path.lineTo(0, 0);
         path.moveTo(0,0);path.lineTo( fac150, -fac250);
     } else if (yokofix && !tatefix && !rollfix) { //上下ピンローラー
         path.moveTo(fac250, fac150);path.lineTo( fac250, -fac150);
         path.moveTo(fac250, -fac150);path.lineTo(0, 0);
         path.moveTo(0,0);path.lineTo( fac250, +fac150);
//         path.moveTo(fac300, fac150);path.lineTo( fac300, -fac150);

         path.addEllipse(QPointF(fac290,-fac125),fac040,fac040);
         path.addEllipse(QPointF(fac290,fac125),fac040,fac040);
     } else if (!yokofix && tatefix && !rollfix) { //水平ピンローラー
         path.moveTo(-fac150, -fac250);path.lineTo( fac150, -fac250);
         path.moveTo(-fac150, -fac250);path.lineTo(0, 0);
         path.moveTo(0,0);path.lineTo( fac150, -fac250);
//         path.moveTo(-fac150, -fac300);path.lineTo( fac150, -fac300);

         path.addEllipse(QPointF(-fac125,-fac290),fac040,fac040);
         path.addEllipse(QPointF(fac125,-fac290),fac040,fac040);
     } else if (yokofix && !tatefix && absolute_rollfix) { //上下固定ローラー
         path.moveTo(0,-fac250);path.lineTo( 0,fac250);
         path.moveTo(fac125, -fac250);path.lineTo(0, -fac125);
         path.moveTo(fac250, -fac250);path.lineTo(0, 0);
         path.moveTo(fac250, -fac125);path.lineTo( 0,fac125);
         path.moveTo( fac250,0);path.lineTo( 0,fac250);
         path.moveTo(fac250, fac125);path.lineTo( fac125, fac250);

         path.moveTo(0, fac250);path.lineTo( fac250, fac250);
         path.moveTo(0,-fac250);path.lineTo( fac250, -fac250);
         path.moveTo(fac250, fac250);path.lineTo( fac250, -fac250);

         path.addEllipse(QPointF(fac290,fac125),fac040,fac040);
         path.addEllipse(QPointF(fac290,-fac125),fac040,fac040);
     } else if (!yokofix && tatefix && absolute_rollfix) { //水平固定ローラー
         path.moveTo(-fac250,0);path.lineTo( fac250, 0);
         path.moveTo(-fac250, -fac125);path.lineTo( -fac125, 0);
         path.moveTo(-fac250, -fac250);path.lineTo(0, 0);
         path.moveTo(-fac125, -fac250);path.lineTo( fac125, 0);
         path.moveTo(0, -fac250);path.lineTo( fac250, 0);
         path.moveTo(fac125, -fac250);path.lineTo( fac250, -fac125);
         path.moveTo(-fac250, 0);path.lineTo( -fac250, -fac250);
         path.moveTo(-fac250, -fac250);path.lineTo( fac250, -fac250);
         path.moveTo(fac250, -fac250);path.lineTo( fac250, 0);

//         path.moveTo(-fac250, -fac300);path.lineTo( fac250, -fac300);
         path.addEllipse(QPointF(-fac125,-fac290),fac040,fac040);
         path.addEllipse(QPointF(fac125,-fac290),fac040,fac040);
     }

     if(!rollfix){
         //白丸
         path.addEllipse(QPointF(0,0),fac040,fac040);
     }

     int fix = 0;
     if (boundary.at(0) == IFFIX)fix += 100000;
     if (boundary.at(1) == IFFIX)fix += 10000;
     if (boundary.at(2) == IFFIX)fix += 1000;
     if (boundary.at(3) == IFFIX)fix += 100;
     if (boundary.at(4) == IFFIX)fix += 10;
     if (boundary.at(5) == IFFIX)fix += 1;
     qDebug()<<"OutputJointItem::boundary ="<<fix;

//     QPainterPath path;
//     //2^6 * n方向。斜めもある。その方向から見てよくあるパターンだけ定義していればOK。（固定、ピン、ピンローラー、固定ローラー。それぞれ上下・左右。ローラーに関しては斜めも）


     OTpathes.append(PATH_AND_PEN(path,pen));

     return;

     //斜めローラー ※斜めローラーは2023年3月現在未実装のため、なんとなくのもの
     DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
     int jno = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
     if(cmdl.jntrl.at(jno) > 0){
         int jntrl = cmdl.jntrl.at(jno)-1;
         qreal theta_r = cmdl.drl.at(jntrl).at(0);
         qreal theta_h = cmdl.drl.at(jntrl).at(1);
         //
         if(isplan  && absolute_rollfix){
             //斜め固定ローラー
             qreal s = qSin(theta_r);
             qreal c = qCos(theta_r);
             path.moveTo(-fac250*c,-fac250*s);path.lineTo( fac250*c, fac250*s);
             path.lineTo(fac250*c+fac250*s,fac250*s -fac250*c);path.lineTo(-fac250*c+fac250*s,-fac250*s -fac250*c);path.lineTo(-fac250*c, -fac250*s);
             path.moveTo(-fac250*c+fac125*s,-fac250*s -fac125*c);path.lineTo( -fac125*c, -fac125*s);
             path.moveTo(-fac250*c+fac250*s,-fac250*s -fac250*c);path.lineTo(0, 0);
             path.moveTo(-fac125*c+fac250*s,-fac125*s -fac250*c);path.lineTo( fac125*c, fac125*s);
             path.moveTo(fac250*s, -fac250*c);path.lineTo( fac250*c, fac125*s);
             path.moveTo(fac125*c +fac250*s,fac125*s -fac250*c);path.lineTo( fac250*c+fac125*s,fac250*s -fac125*c);

             path.addEllipse(QPointF(-fac125*c +fac290*s,-fac125*s -fac290*c),fac040,fac040);
             path.addEllipse(QPointF(fac125*c +fac290*s,fac125*s -fac290*c),fac040,fac040);

         }else if(isplan  && !absolute_rollfix){
             //斜めピンローラー
             qreal s = qSin(theta_r);
             qreal c = qCos(theta_r);
             path.moveTo(-fac150 * c + fac250 * s,-fac150 * s - fac250 * c);path.lineTo( fac150 * c + fac250 * s,fac150 * s  - fac250 * c);
             path.moveTo(-fac150 * c -fac250 * s,-fac150 * s + fac250 * c);path.lineTo(0, 0);
             path.moveTo(0,0);path.lineTo( fac150 * c  + fac250 * s ,fac150 * s  - fac250 * c);

             path.addEllipse(QPointF(-fac125*c +fac290*s,-fac125*s -fac290*c),fac040,fac040);
             path.addEllipse(QPointF(fac125*c +fac290*s,fac125*s -fac290*c),fac040,fac040);

         }else if(!isplan  && absolute_rollfix){
             qreal s = qSin(theta_h);
             qreal c = qCos(theta_h);
             path.moveTo(-fac250*c,-fac250*s);path.lineTo( fac250*c, fac250*s);
             path.lineTo(fac250*c+fac250*s,fac250*s -fac250*c);path.lineTo(-fac250*c+fac250*s,-fac250*s -fac250*c);path.lineTo(-fac250*c, -fac250*s);
             path.moveTo(-fac250*c+fac125*s,-fac250*s -fac125*c);path.lineTo( -fac125*c, -fac125*s);
             path.moveTo(-fac250*c+fac250*s,-fac250*s -fac250*c);path.lineTo(0, 0);
             path.moveTo(-fac125*c+fac250*s,-fac125*s -fac250*c);path.lineTo( fac125*c, fac125*s);
             path.moveTo(fac250*s, -fac250*c);path.lineTo( fac250*c, fac125*s);
             path.moveTo(fac125*c +fac250*s,fac125*s -fac250*c);path.lineTo( fac250*c+fac125*s,fac250*s -fac125*c);

             path.addEllipse(QPointF(-fac125*c +fac290*s,-fac125*s -fac290*c),fac040,fac040);
             path.addEllipse(QPointF(fac125*c +fac290*s,fac125*s -fac290*c),fac040,fac040);
         }else if(!isplan  && !absolute_rollfix){
             qreal s = qSin(theta_h);
             qreal c = qCos(theta_h);
             path.moveTo(-fac150 * c + fac250 * s,-fac150 * s - fac250 * c);path.lineTo( fac150 * c + fac250 * s,fac150 * s  - fac250 * c);
             path.moveTo(-fac150 * c -fac250 * s,-fac150 * s + fac250 * c);path.lineTo(0, 0);
             path.moveTo(0,0);path.lineTo( fac150 * c  + fac250 * s ,fac150 * s  - fac250 * c);

             path.addEllipse(QPointF(-fac125*c +fac290*s,-fac125*s -fac290*c),fac040,fac040);
             path.addEllipse(QPointF(fac125*c +fac290*s,fac125*s -fac290*c),fac040,fac040);

         }

     }


}
void OutputJointItem::createSpringPath(){
    qDebug()<<"OutputJointItem::createSpringPath()";
    const QPen pen = QPen(QBrush(myFigureTemplate->figureColor(DATATYPE::TPSPRING,STRUCTTYPE::NOSTRUCTTYPE,ATTENTION_STATUS::STATUS_NORMAL)), 0);
    qreal factor = 25+(myFigureTemplate->jointSize()-3)*5;

    QList<JOINTSPRING *> springs = UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer( myUuid)->getJointSpring();
    qDebug()<<"OutputJointItem::createSpringPath spring count"<<springs.count();
    for (int iii = 0; iii < springs.count(); iii++) {
        JOINTSPRING *spring = springs.at(iii);
        VEC3D vn = CalcVector3D::normalize(myPlaneVector);
        VEC3D vb = CalcVector3D::normalize(spring->springVec);
        if (spring->isRotation && qAbs(CalcVector3D::dot(vn, vb)) > 0.9) {
            qDebug()<<"OutputJointItem::createSpringPath rotation";

//            QPainterPath circle;
//            circle.addEllipse(-fac150, -fac150, fac300, fac300);
//            circle.addEllipse(-fac125, -fac125, fac250, fac250);
//            circle.addEllipse(-fac100, -fac100, fac200, fac200);
//            OTpathes.append(PATH_AND_PEN(circle,pen));

            QPainterPath circle;
            qreal rr =12.5*factor;
            for(int ii=0;ii<36*2+1;ii++){
                qreal r = rr*(qreal)ii/36.0;
                qreal th = (qreal)ii/36.0*M_TAU ;
                circle.lineTo(QPointF(r*qCos(th),r*qSin(th)));
            }
            OTpathes.append(PATH_AND_PEN(circle,pen,QBrush(Qt::NoBrush)));

        }
        if ((!spring->isRotation) && qAbs(CalcVector3D::dot(vn, vb)) < 0.1) {
            qDebug()<<"OutputJointItem::createSpringPath jiku";
            VEC3D v1 = myPlaneXVector;
            VEC3D v3 = myPlaneVector;
            VEC3D v2 = CalcVector3D::crossNormalized(v3, v1);
            VEC3D baneXY = VEC3D(CalcVector3D::dot(v1, spring->springVec), CalcVector3D::dot(v2, spring->springVec));
            QPainterPath path;

            QPointF p0 = QPointF(0, 0);
            QPointF p1 = QPointF(7.5 * factor, 0);
            QPointF p2 = QPointF(10 * factor, 5 * factor);
            QPointF p3 = QPointF(15 * factor, -5 * factor);
            QPointF p4 = QPointF(20 * factor, 5 * factor);
            QPointF p5 = QPointF(25 * factor, -5 * factor);
            QPointF p6 = QPointF(30 * factor, 5 * factor);
            QPointF p7 = QPointF(35 * factor, -5 * factor);
            QPointF p8 = QPointF(37.5 * factor, 0);
            QPointF p9 = QPointF(45 * factor, 0);
            path.moveTo(p0);
            path.lineTo(p1);
            path.lineTo(p2);
            path.lineTo(p3);
            path.lineTo(p4);
            path.lineTo(p5);
            path.lineTo(p6);
            path.lineTo(p7);
            path.lineTo(p8);
            path.lineTo(p9);

            QTransform rot;
            rot.rotate(180 / M_PI * atan2(baneXY.y, baneXY.x) + 180);
            OTpathes.append(PATH_AND_PEN(rot.map(path),pen));
//            painter->rotate(-180 / M_PI * atan2(baneXY.y, baneXY.x) - 180);
        }
    }
    qDebug()<<"OutputJointItem::createSpringPath done";

}
} // namespace post3dapp

