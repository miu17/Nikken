#include "manage_planeactions.h"

#include <QtGui>
#include <QAction>
#include "customdraw_grview.h"
#include "define_unifieddata.h"
#include "eventoperation_plane.h"
#include "unified_data.h"

namespace post3dapp{
ManagePlaneActions::ManagePlaneActions(PlaneEventOperation *operation)
    : QObject(operation), planeOperation(operation)
{
    myGrView = planeOperation->view();
    myGrScene = planeOperation->scene();
    createActions();

    currentMenu = new QMenu(myGrView);
    currentMenu->addActions(actionList);
    currentMenu->insertSeparator(deleteAction);
    currentMenu->insertSeparator(changeSectionAction);
    currentMenu->insertSeparator(changeOpenAction);
}

void ManagePlaneActions::resetActions(bool isEditMode)
{
    QList<QAction *> actions = myGrView->actions();
    Q_FOREACH (QAction *act, actions)
        myGrView->removeAction(act);
    myGrView->addActions(actionList);
    Q_FOREACH ( QAction *act, actionList ) act->setEnabled(false);
    findAction->setEnabled(isEditMode);
}

void ManagePlaneActions::createActions()
{
    waitingSelectAction = new QAction(u8"選択解除", myGrView);
    waitingSelectAction->setShortcut(tr("Esc"));
    waitingSelectAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( waitingSelectAction, &QAction::triggered, planeOperation, &PlaneEventOperation::setWaitingMode );

    findAction = new QAction(u8"検索", myGrView);
    findAction->setIcon(QIcon(":/icons/search.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( findAction, &QAction::triggered, planeOperation, &PlaneEventOperation::slotFindItems );
    findAction->setEnabled(true);

    deleteAction = new QAction(u8"削除", myGrView);
    deleteAction->setIcon(QIcon(":/icons/delete.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteAction, &QAction::triggered, planeOperation, &PlaneEventOperation::slotDeleteItems );

    changeSectionAction = new QAction(u8"符号の変更", myGrView);
    changeSectionAction->setShortcut(tr("Ctrl+s"));
    changeSectionAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeSectionAction, &QAction::triggered, planeOperation, &PlaneEventOperation::slotChangeSection );

    reverseJointAction = new QAction(u8"節点構成の反転", myGrView);
    reverseJointAction->setShortcut(tr("Ctrl+r"));
    reverseJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( reverseJointAction, &QAction::triggered, planeOperation,
              &PlaneEventOperation::slotReverseMemberJoint );

    moveJointAction = new QAction(u8"端点の移動", myGrView);
    moveJointAction->setShortcut(tr("Ctrl+m"));
    moveJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( moveJointAction, &QAction::triggered, planeOperation,
              &PlaneEventOperation::slotSetMoveWaitingMode );

    changeShiftAction = new QAction(u8"寄り長さの変更", myGrView);
    changeShiftAction->setShortcut(tr("Ctrl+h"));
    changeShiftAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeShiftAction, &QAction::triggered, planeOperation, &PlaneEventOperation::slotChangeShift );

    changeOpenAction = new QAction(u8"壁開口の追加・変更", myGrView);
    changeOpenAction->setShortcut(tr("Ctrl+o"));
    changeOpenAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeOpenAction, &QAction::triggered, planeOperation, &PlaneEventOperation::slotChangeWallOpen );

    deleteOpenAction = new QAction(u8"壁開口の削除", myGrView);
    deleteOpenAction->setShortcut(tr("Ctrl+e"));
    deleteOpenAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteOpenAction, &QAction::triggered, planeOperation, &PlaneEventOperation::slotDeleteWallOpen );

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

void ManagePlaneActions::setActionStatus(DATATYPE dtype, bool isMoveWaiting, bool isSelected,
                                         bool isMultiSelected, bool isMovable)
{
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

void ManagePlaneActions::showContextMenu()
{
    currentMenu->exec(QCursor::pos());
}
} // namespace post3dapp
