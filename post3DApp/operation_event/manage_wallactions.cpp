#include "manage_wallactions.h"

#include <QtGui>
#include <QAction>

#include "customdraw_grview.h"
#include "define_unifieddata.h"
#include "eventoperation_wall.h"
#include "unified_data.h"

namespace post3dapp{
ManageWallActions::ManageWallActions(WallEventOperation *operation)
    : QObject(operation), wallOperation(operation)
{
    qDebug()<<"initwallactions";
    myGrView = wallOperation->view();
    myGrScene = wallOperation->scene();
    createActions();

    currentMenu = new QMenu(myGrView);
    currentMenu->addActions(actionList);
    currentMenu->insertSeparator(deleteAction);
    currentMenu->insertSeparator(changeSectionAction);
    currentMenu->insertSeparator(changeOpenAction);
}

void ManageWallActions::resetActions(bool isEditMode)
{
    qDebug()<<"reset actions";
    QList<QAction *> actions = myGrView->actions();
    Q_FOREACH (QAction *act, actions)
        myGrView->removeAction(act);
    myGrView->addActions(actionList);
    Q_FOREACH ( QAction *act, actionList ) act->setEnabled(false);
    findAction->setEnabled(isEditMode);
}

void ManageWallActions::createActions()
{
    qDebug()<<"create actions";

    waitingSelectAction = new QAction(u8"選択解除", myGrView);
    waitingSelectAction->setShortcut(tr("Esc"));
    waitingSelectAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( waitingSelectAction, &QAction::triggered, wallOperation, &WallEventOperation::setWaitingMode );

    findAction = new QAction(u8"検索", myGrView);
    findAction->setIcon(QIcon(":/icons/search.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( findAction, &QAction::triggered, wallOperation, &WallEventOperation::slotFindItems );
    findAction->setEnabled(true);

    deleteAction = new QAction(u8"削除", myGrView);
    deleteAction->setIcon(QIcon(":/icons/delete.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteAction, &QAction::triggered, wallOperation, &WallEventOperation::slotDeleteItems );

    changeSectionAction = new QAction(u8"符号の変更", myGrView);
    changeSectionAction->setShortcut(tr("Ctrl+s"));
    changeSectionAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeSectionAction, &QAction::triggered, wallOperation, &WallEventOperation::slotChangeSection );

    reverseJointAction = new QAction(u8"節点構成の反転", myGrView);
    reverseJointAction->setShortcut(tr("Ctrl+r"));
    reverseJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( reverseJointAction, &QAction::triggered, wallOperation,
              &WallEventOperation::slotReverseMemberJoint );

    moveJointAction = new QAction(u8"端点の移動", myGrView);
    moveJointAction->setShortcut(tr("Ctrl+m"));
    moveJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( moveJointAction, &QAction::triggered, wallOperation,
              &WallEventOperation::slotSetMoveWaitingMode );

    changeShiftAction = new QAction(u8"寄り長さの変更", myGrView);
    changeShiftAction->setShortcut(tr("Ctrl+h"));
    changeShiftAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeShiftAction, &QAction::triggered, wallOperation, &WallEventOperation::slotChangeShift );

    changeOpenAction = new QAction(u8"Addition and change of wall opening", myGrView);
    changeOpenAction->setShortcut(tr("Ctrl+o"));
    changeOpenAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeOpenAction, &QAction::triggered, wallOperation, &WallEventOperation::slotChangeWallOpen );

    deleteOpenAction = new QAction(u8"壁開口の削除", myGrView);
    deleteOpenAction->setShortcut(tr("Ctrl+e"));
    deleteOpenAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteOpenAction, &QAction::triggered, wallOperation, &WallEventOperation::slotDeleteWallOpen );

    actionList.append(waitingSelectAction);
    actionList.append(findAction);
    actionList.append(deleteAction);
    actionList.append(changeSectionAction);
    actionList.append(reverseJointAction);
    actionList.append(moveJointAction);
    actionList.append(changeShiftAction);
    actionList.append(changeOpenAction);
    actionList.append(deleteOpenAction);
}

void ManageWallActions::setActionStatus(DATATYPE dtype, bool isMoveWaiting, bool isSelected,
                                         bool isMultiSelected, bool isMovable)
{
    qDebug()<<"set action status";

    if ( isMoveWaiting ) {
        Q_FOREACH ( QAction *act, actionList ) act->setEnabled(false);
        waitingSelectAction->setEnabled(true);
        findAction->setEnabled(true);
        return;
    }

    waitingSelectAction->setEnabled( isSelected );
    changeSectionAction->setEnabled( isSelected );
    reverseJointAction->setEnabled( isSelected );
    deleteAction->setEnabled( isSelected );
    moveJointAction->setEnabled( isSelected && !isMultiSelected && isMovable );

    changeShiftAction->setVisible( dtype != DATATYPE::TPSLABLOAD );
    if ( dtype != DATATYPE::TPSLABLOAD ) {
        changeShiftAction->setEnabled( isSelected );
    }

    changeOpenAction->setVisible( dtype == DATATYPE::TPWALL || dtype == DATATYPE::TPUWALL );
    deleteOpenAction->setVisible( dtype == DATATYPE::TPWALL || dtype == DATATYPE::TPUWALL );

    if ( dtype == DATATYPE::TPWALL || dtype == DATATYPE::TPUWALL ) {
        changeOpenAction->setEnabled( isSelected );
        deleteOpenAction->setEnabled( isSelected );
    }
}

void ManageWallActions::showContextMenu()
{
    currentMenu->exec(QCursor::pos());
}
} // namespace post3dapp
