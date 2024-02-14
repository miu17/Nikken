#include "eventoperation_wall.h"

#include <QtGui>

#include "calc_vector2d.h"
#include "changeopen_dialog.h"
#include "changesection_dialog.h"
#include "changeshift_dialog.h"
#include "customdraw_grview.h"
#include "define_draw_figure.h"
#include "factory_unifieddata.h"
#include "finditem_dialog.h"
#include "input_grscene.h"
#include "manage_memberdata.h"
#include "manage_wallactions.h"
#include "operation_commands.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "unified_settings.h"

namespace post3dapp{
WallEventOperation::WallEventOperation(CustomDrawGrView *view)
    : AbstractEventOperation(view)
{
    qDebug()<<"init";

    hideRubberBand(false);
    setMoveWaitingOn(false);
    manageActions = new ManageWallActions(this);

    myElementType = UnifiedEditingData::getInstance()->currentElement();
    currentAddEditMode = UnifiedEditingData::getInstance()->currentAddEditMode();
    currentTarget = UnifiedEditingData::getInstance()->currentTargetAction();
    waitingJoints = JOINTGROUP();

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentStatusChanged, this, &WallEventOperation::reloadSettings);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentTargetActionChanged, this, &WallEventOperation::reloadSettings);
}

void WallEventOperation::reloadSettings()
{
    qDebug()<<"reload";
    ELEMENTTYPE old_element = myElementType;
    ADDEDITMODE old_mode = currentAddEditMode;
    TARGETACTION old_target = currentTarget;
    myElementType = UnifiedEditingData::getInstance()->currentElement();
    currentAddEditMode = UnifiedEditingData::getInstance()->currentAddEditMode();
    currentTarget = UnifiedEditingData::getInstance()->currentTargetAction();
    if ( UnifiedEditingData::getInstance()->elementGroup(myElementType) != ELGWALL ) return;
    if ( old_mode != currentAddEditMode || old_element != myElementType || old_target != currentTarget )
        initializeSettings();
}

void WallEventOperation::initializeSettings()
{
    qDebug()<<"initialize";
    previousHitGP.clear();
    setWaitingMode();
    refreshBatchPlanes();
    myGrScene->setItemSelectable( currentDataType(), currentAddEditMode == EDITMODE );
    if ( currentAddEditMode == SELECTHITMODE ) myGrScene->setInputItemSelectable( currentDataType() );
    manageActions->resetActions( currentAddEditMode == EDITMODE );

    if ( currentAddEditMode == ONEHITMODE || currentAddEditMode == BATCHMODE ) {
        myGrScene->prepareBatchInputPlanes( currentTarget == TARGETCGB );
    }
}

void WallEventOperation::slotChangeActionStatus()
{
    qDebug()<<"slotChangeActionstatus";

    if ( isEditMode() ) {
        manageActions->setActionStatus( currentDataType(), isMoveWaiting(),
                                        myGrScene->selectedItems().count() > 0,
                                        myGrScene->selectedItems().count() > 1,
                                        setMoveWaitingJoints(QPoint(-100, -100)) );
    }
}

void WallEventOperation::setWaitingMode()
{
    qDebug()<<"set waiting mode";

    myGrView->clearSelectedItems();
    myGrScene->clearCurrentAttach();
    hideRubberBand(false);
    setMoveWaitingOn(false);
    waitingJoints = JOINTGROUP();
//    qDebug() << "plane setWaitingMode";
}

void WallEventOperation::toggleViewMode()
{
    qDebug()<<"toggle view mode";

    setViewModeOn( !viewMode );
    if ( currentAddEditMode == POINTHITMODE ) myGrScene->clearCurrentAttach();
    if ( currentAddEditMode == BATCHMODE ) refreshBatchPlanes();
    if ( currentAddEditMode == SELECTHITMODE && viewMode ) setWaitingMode();
    if ( currentAddEditMode == EDITMODE && viewMode ) setWaitingMode();
//    if( isBeamInputMode() ) refreshBeamInputStatus();
}

bool WallEventOperation::isBatchInputMode() const
{
    return ( isBatchMode() && isMousePressed() );
}

bool WallEventOperation::isSelectItemMode() const
{
    if ( isSelectHitMode() && isMousePressed() ) return true;
    return ( isEditMode() && !isMoveWaiting() && isMousePressed() );
}

bool WallEventOperation::isPositionSnapOn() const
{
    return ( isPointHitMode() || isOneHitMode() || isMoveWaiting() );
}

bool WallEventOperation::isPointHitMode() const
{
//    if( currentAddEditMode==ONEHITMODE && myGrScene->sceneType()==ELEVATIONSCENE ) return false;
    return ( !viewMode && currentAddEditMode == POINTHITMODE );
}

bool WallEventOperation::isBatchMode() const
{
    return ( !viewMode && currentAddEditMode == BATCHMODE );
}

bool WallEventOperation::isSelectHitMode() const
{
    return ( !viewMode && myElementType == ELSLABLOAD && currentAddEditMode == SELECTHITMODE );
}

bool WallEventOperation::isOneHitMode() const
{
    return ( !viewMode && currentAddEditMode == ONEHITMODE );
}

bool WallEventOperation::isEditMode() const
{
    return ( !viewMode && currentAddEditMode == EDITMODE );
}

bool WallEventOperation::isMoveWaiting() const
{
    return onMoveWaiting ;
}

void WallEventOperation::setMoveWaitingOn(bool on)
{
    onMoveWaiting = on;
}

void WallEventOperation::execDrawForeground(QPainter *painter, const QRect &vrect)
{
    if ( !isPositionSnapOn() && !isBatchMode() ) return;

    painter->setTransform(QTransform(1, 0, 0, 1, 0, 0), false);
    qreal factor = qPow(myGrView->transform().m11(), 0.25);
    int dd = int(factor * 10.0);


    if ( isBatchMode() || isOneHitMode() ) {

        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setBrush(QBrush(QColor(255, 200, 200, 50)));
        painter->setPen(QColor(255, 50, 0));

        Q_FOREACH ( QPolygonF poly, batchPlanes ) {
            QPolygon polygon = myGrView->mapFromScene(poly);
            painter->drawPolygon(polygon);
        }

        if ( currentModifier == Qt::AltModifier ) {
            painter->setBrush(QBrush(QColor(200, 200, 255, 50)));
            painter->setPen(QColor(0, 50, 255));
        }

        Q_FOREACH ( QPolygonF poly, tmpBatchPlanes ) {
            QPolygon polygon = myGrView->mapFromScene(poly);
            painter->drawPolygon(polygon);
        }
        return;
    }


    AbstractEventOperation::execDrawForeground(painter, vrect);

    if ( isPointHitMode() && !previousHitGP.isEmpty() ) {

        for ( int i = 0; i < previousHitGP.count() - 1 ; i++ ) {
            QPoint pp = myGrView->mapFromScene(previousHitGP.at(i).getElvQPointF() * 1000.);
            QPoint qq = myGrView->mapFromScene(previousHitGP.at(i + 1).getElvQPointF() * 1000.);
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->setBrush(QBrush(QColor(255, 50, 0)));
            painter->drawEllipse(pp, dd, dd);
            painter->drawEllipse(qq, dd, dd);
            painter->setRenderHint(QPainter::Antialiasing, false);
            painter->setPen(QPen(QBrush(QColor(255, 50, 0)), 1, Qt::DashLine));
            painter->drawLine(pp, qq);
        }

        QPoint pp = myGrView->mapFromScene(previousHitGP.last().getElvQPointF() * 1000.);
        QPoint qq = myGrView->mapFromScene(currentGP.getElvQPointF() * 1000.);
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setBrush(QBrush(QColor(255, 50, 0)));
        painter->drawEllipse(pp, dd, dd);
        painter->drawEllipse(qq, dd, dd);
        painter->setRenderHint(QPainter::Antialiasing, false);
        painter->setPen(QPen(QBrush(QColor(255, 50, 0)), 1, Qt::DashLine));
        painter->drawLine(pp, qq);

    }

    if ( !isMoveWaiting() ) return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(QBrush(QColor(200, 255, 200)));
    painter->setPen(QPen(QBrush(QColor(0, 255, 50)), 1, Qt::DashLine));

    QPointF q1 = currentGP.getElvQPointF() * 1000.;
    QPoint p1 = myGrView->mapFromScene( q1 );
    painter->drawEllipse(p1, dd, dd);

    painter->setRenderHint(QPainter::Antialiasing, false);

    if ( waitingJoints.j1 != nullptr ) {
        QPoint p2 = myGrView->mapFromScene( myGrScene->itemPosition(waitingJoints.j1, q1) );
        painter->drawLine(p1, p2);
    }

    if ( waitingJoints.j3 != nullptr ) {
        QPoint p2 = myGrView->mapFromScene( myGrScene->itemPosition(waitingJoints.j3, q1) );
        painter->drawLine(p1, p2);
    }

}

void WallEventOperation::execMousePress(QMouseEvent *event)
{
    if ( myGrScene->sceneType() == SCENETYPE::PLANSCENE && myGrScene->floorID().isNull() ) return;
    if ( myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE && myGrScene->frameID().isNull() ) return;

    mousePressOn(true);

    if ( isPointHitMode() &&  event->button() == Qt::LeftButton ) {

        if ( previousHitGP.isEmpty() ) {
            previousHitGP.append(currentGP);
        } else {
            bool overlap = false, isClose = false;

            for ( int i = 0; i < previousHitGP.count(); i++ ) {
                GlobalPoint gp = previousHitGP.at(i);
                if ( (gp.getElvQPointF() - currentGP.getElvQPointF()).manhattanLength() < 1.0e-3 ) {
                    overlap = true;
                    if ( i == 0 && previousHitGP.count() > 2 ) isClose = true;
                    break;
                }
            }
            if ( myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE){
                if ( isClose ) createMemberFromPoints();
                if ( !overlap ) previousHitGP.append(currentGP);
            }else if(myGrScene->sceneType() == SCENETYPE::PLANSCENE){
                previousHitGP.append(currentGP);
                if(previousHitGP.count()>1) createMemberFrom2Points();
            }
        }
    }

    if ( isPointHitMode() &&  event->button() == Qt::RightButton ) {
        if ( !previousHitGP.isEmpty() ) previousHitGP.clear();
    }

    if ( isOneHitMode() &&  event->button() == Qt::LeftButton ) {
        if ( batchPlanes.count() == 1 ) {
            batchPlanesToPlaneItems(false);
            batchPlanes.clear();
        }
    }

    if ( isBatchMode() &&  event->button() == Qt::LeftButton ) {
        // 既に選択された状態で左クリック → ラバーバンド入力モード、ラバーバンドの表示は OFF に
        currentModifier = event->modifiers();

        if ( currentModifier == Qt::NoModifier ) {
            if ( !batchPlanes.isEmpty() ) {
                batchPlanesToPlaneItems();
                mousePressOn(false);
            }
        } else {
            QPointF pp = myGrView->mapToScene( event->pos() );
            QPolygonF poly = myGrScene->searchContainPlane( pp );
            tmpBatchPlanes = QList<QPolygonF>();
            if ( !poly.isEmpty() ) tmpBatchPlanes << poly;
            combineBatchPlanes();
        }

//        bool inputOn=( !batchPlanes.isEmpty() && event->modifiers() == Qt::NoModifier );
    }

    if ( isSelectHitMode() &&  event->button() == Qt::LeftButton ) {
        bool inputOn = ( !myGrScene->selectedItems().isEmpty() && event->modifiers() == Qt::NoModifier );
        if ( inputOn ) {
            selectItemToLoadItem();
            mousePressOn(false);
        }
    }

    if ( isSelectHitMode() && event->button() == Qt::RightButton ) {
        myGrView->clearSelectedItems();
    }

    if ( isMoveWaiting() ) {
        if ( event->button() == Qt::LeftButton ) {
            moveMemberJoints();
        } else if ( event->button() == Qt::RightButton ) {
            setWaitingMode();
        }
        mousePressOn(false);
        return;
    }

    if ( isEditMode() &&  event->button() == Qt::RightButton ) {

        manageActions->setActionStatus( currentDataType(), isMoveWaiting(),
                                        myGrScene->selectedItems().count() > 0,
                                        myGrScene->selectedItems().count() > 1,
                                        setMoveWaitingJoints(event->pos()) );
        manageActions->showContextMenu();

    }
}

void WallEventOperation::execMouseRelease(QMouseEvent *event)
{
    mousePressOn(false);

    if ( isBatchMode() ) {
        if ( event->button() == Qt::LeftButton ) {
            combineBatchPlanes();
            myGrScene->update();
        } else {
            refreshBatchPlanes();
        }
    }
}

void WallEventOperation::execKeyPress(QKeyEvent *event)
{
//    qDebug() << event->key();
    if ( isMoveWaiting() ) setWaitingMode();

    if ( isPointHitMode() ) {
        if ( event->key() == Qt::Key_Escape && !previousHitGP.isEmpty() ) {
            previousHitGP.clear();
        }
    }

    if ( isBatchMode() ) {
        if ( event->key() == Qt::Key_Escape ) {
            refreshBatchPlanes();
            mousePressOn(false);
        } else if ( event->key() == Qt::Key_Return ) {
            if ( !batchPlanes.isEmpty() ) batchPlanesToPlaneItems();
            mousePressOn(false);
        }
    }

    if ( isSelectHitMode() ) {
        if ( event->key() == Qt::Key_Escape ) {
            myGrView->clearSelectedItems();
            mousePressOn(false);
        } else if ( event->key() == Qt::Key_Return ) {
            if ( !myGrScene->selectedItems().isEmpty() ) {
                selectItemToLoadItem();
                mousePressOn(false);
            }
        }
    }
}

void WallEventOperation::execDoubleClick(QMouseEvent *)
{
    if ( isPointHitMode() ) {
        ( previousHitGP.count() < 3 ) ? previousHitGP.clear() : createMemberFromPoints();
    }
}

void WallEventOperation::changePosition(const QPoint &cur_p)
{
    if ( isOneHitMode() ) {
        QPointF pp = myGrView->mapToScene( cur_p );
        QPolygonF poly = myGrScene->searchContainPlane( pp );
        batchPlanes = QList<QPolygonF>();
        if ( !poly.isEmpty() ) batchPlanes << poly;
        myGrScene->update();
        return;
    }
    AbstractEventOperation::positionSnap(cur_p, true);
    changePositionText();
    myGrScene->setCurrentFrames(currentGP.getAttachedFrameList());
    if (myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE)
        myGrScene->setCurrentFloors(currentGP.getAttachedFloorList());
    myGrScene->update();
}

bool WallEventOperation::existNearlyPoint(QPoint )
{
    return false;
}

void WallEventOperation::changeBatchRect(QPoint p1, QPoint p2)
{
    QPointF pp1 = myGrView->mapToScene ( p1 );
    QPointF pp2 = myGrView->mapToScene ( p2 );

    QRectF rect;
    ( pp1.x() > pp2.x() ) ? rect.setX(pp2.x()) : rect.setX(pp1.x()) ;
    ( pp1.y() > pp2.y() ) ? rect.setY(pp2.y()) : rect.setY(pp1.y()) ;

    rect.setWidth(qAbs(pp1.x() - pp2.x()));
    rect.setHeight(qAbs(pp1.y() - pp2.y()));

    selectBatchPlanes(rect);
}

void WallEventOperation::refreshBatchPlanes()
{
    tmpBatchPlanes.clear();
    batchPlanes.clear();
    myGrScene->update();
}

void WallEventOperation::selectBatchPlanes(const QRectF &rangeRect)
{
    tmpBatchPlanes.clear();
    Q_FOREACH ( QPolygonF polygon, myGrScene->getBatchInputPlanes() ) {
        if ( rangeRect.contains( polygon.boundingRect() ) ) tmpBatchPlanes.append(polygon);
    }
}

void WallEventOperation::combineBatchPlanes()
{
    if ( currentModifier == Qt::ControlModifier ) {
        QList<QPolygonF> addPlanes;
        Q_FOREACH (QPolygonF a_poly, tmpBatchPlanes) {
            bool detected = false;
            Q_FOREACH (QPolygonF b_poly, batchPlanes) {
                if ( a_poly.count() != b_poly.count() ) continue;
                bool ok = true;
                for ( int i = 0; i < a_poly.count() ; i++ ) {
                    if ( (a_poly.at(i) - b_poly.at(i)).manhattanLength() > 1.0e-3 ) {
                        ok = false;
                        break;
                    }
                }
                if ( ok ) {
                    detected = true;
                    break;
                }
            }
            if ( !detected ) addPlanes.append(a_poly);
        }
        batchPlanes.append(addPlanes);
    } else if ( currentModifier == Qt::AltModifier ) {
        Q_FOREACH (QPolygonF a_poly, tmpBatchPlanes) {
            for (int i = batchPlanes.count() - 1; i >= 0 ; i--) {
                QPolygonF b_poly = batchPlanes.at(i);
                if ( a_poly.count() != b_poly.count() ) continue;
                bool ok = true;
                for ( int j = 0; j < a_poly.count() ; j++ ) {
                    if ( (a_poly.at(j) - b_poly.at(j)).manhattanLength() > 1.0e-3 ) {
                        ok = false;
                        break;
                    }
                }
                if ( ok ) {
                    batchPlanes.removeAt(i);
                    break;
                }
            }
        }
    } else {
        batchPlanes = tmpBatchPlanes;
    }
    tmpBatchPlanes.clear();
}

void WallEventOperation::batchPlanesToPlaneItems(bool messageOn)
{
    if ( messageOn && warningMemberBatchInput() ) return;

    QList<QList<JointData *> > batchJoints;
    int overlap_cnt = 0;
    Q_FOREACH ( QPolygonF polygon, batchPlanes ) {
        QList<JointData *> joints = myGrScene->polygonToBatchJoints(polygon);
        if ( !joints.isEmpty() ) {
            if ( FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(joints, currentDataType()) ) {
                overlap_cnt++;
                continue;
            }
            batchJoints.append(joints);
        }
    }

    if ( !batchJoints.isEmpty() ) {
        QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
        UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMembersCommand(batchJoints, currentDataType(), in_name));
    }

    refreshBatchPlanes();

    if ( overlap_cnt > 0 ) {
        showInformationMessage(QString(u8"%1箇所について重複部材があるため新規作成しませんでした。").arg(overlap_cnt));
    }
}

void WallEventOperation::selectItemToLoadItem()
{
    if ( warningLoadBatchInput() ) return;
    QList<MemberData *> members = myGrScene->selectedMembers();
    if ( members.isEmpty() ) return;
    QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
    QList< QList<JointData *> > jd_alllist;
    int overlap_cnt = 0;
    Q_FOREACH ( MemberData *md, members ) {
        if ( md->dataType() != DATATYPE::TPSLAB ) continue;
        if ( FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(md->getJointList(), DATATYPE::TPSLABLOAD) ) {
            overlap_cnt++;
            continue;
        }
        jd_alllist.append( md->getJointList() );
    }
    UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMembersCommand(jd_alllist, DATATYPE::TPSLABLOAD, in_name));
    myGrView->clearSelectedItems();

    if ( overlap_cnt > 0 ) {
        showInformationMessage(QString(u8"%1箇所について重複荷重があるため新規作成しませんでした。").arg(overlap_cnt));
    }
}

void WallEventOperation::createMemberFromPoints()
{
    QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
    ADDMOVEDACTION amd_act = UnifiedEditingData::getInstance()->currentAddMovedAction();

    UnifiedEditingData::getInstance()->beginUndoStackMacro("create Member From Points");

    QList<JointData *> jdlist, new_jdlist;
    JointData *jj;

    for ( int i = 0; i < previousHitGP.count(); i++ ) {

        GlobalPoint gp1 = previousHitGP.at(i);
        jj = myGrScene->searchNearlyJoint(gp1.getElvQPointF() * 1000.);

        if ( jj == NULL ) {
            int idx = -1;
            for ( int j = 0; j < jdlist.count(); j++ ) {
                QPointF len( gp1.getElvQPointF() - previousHitGP.at(j).getElvQPointF() );
                if ( len.manhattanLength() < 1.0e-3 ) idx = j;
            }
            if ( idx > -1 ) jj = jdlist.at(idx);
        }

        if ( jj == NULL ) {
            CreateJointCommand *cj_com = new CreateJointCommand(gp1, false);
            UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
            jj = cj_com->createdJoint();
            new_jdlist.append(jj);
        }

        jdlist.append(jj);
    }

    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalJointAppendedCommand(new_jdlist));

    if ( amd_act == AMDINTER ) {
        QList<JointData *> c_jdlist;
        for ( int i = 0; i < jdlist.count(); i++ ) {
//            JointData* j1=jdlist.at(i);
//            JointData* j2=( i==jdlist.count()-1 ) ? jdlist.first() : jdlist.at(i+1) ;
            QPointF p1 = previousHitGP.at(i).getElvQPointF() * 1000.;
            QPointF p2 = ( i == jdlist.count() - 1 ) ? previousHitGP.first().getElvQPointF() * 1000.
                         : previousHitGP.at(i + 1).getElvQPointF() * 1000.;

            c_jdlist.append(jdlist.at(i));
            QList<JointData *> d_jdlist = myGrScene->searchBetweenPoints(p1, p2);
            Q_FOREACH ( JointData *jj, d_jdlist ) {
                if ( !jdlist.contains(jj) ) c_jdlist.append(jj);
            }
        }
        jdlist = c_jdlist;
    }

    bool isOverlap = FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(jdlist, currentDataType());
    if ( !isOverlap ) {
        UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMemberCommand(jdlist, currentDataType(), in_name));
    }

    divideMembersByJoints(new_jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    previousHitGP.clear();

    if ( isOverlap ) showInformationMessage(u8"重複部材（荷重）があるため新規作成しませんでした。");
}

void WallEventOperation::createMemberFrom2Points()
{
    qDebug()<<"2pointhit";
    QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
    ADDMOVEDACTION amd_act = UnifiedEditingData::getInstance()->currentAddMovedAction();

    UnifiedEditingData::getInstance()->beginUndoStackMacro("create Member From Points");

    QList<JointData *> jdlist, new_jdlist;
    JointData *jj;

    qDebug()<<"aaa";
    for ( int i = 0; i < 2; i++ ) {

        GlobalPoint gp1 = previousHitGP.at(i);
        jj = myGrScene->searchNearlyJoint(gp1.getElvQPointF() * 1000.);

        if ( jj == nullptr ) {
            int idx = -1;
            for ( int j = 0; j < jdlist.count(); j++ ) {
                QPointF len( gp1.getElvQPointF() - previousHitGP.at(j).getElvQPointF() );
                if ( len.manhattanLength() < 1.0e-3 ) idx = j;
            }
            if ( idx > -1 ) jj = jdlist.at(idx);
        }

        if ( jj == nullptr ) {
            CreateJointCommand *cj_com = new CreateJointCommand(gp1, false);
            UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
            jj = cj_com->createdJoint();
            new_jdlist.append(jj);
        }

        jdlist.append(jj);
    }
    qDebug()<<"bbb";
    for ( int i = 0; i < 2; i++ ) {

        QUuid flid;
        GlobalPoint gp1 = previousHitGP.at(1-i);
        qreal myheight = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(jdlist.first()->getAttachedFloorList().first());
        qreal min = 99999999.0;
        foreach(FLOORVALUES val,FactoryUnifiedData::getInstance(isOutput)->getFloorList()){
            if(val.floorHeight > myheight && val.floorType != FLOORTYPE::FL_CHILD){
                if(val.floorHeight <= min){min=val.floorHeight;flid = val.UUID;}
            }
        }

        gp1.setZ(gp1.z()+min-myheight);
        qDebug()<<myheight<<min<<gp1.z();
        jj = FactoryUnifiedData::getInstance(isOutput)->nearlyJoint(gp1);
        //jj = myGrScene->searchNearlyJoint(gp1.getElvQPointF() * 1000.);

//        if ( jj == nullptr ) {
//            int idx = -1;
//            for ( int j = 0; j < jdlist.count(); j++ ) {
//                QPointF len( gp1.getElvQPointF() - previousHitGP.at(j).getElvQPointF() );
//                if ( len.manhattanLength() < 1.0e-3 ) idx = j;
//            }
//            if ( idx > -1 ) jj = jdlist.at(idx);
//        }

        if ( jj == nullptr ) {
            CreateJointCommand *cj_com = new CreateJointCommand(gp1, false);
            UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
            jj = cj_com->createdJoint();
            FactoryUnifiedData::getInstance(isOutput)->changeJointsAttachment(QList<JointData*>()<<jj,QList<QUuid>()<<flid,DATATYPE::TPFLOOR);
            new_jdlist.append(jj);
        }

        jdlist.append(jj);
        previousHitGP.append(gp1);
    }

    qDebug()<<"ccc";
    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalJointAppendedCommand(new_jdlist));

    qDebug()<<"c1";
    if(amd_act == AMDINTER){
        QList<JointData* > c_jdlist;
        for ( int i = 0; i < jdlist.count(); i++ ) {
            GlobalPoint p1 = previousHitGP.at(i);
            GlobalPoint p2 = ( i == jdlist.count() - 1 ) ? previousHitGP.first(): previousHitGP.at(i + 1);

            c_jdlist.append(jdlist.at(i));
            QList<JointData *> d_jdlist = FactoryUnifiedData::getInstance(isOutput)->getJointBetween(p1,p2);
            Q_FOREACH ( JointData *jj, d_jdlist ) {
                if ( !jdlist.contains(jj) ) c_jdlist.append(jj);
            }
        }
        jdlist = c_jdlist;

    }
//    if ( amd_act == AMDINTER ) {
//        QList<JointData *> c_jdlist;
//        for ( int i = 0; i < jdlist.count(); i++ ) {
//            QPointF p1 = previousHitGP.at(i).getElvQPointF() * 1000.;
//            QPointF p2 = ( i == jdlist.count() - 1 ) ? previousHitGP.first().getElvQPointF() * 1000.
//                         : previousHitGP.at(i + 1).getElvQPointF() * 1000.;

//            c_jdlist.append(jdlist.at(i));
//            QList<JointData *> d_jdlist = myGrScene->searchBetweenPoints(p1, p2);
//            Q_FOREACH ( JointData *jj, d_jdlist ) {
//                if ( !jdlist.contains(jj) ) c_jdlist.append(jj);
//            }
//        }
//        jdlist = c_jdlist;
//    }
    qDebug()<<"ddd";

    bool isOverlap = FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(jdlist, currentDataType());
    if ( !isOverlap ) {
        UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMemberCommand(jdlist, currentDataType(), in_name));
    }

    divideMembersByJoints(new_jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    previousHitGP.clear();
    qDebug()<<"eee";

    if ( isOverlap ) showInformationMessage(u8"重複部材（荷重）があるため新規作成しませんでした。");
}


bool WallEventOperation::setMoveWaitingJoints(QPoint mousePoint)
{
    if ( mousePoint.x() < 0 || mousePoint.y() < 0 ) {
        waitingJoints = JOINTGROUP();
        return false;
    }

    QPointF pp = myGrView->mapToScene( mousePoint );
    waitingJoints = myGrScene->selectedMemberJoints(pp);
    return ( waitingJoints.j2 != NULL );
}

void WallEventOperation::slotSetMoveWaitingMode()
{
    setMoveWaitingOn(true);
    manageActions->resetActions();
    myGrScene->update();
}

void WallEventOperation::moveMemberJoints()
{
    MemberData *md = myGrScene->selectedMember();
    QPointF curPos = currentGP.getElvQPointF() * 1000.;
    JointData *jd = myGrScene->searchNearlyJoint(curPos);

    if ( waitingJoints.j2 == NULL ) return;

    if ( md == NULL || jd == NULL ) {
        showWarningMessage(u8"移動先に節点が存在しません。");
        return;
    }

    if ( md->getJointList().contains(jd) ) {
        showWarningMessage(u8"部材内の点には移動できません。");
        return;
    }

    UnifiedEditingData::getInstance()->beginUndoStackMacro("move Member Joints");
    UnifiedEditingData::getInstance()->pushUndoCommand(new ReplaceMemberJointCommand(md, waitingJoints.j2, jd));

    if ( UnifiedEditingData::getInstance()->currentAddMovedAction() == AMDINTER ) {
        QList<JointData *> jdlist;
        if ( waitingJoints.j1 != NULL ) {
            QPointF p1 = myGrScene->itemPosition(waitingJoints.j1, curPos);
            jdlist.append(myGrScene->searchBetweenPoints(p1, curPos));
        }
        if ( waitingJoints.j3 != NULL ) {
            QPointF p2 = myGrScene->itemPosition(waitingJoints.j3, curPos);
            jdlist.append(myGrScene->searchBetweenPoints(curPos, p2));
        }
        if ( !jdlist.isEmpty() ) {
            UnifiedEditingData::getInstance()->pushUndoCommand(new InteriorMembersCommand(QList<MemberData *>{md},QList<QList<JointData *>>{jdlist}));
        }
    }

    if ( UnifiedEditingData::getInstance()->currentDeleteAction() == DELETEJOINT ) {
        myGrScene->blockSignals(true);
        if ( waitingJoints.j2->isIndependentJoint() ) {
            UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( QList<JointData *>{waitingJoints.j2}));
        }
        myGrScene->blockSignals(false);
    }
    myGrScene->sendSelectedItems();

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    setWaitingMode();
}

void WallEventOperation::slotFindItems()
{
    FindItemDialog dialog(currentDataType(), myGrView,myGrView->askIsOutput());

    if ( dialog.exec() == QDialog::Accepted ) {

        mousePressOn(false);
        myGrView->clearSelectedItems();

        QString msg = dialog.checkDialogInput();
        if ( !msg.isEmpty() ) {
            showInformationMessage(msg);
            return;
        }

        FINDDIALOGSTATUS fst = dialog.dialogStatus();
        SCENETYPE stype = myGrScene->sceneType();
        QList<MemberData *> mdlist;

        // 節点のサーチ
        if ( fst.isJflChecked || fst.isJfrChecked || fst.isJnoChecked ) {
            showInformationMessage(u8"現在の編集モードでは節点を選択できません。");
            return;
        }

        // 部材のサーチ
        if ( fst.isMtpChecked || fst.isMstChecked || fst.isMflChecked || fst.isMfrChecked ) {

            DATATYPE dtype = currentDataType();
            if ( fst.isMtpChecked && dtype != fst.dataType ) {
                showInformationMessage(u8"現在の編集モードでは指定の要素を選択できません。");
                return;
            }

            if ( stype == SCENETYPE::PLANSCENE ) {
                QList<MemberData *> mdlist1 = FactoryUnifiedData::getInstance(isOutput)->memberListOfType(dtype);
                Q_FOREACH ( MemberData *md, mdlist1 ) {
                    if ( md->touchFloor( myGrScene->floorID() ) ) mdlist.append(md);
                }
            } else {
                QList<MemberData *> mdlist1 = FactoryUnifiedData::getInstance(isOutput)->memberListOfType(dtype);
                Q_FOREACH ( MemberData *md, mdlist1 ) {
                    if ( md->touchFrame( myGrScene->frameID() ) ) mdlist.append(md);
                }
            }

            for ( int i = mdlist.count() - 1; i >= 0; i-- ) {
                MemberData *md = mdlist.at(i);
                if ( fst.isMstChecked ) {
                    if ( fst.mstMatch && md->sectionName() != fst.memberName ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                    if ( !fst.mstMatch && !md->sectionName().contains( fst.memberName ) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                }
                if ( fst.isMflChecked ) {
                    if ( fst.mflMatch && !md->containsFloors(fst.mFloors) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                    if ( !fst.mflMatch && !md->touchFloors(fst.mFloors) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                }
                if ( fst.isMfrChecked ) {
                    if ( fst.mfrMatch && !md->containsFrames(fst.mFrames) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                    if ( !fst.mfrMatch && !md->touchFrames(fst.mFrames) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                }
            }
        }

        if ( mdlist.isEmpty() ) {
            showInformationMessage(u8"指定の要素は見つかりませんでした。");
            return;
        }

        myGrScene->setMemberSelect( mdlist );

    }
}

void WallEventOperation::slotDeleteItems()
{
    myGrScene->blockSignals(true);

    QList<MemberData *> delMembers = myGrScene->selectedMembers();

    if ( UnifiedEditingData::getInstance()->currentDeleteAction() == REMAINJOINT ) {
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveMembersCommand(delMembers));
    } else {

        UnifiedEditingData::getInstance()->beginUndoStackMacro("delete Members");

        QList<JointData *> relateJoints;
        Q_FOREACH ( MemberData *md, delMembers ) {
            Q_FOREACH ( JointData *jd, md->getJointList() ) {
                if ( !relateJoints.contains(jd) ) relateJoints.append(jd);
            }
        }
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveMembersCommand(delMembers));
        QList<JointData *> delJoints;
        Q_FOREACH ( JointData *jd, relateJoints ) {
            if ( jd->isIndependentJoint() ) delJoints.append(jd);
        }
        if ( !delJoints.isEmpty() ) {
            UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( delJoints ));
        }

        UnifiedEditingData::getInstance()->endUndoStackMacro();

    }

    myGrScene->blockSignals(false);
    myGrScene->sendSelectedItems();

    if ( delMembers.count() < 1 ) return;

    QString msg = QString(tr("%1 pieces")).arg(delMembers.count());
    msg += FactoryUnifiedData::getInstance(isOutput)->dataTypeToJapanese( currentDataType() );
    msg += u8"を削除しました。";
    showInformationMessage(msg);
    setWaitingMode();
}

void WallEventOperation::slotChangeSection()
{
    ChangeSectionDialog dialog(myGrView,isOutput);
    dialog.setCurrentDataType( currentDataType() );

    if ( dialog.exec() == QDialog::Accepted ) {

        DATATYPE dtype = dialog.currentDataType();
        QString str = dialog.currentName();

        QList<MemberData *> mdlist;
        Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() ) {
            if ( md->dataType() == dtype ) mdlist.append(md);
        }

        if ( mdlist.isEmpty() ) {
            showInformationMessage(u8"対象の部材（または荷重）が選択されていません。");
            return;
        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeMemberSectionCommand(mdlist, str));
        showInformationMessage(QString(u8"%1個の部材（または荷重）について符号を変更しました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void WallEventOperation::slotReverseMemberJoint()
{
    QList<MemberData *> mdlist = myGrScene->selectedMembers();
    if ( mdlist.isEmpty() ) {
        showInformationMessage(u8"部材（または荷重）が選択されていません。");
        return;
    }

    UnifiedEditingData::getInstance()->pushUndoCommand(new ReverseMemberJointCommand(mdlist));
    showInformationMessage(QString(u8"%1個の部材（または荷重）について節点構成を反転しました。").arg(mdlist.count()));
    setWaitingMode();
}

void WallEventOperation::slotChangeShift()
{
    if ( currentDataType() == DATATYPE::TPSLABLOAD ) return;

    QList<MemberData *> mdlist = myGrScene->selectedMembers();
    if ( mdlist.count() < 1 ) {
        showInformationMessage(u8"部材が選択されていません。");
        return;
    }

    bool fixed = true;
    qreal shy = mdlist.first()->getShiftY();
    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( qAbs( shy - md->getShiftY() ) > 1.0e-3 ) {
            fixed = false;
            break;
        }
    }
    QString str1 = ( fixed ) ? QString("%1").arg(shy, 0, 'f', 3) : QString("***") ;

    fixed = true;
    qreal shz = mdlist.first()->getShiftZ();
    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( qAbs( shz - md->getShiftZ() ) > 1.0e-3 ) {
            fixed = false;
            break;
        }
    }
    QString str2 = ( fixed ) ? QString("%1").arg(shz, 0, 'f', 3) : QString("***") ;

    ChangeShiftDialog dialog( str1, str2, myGrView );

    if ( dialog.exec() == QDialog::Accepted ) {
        QString value_shy = dialog.shiftYText();
        QString value_shz = dialog.shiftZText();

        bool y_chg = true, z_chg = true, ok = true;
        qreal shy = value_shy.toDouble(&ok);
        if ( value_shy == "***" ) {
            y_chg = false;
            ok = true;
        }
        if ( !ok ) {
            warningInvalidValue();
            return;
        }
        qreal shz = value_shz.toDouble(&ok);
        if ( value_shz == "***" ) {
            z_chg = false;
            ok = true;
        }
        if ( !ok ) {
            warningInvalidValue();
            return;
        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeShiftValueCommand(mdlist, y_chg, shy, z_chg, shz));
        showInformationMessage(QString(u8"%1個の部材の寄り長さが変更されました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void WallEventOperation::slotChangeWallOpen()
{
    ChangeOpenDialog dialog(myGrView,isOutput);

    if ( dialog.exec() == QDialog::Accepted ) {
        QList<MemberData *> mdlist;
        Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() )
            if ( md->dataType() == DATATYPE::TPWALL || md->dataType() == DATATYPE::TPUWALL ) mdlist.append(md);
        if ( mdlist.isEmpty() ) {
            showInformationMessage(u8"壁または地下外壁が選択されていません。");
            return;
        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeWallOpenCommand(mdlist, dialog.currentName()));
        showInformationMessage(QString(u8"%1個の壁について開口符号が変更されました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void WallEventOperation::slotDeleteWallOpen()
{
    QList<MemberData *> mdlist;
    Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() )
        if ( md->dataType() == DATATYPE::TPWALL || md->dataType() == DATATYPE::TPUWALL ) mdlist.append(md);
    if ( mdlist.isEmpty() ) {
        showInformationMessage(u8"壁または地下外壁が選択されていません。");
        return;
    }

    UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeWallOpenCommand(mdlist, QString()));
    showInformationMessage(QString(u8"%1個の壁について開口符号が削除されました。").arg(mdlist.count()));
    setWaitingMode();
}

} // namespace post3dapp
