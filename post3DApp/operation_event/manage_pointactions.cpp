#include "manage_pointactions.h"

#include <QtGui>

#include "customdraw_grview.h"
#include "eventoperation_point.h"
#include "factory_unifieddata.h"
#include "unified_editingdata.h"

namespace post3dapp{
ManagePointActions::ManagePointActions(PointEventOperation *operation)
    : QObject(operation), pointOperation(operation)
{
    myGrView = pointOperation->view();
    myGrScene = pointOperation->scene();
    isOutput = myGrView->askIsOutput();
    createActions();

    currentMenu = new QMenu(myGrView);
    currentMenu->addActions(actionList);
    currentMenu->insertSeparator(deleteAction);
    currentMenu->insertSeparator(changeSectionAction);
    currentMenu->insertSeparator(moveAction);
    currentMenu->insertSeparator(divideMemberAction);
}

void ManagePointActions::resetActions(bool isEditMode)
{
    QList<QAction *> actions = myGrView->actions();
    Q_FOREACH ( QAction *act, actions )
        myGrView->removeAction(act);
    myGrView->addActions(actionList);
    Q_FOREACH ( QAction *act, actionList ) act->setEnabled(false);
    findAction->setEnabled(isEditMode);
}

void ManagePointActions::createActions()
{
    waitingSelectAction = new QAction(u8"選択解除", myGrView);
    waitingSelectAction->setShortcut(tr("Esc"));
    waitingSelectAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( waitingSelectAction, &QAction::triggered, pointOperation, &PointEventOperation::setWaitingMode );

    findAction = new QAction(u8"検索", myGrView);
    findAction->setIcon(QIcon(":/icons/search.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( findAction, &QAction::triggered, pointOperation, &PointEventOperation::slotFindItems );
    findAction->setEnabled(true);

    deleteAction = new QAction(u8"削除", myGrView);
    deleteAction->setIcon(QIcon(":/icons/delete.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteAction, &QAction::triggered, pointOperation, &PointEventOperation::slotDeleteItems );

    deleteIndependentAction = new QAction(u8"独立節点の削除", myGrView);
    deleteIndependentAction->setShortcut(tr("Ctrl+q"));
    deleteIndependentAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteIndependentAction, &QAction::triggered,
              pointOperation, &PointEventOperation::slotDeleteIndependentJoints );

    changeDialogAction = new QAction(u8"座標値・属性の変更", myGrView);
    changeDialogAction->setShortcut(tr("Ctrl+j"));
    changeDialogAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeDialogAction, &QAction::triggered,
              pointOperation, &PointEventOperation::slotShowChangeJointDialog );

    autoAttachAction = new QAction(u8"階・通り属性の自動付加", myGrView);
    autoAttachAction->setShortcut(tr("Ctrl+t"));
    autoAttachAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( autoAttachAction, &QAction::triggered,
              pointOperation, &PointEventOperation::slotShowAutoAttachDialog );

    changeSectionAction = new QAction(u8"符号の変更", myGrView);
    changeSectionAction->setShortcut(tr("Ctrl+s"));
    changeSectionAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeSectionAction, &QAction::triggered,
              pointOperation, &PointEventOperation::slotChangeSection );

    moveJointAction = new QAction(u8"荷重点の移動", myGrView);
    moveJointAction->setIcon(QIcon(":/icons/move.png"));
    moveJointAction->setShortcut(tr("Ctrl+m"));
    moveJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( moveJointAction, &QAction::triggered,
              pointOperation, &PointEventOperation::slotSetMoveWaitingMode );

    moveAction = new QAction(u8"移動", myGrView);
    moveAction->setIcon(QIcon(":/icons/move.png"));
    moveAction->setShortcut(tr("Ctrl+m"));
    moveAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( moveAction, &QAction::triggered,
              pointOperation, &PointEventOperation::slotSetMoveWaitingMode );

    copyAction = new QAction(u8"コピー", myGrView);
    copyAction->setIcon(QIcon(":/icons/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( copyAction, &QAction::triggered,
              pointOperation, &PointEventOperation::slotCopyItems );

    pasteAction = new QAction(u8"貼り付け", myGrView);
    pasteAction->setIcon(QIcon(":/icons/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( pasteAction, &QAction::triggered,
              pointOperation, &PointEventOperation::slotSetPasteWaitingMode );

    divideMemberAction = new QAction(u8"部材軸上の点の内分化", myGrView);
    divideMemberAction->setShortcut(tr("Ctrl+n"));
    divideMemberAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( divideMemberAction, &QAction::triggered,
              pointOperation, &PointEventOperation::slotDivideMember );

    actionList.append(waitingSelectAction);
    actionList.append(findAction);
    actionList.append(deleteAction);
    actionList.append(deleteIndependentAction);
    actionList.append(changeDialogAction);
    actionList.append(autoAttachAction);
    actionList.append(changeSectionAction);
    actionList.append(moveJointAction);
    actionList.append(moveAction);
    actionList.append(copyAction);
    actionList.append(pasteAction);
    actionList.append(divideMemberAction);
}

void ManagePointActions::setActionStatus(DATATYPE dtype, bool isMoveWaiting, bool isItemSelected,
                                         bool isMultiSelected, bool isItemDetected)
{
    if ( isMoveWaiting ) {
        Q_FOREACH ( QAction *act, actionList ) act->setEnabled(false);
        waitingSelectAction->setEnabled(true);
        findAction->setEnabled(true);
        return;
    }

    moveAction->setVisible(dtype == DATATYPE::TPJOINT);
    changeSectionAction->setVisible(dtype == DATATYPE::TPJOINTLOAD);
    moveJointAction->setVisible(dtype == DATATYPE::TPJOINTLOAD);
    deleteIndependentAction->setVisible(dtype == DATATYPE::TPJOINT);
    copyAction->setVisible(dtype == DATATYPE::TPJOINT);
    pasteAction->setVisible(dtype == DATATYPE::TPJOINT);
    changeDialogAction->setVisible(dtype == DATATYPE::TPJOINT);
    autoAttachAction->setVisible(dtype == DATATYPE::TPJOINT);
    divideMemberAction->setVisible(dtype == DATATYPE::TPJOINT);

    if ( dtype == DATATYPE::TPJOINTLOAD ) {
        changeSectionAction->setEnabled( isItemSelected );
        moveJointAction->setEnabled( isItemSelected && !isMultiSelected );
    } else {
        moveAction->setEnabled( isItemSelected && isItemDetected );
        copyAction->setEnabled( isItemSelected && isItemDetected );
        pasteAction->setEnabled( UnifiedEditingData::getInstance()->existJointCopyBuffer() );
        changeDialogAction->setEnabled( isItemSelected );
        deleteIndependentAction->setEnabled( isItemSelected );
        autoAttachAction->setEnabled( isItemSelected );
        divideMemberAction->setEnabled( isItemSelected );
    }

    waitingSelectAction->setEnabled( isItemSelected );
    deleteAction->setEnabled( isItemSelected );
}

void ManagePointActions::showContextMenu()
{
    currentMenu->exec(QCursor::pos());
}
} // namespace post3dapp
