#include "manage_lineactions.h"

#include <QtGui>
#include <QAction>

#include "customdraw_grview.h"
#include "define_unifieddata.h"
#include "eventoperation_line.h"
#include "unified_data.h"

namespace post3dapp{
ManageLineActions::ManageLineActions(LineEventOperation *operation)
    : QObject(operation), lineOperation(operation)
{
    myGrView = lineOperation->view();
    myGrScene = lineOperation->scene();
    createActions();

    currentMenu = new QMenu(myGrView);
    currentMenu->addActions(actionList);
    currentMenu->insertSeparator(deleteAction);
    currentMenu->insertSeparator(changeSectionAction);
    currentMenu->insertSeparator(divideAction);
    currentMenu->insertSeparator(changeAngleAction);
}

void ManageLineActions::resetActions(bool isEditMode)
{
    QList<QAction *> actions = myGrView->actions();
    Q_FOREACH (QAction *act, actions)
        myGrView->removeAction(act);
    myGrView->addActions(actionList);
    Q_FOREACH ( QAction *act, actionList ) act->setEnabled(false);
    findAction->setEnabled(isEditMode);
}

void ManageLineActions::createActions()
{
    waitingSelectAction = new QAction(u8"選択解除", myGrView);
    waitingSelectAction->setShortcut(tr("Esc"));
    waitingSelectAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( waitingSelectAction, &QAction::triggered, lineOperation, &LineEventOperation::setWaitingMode );

    findAction = new QAction(u8"検索", myGrView);
    findAction->setIcon(QIcon(":/icons/search.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( findAction, &QAction::triggered, lineOperation, &LineEventOperation::slotFindItems );
    findAction->setEnabled(true);

    deleteAction = new QAction(u8"削除", myGrView);
    deleteAction->setIcon(QIcon(":/icons/delete.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteAction, &QAction::triggered, lineOperation, &LineEventOperation::slotDeleteItems );

    changeSectionAction = new QAction(u8"符号の変更", myGrView);
    changeSectionAction->setShortcut(tr("Ctrl+s"));
    changeSectionAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeSectionAction, &QAction::triggered, lineOperation, &LineEventOperation::slotChangeSection );

    reverseJointAction = new QAction(u8"I,J端の入れ替え", myGrView);
    reverseJointAction->setShortcut(tr("Ctrl+r"));
    reverseJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( reverseJointAction, &QAction::triggered, lineOperation,
              &LineEventOperation::slotReverseMemberJoint );

    moveJointAction = new QAction(u8"端点の移動", myGrView);
    moveJointAction->setShortcut(tr("Ctrl+m"));
    moveJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( moveJointAction, &QAction::triggered, lineOperation, &LineEventOperation::slotSetMoveWaitingMode );

    divideAction = new QAction(u8"部材の分割", myGrView);
    divideAction->setShortcut(tr("Ctrl+d"));
    divideAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( divideAction, &QAction::triggered, lineOperation, &LineEventOperation::slotDivideMember );

    uniteAction = new QAction(u8"部材の結合（一本化）", myGrView);
    uniteAction->setShortcut(tr("Ctrl+u"));
    uniteAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( uniteAction, &QAction::triggered, lineOperation, &LineEventOperation::slotUniteMembers );

    changeAngleAction = new QAction(u8"コードアングルの変更", myGrView);
    changeAngleAction->setShortcut(tr("Ctrl+a"));
    changeAngleAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeAngleAction, &QAction::triggered, lineOperation, &LineEventOperation::slotChangeAngle );

    changeFaceAction = new QAction(u8"フェイス長さの変更", myGrView);
    changeFaceAction->setShortcut(tr("Ctrl+i"));
    changeFaceAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeFaceAction, &QAction::triggered, lineOperation, &LineEventOperation::slotChangeFace );

    changeShiftAction = new QAction(u8"寄り長さの変更", myGrView);
    changeShiftAction->setShortcut(tr("Ctrl+h"));
    changeShiftAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeShiftAction, &QAction::triggered, lineOperation, &LineEventOperation::slotChangeShift );

    actionList.append(waitingSelectAction);
    actionList.append(findAction);
    actionList.append(deleteAction);
    actionList.append(changeSectionAction);
    actionList.append(reverseJointAction);
    actionList.append(moveJointAction);
    actionList.append(divideAction);
    actionList.append(uniteAction);
    actionList.append(changeAngleAction);
    actionList.append(changeFaceAction);
    actionList.append(changeShiftAction);
}

void ManageLineActions::setActionStatus(DATATYPE dtype, bool isMoveWaiting, bool isSelected,
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
    deleteAction->setEnabled( isSelected );

    moveJointAction->setEnabled( isSelected && !isMultiSelected && isMovable );
    reverseJointAction->setEnabled( isSelected );
    divideAction->setEnabled( isSelected );
    uniteAction->setEnabled( isMultiSelected );


    changeFaceAction->setVisible( dtype != DATATYPE::TPMEMBERLOAD );
    if ( dtype != DATATYPE::TPMEMBERLOAD  ) {
        changeFaceAction->setEnabled( isSelected );
    }

    bool isChangeable = ( dtype != DATATYPE::TPMEMBERLOAD && dtype != DATATYPE::TPDAMPER && dtype != DATATYPE::TPISO );
    changeAngleAction->setVisible( isChangeable );
    changeShiftAction->setVisible( isChangeable );

    if ( isChangeable  ) {
        changeAngleAction->setEnabled( isSelected );
        changeShiftAction->setEnabled( isSelected );
    }
}

void ManageLineActions::showContextMenu()
{
    currentMenu->exec(QCursor::pos());
}
} // namespace post3dapp
