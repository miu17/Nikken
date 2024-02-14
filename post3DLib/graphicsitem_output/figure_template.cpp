#include "figure_template.h"

#include "calc_vector2d.h"
#include "elevationOT_grscene.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "manage_springdata.h"
#include "output_grscene.h"
#include "output_jointitem.h"
#include "output_lineitem.h"
#include "output_lineplaneitem.h"
#include "output_linesectionitem.h"
#include "output_planeitem.h"
#include "output_springitem.h"
#include "planOT_grscene.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace  post3dapp {

FigureTemplate::FigureTemplate(const Analysis2DViewSettings &_setting, const FIGURE_TYPE_PROPERTY &_property, QUuid id)
    : setting(_setting), option(_property),uuid(id)
{

}

bool FigureTemplate::useScaledFont()
{
    return setting.UseScaledFont();
}

OutputGraphicsItem* FigureTemplate::createOffJointItem(const JointData& jd, const QPointF &pp)
{
    OutputGraphicsItem *item = new OutputGraphicsItem(this, DATATYPE::TPJOINT, STRUCTTYPE::NOSTRUCTTYPE,jd.getUuid());
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);
    return item;
}

QPointF FigureTemplate::jointToPoint(JointData *jd, qreal near_x) const
{
    XYZ xyz = UnifiedFixedData::getInstance()->getAnalysisData()->globalXYZToFrameXYZ(uuid, jd->xyz(true));

    // 解析階の高さ補正
    //    xyz.y=xyz.y+UnifiedAnalysisData::getOutInstance()->getFloorValue( jd->getFirstFloor() ).adjustedLevel;

    if ( qAbs(xyz.x) < 1.0e-5 ) {
        if ( qAbs(near_x) < 1.0e-5 ) return QPointF(0.0, xyz.y * 1000.0);
        qreal cx = UnifiedFixedData::getInstance()->getAnalysisData()->globalZToClosedFrameEndX(uuid, jd->zPos()) * 1000.0;
        return ( qAbs(cx - near_x) > qAbs(near_x) ) ? QPointF(0.0, xyz.y * 1000.0)
                                                    : QPointF(cx, xyz.y * 1000.0);
    } else {
        return QPointF(xyz.x * 1000.0, xyz.y * 1000.0);
    }
}

void FigureTemplate::addItems(OutputGrScene* scene){
//    try{
        preCreateItems(scene);
//    }catch(...){
//    }
//    try{
        createJointItems(scene);
//    }catch(...){
//        qDebug()<<"createJointItems error!";
//    }
    createMemberItems(scene);
    createSpringItems(scene);
    createPointItems(scene);
    createTextItems(scene);
    postCreateItems(scene);

}
void FigureTemplate::createSpringItems(OutputGrScene* scene){
    FrameType::FRAMETYPE f_type = UnifiedFixedData::getInstance()->getAnalysisData()->getFrameTypeOfFloor(uuid);
    bool isClosedFrame = ( f_type == FrameType::FRAMETYPE::CLCIRCLE || f_type == FrameType::FRAMETYPE::CLVARIABLE ) ;

    qDebug() << "createSprings";

    QList<QUuid> springList = UnifiedFixedData::getInstance()->getAnalysisData()->getIDList(DATATYPE::TPSPRING);

    Q_FOREACH ( QUuid uid, springList ) {
        SpringData *spring = UnifiedFixedData::getInstance()->getAnalysisData()->springIDToPointer(uid);
        //面上にあるかどうかの処理
        QList<JointData *> jdlist;
        jdlist << spring->iJoint() << spring->jJoint();


        QList<QList<QPointF> > hlinePoints;

        bool prev_on;
        bool cur_on = false;
        bool next_on = jdlist.first()->containsFrame(uuid);

        for (int i = 0; i < jdlist.count() - 1; i++) {

            JointData *jd0 = jdlist.at(i);
            JointData *jd1 = jdlist.at(i + 1);

            QPointF p0 = jointToPoint(jd0);
            QPointF p1 = jointToPoint(jd1);

            bool isIc = ( isClosedFrame && qAbs(p0.x()) < 1.0e-3 );
            bool isJc = ( isClosedFrame && qAbs(p1.x()) < 1.0e-3 );

            prev_on = cur_on;
            cur_on = next_on;
            next_on = jd1->containsFrame(uuid);

            if ( cur_on ) {
                if ( next_on ) {
                    if ( prev_on ) {

                        QList<QPointF> poly = hlinePoints.last();
                        QPointF cpos = ( isJc ) ? jointToPoint(jd1, poly.last().x()) : p1 ;
                        QPointF q1 = poly.at( poly.count() - 2 );
                        QPointF q2 = poly.last();
                        q1 = q2 - q1;
                        q2 = p1 - q2;
                        q1 = q1 / CalcVector2D::length(q1);
                        q2 = q2 / CalcVector2D::length(q2);
                        if ( CalcVector2D::dot(q1, q2) > 0.99985 ) {
                            poly.replace(poly.count() - 1, cpos);
                        } else {
                            poly.append(cpos);
                        }
                        hlinePoints.replace(hlinePoints.count() - 1, poly);

                    } else {

                        QList<QPointF> poly;
                        QPointF c1pos, c2pos;
                        if ( isIc && !isJc ) {
                            c2pos = p1;
                            c1pos = jointToPoint(jd0, c2pos.x());
                        } else if ( !isIc && isJc ) {
                            c1pos = p0;
                            c2pos = jointToPoint(jd1, c1pos.x());
                        } else {
                            c1pos = p0;
                            c2pos = p1;
                        }
                        poly.append( c1pos );
                        poly.append( c2pos );
                        hlinePoints.append(poly);

                    }
                }
            }



            if ( hlinePoints.isEmpty() ) continue;

            for ( int i = 0; i < hlinePoints.count(); i++ ) {
                createSpringItem(scene, spring, hlinePoints.at(i));
            }

        }
    }

}
void FigureTemplate::createSpringItem(OutputGrScene *scene, SpringData* sd,QList<QPointF> points )
{
    OutputGraphicsItem *item = nullptr;

    if ( item == nullptr ) {
        item = new OutputSpringItem(this, DATATYPE::TPSPRING,STRUCTTYPE::NOSTRUCTTYPE, sd->getUuid(), points);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);
    scene->addItem(item);
}

QString FigureTemplate::getMemberInfo(const QUuid &id)const
{
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();
    UnifiedOutput* uo = UnifiedFixedData::getInstance()->getPstnData();


    //i,jの座標と節点番号、自分の部材番号、長さ、所属通り、断面名、断面リストのデータ
    QString str;
    MemberData* md=uad->memberIDToPointer(id);
    QList<JointData*> jlist = md->getJointList();
    str += u8"部材番号　　　　：　" + QString::number(md->getMemberNo())+ u8" \n";
    str += u8"部材長さ　　　　：　" + QString::number(md->getTotalLength())+ u8" \n";
    str += u8"コードアングル　：　" + QString::number(md->codeAngle())+ u8" \n";
    str += u8"断面名　　　　　：　" + md->sectionName()+ u8" \n";
    //    str += u8"節点リスト　　　：　\n" ;
    for (int nj=0;nj<jlist.count();nj++){
        JointData* jd = jlist.at(nj);
        str += u8"節点番号　　　　：　" + QString::number(uo->uuidToB3dIndex(jd->getUuid()))+ u8" \n";
        str += u8"座標　　　　　　：　( ";
        str += QString::number(jd->xPos(),'f',3).rightJustified(15,' ') + u8" , ";
        str += QString::number(jd->yPos(),'f',3).rightJustified(15,' ') + u8" , ";
        str += QString::number(jd->zPos(),'f',3).rightJustified(15,' ') + u8" ) \n";

    }

    return str;
}
QString FigureTemplate::getJointInfo(const QUuid &id)const
{
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();
    UnifiedOutput* uo = UnifiedFixedData::getInstance()->getPstnData();
    QString str;

    JointData* jd=uad->jointIDToPointer(id);
    QList<QUuid> fllist = jd->getAttachedFloorList();
    QList<QUuid> frlist = jd->getAttachedFrameList();
    QVector<int> boundary = jd->getBoundary();
    int fix = 0;
    if (boundary.at(0) == IFFIX)fix += 100000;
    if (boundary.at(1) == IFFIX)fix += 10000;
    if (boundary.at(2) == IFFIX)fix += 1000;
    if (boundary.at(3) == IFFIX)fix += 100;
    if (boundary.at(4) == IFFIX)fix += 10;
    if (boundary.at(5) == IFFIX)fix += 1;

    str += u8"節点番号　　　　：　" + QString::number(uo->uuidToB3dIndex(jd->getUuid()))+ u8" \n";
    str += u8"境界条件　　　　：　";
    str += QString::number(fix).rightJustified(6,'0');
    str += u8"\n";
    str += u8"座標　　　　　　：　(";
    str += QString::number(jd->xPos(),'f',3).rightJustified(15,' ') + u8" , ";
    str += QString::number(jd->yPos(),'f',3).rightJustified(15,' ') + u8" , ";
    str += QString::number(jd->zPos(),'f',3).rightJustified(15,' ') + u8" ) \n";
    str += u8"所属階　　　　　：　";
    for(int i=0;i<fllist.count();i++){str += uad->idToName(fllist.at(i),DATATYPE::TPFLOOR) + u8"  ";}
    str += u8"\n";
    str += u8"所属通り　　　　：　";
    for(int i=0;i<frlist.count();i++){str += uad->idToName(frlist.at(i),DATATYPE::TPFRAMEP) + u8"  ";}
    str += u8"\n";

    return str;

}
QString FigureTemplate::getSpringInfo(const QUuid &id)const
{
    return QString();
}

FigureCommonTemplate::FigureCommonTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& property, QUuid id)
    : FigureTemplate(_setting, property, id)
{
}





QColor FigureCommonTemplate::valueTextColor(DATATYPE dataType,ATTENTION_STATUS status) const
{
    /*
     * 応力図などでは以下のような値を返すようにオーバーライド
    UnifiedSettings::getInstance()->getOutput2DFStressSettings().columnColor;
    UnifiedSettings::getInstance()->getOutput2DFStressSettings().columnVisible;
    */

    Q_UNUSED(dataType);
    switch(status){
    case ATTENTION_STATUS::STATUS_NORMAL:
        return getViewSettings()->getValueColor();
    case ATTENTION_STATUS::STATUS_WARNING:
        return Qt::darkYellow;
    case ATTENTION_STATUS::STATUS_FATAL:
        return Qt::red;
    default:
        return getViewSettings()->getValueColor();
    }

}
QColor FigureCommonTemplate::figureColor(DATATYPE dataType,STRUCTTYPE material,ATTENTION_STATUS status) const
{
    Q_UNUSED(status);
    if ( dataType == DATATYPE::TPJOINT ) {
        return getViewSettings()->getJointColor();
    } else if ( dataType == DATATYPE::TPSLAB || dataType == DATATYPE::TPWALL  ) {
        if(material == STRUCTTYPE::STP_S){
            return getViewSettings()->getPlaneSColor();
        }else {
            return getViewSettings()->getLineRcColor();
        }
    } else if ( dataType == DATATYPE::TPDAMPER ) {
        return getViewSettings()->getLineDamperColor();
    } else if ( dataType == DATATYPE::TPISO ) {
        return getViewSettings()->getLineIsoColor();
    }else if(dataType == DATATYPE::TPSPRING){
        if(status == ATTENTION_STATUS::STATUS_WARNING){
            return getViewSettings()->getLineIsoColor();
        }else{
            return getViewSettings()->getSupportColor();
        }
    } else {
        if(material == STRUCTTYPE::STP_RC){
            return getViewSettings()->getLineRcColor();
        }else if(material == STRUCTTYPE::STP_S){
            return getViewSettings()->getLineSColor();
        }else if(material == STRUCTTYPE::STP_SRC){
            return getViewSettings()->getLineSrcColor();
        }else if(material == STRUCTTYPE::STP_CFT){
            return getViewSettings()->getLineCftColor();
        }
    }
    return QColor{};
}
QColor FigureCommonTemplate::selectColor(DATATYPE) const
{
    return getViewSettings()->getSelectedColor();
}
QColor FigureCommonTemplate::supportColor() const
{
    return getViewSettings()->getSupportColor();
}
//QColor FigureTemplate::springColor()
//{
////    return Qt::red;
//    return getViewSettings()->supportColor;
//}
int FigureCommonTemplate::jointSize() const
{
    return getViewSettings()->getJointSize();
}
int FigureCommonTemplate::noteSize() const
{
    return getViewSettings()->getNoteTextSize();
}
QColor FigureCommonTemplate::noteColor() const
{
    return getViewSettings()->getNoteColor();
}
int FigureCommonTemplate::plasticSymbolSize() const
{
    return getViewSettings()->getPlasticSymbolSize();
}
QColor FigureCommonTemplate::plasticSymbolColor() const
{
    return getViewSettings()->getPlasticSymbolColor();
}

QColor FigureCommonTemplate::scaleColor() const
{
    return getViewSettings()->getScaleColor();
}
int FigureCommonTemplate::scaleTextSize() const
{
    return getViewSettings()->getScaleSize();
}
bool FigureCommonTemplate::scaleFloorTextVisible() const
{
    return getViewSettings()->getFloorStringVisible();
}
bool FigureCommonTemplate::scaleGridTextVisible() const
{
    return getViewSettings()->getGridStringVisible();
}

bool FigureCommonTemplate::isLineDirVisible() const
{
    return false;
}
int FigureCommonTemplate::nameTextSize(DATATYPE dataType) const
{
    Q_UNUSED(dataType);
    //符号サイズはモデル図でオーバーライド
    return 0;
}
int FigureCommonTemplate::valueTextSize(DATATYPE) const
{
        return getViewSettings()->getValueTextSize();
}
bool FigureCommonTemplate::nameVisible(DATATYPE dataType) const
{
    Q_UNUSED(dataType);
    //モデル図でオーバーライド
    return false;
}
bool FigureCommonTemplate::figureVisible(DATATYPE dataType) const
{
    Q_UNUSED(dataType);
    //応力図でオーバーライド
    return true;
}

bool FigureCommonTemplate::valueVisible(DATATYPE dataType) const
{
    Q_UNUSED(dataType);
    //応力図でオーバーライド
    return true;
}

ATTENTION_STATUS  FigureCommonTemplate::getAttentionStatus(qreal value) const
{
    //valueを評価して通常・注意・危険などのステータスを返す。とりあえずqreal一つだが、おそらくもっと複雑な評価が必要
    Q_UNUSED(value);
    return ATTENTION_STATUS::STATUS_NORMAL;
}


void FigureCommonTemplate::createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)
{
    Q_UNUSED(ino);
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();

    VEC3D v1,v2,v3;
    if(typeid(*scene)==typeid(ElevationOutputGrScene)){
        qreal fr_angle = uad->globalXYZToFrameAngle(uuid, jd->xyz());
        v1=VEC3D(qCos(fr_angle), qSin(fr_angle), 0.0);
        v2=VEC3D(0.0, 0.0, 1.0);
        v3 = CalcVector3D::crossNormalized(v1, v2);
    }else if(typeid(*scene)==typeid(PlanOutputGrScene)){
        v1 = VEC3D(1.0, 0.0, 0.0);
        v3 = VEC3D(0.0, 0.0, 1.0);
    }else{
        qDebug()<<"unknown scene type";
        v1 = VEC3D(1.0, 0.0, 0.0);
        v3 = VEC3D(0.0, 0.0, 1.0);
    }


    OutputGraphicsItem *item = new OutputJointItem(this, DATATYPE::TPJOINT,STRUCTTYPE::NOSTRUCTTYPE, jd->getUuid(),v3,v1);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);
    scene->addItem(item);

    FrameType::FRAMETYPE f_type = UnifiedFixedData::getInstance()->getAnalysisData()->getFrameTypeOfFloor(uuid);
    bool isClosedFrame = ( f_type == FrameType::FRAMETYPE::CLCIRCLE || f_type == FrameType::FRAMETYPE::CLVARIABLE ) ;

    if ( isClosedFrame && qAbs(pp.x()) < 1.0e-3 ) { // 閉じた通り
        qreal cx = UnifiedFixedData::getInstance()->getAnalysisData()->globalZToClosedFrameEndX(uuid, jd->zPos());

        OutputGraphicsItem *item = new OutputJointItem(this, DATATYPE::TPJOINT,STRUCTTYPE::NOSTRUCTTYPE, jd->getUuid(),v3,v1);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setPos( QPointF(cx * 1000.0, pp.y()));
        item->setZValue(-100.0);
        item->setEnabled(true);
        item->setSelected(false);
        scene->addItem(item);
    }
}

void FigureCommonTemplate::createLineItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points )
{
    OutputGraphicsItem *item = nullptr;
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();


    if ( item == nullptr ) {
        item = new OutputLineItem(this, md->dataType(), md->structureType(*uad),md->getUuid(), points);
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}
void FigureCommonTemplate::createLineSectionItem(OutputGrScene *scene, MemberData* md, const QPointF &point )
{
    Q_UNUSED(scene);
    Q_UNUSED(md);
    Q_UNUSED(point);
    //map系のとき

}
void FigureCommonTemplate::createPlaneItem(OutputGrScene *scene, MemberData* md,QList<QPointF> points,bool isNormalDirection )
{
    qDebug()<<"FigureTemplate::createPlaneItem";

    OutputGraphicsItem *item;
    UnifiedAnalysisData* uad = UnifiedFixedData::getInstance()->getAnalysisData();

    if(md->getPlaneModel()==PLANEMODEL::ZWALL_MODEL)return;
    if ( !isNormalDirection ) {
        item = new OutputPlaneItem(this,md->dataType(), md->structureType(*uad),md->getUuid(),points);
        item->setDrawingStatusDirect();
    } else {
        item = new OutputLinePlaneItem(this,md->dataType(),md->structureType(*uad), md->getUuid(), points);
    }

    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setPos(points.first());
    item->setZValue(-300.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
}


} // namespace post3dapp
