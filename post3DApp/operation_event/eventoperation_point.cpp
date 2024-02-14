#include "eventoperation_point.h"

#include <QtGui>
#include <QDebug>

#include "autoattach_dialog.h"
#include "changeplanejoint_dialog.h"
#include "changesection_dialog.h"
#include "customdraw_grview.h"
#include "define_draw_figure.h"
#include "factory_unifieddata.h"
#include "finditem_dialog.h"
#include "input_grscene.h"
#include "jointitem.h"
#include "jointloaditem.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "manage_pointactions.h"
#include "operation_commands.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "unified_settings.h"

namespace post3dapp{
PointEventOperation::PointEventOperation(CustomDrawGrView *view)
    : AbstractEventOperation(view)
{
    hideRubberBand(false);
    setMoveWaitingOn(false);
    setPasteWaitingOn(false);

    copySourceItem = nullptr;
    moveSourceJoint = nullptr;

    manageActions = new ManagePointActions(this);
    changeJointDialog = new ChangePlaneJointDialog(this, view);

    myElementType = UnifiedEditingData::getInstance()->currentElement();
    currentAddEditMode = UnifiedEditingData::getInstance()->currentAddEditMode();

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentStatusChanged, this, &PointEventOperation::reloadSettings);
}

void PointEventOperation::reloadSettings()
{
    ELEMENTTYPE old_element = myElementType;
    ADDEDITMODE old_mode = currentAddEditMode;
    myElementType = UnifiedEditingData::getInstance()->currentElement();
    currentAddEditMode = UnifiedEditingData::getInstance()->currentAddEditMode();
    if ( UnifiedEditingData::getInstance()->elementGroup(myElementType) != ELGPOINT ) return;
    if ( old_mode != currentAddEditMode || old_element != myElementType ) initializeSettings();
}

void PointEventOperation::initializeSettings()
{
    setWaitingMode();
    refreshBatchPoints();
    myGrScene->setItemSelectable( currentDataType(), currentAddEditMode == EDITMODE );
    if ( currentAddEditMode == SELECTHITMODE ) myGrScene->setInputItemSelectable( currentDataType() );
    manageActions->resetActions( currentAddEditMode == EDITMODE );
    copySourceItem = nullptr;
    moveSourceJoint = nullptr;
}

void PointEventOperation::slotChangeActionStatus()
{
    if ( isEditMode() ) {
        manageActions->setActionStatus( currentDataType(), isMoveWaiting(),
                                        !myGrScene->selectedItems().isEmpty(),
                                        myGrScene->selectedItems().count() > 1,
                                        setWaitingPoints(QPoint(-100, -100)) );
    }
}

void PointEventOperation::setWaitingMode()
{
    myGrScene->clearCurrentAttach();
    myGrView->clearSelectedItems();
    hideRubberBand(false);
    setMoveWaitingOn(false);
    setPasteWaitingOn(false);
    copySourceItem = NULL;
    moveSourceJoint = NULL;
//    qDebug() << "point setWaitingMode";
}

void PointEventOperation::toggleViewMode()
{
    setViewModeOn(!viewMode);
    if ( currentAddEditMode == POINTHITMODE ) myGrScene->clearCurrentAttach();
    if ( currentAddEditMode == BATCHMODE ) refreshBatchPoints();
    if ( currentAddEditMode == SELECTHITMODE && viewMode ) setWaitingMode();
    if ( currentAddEditMode == EDITMODE && viewMode ) setWaitingMode();
}

bool PointEventOperation::isPositionSnapOn() const
{
    return ( isPointHitMode() || isActionWaiting() );
}

bool PointEventOperation::isBatchInputMode() const
{
    return ( isBatchMode() && isMousePressed() );
}

bool PointEventOperation::isSelectItemMode() const
{
    if ( isSelectHitMode() && isMousePressed() ) return true;
    return ( isEditMode() && !isActionWaiting() && isMousePressed() );
}

bool PointEventOperation::isPointHitMode() const
{
    return ( !viewMode && currentAddEditMode == POINTHITMODE );
}

bool PointEventOperation::isBatchMode() const
{
    return ( !viewMode && myElementType == ELJOINT && currentAddEditMode == BATCHMODE );
}

bool PointEventOperation::isSelectHitMode() const
{
    return ( !viewMode && myElementType == ELJOINTLOAD && currentAddEditMode == SELECTHITMODE );
}

bool PointEventOperation::isEditMode() const
{
    return ( !viewMode && currentAddEditMode == EDITMODE );
}

bool PointEventOperation::isActionWaiting() const
{
    return ( isMoveWaiting() || isPasteWaiting() );
}

bool PointEventOperation::isMoveWaiting() const
{
    return onMoveWaiting ;
}

bool PointEventOperation::isPasteWaiting() const
{
    return onPasteWaiting ;
}

void PointEventOperation::setMoveWaitingOn(bool on)
{
    onMoveWaiting = on;
}

void PointEventOperation::setPasteWaitingOn(bool on)
{
    onPasteWaiting = on;
    if ( onPasteWaiting ) myGrScene->clearSelection();
}

void PointEventOperation::execDrawForeground(QPainter *painter, const QRect &vrect)
{
    if ( !isPositionSnapOn() && !isBatchMode() ) return;

    painter->setTransform(QTransform(1, 0, 0, 1, 0, 0), false);
    qreal factor = qPow(myGrView->transform().m11(), 0.25);
    int dd = int(factor * 10.0);

    if ( isBatchMode() ) {

        painter->setBrush(QBrush(QColor(255, 200, 200)));
        painter->setPen(QColor(255, 50, 0));

        Q_FOREACH (QPointF pp, batchPoints)
            painter->drawEllipse(myGrView->mapFromScene(pp), dd, dd);

        if ( currentModifier == Qt::AltModifier ) {
            painter->setBrush(QBrush(QColor(200, 200, 255)));
            painter->setPen(QColor(0, 50, 255));
        }
        Q_FOREACH (QPointF pp, tmpBatchPoints)
            painter->drawEllipse(myGrView->mapFromScene(pp), dd, dd);

        return;
    }

    AbstractEventOperation::execDrawForeground(painter, vrect);

    if ( !isActionWaiting() ) return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(QBrush(QColor(200, 255, 200)));
    painter->setPen(QPen(QBrush(QColor(0, 255, 50)), 1, Qt::DashLine));

    if ( myElementType == ELJOINTLOAD ) {
        QPoint qq = myGrView->mapFromScene(currentGP.getElvQPointF() * 1000.);
        painter->drawEllipse(qq, dd, dd);
    }

    if ( myElementType == ELJOINT && isPasteWaiting() ) {
        QPointF q1 = currentGP.getElvQPointF() * 1000.;
        Q_FOREACH (QPointF pp, UnifiedEditingData::getInstance()->getPointsCopyBuffer()) {
            QPoint qq = myGrView->mapFromScene(pp + q1);
            painter->drawEllipse(qq, dd, dd);
        }
    }

    if ( copySourceItem == NULL ) return;
    if ( myElementType == ELJOINT && isMoveWaiting() ) {
        QPointF q1 = currentGP.getElvQPointF() * 1000.;
        Q_FOREACH ( QGraphicsItem *item, myGrScene->selectedItems() ) {
            QPoint qq = myGrView->mapFromScene( item->pos() - copySourceItem->pos() + q1 );
            painter->drawEllipse(qq, dd, dd);
        }
        painter->setRenderHint(QPainter::Antialiasing, false);
        Q_FOREACH ( QLineF ll, myGrScene->getMoveWaitingLines(copySourceItem, q1, UnifiedEditingData::getInstance()->currentElement()) ) {
            painter->drawLine( myGrView->mapFromScene(ll.p1()), myGrView->mapFromScene(ll.p2()) );
        }
    }
}

void PointEventOperation::execMousePress(QMouseEvent *event)
{
    if ( myGrScene->sceneType() == SCENETYPE::PLANSCENE && myGrScene->floorID().isNull() ) return;
    if ( myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE && myGrScene->frameID().isNull() ) return;

    mousePressOn(true);

    if ( isPointHitMode() &&  event->button() == Qt::LeftButton ) {

        UnifiedEditingData::getInstance()->beginUndoStackMacro("create Joint or JointLoad By Mouse Hit");
        JointData *jd = myGrScene->searchNearlyJoint( currentGP.getElvQPointF() * 1000. );
        if ( jd == NULL ) {
            CreateJointCommand *cj_com = new CreateJointCommand(currentGP);
            UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
            jd = cj_com->createdJoint();
        }
        bool isOverlap = false;
        if ( myElementType == ELJOINTLOAD ) {
            isOverlap = FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(QList<JointData *>() << jd, DATATYPE::TPJOINTLOAD);
            if ( !isOverlap ) {
                QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
                CreateMemberCommand *cm_com = new CreateMemberCommand(QList<JointData *>() << jd, DATATYPE::TPJOINTLOAD,
                                                                      in_name);
                UnifiedEditingData::getInstance()->pushUndoCommand(cm_com);
            }
        }
        divideMembersByJoints( QList<JointData *>() << jd,
                               UnifiedEditingData::getInstance()->currentAddMoveAction() );
        UnifiedEditingData::getInstance()->endUndoStackMacro();

        if ( isOverlap ) showInformationMessage(u8"重複荷重があるため新規作成しませんでした。");

    }

    if ( isBatchMode() &&  event->button() == Qt::LeftButton ) {
        // 既に選択された状態で左クリック → ラバーバンド入力モード、ラバーバンドの表示は OFF に
        bool inputOn = ( !batchPoints.isEmpty() && event->modifiers() == Qt::NoModifier );
        if ( inputOn ) {
            batchPointsToJointItems();
            mousePressOn(false);
        }
        currentModifier = event->modifiers();
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
            ( myElementType == ELJOINTLOAD ) ? moveMemberJoints() : moveJoints() ;
        } else if ( event->button() == Qt::RightButton ) {
            setWaitingMode();
        }
        mousePressOn(false);
        return;
    }

    if ( isPasteWaiting() ) {
        if ( event->button() == Qt::LeftButton ) {
            pasteJoints();
        } else if ( event->button() == Qt::RightButton ) {
            setWaitingMode();
        }
        mousePressOn(false);
        return;
    }

    if ( isEditMode() &&  event->button() == Qt::RightButton ) {
        manageActions->setActionStatus( currentDataType(), isMoveWaiting(),
                                        !myGrScene->selectedItems().isEmpty(),
                                        myGrScene->selectedItems().count() > 1,
                                        setWaitingPoints(event->pos()) );
        manageActions->showContextMenu();
        mousePressOn(false);
    }
    /*
        if( isPasteWaiting() && event->button() == Qt::LeftButton ){
            pasteJoints();
            mousePressOn(false);
        }
    */
}

void PointEventOperation::execMouseRelease(QMouseEvent *event)
{
    mousePressOn(false);

    if ( isBatchMode() ) {
        if ( event->button() == Qt::LeftButton ) {
            combineBatchPoints();
            myGrScene->update();
        } else {
            refreshBatchPoints();
        }
    }
}

void PointEventOperation::execKeyPress(QKeyEvent *event)
{
    if ( isMoveWaiting() || isPasteWaiting() ) setWaitingMode();

    if ( isBatchMode() ) {
        if ( event->key() == Qt::Key_Escape ) {
            refreshBatchPoints();
            mousePressOn(false);
        } else if ( event->key() == Qt::Key_Return ) {
            if ( !batchPoints.isEmpty() ) batchPointsToJointItems();
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

void PointEventOperation::refreshBatchPoints()
{
    tmpBatchPoints.clear();
    batchPoints.clear();
    myGrScene->update();
}

bool PointEventOperation::existNearlyPoint(QPoint p1)
{
    QPointF pp1 = myGrView->mapToScene ( p1 + QPoint(-8, -8) );
    QPointF pp2 = myGrView->mapToScene ( p1 + QPoint(8, 8) );
    QRectF rect(pp1, pp2);

    selectBatchPoints(rect);
    hideRubberBand( !tmpBatchPoints.isEmpty() );
    return isHiddenRubberBand();
}

void PointEventOperation::positionSnap(const QPoint &cur_p, bool)
{
    AbstractEventOperation::positionSnap( cur_p, (myElementType == ELJOINTLOAD) );
}

void PointEventOperation::changeBatchRect(QPoint p1, QPoint p2)
{
    QPointF pp1 = myGrView->mapToScene ( p1 );
    QPointF pp2 = myGrView->mapToScene ( p2 );

    QRectF rect;
    ( pp1.x() > pp2.x() ) ? rect.setX(pp2.x()) : rect.setX(pp1.x()) ;
    ( pp1.y() > pp2.y() ) ? rect.setY(pp2.y()) : rect.setY(pp1.y()) ;

    rect.setWidth(qAbs(pp1.x() - pp2.x()));
    rect.setHeight(qAbs(pp1.y() - pp2.y()));

    selectBatchPoints(rect);
}

void PointEventOperation::selectBatchPoints(const QRectF &rangeRect)
{
    tmpBatchPoints.clear();

    JOINTBATCHACTION act = UnifiedEditingData::getInstance()->currentJointBatchAction();

    if (act == SPCROSS) {

        QList<GlobalPoint> gplist = myGrScene->getCrossPoints();

        if (myGrScene->sceneType() == SCENETYPE::PLANSCENE) {
            Q_FOREACH (GlobalPoint gp, gplist) {
                QPointF pp = gp.getXYQPointF() * 1000.0;
                if ( rangeRect.contains( pp ) ) tmpBatchPoints.append(pp);
            }
        } else if (myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE) {
            Q_FOREACH (GlobalPoint gp, gplist) {
                QPointF pp = gp.getElvQPointF() * 1000.0;
                if ( rangeRect.contains( pp ) ) tmpBatchPoints.append(pp);
            }
        }

    } else if (act == SPDEFINE) {

        QList<GlobalPoint> gplist = myGrScene->getDefinePoints();

        if (myGrScene->sceneType() == SCENETYPE::PLANSCENE) {
            Q_FOREACH (GlobalPoint gp, gplist) {
                QPointF pp = gp.getXYQPointF() * 1000.0;
                if ( rangeRect.contains( pp ) ) tmpBatchPoints.append(pp);
            }
        } else if (myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE) {
            Q_FOREACH (GlobalPoint gp, gplist) {
                QPointF pp = gp.getElvQPointF() * 1000.0;
                if ( rangeRect.contains( pp ) ) tmpBatchPoints.append(pp);
            }
        }

    } else if (act == SPGRID) {

        int g_pitch = UnifiedEditingData::getInstance()->currentJointBatchPitch();

        int ix1 = int(rangeRect.x()) / g_pitch + 1;
        int ix2 = int(rangeRect.x() + rangeRect.width()) / g_pitch;
        int iy1 = int(rangeRect.y()) / g_pitch + 1;
        int iy2 = int(rangeRect.y() + rangeRect.height()) / g_pitch;

        if ( rangeRect.x() < 0 ) ix1 = ix1 - 1;
        if ( rangeRect.x() + rangeRect.width() < 0 ) ix2 = ix2 - 1;
        if ( rangeRect.y() < 0 ) iy1 = iy1 - 1;
        if ( rangeRect.y() + rangeRect.height() < 0 ) iy2 = iy2 - 1;

        for (int i = ix1; i <= ix2; i++) {
            for (int j = iy1; j <= iy2; j++) {
                tmpBatchPoints.append(QPointF(i * g_pitch, j * g_pitch));
            }
        }

    }
}

void PointEventOperation::combineBatchPoints()
{
    if ( currentModifier == Qt::ControlModifier ) {
        QList<QPointF> addPoints;
        Q_FOREACH (QPointF pp, tmpBatchPoints) {
            bool ok = true;
            Q_FOREACH (QPointF qq, batchPoints) {
                if ( (pp - qq).manhattanLength() < 1.0e-4 ) {
                    ok = false;
                    break;
                }
            }
            if (ok) addPoints.append(pp);
        }
        batchPoints.append(addPoints);
    } else if ( currentModifier == Qt::AltModifier ) {
        Q_FOREACH (QPointF pp, tmpBatchPoints) {
            for (int i = batchPoints.count() - 1; i >= 0 ; i--) {
                if ( (pp - batchPoints.at(i)).manhattanLength() < 1.0e-4 )
                    batchPoints.removeAt(i);
            }
        }
    } else {
        batchPoints = tmpBatchPoints;
    }
    tmpBatchPoints.clear();
}

void PointEventOperation::batchPointsToJointItems()
{
    if ( !warningJointBatchInput() ) {

        for (int i = batchPoints.count() - 1; i > -1; i--) {
            JointData *jd = myGrScene->searchNearlyJoint( batchPoints.at(i) );
            if ( jd != NULL ) batchPoints.removeAt(i);
        }

        UnifiedEditingData::getInstance()->beginUndoStackMacro("batch Input Joints");

        QList<GlobalPoint> gplist = scenePointsToGlobalPoints(batchPoints);
        CreateJointsCommand *cj_com = new CreateJointsCommand(gplist);
        UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
        QList<JointData *> jdlist = cj_com->createdJoints();
        if ( jdlist.isEmpty() ) {
            UnifiedEditingData::getInstance()->endUndoStackMacro();
            return;
        }
        divideMembersByJoints(jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());
        refreshBatchPoints();

        UnifiedEditingData::getInstance()->endUndoStackMacro();
    }
}

void PointEventOperation::selectItemToLoadItem()
{
    if ( warningLoadBatchInput() ) return;
    QList<JointData *> joints = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
    if ( joints.isEmpty() ) return;
    QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
    QList< QList<JointData *> > jd_alllist;
    int overlap_cnt = 0;
    Q_FOREACH ( JointData *jd, joints ) {
        if ( FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(QList<JointData *>() << jd, DATATYPE::TPJOINTLOAD) ) {
            overlap_cnt++;
            continue;
        }
        jd_alllist.append( QList<JointData *>() << jd );
    }
    CreateMembersCommand *cm_com = new CreateMembersCommand(jd_alllist, DATATYPE::TPJOINTLOAD, in_name);
    UnifiedEditingData::getInstance()->pushUndoCommand(cm_com);

    myGrView->clearSelectedItems();

    if ( overlap_cnt > 0 ) {
        showInformationMessage(QString(u8"%1箇所について重複荷重があるため新規作成しませんでした。").arg(overlap_cnt));
    }
}

bool PointEventOperation::setWaitingPoints(QPoint mousePoint)
{
    if ( myElementType == ELJOINTLOAD ) {
        moveSourceJoint = NULL;
        if ( myGrScene->selectedItems().count() != 1 ) return false;
        JointLoadItem *item = qgraphicsitem_cast<JointLoadItem *>(myGrScene->selectedItems().first());
        moveSourceJoint = myGrScene->jointLoadItemToJointData(item);
        return false;
    }

    copySourceItem = NULL;
    bool detected = false;

    Q_FOREACH (JointData *jd, myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement())) {
        JointItem *j_item = myGrScene->jointDataToJointItem(jd);
        QPoint wp = myGrView->mapFromScene( j_item->pos() );
        if ( (wp - mousePoint).manhattanLength() < 8 ) {
            detected = true;
            copySourceItem = j_item;
        }
        j_item->setInputState(false);
    }

    if ( detected ) {
        copySourceItem->setInputState(true);
    } else {
        myGrScene->clearCurrentAttach();
        hideRubberBand(false);
        setMoveWaitingOn(false);
        setPasteWaitingOn(false);
    }

    return detected;
}

void PointEventOperation::slotFindItems()
{
    setPasteWaitingOn(false);

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
        QList<JointData *> jdlist;
        QList<MemberData *> mdlist;
        DATATYPE dtype = currentDataType();

        // 節点のサーチ
        if ( fst.isJflChecked || fst.isJfrChecked || fst.isJnoChecked ) {
            if ( dtype == DATATYPE::TPJOINTLOAD ) {
                showInformationMessage(u8"現在の編集モードでは節点を選択できません。");
                return;
            }
            if ( stype == SCENETYPE::PLANSCENE ) {
                if ( fst.isJflChecked && !fst.jFloors.contains( myGrScene->floorID() ) ) {
                    showInformationMessage(u8"表示中の階は指定の階範囲に含まれません。");
                    return;
                }
                jdlist = FactoryUnifiedData::getInstance(isOutput)->jointListOfFloor( myGrScene->floorID() );
            } else {
                if ( fst.isJfrChecked && !fst.jFrames.contains( myGrScene->frameID() ) ) {
                    showInformationMessage(u8"表示中の通りは指定の通り範囲に含まれません。");
                    return;
                }
                jdlist = FactoryUnifiedData::getInstance(isOutput)->jointListOfFrame( myGrScene->frameID() );
            }

            for ( int i = jdlist.count() - 1; i >= 0; i-- ) {
                JointData *jd = jdlist.at(i);
                if ( stype == SCENETYPE::PLANSCENE ) {
                    if ( fst.isJfrChecked && !jd->containsFrame( fst.jFrames ) ) {
                        jdlist.removeAt(i);
                        continue;
                    }
                } else {
                    if ( fst.isJflChecked && !jd->containsFloor( fst.jFloors ) ) {
                        jdlist.removeAt(i);
                        continue;
                    }
                }
                if ( fst.isJnoChecked ) {
                    int idx = FactoryUnifiedData::getInstance(isOutput)->indexOfJoint(jd) + 1;
                    if ( idx < fst.jnoFrom || idx > fst.jnoTo ) {
                        jdlist.removeAt(i);
                        continue;
                    }
                }
            }
        }

        // 部材のサーチ
        if ( fst.isMtpChecked || fst.isMstChecked || fst.isMflChecked || fst.isMfrChecked ) {

            if ( dtype == DATATYPE::TPJOINT ) {
                showInformationMessage(u8"現在の編集モードでは部材・荷重を選択できません。");
                return;
            }

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

        if ( jdlist.isEmpty() && mdlist.isEmpty() ) {
            showInformationMessage(u8"指定の要素は見つかりませんでした。");
            return;
        }

        if ( !jdlist.isEmpty() ) myGrScene->setJointSelect( jdlist );
        if ( !mdlist.isEmpty() ) myGrScene->setMemberSelect( mdlist );

    }
}

void PointEventOperation::slotDeleteItems()
{
    myGrScene->blockSignals(true);
    int d_count = 0;

    if ( myElementType == ELJOINT ) {

        if ( UnifiedEditingData::getInstance()->currentDeleteAction() == REMAINJOINT ) {

            d_count = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement()).count();
            RemoveJointsCommand *rj_com = new RemoveJointsCommand( myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement()) );
            UnifiedEditingData::getInstance()->pushUndoCommand(rj_com);

        } else {

            UnifiedEditingData::getInstance()->beginUndoStackMacro("remove Joints");
            QList<JointData *> delJoints = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
            QList<JointData *> relateJoints;
            Q_FOREACH ( JointData *jd, delJoints ) {
                Q_FOREACH ( JointData *jjd, jd->linkedJoints() ) {
                    if ( !delJoints.contains(jjd) && !relateJoints.contains(jjd) )
                        relateJoints.append(jjd);
                }
            }
            if ( !delJoints.isEmpty() ) {
                d_count += delJoints.count();
                RemoveJointsCommand *rj_com = new RemoveJointsCommand( delJoints );
                UnifiedEditingData::getInstance()->pushUndoCommand(rj_com);
            }
            QList<JointData *> laterDelJoints;
            Q_FOREACH ( JointData *jd, relateJoints ) {
                if ( jd->isIndependentJoint() ) laterDelJoints.append(jd);
            }
            if ( !laterDelJoints.isEmpty() ) {
                RemoveJointsCommand *rj_com = new RemoveJointsCommand( laterDelJoints );
                UnifiedEditingData::getInstance()->pushUndoCommand(rj_com);
            }
            UnifiedEditingData::getInstance()->endUndoStackMacro();

        }

    } else {

        QList<MemberData *> delMembers = myGrScene->selectedMembers();
        d_count += delMembers.count();

        if ( UnifiedEditingData::getInstance()->currentDeleteAction() == REMAINJOINT ) {
            RemoveMembersCommand *rm_com = new RemoveMembersCommand( delMembers );
            UnifiedEditingData::getInstance()->pushUndoCommand(rm_com);
        } else {
            UnifiedEditingData::getInstance()->beginUndoStackMacro("remove JointLoads");
            QList<JointData *> relateJoints;
            Q_FOREACH ( MemberData *md, delMembers ) {
                Q_FOREACH ( JointData *jd, md->getJointList() ) {
                    if ( !relateJoints.contains(jd) ) relateJoints.append(jd);
                }
            }
            RemoveMembersCommand *rm_com = new RemoveMembersCommand( delMembers );
            UnifiedEditingData::getInstance()->pushUndoCommand(rm_com);

            QList<JointData *> delJoints;
            Q_FOREACH ( JointData *jd, relateJoints ) {
                if ( jd->isIndependentJoint() ) delJoints.append(jd);
            }
            if ( !delJoints.isEmpty() ) {
                RemoveJointsCommand *rj_com = new RemoveJointsCommand( delJoints );
                UnifiedEditingData::getInstance()->pushUndoCommand(rj_com);
            }
            UnifiedEditingData::getInstance()->endUndoStackMacro();
        }

    }

    myGrScene->blockSignals(false);
    myGrScene->sendSelectedItems();

    if ( d_count < 1 ) return;

    QString msg = QString(u8"%1個の").arg(d_count);
    msg += FactoryUnifiedData::getInstance(isOutput)->dataTypeToJapanese( currentDataType() );
    msg += u8"を削除しました。";
    showInformationMessage(msg);
    setWaitingMode();
}

void PointEventOperation::slotDeleteIndependentJoints()
{
    myGrScene->blockSignals(true);

    QList<JointData *> delJoints;
    Q_FOREACH ( JointData *jd, myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement()) ) {
        if ( jd->isIndependentJoint() ) delJoints.append(jd);
    }

    if ( delJoints.isEmpty() ) {
        showInformationMessage(u8"独立節点は見つかりませんでした。");
    } else {
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( delJoints ));
        showInformationMessage(QString(u8"%1個の独立節点を削除しました。").arg(delJoints.count()));
    }

    myGrScene->blockSignals(false);
    myGrScene->sendSelectedItems();
    setWaitingMode();
}

void PointEventOperation::slotCopyItems()
{
    if ( copySourceItem == nullptr ) return;
    QList<QPointF> copyPoints;
    Q_FOREACH ( JointData *jd, myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement()) )
        copyPoints.append( myGrScene->itemPosition(jd) - copySourceItem->pos() );
    UnifiedEditingData::getInstance()->setCopyBuffer(copyPoints);
    setWaitingMode();
}

void PointEventOperation::slotSetMoveWaitingMode()
{
    setMoveWaitingOn(true);
    manageActions->resetActions();
    myGrScene->update();
}

void PointEventOperation::slotSetPasteWaitingMode()
{
    setPasteWaitingOn(true);
    myGrScene->update();
}

void PointEventOperation::moveJoints()
{
    if ( copySourceItem == nullptr ) return;

    UnifiedEditingData::getInstance()->beginUndoStackMacro("move Joints");

    QList<QPointF> plist;
    QPointF pxy = currentGP.getElvQPointF() * 1000.0 - copySourceItem->pos();
    Q_FOREACH ( QGraphicsItem *item, myGrScene->selectedItems() )
        plist.append( item->pos() + pxy );
    QList<GlobalPoint> gplist = scenePointsToGlobalPoints(plist);
    QList<JointData *> jdlist = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());

    ChangeJointsCommand *cj_com = new ChangeJointsCommand(jdlist, gplist);
    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
    if ( cj_com->isSuccessfullyChanged() ) {
        divideMembersByJoints(jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());
        setWaitingMode();
    }

    UnifiedEditingData::getInstance()->endUndoStackMacro();
}

void PointEventOperation::pasteJoints()
{
    UnifiedEditingData::getInstance()->beginUndoStackMacro("paste Joints");

    QList<QPointF> plist;
    Q_FOREACH (QPointF pp, UnifiedEditingData::getInstance()->getPointsCopyBuffer() )
        plist.append(pp + currentGP.getElvQPointF() * 1000.);

    for ( int i = plist.count() - 1; i > -1; i-- ) {
        JointData *jd = myGrScene->searchNearlyJoint( plist.at(i) );
        if ( jd != NULL ) plist.removeAt(i);
    }

    QList<GlobalPoint> gplist = scenePointsToGlobalPoints(plist);
    CreateJointsCommand *cj_com = new CreateJointsCommand(gplist);
    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
    QList<JointData *> jdlist = cj_com->createdJoints();

    if ( !jdlist.isEmpty() ) {
        divideMembersByJoints(jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());
        setPasteWaitingOn(true);
        myGrScene->update();
    }

    UnifiedEditingData::getInstance()->endUndoStackMacro();
}

void PointEventOperation::slotShowChangeJointDialog()
{
    QList<GlobalPoint> gplist = myGrScene->selectedPoints();
    if ( gplist.isEmpty() ) return;
    changeJointDialog->setValues( getChangeJointDialogValues(gplist) );
    changeJointDialog->exec();
}

void PointEventOperation::slotShowAutoAttachDialog()
{
    AutoAttachDialog dialog(myGrView);

    if ( dialog.exec() == QDialog::Accepted ) {

        QList<JointData *> jdlist = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
        if ( jdlist.isEmpty() ) {
            showInformationMessage(u8"節点が選択されていません。");
            return;
        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new CompleteJointsAttachmentCommand(jdlist, dialog.isOverwriteChecked()));
        showInformationMessage(QString(u8"%1個の節点に属性が付加されました。").arg(jdlist.count()));
        setWaitingMode();
    }
}

PLANEJOINTVALUES PointEventOperation::getChangeJointDialogValues( const QList<GlobalPoint> &gplist )
const
{
    if ( gplist.isEmpty() ) return PLANEJOINTVALUES();

    qreal gx = gplist.first().x();
    qreal gy = gplist.first().y();
    qreal gz = gplist.first().z();
    bool eq_gx = true, eq_gy = true, eq_gz = true;

    qreal lx = gplist.first().elvX();
    qreal ly = gplist.first().elvY();
    qreal lz = gplist.first().elvZ();
    bool eq_lx = true, eq_ly = true, eq_lz = true;

    QList<QUuid> floors = gplist.first().getAttachedFloorList();
    QList<QUuid> frames = gplist.first().getAttachedFrameList();
    bool eq_fl = true, eq_fr = true;

    qreal eps = 1.0e-5;
    for (int i = 1; i < gplist.count(); i++) {

        GlobalPoint gp = gplist.at(i);

        if ( eq_gx && qAbs(gp.x() - gx) > eps ) eq_gx = false;
        if ( eq_gy && qAbs(gp.y() - gy) > eps ) eq_gy = false;
        if ( eq_gz && qAbs(gp.z() - gz) > eps ) eq_gz = false;

        if ( eq_lx && qAbs(gp.elvX() - lx) > eps ) eq_lx = false;
        if ( eq_ly && qAbs(gp.elvY() - ly) > eps ) eq_ly = false;
        if ( eq_lz && qAbs(gp.elvZ() - lz) > eps ) eq_lz = false;

        if ( eq_fl ) {
            QList<QUuid> fls = gp.getAttachedFloorList();
            if ( floors.count() != fls.count() ) {
                eq_fl = false;
            } else {
                Q_FOREACH (QUuid id, fls) {
                    if ( !floors.contains(id) ) {
                        eq_fl = false;
                        break;
                    }
                }
            }
        }

        if ( eq_fr ) {
            QList<QUuid> frs = gp.getAttachedFrameList();
            if ( frames.count() != frs.count() ) {
                eq_fr = false;
            } else {
                Q_FOREACH (QUuid id, frs) {
                    if ( !frames.contains(id) ) {
                        eq_fr = false;
                        break;
                    }
                }
            }
        }

    }

    QString s_gx = (eq_gx) ? QString("%1").arg(gx, 0, 'f', 3) : "***" ;
    QString s_gy = (eq_gy) ? QString("%1").arg(gy, 0, 'f', 3) : "***" ;
    QString s_gz = (eq_gz) ? QString("%1").arg(gz, 0, 'f', 3) : "***" ;

    QString s_lx = (eq_lx) ? QString("%1").arg(lx, 0, 'f', 3) : "***" ;
    QString s_ly = (eq_ly) ? QString("%1").arg(ly, 0, 'f', 3) : "***" ;
    QString s_lz = (eq_lz) ? QString("%1").arg(lz, 0, 'f', 3) : "***" ;

    QString s_floors;
    if ( eq_fl ) {
        Q_FOREACH (QUuid id, floors)
            s_floors += FactoryUnifiedData::getInstance(isOutput)->idToName(id, DATATYPE::TPFLOOR) + ",";
    } else {
        s_floors = "***";
    }
    QString s_frames;
    if ( eq_fr ) {
        Q_FOREACH (QUuid id, frames)
            s_frames += FactoryUnifiedData::getInstance(isOutput)->idToName(id, DATATYPE::TPFRAMEP) + ",";
    } else {
        s_frames = "***";
    }

    return PLANEJOINTVALUES( s_gx, s_gy, s_gz, "0.000", "0.000", "0.000",
                             s_lx, s_ly, s_lz, "0.000", "0.000", "0.000",
                             s_floors, s_frames );
}

bool PointEventOperation::changeSelectedJointsGlobalXYZ(bool ok_x, bool ok_y, bool ok_z,
                                                        qreal gx, qreal gy, qreal gz)
{
    if ( ok_x && ok_y && ok_z ) return false;

    QList<GlobalPoint> gplist;
    QList<JointData *> jdlist = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
    Q_FOREACH (JointData *jd, jdlist) {
        GlobalPoint gp;
        qreal xx = ( ok_x ) ? gx : jd->xPos();
        qreal yy = ( ok_y ) ? gy : jd->yPos();
        qreal zz = ( ok_z ) ? gz : jd->zPos();
        gp.setX(xx);
        gp.setY(yy);
        gp.setZ(zz);
        gp.appendAttachedFloor(jd->getAttachedFloorList());
        gp.appendAttachedFrame(jd->getAttachedFrameList());
        gplist.append(gp);
    }

    UnifiedEditingData::getInstance()->beginUndoStackMacro("change Joints");

    ChangeJointsCommand *cj_com = new ChangeJointsCommand(jdlist, gplist);
    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
    if ( !cj_com->isSuccessfullyChanged() ) {
        UnifiedEditingData::getInstance()->endUndoStackMacro();
        return false;
    }

    divideMembersByJoints( jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction() );
    changeJointDialog->setValues( getChangeJointDialogValues(gplist) );

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    return true;
}

bool PointEventOperation::changeSelectedJointsGlobalDelta(bool ok_x, bool ok_y, bool ok_z,
                                                          qreal dx, qreal dy, qreal dz)
{
    if ( ok_x && ok_y && ok_z ) return false;

    QList<GlobalPoint> gplist;
    QList<JointData *> jdlist = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
    Q_FOREACH (JointData *jd, jdlist) {
        GlobalPoint gp;
        qreal xx = ( ok_x ) ? jd->xPos() + dx : jd->xPos();
        qreal yy = ( ok_y ) ? jd->yPos() + dy : jd->yPos();
        qreal zz = ( ok_z ) ? jd->zPos() + dz : jd->zPos();
        gp.setX(xx);
        gp.setY(yy);
        gp.setZ(zz);
        gp.appendAttachedFloor(jd->getAttachedFloorList());
        gp.appendAttachedFrame(jd->getAttachedFrameList());
        gplist.append(gp);
    }

    UnifiedEditingData::getInstance()->beginUndoStackMacro("change Joints");

    ChangeJointsCommand *cj_com = new ChangeJointsCommand(jdlist, gplist);
    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
    if ( !cj_com->isSuccessfullyChanged() ) {
        UnifiedEditingData::getInstance()->endUndoStackMacro();
        return false;
    }

    divideMembersByJoints(jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());
    changeJointDialog->setValues( getChangeJointDialogValues(gplist) );

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    return true;
}

bool PointEventOperation::changeSelectedJointsLocalXYZ(bool changeAttach,
                                                       bool ok_x, bool ok_y, bool ok_z,
                                                       qreal lx, qreal ly, qreal lz)
{
    if ( ok_x && ok_y && ok_z ) return false;

    QList<GlobalPoint> gplist;
    QList<JointData *> jdlist = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());

    QList<GlobalPoint> s_gplist = myGrScene->selectedPoints();
    Q_FOREACH (GlobalPoint gp, s_gplist) {
        qreal xx = ( ok_x ) ? lx : gp.elvX();
        qreal yy = ( ok_y ) ? ly : gp.elvY();
        qreal zz = ( ok_z ) ? lz : gp.elvZ();
        gp.setElvX(xx);
        gp.setElvY(yy);
        gp.setElvZ(zz);
        QList<QUuid> floors = gp.getAttachedFloorList();
        QList<QUuid> frames = gp.getAttachedFrameList();
        gp.clearAttachedFloor();
        gp.clearAttachedFrame();
        myGrScene->completeGlobalXYZ(gp);

        if (!changeAttach) {
            gp.clearAttachedFloor();
            gp.clearAttachedFrame();
            gp.appendAttachedFloor(floors);
            gp.appendAttachedFrame(frames);
        }
        gplist.append(gp);
    }

    UnifiedEditingData::getInstance()->beginUndoStackMacro("change Joints");

    ChangeJointsCommand *cj_com = new ChangeJointsCommand(jdlist, gplist);
    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
    if ( !cj_com->isSuccessfullyChanged() ) {
        UnifiedEditingData::getInstance()->endUndoStackMacro();
        return false;
    }

    divideMembersByJoints(jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());
    changeJointDialog->setValues( getChangeJointDialogValues(gplist) );

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    return true;
}

bool PointEventOperation::changeSelectedJointsLocalDelta(bool changeAttach,
                                                         bool ok_x, bool ok_y, bool ok_z,
                                                         qreal dx, qreal dy, qreal dz)
{
    if ( ok_x && ok_y && ok_z ) return false;

    QList<GlobalPoint> gplist;
    QList<JointData *> jdlist = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());

    QList<GlobalPoint> s_gplist = myGrScene->selectedPoints();
    Q_FOREACH (GlobalPoint gp, s_gplist) {
        qreal xx = ( ok_x ) ? gp.elvX() + dx : gp.elvX();
        qreal yy = ( ok_y ) ? gp.elvY() + dy : gp.elvY();
        qreal zz = ( ok_z ) ? gp.elvZ() + dz : gp.elvZ();
        gp.setElvX(xx);
        gp.setElvY(yy);
        gp.setElvZ(zz);
        QList<QUuid> floors = gp.getAttachedFloorList();
        QList<QUuid> frames = gp.getAttachedFrameList();
        gp.clearAttachedFloor();
        gp.clearAttachedFrame();
        myGrScene->completeGlobalXYZ(gp);

        if (!changeAttach) {
            gp.clearAttachedFloor();
            gp.clearAttachedFrame();
            gp.appendAttachedFloor(floors);
            gp.appendAttachedFrame(frames);
        }
        gplist.append(gp);
    }

    UnifiedEditingData::getInstance()->beginUndoStackMacro("change Joints");

    ChangeJointsCommand *cj_com = new ChangeJointsCommand(jdlist, gplist);
    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
    if ( !cj_com->isSuccessfullyChanged() ) {
        UnifiedEditingData::getInstance()->endUndoStackMacro();
        return false;
    }

    divideMembersByJoints(jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());
    changeJointDialog->setValues( getChangeJointDialogValues(gplist) );

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    return true;
}

bool PointEventOperation::changeSelectedJointsAttachment(bool ok_fl, bool ok_fr,
                                                         const QString &s_floors, const QString &s_frames)
{
    QList<QUuid> fl_idlist, fr_idlist;
    if ( ok_fl ) {
        QStringList strlist = s_floors.split(",");
        if ( strlist.last().isEmpty() ) strlist.removeLast();
        Q_FOREACH ( QString str, strlist ) {
            QUuid id = FactoryUnifiedData::getInstance(isOutput)->nameToID(str, DATATYPE::TPFLOOR);
            if ( id.isNull() ) return false;
            if ( !fl_idlist.contains(id) ) fl_idlist.append(id);
        }
    }
    if ( ok_fr ) {
        QStringList strlist = s_frames.split(",");
        if ( strlist.last().isEmpty() ) strlist.removeLast();
        Q_FOREACH ( QString str, strlist ) {
            QUuid id = FactoryUnifiedData::getInstance(isOutput)->nameToID(str, DATATYPE::TPFRAMEP);
            if ( id.isNull() ) return false;
            if ( !fr_idlist.contains(id) ) fr_idlist.append(id);
        }
    }

    QList<JointData *> jdlist = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
    if ( ok_fl ) {
        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeJointsAttachmentCommand(jdlist, fl_idlist, DATATYPE::TPFLOOR));
    }
    if ( ok_fr ) {
        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeJointsAttachmentCommand(jdlist, fr_idlist, DATATYPE::TPFRAMEP));
    }
    changeJointDialog->setValues( getChangeJointDialogValues(myGrScene->selectedPoints()) );
    return true;
}

void PointEventOperation::slotChangeSection()
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
            showInformationMessage(u8"対象の荷重が選択されていません。");
            return;
        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeMemberSectionCommand(mdlist, str));
        showInformationMessage(QString(u8"%1個の荷重について符号を変更しました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void PointEventOperation::slotDivideMember()
{
    divideMembersByJoints(myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement()), AMINTER);
}

void PointEventOperation::moveMemberJoints()
{
    MemberData *md = myGrScene->selectedMember();
    JointData *jd = myGrScene->searchNearlyJoint(currentGP.getElvQPointF() * 1000.);
    JointData *s_jd = moveSourceJoint;

    if ( s_jd == NULL ) return;

    if ( md == NULL || jd == NULL ) {
        showWarningMessage(u8"移動先に節点が存在しません。");
        return;
    }

    UnifiedEditingData::getInstance()->beginUndoStackMacro("move Member Joints");

    bool isOverlap = FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(QList<JointData *>{jd}, DATATYPE::TPJOINTLOAD);
    if ( !isOverlap ) {
        UnifiedEditingData::getInstance()->pushUndoCommand(new ReplaceMemberJointCommand(md, s_jd, jd));

        if ( UnifiedEditingData::getInstance()->currentDeleteAction() == DELETEJOINT ) {
            myGrScene->blockSignals(true);
            if ( s_jd->isIndependentJoint() ) {
                UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( QList<JointData *>{s_jd}));
            }
            myGrScene->blockSignals(false);
        }
    }

    myGrScene->sendSelectedItems();
    setWaitingMode();

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    if ( isOverlap ) showInformationMessage(u8"重複する荷重があります。");
}
} // namespace post3dapp
