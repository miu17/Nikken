#include "input_grscene.h"

#include <QDebug>

#include "calc_vector2d.h"
#include "customdraw_hlineitem.h"
#include "customdraw_hplaneitem.h"
#include "customdraw_vlineitem.h"
#include "customdraw_vplaneitem.h"
#include "define_draw_figure.h"
#include "global_point.h"
#include "jointitem.h"
#include "jointloaditem.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "manage_springdata.h"
#include "parameters.h"
#include "unified_inputdata.h"
#include "unified_settings.h"

namespace post3dapp{
InputGrScene::InputGrScene(LIMITSELECTTERM term, SCENETYPE type, const Input2DViewSettings& setting, bool _syncSetting, QObject *parent)
    : CustomDrawGrScene(_syncSetting, parent ), viewSettings(setting), mySceneType(type), myselectTerm(term)
{
    this->setSceneRect(PARAMETERS::scene_x_origin, PARAMETERS::scene_y_origin,
                                   PARAMETERS::scene_width, PARAMETERS::scene_height);

    if (syncSetting){
        connect( UnifiedSettings::getInstance(), &UnifiedSettings::in2D_SettingChanged,
                 this, &InputGrScene::updateViewSettings );
    }

    connect( this, &QGraphicsScene::selectionChanged, this, &InputGrScene::sendSelectedItems );
}

InputGrScene::~InputGrScene()
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


}

void InputGrScene::updateViewSettings()
{
    viewSettings = UnifiedSettings::getInstance()->getInput2DViewSettings();
    // 通芯を再描画するには、上位にシグナルを投げる
    emit resetSceneView();
}

QList<GLOBALLINE> InputGrScene::getBatchInputLines() const
{
    return batchInputLines;
}

QList<QPolygonF> InputGrScene::getBatchInputPlanes() const
{
    QList<QPolygonF> polygons;
    Q_FOREACH ( QList<JointItem *> items, batchInputPlanes ) {
        QPolygonF polygon;
        Q_FOREACH ( JointItem *item, items ) polygon.append(item->pos());
        polygons.append(polygon);
    }
    return polygons;
}

void InputGrScene::searchNearerJointItem(QPointF pp, GlobalPoint &nearly_point, qreal &min_len)
{
    JointItem *nearly_item = nullptr;
    Q_FOREACH (JointItem *item, jointAndPoint.keys()) {
        qreal len = (pp - item->pos()).manhattanLength();
        if ( len < min_len + 1.0e-3 ) {
            if ( nearly_item != nullptr && qAbs(min_len - len) < 1.0e-3 ) {
                if ( item->zValue() < nearly_item->zValue() ) continue;
            }
            min_len = len;
            nearly_item = item;
        }
    }
    if (nearly_item == nullptr) return;
    JointData *jd = jointAndPoint.value(nearly_item);
    GlobalPoint gp(jd->xPos(), jd->yPos(), jd->zPos());
    gp.setElvX(nearly_item->x() / 1000.0);
    gp.setElvY(nearly_item->y() / 1000.0);
    gp.setElvZ(nearly_item->zValue() / 1000.0);
    gp.appendAttachedFloor(jd->getAttachedFloorList());
    gp.appendAttachedFrame(jd->getAttachedFrameList());
    nearly_point = gp;
}

JointData *InputGrScene::searchNearlyJoint(QPointF pp, qreal eps)
{
    JointItem *nearly_item = nullptr;
    Q_FOREACH (JointItem *item,  jointAndPoint.keys()) {
        qreal len = (pp - item->pos()).manhattanLength();
        if ( len < eps ) {
            if ( nearly_item != nullptr && item->zValue() < nearly_item->zValue() ) continue;
            nearly_item = item;
        }
    }
    return ( nearly_item != nullptr ) ? jointAndPoint.value(nearly_item) : nullptr;
}

QList<JointData *> InputGrScene::selectedJoints(ELEMENTTYPE etype) const
{
    if ( etype != ELJOINT && etype != ELJOINTLOAD && etype != ALLELEMENT ) return QList<JointData *>();

    QList<JointData *> jdlist;
    Q_FOREACH ( QGraphicsItem *item, this->selectedItems() ) {
        JointItem *c_item = qgraphicsitem_cast<JointItem *>(item);
        if ( !jointAndPoint.contains(c_item) ) continue;
        jdlist.append( jointAndPoint.value(c_item) );
    }
    return jdlist;
}
QList<SpringData *> InputGrScene::selectedSprings() const
{
    QList<SpringData *> sdlist;return sdlist;
}
MemberData *InputGrScene::selectedMember() const
{
    QList<MemberData *> mdlist = selectedMembers();
    return ( mdlist.isEmpty() ) ? nullptr : mdlist.first() ;
}

QList<MemberData *> InputGrScene::selectedMembers() const
{
    QList<MemberData *> mdlist;
    Q_FOREACH ( QGraphicsItem *item, this->selectedItems() ) {

        CustomDrawItem *citem = qgraphicsitem_cast<CustomDrawItem *>(item);
        switch ( citem->elementType() ) {
        case ELCOLUMN:
            if ( citem->itemType() == ITEMTYPE::HLINEITEM ) {
                CustomDrawHLineItem *hitem = qgraphicsitem_cast<CustomDrawHLineItem *>(item);
                if ( memberAndHColumn.contains(hitem) ) {
                    MemberData *md = memberAndHColumn.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndHColumn.value(hitem) );
                    break;
                }
            } else {
                CustomDrawVLineItem *vitem = qgraphicsitem_cast<CustomDrawVLineItem *>(item);
                if ( memberAndVColumn.contains(vitem) ) {
                    MemberData *md = memberAndVColumn.value(vitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndVColumn.value(vitem) );
                    break;
                }
            }
            break;
        case ELGIRDER:
            if ( citem->itemType() == ITEMTYPE::HLINEITEM ) {
                CustomDrawHLineItem *hitem = qgraphicsitem_cast<CustomDrawHLineItem *>(item);
                if ( memberAndHGirder.contains(hitem) ) {
                    MemberData *md = memberAndHGirder.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndHGirder.value(hitem) );
                    break;
                }
            } else {
                CustomDrawVLineItem *vitem = qgraphicsitem_cast<CustomDrawVLineItem *>(item);
                if ( memberAndVGirder.contains(vitem) ) {
                    MemberData *md = memberAndVGirder.value(vitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndVGirder.value(vitem) );
                    break;
                }
            }
            break;
        case ELBRACE:
            if ( citem->itemType() == ITEMTYPE::HLINEITEM ) {
                CustomDrawHLineItem *hitem = qgraphicsitem_cast<CustomDrawHLineItem *>(item);
                if ( memberAndHBrace.contains(hitem) ) {
                    MemberData *md = memberAndHBrace.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndHBrace.value(hitem) );
                    break;
                }
            } else {
                CustomDrawVLineItem *vitem = qgraphicsitem_cast<CustomDrawVLineItem *>(item);
                if ( memberAndVBrace.contains(vitem) ) {
                    MemberData *md = memberAndVBrace.value(vitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndVBrace.value(vitem) );
                    break;
                }
            }
            break;
        case ELBEAM:
            if ( citem->itemType() == ITEMTYPE::HLINEITEM ) {
                CustomDrawHLineItem *hitem = qgraphicsitem_cast<CustomDrawHLineItem *>(item);
                if ( memberAndHBeam.contains(hitem) ) {
                    MemberData *md = memberAndHBeam.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndHBeam.value(hitem) );
                    break;
                }
            } else {
                CustomDrawVLineItem *vitem = qgraphicsitem_cast<CustomDrawVLineItem *>(item);
                if ( memberAndVBeam.contains(vitem) ) {
                    MemberData *md = memberAndVBeam.value(vitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndVBeam.value(vitem) );
                    break;
                }
            }
            break;
        case ELSLAB:
            if ( citem->itemType() == ITEMTYPE::HPLANEITEM ) {
                CustomDrawHPlaneItem *hitem = qgraphicsitem_cast<CustomDrawHPlaneItem *>(item);
                if ( memberAndHSlab.contains(hitem) ) {
                    MemberData *md = memberAndHSlab.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndHSlab.value(hitem) );
                    break;
                }
            } else {
                CustomDrawVPlaneItem *vitem = qgraphicsitem_cast<CustomDrawVPlaneItem *>(item);
                if ( memberAndVSlab.contains(vitem) ) {
                    MemberData *md = memberAndVSlab.value(vitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndVSlab.value(vitem) );
                    break;
                }
            }
            break;
        case ELWALL:
            if ( citem->itemType() == ITEMTYPE::HPLANEITEM ) {
                CustomDrawHPlaneItem *hitem = qgraphicsitem_cast<CustomDrawHPlaneItem *>(item);
                if ( memberAndHWall.contains(hitem) ) {
                    MemberData *md = memberAndHWall.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndHWall.value(hitem) );
                    break;
                }
            } else {
                CustomDrawVPlaneItem *vitem = qgraphicsitem_cast<CustomDrawVPlaneItem *>(item);
                if ( memberAndVWall.contains(vitem) ) {
                    MemberData *md = memberAndVWall.value(vitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndVWall.value(vitem) );
                    break;
                }
            }
            break;
        case ELUWALL:
            if ( citem->itemType() == ITEMTYPE::HPLANEITEM ) {
                CustomDrawHPlaneItem *hitem = qgraphicsitem_cast<CustomDrawHPlaneItem *>(item);
                if ( memberAndHUWall.contains(hitem) ) {
                    MemberData *md = memberAndHUWall.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndHUWall.value(hitem) );
                    break;
                }
            } else {
                CustomDrawVPlaneItem *vitem = qgraphicsitem_cast<CustomDrawVPlaneItem *>(item);
                if ( memberAndVUWall.contains(vitem) ) {
                    MemberData *md = memberAndVUWall.value(vitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndVUWall.value(vitem) );
                    break;
                }
            }
            break;
        case ELDAMPER:
            if ( citem->itemType() == ITEMTYPE::HLINEITEM ) {
                CustomDrawHLineItem *hitem = qgraphicsitem_cast<CustomDrawHLineItem *>(item);
                if ( memberAndHDamper.contains(hitem) ) {
                    MemberData *md = memberAndHDamper.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndHDamper.value(hitem) );
                    break;
                }
            } else {
                CustomDrawVLineItem *vitem = qgraphicsitem_cast<CustomDrawVLineItem *>(item);
                if ( memberAndVDamper.contains(vitem) ) {
                    MemberData *md = memberAndVDamper.value(vitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndVDamper.value(vitem) );
                    break;
                }
            }
            break;
        case ELISO:
            if ( citem->itemType() == ITEMTYPE::HLINEITEM ) {
                CustomDrawHLineItem *hitem = qgraphicsitem_cast<CustomDrawHLineItem *>(item);
                if ( memberAndHIsolator.contains(hitem) ) {
                    MemberData *md = memberAndHIsolator.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndHIsolator.value(hitem) );
                    break;
                }
            } else {
                CustomDrawVLineItem *vitem = qgraphicsitem_cast<CustomDrawVLineItem *>(item);
                if ( memberAndVIsolator.contains(vitem) ) {
                    MemberData *md = memberAndVIsolator.value(vitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndVIsolator.value(vitem) );
                    break;
                }
            }
            break;
        case ELJOINTLOAD:
            if ( citem->itemType() == ITEMTYPE::POINTITEM ) {
                JointLoadItem *jitem = qgraphicsitem_cast<JointLoadItem *>(item);
                if ( memberAndJointLoad.contains(jitem) ) {
                    MemberData *md = memberAndJointLoad.value(jitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndJointLoad.value(jitem) );
                    break;
                }
            }
            break;
        case ELMEMBERLOAD:
            if ( citem->itemType() == ITEMTYPE::HLINEITEM ) {
                CustomDrawHLineItem *hitem = qgraphicsitem_cast<CustomDrawHLineItem *>(item);
                if ( memberAndMemberLoad.contains(hitem) ) {
                    MemberData *md = memberAndMemberLoad.value(hitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndMemberLoad.value(hitem) );
                    break;
                }
            }
            break;
        case ELSLABLOAD:
            if ( citem->itemType() == ITEMTYPE::HPLANEITEM ) {
                CustomDrawHPlaneItem *pitem = qgraphicsitem_cast<CustomDrawHPlaneItem *>(item);
                if ( memberAndSlabLoad.contains(pitem) ) {
                    MemberData *md = memberAndSlabLoad.value(pitem);
                    if ( !mdlist.contains(md) ) mdlist.append( memberAndSlabLoad.value(pitem) );
                    break;
                }
            }
            break;
        default:
            break;
        }

    }
    return mdlist;
}

QList<GlobalPoint> InputGrScene::selectedPoints() const
{
    QList<GlobalPoint> gplist;
    Q_FOREACH ( QGraphicsItem *item, this->selectedItems() ) {
        JointItem *jitem = qgraphicsitem_cast<JointItem *>(item);
        if ( !jointAndPoint.contains(jitem) ) continue;
        JointData *jd = jointAndPoint.value(jitem);
        GlobalPoint gp(jd->xPos(), jd->yPos(), jd->zPos());
        gp.setElvX(item->x() / 1000.0);
        gp.setElvY(item->y() / 1000.0);
        gp.setElvZ(item->zValue() / 1000.0);
        gp.appendAttachedFloor(jd->getAttachedFloorList());
        gp.appendAttachedFrame(jd->getAttachedFrameList());
        gplist.append(gp);
    }
    return gplist;
}

bool InputGrScene::isColumnSelected() const
{
    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHColumn.keys() ) {
        if ( item->isSelected() ) return true;
    }
    Q_FOREACH ( CustomDrawVLineItem *item, memberAndVColumn.keys() ) {
        if ( item->isSelected() ) return true;
    }
    return false;
}

bool InputGrScene::isWallSelected() const
{
    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHWall.keys() ) {
        if ( item->isSelected() ) return true;
    }
    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVWall.keys() ) {
        if ( item->isSelected() ) return true;
    }
    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHUWall.keys() ) {
        if ( item->isSelected() ) return true;
    }
    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVUWall.keys() ) {
        if ( item->isSelected() ) return true;
    }
    return false;
}

bool InputGrScene::isWallOpenSelected() const
{
    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHWall.keys() ) {
        if ( item->isSelected() && memberAndHWall.value(item)->hasWallOpen() ) return true;
    }
    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVWall.keys() ) {
        if ( item->isSelected() && memberAndVWall.value(item)->hasWallOpen() ) return true;
    }
    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHUWall.keys() ) {
        if ( item->isSelected() && memberAndHUWall.value(item)->hasWallOpen() ) return true;
    }
    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVUWall.keys() ) {
        if ( item->isSelected() && memberAndVUWall.value(item)->hasWallOpen() ) return true;
    }
    return false;
}

bool InputGrScene::isExistedJoint( JointData *jd ) const
{
    return jointAndPoint.values().contains(jd);
}

bool InputGrScene::isExistedMember( MemberData *md ) const
{
    bool isExisted = false;

    switch ( md->dataType() ) {
    case DATATYPE::TPCOLUMN:
        isExisted = ( memberAndHColumn.values().contains(md)
                      || memberAndVColumn.values().contains(md)
                      || memberAndOffVColumn.values().contains(md) );
        break;
    case DATATYPE::TPGIRDER:
        isExisted = ( memberAndHGirder.values().contains(md)
                      || memberAndVGirder.values().contains(md) );
        break;
    case DATATYPE::TPBRACE:
        isExisted = ( memberAndHBrace.values().contains(md)
                      || memberAndVBrace.values().contains(md) );
        break;
    case DATATYPE::TPBEAM:
        isExisted = ( memberAndHBeam.values().contains(md)
                      || memberAndVBeam.values().contains(md) );
        break;
    case DATATYPE::TPSLAB:
        isExisted = ( memberAndHSlab.values().contains(md)
                      || memberAndVSlab.values().contains(md) );
        break;
    case DATATYPE::TPWALL:
        isExisted = ( memberAndHWall.values().contains(md)
                      || memberAndVWall.values().contains(md) );
        break;
    case DATATYPE::TPUWALL:
        isExisted = ( memberAndHUWall.values().contains(md)
                      || memberAndVUWall.values().contains(md) );
        break;
    case DATATYPE::TPDAMPER:
        isExisted = ( memberAndHDamper.values().contains(md)
                      || memberAndVDamper.values().contains(md) );
        break;
    case DATATYPE::TPISO:
        isExisted = ( memberAndHIsolator.values().contains(md)
                      || memberAndVIsolator.values().contains(md) );
        break;
    case DATATYPE::TPJOINTLOAD:
        isExisted = ( memberAndJointLoad.values().contains(md) );
        break;
    case DATATYPE::TPMEMBERLOAD:
        isExisted = ( memberAndMemberLoad.values().contains(md) );
        break;
    case DATATYPE::TPSLABLOAD:
        isExisted = ( memberAndSlabLoad.values().contains(md) );
        break;
    default:
        break;
    }

    return isExisted;
}

QList<QLineF> InputGrScene::getMoveWaitingLines(QGraphicsItem *b_item, QPointF cp, ELEMENTTYPE etype)
{
    QList<JointData *> all_jdlist = jointAndPoint.values();
    QList<JointData *> select_jdlist = selectedJoints(etype);
    QList<JointData *> done_jdlist;
    QList<QLineF> lineList;

    Q_FOREACH ( JointData *ijd, select_jdlist ) {
        QList<JointData *> linked_jdlist = ijd->linkedJoints();
//        QPointF p1=jointDataToJointItem(ijd)->pos()+cp-b_item->pos();
        QPointF p1 = itemPosition(ijd) + cp - b_item->pos();
        Q_FOREACH ( JointData *jjd, linked_jdlist ) {
            if ( !all_jdlist.contains(jjd) || done_jdlist.contains(jjd) ) continue;
//            QPointF p2=jointDataToJointItem(jjd)->pos();
            QPointF p2 = itemPosition(jjd, p1);
            if ( select_jdlist.contains(jjd) ) p2 = p2 + cp - b_item->pos();
            lineList.append( QLineF(p1, p2) );
        }
        done_jdlist.append(ijd);
    }
    return lineList;
}

JOINTGROUP InputGrScene::selectedMemberJoints(QPointF pp) const
{
    if ( selectedItems().count() != 1 ) return JOINTGROUP();

    CustomDrawItem *item = qgraphicsitem_cast<CustomDrawItem *>(selectedItems().first());
    if ( item->itemType() == ITEMTYPE::POINTITEM ) return JOINTGROUP();

    MemberData *md = NULL;
    if ( item->itemType() == ITEMTYPE::HLINEITEM ) {
        CustomDrawHLineItem *hitem = qgraphicsitem_cast<CustomDrawHLineItem *>(item);
        if ( item->elementType() == ELCOLUMN ) {
            if ( memberAndHColumn.contains(hitem) ) md = memberAndHColumn.value(hitem);
        } else if ( item->elementType() == ELGIRDER ) {
            if ( memberAndHGirder.contains(hitem) ) md = memberAndHGirder.value(hitem);
        } else if ( item->elementType() == ELBRACE ) {
            if ( memberAndHBrace.contains(hitem) ) md = memberAndHBrace.value(hitem);
        } else if ( item->elementType() == ELBEAM ) {
            if ( memberAndHBeam.contains(hitem) ) md = memberAndHBeam.value(hitem);
        } else if ( item->elementType() == ELDAMPER ) {
            if ( memberAndHDamper.contains(hitem) ) md = memberAndHDamper.value(hitem);
        } else if ( item->elementType() == ELISO ) {
            if ( memberAndHIsolator.contains(hitem) ) md = memberAndHIsolator.value(hitem);
        } else if ( item->elementType() == ELMEMBERLOAD ) {
            if ( memberAndMemberLoad.contains(hitem) ) md = memberAndMemberLoad.value(hitem);
        }
    } else if ( item->itemType() == ITEMTYPE::VLINEITEM ) {
        CustomDrawVLineItem *vitem = qgraphicsitem_cast<CustomDrawVLineItem *>(item);
        if ( item->elementType() == ELCOLUMN ) {
            if ( memberAndVColumn.contains(vitem) ) md = memberAndVColumn.value(vitem);
        } else if ( item->elementType() == ELGIRDER ) {
            if ( memberAndVGirder.contains(vitem) ) md = memberAndVGirder.value(vitem);
        } else if ( item->elementType() == ELBRACE ) {
            if ( memberAndVBrace.contains(vitem) ) md = memberAndVBrace.value(vitem);
        } else if ( item->elementType() == ELBEAM ) {
            if ( memberAndVBeam.contains(vitem) ) md = memberAndVBeam.value(vitem);
        } else if ( item->elementType() == ELDAMPER ) {
            if ( memberAndVDamper.contains(vitem) ) md = memberAndVDamper.value(vitem);
        } else if ( item->elementType() == ELISO ) {
            if ( memberAndVIsolator.contains(vitem) ) md = memberAndVIsolator.value(vitem);
        }
    } else if ( item->itemType() == ITEMTYPE::HPLANEITEM ) {
        CustomDrawHPlaneItem *hitem = qgraphicsitem_cast<CustomDrawHPlaneItem *>(item);
        if ( item->elementType() == ELSLAB ) {
            if ( memberAndHSlab.contains(hitem) ) md = memberAndHSlab.value(hitem);
        } else if ( item->elementType() == ELWALL ) {
            if ( memberAndHWall.contains(hitem) ) md = memberAndHWall.value(hitem);
        } else if ( item->elementType() == ELUWALL ) {
            if ( memberAndHUWall.contains(hitem) ) md = memberAndHUWall.value(hitem);
        } else if ( item->elementType() == ELSLABLOAD ) {
            if ( memberAndSlabLoad.contains(hitem) ) md = memberAndSlabLoad.value(hitem);
        }
    } else if ( item->itemType() == ITEMTYPE::VPLANEITEM ) {
        CustomDrawVPlaneItem *vitem = qgraphicsitem_cast<CustomDrawVPlaneItem *>(item);
        if ( item->elementType() == ELSLAB ) {
            if ( memberAndVSlab.contains(vitem) ) md = memberAndVSlab.value(vitem);
        } else if ( item->elementType() == ELWALL ) {
            if ( memberAndVWall.contains(vitem) ) md = memberAndVWall.value(vitem);
        } else if ( item->elementType() == ELUWALL ) {
            if ( memberAndVUWall.contains(vitem) ) md = memberAndVUWall.value(vitem);
        }
    } else if ( item->itemType() == ITEMTYPE::POINTITEM ) {
        if ( item->elementType() == ELJOINTLOAD ) {
            JointLoadItem *jitem = qgraphicsitem_cast<JointLoadItem *>(item);
            if ( memberAndJointLoad.contains(jitem) ) md = memberAndJointLoad.value(jitem);
        }
    }

    if ( md == NULL ) return JOINTGROUP();

    if ( item->elementType() == ELJOINTLOAD ) {
        JointData *jd = md->getJointList().first();
        return ( jointAndPoint.values().contains(jd) ) ? JOINTGROUP( NULL, jd, NULL ) : JOINTGROUP();
    }

    JointData *nearlyJoint = NULL;
    qreal min_len = 1.0e+10;
    Q_FOREACH ( JointData *jd, md->getJointList() ) {
        if ( jointAndPoint.values().contains(jd) ) {
//            JointItem* j_item=jointDataToJointItem(jd,!md->isJJoint(jd));
            qreal len = (pp - itemPosition(jd, pp)).manhattanLength();
            if ( len < min_len ) {
                nearlyJoint = jd;
                min_len = len;
            }
        }
    }

    JointData *j1 = md->previousJoint(nearlyJoint);
    if ( !jointAndPoint.values().contains(j1) ) j1 = NULL;
    JointData *j3 = md->nextJoint(nearlyJoint);
    if ( !jointAndPoint.values().contains(j3) ) j3 = NULL;

    return JOINTGROUP( j1, nearlyJoint, j3 );
}

QList<JointData *> InputGrScene::searchBetweenJoints(JointData *ijd, JointData *jjd)
{
    if ( !jointAndPoint.values().contains(ijd) ||
            !jointAndPoint.values().contains(jjd) ) return QList<JointData *>();
    /*
        JointItem* i_item=jointAndPoint.key(ijd);
        JointItem* j_item=jointAndPoint.key(jjd);
    */
    JointItem *i_item = jointDataToJointItem(ijd, true);
    JointItem *j_item = jointDataToJointItem(jjd, false);

    QList<JointData *> jdlist;
    QList<qreal> dxlist;
    qreal eps = 1.0e-3;
    Q_FOREACH ( JointItem *item, jointAndPoint.keys() ) {
        if ( item == i_item || item == j_item || qAbs(item->zValue()) > eps ) continue;
        bool online = CalcVector2D::between(i_item->pos(), j_item->pos(), item->pos());
        if ( !online ) continue;
        qreal dx = CalcVector2D::length(item->pos() - i_item->pos());
        int idx = dxlist.count();
        for ( int i = 0; i < dxlist.count() ; i++) {
            if ( dx < dxlist.at(i) ) {
                idx = i;
                break;
            }
        }
        jdlist.insert(idx, jointAndPoint.value(item));
        dxlist.insert(idx, dx);
    }
    return jdlist;
}

QList<JointData *> InputGrScene::searchBetweenJoints(const QList<JointData *> &jdlist)
{
    QList<JointData *> b_jdlist;
    for ( int i = 0; i < jdlist.count() - 1; i++ ) {
        JointData *j1 = jdlist.at(i);
        JointData *j2 = jdlist.at(i + 1);
        QList<JointData *> ap_jdlist = searchBetweenJoints(j1, j2);
        ap_jdlist.prepend(j1);
        b_jdlist.append(ap_jdlist);
    }
    b_jdlist.append( jdlist.last() );
    return b_jdlist;
}

QList<JointData *> InputGrScene::searchBetweenPoints(QPointF p1, QPointF p2)
{
    QList<JointData *> jdlist;
    QList<qreal> dxlist;
    qreal eps = 1.0e-3;
    Q_FOREACH ( JointItem *item, jointAndPoint.keys() ) {
        if ( qAbs(item->zValue()) > eps ) continue;
        qreal dx = CalcVector2D::length(item->pos() - p1);
        if ( dx < eps || CalcVector2D::length(item->pos() - p2) < eps ) continue;
        bool online = CalcVector2D::between(p1, p2, item->pos());
        if ( !online ) continue;
        int idx = dxlist.count();
        for ( int i = 0; i < dxlist.count() ; i++) {
            if ( dx < dxlist.at(i) ) {
                idx = i;
                break;
            }
        }
        jdlist.insert(idx, jointAndPoint.value(item));
        dxlist.insert(idx, dx);
    }
    return jdlist;
}

bool InputGrScene::searchDivideMemberPoint( QPointF pp, QPointF &nearly_point, qreal &min_len,
                                                 bool equallyChecked, int partsCount,
                                                 bool fromIJChecked, int fromI, int fromJ )
{
    bool detected = false;

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHColumn.keys() ) {

        if ( item->approximatelyContains(pp) && !item->isIncompleteItem() ) {

            QList<QPointF> points;
            if ( equallyChecked ) points.append( item->divideEquallyPoints(partsCount) );
            if ( fromIJChecked )  points.append( item->divideFromIJPoints( qreal(fromI), qreal(fromJ) ) );

            Q_FOREACH (QPointF mp, points) {
                qreal len = (pp - mp).manhattanLength();
                if (len < min_len) {
                    min_len = len;
                    detected = true;
                    nearly_point = mp;
                }
            }

        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHGirder.keys() ) {

        if ( item->approximatelyContains(pp) && !item->isIncompleteItem() ) {

            QList<QPointF> points;
            if ( equallyChecked ) points.append( item->divideEquallyPoints(partsCount) );
            if ( fromIJChecked )  points.append( item->divideFromIJPoints(fromI, fromJ) );

            Q_FOREACH (QPointF mp, points) {
                qreal len = (pp - mp).manhattanLength();
                if (len < min_len) {
                    min_len = len;
                    detected = true;
                    nearly_point = mp;
                }
            }

        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHBrace.keys() ) {

        if ( item->approximatelyContains(pp) && !item->isIncompleteItem() ) {

            QList<QPointF> points;
            if ( equallyChecked ) points.append( item->divideEquallyPoints(partsCount) );
            if ( fromIJChecked )  points.append( item->divideFromIJPoints(fromI, fromJ) );

            Q_FOREACH (QPointF mp, points) {
                qreal len = (pp - mp).manhattanLength();
                if (len < min_len) {
                    min_len = len;
                    detected = true;
                    nearly_point = mp;
                }
            }

        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHBeam.keys() ) {

        if ( item->approximatelyContains(pp) && !item->isIncompleteItem() ) {

            QList<QPointF> points;
            if ( equallyChecked ) points.append( item->divideEquallyPoints(partsCount) );
            if ( fromIJChecked )  points.append( item->divideFromIJPoints(fromI, fromJ) );

            Q_FOREACH (QPointF mp, points) {
                qreal len = (pp - mp).manhattanLength();
                if (len < min_len) {
                    min_len = len;
                    detected = true;
                    nearly_point = mp;
                }
            }

        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHDamper.keys() ) {

        if ( item->approximatelyContains(pp) && !item->isIncompleteItem() ) {

            QList<QPointF> points;
            if ( equallyChecked ) points.append( item->divideEquallyPoints(partsCount) );
            if ( fromIJChecked )  points.append( item->divideFromIJPoints(fromI, fromJ) );

            Q_FOREACH (QPointF mp, points) {
                qreal len = (pp - mp).manhattanLength();
                if (len < min_len) {
                    min_len = len;
                    detected = true;
                    nearly_point = mp;
                }
            }

        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHIsolator.keys() ) {

        if ( item->approximatelyContains(pp) && !item->isIncompleteItem() ) {

            QList<QPointF> points;
            if ( equallyChecked ) points.append( item->divideEquallyPoints(partsCount) );
            if ( fromIJChecked )  points.append( item->divideFromIJPoints(fromI, fromJ) );

            Q_FOREACH (QPointF mp, points) {
                qreal len = (pp - mp).manhattanLength();
                if (len < min_len) {
                    min_len = len;
                    detected = true;
                    nearly_point = mp;
                }
            }

        }
    }

    return detected;
}

QList<MemberData *> InputGrScene::searchContainMembers(JointData *jd, bool exceptJoint)
{
    if ( !jointAndPoint.values().contains(jd) ) return QList<MemberData *>();
    return this->searchContainMembers( jointAndPoint.key(jd)->pos(), exceptJoint );
}

QList<MemberData *> InputGrScene::searchContainMembers(QPointF pp, bool exceptJoint)
{
    QList<MemberData *> mdlist;

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHColumn.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndHColumn.value(item));
        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHGirder.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndHGirder.value(item));
        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHBrace.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndHBrace.value(item));
        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHBeam.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndHBeam.value(item));
        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHDamper.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndHDamper.value(item));
        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndHIsolator.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndHIsolator.value(item));
        }
    }

    Q_FOREACH ( CustomDrawHLineItem *item, memberAndMemberLoad.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndMemberLoad.value(item));
        }
    }

    /* スラブ、壁の周囲線  */

    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHSlab.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndHSlab.value(item));
        }
    }

    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVSlab.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndVSlab.value(item));
        }
    }

    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHWall.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndHWall.value(item));
        }
    }

    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVWall.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndVWall.value(item));
        }
    }

    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndHUWall.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndHUWall.value(item));
        }
    }

    Q_FOREACH ( CustomDrawVPlaneItem *item, memberAndVUWall.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndVUWall.value(item));
        }
    }

    Q_FOREACH ( CustomDrawHPlaneItem *item, memberAndSlabLoad.keys() ) {
        if ( item->approximatelyContains(pp) ) {
            if ( item->isOnLinePoint(pp, exceptJoint) ) mdlist.append(memberAndSlabLoad.value(item));
        }
    }

    return mdlist;
}

void InputGrScene::setItemSelectable(DATATYPE type, bool ok)
{
    bool on_jot = false;
    bool on_col = false;
    bool on_gir = false;
    bool on_bra = false;
    bool on_bea = false;
    bool on_slab = false;
    bool on_wall = false;
    bool on_uwall = false;
    bool on_damp = false;
    bool on_iso = false;
    bool on_jointload = false;
    bool on_memberload = false;
    bool on_slabload = false;

    if (type == DATATYPE::TPJOINT) {
        on_jot = ok;
    } else if (type == DATATYPE::TPCOLUMN) {
        on_col = ok;
    } else if (type == DATATYPE::TPGIRDER) {
        on_gir = ok;
    } else if (type == DATATYPE::TPBRACE) {
        on_bra = ok;
    } else if (type == DATATYPE::TPBEAM) {
        on_bea = ok;
    } else if (type == DATATYPE::TPSLAB) {
        on_slab = ok;
    } else if (type == DATATYPE::TPWALL) {
        on_wall = ok;
    } else if (type == DATATYPE::TPUWALL) {
        on_uwall = ok;
    } else if (type == DATATYPE::TPDAMPER) {
        on_damp = ok;
    } else if (type == DATATYPE::TPISO) {
        on_iso = ok;
    } else if (type == DATATYPE::TPJOINTLOAD) {
        on_jointload = ok;
    } else if (type == DATATYPE::TPMEMBERLOAD) {
        on_memberload = ok;
    } else if (type == DATATYPE::TPSLABLOAD) {
        on_slabload = ok;
    }

    Q_FOREACH (JointItem *item, jointAndPoint.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_jot);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHGirder.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_gir);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVGirder.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_gir);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHColumn.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_col);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVColumn.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_col);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHBrace.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_bra);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVBrace.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_bra);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHBeam.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_bea);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVBeam.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_bea);
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndHSlab.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_slab);
    Q_FOREACH (CustomDrawVPlaneItem *item, memberAndVSlab.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_slab);
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndHWall.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_wall);
    Q_FOREACH (CustomDrawVPlaneItem *item, memberAndVWall.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_wall);
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndHUWall.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_uwall);
    Q_FOREACH (CustomDrawVPlaneItem *item, memberAndVUWall.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_uwall);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHDamper.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_damp);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVDamper.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_damp);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHIsolator.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_iso);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVIsolator.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_iso);
    Q_FOREACH (JointLoadItem *item, memberAndJointLoad.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_jointload);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndMemberLoad.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_memberload);
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndSlabLoad.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_slabload);
}

void InputGrScene::setInputItemSelectable(DATATYPE type)
{
    bool on_jot = false;
    bool on_col = false;
    bool on_gir = false;
    bool on_bra = false;
    bool on_bea = false;
    bool on_slab = false;
    bool on_wall = false;
    bool on_uwall = false;
    bool on_damp = false;
    bool on_iso = false;
    bool on_jointload = false;
    bool on_memberload = false;
    bool on_slabload = false;

    if (type == DATATYPE::TPJOINTLOAD) {
        on_jot = true;
    } else if (type == DATATYPE::TPMEMBERLOAD) {
        on_col = true;
        on_gir = true;
        on_bra = true;
        on_bea = true;
        on_damp = true;
        on_iso = true;
    } else if (type == DATATYPE::TPSLABLOAD) {
        on_slab = true;
    }

    Q_FOREACH (JointItem *item, jointAndPoint.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_jot);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHGirder.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_gir);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVGirder.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHColumn.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_col);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVColumn.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHBrace.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_bra);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVBrace.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHBeam.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_bea);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVBeam.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndHSlab.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_slab);
    Q_FOREACH (CustomDrawVPlaneItem *item, memberAndVSlab.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndHWall.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_wall);
    Q_FOREACH (CustomDrawVPlaneItem *item, memberAndVWall.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndHUWall.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_uwall);
    Q_FOREACH (CustomDrawVPlaneItem *item, memberAndVUWall.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHDamper.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_damp);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVDamper.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHIsolator.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_iso);
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVIsolator.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    Q_FOREACH (JointLoadItem *item, memberAndJointLoad.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_jointload);
    Q_FOREACH (CustomDrawHLineItem *item, memberAndMemberLoad.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_memberload);
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndSlabLoad.keys())
        item->setFlag(QGraphicsItem::ItemIsSelectable, on_slabload);
}

void InputGrScene::setItemSelectLimited(LIMITSELECTTERM selectTerm)
{
    // 節点　選択
    bool on_jot = false;
    if ( !selectTerm.isLimitDataType ) {
        on_jot = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPJOINT) ) {
        on_jot = true;
    }
    if ( on_jot && selectTerm.isLimitName ) on_jot = false;
    Q_FOREACH (JointItem *item, jointAndPoint.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_jot;
        if ( selectOn ) selectOn = isSelectableJoint(jointAndPoint.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // 大梁　選択
    bool on_gir = false;
    if ( !selectTerm.isLimitDataType ) {
        on_gir = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPGIRDER) ) {
        on_gir = true;
    }
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHGirder.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_gir;
        if ( selectOn ) selectOn = isSelectableMember(memberAndHGirder.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVGirder.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_gir;
        if ( selectOn ) selectOn = isSelectableMember(memberAndVGirder.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // 柱　選択
    bool on_col = false;
    if ( !selectTerm.isLimitDataType ) {
        on_col = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPCOLUMN) ) {
        on_col = true;
    }
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHColumn.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_col;
        if ( selectOn ) selectOn = isSelectableMember(memberAndHColumn.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVColumn.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_col;
        if ( selectOn ) selectOn = isSelectableMember(memberAndVColumn.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // ブレース　選択
    bool on_bra = false;
    if ( !selectTerm.isLimitDataType ) {
        on_bra = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPBRACE) ) {
        on_bra = true;
    }
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHBrace.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_bra;
        if ( selectOn ) selectOn = isSelectableMember(memberAndHBrace.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVBrace.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_bra;
        if ( selectOn ) selectOn = isSelectableMember(memberAndVBrace.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // 小梁　選択
    bool on_bea = false;
    if ( !selectTerm.isLimitDataType ) {
        on_bea = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPBEAM) ) {
        on_bea = true;
    }
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHBeam.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_bea;
        if ( selectOn ) selectOn = isSelectableMember(memberAndHBeam.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVBeam.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_bea;
        if ( selectOn ) selectOn = isSelectableMember(memberAndVBeam.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // スラブ　選択
    bool on_slab = false;
    if ( !selectTerm.isLimitDataType ) {
        on_slab = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPSLAB) ) {
        on_slab = true;
    }
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndHSlab.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_slab;
        if ( selectOn ) selectOn = isSelectableMember(memberAndHSlab.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
    Q_FOREACH (CustomDrawVPlaneItem *item, memberAndVSlab.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_slab;
        if ( selectOn ) selectOn = isSelectableMember(memberAndVSlab.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // 壁　選択
    bool on_wall = false;
    if ( !selectTerm.isLimitDataType ) {
        on_wall = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPWALL) ) {
        on_wall = true;
    }
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndHWall.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_wall;
        if ( selectOn ) selectOn = isSelectableMember(memberAndHWall.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
    Q_FOREACH (CustomDrawVPlaneItem *item, memberAndVWall.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_wall;
        if ( selectOn ) selectOn = isSelectableMember(memberAndVWall.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // 地下外壁　選択
    bool on_uwall = false;
    if ( !selectTerm.isLimitDataType ) {
        on_uwall = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPUWALL) ) {
        on_uwall = true;
    }
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndHUWall.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_uwall;
        if ( selectOn ) selectOn = isSelectableMember(memberAndHUWall.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
    Q_FOREACH (CustomDrawVPlaneItem *item, memberAndVUWall.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_uwall;
        if ( selectOn ) selectOn = isSelectableMember(memberAndVUWall.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // 制振要素　選択
    bool on_damp = false;
    if ( !selectTerm.isLimitDataType ) {
        on_damp = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPDAMPER) ) {
        on_damp = true;
    }
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHDamper.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_damp;
        if ( selectOn ) selectOn = isSelectableMember(memberAndHDamper.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVDamper.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_damp;
        if ( selectOn ) selectOn = isSelectableMember(memberAndVDamper.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // 免震要素　選択
    bool on_iso = false;
    if ( !selectTerm.isLimitDataType ) {
        on_iso = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPISO) ) {
        on_iso = true;
    }
    Q_FOREACH (CustomDrawHLineItem *item, memberAndHIsolator.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_iso;
        if ( selectOn ) selectOn = isSelectableMember(memberAndHIsolator.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
    Q_FOREACH (CustomDrawVLineItem *item, memberAndVIsolator.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_iso;
        if ( selectOn ) selectOn = isSelectableMember(memberAndVIsolator.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // 節点荷重　選択
    bool on_jointload = false;
    if ( !selectTerm.isLimitDataType ) {
        on_jointload = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPJOINTLOAD) ) {
        on_jointload = true;
    }
    Q_FOREACH (JointLoadItem *item, memberAndJointLoad.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_jointload;
        if ( selectOn ) selectOn = isSelectableMember(memberAndJointLoad.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }

    // 部材荷重　選択
    bool on_memberload = false;
    if ( !selectTerm.isLimitDataType ) {
        on_memberload = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPMEMBERLOAD) ) {
        on_memberload = true;
    }
    Q_FOREACH (CustomDrawHLineItem *item, memberAndMemberLoad.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_memberload;
        if ( selectOn ) selectOn = isSelectableMember(memberAndMemberLoad.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }


    // 部材荷重　選択
    bool on_slabload = false;
    if ( !selectTerm.isLimitDataType ) {
        on_slabload = true;
    } else if ( selectTerm.limitedDataTypes.contains(DATATYPE::TPSLABLOAD) ) {
        on_slabload = true;
    }
    Q_FOREACH (CustomDrawHPlaneItem *item, memberAndSlabLoad.keys()) {
        if ( !selectTerm.isSelectLimited ) {
            item->setFlag(QGraphicsItem::ItemIsSelectable, true);
            continue;
        }
        bool selectOn = on_slabload;
        if ( selectOn ) selectOn = isSelectableMember(memberAndSlabLoad.value(item), selectTerm);
        item->setFlag(QGraphicsItem::ItemIsSelectable, selectOn);
    }
}

bool InputGrScene::isSelectableJoint(JointData *jd, const LIMITSELECTTERM &selectTerm)
{
    if ( selectTerm.isLimitFloor ) {
        bool isOk = false;
        Q_FOREACH ( QUuid id, jd->getAttachedFloorList() ) {
            if ( selectTerm.limitedFloors.contains(id) ) {
                isOk = true;
                break;
            }
        }
        if ( !isOk ) return false;
    }
    if ( selectTerm.isLimitFrame ) {
        bool isOk = false;
        Q_FOREACH ( QUuid id, jd->getAttachedFrameList() ) {
            if ( selectTerm.limitedFrames.contains(id) ) {
                isOk = true;
                break;
            }
        }
        if ( !isOk ) return false;
    }
    return true;
}

bool InputGrScene::isSelectableMember(MemberData *md, const LIMITSELECTTERM &selectTerm)
{
    if ( selectTerm.isLimitName ) {
        if ( !selectTerm.limitedNames.contains(md->sectionName()) ) return false;
    }
    if ( selectTerm.isLimitFloor ) {
        if ( selectTerm.isFloorContained ) {
            if ( !md->containsFloors(selectTerm.limitedFloors) ) return false;
        } else {
            if ( !md->touchFloors(selectTerm.limitedFloors) ) return false;
        }
    }
    if ( selectTerm.isLimitFrame ) {
        if ( selectTerm.isFrameContained ) {
            if ( !md->containsFrames(selectTerm.limitedFrames) ) return false;
        } else {
            if ( !md->touchFrames(selectTerm.limitedFrames) ) return false;
        }
    }
    return true;
}

// 全編集時、Selectable=false であることが前提
void InputGrScene::setMemberSelectLimited( const QList<MemberData *> &mdlist )
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( myselectTerm.isSelectLimited ) {
            if ( myselectTerm.isLimitDataType &&
                    !myselectTerm.limitedDataTypes.contains(md->dataType()) ) continue;
            if ( !isSelectableMember(md, myselectTerm) ) continue;
        }
        switch ( md->dataType() ) {
        case DATATYPE::TPCOLUMN:
            if ( memberAndHColumn.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHColumn.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            if ( memberAndVColumn.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVColumn.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            /*
                        if( memberAndOffVColumn.values().contains(md) ){
                            QList<CustomDrawVLineItem*> items=memberAndOffVColumn.keys(md);
                            Q_FOREACH(CustomDrawVLineItem* item, items)
                                item->setFlag(QGraphicsItem::ItemIsSelectable,true);
                            break;
                        }
            */
            break;
        case DATATYPE::TPGIRDER:
            if ( memberAndHGirder.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHGirder.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            if ( memberAndVGirder.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVGirder.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPBRACE:
            if ( memberAndHBrace.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHBrace.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            if ( memberAndVBrace.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVBrace.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPBEAM:
            if ( memberAndHBeam.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHBeam.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            if ( memberAndVBeam.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVBeam.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPSLAB:
            if ( memberAndHSlab.values().contains(md) ) {
                QList<CustomDrawHPlaneItem *> items = memberAndHSlab.keys(md);
                Q_FOREACH (CustomDrawHPlaneItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            if ( memberAndVSlab.values().contains(md) ) {
                QList<CustomDrawVPlaneItem *> items = memberAndVSlab.keys(md);
                Q_FOREACH (CustomDrawVPlaneItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPWALL:
            if ( memberAndHWall.values().contains(md) ) {
                QList<CustomDrawHPlaneItem *> items = memberAndHWall.keys(md);
                Q_FOREACH (CustomDrawHPlaneItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            if ( memberAndVWall.values().contains(md) ) {
                QList<CustomDrawVPlaneItem *> items = memberAndVWall.keys(md);
                Q_FOREACH (CustomDrawVPlaneItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPUWALL:
            if ( memberAndHUWall.values().contains(md) ) {
                QList<CustomDrawHPlaneItem *> items = memberAndHUWall.keys(md);
                Q_FOREACH (CustomDrawHPlaneItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            if ( memberAndVUWall.values().contains(md) ) {
                QList<CustomDrawVPlaneItem *> items = memberAndVUWall.keys(md);
                Q_FOREACH (CustomDrawVPlaneItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPDAMPER:
            if ( memberAndHDamper.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHDamper.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            if ( memberAndVDamper.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVDamper.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPISO:
            if ( memberAndHIsolator.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHIsolator.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            if ( memberAndVIsolator.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVIsolator.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPJOINTLOAD:
            if ( memberAndJointLoad.values().contains(md) ) {
                QList<JointLoadItem *> items = memberAndJointLoad.keys(md);
                Q_FOREACH (JointLoadItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPMEMBERLOAD:
            if ( memberAndMemberLoad.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndMemberLoad.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        case DATATYPE::TPSLABLOAD:
            if ( memberAndSlabLoad.values().contains(md) ) {
                QList<CustomDrawHPlaneItem *> items = memberAndSlabLoad.keys(md);
                Q_FOREACH (CustomDrawHPlaneItem *item, items)
                    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
                break;
            }
            break;
        default:
            break;
        }
    }
}

void InputGrScene::filterCreateMember( const QList<MemberData *> &mdlist )
{
    createMemberItems( filterSceneMember(mdlist) );
}

void InputGrScene::filterRemoveMember( const QList<MemberData *> &mdlist, DATATYPE type )
{
    this->clearSelection();
    removeMemberItems( mdlist, type );
}

void InputGrScene::filterChangeMember( const QList<MemberData *> &mdlist )
{
    this->clearSelection();
    changeMemberItems( mdlist );
}

void InputGrScene::slotSelectTermChanged(const LIMITSELECTTERM& term)
{
    myselectTerm = term;
}

void InputGrScene::slotOperationSettiingChanged(const OPERATIONSETTING& setting)
{
    myElementType = setting.elementType;
    myMode = setting.addEditMode;
}

void InputGrScene::removeJointItems( const QList<JointData *> &jdlist )
{
    batchInputPlanes.clear();
    this->clearSelection();
    Q_FOREACH (JointData *jd, jdlist) {
        if ( jointAndPoint.values().contains(jd) ) {
            QList<JointItem *> j_items = jointAndPoint.keys(jd);
            Q_FOREACH ( JointItem *item, j_items ) {
                this->removeItem(item);
                jointAndPoint.remove(item);
                delete item;
            }
        }
    }

    Q_FOREACH (JointItem *item, jointAndPoint.keys()) {
        JointData *jd = jointAndPoint.value(item);
        int ino = UnifiedInputData::getInInstance()->indexOfJoint(jd) + 1;
        item->setNodeNumber(ino);
    }
}

void InputGrScene::changeJointItems( const QList<JointData *> &jdlist )
{
    removeJointItems(jdlist);
    createJointItems(jdlist);
}

void InputGrScene::removeMemberItems( const QList<MemberData *> &mdlist, DATATYPE type )
{
    Q_FOREACH (MemberData *md, mdlist) removeMemberItem(md, type);
//    this->update( itemsBoundingRect() );
}

void InputGrScene::removeMemberItem( MemberData *md, DATATYPE type )
{
    // MemberData*はdelete後のため、メンバー関数使えない
    switch ( type ) {
    case DATATYPE::TPCOLUMN:
        if ( memberAndHColumn.values().contains(md) ) {
            QList<CustomDrawHLineItem *> items = memberAndHColumn.keys(md);
            Q_FOREACH (CustomDrawHLineItem *item, items) {
                this->removeItem(item);
                memberAndHColumn.remove(item);
                delete item;
            }
            break;
        }
        if ( memberAndVColumn.values().contains(md) ) {
            QList<CustomDrawVLineItem *> items = memberAndVColumn.keys(md);
            Q_FOREACH (CustomDrawVLineItem *item, items) {
                this->removeItem(item);
                memberAndVColumn.remove(item);
                delete item;
            }
            break;
        }
        if ( memberAndOffVColumn.values().contains(md) ) {
            QList<CustomDrawVLineItem *> items = memberAndOffVColumn.keys(md);
            Q_FOREACH (CustomDrawVLineItem *item, items) {
                this->removeItem(item);
                memberAndOffVColumn.remove(item);
                delete item;
            }
            break;
        }
        break;
    case DATATYPE::TPGIRDER:
        if ( memberAndHGirder.values().contains(md) ) {
            QList<CustomDrawHLineItem *> items = memberAndHGirder.keys(md);
            Q_FOREACH (CustomDrawHLineItem *item, items) {
                memberAndHGirder.remove(item);
                this->removeItem(item);
                delete item;
            }
            break;
        }
        if ( memberAndVGirder.values().contains(md) ) {
            QList<CustomDrawVLineItem *> items = memberAndVGirder.keys(md);
            Q_FOREACH (CustomDrawVLineItem *item, items) {
                this->removeItem(item);
                memberAndVGirder.remove(item);
                delete item;
            }
            break;
        }
        break;
    case DATATYPE::TPBRACE:
        if ( memberAndHBrace.values().contains(md) ) {
            QList<CustomDrawHLineItem *> items = memberAndHBrace.keys(md);
            Q_FOREACH (CustomDrawHLineItem *item, items) {
                memberAndHBrace.remove(item);
                this->removeItem(item);
                delete item;
            }
            break;
        }
        if ( memberAndVBrace.values().contains(md) ) {
            QList<CustomDrawVLineItem *> items = memberAndVBrace.keys(md);
            Q_FOREACH (CustomDrawVLineItem *item, items) {
                this->removeItem(item);
                memberAndVBrace.remove(item);
                delete item;
            }
            break;
        }
        break;
    case DATATYPE::TPBEAM:
        if ( memberAndHBeam.values().contains(md) ) {
            QList<CustomDrawHLineItem *> items = memberAndHBeam.keys(md);
            Q_FOREACH (CustomDrawHLineItem *item, items) {
                memberAndHBeam.remove(item);
                this->removeItem(item);
                delete item;
            }
            break;
        }
        if ( memberAndVBeam.values().contains(md) ) {
            QList<CustomDrawVLineItem *> items = memberAndVBeam.keys(md);
            Q_FOREACH (CustomDrawVLineItem *item, items) {
                this->removeItem(item);
                memberAndVBeam.remove(item);
                delete item;
            }
            break;
        }
        break;
    case DATATYPE::TPSLAB:
        if ( memberAndHSlab.values().contains(md) ) {
            QList<CustomDrawHPlaneItem *> items = memberAndHSlab.keys(md);
            Q_FOREACH (CustomDrawHPlaneItem *item, items) {
                memberAndHSlab.remove(item);
                this->removeItem(item);
                delete item;
            }
            break;
        }
        if ( memberAndVSlab.values().contains(md) ) {
            QList<CustomDrawVPlaneItem *> items = memberAndVSlab.keys(md);
            Q_FOREACH (CustomDrawVPlaneItem *item, items) {
                this->removeItem(item);
                memberAndVSlab.remove(item);
                delete item;
            }
            break;
        }
        break;
    case DATATYPE::TPWALL:
        if ( memberAndHWall.values().contains(md) ) {
            QList<CustomDrawHPlaneItem *> items = memberAndHWall.keys(md);
            Q_FOREACH (CustomDrawHPlaneItem *item, items) {
                memberAndHWall.remove(item);
                this->removeItem(item);
                delete item;
            }
            break;
        }
        if ( memberAndVWall.values().contains(md) ) {
            QList<CustomDrawVPlaneItem *> items = memberAndVWall.keys(md);
            Q_FOREACH (CustomDrawVPlaneItem *item, items) {
                this->removeItem(item);
                memberAndVWall.remove(item);
                delete item;
            }
            break;
        }
        break;
    case DATATYPE::TPUWALL:
        if ( memberAndHUWall.values().contains(md) ) {
            QList<CustomDrawHPlaneItem *> items = memberAndHUWall.keys(md);
            Q_FOREACH (CustomDrawHPlaneItem *item, items) {
                memberAndHUWall.remove(item);
                this->removeItem(item);
                delete item;
            }
            break;
        }
        if ( memberAndVUWall.values().contains(md) ) {
            QList<CustomDrawVPlaneItem *> items = memberAndVUWall.keys(md);
            Q_FOREACH (CustomDrawVPlaneItem *item, items) {
                this->removeItem(item);
                memberAndVUWall.remove(item);
                delete item;
            }
            break;
        }
        break;
    case DATATYPE::TPDAMPER:
        if ( memberAndHDamper.values().contains(md) ) {
            QList<CustomDrawHLineItem *> items = memberAndHDamper.keys(md);
            Q_FOREACH (CustomDrawHLineItem *item, items) {
                memberAndHDamper.remove(item);
                this->removeItem(item);
                delete item;
            }
            break;
        }
        if ( memberAndVDamper.values().contains(md) ) {
            QList<CustomDrawVLineItem *> items = memberAndVDamper.keys(md);
            Q_FOREACH (CustomDrawVLineItem *item, items) {
                this->removeItem(item);
                memberAndVDamper.remove(item);
                delete item;
            }
            break;
        }
        break;
    case DATATYPE::TPISO:
        if ( memberAndHIsolator.values().contains(md) ) {
            QList<CustomDrawHLineItem *> items = memberAndHIsolator.keys(md);
            Q_FOREACH (CustomDrawHLineItem *item, items) {
                memberAndHIsolator.remove(item);
                this->removeItem(item);
                delete item;
            }
            break;
        }
        if ( memberAndVIsolator.values().contains(md) ) {
            QList<CustomDrawVLineItem *> items = memberAndVIsolator.keys(md);
            Q_FOREACH (CustomDrawVLineItem *item, items) {
                this->removeItem(item);
                memberAndVIsolator.remove(item);
                delete item;
            }
            break;
        }
        break;
    case DATATYPE::TPJOINTLOAD:
        if ( memberAndJointLoad.values().contains(md) ) {
            QList<JointLoadItem *> items = memberAndJointLoad.keys(md);
            Q_FOREACH (JointLoadItem *item, items) {
                memberAndJointLoad.remove(item);
                this->removeItem(item);
                delete item;
            }
        }
        break;
    case DATATYPE::TPMEMBERLOAD:
        if ( memberAndMemberLoad.values().contains(md) ) {
            QList<CustomDrawHLineItem *> items = memberAndMemberLoad.keys(md);
            Q_FOREACH (CustomDrawHLineItem *item, items) {
                memberAndMemberLoad.remove(item);
                this->removeItem(item);
                delete item;
            }
        }
        break;
    case DATATYPE::TPSLABLOAD:
        if ( memberAndSlabLoad.values().contains(md) ) {
            QList<CustomDrawHPlaneItem *> items = memberAndSlabLoad.keys(md);
            Q_FOREACH (CustomDrawHPlaneItem *item, items) {
                memberAndSlabLoad.remove(item);
                this->removeItem(item);
                delete item;
            }
        }
        break;
    default:
        break;
    }
}

//void InputGrScene::sendSelectedItems()
//{
//    if ( selectedItems().count() == 0)
//        return;
//    emit sendSelectedItems2();
//}

JointItem *InputGrScene::jointDataToJointItem(JointData *jd, bool isFirst) const
{
    QList<JointItem *> items = jointAndPoint.keys(jd);
    if ( items.count() == 0 ) {
        return NULL;
    } else if ( items.count() == 1 ) {
        return items.first();
    } else {
        Q_FOREACH ( JointItem *item, items ) {
            if ( isFirst && !item->isCopyedJoint() ) return item;
            if ( !isFirst && item->isCopyedJoint() ) return item;
        }
        return NULL;
    }
    return NULL;
}

JointItem *InputGrScene::jointDataToJointItem(JointData *jd, QPointF pp) const
{
    QList<JointItem *> items = jointAndPoint.keys(jd);

    if ( items.count() == 0 ) {
        return NULL;
    } else if ( items.count() == 1 ) {
        return items.first();
    } else {
        qreal len1 = (items.first()->pos() - pp).manhattanLength();
        qreal len2 = (items.last()->pos() - pp).manhattanLength();
        return ( len1 <= len2 ) ? items.first() : items.last();
    }
    return NULL;
}

QPointF InputGrScene::itemPosition(JointData *jd) const
{
    JointItem *j_item = jointDataToJointItem(jd, true);
    return ( jd != NULL ) ? j_item->pos() : QPointF();
}

QPointF InputGrScene::itemPosition(JointData *jd, QPointF pp) const
{
    JointItem *j_item = jointDataToJointItem(jd, pp);
    return ( jd != NULL ) ? j_item->pos() : QPointF();
}

JointData *InputGrScene::jointLoadItemToJointData(JointLoadItem *item) const
{
    if ( !memberAndJointLoad.contains(item) ) return NULL;
    MemberData *md = memberAndJointLoad.value(item);
    return md->getJointList().first();
}

QList<QPointF> InputGrScene::itemToDividedPointList(CustomDrawItem *item,
                                                         ITEMDIVRULE rule, qreal r1, qreal r2) const
{
    if ( item->itemType() != ITEMTYPE::HLINEITEM ) return QList<QPointF>();

    CustomDrawHLineItem *hitem = qgraphicsitem_cast<CustomDrawHLineItem *>(item);
    QList<QPointF> divPoints;

    if ( rule == ITEMDIVRULE::IDVEQUAL ) {
        divPoints = hitem->divideEquallyPoints(qRound(r1));
    } else if ( rule == ITEMDIVRULE::IDVRATIO ) {
        divPoints.append( hitem->divideRatioPoint(r1, r2) );
    } else if ( rule == ITEMDIVRULE::IDVILEN ) {
        divPoints.append( hitem->divideFromIPoint(r1) );
    } else if ( rule == ITEMDIVRULE::IDVJLEN ) {
        divPoints.append( hitem->divideFromJPoint(r1) );
    }

    return divPoints;
}

CustomDrawHLineItem *InputGrScene::searchNearlyHItem(QPointF pp, qreal min_len) const
{
    CustomDrawHLineItem *nearly_item = NULL;

    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHColumn.keys()) {
        qreal len = item->distanceToPoint(pp);
        if ( len < min_len ) {
            nearly_item = item;
            min_len = len;
        }
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHGirder.keys()) {
        qreal len = item->distanceToPoint(pp);
        if ( len < min_len ) {
            nearly_item = item;
            min_len = len;
        }
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHBrace.keys()) {
        qreal len = item->distanceToPoint(pp);
        if ( len < min_len ) {
            nearly_item = item;
            min_len = len;
        }
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHBeam.keys()) {
        qreal len = item->distanceToPoint(pp);
        if ( len < min_len ) {
            nearly_item = item;
            min_len = len;
        }
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHDamper.keys()) {
        qreal len = item->distanceToPoint(pp);
        if ( len < min_len ) {
            nearly_item = item;
            min_len = len;
        }
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHIsolator.keys()) {
        qreal len = item->distanceToPoint(pp);
        if ( len < min_len ) {
            nearly_item = item;
            min_len = len;
        }
    }
    return nearly_item;
}

QList<QPointF> InputGrScene::searchCrossPointsToLine(const QLineF &l1) const
{
    QList<QPointF> crossPoints;

    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHColumn.keys()) {
        QList<QLineF> lines = item->getItemLines();
        Q_FOREACH (QLineF l2, lines) crossPoints.append(CalcVector2D::intersectSegmentToLine(l1, l2));
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHGirder.keys()) {
        QList<QLineF> lines = item->getItemLines();
        Q_FOREACH (QLineF l2, lines) crossPoints.append(CalcVector2D::intersectSegmentToLine(l1, l2));
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHBrace.keys()) {
        QList<QLineF> lines = item->getItemLines();
        Q_FOREACH (QLineF l2, lines) crossPoints.append(CalcVector2D::intersectSegmentToLine(l1, l2));
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHBeam.keys()) {
        QList<QLineF> lines = item->getItemLines();
        Q_FOREACH (QLineF l2, lines) crossPoints.append(CalcVector2D::intersectSegmentToLine(l1, l2));
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHDamper.keys()) {
        QList<QLineF> lines = item->getItemLines();
        Q_FOREACH (QLineF l2, lines) crossPoints.append(CalcVector2D::intersectSegmentToLine(l1, l2));
    }
    Q_FOREACH (CustomDrawHLineItem *item,  memberAndHIsolator.keys()) {
        QList<QLineF> lines = item->getItemLines();
        Q_FOREACH (QLineF l2, lines) crossPoints.append(CalcVector2D::intersectSegmentToLine(l1, l2));
    }

    return crossPoints;
}

QPolygonF InputGrScene::searchContainPlane(QPointF pp) const
{
    Q_FOREACH ( QList<JointItem *> items, batchInputPlanes ) {
        QPolygonF polygon;
        Q_FOREACH ( JointItem *item, items ) polygon.append(item->pos());
        if ( polygon.containsPoint(pp, Qt::WindingFill) ) return polygon;
    }
    return QPolygonF();
}

QList<JointData *> InputGrScene::polygonToBatchJoints(const QPolygonF poly) const
{
    QList<JointData *> joints;
    Q_FOREACH ( QList<JointItem *> items, batchInputPlanes ) {
        if ( items.count() != poly.count() ) continue;
        bool ok = true;
        for ( int i = 0; i < poly.count() ; i++ ) {
            if ( (poly.at(i) - items.at(i)->pos()).manhattanLength() > 1.0e-3 ) {
                ok = false;
                break;
            }
        }
        if ( ok ) {
            Q_FOREACH ( JointItem *item, items ) joints.append( jointAndPoint.value(item) );
            break;
        }
    }
    return joints;
}

void InputGrScene::setJointSelect( const QList<JointData *> &jdlist )
{
    Q_FOREACH ( JointData *jd, jdlist ) {
        if ( jointAndPoint.values().contains(jd) ) {
            QList<JointItem *> items = jointAndPoint.keys(jd);
            Q_FOREACH (JointItem *item, items)
                item->setSelected(true);
        }
    }
}

void InputGrScene::setMemberSelect( const QList<MemberData *> &mdlist )
{
    Q_FOREACH ( MemberData *md, mdlist ) {
        switch ( md->dataType() ) {
        case DATATYPE::TPCOLUMN:
            if ( memberAndHColumn.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHColumn.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setSelected(true);

            }
            if ( memberAndVColumn.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVColumn.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPGIRDER:
            if ( memberAndHGirder.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHGirder.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setSelected(true);
            }
            if ( memberAndVGirder.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVGirder.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPBRACE:
            if ( memberAndHBrace.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHBrace.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setSelected(true);
            }
            if ( memberAndVBrace.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVBrace.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPBEAM:
            if ( memberAndHBeam.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHBeam.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setSelected(true);
            }
            if ( memberAndVBeam.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVBeam.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPSLAB:
            if ( memberAndHSlab.values().contains(md) ) {
                QList<CustomDrawHPlaneItem *> items = memberAndHSlab.keys(md);
                Q_FOREACH (CustomDrawHPlaneItem *item, items)
                    item->setSelected(true);
            }
            if ( memberAndVSlab.values().contains(md) ) {
                QList<CustomDrawVPlaneItem *> items = memberAndVSlab.keys(md);
                Q_FOREACH (CustomDrawVPlaneItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPWALL:
            if ( memberAndHWall.values().contains(md) ) {
                QList<CustomDrawHPlaneItem *> items = memberAndHWall.keys(md);
                Q_FOREACH (CustomDrawHPlaneItem *item, items)
                    item->setSelected(true);
            }
            if ( memberAndVWall.values().contains(md) ) {
                QList<CustomDrawVPlaneItem *> items = memberAndVWall.keys(md);
                Q_FOREACH (CustomDrawVPlaneItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPUWALL:
            if ( memberAndHUWall.values().contains(md) ) {
                QList<CustomDrawHPlaneItem *> items = memberAndHUWall.keys(md);
                Q_FOREACH (CustomDrawHPlaneItem *item, items)
                    item->setSelected(true);
            }
            if ( memberAndVUWall.values().contains(md) ) {
                QList<CustomDrawVPlaneItem *> items = memberAndVUWall.keys(md);
                Q_FOREACH (CustomDrawVPlaneItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPDAMPER:
            if ( memberAndHDamper.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHDamper.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setSelected(true);
            }
            if ( memberAndVDamper.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVDamper.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPISO:
            if ( memberAndHIsolator.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndHIsolator.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setSelected(true);
            }
            if ( memberAndVIsolator.values().contains(md) ) {
                QList<CustomDrawVLineItem *> items = memberAndVIsolator.keys(md);
                Q_FOREACH (CustomDrawVLineItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPJOINTLOAD:
            if ( memberAndJointLoad.values().contains(md) ) {
                QList<JointLoadItem *> items = memberAndJointLoad.keys(md);
                Q_FOREACH (JointLoadItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPMEMBERLOAD:
            if ( memberAndMemberLoad.values().contains(md) ) {
                QList<CustomDrawHLineItem *> items = memberAndMemberLoad.keys(md);
                Q_FOREACH (CustomDrawHLineItem *item, items)
                    item->setSelected(true);
            }
            break;
        case DATATYPE::TPSLABLOAD:
            if ( memberAndSlabLoad.values().contains(md) ) {
                QList<CustomDrawHPlaneItem *> items = memberAndSlabLoad.keys(md);
                Q_FOREACH (CustomDrawHPlaneItem *item, items)
                    item->setSelected(true);
            }
            break;
        default:
            break;
        }
    }
}
} // namespace post3dapp
