#include "outputitem_jshearmap.h"
#include "unified_data.h"
#include "unified_output.h"
#include "unified_settings.h"

#include <QStyleOption>
#include <QPainter>
#include <qmath.h>
#include <QDebug>
#include <QStyle>
QUuid OutputJointShearMapItem:: myFloorId;
qreal OutputJointShearMapItem:: sumShearX;
qreal OutputJointShearMapItem:: sumShearY;
QStringList OutputJointShearMapItem:: loadName;
QList<qreal> OutputJointShearMapItem:: coeflist;
QList<int> OutputJointShearMapItem:: steplist;

OutputJointShearMapItem::OutputJointShearMapItem(OUTFIGTYPE figtype, DATATYPE dtype, VEC3D myPlaneVec,
                                                 const QUuid &uid)
    : OutputGraphicsItem( dtype, uid, QString() )
{
    stringSize = UnifiedSettings::getInstance()->getOutput2DViewSettings().valueFontSize;
    stringColor = UnifiedSettings::getInstance()->getOutput2DViewSettings().valueColor;
    figType = figtype;
    myId = uid;
    myPlaneVector = myPlaneVec;
    putShearValue(loadName,coeflist,steplist);
    myModel = "MODEL1";

}

void OutputJointShearMapItem::setDrawingStatus( int idx_a, int idx_b, int idx_c, int idx_d,
                                                int step )
{
    qDebug() << "from widget";

    OUTPUTDIR outdir = static_cast<OUTPUTDIR>(idx_a);
    bool isInplane = (outdir == ODIR_PLIN);


    loadName.clear();
    coeflist.clear();
    steplist.clear();
    QStringList stress = UnifiedOutput::getInstance()->getStressCaseList();
    if (idx_b != 0) {
        QString load = stress.at(idx_b);
        loadName << load;
        coeflist << 1.0;
        steplist << UnifiedOutput::getInstance()->getMaxStep(load);
    }
    if (idx_c != 0) {
        QString load = stress.at(idx_c);
        loadName << load;
        coeflist << 1.0;
        steplist << UnifiedOutput::getInstance()->getMaxStep(load);
    }
    if (idx_d != 0) {
        QString load = stress.at(idx_d);
        loadName << load;
        coeflist << 1.0;
        steplist << step;
    }
    OUTFIG_RANGE_OPTION op;
    op.coefList = coeflist;
    op.stepList = steplist;
    op.stressCaseList = loadName;
    setDrawingStatusDirect(op);

}

void OutputJointShearMapItem::setDrawingStatusDirect(const OUTFIG_RANGE_OPTION &range)
{
    loadName = range.stressCaseList;
    coeflist = range.coefList;
    steplist = range.stepList;
    QList<qreal> reaction = UnifiedOutput::getInstance()->getCombinationReaction(myId, loadName,
                                                                                 coeflist, steplist);
    XYZ disp = UnifiedOutput::getInstance()->getCombinationJointDisp(myId, loadName,
                                                                     coeflist, steplist);

    JointData *joint = UnifiedData::getInstance()->jointIDToPointer(myId);
    qreal xcoord = joint->xPos();
    qreal ycoord = joint->yPos();
    qreal zcoord = joint->zPos();
    putShearValue(loadName,coeflist,steplist);


}
void OutputJointShearMapItem::setDrawingStatusOnly( int idx_a, int idx_b, int idx_c, int idx_d,
                                                    int step )
{
    OUTPUTDIR outdir = static_cast<OUTPUTDIR>(idx_a);
    bool isInplane = (outdir == ODIR_PLIN);

    loadName.clear();
    coeflist.clear();
    steplist.clear();
    QStringList stress = UnifiedOutput::getInstance()->getStressCaseList();
    if (idx_b != 0) {
        QString load = stress.at(idx_b);
        loadName << load;
        coeflist << 1.0;
        steplist << UnifiedOutput::getInstance()->getMaxStep(load);
    }
    if (idx_c != 0) {
        QString load = stress.at(idx_c);
        loadName << load;
        coeflist << 1.0;
        steplist << UnifiedOutput::getInstance()->getMaxStep(load);
    }
    if (idx_d != 0) {
        QString load = stress.at(idx_d);
        loadName << load;
        coeflist << 1.0;
        steplist << step;
    }
}

void OutputJointShearMapItem::setDrawingStatusDirectOnly(const OUTFIG_RANGE_OPTION &range)
{
    loadName = range.stressCaseList;
    coeflist = range.coefList;
    steplist = range.stepList;

}

void OutputJointShearMapItem::putShearValue(QStringList loadName,QList<qreal>coeflist,QList<int>steplist){
    qreal shearx=0.0;
    qreal sheary=0.0;
    JointData* jd = UnifiedData::getInstance()->jointIDToPointer(myId);
    JointData* other;
    QList<MemberData*> mds = jd->getComposeMembers();
    VEC3D vx;
    foreach(MemberData* md , mds){
        if(md->dataType() == TPCOLUMN || md->dataType() == TPBRACE || md->dataType() == TPGIRDER){
            continue;
            QList<JointData*> jdlist = md->getJointList();
            if(jdlist.first()==jd){
                other = jdlist.last();
                vx = VEC3D(other->xPos()-jd->xPos(),other->yPos()-jd->yPos(),other->zPos()-jd->zPos());
            }else{
                other = jdlist.first();
                vx = VEC3D(other->xPos()-jd->xPos(),other->yPos()-jd->yPos(),other->zPos()-jd->zPos());
            }
            qreal floorheight = UnifiedData::getInstance()->getFloorHeight(myFloorId);
            qreal otherheight = UnifiedData::getInstance()->getFloorHeight(other->getFirstFloor());
            qreal myheight = UnifiedData::getInstance()->getFloorHeight(jd->getFirstFloor());
//            if(UnifiedData::getInstance()->getFloorValue(myFloorId).floorType==FLOORTYPE::FL_CHILD)
            if(otherheight <= floorheight){continue;}
            if(myheight > floorheight){continue;}//自分が階上の時無視
            if(qAbs(CalcVector3D::dot(vx,myPlaneVector)) < 0.001){continue;}//法線と直交＝平面内のとき、無視
            qDebug()<<"shearmap"<<jd->zPos()<<other->zPos()<<floorheight;
            MemberForce force=UnifiedOutput::getInstance()->getCombinationNodeMemberForce(md->getUuid(),coeflist,loadName,steplist);
            qreal Qx1=CalcVector3D::dot(md->getUVector(),VEC3D(1,0,0))*force.N;
            qreal Qx2=CalcVector3D::dot(md->getVVector(),VEC3D(1,0,0))*force.getQy(0.5);
            qreal Qx3=CalcVector3D::dot(md->getWVector(),VEC3D(1,0,0))*force.getQz(0.5);
            qreal Qy1=CalcVector3D::dot(md->getUVector(),VEC3D(0,1,0))*force.N;
            qreal Qy2=CalcVector3D::dot(md->getVVector(),VEC3D(0,1,0))*force.getQy(0.5);
            qreal Qy3=CalcVector3D::dot(md->getWVector(),VEC3D(0,1,0))*force.getQz(0.5);
            shearx = Qx1+Qx2+Qx3;
            sheary = Qy1+Qy2+Qy3;

            qreal shear;// = qSqrt(shearx*shearx + sheary*sheary);
            qreal sumShear;// = qSqrt(sumShearX*sumShearX + sumShearY*sumShearY);
            if(qAbs(sumShearX)>qAbs(sumShearY)){
                sumShear=sumShearX;
                shear = shearx;
            }else{
                sumShear=sumShearY;
                shear = sheary;
            }

            qDebug()<<"sumShear"<<sumShear;
            GlobalPoint sgp = jd->toGlobalPoint();
            GlobalPoint egp = other->toGlobalPoint();
            QPointF ep = egp.getXYQPointF();
            QPointF sp = sgp.getXYQPointF();
            ep.setY(-ep.y());
            sp.setY(-sp.y());
            qDebug()<<sp<<ep;

            if(qAbs(shear) > 0.001){
                valueTextPos.append( POINTANDTEXT(1000*0.5*(ep-sp), 0.0, QString::number(shearx, 'f', 1), stringColor,
                                                  OFFSET_RIGHTTOP, 999999) );
                valueTextPos.append( POINTANDTEXT(1000*0.5*(ep-sp), 0.0, QString::number(sheary, 'f', 1), stringColor,
                                                  OFFSET_RIGHTMIDDLE, 999999) );
                valueTextPos.append( POINTANDTEXT(1000*0.5*(ep-sp), 0.0, "("+QString::number(100*shear/sumShear, 'f', 1)+")", stringColor,
                                                  OFFSET_RIGHTBOTTOM, 999999) );
            }
        }
        else if(md->dataType() == TPWALL){

            QList<JointData*> jdlist = md->getJointList();
            JointData* next;
            JointData* oppo;
            QList<bool> isUpper;
            QList<bool> isOnFloor;
            if(jdlist.first()==jd){
                other = jdlist.last();
                next = jdlist.at(1);
                oppo = jdlist.at(2);
            }else{
                continue;
            }
            qreal floorheight = UnifiedData::getInstance()->getFloorHeight(myFloorId);
            qreal otherheight = UnifiedData::getInstance()->getFloorHeight(other->getFirstFloor());
            qreal nextheight = UnifiedData::getInstance()->getFloorHeight(next->getFirstFloor());
            qreal oppoheight = UnifiedData::getInstance()->getFloorHeight(oppo->getFirstFloor());
            qreal myheight = UnifiedData::getInstance()->getFloorHeight(jd->getFirstFloor());
            isUpper << (myheight > floorheight);
            isUpper << (nextheight > floorheight);
            isUpper << (oppoheight > floorheight);
            isUpper << (otherheight > floorheight);
            isOnFloor << (qAbs(myheight - floorheight)<0.001);
            isOnFloor  << (qAbs(nextheight - floorheight)<0.001);
            isOnFloor  << (qAbs(oppoheight - floorheight)<0.001);
            isOnFloor  << (qAbs(otherheight - floorheight)<0.001);
            if(isUpper[0] == true && isUpper[1] == true &&isUpper[2] == true &&isUpper[3] == true)continue;
            if(isUpper[0] == false && isUpper[1] == false &&isUpper[2] == false &&isUpper[3] == false)continue;


            //方向は、EVMEMWの値を保持。UVWベクトルに保管
            MemberForce force=UnifiedOutput::getInstance()->getCombinationNodeMemberForce(md->getUuid(),coeflist,loadName,steplist);
            qreal shear = force.getQy(0.5);
            QList<QList<qreal>> tmat = getTMat(md,myModel);
            QList<qreal> localForce;localForce<<shear<<force.N<<force.iMy;
            QList<qreal> globalForce = getGlobalForce(tmat,localForce,md);
            qreal Qx1 = 0.0;
            qreal Qy1 = 0.0;
            qreal Qz1 = 0.0;
            for(int i=0;i<4;i++){
//                if(!isUpper[i] && i<2){
//                    Qx1 += globalForce.at(3*i);
//                    Qy1 += globalForce.at(3*i+1);
//                    Qz1 += globalForce.at(3*i+2);
//                }
                if((isOnFloor[i] || isUpper[i]) && i>=2){
                    Qx1 += globalForce.at(3*i);
                    Qy1 += globalForce.at(3*i+1);
                    Qz1 += globalForce.at(3*i+2);
                }
            }
//            qreal nQx1 =-1*CalcVector3D::dot(md->getUVector(),VEC3D(1,0,0))*Qx1;
//            nQx1+=-1*CalcVector3D::dot(md->getVVector(),VEC3D(1,0,0))*Qy1;
//            nQx1+=-1*CalcVector3D::dot(md->getWVector(),VEC3D(1,0,0))*Qz1;
//            qreal nQy1 =-1*CalcVector3D::dot(md->getUVector(),VEC3D(0,1,0))*Qx1;
//            nQy1+=-1*CalcVector3D::dot(md->getVVector(),VEC3D(0,1,0))*Qy1;
//            nQy1+=-1*CalcVector3D::dot(md->getWVector(),VEC3D(0,1,0))*Qz1;

            qDebug()<<"U"<<md->getUVector().x<<md->getUVector().y<<md->getUVector().z;
            qDebug()<<"V"<<md->getVVector().x<<md->getVVector().y<<md->getVVector().z;
            qDebug()<<"W"<<md->getWVector().x<<md->getWVector().y<<md->getWVector().z;
//            qreal nQx1 = 1*Qx1*CalcVector3D::dot(VEC3D(1,0,0),md->getUVector())
//                    + Qy1*CalcVector3D::dot(VEC3D(1,0,0),md->getVVector())
//                    + Qz1*CalcVector3D::dot(VEC3D(1,0,0),md->getWVector());
//            qreal nQy1 = -1*Qx1*CalcVector3D::dot(VEC3D(0,1,0),md->getUVector())
//                    - Qy1*CalcVector3D::dot(VEC3D(0,1,0),md->getVVector())
//                    - Qz1*CalcVector3D::dot(VEC3D(0,1,0),md->getWVector());
            qreal nQx1 = 1*Qz1*CalcVector3D::dot(VEC3D(0,1,0),md->getVVector())
                    + Qy1*CalcVector3D::dot(VEC3D(0,1,0),md->getUVector())
                    + Qx1*CalcVector3D::dot(VEC3D(0,1,0),md->getWVector());
            qreal nQy1 = -1*Qz1*CalcVector3D::dot(VEC3D(1,0,0),md->getVVector())
                    - Qy1*CalcVector3D::dot(VEC3D(1,0,0),md->getUVector())
                    - Qx1*CalcVector3D::dot(VEC3D(1,0,0),md->getWVector());
            qreal nQz1 = -1*Qz1*CalcVector3D::dot(VEC3D(0,0,1),md->getVVector())
                    - Qy1*CalcVector3D::dot(VEC3D(0,0,1),md->getUVector())
                    - Qx1*CalcVector3D::dot(VEC3D(0,0,1),md->getWVector());
            shearx = nQx1;//+Qx3;
            sheary = nQy1;//+Qy3;
            qreal axialz = nQz1;//+Qy3;

            //座標は壁の節点の平均でOK
            QPointF center = QPointF(0,0);
            foreach(JointData* joint,jdlist){
                GlobalPoint gp = joint->toGlobalPoint();
                QPointF pp = gp.getXYQPointF();
                center  += pp;
            }
            center /= jdlist.count();
            QPointF sp = jd->toGlobalPoint().getXYQPointF();
            center.setY(-center.y());
            sp.setY(-sp.y());
            qreal sumShear;// = qSqrt(sumShearX*sumShearX + sumShearY*sumShearY);
            if(qAbs(sumShearX)>qAbs(sumShearY)){
                sumShear=sumShearX;
                shear = shearx;
            }else{
                sumShear=sumShearY;
                shear = sheary;
            }


            if(qAbs(shearx)+qAbs(sheary) > 0.001){
                valueTextPos.append( POINTANDTEXT(-1000*(sp-center), 0.0, QString::number(shearx, 'f', 1), stringColor,
                                                  OFFSET_RIGHTTOP, 999999) );
                valueTextPos.append( POINTANDTEXT(-1000*(sp-center), 0.0, QString::number(sheary, 'f', 1), stringColor,
                                                  OFFSET_RIGHTMIDDLE, 999999) );
                valueTextPos.append( POINTANDTEXT(-1000*(sp-center), 0.0, QString::number(axialz, 'f', 1), stringColor,
                                                  OFFSET_RIGHTBOTTOM, 999999) );
//                valueTextPos.append( POINTANDTEXT(-1000*(sp-center), 0.0, "("+QString::number(100*shear/sumShear, 'f', 1)+")", stringColor,
//                                                  OFFSET_RIGHTBOTTOM, 999999) );
            }

        }
        else if(md->dataType() == TPSPRING){

        }
    }

}
void OutputJointShearMapItem::resetSumShear(const QUuid& floorId){
    myFloorId = floorId;
    sumShearX=0.0;
    sumShearY=0.0;
    foreach(JointData* jd,UnifiedData::getInstance()->jointListOfAll()){
        QList<MemberData*> mds = jd->getComposeMembers();
        VEC3D vx;
        JointData* other;

        foreach(MemberData* md , mds){
            if(md->dataType() == TPCOLUMN || md->dataType() == TPBRACE || md->dataType() == TPGIRDER){
                QList<JointData*> jdlist = md->getJointList();
                if(jdlist.first()==jd){
                    other = jdlist.last();
                    vx = VEC3D(other->xPos()-jd->xPos(),other->yPos()-jd->yPos(),other->zPos()-jd->zPos());
                }else{
                    other = jdlist.first();
                    vx = VEC3D(other->xPos()-jd->xPos(),other->yPos()-jd->yPos(),other->zPos()-jd->zPos());
                }
                qreal floorheight = UnifiedData::getInstance()->getFloorHeight(myFloorId);
                qreal otherheight = UnifiedData::getInstance()->getFloorHeight(other->getFirstFloor());
                qreal myheight = UnifiedData::getInstance()->getFloorHeight(jd->getFirstFloor());
    //            if(UnifiedData::getInstance()->getFloorValue(myFloorId).floorType==FLOORTYPE::FL_CHILD)
                if(otherheight <= floorheight){continue;}
                if(myheight > floorheight){continue;}//自分が階上の時無視
                if(qAbs(myheight - otherheight) < 0.001){continue;}//平面内のとき、無視
                qDebug()<<"shearmap"<<jd->zPos()<<other->zPos()<<floorheight;
                MemberForce force=UnifiedOutput::getInstance()->getCombinationNodeMemberForce(md->getUuid(),coeflist,loadName,steplist);
                qreal Qx1=CalcVector3D::dot(md->getUVector(),VEC3D(1,0,0))*force.N;
                qreal Qx2=CalcVector3D::dot(md->getVVector(),VEC3D(1,0,0))*force.getQy(0.5);
                qreal Qx3=CalcVector3D::dot(md->getWVector(),VEC3D(1,0,0))*force.getQz(0.5);
                qreal Qy1=CalcVector3D::dot(md->getUVector(),VEC3D(0,1,0))*force.N;
                qreal Qy2=CalcVector3D::dot(md->getVVector(),VEC3D(0,1,0))*force.getQy(0.5);
                qreal Qy3=CalcVector3D::dot(md->getWVector(),VEC3D(0,1,0))*force.getQz(0.5);
                sumShearX += Qx1+Qx2+Qx3;
                sumShearY += Qy1+Qy2+Qy3;



            }
            else if(md->dataType() == TPWALL){

                QList<JointData*> jdlist = md->getJointList();
                JointData* next;
                JointData* oppo;
                QList<bool> isUpper;
                QList<bool> isOnFloor;
                if(jdlist.first()==jd){
                    other = jdlist.last();
                    next = jdlist.at(1);
                    oppo = jdlist.at(2);
                    qDebug()<<md->getUuid();
                }else{
                    continue;
                }
                qreal floorheight = UnifiedData::getInstance()->getFloorHeight(myFloorId);
                qreal otherheight = UnifiedData::getInstance()->getFloorHeight(other->getFirstFloor());
                qreal nextheight = UnifiedData::getInstance()->getFloorHeight(next->getFirstFloor());
                qreal oppoheight = UnifiedData::getInstance()->getFloorHeight(oppo->getFirstFloor());
                qreal myheight = UnifiedData::getInstance()->getFloorHeight(jd->getFirstFloor());
                isUpper << (myheight > floorheight);
                isUpper << (nextheight > floorheight);
                isUpper << (oppoheight > floorheight);
                isUpper << (otherheight > floorheight);
                isOnFloor << (qAbs(myheight - floorheight)<0.001);
                isOnFloor  << (qAbs(nextheight - floorheight)<0.001);
                isOnFloor  << (qAbs(oppoheight - floorheight)<0.001);
                isOnFloor  << (qAbs(otherheight - floorheight)<0.001);
                if(isUpper[0] == true && isUpper[1] == true &&isUpper[2] == true &&isUpper[3] == true)continue;
                if(isUpper[0] == false && isUpper[1] == false &&isUpper[2] == false &&isUpper[3] == false)continue;


                //方向は、EVMEMWの値を保持。UVWベクトルに保管
                MemberForce force=UnifiedOutput::getInstance()->getCombinationNodeMemberForce(md->getUuid(),coeflist,loadName,steplist);
                qreal shear = force.getQy(0.5);
                QList<QList<qreal>> tmat = getTMat(md,"MODEL1");
                QList<qreal> localForce;localForce<<shear<<force.N<<force.iMy;
                QList<qreal> globalForce = getGlobalForce(tmat,localForce,md);
                qreal Qx1 = 0.0;
                qreal Qy1 = 0.0;
                qreal Qz1 = 0.0;
                for(int i=0;i<4;i++){
                    if(isUpper[i]==true){
//                    if(isOnFloor[i]==true){
                        Qx1 += globalForce.at(3*i);
                        Qy1 += globalForce.at(3*i+1);
                        Qz1 += globalForce.at(3*i+2);
                    }
                }
                qreal nQx1 =-1*CalcVector3D::dot(md->getUVector(),VEC3D(1,0,0))*Qx1;
                nQx1+=-1*CalcVector3D::dot(md->getVVector(),VEC3D(1,0,0))*Qy1;
                nQx1+=-1*CalcVector3D::dot(md->getWVector(),VEC3D(1,0,0))*Qz1;
                qreal nQy1 =-1*CalcVector3D::dot(md->getUVector(),VEC3D(0,1,0))*Qx1;
                nQy1+=-1*CalcVector3D::dot(md->getVVector(),VEC3D(0,1,0))*Qy1;
                nQy1+=-1*CalcVector3D::dot(md->getWVector(),VEC3D(0,1,0))*Qz1;
                sumShearX += nQx1;//+Qx3;
                sumShearY += nQy1;//+Qy3;


            }

        }
    }
    qDebug()<<"shear = "<<sumShearX<<sumShearY;
}

void OutputJointShearMapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                    QWidget *)
{

    qreal dpiScale = 72.0 / painter->device()->logicalDpiX();
    QFont orgfont = painter->font();

    setPainter(painter, option);
    QPen orgpen = painter->pen();
    const QPen pen = QPen(QBrush(stringColor), 0);
    painter->setPen(pen);

    qreal factor = 1.0 / qPow(painter->transform().m11(), 0.75);
    boundRect = QRectF(-10.0 * factor, -10.0 * factor, 20.0 * factor, 20.0 * factor);
    JointData* jd = UnifiedData::getInstance()->jointIDToPointer(myId);
    if(jd->getAttachedFloorList().contains(myFloorId))painter->drawEllipse(-5.0 * factor, -5.0 * factor, 10.0 * factor, 10.0 * factor);


    painter->setPen(orgpen);
    QTransform orgtrans = painter->transform();

    QFont scaledfont = QFont("Consolas", (stringSize * 3 + 20) * factor * dpiScale);
    painter->setFont(scaledfont);

    qreal maxTextWidth = 0.0;
    Q_FOREACH ( POINTANDTEXT pst, valueTextPos ) {
        qreal textwidth = painter->fontMetrics().width(pst.vtext);
        if (maxTextWidth < textwidth)maxTextWidth = textwidth;

    }

    Q_FOREACH ( POINTANDTEXT pst, valueTextPos ) {
        ( option->state & QStyle::State_Selected ) ?
                    painter->setPen(selectColor) : painter->setPen(pst.color);
        painter->setTransform(orgtrans);
        painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
        qreal textwidth = maxTextWidth;
         qreal xmargin = painter->fontMetrics().width('8'); ///1.5;
        qreal ymargin = painter->fontMetrics().height() / 4.0;
        qreal widthStrech = 1.0;
        if (textwidth > pst.MaxLength - xmargin)widthStrech = (pst.MaxLength - xmargin) / textwidth;
        widthStrech = qMin(0.8, widthStrech);
        widthStrech = qMax(0.3, widthStrech); //横のスケールの上下限を決める
        int textheight = painter->fontMetrics().height();
        painter->translate(pst.pos.x(), pst.pos.y());
        QPainterPath path;
        path.addEllipse(-15.0 * factor, -15.0 * factor, 30.0 * factor, 30.0 * factor);
        painter->setBrush(QBrush(Qt::NoBrush));
        QPen pen(QBrush(),0);
        painter->strokePath(path,pen);
        //        painter->drawEllipse(-10.0 * factor, -10.0 * factor, 20.0 * factor, 20.0 * factor);
        textwidth = textwidth * widthStrech;
         painter->rotate(-pst.angle * 180.0 / M_PI);
        painter->scale(widthStrech, 1.0);
        if (pst.vtext == "-0" || pst.vtext == "0")continue;
        switch (pst.offset) {
        case (OFFSET_RIGHTTOP):
            painter->drawText(QRect(0.0 + xmargin, 0.0, textwidth + xmargin, textheight),
                              Qt::AlignRight | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSET_RIGHTMIDDLE):
            painter->drawText(QRect(0.0 + xmargin, textheight, textwidth + xmargin, textheight),
                              Qt::AlignRight | Qt::AlignVCenter, pst.vtext);
            break;
        case (OFFSET_RIGHTBOTTOM):
            painter->drawText(QRect(0.0 + xmargin, 2 * textheight, textwidth + xmargin, textheight),
                              Qt::AlignRight | Qt::AlignVCenter, pst.vtext);
            break;
        default:
            painter->drawText(QRect(-0.5 * textwidth, -0.5 * textheight, textwidth + xmargin, textheight),
                              Qt::AlignHCenter | Qt::AlignVCenter, pst.vtext);
            break;
        }
    }
    painter->setPen(orgpen);
    painter->setFont(orgfont);
    painter->setTransform(orgtrans);

}

void OutputJointShearMapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void OutputJointShearMapItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void OutputJointShearMapItem::mouseMoveEvent(QGraphicsSceneMouseEvent *)
{
}
QList<QList<qreal>> OutputJointShearMapItem::getTMat(MemberData* md,QString model){
    qDebug()<<"getTMat";
    QList<QList<qreal>> TMat;
    QList<qreal> row1,row2,row3;

    VEC3D uvec = md->getUVector();
    VEC3D vvec = md->getVVector();
    VEC3D wvec = CalcVector3D::cross(uvec,vvec);
    qDebug()<<uvec.z<<vvec.z<<wvec.z;
    PREADATA* prea = UnifiedOutput::getInstance()->getPrea("MODEL1",md->getUuid());
    qDebug()<<prea->EIy;
    qreal phiB=prea->phaiB;//梁の部材角
    qreal phiU=prea->phaiU;//梁の部材角
    qreal bmLB=prea->lenBeamB;//梁の長さ
    qreal bmLU=prea->lenBeamU;//梁の長さ

    row1<<0.5*uvec.x<<0.5*vvec.x<<0.5*wvec.x<<0.5*uvec.x<<0.5*vvec.x<<0.5*wvec.x;
    row1<<0.5*uvec.x<<0.5*vvec.x<<0.5*wvec.x<<0.5*uvec.x<<0.5*vvec.x<<0.5*wvec.x;
    row2<<0.5*uvec.y<<0.5*vvec.y<<0.5*wvec.y<<0.5*uvec.y<<0.5*vvec.y<<0.5*wvec.y;
    row2<<0.5*uvec.y<<0.5*vvec.y<<0.5*wvec.y<<0.5*uvec.y<<0.5*vvec.y<<0.5*wvec.y;
    qreal cc = qCos(phiB)/bmLB;
    qreal ss = qSin(phiB)/bmLB;
    row3<<cc*uvec.x-ss*uvec.y<<cc*vvec.x-ss*vvec.y<<cc*wvec.x-ss*wvec.y;
    row3<<-cc*uvec.x+ss*uvec.y<<-cc*vvec.x+ss*vvec.y<<-cc*wvec.x+ss*wvec.y;
    cc = qCos(phiU)/bmLU;
    ss = qSin(phiU)/bmLU;
    row3<<cc*uvec.x-ss*uvec.y<<cc*vvec.x-ss*vvec.y<<cc*wvec.x-ss*wvec.y;
    row3<<-cc*uvec.x+ss*uvec.y<<-cc*vvec.x+ss*vvec.y<<-cc*wvec.x+ss*wvec.y;
    TMat<<row1<<row2<<row3;

    return TMat;
}
QList<qreal> OutputJointShearMapItem::getGlobalForce(QList<QList<qreal>> TMat,QList<qreal>localForce,MemberData* md){

    qDebug()<<"getGlobalForce";
    QList<qreal> cornerForce;
    QList<qreal> globalForce;
//    {globalForce} = [Tmat]-1 * {localForce}
    for(int i=0;i<12;i++){
        cornerForce << TMat[0][i]*localForce[0] + TMat[1][i]*localForce[1] + TMat[2][i]*localForce[2];
    }
    VEC3D xvec = VEC3D(1,0,0);
    VEC3D yvec = VEC3D(0,1,0);
    VEC3D zvec = VEC3D(0,0,1);
    VEC3D uvec = md->getUVector();
    VEC3D vvec = md->getVVector();
    VEC3D wvec = CalcVector3D::cross(uvec,vvec);

    globalForce << cornerForce[0]*CalcVector3D::dot(uvec,xvec)+cornerForce[1]*CalcVector3D::dot(vvec,xvec)+cornerForce[2]*CalcVector3D::dot(wvec,xvec);
    globalForce << cornerForce[0]*CalcVector3D::dot(uvec,yvec)+cornerForce[1]*CalcVector3D::dot(vvec,yvec)+cornerForce[2]*CalcVector3D::dot(wvec,yvec);
    globalForce << cornerForce[0]*CalcVector3D::dot(uvec,zvec)+cornerForce[1]*CalcVector3D::dot(vvec,zvec)+cornerForce[2]*CalcVector3D::dot(wvec,zvec);

    globalForce << cornerForce[3]*CalcVector3D::dot(uvec,xvec)+cornerForce[4]*CalcVector3D::dot(vvec,xvec)+cornerForce[5]*CalcVector3D::dot(wvec,xvec);
    globalForce << cornerForce[3]*CalcVector3D::dot(uvec,yvec)+cornerForce[4]*CalcVector3D::dot(vvec,yvec)+cornerForce[5]*CalcVector3D::dot(wvec,yvec);
    globalForce << cornerForce[3]*CalcVector3D::dot(uvec,zvec)+cornerForce[4]*CalcVector3D::dot(vvec,zvec)+cornerForce[5]*CalcVector3D::dot(wvec,zvec);

    globalForce << cornerForce[6]*CalcVector3D::dot(uvec,xvec)+cornerForce[7]*CalcVector3D::dot(vvec,xvec)+cornerForce[8]*CalcVector3D::dot(wvec,xvec);
    globalForce << cornerForce[6]*CalcVector3D::dot(uvec,yvec)+cornerForce[7]*CalcVector3D::dot(vvec,yvec)+cornerForce[8]*CalcVector3D::dot(wvec,yvec);
    globalForce << cornerForce[6]*CalcVector3D::dot(uvec,zvec)+cornerForce[7]*CalcVector3D::dot(vvec,zvec)+cornerForce[8]*CalcVector3D::dot(wvec,zvec);

    globalForce << cornerForce[9]*CalcVector3D::dot(uvec,xvec)+cornerForce[10]*CalcVector3D::dot(vvec,xvec)+cornerForce[11]*CalcVector3D::dot(wvec,xvec);
    globalForce << cornerForce[9]*CalcVector3D::dot(uvec,yvec)+cornerForce[10]*CalcVector3D::dot(vvec,yvec)+cornerForce[11]*CalcVector3D::dot(wvec,yvec);
    globalForce << cornerForce[9]*CalcVector3D::dot(uvec,zvec)+cornerForce[10]*CalcVector3D::dot(vvec,zvec)+cornerForce[11]*CalcVector3D::dot(wvec,zvec);
    return globalForce;
//    return cornerForce;
}

