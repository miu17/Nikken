#include "eventoperation_editall.h"

#include <QtGui>
#include <QDebug>

#include "autoattach_dialog.h"
#include "calc_vector2d.h"
#include "changeangle_dialog.h"
#include "changeface_dialog.h"
#include "changeopen_dialog.h"
#include "changeplanejoint_dialog.h"
#include "changesection_dialog.h"
#include "changeshift_dialog.h"
#include "customdraw_grview.h"
#include "define_draw_figure.h"
#include "factory_unifieddata.h"
#include "finditem_dialog.h"
#include "input_grscene.h"
#include "jointitem.h"
#include "manage_editallactions.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "operation_commands.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "unified_settings.h"

namespace post3dapp{
EditAllEventOperation::EditAllEventOperation(CustomDrawGrView *view)
    : AbstractEventOperation(view)
{
    hideRubberBand(false);
    setMoveWaitingOn(false);
    setPasteWaitingOn(false);

    manageActions = new ManageEditAllActions(this);
    changeJointDialog = new ChangePlaneJointDialog(this, view);

    copySourceItem = nullptr;

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentStatusChanged, this, &EditAllEventOperation::reloadSettings);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::selectTermChanged, this, &EditAllEventOperation::reloadSettings);

//    connect(myGrScene,SIGNAL(itemSelectChanged()),this,SLOT(slotChangeActionStatus()));
}

void EditAllEventOperation::reloadSettings()
{
    if ( UnifiedEditingData::getInstance()->currentElement() != ALLELEMENT ) return;
    initializeSettings();
}

void EditAllEventOperation::initializeSettings()
{
    setWaitingMode();
    myGrScene->setItemSelectLimited(UnifiedEditingData::getInstance()->currentLimitSelectTerm());
    manageActions->resetActions();
    copySourceItem = nullptr;
}

void EditAllEventOperation::slotChangeActionStatus()
{
    QList<DATATYPE> dtypes;
    if ( !myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement()).isEmpty() ) dtypes.append(DATATYPE::TPJOINT);
    Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() ) {
        if ( !dtypes.contains(md->dataType()) ) dtypes.append(md->dataType());
    }
    bool existBuffer = UnifiedEditingData::getInstance()->existCopyBuffer(FactoryUnifiedData::getInstance(isOutput));
    manageActions->setActionStatus( dtypes, isMoveWaiting(),
                                    existBuffer, setWaitingPoints(QPoint(-100, -100)) );
}

void EditAllEventOperation::setWaitingMode()
{
    myGrView->clearSelectedItems();
    myGrScene->clearCurrentAttach();
    hideRubberBand(false);
    setMoveWaitingOn(false);
    setPasteWaitingOn(false);
    copySourceItem = NULL;
}

void EditAllEventOperation::toggleViewMode()
{
    setViewModeOn( !viewMode );
    if ( isViewModeOn() ) setWaitingMode();
}

bool EditAllEventOperation::isBatchInputMode() const
{
    return false;
}

bool EditAllEventOperation::isSelectItemMode() const
{
    if ( isMoveWaiting() || isPasteWaiting() ) return false;
    return ( !isViewModeOn() && isMousePressed() );
}

bool EditAllEventOperation::isPositionSnapOn() const
{
    return ( isMoveWaiting() || isPasteWaiting() ) ;
}

bool EditAllEventOperation::isMoveWaiting() const
{
    return onMoveWaiting ;
}

void EditAllEventOperation::setMoveWaitingOn(bool on)
{
    onMoveWaiting = on;
}

bool EditAllEventOperation::isPasteWaiting() const
{
    return onPasteWaiting ;
}

void EditAllEventOperation::setPasteWaitingOn(bool on)
{
    onPasteWaiting = on;
    if ( onPasteWaiting ) myGrScene->clearSelection();
}


void EditAllEventOperation::execDrawForeground(QPainter *painter, const QRect &vrect)
{
//    if( !isPositionSnapOn() && !isBatchMode() ) return;
    if ( !isPositionSnapOn() ) return;

    painter->setTransform(QTransform(1, 0, 0, 1, 0, 0), false);
    qreal factor = qPow(myGrView->transform().m11(), 0.25);
    int dd = int(factor * 10.0);

    AbstractEventOperation::execDrawForeground(painter, vrect);

    if ( !isMoveWaiting() && !isPasteWaiting() ) return;

    painter->setBrush(QBrush(QColor(200, 255, 200)));
    painter->setPen(QPen(QBrush(QColor(0, 255, 50)), 1, Qt::DashLine));
    QPointF q1 = currentGP.getElvQPointF() * 1000.;
    painter->setRenderHint(QPainter::Antialiasing, true);

    if ( isPasteWaiting() ) {
        if ( !UnifiedEditingData::getInstance()->existCopyBuffer(FactoryUnifiedData::getInstance(isOutput)) ) {
            setWaitingMode();
            return;
        }

        QList<QPoint> wplist;
        Q_FOREACH ( QPointF pp, UnifiedEditingData::getInstance()->getPointsCopyBuffer() ) {
            QPoint qq = myGrView->mapFromScene(pp + q1);
            painter->drawEllipse(qq, dd, dd);
            wplist.append(qq);
        }
        painter->setRenderHint(QPainter::Antialiasing, false);
        QList<JointData *> jdlist = UnifiedEditingData::getInstance()->getJointsCopyBuffer(FactoryUnifiedData::getInstance(isOutput));
        Q_FOREACH ( MemberData *md, UnifiedEditingData::getInstance()->getMembersCopyBuffer(FactoryUnifiedData::getInstance(isOutput)) ) {
            if ( md == NULL ) continue;
            QList<JointData *> r_jdlist = md->getJointList();
            if ( r_jdlist.count() < 2 ) continue;
            for ( int i = 1; i < r_jdlist.count(); i++ ) {
                int ii = jdlist.indexOf( r_jdlist.at(i - 1) );
                int jj = jdlist.indexOf( r_jdlist.at(i) );
                if ( ii >= 0 && jj >= 0 ) painter->drawLine( wplist.at(ii), wplist.at(jj) );
            }
            if ( md->isClosedMember() ) {
                int ii = jdlist.indexOf( r_jdlist.last() );
                int jj = jdlist.indexOf( r_jdlist.first() );
                if ( ii >= 0 && jj >= 0 ) painter->drawLine( wplist.at(ii), wplist.at(jj) );
            }
        }
    }

    if ( isMoveWaiting() ) {
        Q_FOREACH (QGraphicsItem *item, myGrScene->selectedItems()) {
            QPoint qq = myGrView->mapFromScene(item->pos() - copySourceItem->pos() + q1);
            painter->drawEllipse(qq, dd, dd);
        }
        painter->setRenderHint(QPainter::Antialiasing, false);
        Q_FOREACH ( QLineF ll, myGrScene->getMoveWaitingLines(copySourceItem, q1, UnifiedEditingData::getInstance()->currentElement()) ) {
            painter->drawLine( myGrView->mapFromScene(ll.p1()), myGrView->mapFromScene(ll.p2()) );
        }
    }
}

void EditAllEventOperation::execMousePress(QMouseEvent *event)
{
    if ( myGrScene->sceneType() == SCENETYPE::PLANSCENE && myGrScene->floorID().isNull() ) return;
    if ( myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE && myGrScene->frameID().isNull() ) return;

    mousePressOn(true);

    if ( isMoveWaiting() ) {
        if ( event->button() == Qt::LeftButton ) {
            moveJoints();
        } else if ( event->button() == Qt::RightButton ) {
            setWaitingMode();
        }
        mousePressOn(false);
        return;
    }

    if ( isPasteWaiting() ) {
        if ( event->button() == Qt::LeftButton ) {
            pasteCopyBuffer();
        } else if ( event->button() == Qt::RightButton ) {
            setWaitingMode();
        }
        mousePressOn(false);
        return;
    }

    if ( !isViewModeOn() &&  event->button() == Qt::RightButton ) {

        QList<DATATYPE> dtypes;
        if ( !myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement()).isEmpty() ) dtypes.append(DATATYPE::TPJOINT);
        Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() ) {
            if ( !dtypes.contains(md->dataType()) ) dtypes.append(md->dataType());
        }
        bool existBuffer = UnifiedEditingData::getInstance()->existCopyBuffer(FactoryUnifiedData::getInstance(isOutput));
        manageActions->setActionStatus(dtypes, isMoveWaiting(), existBuffer, setWaitingPoints(event->pos()));
        manageActions->showContextMenu();
    }
    /*
        if( isMoveWaiting() && event->button() == Qt::LeftButton ){
            moveJoints();
            mousePressOn(false);
        }

        if( isPasteWaiting() && event->button() == Qt::LeftButton ){
            pasteCopyBuffer();
            mousePressOn(false);
        }
    */
}

void EditAllEventOperation::execMouseRelease(QMouseEvent *)
{
    mousePressOn(false);
}

void EditAllEventOperation::execKeyPress(QKeyEvent *)
{
    if ( isMoveWaiting() || isPasteWaiting() ) setWaitingMode();
    /*
        QList<DATATYPE> dtypes;
        if( !myGrScene->selectedJoints().isEmpty() ) dtypes.append(TPJOINT);
        Q_FOREACH( MemberData* md, myGrScene->selectedMembers() ){
            if( !dtypes.contains(md->dataType()) ) dtypes.append(md->dataType());
        }
        bool existBuffer=FactoryUnifiedData::getInstance(isOutput)->existCopyBuffer();
        manageActions->setActionStatus( dtypes, isMoveWaiting(),
                                        existBuffer, setWaitingPoints(QPoint(-100,-100)) );
    */
}

void EditAllEventOperation::positionSnap(const QPoint &cur_p, bool)
{
    AbstractEventOperation::positionSnap(cur_p, true);
}

bool EditAllEventOperation::existNearlyPoint(QPoint )
{
    return false;
}

void EditAllEventOperation::slotFindItems()
{
    setPasteWaitingOn(false);

    FindItemDialog dialog(DATATYPE::NODATATYPE, myGrView,myGrView->askIsOutput());

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

        // 節点のサーチ
        if ( fst.isJflChecked || fst.isJfrChecked || fst.isJnoChecked ) {

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

            if ( stype == SCENETYPE::PLANSCENE ) {
                mdlist = FactoryUnifiedData::getInstance(isOutput)->memberListOfFloor( myGrScene->floorID() );
            } else {
                mdlist = FactoryUnifiedData::getInstance(isOutput)->memberListOfFrame( myGrScene->frameID() );
            }

            for ( int i = mdlist.count() - 1; i >= 0; i-- ) {
                MemberData *md = mdlist.at(i);
                if ( fst.isMtpChecked && md->dataType() != fst.dataType ) {
                    mdlist.removeAt(i);
                    continue;
                }
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

        myGrScene->setJointSelect( jdlist );
        myGrScene->setMemberSelect( mdlist );

    }
}

void EditAllEventOperation::slotDeleteItems()
{
    QList<MemberData *> select_members = myGrScene->selectedMembers();
    QList<JointData *> select_joints = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
    int m_count = select_members.count();
    int j_count = select_joints.count();

    if ( m_count == 0 && j_count == 0 ) {
        showInformationMessage(u8"節点または部材が選択されていません。");
        return;
    }

    myGrScene->blockSignals(true);
    myGrView->clearSelectedItems();

    UnifiedEditingData::getInstance()->beginUndoStackMacro("delete Joints And Members");

    if ( UnifiedEditingData::getInstance()->currentDeleteAction() == REMAINJOINT ) {
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( select_joints ));

        for ( int i = select_members.count() - 1; i >= 0; i-- ) {
            MemberData *md = select_members.at(i);
            if ( !FactoryUnifiedData::getInstance(isOutput)->isExistedMember(md) ) select_members.removeAt(i);
        }
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveMembersCommand(select_members));

    } else {

        QList<JointData *> relateJoints;
        Q_FOREACH ( JointData *jd, select_joints ) {
            Q_FOREACH ( JointData *jjd, jd->linkedJoints() ) {
                if ( !select_joints.contains(jjd) && !relateJoints.contains(jjd) )
                    relateJoints.append(jjd);
            }
        }
        Q_FOREACH ( MemberData *md, select_members ) {
            Q_FOREACH ( JointData *jjd, md->getJointList() ) {
                if ( !select_joints.contains(jjd) && !relateJoints.contains(jjd) )
                    relateJoints.append(jjd);
            }
        }
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( select_joints ));
        for ( int i = select_members.count() - 1; i >= 0; i-- ) {
            MemberData *md = select_members.at(i);
            if ( !FactoryUnifiedData::getInstance(isOutput)->isExistedMember(md) ) select_members.removeAt(i);
        }
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveMembersCommand(select_members));

        QList<JointData *> delJoints;
        Q_FOREACH ( JointData *jd, relateJoints ) {
            if ( jd->isIndependentJoint() ) delJoints.append(jd);
        }
        if ( !delJoints.isEmpty() ) {
            j_count += delJoints.count();
            UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( delJoints ));
        }
    }

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    myGrScene->blockSignals(false);
    myGrScene->sendSelectedItems();

    showInformationMessage(QString(u8"%1個の節点、%2個の部材を削除しました。").arg(j_count).arg(m_count));
    setWaitingMode();
}

void EditAllEventOperation::slotCopyItems()
{
    if ( copySourceItem == NULL ) return;

    QList<JointData *> select_joints = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
    if ( select_joints.isEmpty() ) return;
    QList<QPointF> select_points;
    Q_FOREACH ( JointData *jd, select_joints )
        select_points.append( myGrScene->itemPosition(jd) - copySourceItem->pos() );

    QList<MemberData *> select_members;
    QMultiHash<int, MemberData *> select_mdtable;
    Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() ) {
        bool isOk = true;
        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            if ( !select_joints.contains(jd) ) {
                isOk = false;
                break;
            }
        }
        if ( isOk ) {
            select_members.append(md);
            select_mdtable.insert(static_cast<int>(md->dataType()), md);
        }
    }


    // Log Message
    QString lmsg = "copy Joints and Members. \"";
    lmsg += QString("Joint:%1").arg(select_joints.count());
    Q_FOREACH ( int dtype, select_mdtable.uniqueKeys() ) {
        lmsg += QString(", ") + FactoryUnifiedData::getInstance(isOutput)->dataTypeToEnglish(static_cast<DATATYPE>(dtype))
                + QString(":%1").arg(select_mdtable.values(dtype).count());
    }
    lmsg += "\"";
    UnifiedEditingData::getInstance()->sendLogMessage(lmsg);
    UnifiedEditingData::getInstance()->setCopyBuffer(select_points, select_joints, select_members);
    setWaitingMode();
}

bool EditAllEventOperation::setWaitingPoints(QPoint mousePoint)
{
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
        copySourceItem = NULL;
    }

    return detected;
}

void EditAllEventOperation::slotSetMoveWaitingMode()
{
    setMoveWaitingOn(true);
    manageActions->resetActions();
    myGrScene->update();
}

/*
bool EditAllEventOperation::setMoveWaitingJoints(QPoint mousePoint)
{
    if( mousePoint.x()<0 || mousePoint.y()<0 ){
        waitingJoints=JOINTGROUP();
        return false;
    }

    QPointF pp=myGrView->mapToScene( mousePoint );
    waitingJoints=myGrScene->selectedMemberJoints(pp);
    return ( waitingJoints.j2!=NULL );
}
*/

void EditAllEventOperation::moveJoints()
{
    if ( copySourceItem == NULL ) return;

    UnifiedEditingData::getInstance()->beginUndoStackMacro("move Joints");

    QList<JointData *> jdlist = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
    QList<QPointF> plist;
    QPointF pxy = currentGP.getElvQPointF() * 1000.0 - copySourceItem->pos();
    Q_FOREACH (JointData *jd, jdlist)
        plist.append( myGrScene->jointDataToJointItem(jd)->pos() + pxy );
    QList<GlobalPoint> gplist = scenePointsToGlobalPoints(plist);

    ChangeJointsCommand *cj_com = new ChangeJointsCommand(jdlist, gplist);
    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
    if ( cj_com->isSuccessfullyChanged() ) {
        divideMembersByJoints(jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());
        setWaitingMode();
    }

    UnifiedEditingData::getInstance()->endUndoStackMacro();
}

void EditAllEventOperation::slotSetPasteWaitingMode()
{
    setPasteWaitingOn(true);
    myGrScene->update();
}

void EditAllEventOperation::pasteCopyBuffer()
{
    QList<JointData *> source_jdlist = UnifiedEditingData::getInstance()->getJointsCopyBuffer(FactoryUnifiedData::getInstance(isOutput));
    QList<MemberData *> source_mdlist = UnifiedEditingData::getInstance()->getMembersCopyBuffer(FactoryUnifiedData::getInstance(isOutput));

    UnifiedEditingData::getInstance()->beginUndoStackMacro("paste Copy Buffer");
    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalElementCountChangedCommand(false));

    QList<JointData *> new_jdlist;
    QList<JointData *> dest_jdlist;

    Q_FOREACH ( QPointF pp, UnifiedEditingData::getInstance()->getPointsCopyBuffer() ) {
        QPointF p1 = pp + currentGP.getElvQPointF() * 1000.0;
        JointData *jd = myGrScene->searchNearlyJoint(p1);
        if ( jd == NULL ) {
            GlobalPoint gp = scenePointsToGlobalPoints( QList<QPointF>() << p1 ).first();
            CreateJointCommand *cj_com = new CreateJointCommand(gp, false);
            UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
            jd = cj_com->createdJoint();
            new_jdlist.append(jd);
        }
        dest_jdlist.append(jd);
    }

    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalJointAppendedCommand(new_jdlist));

    QList<QList<JointData *> > create_jdlist;
    QList<MEMBERVALUES> create_values;

    QList<MemberData *> cp_source_mdlist;
    QList<QList<JointData *> > cp_dest_jdlist;
    QList<MEMBERVALUES> cp_dest_mvlist;

    ADDMOVEDACTION amd_act = UnifiedEditingData::getInstance()->currentAddMovedAction();

    Q_FOREACH ( MemberData *md, source_mdlist ) {

        if ( md == NULL ) continue;
        QList<JointData *> d_jdlist;
        Q_FOREACH ( JointData *jd, md->getJointList() ) {
            int idx = source_jdlist.indexOf(jd);
            if ( idx < 0 ) continue; // paste により source_md->jointList が変化するケース
            d_jdlist.append( dest_jdlist.at(idx) );
        }

        if (amd_act == AMDINTER) {
            d_jdlist = myGrScene->searchBetweenJoints(d_jdlist);
        }

        MemberData *overlapMember = NULL;

        JointData *first_jd = ( md->isClosedMember() )
                              ? FactoryUnifiedData::getInstance(isOutput)->simplifiedMemberJoints(d_jdlist, true).first()
                              : d_jdlist.first();

        Q_FOREACH ( MemberData *r_md, first_jd->getRelatedMembers() ) {
            if ( md->dataType() != r_md->dataType() ) continue;
            if ( FactoryUnifiedData::getInstance(isOutput)->isOverlapJoints( d_jdlist, r_md->getJointList(),
                                                              r_md->isClosedMember() ) ) {
                overlapMember = r_md;
                break;
            }
        }

        if ( overlapMember == NULL ) {
            create_jdlist.append(d_jdlist);
            create_values.append(md->getMemberValues());
        } else {
            cp_source_mdlist.append(overlapMember);
            cp_dest_jdlist.append(d_jdlist);
            cp_dest_mvlist.append(md->getMemberValues());
        }
    }

    divideMembersByJoints(new_jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());

    UnifiedEditingData::getInstance()->pushUndoCommand(new CopyMembersValuesCommand(cp_source_mdlist, cp_dest_jdlist, cp_dest_mvlist));
    UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMembersMixedCommand(create_jdlist, create_values));
    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalElementCountChangedCommand( true ));
    UnifiedEditingData::getInstance()->endUndoStackMacro();

    // Log Message
    QString lmsg = "paste Joints and Members. \"";
    lmsg += QString("new Joints:%1, new Members:%2, changed Members:%3")
            .arg(new_jdlist.count()).arg(create_jdlist.count()).arg(cp_source_mdlist.count());
    /*
        Q_FOREACH( DATATYPE dtype, new_mdtable.uniqueKeys() ){
            lmsg+=QString(", new ")+FactoryUnifiedData::getInstance(isOutput)->dataTypeToEnglish(dtype)
                    +QString(":%1").arg(new_mdtable.values(dtype).count());
        }
        Q_FOREACH( DATATYPE dtype, overwrite_mdtable.uniqueKeys() ){
            lmsg+=QString(", overwrite ")+FactoryUnifiedData::getInstance(isOutput)->dataTypeToEnglish(dtype)
                    +QString(":%1").arg(overwrite_mdtable.values(dtype).count());
        }
    */
    lmsg += "\"";
    UnifiedEditingData::getInstance()->sendLogMessage(lmsg);

    setPasteWaitingOn(true);
    myGrScene->update();
}

void EditAllEventOperation::slotChangeSection()
{
    ChangeSectionDialog dialog(myGrView,isOutput);

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

void EditAllEventOperation::slotReverseMemberJoint()
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

void EditAllEventOperation::slotChangeAngle()
{
    QList<MemberData *> mdlist;
    Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() )
        if ( md->isLineMember(false) ) mdlist.append(md);
    if ( mdlist.isEmpty() ) {
        showInformationMessage(u8"操作可能な部材が選択されていません。");
        return;
    }
    bool fixed = true;
    qreal ag = mdlist.first()->codeAngle();
    Q_FOREACH ( MemberData *md, mdlist )
        if ( qAbs( ag - md->codeAngle() ) > 1.0e-3 ) {
            fixed = false;
            break;
        }

    QString str_angle = ( fixed ) ? QString("%1").arg(ag * 180.0 / M_PI, 0, 'f', 3) : QString("***") ;

    ChangeAngleDialog dialog( str_angle, myGrView );

    if ( dialog.exec() == QDialog::Accepted ) {
        QString value_str = dialog.valueText();
        bool ok = true;
        qreal ag = value_str.toDouble(&ok);
        if ( value_str == "***" ) {
            showInformationMessage(u8"コードアングルは変更されませんでした。");
            return;
        }
        if ( !ok ) {
            warningInvalidValue();
            return;
        }

        qreal eps = 1.0e-5;
        bool inRange = ( ag > -180.0 - eps && ag < 180.0 + eps );
        while ( !inRange ) {
            if ( ag < -180.0 ) ag += 360.0;
            if ( ag > 180.0 ) ag -= 360.0;
            inRange = ( ag > -180.0 - eps && ag < 180.0 + eps );
        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeCodeAngleCommand(mdlist, ag * M_PI / 180.0));
        showInformationMessage(QString(u8"%1個の部材についてコードアングルを変更しました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void EditAllEventOperation::slotChangeShift()
{
    QList<MemberData *> mdlist;
    Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() )
        if ( md->dataType() != DATATYPE::TPJOINTLOAD && md->dataType() != DATATYPE::TPMEMBERLOAD
                && md->dataType() != DATATYPE::TPSLABLOAD
                && md->dataType() != DATATYPE::TPDAMPER && md->dataType() != DATATYPE::TPISO ) mdlist.append(md);
    if ( mdlist.count() < 1 ) {
        showInformationMessage(u8"操作可能な部材が選択されていません。");
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
        showInformationMessage(QString(u8"%1個の部材について寄り長さを変更しました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void EditAllEventOperation::slotChangeFace()
{
    QList<MemberData *> mdlist;
    Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() )
        if ( md->isLineMember(false) ) mdlist.append(md);
    if ( mdlist.count() < 1 ) {
        showInformationMessage(u8"操作可能な部材が選択されていません。");
        return;
    }

    MemberData *mfirst = mdlist.first();

    FACERULE iface_rule = mfirst->getIFaceRule();
    qreal iface_len = mfirst->getIFace();
    FACERULE jface_rule = mfirst->getJFaceRule();
    qreal jface_len = mfirst->getJFace();
    bool ok_irule = true, ok_ilen = true, ok_jrule = true, ok_jlen = true;
    qreal eps = 1.0e-3;

    Q_FOREACH (MemberData *md, mdlist) {
        if ( md == mfirst ) continue;
        if ( ok_irule ) ok_irule = ( md->getIFaceRule() == iface_rule ) ;
        if ( ok_ilen ) ok_ilen = ( qAbs( iface_len - md->getIFace() ) < eps ) ;
        if ( ok_jrule ) ok_jrule = ( md->getJFaceRule() == jface_rule ) ;
        if ( ok_jlen ) ok_jlen = ( qAbs( jface_len - md->getJFace() ) < eps ) ;
    }

    int i_id = 0, j_id = 0;

    if ( ok_irule ) {
        if ( iface_rule == FACERULE::FACE_AUTO ) {
            i_id = 0;
        } else {
            i_id = 1;
        }
    } else {
        i_id = 2;
    }
    QString str_ilen = ( ok_ilen ) ? QString("%1").arg(iface_len, 0, 'f', 3) : QString("***") ;

    if ( ok_jrule ) {
        if ( jface_rule == FACERULE::FACE_AUTO ) {
            j_id = 0;
        } else {
            j_id = 1;
        }
    } else {
        j_id = 2;
    }
    QString str_jlen = ( ok_jlen ) ? QString("%1").arg(jface_len, 0, 'f', 3) : QString("***") ;

    ChangeFaceDialog dialog(i_id, str_ilen, j_id, str_jlen, myGrView);

    if ( dialog.exec() == QDialog::Accepted ) {
        bool ir_chg = true, il_chg = true, jr_chg = true, jl_chg = true;
        qreal i_value, j_value;

        int i_id = dialog.ifaceRuleCheckedID();
        QString i_str = dialog.ifaceValueText();
        if ( i_id == 1 ) {
            bool ok = true;
            i_value = i_str.toDouble(&ok);
            if ( i_str == "***" ) {
                il_chg = false;
            } else if ( !ok ) {
                warningInvalidValue();
                return;
            }
        } else if ( i_id == 2 ) {
            ir_chg = false;
            il_chg = false;
        } else if ( i_id == 1 ) {
            il_chg = false;
        }

        int j_id = dialog.jfaceRuleCheckedID();
        QString j_str = dialog.jfaceValueText();
        if ( j_id == 1 ) {
            bool ok = true;
            j_value = j_str.toDouble(&ok);
            if ( j_str == "***" ) {
                jl_chg = false;
            } else if ( !ok ) {
                warningInvalidValue();
                return;
            }
        } else if ( j_id == 2 ) {
            jr_chg = false;
            jl_chg = false;
        } else if ( j_id == 1 ) {
            jl_chg = false;
        }
        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeFaceCommand(mdlist, ir_chg, i_id, il_chg, i_value, jr_chg, j_id, jl_chg, j_value));
        showInformationMessage(QString(u8"%1個の部材についてフェイス長さを変更しました。").arg(mdlist.count()));
        setWaitingMode();
    }
}


void EditAllEventOperation::slotDeleteIndependentJoints()
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

void EditAllEventOperation::slotShowAutoAttachDialog()
{
    AutoAttachDialog dialog(myGrView);

    if ( dialog.exec() == QDialog::Accepted ) {

        QList<JointData *> jdlist = myGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
        if ( jdlist.isEmpty() ) {
            showInformationMessage(u8"節点が選択されていません。");
            return;
        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new CompleteJointsAttachmentCommand(jdlist, dialog.isOverwriteChecked()));
        showInformationMessage(QString(u8"%1個の節点に属性が付加されました").arg(jdlist.count()));
        setWaitingMode();

    }
}

void EditAllEventOperation::slotShowChangeJointDialog()
{
    QList<GlobalPoint> gplist = myGrScene->selectedPoints();
    if ( gplist.isEmpty() ) return;
    changeJointDialog->setValues( getChangeJointDialogValues(gplist) );
    changeJointDialog->exec();
}

PLANEJOINTVALUES EditAllEventOperation::getChangeJointDialogValues( const QList<GlobalPoint>
                                                                    &gplist ) const
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

bool EditAllEventOperation::changeSelectedJointsGlobalXYZ(bool ok_x, bool ok_y, bool ok_z,
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

bool EditAllEventOperation::changeSelectedJointsGlobalDelta(bool ok_x, bool ok_y, bool ok_z,
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

bool EditAllEventOperation::changeSelectedJointsLocalXYZ(bool changeAttach,
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

bool EditAllEventOperation::changeSelectedJointsLocalDelta(bool changeAttach,
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

bool EditAllEventOperation::changeSelectedJointsAttachment(bool ok_fl, bool ok_fr,
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

void EditAllEventOperation::slotChangeWallOpen()
{
    ChangeOpenDialog dialog(myGrView,isOutput);

    if ( dialog.exec() == QDialog::Accepted ) {
        QString name = dialog.currentName();

        QList<MemberData *> mdlist;
        Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() )
            if ( md->dataType() == DATATYPE::TPWALL || md->dataType() == DATATYPE::TPUWALL ) mdlist.append(md);
        if ( mdlist.isEmpty() ) {
            showInformationMessage(u8"壁または地下外壁が選択されていません。");
            return;
        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeWallOpenCommand(mdlist, name));
        showInformationMessage(QString(u8"%1個の壁について開口符号を変更しました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void EditAllEventOperation::slotDeleteWallOpen()
{
    QList<MemberData *> mdlist;
    Q_FOREACH ( MemberData *md, myGrScene->selectedMembers() )
        if ( md->dataType() == DATATYPE::TPWALL || md->dataType() == DATATYPE::TPUWALL ) mdlist.append(md);
    if ( mdlist.isEmpty() ) {
        showInformationMessage(u8"壁または地下外壁が選択されていません。");
        return;
    }

    UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeWallOpenCommand(mdlist, QString()));
    showInformationMessage(QString(u8"%1個の壁について開口符号を削除しました。").arg(mdlist.count()));
    setWaitingMode();
}
} // namespace post3dapp
