#include "eventoperation_line.h"

#include <QtGui>
#include <QDebug>

#include "calc_vector2d.h"
#include "changeangle_dialog.h"
#include "changeface_dialog.h"
#include "changesection_dialog.h"
#include "changeshift_dialog.h"
#include "customdraw_grview.h"
#include "customdraw_hlineitem.h"
#include "define_draw_figure.h"
#include "dividemember_dialog.h"
#include "factory_unifieddata.h"
#include "finditem_dialog.h"
#include "input_grscene.h"
#include "manage_lineactions.h"
#include "manage_memberdata.h"
#include "operation_commands.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "unified_settings.h"

namespace post3dapp{
LineEventOperation::LineEventOperation(CustomDrawGrView *view)
    : AbstractEventOperation(view)
{
    onePointHit = false;
    hideRubberBand(false);
    setMoveWaitingOn(false);
    manageActions = new ManageLineActions(this);

    myElementType = UnifiedEditingData::getInstance()->currentElement();
    currentAddEditMode = UnifiedEditingData::getInstance()->currentAddEditMode();

    oneLineItemHit = false;
    hitLineItem = NULL;
    waitingJoints = JOINTGROUP();

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentStatusChanged, this, &LineEventOperation::reloadSettings);

//    connect(myGrScene,SIGNAL(itemSelectChanged()),this,SLOT(slotChangeActionStatus()));
}

void LineEventOperation::reloadSettings()
{
    ELEMENTTYPE old_element = myElementType;
    ADDEDITMODE old_mode = currentAddEditMode;
    myElementType = UnifiedEditingData::getInstance()->currentElement();
    currentAddEditMode = UnifiedEditingData::getInstance()->currentAddEditMode();
    if ( UnifiedEditingData::getInstance()->elementGroup(myElementType) != ELGLINE ) return;
    if ( old_mode != currentAddEditMode || old_element != myElementType ) initializeSettings();
}

void LineEventOperation::initializeSettings()
{
    onePointHit = false;
    setWaitingMode();
    refreshBatchLines();
    refreshBeamInputStatus();
    myGrScene->setItemSelectable( currentDataType(), currentAddEditMode == EDITMODE );
    if ( currentAddEditMode == SELECTHITMODE ) myGrScene->setInputItemSelectable( currentDataType() );
    manageActions->resetActions( currentAddEditMode == EDITMODE );

    if ( currentAddEditMode == ONEHITMODE || currentAddEditMode == BATCHMODE ) {
//        if( old_element==myElementType && ( old_mode==ONEHITMODE || old_mode==BATCHMODE ) ) return;
        myGrScene->prepareBatchInputLines( currentDataType() );
    }
}

void LineEventOperation::slotChangeActionStatus()
{
    if ( isEditMode() ) {
        manageActions->setActionStatus(currentDataType(), isMoveWaiting(),
                                       myGrScene->selectedItems().count() > 0,
                                       myGrScene->selectedItems().count() > 1,
                                       setMoveWaitingJoints(QPoint(-100, -100)) );
    }
}

void LineEventOperation::setWaitingMode()
{
    myGrView->clearSelectedItems();
    myGrScene->clearCurrentAttach();
    hideRubberBand(false);
    setMoveWaitingOn(false);
    waitingJoints = JOINTGROUP();
//    qDebug() << "line setWaitingMode";
}

void LineEventOperation::toggleViewMode()
{
    setViewModeOn( !viewMode );
    if ( currentAddEditMode == POINTHITMODE ) myGrScene->clearCurrentAttach();
    if ( currentAddEditMode == BATCHMODE ) refreshBatchLines();
    if ( currentAddEditMode == SELECTHITMODE && viewMode ) setWaitingMode();
    if ( currentAddEditMode == EDITMODE && viewMode ) setWaitingMode();
    if ( isBeamInputMode() ) refreshBeamInputStatus();
}

bool LineEventOperation::isBatchInputMode() const
{
    return ( isBatchMode() && isMousePressed() );
}

bool LineEventOperation::isSelectItemMode() const
{
    if ( isSelectHitMode() && isMousePressed() ) return true;
    return ( isEditMode() && !isMoveWaiting() && isMousePressed() );
}

bool LineEventOperation::isPositionSnapOn() const
{
    return ( isPointHitMode() || isMoveWaiting() );
}

bool LineEventOperation::isPointHitMode() const
{
    if ( currentAddEditMode == ONEHITMODE && myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE ) return false;
    return ( !viewMode && ( currentAddEditMode == POINTHITMODE || currentAddEditMode == ONEHITMODE ) );
}

bool LineEventOperation::isBatchMode() const
{
    return ( !viewMode && currentAddEditMode == BATCHMODE );
}

bool LineEventOperation::isSelectHitMode() const
{
    return ( !viewMode && myElementType == ELMEMBERLOAD && currentAddEditMode == SELECTHITMODE );
}

bool LineEventOperation::isBeamInputMode() const
{
    if ( viewMode ) return false;
    return ( currentAddEditMode == DIVIDEMODE || currentAddEditMode == PARALLELMODE
             || currentAddEditMode == VERTICALMODE || currentAddEditMode == EXTENDMODE );
}

bool LineEventOperation::isEditMode() const
{
    return ( !viewMode && currentAddEditMode == EDITMODE );
}

bool LineEventOperation::isMoveWaiting() const
{
    return onMoveWaiting ;
}

bool LineEventOperation::isPlanOneHit() const
{
    return ( ( myElementType == ELCOLUMN || myElementType == ELISO )
             && myGrScene->sceneType() == SCENETYPE::PLANSCENE ) ;
}

void LineEventOperation::setMoveWaitingOn(bool on)
{
    onMoveWaiting = on;
}

void LineEventOperation::execDrawForeground(QPainter *painter, const QRect &vrect)
{
    if ( !isPositionSnapOn() && !isBatchMode() ) return;

    painter->setTransform(QTransform(1, 0, 0, 1, 0, 0), false);
    qreal factor = qPow(myGrView->transform().m11(), 0.25);
    int dd = int(factor * 10.0);

    if ( isBatchMode() ) {

        bool isColumnPlan = isPlanOneHit();
        painter->setRenderHint(QPainter::Antialiasing, isColumnPlan);

        painter->setBrush(QBrush(QColor(255, 200, 200)));
        painter->setPen(QColor(255, 50, 0));

        if ( isColumnPlan ) {
            Q_FOREACH (GLOBALLINE gl, batchLines)
                painter->drawEllipse(myGrView->mapFromScene( gl.p1.getElvQPointF() * 1000.0 ), dd, dd);
        } else {
            Q_FOREACH (GLOBALLINE gl, batchLines) {
                QPointF p1 = myGrView->mapFromScene( gl.p1.getElvQPointF() * 1000.0 );
                QPointF p2 = myGrView->mapFromScene( gl.p2.getElvQPointF() * 1000.0 );
                painter->drawLine(p1, p2);
                painter->drawEllipse(p1, dd, dd);
                painter->drawEllipse(p2, dd, dd);
            }
        }

        if ( currentModifier == Qt::AltModifier ) {
            painter->setBrush(QBrush(QColor(200, 200, 255)));
            painter->setPen(QColor(0, 50, 255));
        }

        if ( isColumnPlan ) {
            Q_FOREACH (GLOBALLINE gl, tmpBatchLines)
                painter->drawEllipse(myGrView->mapFromScene( gl.p1.getElvQPointF() * 1000.0 ), dd, dd);
        } else {
            Q_FOREACH (GLOBALLINE gl, tmpBatchLines) {
                QPointF p1 = myGrView->mapFromScene( gl.p1.getElvQPointF() * 1000.0 );
                QPointF p2 = myGrView->mapFromScene( gl.p2.getElvQPointF() * 1000.0 );
                painter->drawLine(p1, p2);
                painter->drawEllipse(p1, dd, dd);
                painter->drawEllipse(p2, dd, dd);
            }
        }
        return;
    }

    AbstractEventOperation::execDrawForeground(painter, vrect);

    painter->setBrush(QBrush(QColor(255, 50, 0)));
    painter->setRenderHint(QPainter::Antialiasing, true);

    if ( isPointHitMode() &&  onePointHit ) {
        QPoint pp = myGrView->mapFromScene(firstHitGP.getElvQPointF() * 1000.);
        QPoint qq = myGrView->mapFromScene(currentGP.getElvQPointF() * 1000.);
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

    if ( waitingJoints.j1 != NULL ) {
        QPoint p2 = myGrView->mapFromScene( myGrScene->itemPosition(waitingJoints.j1, q1) );
        painter->drawLine(p1, p2);
    }

    if ( waitingJoints.j3 != NULL ) {
        QPoint p2 = myGrView->mapFromScene( myGrScene->itemPosition(waitingJoints.j3, q1) );
        painter->drawLine(p1, p2);
    }
}

void LineEventOperation::execMousePress(QMouseEvent *event)
{
    if ( myGrScene->sceneType() == SCENETYPE::PLANSCENE && myGrScene->floorID().isNull() ) return;
    if ( myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE && myGrScene->frameID().isNull() ) return;

    mousePressOn(true);

    if ( isPointHitMode() &&  event->button() == Qt::LeftButton ) {

        if ( currentAddEditMode == ONEHITMODE ) { // １点ヒット入力　i端伏図上、j端上階

            if ( batchLines.count() < 1 ) return;

            UnifiedEditingData::getInstance()->beginUndoStackMacro("create Member By Mouse OneHit");

            GLOBALLINE gl = batchLines.first();
            QString in_name = UnifiedEditingData::getInstance()->currentSectionName();

            QList<JointData *> jdlist, m_jdlist;
            JointData *ijd = myGrScene->searchNearlyJoint(gl.p1.getElvQPointF() * 1000.);
            if ( ijd == NULL ) {
                CreateJointCommand *cj_com = new CreateJointCommand(gl.p1);
                UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
                ijd = cj_com->createdJoint();
                m_jdlist.append(ijd);
            }
            JointData *jjd = FactoryUnifiedData::getInstance(isOutput)->nearlyJoint(gl.p2);
            if ( jjd == NULL ) {
                CreateJointCommand *cj_com = new CreateJointCommand(gl.p2);
                UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
                jjd = cj_com->createdJoint();
            }
            jdlist.append(ijd);
            jdlist.append(jjd);

            bool isOverlap = FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(jdlist, currentDataType());
            if ( !isOverlap ) {
                CreateMemberCommand *cm_com = new CreateMemberCommand(jdlist, currentDataType(), in_name);
                UnifiedEditingData::getInstance()->pushUndoCommand(cm_com);
            }

            divideMembersByJoints(m_jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());

            UnifiedEditingData::getInstance()->endUndoStackMacro();

            if ( isOverlap ) showInformationMessage(u8"重複部材があるため新規作成しませんでした。");

        } else if ( !onePointHit ) {

            onePointHit = true;
            firstHitGP = currentGP;

        } else if ( (firstHitGP.getElvQPointF() - currentGP.getElvQPointF()).manhattanLength() > 1.0e-3 ) {

            UnifiedEditingData::getInstance()->beginUndoStackMacro("create Member By Mouse Hit");

            QList<JointData *> jdlist, m_jdlist;
            JointData *ijd = myGrScene->searchNearlyJoint(firstHitGP.getElvQPointF() * 1000.);
            JointData *jjd = myGrScene->searchNearlyJoint(currentGP.getElvQPointF() * 1000.);
            if (ijd == NULL) {
                CreateJointCommand *cj_com = new CreateJointCommand(firstHitGP);
                UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
                ijd = cj_com->createdJoint();
                m_jdlist.append(ijd);
            }
            if (jjd == NULL) {
                CreateJointCommand *cj_com = new CreateJointCommand(currentGP);
                UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
                jjd = cj_com->createdJoint();
                m_jdlist.append(jjd);
            }

            ADDMOVEDACTION amd_act = UnifiedEditingData::getInstance()->currentAddMovedAction();
            if (amd_act == AMDINTER) {
                jdlist = myGrScene->searchBetweenPoints(firstHitGP.getElvQPointF() * 1000.,
                                                        currentGP.getElvQPointF() * 1000.);
            }
            jdlist.prepend(ijd);
            jdlist.append(jjd);

            bool isOverlap = FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(jdlist, currentDataType());
            if ( !isOverlap ) {
                QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
                UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMemberCommand(jdlist, currentDataType(), in_name));
            }

            divideMembersByJoints(m_jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());
            onePointHit = false;
            UnifiedEditingData::getInstance()->endUndoStackMacro();

            if ( isOverlap ) showInformationMessage(u8"重複部材（荷重）があるため新規作成しませんでした。");
        }

    }

    if ( isPointHitMode() &&  event->button() == Qt::RightButton ) {
        if (onePointHit) onePointHit = false;
    }

    if ( isBatchMode() &&  event->button() == Qt::LeftButton ) {
        // 既に選択された状態で左クリック → ラバーバンド入力モード、ラバーバンドの表示は OFF に
        bool inputOn = ( !batchLines.isEmpty() && event->modifiers() == Qt::NoModifier );
        if ( inputOn ) {
            batchLinesToMemberItems();
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

    if ( isBeamInputMode() &&  event->button() == Qt::LeftButton ) {
        if (oneLineItemHit) {
            mousePressOn(false);
            if ( hitLineItem != NULL ) hitLineItem->setInputState(false);
            if ( currentAddEditMode == DIVIDEMODE ) {
                appendEquallyBeam(event->pos());
            } else if ( currentAddEditMode == PARALLELMODE ) {
                appendParallelBeam(event->pos());
            } else if ( currentAddEditMode == VERTICALMODE ) {
                appendVerticalBeam(event->pos());
            } else if ( currentAddEditMode == EXTENDMODE ) {
                appendExtendBeam(event->pos());
            }
            hitLineItem = NULL;
            oneLineItemHit = false;
        } else {
            hitLineItem = myGrScene->searchNearlyHItem(myGrView->mapToScene(event->pos()), 1000.0);
            if ( hitLineItem != NULL ) {
                hitLineItem->setInputState(true);
                oneLineItemHit = true;
            }
        }
    }

    if ( isBeamInputMode() &&  event->button() == Qt::RightButton ) {
        if ( hitLineItem != NULL ) hitLineItem->setInputState(false);
        oneLineItemHit = false;
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
        manageActions->setActionStatus(currentDataType(), isMoveWaiting(),
                                       myGrScene->selectedItems().count() > 0,
                                       myGrScene->selectedItems().count() > 1,
                                       setMoveWaitingJoints(event->pos()) );
        manageActions->showContextMenu();
    }
}

void LineEventOperation::execMouseRelease(QMouseEvent *event)
{
    mousePressOn(false);

    if ( isBatchMode() ) {
        if ( event->button() == Qt::LeftButton ) {
            combineBatchLines();
            myGrScene->update();
        } else {
            refreshBatchLines();
        }
    }
}

void LineEventOperation::execKeyPress(QKeyEvent *event)
{
//    qDebug() << event->key();
    if ( isMoveWaiting() ) setWaitingMode();

    if ( isBatchMode() ) {
        if ( event->key() == Qt::Key_Escape ) {
            refreshBatchLines();
            mousePressOn(false);
        } else if ( event->key() == Qt::Key_Return ) {
            if ( !batchLines.isEmpty() ) batchLinesToMemberItems();
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

void LineEventOperation::positionSnap(const QPoint &cur_p, bool)
{
    if ( currentAddEditMode == ONEHITMODE ) {
        QPointF pp = myGrView->mapToScene( cur_p );
        QList<GLOBALLINE> lines = myGrScene->getBatchInputLines();
        GLOBALLINE nearly_line;
        qreal min_len = 1.0e+10;
        Q_FOREACH (GLOBALLINE gline, lines) {
            qreal len = (pp - gline.p1.getElvQPointF() * 1000.0).manhattanLength();
            if (len < min_len) {
                min_len = len;
                nearly_line = gline;
            }
        }
        currentGP = nearly_line.p1;
        batchLines = ( QList<GLOBALLINE>() << nearly_line ) ;
    } else {
        AbstractEventOperation::positionSnap(cur_p, true);
    }
}

void LineEventOperation::refreshBatchLines()
{
    tmpBatchLines.clear();
    batchLines.clear();
    myGrScene->update();
}

void LineEventOperation::refreshBeamInputStatus()
{
    oneLineItemHit = false;
    if ( hitLineItem != NULL ) hitLineItem->setInputState(false);
    hitLineItem = NULL;
}

bool LineEventOperation::existNearlyPoint(QPoint )
{
    return false;
}

void LineEventOperation::changeBatchRect(QPoint p1, QPoint p2)
{
    QPointF pp1 = myGrView->mapToScene ( p1 );
    QPointF pp2 = myGrView->mapToScene ( p2 );

    QRectF rect;
    ( pp1.x() > pp2.x() ) ? rect.setX(pp2.x()) : rect.setX(pp1.x()) ;
    ( pp1.y() > pp2.y() ) ? rect.setY(pp2.y()) : rect.setY(pp1.y()) ;

    rect.setWidth(qAbs(pp1.x() - pp2.x()));
    rect.setHeight(qAbs(pp1.y() - pp2.y()));

    selectBatchLines(rect);
}

void LineEventOperation::selectBatchLines(const QRectF &rangeRect)
{
    tmpBatchLines.clear();
    if ( isPlanOneHit() ) {
        Q_FOREACH ( GLOBALLINE gl, myGrScene->getBatchInputLines() ) {
            QPointF pp = gl.p1.getElvQPointF() * 1000.0;
            if ( rangeRect.contains( pp ) ) tmpBatchLines.append(gl);
        }
    } else {

        Q_FOREACH ( GLOBALLINE gl, myGrScene->getBatchInputLines() ) {
            QPointF pp1 = gl.p1.getElvQPointF() * 1000.0;
            QPointF pp2 = gl.p2.getElvQPointF() * 1000.0;
            if ( rangeRect.contains( pp1 ) &&  rangeRect.contains( pp2 ) ) tmpBatchLines.append(gl);
        }
    }
}

void LineEventOperation::combineBatchLines()
{
    bool isColumnPlan = isPlanOneHit();

    if ( currentModifier == Qt::ControlModifier ) {
        QList<GLOBALLINE> addLines;
        Q_FOREACH (GLOBALLINE pp, tmpBatchLines) {
            bool ok = true;
            Q_FOREACH (GLOBALLINE qq, batchLines) {
                if ( pp.p1.isNearlyPoint(qq.p1) ) {
                    if ( isColumnPlan ) {
                        ok = false;
                        break;
                    } else if ( pp.p2.isNearlyPoint(qq.p2) ) {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok) addLines.append(pp);
        }
        batchLines.append(addLines);
    } else if ( currentModifier == Qt::AltModifier ) {
        Q_FOREACH (GLOBALLINE pp, tmpBatchLines) {
            for (int i = batchLines.count() - 1; i >= 0 ; i--) {
                GLOBALLINE qq = batchLines.at(i);
                if ( pp.p1.isNearlyPoint(qq.p1) ) {
                    if ( isColumnPlan ) {
                        batchLines.removeAt(i);
                    } else if ( pp.p2.isNearlyPoint(qq.p2) ) {
                        batchLines.removeAt(i);
                    }
                }
            }
        }
    } else {
        batchLines = tmpBatchLines;
    }
    tmpBatchLines.clear();
}

void LineEventOperation::batchLinesToMemberItems()
{
    if ( warningMemberBatchInput() ) return;

    bool isColumnPlan = isPlanOneHit();

    QList< QList<JointData *> > jd_alllist;
    QList<JointData *> new_jdlist;

    UnifiedEditingData::getInstance()->beginUndoStackMacro("batch Lines To Member");

    Q_FOREACH ( GLOBALLINE gl, batchLines ) {

        JointData *ijd = myGrScene->searchNearlyJoint(gl.p1.getElvQPointF() * 1000.);
        if (ijd == NULL) {
            Q_FOREACH ( JointData *jd, new_jdlist ) {
                qreal mlen = qAbs(jd->xPos() - gl.p1.x()) + qAbs(jd->yPos() - gl.p1.y()) + qAbs(
                                 jd->zPos() - gl.p1.z());
                if ( mlen < 0.001 ) {
                    ijd = jd;
                    break;
                }
            }
            if (ijd == NULL) {
                CreateJointCommand *cj_com = new CreateJointCommand(gl.p1, false);
                UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
                ijd = cj_com->createdJoint();
                new_jdlist.append(ijd);
            }
        }
        JointData *jjd;
        if (isColumnPlan) {
            jjd = FactoryUnifiedData::getInstance(isOutput)->nearlyJoint(gl.p2);
            if (jjd == NULL) {
                Q_FOREACH ( JointData *jd, new_jdlist ) {
                    qreal mlen = qAbs(jd->xPos() - gl.p2.x()) + qAbs(jd->yPos() - gl.p2.y()) + qAbs(
                                     jd->zPos() - gl.p2.z());
                    if ( mlen < 0.001 ) {
                        jjd = jd;
                        break;
                    }
                }
                if (jjd == NULL) {
                    CreateJointCommand *cj_com = new CreateJointCommand(gl.p2, false);
                    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
                    jjd = cj_com->createdJoint();
                    new_jdlist.append(jjd);
                }
            }
        } else {
            jjd = myGrScene->searchNearlyJoint(gl.p2.getElvQPointF() * 1000.);
            if (jjd == NULL) {
                Q_FOREACH ( JointData *jd, new_jdlist ) {
                    qreal mlen = qAbs(jd->xPos() - gl.p2.x()) + qAbs(jd->yPos() - gl.p2.y()) + qAbs(
                                     jd->zPos() - gl.p2.z());
                    if ( mlen < 0.001 ) {
                        jjd = jd;
                        break;
                    }
                }
                if (jjd == NULL) {
                    CreateJointCommand *cj_com = new CreateJointCommand(gl.p2, false);
                    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
                    jjd = cj_com->createdJoint();
                    new_jdlist.append(jjd);
                }
            }
        }

        jd_alllist.append( QList<JointData *>() << ijd << jjd );
    }

    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalJointAppendedCommand(new_jdlist));

    int overlap_cnt = 0;
    QList< QList<JointData *> > all_joints;
    for ( int i = 0; i < jd_alllist.count(); i++ ) {

        JointData *ijd = jd_alllist.at(i).first();
        JointData *jjd = jd_alllist.at(i).last();

        if ( FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(
                    QList<JointData *>() << ijd << jjd, currentDataType()) ) {
            overlap_cnt++;
            continue;
        }

        QList<JointData *> jdlist;
        if ( !isColumnPlan && UnifiedEditingData::getInstance()->currentAddMovedAction() == AMDINTER ) {
            QPointF p1 = batchLines.at(i).p1.getElvQPointF() * 1000.;
            QPointF p2 = batchLines.at(i).p2.getElvQPointF() * 1000.;
            jdlist = myGrScene->searchBetweenPoints(p1, p2);
        }
        jdlist.prepend(ijd);
        jdlist.append(jjd);

        all_joints.append( jdlist );
    }

    QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
    UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMembersCommand(all_joints, currentDataType(), in_name));

    divideMembersByJoints(new_jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());

    refreshBatchLines();

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    if ( overlap_cnt > 0 ) {
        showInformationMessage(QString(u8"%1箇所について重複部材があるため新規作成しませんでした。").arg(overlap_cnt));
    }
}

void LineEventOperation::selectItemToLoadItem()
{
    if ( warningLoadBatchInput() ) return;
    QList<MemberData *> members = myGrScene->selectedMembers();
    if ( members.isEmpty() ) return;
    QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
    QList< QList<JointData *> > jd_alllist;
    int overlap_cnt = 0;
    Q_FOREACH ( MemberData *md, members ) {
        if ( !md->isLineMember(false) ) continue;
        if ( FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(md->getJointList(), DATATYPE::TPMEMBERLOAD) ) {
            overlap_cnt++;
            continue;
        }
        jd_alllist.append( md->getJointList() );
    }
    UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMembersCommand(jd_alllist, DATATYPE::TPMEMBERLOAD, in_name));

    myGrView->clearSelectedItems();

    if ( overlap_cnt > 0 ) {
        showInformationMessage(QString(u8"%1箇所について重複荷重があるため新規作成しませんでした。").arg(overlap_cnt));
    }
}

bool LineEventOperation::setMoveWaitingJoints(QPoint mousePoint)
{
    if ( mousePoint.x() < 0 || mousePoint.y() < 0 ) {
        waitingJoints = JOINTGROUP();
        return false;
    }
    waitingJoints = myGrScene->selectedMemberJoints(myGrView->mapToScene(mousePoint));
    return ( waitingJoints.j2 != NULL );
}

void LineEventOperation::slotSetMoveWaitingMode()
{
    setMoveWaitingOn(true);
    manageActions->resetActions();
    myGrScene->update();
}

void LineEventOperation::moveMemberJoints()
{
    MemberData *md = myGrScene->selectedMember();
    QPointF curPos = currentGP.getElvQPointF() * 1000.;
    JointData *jd = myGrScene->searchNearlyJoint(curPos);

    JointData *wj1 = waitingJoints.j1;
    JointData *wj2 = waitingJoints.j2;
    JointData *wj3 = waitingJoints.j3;

    if ( wj2 == NULL ) return;

    if ( md == NULL || jd == NULL ) {
        showWarningMessage(u8"移動先に節点が存在しません。");
        return;
    }

    if ( md->getJointList().contains(jd) ) {
        showWarningMessage(u8"部材内の点には移動できません。");
        return;
    }

    UnifiedEditingData::getInstance()->beginUndoStackMacro("move Member Joints");

    ReplaceMemberJointCommand *rj_com = new ReplaceMemberJointCommand(md, wj2, jd);
    UnifiedEditingData::getInstance()->pushUndoCommand(rj_com);

    if ( UnifiedEditingData::getInstance()->currentAddMovedAction() == AMDINTER ) {
        QList<JointData *> jdlist;
        if ( wj1 != NULL ) {
            QPointF p1 = myGrScene->itemPosition(wj1, curPos);
            jdlist.append(myGrScene->searchBetweenPoints(p1, curPos));
        }
        if ( wj3 != NULL ) {
            QPointF p2 = myGrScene->itemPosition(wj3, curPos);
            jdlist.append(myGrScene->searchBetweenPoints(curPos, p2));
        }

        if ( !jdlist.isEmpty() ) {
            UnifiedEditingData::getInstance()->pushUndoCommand(new InteriorMembersCommand(QList<MemberData *>{md}, QList<QList<JointData *>>{jdlist}));
        }
    }

    if ( UnifiedEditingData::getInstance()->currentDeleteAction() == DELETEJOINT ) {
        myGrScene->blockSignals(true);
        if ( wj2->isIndependentJoint() ) {
            UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand(QList<JointData *>{wj2}));
        }
        myGrScene->blockSignals(false);
    }
    myGrScene->sendSelectedItems();
    setWaitingMode();

    UnifiedEditingData::getInstance()->endUndoStackMacro();
}

void LineEventOperation::appendEquallyBeam( const QPoint &epos )
{
    CustomDrawHLineItem *secondItem = myGrScene->searchNearlyHItem(myGrView->mapToScene(epos), 1000.0);

    if ( hitLineItem == NULL || secondItem == NULL ) return;
    if ( hitLineItem == secondItem ) return;

    UnifiedEditingData::getInstance()->beginUndoStackMacro("append Equally Beam");

    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalElementCountChangedCommand( false ));

    int idiv = UnifiedEditingData::getInstance()->currentBeamDivValue();

    QList<QPointF> p1_list = myGrScene->itemToDividedPointList(hitLineItem, ITEMDIVRULE::IDVEQUAL, qreal(idiv), 0.0);
    QList<JointData *> d1_jdlist;
    QList<JointData *> new_jdlist;
    Q_FOREACH ( QPointF pp, p1_list ) {
        JointData *jd = myGrScene->searchNearlyJoint(pp);
        if (jd == NULL) {
            GlobalPoint gp = scenePointsToGlobalPoints( QList<QPointF>() << pp ).first();
            CreateJointCommand *cj_com = new CreateJointCommand(gp, false);
            UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
            jd = cj_com->createdJoint();
            new_jdlist.append(jd);
        }
        d1_jdlist.append(jd);
    }
    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalJointAppendedCommand(new_jdlist));

    new_jdlist.clear();
    divideMembersByJoints(d1_jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());

    QList<QPointF> p2_list = myGrScene->itemToDividedPointList(secondItem, ITEMDIVRULE::IDVEQUAL, qreal(idiv), 0.0);
    QList<JointData *> d2_jdlist;
    Q_FOREACH ( QPointF pp, p2_list ) {
        JointData *jd = myGrScene->searchNearlyJoint(pp);
        if (jd == NULL) {
            GlobalPoint gp = scenePointsToGlobalPoints( QList<QPointF>() << pp ).first();
            CreateJointCommand *cj_com = new CreateJointCommand(gp, false);
            UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
            jd = cj_com->createdJoint();
            new_jdlist.append(jd);
        }
        d2_jdlist.append(jd);
    }
    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalJointAppendedCommand(new_jdlist));

    divideMembersByJoints(d2_jdlist, UnifiedEditingData::getInstance()->currentAddMoveAction());


    bool reverse = false;
    if ( idiv > 2 ) {
        reverse = ( CalcVector2D::checkSegmentIntersection(QLineF(p1_list.at(0), p2_list.at(0)),
                                                           QLineF(p1_list.at(1), p2_list.at(1))) );
    }

    QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
    int overlap_cnt = 0;
    QList<MemberData *> new_mdlist;
    for ( int i = 0; i < d1_jdlist.count(); i++ ) {
        QList<JointData *> jdlist;
        JointData *j1 = d1_jdlist.at(i);
        JointData *j2 = ( reverse ) ? d2_jdlist.at(d2_jdlist.count() - 1 - i) : d2_jdlist.at(i) ;
        if ( UnifiedEditingData::getInstance()->currentAddMovedAction() == AMDINTER ) {
            QPointF p1 = p1_list.at(i);
            QPointF p2 = ( reverse ) ? p2_list.at(d2_jdlist.count() - 1 - i) : p2_list.at(i) ;
            jdlist = myGrScene->searchBetweenPoints(p1, p2);
        }
        jdlist.prepend(j1);
        jdlist.append(j2);
        if ( FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(jdlist, DATATYPE::TPBEAM) ) {
            overlap_cnt++;
            continue;
        }

        CreateMemberCommand *cm_com = new CreateMemberCommand(jdlist, DATATYPE::TPBEAM, in_name, false);
        UnifiedEditingData::getInstance()->pushUndoCommand(cm_com);
        new_mdlist.append( cm_com->createdMember() );
    }

    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalMemberAppendedCommand(new_mdlist));
    UnifiedEditingData::getInstance()->pushUndoCommand(new SignalElementCountChangedCommand( true ));

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    if ( overlap_cnt > 0 ) {
        showInformationMessage(QString(u8"%1箇所について重複部材があるため新規作成しませんでした。").arg(overlap_cnt));
    }
}

void LineEventOperation::appendParallelBeam(const QPoint &epos)
{
    if ( hitLineItem == NULL ) return;

    QPointF ep = myGrView->mapToScene(epos);
    qreal div = UnifiedEditingData::getInstance()->currentBeamPalValue();

    QLineF l1 = hitLineItem->getIJLine();
    int side = CalcVector2D::sideToSegment(l1.p1(), l1.p2(), ep);
    qreal theta = qAtan2(l1.y2() - l1.y1(), l1.x2() - l1.x1());

    QPointF p1, p2;
    if ( side > 0 ) {
        p1 = QPointF( l1.x1() + div * qSin(theta), l1.y1() - div * qCos(theta) );
        p2 = QPointF( l1.x2() + div * qSin(theta), l1.y2() - div * qCos(theta) );
    } else {
        p1 = QPointF( l1.x1() - div * qSin(theta), l1.y1() + div * qCos(theta) );
        p2 = QPointF( l1.x2() - div * qSin(theta), l1.y2() + div * qCos(theta) );
    }

    QPointF np = CalcVector2D::perpPointToLine(p1, p2, ep);

    QList<QPointF> clist = myGrScene->searchCrossPointsToLine(QLineF(p1, p2));
    qreal r_min = 999999.9, l_min = 999999.9;
    QPointF r_p, l_p;

    Q_FOREACH ( QPointF pp, clist ) {
        qreal tt = qAtan2(pp.y() - np.y(), pp.x() - np.x());
        qreal len = CalcVector2D::length(pp - np);

        if ( qAbs(theta) < 1.0e-5 || qAbs(theta) > M_PI - 1.0e-5 ) {
            if ( qAbs( qAbs(tt) - qAbs(theta) ) < 1.0e-5 ) {
                if ( len < r_min ) {
                    r_min = len;
                    r_p = pp;
                }
            } else {
                if ( len < l_min ) {
                    l_min = len;
                    l_p = pp;
                }
            }
        } else {
            if ( theta * tt > 0.0 ) {
                if ( len < r_min ) {
                    r_min = len;
                    r_p = pp;
                }
            } else {
                if ( len < l_min ) {
                    l_min = len;
                    l_p = pp;
                }
            }
        }
    }
    if ( r_min > 999999.0 || l_min > 999999.0 ) {
        showWarningMessage(u8"追加小梁の端点が定まりません。");
        return;
    }

    UnifiedEditingData::getInstance()->beginUndoStackMacro("append Parallel Beam");

    JointData *jd1 = myGrScene->searchNearlyJoint(l_p);
    if (jd1 == NULL) {
        GlobalPoint gp = scenePointsToGlobalPoints( QList<QPointF>() << l_p ).first();
        CreateJointCommand *cj_com = new CreateJointCommand(gp);
        UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
        jd1 = cj_com->createdJoint();
    }

    JointData *jd2 = myGrScene->searchNearlyJoint(r_p);
    if (jd2 == NULL) {
        GlobalPoint gp = scenePointsToGlobalPoints( QList<QPointF>() << r_p ).first();
        CreateJointCommand *cj_com = new CreateJointCommand(gp);
        UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
        jd2 = cj_com->createdJoint();
    }

    divideMembersByJoints( QList<JointData *>{jd1, jd2}, UnifiedEditingData::getInstance()->currentAddMoveAction());

    QList<JointData *> jdlist;
    if (UnifiedEditingData::getInstance()->currentAddMovedAction() == AMDINTER) {
        jdlist = myGrScene->searchBetweenPoints(l_p, r_p);
    }
    jdlist.prepend(jd1);
    jdlist.append(jd2);

    bool isOverlap = FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(jdlist, DATATYPE::TPBEAM);

    if ( !isOverlap ) {
        QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
        UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMemberCommand(jdlist, DATATYPE::TPBEAM, in_name));
    }

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    if ( isOverlap ) showInformationMessage(u8"重複部材があるため新規作成しませんでした。");
}

void LineEventOperation::appendVerticalBeam(const QPoint &epos)
{
    if ( hitLineItem == NULL ) return;

    QPointF ep = myGrView->mapToScene(epos);

    qreal div = UnifiedEditingData::getInstance()->currentBeamVerValue();
    QList<QPointF> nplist = myGrScene->itemToDividedPointList(hitLineItem, ITEMDIVRULE::IDVILEN, div, 0.0);
    if ( nplist.count() < 1 ) {
        showWarningMessage(u8"追加小梁の端点が定まりません。");
        return;
    }
    QPointF np = nplist.first();

    QLineF l1 = hitLineItem->getIJLine();
    int side = CalcVector2D::sideToSegment(l1.p1(), l1.p2(), ep);
    qreal theta = qAtan2(l1.y2() - l1.y1(), l1.x2() - l1.x1());

    QPointF p1;
    if ( side > 0 ) {
        p1 = QPointF( np.x() + 1000.0 * qSin(theta), np.y() - 1000.0 * qCos(theta) );
    } else {
        p1 = QPointF( np.x() - 1000.0 * qSin(theta), np.y() + 1000.0 * qCos(theta) );
    }
    theta = qAtan2(p1.y() - np.y(), p1.x() - np.x());

    QList<QPointF> clist = myGrScene->searchCrossPointsToLine(QLineF(np, p1));
    qreal r_min = 999999.9;
    QPointF r_p;

    Q_FOREACH ( QPointF pp, clist ) {
        qreal tt = qAtan2(pp.y() - np.y(), pp.x() - np.x());
        qreal len = CalcVector2D::length(pp - np);
        if ( len < 1.0e-5 ) continue;

        if ( qAbs(theta) < 1.0e-5 || qAbs(theta) > M_PI - 1.0e-5 ) {
            if ( qAbs( qAbs(tt) - qAbs(theta) ) < 1.0e-5 ) {
                if ( len < r_min ) {
                    r_min = len;
                    r_p = pp;
                }
            }
        } else {
            if ( theta * tt > 0.0 ) {
                if ( len < r_min ) {
                    r_min = len;
                    r_p = pp;
                }
            }
        }
    }
    if ( r_min > 999999.0 ) {
        showWarningMessage(u8"追加小梁の端点が定まりません。");
        return;
    }

    UnifiedEditingData::getInstance()->beginUndoStackMacro("append Vertical Beam");

    JointData *jd1 = myGrScene->searchNearlyJoint(np);
    if (jd1 == NULL) {
        GlobalPoint gp = scenePointsToGlobalPoints( QList<QPointF>() << np ).first();
        CreateJointCommand *cj_com = new CreateJointCommand(gp);
        UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
        jd1 = cj_com->createdJoint();
    }

    JointData *jd2 = myGrScene->searchNearlyJoint(r_p);
    if (jd2 == NULL) {
        GlobalPoint gp = scenePointsToGlobalPoints( QList<QPointF>() << r_p ).first();
        CreateJointCommand *cj_com = new CreateJointCommand(gp);
        UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
        jd2 = cj_com->createdJoint();
    }

    divideMembersByJoints( QList<JointData *>{jd1, jd2}, UnifiedEditingData::getInstance()->currentAddMoveAction());

    QList<JointData *> jdlist;
    if (UnifiedEditingData::getInstance()->currentAddMovedAction() == AMDINTER) {
        jdlist = myGrScene->searchBetweenPoints(np, r_p);
    }
    jdlist.prepend(jd1);
    jdlist.append(jd2);

    bool isOverlap = FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(jdlist, DATATYPE::TPBEAM);

    if ( !isOverlap ) {
        QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
        UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMemberCommand(jdlist, DATATYPE::TPBEAM, in_name));
    }

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    if ( isOverlap ) showInformationMessage(u8"重複部材があるため新規作成しませんでした。");
}

void LineEventOperation::appendExtendBeam(const QPoint &epos)
{
    if ( hitLineItem == NULL ) return;

    QPointF ep = myGrView->mapToScene(epos);
    qreal div = UnifiedEditingData::getInstance()->currentBeamExtValue();

    QList<QLineF> lines = hitLineItem->getItemLines();
    QPointF iPos = lines.first().p1();
    QPointF jPos = lines.last().p2();
    bool dirI = ( CalcVector2D::length(ep - iPos) < CalcVector2D::length(ep - jPos) );

    QPointF pp;
    JointData *jd1;
    if ( dirI ) {
        pp = CalcVector2D::extendPoint( lines.first().p2(), lines.first().p1(), div );
        jd1 = myGrScene->searchNearlyJoint(iPos);
    } else {
        pp = CalcVector2D::extendPoint( lines.last().p1(), lines.last().p2(), div );
        jd1 = myGrScene->searchNearlyJoint(jPos);
    }
    if (jd1 == NULL) return;

    UnifiedEditingData::getInstance()->beginUndoStackMacro("append Extend Beam");

    JointData *jd2 = myGrScene->searchNearlyJoint(pp);
    if (jd2 == NULL) {
        GlobalPoint gp = scenePointsToGlobalPoints( QList<QPointF>() << pp ).first();
        CreateJointCommand *cj_com = new CreateJointCommand(gp);
        UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
        jd2 = cj_com->createdJoint();
    }
    divideMembersByJoints( QList<JointData *>() << jd2,
                           UnifiedEditingData::getInstance()->currentAddMoveAction());

    QList<JointData *> jdlist;
    if (UnifiedEditingData::getInstance()->currentAddMovedAction() == AMDINTER) {
        QPointF p1 = ( dirI ) ? iPos : jPos;
        jdlist = myGrScene->searchBetweenPoints(p1, pp);
    }
    jdlist.prepend(jd1);
    jdlist.append(jd2);

    bool isOverlap = FactoryUnifiedData::getInstance(isOutput)->existOverlapMember(jdlist, DATATYPE::TPBEAM);

    if ( !isOverlap ) {
        QString in_name = UnifiedEditingData::getInstance()->currentSectionName();
        UnifiedEditingData::getInstance()->pushUndoCommand(new CreateMemberCommand(jdlist, DATATYPE::TPBEAM, in_name));
    }

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    if ( isOverlap ) showInformationMessage(u8"重複部材があるため新規作成しませんでした。");
}

void LineEventOperation::slotFindItems()
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

void LineEventOperation::slotDeleteItems()
{
    QList<MemberData *> delMembers = myGrScene->selectedMembers();

    int m_count = delMembers.count();
    if ( m_count == 0 ) {
        showInformationMessage(u8"部材（または荷重）が選択されていません。");
        return;
    }


    myGrScene->blockSignals(true);
    UnifiedEditingData::getInstance()->beginUndoStackMacro("delete Line Members");

    if ( UnifiedEditingData::getInstance()->currentDeleteAction() == REMAINJOINT ) {
        RemoveMembersCommand *rm_com = new RemoveMembersCommand(delMembers);
        UnifiedEditingData::getInstance()->pushUndoCommand(rm_com);
    } else {
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
            UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand(delJoints));
        }
    }

    UnifiedEditingData::getInstance()->endUndoStackMacro();

    myGrScene->blockSignals(false);
    myGrScene->sendSelectedItems();

    if ( m_count < 1 ) return;

    QString msg = QString(u8"%1個の").arg(m_count);
    msg += FactoryUnifiedData::getInstance(isOutput)->dataTypeToJapanese( currentDataType() );
    msg += u8"を削除しました。";
    showInformationMessage(msg);
    setWaitingMode();
}

void LineEventOperation::slotChangeSection()
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
        showInformationMessage(QString(u8"%1個の部材（あるいは荷重）について符号を変更しました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void LineEventOperation::slotReverseMemberJoint()
{
    QList<MemberData *> mdlist = myGrScene->selectedMembers();
    if ( mdlist.isEmpty() ) {
        showInformationMessage(u8"対象の部材（または荷重）が選択されていません");
        return;
    }

    UnifiedEditingData::getInstance()->pushUndoCommand(new ReverseMemberJointCommand(mdlist));
    showInformationMessage(QString(u8"%1個の部材（あるいは荷重）についてI,J端を入れ替えました。").arg(mdlist.count()));
    setWaitingMode();
}

void LineEventOperation::slotUniteMembers()
{
    QList<MemberData *> mdlist = myGrScene->selectedMembers();
    if ( mdlist.count() < 2 ) return;

    QList<MemberData *> sorted_mdlist;

    JointData *ijd = mdlist.first()->iJoint();
    JointData *jjd = mdlist.first()->jJoint();
    sorted_mdlist.append( mdlist.first() );
    mdlist.removeFirst();

    bool isNormalEnd = true;
    int icc = 0, jcc = 0;
    MemberData *tmp_md;
    while ( isNormalEnd ) {
        icc = 0;
        Q_FOREACH ( MemberData *md, mdlist ) {
            if ( md->jJoint() == ijd ) {
                icc++;
                tmp_md = md;
            }
        }
        if ( icc > 1 ) {
            isNormalEnd = false;
            break;
        }
        if ( icc == 1 ) {
            ijd = tmp_md->iJoint();
            sorted_mdlist.prepend(tmp_md);
            mdlist.removeOne(tmp_md);
        }

        jcc = 0;
        Q_FOREACH ( MemberData *md, mdlist ) {
            if ( md->iJoint() == jjd ) {
                jcc++;
                tmp_md = md;
            }
        }
        if ( jcc > 1 ) {
            isNormalEnd = false;
            break;
        }
        if ( jcc == 1 ) {
            jjd = tmp_md->jJoint();
            sorted_mdlist.append(tmp_md);
            mdlist.removeOne(tmp_md);
        }

        if ( mdlist.isEmpty() ) break;
        if ( icc == 0 && jcc == 0 ) break;
    }

    if ( !isNormalEnd || mdlist.count() > 0 ) {
        showWarningMessage(u8"連続した部材（あるいは荷重）ではありません");
        return;
    }

    myGrScene->blockSignals(true);
    UnifiedEditingData::getInstance()->pushUndoCommand(new UniteMembersCommand(sorted_mdlist));

    myGrScene->blockSignals(false);
    myGrScene->sendSelectedItems();
    setWaitingMode();
}

void LineEventOperation::slotChangeAngle()
{
    if ( currentDataType() == DATATYPE::TPMEMBERLOAD
            || currentDataType() == DATATYPE::TPDAMPER || currentDataType() == DATATYPE::TPISO ) return;

    QList<MemberData *> mdlist = myGrScene->selectedMembers();
    if ( mdlist.isEmpty() ) {
        showInformationMessage(u8"部材が選択されていません。");
        return;
    }

    bool fixed = true;
    qreal ag = mdlist.first()->codeAngle();
    Q_FOREACH ( MemberData *md, mdlist ) {
        if ( qAbs( ag - md->codeAngle() ) > 1.0e-3 ) {
            fixed = false;
            break;
        }
    }

    QString str_angle = ( fixed ) ? QString("%1").arg(ag * 180.0 / M_PI, 0, 'f', 3) : QString("***") ;
    ChangeAngleDialog dialog(str_angle, myGrView);

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
        showInformationMessage(QString(u8"%1個の部材についてコードアングルが変更されました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void LineEventOperation::slotChangeShift()
{
    if ( currentDataType() == DATATYPE::TPMEMBERLOAD
            || currentDataType() == DATATYPE::TPDAMPER || currentDataType() == DATATYPE::TPISO ) return;

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
        showInformationMessage(QString(u8"%1個の部材について寄り長さが変更されました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void LineEventOperation::slotChangeFace()
{
    if ( currentDataType() == DATATYPE::TPMEMBERLOAD ) return;

    QList<MemberData *> mdlist = myGrScene->selectedMembers();
    if ( mdlist.count() < 1 ) {
        showInformationMessage(u8"部材が選択されていません。");
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
        showInformationMessage(QString(u8"%1個の部材についてフェイス長さが変更されました。").arg(mdlist.count()));
        setWaitingMode();
    }
}

void LineEventOperation::slotDivideMember()
{
    QList<MemberData *> mdlist = myGrScene->selectedMembers();
    if ( mdlist.count() < 1 ) {
        showInformationMessage(u8"部材が選択されていません。");
        return;
    }

    DivideMemberDialog dialog(myGrView);

    if ( dialog.exec() == QDialog::Accepted ) {

        int id_t = dialog.typeGroupID();
        int id_d = dialog.divideGroupID();

        ADDMOVEACTION am_act;
        switch ( id_t ) {
        case 0:
            am_act = AMINTER;
            break;
        case 1:
            am_act = AMCOMPOSE;
            break;
        default:
            am_act = AMINTER;
            break;
        }

        qreal v1 = 0.0, v2 = 0.0;
        ITEMDIVRULE rule;

        switch (id_d) {
        case 0:
            v1 = qreal( dialog.equalSpinValue() );
            rule = ITEMDIVRULE::IDVEQUAL;
            break;
        case 1:
            v1 = qreal( dialog.ratio1SpinValue() );
            v2 = qreal( dialog.ratio2SpinValue() );
            rule = ITEMDIVRULE::IDVRATIO;
            break;
        case 2:
            v1 = qreal( dialog.iLenSpinValue() );
            rule = ITEMDIVRULE::IDVILEN;
            break;
        case 3:
            v1 = qreal( dialog.jLenSpinValue() );
            rule = ITEMDIVRULE::IDVJLEN;
            break;
        default:
            rule = ITEMDIVRULE::IDVEQUAL;
            break;
        }

        if ( id_t == 2 ) {
            UnifiedEditingData::getInstance()->pushUndoCommand(new DivideMembersByInteriorJoinsCommand(mdlist));
            showInformationMessage(QString(u8"%1個の部材が分割されました。").arg(mdlist.count()));
            setWaitingMode();
            return;
        }

        UnifiedEditingData::getInstance()->beginUndoStackMacro("divide Selected Members");

        QList<QGraphicsItem *> items = myGrScene->selectedItems();
        QList<JointData *> d_jdlist;
        QList<JointData *> new_jdlist;

        Q_FOREACH ( QGraphicsItem *item, items ) {

            CustomDrawItem *litem = qgraphicsitem_cast<CustomDrawItem *>(item);
            if ( litem->itemType() == ITEMTYPE::VLINEITEM ) continue;

            QList<QPointF> plist = myGrScene->itemToDividedPointList(litem, rule, v1, v2);

            Q_FOREACH ( QPointF pp, plist ) {
                JointData *jd = myGrScene->searchNearlyJoint(pp);
                if (jd == NULL) {
                    GlobalPoint gp = scenePointsToGlobalPoints( QList<QPointF>() << pp ).first();
                    CreateJointCommand *cj_com = new CreateJointCommand(gp, false);
                    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
                    jd = cj_com->createdJoint();

                    new_jdlist.append(jd);
                }
                d_jdlist.append(jd);
            }

        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new SignalJointAppendedCommand(new_jdlist));
        divideMembersByJoints(d_jdlist, am_act);

        UnifiedEditingData::getInstance()->endUndoStackMacro();
        showInformationMessage(QString(u8"%1個の部材が分割されました。").arg(mdlist.count()));
        setWaitingMode();

    }
}
} // namespace post3dapp
