#include "elevationCL_grscene.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "calc_vector2d.h"
#include "calc_vector3d.h"
#include "calcload_background_frame.h"
#include "calcloaditem_jload.h"
#include "calcloaditem_mload.h"
#include "calcloaditem_plane.h"
#include "calcloaditem_lplane.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "manage_springdata.h"
#include "unified_calcload.h"
#include "unified_inputdata.h"
#include "unified_settings.h"

namespace post3dapp{
ElevationCalcLoadGrScene::ElevationCalcLoadGrScene(const CalculationLoad2DViewSettings& setting,
                                                   const FIGURE_TYPE_PROPERTY &figureTypeProperty,
                                                   bool _syncSetting,
                                                   QObject *parent )
    : CalcLoadGrScene( setting, figureTypeProperty, _syncSetting, parent )
{
    backGround = new CalcLoadBackGroundFrame(setting.UseScaledFont(), UnifiedFixedData::getInstance()->getInputData());
    backGround->setColor(setting.getScaleColor());

    myFrameID = QUuid();
    connect( UnifiedFixedData::getInstance()->getCalcLoadData(), &UnifiedCalcLoad::dataUpdated,
             this, &ElevationCalcLoadGrScene::resetItems );
}

ElevationCalcLoadGrScene::~ElevationCalcLoadGrScene()
{
    clearItems();
    delete backGround;
}

void ElevationCalcLoadGrScene::setStatus(const QUuid &uid)
{
    myFrameID = uid;
    backGround->resetFramePoints(myFrameID);
    backGround->setColor(viewSettings.getScaleColor());
    figureProperty.name = UnifiedFixedData::getInstance()->getInputData()->idToName(uid, DATATYPE::TPFRAMEP);
    this->resetItems();
}

void ElevationCalcLoadGrScene::createJointItems()
{
    FrameType::FRAMETYPE f_type = UnifiedFixedData::getInstance()->getInputData()->getFrameTypeOfFloor(myFrameID);
    bool isClosedFrame = ( f_type == FrameType::FRAMETYPE::CLCIRCLE || f_type == FrameType::FRAMETYPE::CLVARIABLE ) ;

    Q_FOREACH ( JointData *jd, UnifiedFixedData::getInstance()->getInputData()->jointListOfFrame(myFrameID) ) {

        if ( !jd->isAnalysisJoint(*UnifiedFixedData::getInstance()->getInputData()) ) continue;

        int ino = UnifiedFixedData::getInstance()->getInputData()->indexOfJoint(jd) + 1;
        QPointF pp = jointToPoint(jd);
        CalcLoadGraphicsItem *item = makeJointItem(jd, ino, pp);
        this->addItem(item);
        createdItems.append(item);

        if ( isClosedFrame && qAbs(pp.x()) < 1.0e-3 ) { // 閉じた通り
            qreal cx = UnifiedFixedData::getInstance()->getInputData()->globalZToClosedFrameEndX(myFrameID, jd->zPos());
            item = makeOffJointItem(jd, ino, QPointF(cx * 1000.0, pp.y()));
            this->addItem(item);
            createdItems.append(item);
        }

    }

}

void ElevationCalcLoadGrScene::createMemberItems()
{
    FrameType::FRAMETYPE f_type = UnifiedFixedData::getInstance()->getInputData()->getFrameTypeOfFloor(myFrameID);
    bool isClosedFrame = ( f_type == FrameType::FRAMETYPE::CLCIRCLE || f_type == FrameType::FRAMETYPE::CLVARIABLE ) ;

    qDebug() << "createMembers";
    Q_FOREACH ( MemberData *md, UnifiedFixedData::getInstance()->getInputData()->memberListOfFrame(myFrameID) ) {
        if ( !md->isAnalysisMember(*UnifiedFixedData::getInstance()->getInputData()) ) continue;
        QList<JointData *> jdlist = md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData());

        if ( md->isClosedMember(false) ) {
            bool isOnPlane = false;
            XYZ a1 = jdlist.first()->xyz();
            VEC3D vv(0.0, 0.0, 0.0);
            for ( int i = 1; i < jdlist.count() - 1; i++ ) {
                XYZ a2 = jdlist.at(i)->xyz();
                XYZ a3 = jdlist.at(i + 1)->xyz();
                vv = CalcVector3D::cross(VEC3D(a2.x - a1.x, a2.y - a1.y, a2.z - a1.z),
                                         VEC3D(a3.x - a1.x, a3.y - a1.y, a3.z - a1.z));
                if ( CalcVector3D::norm(vv) > 1.0e-3 ) {
                    isOnPlane = true;
                    break;
                }
            }
        }
    }

    Q_FOREACH ( MemberData *md, UnifiedFixedData::getInstance()->getInputData()->memberListOfFrame(myFrameID) ) {
        qDebug() << "createMember" << md->getMemberValues().sectionName;

        if ( !md->isAnalysisMember(*UnifiedFixedData::getInstance()->getInputData()) ) continue;

        QList<JointData *> jdlist = md->getAnalysisJointList(*UnifiedFixedData::getInstance()->getInputData());

        // 線材の場合
        if ( md->isLineMember(false) ) {

            QList<QList<QPointF> > hlinePoints;

            bool prev_on;
            bool cur_on = false;
            bool next_on = jdlist.first()->containsFrame(myFrameID);

            for (int i = 0; i < jdlist.count() - 1; i++) {

                JointData *jd0 = jdlist.at(i);
                JointData *jd1 = jdlist.at(i + 1);

                QPointF p0 = jointToPoint(jd0);
                QPointF p1 = jointToPoint(jd1);

                bool isIc = ( isClosedFrame && qAbs(p0.x()) < 1.0e-3 );
                bool isJc = ( isClosedFrame && qAbs(p1.x()) < 1.0e-3 );

                prev_on = cur_on;
                cur_on = next_on;
                next_on = jd1->containsFrame(myFrameID);

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
            }



            if ( hlinePoints.isEmpty() ) continue;

            for ( int i = 0; i < hlinePoints.count(); i++ ) {
                CalcLoadGraphicsItem *item = makeLineItem(md, hlinePoints.at(i));
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
                vv = CalcVector3D::cross(VEC3D(a2.x - a1.x, a2.y - a1.y, a2.z - a1.z),
                                         VEC3D(a3.x - a1.x, a3.y - a1.y, a3.z - a1.z));
                if ( CalcVector3D::norm(vv) > 1.0e-3 ) {
                    isOnPlane = true;
                    break;
                }
            }

            QList<QList<QPointF> > planePoints;
            QList<int> planeIndexes;
            bool prev_on;
            bool cur_on = false;


            // 閉じた通りの場合、基準となる点をサーチ
            QPointF cmpPoint;
            bool isCmpDetect = false; // 通り起点でない点が見つかった
            if ( isClosedFrame ) {
                Q_FOREACH ( JointData *jd, jdlist ) {
                    QPointF pp = jointToPoint(jd);
                    if ( jd->containsFrame(myFrameID) && qAbs(pp.x()) > 1.0e-3 ) {
                        cmpPoint = jointToPoint(jd);
                        isCmpDetect = true;
                        break;
                    }
                }
            }

            for (int i = 0; i < jdlist.count(); i++) {

                JointData *jd = jdlist.at(i);
                QPointF pp = jointToPoint(jd);
                bool isIc = ( isClosedFrame && qAbs(pp.x()) < 1.0e-3 ); // 閉じた通りの判定
                prev_on = cur_on;
                cur_on = jd->containsFrame(myFrameID);

                if ( cur_on ) {
                    if ( prev_on ) {
                        QList<QPointF> poly = planePoints.last();
                        QPointF cpos = ( isIc ) ? jointToPoint(jd, poly.last().x()) : pp;
                        poly.append(cpos);
                        planePoints.replace(planePoints.count() - 1, poly);
                    } else {
                        QList<QPointF> poly;
                        QPointF cpos = ( isIc && isCmpDetect ) ? jointToPoint(jd, cmpPoint.x()) : pp;
                        poly.append(cpos);
                        planePoints.append(poly);
                        planeIndexes.append(i);
                    }
                }

                if ( !isOnPlane || i < 3 ) continue;
                XYZ a4 = jd->xyz();
                qreal dot = CalcVector3D::dot(VEC3D(a4.x - a1.x, a4.y - a1.y, a4.z - a1.z), vv);
                isOnPlane = ( qAbs(dot) < 1.0e-3 );
            }

            // 始点と終点がONで,Polygonが2つ以上存在する場合,最初と最後のPolygonを結合する
            if ( cur_on && planePoints.count() > 1 ) {
                if ( jdlist.first()->containsFrame(myFrameID) ) {
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
                        if ( points.first().x() > crossPoints.at(j).x() ) insert_idx = j + 1;
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
                    XYZ p_xyz = UnifiedFixedData::getInstance()->getInputData()->globalXYZToFrameXYZ(myFrameID, pj->xyz());
                    XYZ n_xyz = UnifiedFixedData::getInstance()->getInputData()->globalXYZToFrameXYZ(myFrameID, nj->xyz());

                    if ( p_xyz.z * n_xyz.z < 0.0 ) {
                        int insert_idx = 0;
                        for ( int j = 0; j < crossPoints.count(); j++ ) {
                            if ( points.first().x() > crossPoints.at(j).x() ) insert_idx = i;
                        }
                        if ( p_xyz.z > 0.0 ) {
                            crossPoints.insert( insert_idx, points.first() );
                        } else {
                            crossPoints.insert( insert_idx, points.last() );
                        }
                    } else if ( p_xyz.z > 0.0 && n_xyz.z > 0.0 ) {
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
                    qreal xx = points.first().x();

                    int idx = 0;
                    for ( int j = 0; j < crossPoints.count(); j++ ) {
                        if ( xx > crossPoints.at(j).x() ) idx = j + 1;
                    }

                    if ( idx > 0 && idx < crossPoints.count() && idx % 2 == 1 ) {
                        int insert_idx = 0;
                        for ( int j = 0; j < p_crossPoints.count(); j++ ) {
                            if ( xx > p_crossPoints.at(j).x() ) insert_idx = j + 1;
                        }

                        if ( points.first().x() < points.last().x() ) {
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

            for (int i = 0; i < p_crossPoints.count(); i++) {
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


QPointF ElevationCalcLoadGrScene::jointToPoint(JointData *jd, qreal near_x) const
{
    XYZ xyz = UnifiedFixedData::getInstance()->getInputData()->globalXYZToFrameXYZ(myFrameID, jd->xyz(true));

    // 解析階の高さ補正
//    xyz.y=xyz.y+FactoryUnifiedData::getInstance(false)->getFloorValue( jd->getFirstFloor() ).adjustedLevel;

    if ( qAbs(xyz.x) < 1.0e-5 ) {
        if ( qAbs(near_x) < 1.0e-5 ) return QPointF(0.0, xyz.y * 1000.0);
        qreal cx = UnifiedFixedData::getInstance()->getInputData()->globalZToClosedFrameEndX(myFrameID, jd->zPos()) * 1000.0;
        return ( qAbs(cx - near_x) > qAbs(near_x) ) ? QPointF(0.0, xyz.y * 1000.0)
               : QPointF(cx, xyz.y * 1000.0);
    } else {
        return QPointF(xyz.x * 1000.0, xyz.y * 1000.0);
    }
}


CalcLoadGraphicsItem *ElevationCalcLoadGrScene::makeJointItem(JointData *jd, int ino, const QPointF &pp)
{
    qDebug() << "makeJointItem";
    CalcLoadGraphicsItem *item = nullptr;

    if ( UnifiedFixedData::getInstance()->getCalcLoadData()->isLoadedJoint(jd) ) {
        qreal fr_angle = UnifiedFixedData::getInstance()->getInputData()->globalXYZToFrameAngle(myFrameID, jd->xyz());
        VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
        VEC3D v2(0.0, 0.0, 1.0);
        VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
        item = new CalcLoadJointLoadItem(DATATYPE::TPJOINT, jd->getUuid(), getViewSettings(), figureProperty, ino, v3,
                                       UnifiedFixedData::getInstance()->getCalcLoadData()->loadOfJoint(jd) );
        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    }

    if ( item == nullptr ) return makeOffJointItem(jd, ino, pp);

    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);
    return item;
}

CalcLoadGraphicsItem *ElevationCalcLoadGrScene::makeOffJointItem(JointData *jd, int ino,
                                                             const QPointF &pp)
{
    CalcLoadGraphicsItem *item = new CalcLoadJointItem(DATATYPE::TPJOINT, jd->getUuid(), getViewSettings(), ino);
    item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);
    return item;
}

CalcLoadGraphicsItem *ElevationCalcLoadGrScene::makeLineItem(MemberData *md,
                                                         const QList<QPointF> &points, bool isValue )
{
    qDebug() << "makeLineItem";
    CalcLoadGraphicsItem* item;
    if (!UnifiedFixedData::getInstance()->getCalcLoadData()->isLoadedMember(md)){
        item = new CalcLoadLineItem(md->dataType(), md->getUuid(), getViewSettings(), points);
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    }else{
        qreal fr_angle = UnifiedFixedData::getInstance()->getInputData()->globalXYZToFrameAngle(myFrameID, md->iJoint()->xyz());
        VEC3D v1(qCos(fr_angle), qSin(fr_angle), 0.0);
        VEC3D v2(0.0, 0.0, 1.0);
        VEC3D v3 = CalcVector3D::crossNormalized(v1, v2);
        VEC3D y_v = md->getVVector();
        VEC3D z_v = md->getWVector();
        qreal yt1 = CalcVector3D::dot(y_v, v3);
        qreal zt1 = CalcVector3D::dot(z_v, v3);
        bool isZin = ( qAbs(yt1) > qAbs(zt1) ); //ZがQのin plane,yがQのout of plane
        item = new CalcLoadMemberLoadItem( md->dataType(),
                                                                   md->getUuid(),
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
FIGURE_TYPE_PROPERTY ElevationCalcLoadGrScene::getFigureTypeProperty() const{
    FIGURE_TYPE_PROPERTY type;
    QUuid uuid = this->frameID();
    type.type = OUTFIGTYPE::LOAD;
    type.viewtype=MODELVIEWTYPE::CALC2DMODEL;
    type.scenetype=SCENETYPE::ELEVATIONSCENE;
    type.name = UnifiedFixedData::getInstance()->getInputData()->idToName(uuid, DATATYPE::TPFRAMEP);
    type.filename=UnifiedFixedData::getInstance()->getInputData()->fileName();
    type.title=type.name + u8"通り 荷重拾い図";
    return type;
}
*/

} // namespace post3dapp
