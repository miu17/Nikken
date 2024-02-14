#include "planCL_grscene.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "calc_vector3d.h"
#include "calc_vector2d.h"
#include "calcload_background_floor.h"
#include "calcloaditem_jload.h"
#include "calcloaditem_mload.h"
#include "calcloaditem_plane.h"
#include "calcloaditem_lplane.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_calcload.h"
#include "unified_inputdata.h"
#include "unified_settings.h"



namespace post3dapp{
PlanCalcLoadGrScene::PlanCalcLoadGrScene(const CalculationLoad2DViewSettings& setting,
                                         const FIGURE_TYPE_PROPERTY& figureTypeProperty,
                                         bool _syncSetting,
                                         QObject *parent )
    : CalcLoadGrScene( setting, figureTypeProperty, _syncSetting, parent )
{
    backGround = new CalcLoadBackGroundFloor(setting.UseScaledFont(), UnifiedFixedData::getInstance()->getInputData());
    backGround->setColor(setting.getScaleColor());
    backGround->setSpanColorMode(false);
    backGround->setDrawCrossMode(true);

    myFloorID = QUuid();
    connect( UnifiedFixedData::getInstance()->getCalcLoadData(), &UnifiedCalcLoad::dataUpdated,
             this, &PlanCalcLoadGrScene::resetItems );
}

PlanCalcLoadGrScene::~PlanCalcLoadGrScene()
{
    clearItems();
    delete backGround;
}

void PlanCalcLoadGrScene::setStatus(const QUuid &uid )
{
    myFloorID = uid;
    backGround->resetFramePoints(myFloorID);
    backGround->setColor(getViewSettings().getScaleColor());
    figureProperty.name = UnifiedFixedData::getInstance()->getInputData()->idToName(uid, DATATYPE::TPFLOOR);
    this->resetItems();
}

void PlanCalcLoadGrScene::createJointItems()
{

    Q_FOREACH (JointData *jd, UnifiedFixedData::getInstance()->getInputData()->jointListOfFloor(myFloorID) ) {

        if ( !jd->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()) ) continue;

        CalcLoadGraphicsItem *item = makeJointItem(jd);
        this->addItem(item);
        createdItems.append(item);

    }



}

void PlanCalcLoadGrScene::createMemberItems()
{
    Q_FOREACH ( MemberData *md, UnifiedFixedData::getInstance()->getInputData()->memberListOfFloor(myFloorID) ) {

        if ( !md->isAnalysisMember(*UnifiedFixedData::getInstance()->getInputData()) ) continue;

        QList<JointData *> jdlist = md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData());


        // 線材の場合
        if ( md->isLineMember(false) ) {

            QList<QList<QPointF> > hlinePoints;

            bool prev_on;
            bool cur_on = false;
            bool next_on = jdlist.first()->containsFloor(myFloorID);

            for (int i = 0; i < jdlist.count() - 1; i++) {

                JointData *jd0 = jdlist.at(i);
                JointData *jd1 = jdlist.at(i + 1);

                QPointF p0 = QPointF(jd0->xPos() * 1000., jd0->yPos() * 1000.);
                QPointF p1 = QPointF(jd1->xPos() * 1000., jd1->yPos() * 1000.);

                prev_on = cur_on;
                cur_on = next_on;
                next_on = jd1->containsFloor(myFloorID);

                if ( cur_on ) {
                    if ( next_on ) {
                        if ( prev_on ) {
                            QList<QPointF> poly = hlinePoints.last();
                            QPointF q1 = poly.at( poly.count() - 2 );
                            QPointF q2 = poly.last();
                            q1 = q2 - q1;
                            q2 = p1 - q2;
                            q1 = q1 / CalcVector2D::length(q1);
                            q2 = q2 / CalcVector2D::length(q2);
                            if ( CalcVector2D::dot(q1, q2) > 0.99985 ) {
                                poly.replace(poly.count() - 1, p1);
                            } else {
                                poly.append( p1 );
                            }
                            hlinePoints.replace(hlinePoints.count() - 1, poly);
                        } else {
                            QList<QPointF> poly;
                            poly.append( p0 );
                            poly.append( p1 );
                            hlinePoints.append(poly);
                        }
                    }
                }
            }

            if ( hlinePoints.isEmpty() ) continue;

            for ( int i = 0; i < hlinePoints.count(); i++ ) {
                CalcLoadGraphicsItem *item = makeLineItem( md, hlinePoints.at(i) );
                this->addItem(item);
                createdItems.append(item);
            }

        }


        // 面材の場合
        if ( md->isClosedMember(false) ) {

            bool isOnPlane = false;
            XYZ a1 = jdlist.first()->xyz();
            VEC3D vv(0.0, 0.0, 0.0);
            for ( int i = 1; i < jdlist.count() - 1; i++ ) {
                XYZ a2 = jdlist.at(i)->xyz();
                XYZ a3 = jdlist.at(i + 1)->xyz();
                vv = CalcVector3D::cross( VEC3D(a2.x - a1.x, a2.y - a1.y, a2.z - a1.z),
                                          VEC3D(a3.x - a1.x, a3.y - a1.y, a3.z - a1.z) );
                if ( CalcVector3D::norm(vv) > 1.0e-3 ) {
                    isOnPlane = true;
                    break;
                }
            }

            QList<QList<QPointF> > planePoints;
            QList<int> planeIndexes;
            bool prev_on;
            bool cur_on = false;

            for (int i = 0; i < jdlist.count(); i++) {

                JointData *jd = jdlist.at(i);
                prev_on = cur_on;
                cur_on = jd->containsFloor(myFloorID);

                if ( cur_on ) {
                    if ( prev_on ) {
                        QList<QPointF> poly = planePoints.last();
                        poly.append( QPointF(jd->xPos() * 1000., jd->yPos() * 1000.) );
                        planePoints.replace(planePoints.count() - 1, poly);
                    } else {
                        QList<QPointF> poly;
                        poly.append( QPointF(jd->xPos() * 1000., jd->yPos() * 1000.) );
                        planePoints.append(poly);
                        planeIndexes.append(i);
                    }
                }

                if ( !isOnPlane || i < 3 ) continue;
                XYZ a4 = jd->xyz();
                qreal dot = CalcVector3D::dot(VEC3D(a4.x - a1.x, a4.y - a1.y, a4.z - a1.z), vv);
                isOnPlane = ( qAbs(dot) < 1.0e-3 ) ;
            }

            // 始点と終点がONで,Polygonが2つ以上存在する場合,最初と最後のPolygonを結合する
            if ( cur_on && planePoints.count() > 1 ) {
                if ( jdlist.first()->containsFloor(myFloorID) ) {
                    QList<QPointF> poly = planePoints.last();
                    poly.append( planePoints.first() );
                    planePoints.replace(planePoints.count() - 1, poly);
                    planePoints.removeFirst();
                    planeIndexes.removeFirst();
                }
            }

            QList<QList<QPointF> > hplanePoints, vplanePoints;
            QList<QPointF> crossPoints;

            for ( int i = 0; i < planePoints.count(); i++ ) {

                QList<QPointF> points = planePoints.at(i);

                if ( points.count() == 1 ) {
                    int insert_idx = 0;
                    for ( int j = 0; j < crossPoints.count(); j++ ) {
                        if ( CalcVector2D::length(points.first()) >
                                CalcVector2D::length(crossPoints.at(j)) ) insert_idx = j + 1;
                    }
                    crossPoints.insert( insert_idx, points.first() );
                    continue;
                }

                bool isOnLine = true;
                if ( points.count() > 2 ) {
                    for ( int j = 0; j < points.count() - 2; j++ ) {
                        if ( CalcVector2D::sideToSegment(points.at(j), points.at(j + 1), points.at(j + 2)) != 0 ) {
                            isOnLine = false;
                            break;
                        }
                    }
                }

                if ( isOnLine ) {

                    int s_idx = planeIndexes.at(i);
                    int e_idx = s_idx + points.count() - 1;
                    if ( e_idx > jdlist.count() - 1 ) e_idx = e_idx - jdlist.count();
                    JointData *pj = md->previousJoint(jdlist.at(s_idx));
                    JointData *nj = md->nextJoint(jdlist.at(e_idx));
                    qreal cur_h = jdlist.at(s_idx)->zPos();
                    qreal pre_h = pj->zPos() - cur_h;
                    qreal nex_h = nj->zPos() - cur_h;

                    if ( pre_h * nex_h < 0.0 ) {
                        int insert_idx = 0;
                        for ( int j = 0; j < crossPoints.count(); j++ ) {
                            if ( CalcVector2D::length(points.first()) > CalcVector2D::length(crossPoints.at(
                                                                                                 j)) ) insert_idx = i;
                        }

                        if ( pre_h > 0.0 ) {
                            crossPoints.insert( insert_idx, points.first() );
                        } else {
                            crossPoints.insert( insert_idx, points.last() );
                        }
                    } else if ( pre_h > -1.0e-5 && nex_h > -1.0e-5 ) {
                        vplanePoints.append( points );
                    }

                } else {
                    hplanePoints.append(points);
                }

            }

            QList<QPointF> p_crossPoints = crossPoints;

            // 交差点　奇数の場合は無視
            if ( crossPoints.count() > 1 && crossPoints.count() % 2 == 0 ) {

                for ( int i = vplanePoints.count() - 1; i >= 0 ; i-- ) {

                    QList<QPointF> points = vplanePoints.at(i);
                    qreal xx = CalcVector2D::length(points.first());

                    int idx = 0;
                    for ( int j = 0; j < crossPoints.count(); j++ ) {
                        if ( xx > CalcVector2D::length(crossPoints.at(j)) ) idx = j + 1;
                    }

                    if ( idx > 0 && idx < crossPoints.count() && idx % 2 == 1 ) {
                        int insert_idx = 0;
                        for ( int j = 0; j < p_crossPoints.count(); j++ ) {
                            if ( xx > CalcVector2D::length(p_crossPoints.at(j)) ) insert_idx = j + 1;
                        }

                        if ( CalcVector2D::length(points.first()) < CalcVector2D::length(points.last()) ) {
                            p_crossPoints.insert( insert_idx, points.last() );
                            p_crossPoints.insert( insert_idx, points.first() );
                        } else {
                            p_crossPoints.insert( insert_idx, points.first() );
                            p_crossPoints.insert( insert_idx, points.last() );
                        }
                        vplanePoints.removeAt(i);
                    }

                }
            }

            for ( int i = 0; i < p_crossPoints.count(); i++ ) {
                if ( i % 2 == 1 ) {
                    QList<QPointF> plist;
                    plist.append(p_crossPoints.at(i - 1));
                    plist.append(p_crossPoints.at(i));
                    vplanePoints.append( plist );
                }
            }

            for ( int i = 0; i < hplanePoints.count(); i++ ) {
                CalcLoadGraphicsItem *item = makePlaneItem( md, hplanePoints.at(i), true );
                this->addItem(item);
                createdItems.append(item);
            }

            for ( int i = 0; i < vplanePoints.count(); i++ ) {
                CalcLoadGraphicsItem *item = makePlaneItem( md, vplanePoints.at(i), false );
                this->addItem(item);
                createdItems.append(item);
            }

        }

    }

}


CalcLoadGraphicsItem *PlanCalcLoadGrScene::makeJointItem(JointData *jd)
{
    int ino = UnifiedFixedData::getInstance()->getInputData()->indexOfJoint(jd)+1;
    CalcLoadGraphicsItem* item;
    if (!UnifiedFixedData::getInstance()->getCalcLoadData()->isLoadedJoint(jd)){
        item = new CalcLoadJointItem(DATATYPE::TPJOINT, jd->getUuid(), getViewSettings(), ino);
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    }else{
        item = new CalcLoadJointLoadItem(DATATYPE::TPJOINT, jd->getUuid(), getViewSettings(), figureProperty, ino, VEC3D(0.0, 0.0, 1.0),
                                                                UnifiedFixedData::getInstance()->getCalcLoadData()->loadOfJoint(jd) );
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    item->setPos(QPointF(jd->xPos() * 1000., jd->yPos() * 1000.));
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);
    return item;
}

CalcLoadGraphicsItem *PlanCalcLoadGrScene::makeLineItem(MemberData *md,
                                                        const QList<QPointF> &points,
                                                        bool isValue)
{
    qDebug()<<"makeCL_LineItem";
    CalcLoadGraphicsItem *item;
    if (!UnifiedFixedData::getInstance()->getCalcLoadData()->isLoadedMember(md)){
        item = new CalcLoadLineItem(md->dataType(), md->getUuid(), getViewSettings(), points);
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    }else{
        VEC3D y_v = md->getVVector();
        VEC3D z_v = md->getWVector();
        qreal yt1 = CalcVector3D::dot(y_v, VEC3D(0.0, 0.0, 1.0));
        qreal zt1 = CalcVector3D::dot(z_v, VEC3D(0.0, 0.0, 1.0));
        bool isZin = ( qAbs(yt1) > qAbs(zt1) ); // 部材z軸を面内とする
        item = new CalcLoadMemberLoadItem(md->dataType(), md->getUuid(),
                                          getViewSettings(),
                                          getFigureTypeProperty(),
                                          points,
                                          md->getTotalLength(),
                                          isZin,
                                          UnifiedFixedData::getInstance()->getCalcLoadData()->loadOfMember(md) );
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }
    item->setPos(points.first());
    item->setZValue(-200.0);
    item->setEnabled(true);
    item->setSelected(false);
    return item;
}

/*
FIGURE_TYPE_PROPERTY PlanCalcLoadGrScene::getFigureTypeProperty() const{
    FIGURE_TYPE_PROPERTY type;
    type.type = OUTFIGTYPE::LOAD;
    QUuid uuid = this->floorID();
    type.viewtype=MODELVIEWTYPE::CALC2DMODEL;
    type.scenetype=SCENETYPE::PLANSCENE;
    type.name = UnifiedFixedData::getInstance()->getInputData()->idToName(uuid, DATATYPE::TPFLOOR);
    type.filename=UnifiedFixedData::getInstance()->getInputData()->fileName();
    type.title=type.name + u8"階 荷重拾い図";
    return type;
}
*/

} // namespace post3dapp
