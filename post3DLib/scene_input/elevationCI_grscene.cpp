#include "elevationCI_grscene.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "background_frame.h"
#include "calc_vector2d.h"
#include "customdraw_hlineitem.h"
#include "customdraw_hplaneitem.h"
#include "customdraw_vlineitem.h"
#include "customdraw_vplaneitem.h"
#include "define_draw_figure.h"
#include "fixed_data.h"
#include "h_beamitem.h"
#include "h_braceitem.h"
#include "h_columnitem.h"
#include "h_damperitem.h"
#include "h_girderitem.h"
#include "h_memberloaditem.h"
#include "h_isoitem.h"
#include "h_slabitem.h"
#include "h_slabloaditem.h"
#include "h_wallitem.h"
#include "jointitem.h"
#include "jointloaditem.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_inputdata.h"
#include "unified_settings.h"
#include "v_beamitem.h"
#include "v_braceitem.h"
#include "v_columnitem.h"
#include "v_damperitem.h"
#include "v_girderitem.h"
#include "v_isoitem.h"
#include "v_slabitem.h"
#include "v_wallitem.h"

namespace post3dapp{
ElevationCopyInterfaceGrScene::ElevationCopyInterfaceGrScene(LIMITSELECTTERM term, UnifiedInputData* input, const Input2DViewSettings& setting, bool _syncSetting, QObject *parent )
    : InputGrScene(term, SCENETYPE::ELEVATIONSCENE,setting, _syncSetting, parent), copy(input)
{
    backGround = new BackGroundFrame(setting.UseScaledFont(), input);
    backGround->setColor(setting.getScaleColor(),
                        setting.getScaleFocusColor());
    myFrameID = QUuid();
}

ElevationCopyInterfaceGrScene::~ElevationCopyInterfaceGrScene()
{
    delete backGround;
}

void ElevationCopyInterfaceGrScene::setStatus(const QUuid &uid )
{
    myFrameID = uid; //  FactoryUnifiedData::getInstance(isOutput)->nameToID(fr,TPFRAMEP);

    backGround->resetFramePoints(myFrameID);
    backGround->setColor(viewSettings.getScaleColor());
    this->resetItems();

}
void ElevationCopyInterfaceGrScene::setBackGround(const QString &fr, const QString &)
{
    myFrameID =  copy->nameToID(fr, DATATYPE::TPFRAMEP);
    backGround->resetFramePoints(myFrameID);
    backGround->setColor(viewSettings.getScaleColor(),
                        viewSettings.getScaleFocusColor());
    this->resetItems();
    update();
}

void ElevationCopyInterfaceGrScene::drawBackGround(QPainter *painter)
{
    qDebug() << "init drawbackground";
    backGround->drawWhole(painter);
    qDebug() << "done drawbackground";
}

QList<GlobalPoint> ElevationCopyInterfaceGrScene::getCrossPoints() const
{
    return backGround->getCrossPoints();
}

QList<GlobalPoint> ElevationCopyInterfaceGrScene::getDefinePoints() const
{
    return backGround->getDefinePoints();
}

void ElevationCopyInterfaceGrScene::setCurrentFloors(const QList<QUuid> &idlist)
{
    static_cast<BackGroundFrame*>(backGround)->setCurrentFloors(idlist);
    update();
}

void ElevationCopyInterfaceGrScene::setCurrentFrames(const QList<QUuid> &idlist)
{
    backGround->setCurrentFrames(idlist);
    update();
}

void ElevationCopyInterfaceGrScene::clearCurrentAttach()
{
    static_cast<BackGroundFrame*>(backGround)->clearCurrentFloors();
    backGround->clearCurrentFrames();
    update();
}

void ElevationCopyInterfaceGrScene::completeGlobalXYZ(GlobalPoint &gp)
{
    backGround->completeGlobalXYZ(gp);
}

void ElevationCopyInterfaceGrScene::prepareBatchInputLines(DATATYPE tp)
{
//    bool isOutput = false;
    batchInputLines.clear();

    if ( tp == DATATYPE::TPCOLUMN ) {

        QList<QUuid> fllist = copy->getFloorListOfFrame(myFrameID);
        QList<QUuid> s_fllist = copy->sortFloorOrderOfHeight(fllist);
        QList<GlobalPoint> crossPoints = backGround->getCrossPoints();
        QList<GlobalPoint> d_points;

        for ( int i = crossPoints.count() - 1 ; i >= 0 ; i-- ) {
            if ( crossPoints.at(i).containsFloor( s_fllist.first() ) ) {
                d_points.append(crossPoints.at(i));
                crossPoints.removeAt(i);
            }
        }

        for ( int i = 1; i < s_fllist.count() ; i++ ) {
            QList<GlobalPoint> u_points;
            for ( int j = crossPoints.count() - 1 ; j >= 0 ; j-- ) {
                GlobalPoint gp = crossPoints.at(j);
                if ( gp.containsFloor( s_fllist.at(i) ) ) {
                    QList<QUuid> frlist = gp.getAttachedFrameList();
                    Q_FOREACH ( GlobalPoint d_gp, d_points ) {
                        QList<QUuid> d_frlist = d_gp.getAttachedFrameList();
                        if ( frlist.count() < d_frlist.count() ) continue;
                        bool error = false;
                        Q_FOREACH (QUuid id, d_frlist) {
                            if ( !frlist.contains(id) ) {
                                error = true;
                                break;
                            }
                        }
                        if ( error ) continue;
                        batchInputLines.append(GLOBALLINE(d_gp, gp));
                    }
                    u_points.append(gp);
                    crossPoints.removeAt(j);
                }
            }
            d_points = u_points;
        }

    } else if ( tp == DATATYPE::TPGIRDER ) {

        QList<QUuid> fllist = copy->getFloorListOfFrame(myFrameID);
        QList<GlobalPoint> crossPoints = backGround->getCrossPoints();
        Q_FOREACH ( QUuid fl_id, fllist ) {
            QList<GlobalPoint> gplist;
            QList<qreal> lenlist;
            Q_FOREACH ( GlobalPoint gp, crossPoints ) {
                if ( gp.containsFloor(fl_id) ) {
                    int idx = -1;
                    for (int i = 0; i < lenlist.count(); i++) {
                        if ( gp.getElvQPointF().x() > lenlist.at(i) ) idx = i;
                    }
                    lenlist.insert(idx + 1, gp.getElvQPointF().x());
                    gplist.insert(idx + 1, gp);
                }
            }
            if ( gplist.count() < 2 ) continue;
            for ( int i = 0; i < gplist.count() - 1; i++ ) {
                batchInputLines.append(GLOBALLINE(gplist.at(i), gplist.at(i + 1)));
            }
        }

    }
}

void ElevationCopyInterfaceGrScene::prepareBatchInputPlanes(bool containsBeam)
{
    batchInputPlanes.clear();

    bool isClosed = isClosedFrame();

    QList<JointData *> sortedJoints;
    QList<JointItem *> sortedItems;
    QList<DATATYPE> typeList;

    Q_FOREACH (MemberData *md, memberAndHGirder.values()) {
        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            if ( jd->containsFrame(myFrameID) && !sortedJoints.contains(jd) ) {
                JointItem *item = jointDataToJointItem(jd);
                if ( isClosed && jointAndPoint.values().count(jd) > 1 ) {
                    JointItem *item_o = jointDataToJointItem(jd, memberAndHGirder.key(md)->pos());
                    if ( item_o != item ) continue;
                }
                int idx = 0;
                for ( int i = 0; i < sortedItems.count() ; i++ ) {
                    if ( item->x() > sortedItems.at(i)->x() ) {
                        idx++;
                    } else {
                        break;
                    }
                }
                sortedItems.insert(idx, item);
                sortedJoints.insert(idx, jd);
            }
        }
    }
    if ( !memberAndHGirder.isEmpty() ) typeList.append(DATATYPE::TPGIRDER);

    Q_FOREACH (MemberData *md, memberAndHColumn.values()) {
        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            if ( jd->containsFrame(myFrameID) && !sortedJoints.contains(jd) ) {
                JointItem *item = jointDataToJointItem(jd);
                if ( isClosed && jointAndPoint.values().count(jd) > 1 ) {
                    JointItem *item_o = jointDataToJointItem(jd, memberAndHGirder.key(md)->pos());
                    if ( item_o != item ) continue;
                }
                int idx = 0;
                for ( int i = 0; i < sortedItems.count() ; i++ ) {
                    if ( item->x() > sortedItems.at(i)->x() ) {
                        idx++;
                    } else {
                        break;
                    }
                }
                sortedItems.insert(idx, item);
                sortedJoints.insert(idx, jd);
            }
        }
    }
    if ( !memberAndHColumn.isEmpty() ) typeList.append(DATATYPE::TPCOLUMN);

    if ( containsBeam ) {
        Q_FOREACH (MemberData *md, memberAndHBeam.values()) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                if ( jd->containsFrame(myFrameID) && !sortedJoints.contains(jd) ) {
                    JointItem *item = jointDataToJointItem(jd);
                    if ( isClosed && jointAndPoint.values().count(jd) > 1 ) {
                        JointItem *item_o = jointDataToJointItem(jd, memberAndHGirder.key(md)->pos());
                        if ( item_o != item ) continue;
                    }
                    int idx = 0;
                    for ( int i = 0; i < sortedItems.count() ; i++ ) {
                        if ( item->x() > sortedItems.at(i)->x() ) {
                            idx++;
                        } else {
                            break;
                        }
                    }
                    sortedItems.insert(idx, item);
                    sortedJoints.insert(idx, jd);
                }
            }
        }
        if ( !memberAndHBeam.isEmpty() ) typeList.append(DATATYPE::TPBEAM);
    }

    QList<JointData *> remainJoints = sortedJoints;

    for ( int i = 0; i < sortedJoints.count() - 2; i++ ) {

        JointData *first_jd = sortedJoints.at(i);
        JointItem *first_item = sortedItems.at(i);

        bool isBegin = ( isClosed && qAbs(first_item->x()) < 1.0e-3 );

        QList<JointData *> secondJoints;
        Q_FOREACH (JointData *jd, first_jd->linkedJoints(typeList) ) {
            if ( remainJoints.contains(jd) ) secondJoints.append(jd);
        }

        Q_FOREACH (JointData *second_jd, secondJoints) {

            QList<JointData *> areaJoints;
            QList<JointItem *> areaItems;

            JointData *pre_jd = first_jd;
            JointItem *pre_item = first_item;
            JointData *cur_jd = second_jd;
            JointItem *cur_item = jointDataToJointItem(cur_jd);

            if ( isBegin ) {
                if ( jointDataToJointItem(pre_jd, cur_item->pos()) != pre_item ) continue;
            }

            qreal area = pre_item->x() * cur_item->y() - cur_item->x() * pre_item->y();
            areaJoints.append(cur_jd);
            areaItems.append(cur_item);

            bool normalEnd = true;
            while ( normalEnd ) {

                bool isSecondBegin = ( isClosed && qAbs(cur_item->x()) < 1.0e-3 );

                QList<JointData *> nextJoints;
                Q_FOREACH (JointData *jd, cur_jd->linkedJoints(typeList) ) {
                    if ( jd != pre_jd && !areaJoints.contains(jd) && remainJoints.contains(jd) ) {
                        if ( isSecondBegin ) {
                            QPointF pp = itemPosition(jd);
                            if ( jointDataToJointItem(cur_jd, pp) == cur_item ) nextJoints.append(jd);
                        } else {
                            nextJoints.append(jd);
                        }
                    }
                }

                JointData *next_jd = nullptr;
                JointItem *next_item = nullptr;

                if ( nextJoints.count() > 1 ) {
                    qreal min_th = 3.0 * M_PI;
                    qreal th1 = qAtan2( pre_item->y() - cur_item->y(), pre_item->x() - cur_item->x() );
                    if ( th1 < 0.0 ) th1 += 2.0 * M_PI;
                    Q_FOREACH ( JointData *jd, nextJoints ) {
                        JointItem *item = jointDataToJointItem(jd);
                        qreal th2 = qAtan2( item->y() - cur_item->y(), item->x() - cur_item->x() );
                        if ( th2 < 0.0 ) th2 += 2.0 * M_PI;
                        qreal th = th2 - th1;
                        if ( th < 0.0 ) th += 2.0 * M_PI;
                        if ( th < min_th ) {
                            min_th = th;
                            next_jd = jd;
                            next_item = item;
                        }
                    }
                } else if (nextJoints.count() == 1) {
                    next_jd = nextJoints.first();
                    next_item = jointDataToJointItem(next_jd);
                } else {
                    normalEnd = false;
                    break;
                }

                areaJoints.append(next_jd);
                areaItems.append(next_item);
                area += cur_item->x() * next_item->y() - next_item->x() * cur_item->y();

                if ( next_jd == first_jd ) {
                    if ( area >= 0.0 ) normalEnd = false;
                    break;
                }

                pre_jd = cur_jd;
                pre_item = cur_item;
                cur_jd = next_jd;
                cur_item = next_item;
            }

            if ( normalEnd ) batchInputPlanes.append( areaItems );
        }

        remainJoints.removeFirst();
    }
}

QList<MemberData *> ElevationCopyInterfaceGrScene::filterSceneMember(const QList<MemberData *> &mdlist)
const
{
    QList<MemberData *> filtered_member;
    Q_FOREACH (MemberData *md, mdlist) {
        if ( md->touchFrame(myFrameID) ) filtered_member.append(md);
    }
    return filtered_member;
}

void ElevationCopyInterfaceGrScene::resetItems()
{
    batchInputPlanes.clear();

    Q_FOREACH ( JointItem *item, jointAndPoint.keys() ) delete item;
    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHBrace.keys() ) delete item;
    Q_FOREACH ( CustomDrawVLineItem *item, memberAndVBrace.keys() ) delete item;
    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHBeam.keys() ) delete item;
    Q_FOREACH ( CustomDrawVLineItem *item, memberAndVBeam.keys() ) delete item;
    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHGirder.keys() ) delete item;
    Q_FOREACH ( CustomDrawVLineItem *item, memberAndVGirder.keys() ) delete item;
    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHColumn.keys() ) delete item;
    Q_FOREACH ( CustomDrawVLineItem *item, memberAndVColumn.keys() ) delete item;
    Q_FOREACH ( CustomDrawVLineItem *item, memberAndOffVColumn.keys() ) delete item;
    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHSlab.keys() ) delete item;
    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVSlab.keys() ) delete item;
    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHWall.keys() ) delete item;
    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVWall.keys() ) delete item;
    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHUWall.keys() ) delete item;
    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVUWall.keys() ) delete item;

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHDamper.keys() ) delete item;
    Q_FOREACH ( CustomDrawVLineItem *item, memberAndVDamper.keys() ) delete item;
    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHIsolator.keys() ) delete item;
    Q_FOREACH ( CustomDrawVLineItem *item, memberAndVIsolator.keys() ) delete item;

    Q_FOREACH ( JointLoadItem *item, memberAndJointLoad.keys() ) delete item;
    Q_FOREACH ( CustomDrawHLineItem *item, memberAndMemberLoad.keys() ) delete item;
    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndSlabLoad.keys() ) delete item;

    jointAndPoint.clear();
    memberAndHBrace.clear();
    memberAndVBrace.clear();
    memberAndHBeam.clear();
    memberAndVBeam.clear();
    memberAndHGirder.clear();
    memberAndVGirder.clear();
    memberAndHColumn.clear();
    memberAndVColumn.clear();
    memberAndOffVColumn.clear();
    memberAndHSlab.clear();
    memberAndVSlab.clear();
    memberAndHWall.clear();
    memberAndVWall.clear();
    memberAndHUWall.clear();
    memberAndVUWall.clear();

    memberAndHDamper.clear();
    memberAndVDamper.clear();
    memberAndHIsolator.clear();
    memberAndVIsolator.clear();

    memberAndJointLoad.clear();
    memberAndMemberLoad.clear();
    memberAndSlabLoad.clear();

    QList<JointData *> jdlist =  copy->jointListOfFrame(myFrameID);
    createJointItems(jdlist);
    QList<MemberData *> mdlist =  copy->memberListOfFrame(myFrameID);
    createMemberItems(mdlist);

    update();
}

QUuid ElevationCopyInterfaceGrScene::frameID() const
{
    return myFrameID;
}

void ElevationCopyInterfaceGrScene::createJointItems(const QList<JointData *> &jdlist)
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        if ( isExistedJoint(jd) ) qDebug() << "!!! ERROR !!! Existed Joint";
    }

    bool isSelectable = ( myElementType == ELJOINT &&
                          myMode == EDITMODE ) ;

    /* 全編集の場合の選択可否 */
    bool judgeLimitSelect = ( myElementType == ALLELEMENT );
    bool isTypeLimitOk = true;
    if ( judgeLimitSelect ) {
        if ( myselectTerm.isSelectLimited ) {
            if ( myselectTerm.isLimitDataType && !myselectTerm.limitedDataTypes.contains(DATATYPE::TPJOINT) ) {
                isTypeLimitOk = false;
            }
            if ( isTypeLimitOk && myselectTerm.isLimitName ) isTypeLimitOk = false;
        }
    }

    /* 閉じた通りの判定　*/
    bool isClosed = isClosedFrame();

    Q_FOREACH (JointData *jd, jdlist) {
        if ( jd->containsFrame(myFrameID) ) {

            int ino = copy->indexOfJoint(jd) + 1;
            XYZ pp = copy->globalXYZToFrameXYZ(myFrameID, jd->xyz());

            JointItem *item = new JointItem(viewSettings,ino);
            this->addItem(item);
            item->setPos(QPointF(pp.x * 1000., pp.y * 1000.));
            item->setZValue(pp.z * 1000.0);
            item->setEnabled(true);
            item->setVisible(viewSettings.isVisibleElement(ELJOINT));
            item->setSelected(false);
            item->setInteriorOn(jd->isInteriorJoint());
            if ( judgeLimitSelect ) {
                if ( isTypeLimitOk ) {
                    item->setFlag(QGraphicsItem::ItemIsSelectable, isSelectableJoint(jd, myselectTerm));
                } else {
                    item->setFlag(QGraphicsItem::ItemIsSelectable, false);
                }
            } else {
                item->setFlag(QGraphicsItem::ItemIsSelectable, isSelectable);
            }

            jointAndPoint.insert(item, jd);

            if ( isClosed && qAbs(pp.x) < 1.0e-3 ) {
                qreal cx =  copy->globalZToClosedFrameEndX(myFrameID, jd->zPos());
                JointItem *item = new JointItem(viewSettings,ino, false);
                this->addItem(item);
                item->setPos(QPointF(cx * 1000., pp.y * 1000.));
                item->setZValue(pp.z * 1000.0);
                item->setVisible(viewSettings.isVisibleElement( ELJOINT));
                item->setEnabled(false);
                item->setSelected(false);
                item->setFlag(QGraphicsItem::ItemIsSelectable, false);
                item->setInteriorOn(jd->isInteriorJoint());
                jointAndPoint.insert(item, jd);
            }

        }
    }
}

void ElevationCopyInterfaceGrScene::createMemberItems(const QList<MemberData *> &mdlist)
{
    bool isOutput = false;
    unsigned int selectFlag = 0;

    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( isExistedMember(md) ) qDebug() << "!!! ERROR !!! Existed Member";
    }

    if ( myMode == EDITMODE ) {
        if ( myElementType == ELCOLUMN ) {
            selectFlag |= (1 << COL_SELECT);
        } else if ( myElementType == ELGIRDER ) {
            selectFlag |= (1 << GIR_SELECT);
        } else if ( myElementType == ELBRACE ) {
            selectFlag |= (1 << BRA_SELECT);
        } else if ( myElementType == ELBEAM ) {
            selectFlag |= (1 << BEA_SELECT);
        } else if ( myElementType == ELSLAB ) {
            selectFlag |= (1 << SLA_SELECT);
        } else if ( myElementType == ELWALL ) {
            selectFlag |= (1 << WAL_SELECT);
        } else if ( myElementType == ELUWALL ) {
            selectFlag |= (1 << UWA_SELECT);
        } else if ( myElementType == ELDAMPER ) {
            selectFlag |= (1 << DAM_SELECT);
        } else if ( myElementType == ELISO ) {
            selectFlag |= (1 << ISO_SELECT);
        } else if ( myElementType == ELJOINTLOAD ) {
            selectFlag |= (1 << JLD_SELECT);
        } else if ( myElementType == ELMEMBERLOAD ) {
            selectFlag |= (1 << MLD_SELECT);
        } else if ( myElementType == ELSLABLOAD ) {
            selectFlag |= (1 << SLD_SELECT);
        }
    }

    unsigned int visibleFlag = 0;

    if ( viewSettings.isVisibleElement(ELCOLUMN) ) visibleFlag |= (1 << COL_VISIBLE);
    if ( viewSettings.isVisibleElement(ELGIRDER) ) visibleFlag |= (1 << GIR_VISIBLE);
    if ( viewSettings.isVisibleElement(ELBRACE) ) visibleFlag |= (1 << BRA_VISIBLE);
    if ( viewSettings.isVisibleElement(ELBEAM) ) visibleFlag |= (1 << BEA_VISIBLE);
    if ( viewSettings.isVisibleElement(ELSLAB) ) visibleFlag |= (1 << SLA_VISIBLE);
    if ( viewSettings.isVisibleElement(ELWALL) ) visibleFlag |= (1 << WAL_VISIBLE);
    if ( viewSettings.isVisibleElement(ELUWALL) ) visibleFlag |= (1 << UWA_VISIBLE);
    if ( viewSettings.isVisibleElement(ELDAMPER) ) visibleFlag |= (1 << DAM_VISIBLE);
    if ( viewSettings.isVisibleElement(ELISO) ) visibleFlag |= (1 << ISO_VISIBLE);
    if ( viewSettings.isVisibleElement(ELJOINTLOAD) ) visibleFlag |= (1 << JLD_VISIBLE);
    if ( viewSettings.isVisibleElement(ELMEMBERLOAD) ) visibleFlag |= (1 << MLD_VISIBLE);
    if ( viewSettings.isVisibleElement(ELSLABLOAD) ) visibleFlag |= (1 << SLD_VISIBLE);

    Q_FOREACH (MemberData *md, mdlist) {

        QList<JointData *> jdlist = md->getJointList();

        // 点材の場合
        if ( md->isJointMember() ) {

            JointData *jd = jdlist.first();
            if ( jd->containsFrame(myFrameID) && jointAndPoint.values().contains(jd) ) {
                JointLoadItem *item = new JointLoadItem(viewSettings,md->sectionName());
                this->addItem(item);
                JointItem *j_item = jointDataToJointItem(jd, true);
                item->setPos(j_item->pos());
                item->setZValue(j_item->zValue() + 50.0);
                item->setEnabled(true);
                item->setFlag(QGraphicsItem::ItemIsSelectable, ( selectFlag & (1 << JLD_SELECT) ));
                item->setVisible( visibleFlag & (1 << JLD_VISIBLE) );
                item->setSelected(false);
                memberAndJointLoad.insert(item, md);
            }
            continue;

        }

        // 線材の場合
        if ( md->isLineMember() ) {

            QList<QList<QPointF> > hlinePoints;
            QList<int> hl_index, up_index, dn_index;

            qreal fr_angle = 0.0;
            qreal shift_x = 0.0;
            if ( md->dataType() == DATATYPE::TPCOLUMN ) {
                XYZ xyz = jdlist.first()->xyz();
                fr_angle = copy->globalXYZToFrameAngle(myFrameID, xyz);
                qreal shy = md->getShiftY();
                qreal shz = md->getShiftZ();
                if ( qAbs(shy) > 1.0e-3 || qAbs(shz) > 1.0e-3 ) {
                    qreal x1 = copy->globalXYZToFrameXYZ(myFrameID, xyz).x;
                    xyz.x = xyz.x + shy / 1000.0;
                    xyz.y = xyz.y + shz / 1000.0;
                    qreal x2 = copy->globalXYZToFrameXYZ(myFrameID, xyz).x;
                    shift_x = (x2 - x1) * 1000.0;
                }
            }

            bool prev_on;
            bool cur_on = false;
            bool next_on = jdlist.first()->containsFrame(myFrameID);

            for (int i = 0; i < jdlist.count() - 1; i++) {

                JointData *jd0 = jdlist.at(i);
                JointData *jd1 = jdlist.at(i + 1);

                bool isIc = ( jointAndPoint.values().count(jd0) > 1 );
                bool isJc = ( jointAndPoint.values().count(jd1) > 1 );

                prev_on = cur_on;
                cur_on = next_on;
                next_on = jd1->containsFrame(myFrameID);

                if ( cur_on ) {
                    if ( next_on ) {
                        if ( prev_on ) {
                            QList<QPointF> poly = hlinePoints.last();
                            QPointF cpos = ( isJc ) ? itemPosition(jd1, poly.last()) : itemPosition(jd1);
                            poly.append( cpos );
                            hlinePoints.replace(hlinePoints.count() - 1, poly);
                        } else {
                            QList<QPointF> poly;
                            QPointF c1pos, c2pos;
                            if ( isIc && !isJc ) {
                                c2pos = itemPosition(jd1);
                                c1pos = itemPosition(jd0, c2pos);
                            } else if ( !isIc && isJc ) {
                                c1pos = itemPosition(jd0);
                                c2pos = itemPosition(jd1, c1pos);
                            } else {
                                c1pos = itemPosition(jd0);
                                c2pos = itemPosition(jd1);
                            }
                            poly.append( c1pos );
                            poly.append( c2pos );
                            hlinePoints.append(poly);
                            hl_index.append(i);
                        }
                    } else {
                        if ( !prev_on && i > 0 ) up_index.append(i);
                    }
                    jointDataToJointItem(jd0)->setInteriorOn(jd0->isInteriorJoint());
                    if ( isIc ) jointDataToJointItem(jd0, false)->setInteriorOn(jd0->isInteriorJoint());
                }
            }

            if ( jdlist.first()->containsFrame(myFrameID) ) {
                JointData *jd1 = jdlist.at(1);
                if ( !jd1->containsFrame(myFrameID) ) {
                    XYZ xyz = copy->globalXYZToFrameXYZ(myFrameID, jd1->xyz());
                    if ( xyz.z < 0.0 ) {
                        if ( md->dataType() == DATATYPE::TPCOLUMN ) dn_index.append(0);
                    } else {
                        up_index.append(0);
                    }
                }
            }

            if ( jdlist.last()->containsFrame(myFrameID) ) {
                JointData *jd1 = jdlist.at(jdlist.count() - 2);
                if ( !jd1->containsFrame(myFrameID) ) {
                    XYZ xyz = copy->globalXYZToFrameXYZ(myFrameID, jd1->xyz());
                    if ( xyz.z < 0.0 ) {
                        if ( md->dataType() == DATATYPE::TPCOLUMN ) dn_index.append(jdlist.count() - 1);
                    } else {
                        up_index.append(jdlist.count() - 1);
                    }
                }
            }

            if ( !hl_index.isEmpty() ) createHLineItem(md, selectFlag, visibleFlag, hlinePoints, hl_index,
                                                           fr_angle, shift_x);
            if ( !up_index.isEmpty() && viewSettings.getCrossElementView().isVisible) createVLineUpperItem(md, selectFlag, visibleFlag, up_index, fr_angle,
                                                                shift_x);
            if ( !dn_index.isEmpty() ) createVLineDownItem(md, visibleFlag, dn_index, fr_angle, shift_x);

            continue;
        }

        // 面材の場合
        if ( md->isClosedMember() ) {

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


            // 閉じた通りの場合、基準となる点をサーチ
            QPointF cmpPoint;
            bool isCmpDetect = false; // 通り起点でない点が見つかった
            Q_FOREACH ( JointData *jd, jdlist ) {
                if ( jd->containsFrame(myFrameID) && jointAndPoint.values().count(jd) == 1 ) {
                    cmpPoint = itemPosition(jd);
                    isCmpDetect = true;
                    break;
                }
            }

            for (int i = 0; i < jdlist.count(); i++) {

                JointData *jd = jdlist.at(i);
                bool isIc = ( jointAndPoint.values().count(jd) > 1 );
                prev_on = cur_on;
                cur_on = jd->containsFrame(myFrameID);

                if ( cur_on ) {
                    if ( prev_on ) {
                        QList<QPointF> poly = planePoints.last();
                        QPointF cpos = ( isIc ) ? itemPosition(jd, poly.last()) : itemPosition(jd);
                        poly.append(cpos);
                        planePoints.replace(planePoints.count() - 1, poly);
                    } else {
                        QList<QPointF> poly;
                        QPointF cpos = ( isIc && isCmpDetect ) ? itemPosition(jd, cmpPoint) : itemPosition(jd);
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
                    XYZ p_xyz = copy->globalXYZToFrameXYZ(myFrameID, pj->xyz());
                    XYZ n_xyz = copy->globalXYZToFrameXYZ(myFrameID, nj->xyz());

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

                        /*
                                                if(  points.first().x()<points.last().x() ){
                                                    crossPoints.insert( insert_idx, points.last() );
                                                }else{
                                                    crossPoints.insert( insert_idx, points.first() );
                                                }
                        */
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

            if ( !hplanePoints.isEmpty() ) createHPlaneItem(md, selectFlag, visibleFlag, hplanePoints,
                                                                isOnPlane);
            if ( !vplanePoints.isEmpty() && viewSettings.getCrossElementView().isVisible) createVPlaneItem(md, selectFlag, visibleFlag, vplanePoints,
                                                                isOnPlane);

        }
    }

    if ( myMode == SELECTHITMODE  ) {
        if ( myElementType == ELJOINTLOAD ) {
            this->setInputItemSelectable(DATATYPE::TPJOINTLOAD);
        } else if ( myElementType == ELMEMBERLOAD ) {
            this->setInputItemSelectable(DATATYPE::TPMEMBERLOAD);
        } else if ( myElementType == ELSLABLOAD ) {
            this->setInputItemSelectable(DATATYPE::TPSLABLOAD);
        }
    }

    if ( myElementType == ALLELEMENT ) {
        setMemberSelectLimited( mdlist );
    }
}

void ElevationCopyInterfaceGrScene::changeMemberItems(const QList<MemberData *> &mdlist)
{
    Q_FOREACH ( MemberData *md, mdlist ) removeMemberItem( md, md->dataType() );
    createMemberItems(filterSceneMember(mdlist));
}

void ElevationCopyInterfaceGrScene::createHLineItem(MemberData *md, unsigned int sflag,
                                                unsigned int vflag,
                                                const QList<QList<QPointF> > &hlinePoints, const QList<int> &hl_index,
                                                qreal angle, qreal shift)
{
    QList<JointData *> jdlist = md->getJointList();

    for (int i = 0; i < hlinePoints.count(); i++) {

        QList<QPointF> points = hlinePoints.at(i);
        QList<qreal> lenlist;
        int idx = hl_index.at(i);
        for (int j = 0; j < points.count() ; j++) {
            lenlist.append( md->getJointLength(idx + j) );
        }
        bool incomplete = ( jdlist.count() != points.count() );

        if ( md->dataType() == DATATYPE::TPCOLUMN ) {

            CustomDrawHLineItem *item = new ColumnHLineItem(viewSettings,points, lenlist, md->getHLineShape(),
                                                            ITEMVIEWTYPE::VIEW_ELEVATION, md->getMemberPartition(), angle, shift);
            item->setPos(points.first());
            item->setZValue(-1000.);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << COL_SELECT) ));
            item->setVisible( vflag & (1 << COL_VISIBLE) );
            item->setSelected(false);
            item->setIncompleteFlag(incomplete);
            this->addItem(item);

            memberAndHColumn.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPGIRDER ) {

            CustomDrawHLineItem *item = new GirderHLineItem(viewSettings,ELGIRDER, points, lenlist, md->getHLineShape(),
                                                            ITEMVIEWTYPE::VIEW_ELEVATION, md->getMemberPartition());
            item->setPos(points.first());
            item->setZValue(-1100.);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << GIR_SELECT) ));
            item->setVisible( vflag & (1 << GIR_VISIBLE) );
            item->setSelected(false);
            item->setIncompleteFlag(incomplete);
            this->addItem(item);

            memberAndHGirder.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPBRACE ) {

            CustomDrawHLineItem *item = new BraceHLineItem(viewSettings,points, lenlist, md->getHLineShape(),
                                                           ITEMVIEWTYPE::VIEW_ELEVATION, md->getMemberPartition());
            item->setPos(points.first());
            item->setZValue(-1100.);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << BRA_SELECT) ));
            item->setVisible( vflag & (1 << BRA_VISIBLE) );
            item->setSelected(false);
            item->setIncompleteFlag(incomplete);
            this->addItem(item);

            memberAndHBrace.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPBEAM ) {

            CustomDrawHLineItem *item = new BeamHLineItem(viewSettings,points, lenlist, md->getHLineShape(),
                                                          ITEMVIEWTYPE::VIEW_ELEVATION, md->getMemberPartition());
            item->setPos(points.first());
            item->setZValue(-1200.);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << BEA_SELECT) ));
            item->setVisible( vflag & (1 << BEA_VISIBLE) );
            item->setSelected(false);
            item->setIncompleteFlag(incomplete);
            this->addItem(item);

            memberAndHBeam.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPDAMPER ) {

            CustomDrawHLineItem *item = new DamperHLineItem(viewSettings,points, lenlist, md->getHLineShape(),
                                                            ITEMVIEWTYPE::VIEW_ELEVATION, md->getMemberPartition());
            item->setPos(points.first());
            item->setZValue(-1300.);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << DAM_SELECT) ));
            item->setVisible( vflag & (1 << DAM_VISIBLE) );
            item->setSelected(false);
            item->setIncompleteFlag(incomplete);
            this->addItem(item);

            memberAndHDamper.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPISO ) {

            CustomDrawHLineItem *item = new IsolatorHLineItem(viewSettings,points, lenlist, md->getHLineShape(),
                                                              ITEMVIEWTYPE::VIEW_ELEVATION, md->getMemberPartition());
            item->setPos(points.first());
            item->setZValue(-1400.);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << ISO_SELECT) ));
            item->setVisible( vflag & (1 << ISO_VISIBLE) );
            item->setSelected(false);
            item->setIncompleteFlag(incomplete);
            this->addItem(item);

            memberAndHIsolator.insert(item, md);


        } else if ( md->dataType() == DATATYPE::TPMEMBERLOAD ) {

            CustomDrawHLineItem *item = new MemberLoadHLineItem(viewSettings,points, lenlist,
                                                                md->sectionName());
            item->setPos(points.first());
            item->setZValue(-950.);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << MLD_SELECT) ));
            item->setVisible( vflag & (1 << MLD_VISIBLE) );
            item->setSelected(false);
            item->setIncompleteFlag(incomplete);
            this->addItem(item);

            memberAndMemberLoad.insert(item, md);
        }

    }
}

void ElevationCopyInterfaceGrScene::createVLineUpperItem(MemberData *md, unsigned int sflag,
                                                     unsigned int vflag,
                                                     const QList<int> &up_index, qreal angle, qreal shift)
{
     if ( md->dataType() == DATATYPE::TPMEMBERLOAD ) return;

    QList<JointData *> jdlist = md->getJointList();
    /* VLineItem Upper */
    for (int i = 0; i < up_index.count(); i++) {

        JointData *jd = jdlist.at(up_index.at(i));
        QPointF pp = itemPosition(jd);
        bool forwd = true;

        qreal len;
        if ( up_index.at(i) == 0 ) {
            len = md->getIFaceLength();
            forwd = true;
        } else if ( up_index.at(i) == jdlist.count() - 1 ) {
            len = md->getJFaceLength();
            forwd = false;
        } else {
            len = md->getJointLength(up_index.at(i));
            XYZ xyz = copy->globalXYZToFrameXYZ(myFrameID,
                                                                      jdlist.at(up_index.at(i) - 1)->xyz());
            forwd = ( xyz.z < 0.0 );
        }
        VLINESHAPE size = md->getVLineShape(len);

        if ( md->dataType() == DATATYPE::TPCOLUMN ) {

            CustomDrawVLineItem *item = new ColumnVLineItem(false, viewSettings,pp, size, ITEMVIEWTYPE::VIEW_ELEVATION, ELEMENTDIR::ELCROSS, forwd, angle,
                                                            shift);
            item->setPos(pp);
            item->setZValue(-500.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << COL_SELECT) ));
            item->setVisible( vflag & (1 << COL_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);

            memberAndVColumn.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPGIRDER ) {

            CustomDrawVLineItem *item = new GirderVLineItem(false, viewSettings,ELGIRDER, pp, size, ITEMVIEWTYPE::VIEW_ELEVATION, forwd, 0.0,
                                                            0.0);
            item->setPos(pp);
            item->setZValue(-600.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << GIR_SELECT) ));
            item->setVisible( vflag & (1 << GIR_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);

            memberAndVGirder.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPBRACE ) {

            CustomDrawVLineItem *item = new BraceVLineItem(false, viewSettings,pp, size, ITEMVIEWTYPE::VIEW_ELEVATION, forwd, 0.0, 0.0);
            item->setPos(pp);
            item->setZValue(-600.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << BRA_SELECT) ));
            item->setVisible( vflag & (1 << BRA_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);

            memberAndVBrace.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPBEAM ) {

            CustomDrawVLineItem *item = new BeamVLineItem(false, viewSettings,pp, size, ITEMVIEWTYPE::VIEW_ELEVATION, forwd, 0.0, 0.0);
            item->setPos(pp);
            item->setZValue(-700.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << BEA_SELECT) ));
            item->setVisible( vflag & (1 << BEA_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);

            memberAndVBeam.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPDAMPER ) {

            CustomDrawVLineItem *item = new DamperVLineItem(false, viewSettings,pp, size, ITEMVIEWTYPE::VIEW_ELEVATION, forwd);
            item->setPos(pp);
            item->setZValue(-800.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << DAM_SELECT) ));
            item->setVisible( vflag & (1 << DAM_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);

            memberAndVDamper.insert(item, md);

        } else if ( md->dataType() == DATATYPE::TPISO ) {

            CustomDrawVLineItem *item = new IsolatorVLineItem(false, viewSettings,pp, size, ITEMVIEWTYPE::VIEW_ELEVATION, forwd);
            item->setPos(pp);
            item->setZValue(-900.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << ISO_SELECT) ));
            item->setVisible( vflag & (1 << ISO_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);

            memberAndVIsolator.insert(item, md);

        }

    }
}

void ElevationCopyInterfaceGrScene::createVLineDownItem(MemberData *md, unsigned int vflag,
                                                    const QList<int> &dn_index, qreal angle, qreal shift)
{
    QList<JointData *> jdlist = md->getJointList();

    for (int i = 0; i < dn_index.count(); i++) {

        JointData *jd = jdlist.at(dn_index.at(i));
        QPointF pp = itemPosition(jd);
        bool forwd = true;

        qreal len;
        if ( dn_index.at(i) == 0 ) {
            len = md->getIFaceLength();
            forwd = false;
        } else if ( dn_index.at(i) == jdlist.count() - 1 ) {
            len = md->getJFaceLength();
            forwd = true;
        }
//            else{
//                len=md->getJointLength(dn_index.at(i));
//                XYZ xyz= FactoryUnifiedData::getInstance(isOutput)->globalXYZToFrameXYZ(myFrameID,jdlist.at(up_index.at(i)+1)->xyz());
//                rev=( xyz.z > 0.0 );
//            }
        VLINESHAPE size = md->getVLineShape(len);

        if ( md->dataType() == DATATYPE::TPCOLUMN ) {

            CustomDrawVLineItem *item = new ColumnVLineItem(false, viewSettings,pp, size, ITEMVIEWTYPE::VIEW_ELEVATION, ELEMENTDIR::ELDOWN, forwd, angle,
                                                            shift);
            item->setPos(pp);
            item->setZValue(-1500.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, false);
            item->setVisible( vflag & (1 << COL_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);

            memberAndOffVColumn.insert(item, md);

        }
    }
}

void ElevationCopyInterfaceGrScene::createHPlaneItem(MemberData *md, unsigned int sflag,
                                                 unsigned int vflag,
                                                 const QList<QList<QPointF> > &planePoints, bool isOnPlane)
{
    for ( int i = 0; i < planePoints.count(); i++ ) {

        QList<QPointF> points = planePoints.at(i);

        if ( md->dataType() == DATATYPE::TPSLAB ) {

            CustomDrawHPlaneItem *item = new SlabHPlaneItem(viewSettings,points, md->sectionName(),
                                                            isOnPlane, md->getPlaneThickness());
            item->setPos(points.first());
            item->setZValue(-1300.0);
            item->setEnabled(true);
            item->setFlag( QGraphicsItem::ItemIsSelectable, ( sflag & (1 << SLA_SELECT) ) );
            item->setVisible( vflag & (1 << SLA_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);
            memberAndHSlab.insert(item, md);

        }

        if ( md->dataType() == DATATYPE::TPWALL ) {

            CustomDrawHPlaneItem *item = new WallHPlaneItem(viewSettings,ELWALL, points, md->sectionName(),
                                                            isOnPlane, md->getPlaneThickness(), md->wallOpenName());
            item->setPos(points.first());
            item->setZValue(-1300.0);
            item->setEnabled(true);
            item->setFlag( QGraphicsItem::ItemIsSelectable, ( sflag & (1 << WAL_SELECT) ) );
            item->setVisible( vflag & (1 << WAL_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);
            memberAndHWall.insert(item, md);

        }

        if ( md->dataType() == DATATYPE::TPUWALL ) {

            CustomDrawHPlaneItem *item = new WallHPlaneItem(viewSettings,ELUWALL, points, md->sectionName(),
                                                            isOnPlane, md->getPlaneThickness(), md->wallOpenName());
            item->setPos(points.first());
            item->setZValue(-1300.0);
            item->setEnabled(true);
            item->setFlag( QGraphicsItem::ItemIsSelectable, ( sflag & (1 << UWA_SELECT) ) );
            item->setVisible( vflag & (1 << UWA_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);
            memberAndHUWall.insert(item, md);

        }

        if ( md->dataType() == DATATYPE::TPSLABLOAD ) {

            CustomDrawHPlaneItem *item = new SlabLoadHPlaneItem(viewSettings,points, md->sectionName(), isOnPlane);
            item->setPos(points.first());
            item->setZValue(-1250.0);
            item->setEnabled(true);
            item->setFlag( QGraphicsItem::ItemIsSelectable, ( sflag & (1 << SLD_SELECT) ) );
            item->setVisible( vflag & (1 << SLD_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);
            memberAndSlabLoad.insert(item, md);

        }
    }
}

void ElevationCopyInterfaceGrScene::createVPlaneItem(MemberData *md, unsigned int sflag,
                                                 unsigned int vflag,
                                                 const QList<QList<QPointF> > &planePoints, bool isOnPlane)
{
    if ( md->dataType() == DATATYPE::TPSLABLOAD ) return;

    for ( int i = 0; i < planePoints.count(); i++ ) {

        QList<QPointF> points = planePoints.at(i);

        if ( md->dataType() == DATATYPE::TPSLAB ) {

            CustomDrawVPlaneItem *item = new SlabVPlaneItem(false, viewSettings,points, md->sectionName(), isOnPlane,
                                                            ITEMVIEWTYPE::VIEW_ELEVATION,
                                                            md->getPlaneThickness(), md->getShiftZ());
            item->setPos(points.first());
            item->setZValue(-800.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << SLA_SELECT) ) );
            item->setVisible( vflag & (1 << SLA_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);
            memberAndVSlab.insert(item, md);

        }

        if ( md->dataType() == DATATYPE::TPWALL ) {

            CustomDrawVPlaneItem *item = new WallVPlaneItem(false, viewSettings,ELWALL, points, md->sectionName(), isOnPlane,
                                                            md->getPlaneThickness(), md->getShiftZ());
            item->setPos(points.first());
            item->setZValue(-800.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << WAL_SELECT) ) );
            item->setVisible( vflag & (1 << WAL_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);
            memberAndVWall.insert(item, md);

        }

        if ( md->dataType() == DATATYPE::TPUWALL ) {

            CustomDrawVPlaneItem *item = new WallVPlaneItem(false, viewSettings,ELUWALL, points, md->sectionName(), isOnPlane,
                                                            md->getPlaneThickness(), md->getShiftZ());
            item->setPos(points.first());
            item->setZValue(-800.0);
            item->setEnabled(true);
            item->setFlag(QGraphicsItem::ItemIsSelectable, ( sflag & (1 << UWA_SELECT) ) );
            item->setVisible( vflag & (1 << UWA_VISIBLE) );
            item->setSelected(false);
            this->addItem(item);
            memberAndVUWall.insert(item, md);

        }
    }
}

bool ElevationCopyInterfaceGrScene::isClosedFrame() const
{
    FrameType::FRAMETYPE f_type =  copy->getFrameTypeOfFloor(myFrameID);
    return ( f_type == FrameType::FRAMETYPE::CLCIRCLE || f_type == FrameType::FRAMETYPE::CLVARIABLE ) ;
}

FIGURE_TYPE_PROPERTY ElevationCopyInterfaceGrScene::getFigureTypeProperty() const{
    FIGURE_TYPE_PROPERTY type;
    type.type = OUTFIGTYPE::SECTION;
    QUuid uuid = this->frameID();
    type.viewtype=MODELVIEWTYPE::INPUT2DMODELCOPY;
    type.scenetype=SCENETYPE::ELEVATIONSCENE;
    type.name = UnifiedFixedData::getInstance()->getInputData()->idToName(uuid, DATATYPE::TPFRAMEP);
    type.filename=UnifiedFixedData::getInstance()->getInputData()->fileName();
    return type;
}

} // namespace post3dapp
