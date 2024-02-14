#include "manage_editallactions.h"

#include <QtGui>
#include <QAction>

#include "define_unifieddata.h"
#include "customdraw_grview.h"
#include "eventoperation_editall.h"
#include "unified_data.h"

namespace post3dapp{
ManageEditAllActions::ManageEditAllActions(EditAllEventOperation *operation)
    : QObject(operation), editAllOperation(operation)
{
    myGrView = editAllOperation->view();
    myGrScene = editAllOperation->scene();
    createActions();

    currentMenu = new QMenu(myGrView);
    currentMenu->addActions(actionList);
    currentMenu->insertSeparator(deleteAction);
    currentMenu->insertSeparator(changeSectionAction);
    currentMenu->insertSeparator(moveAction);
    currentMenu->insertSeparator(changeAngleAction);
    currentMenu->insertSeparator(changeOpenAction);
}

void ManageEditAllActions::resetActions()
{
    QList<QAction *> actions = myGrView->actions();
    Q_FOREACH (QAction *act, actions)
        myGrView->removeAction(act);
    myGrView->addActions(actionList);
    Q_FOREACH ( QAction *act, actionList ) act->setEnabled(false);
    findAction->setEnabled(true);
}

void ManageEditAllActions::createActions()
{
    waitingSelectAction = new QAction(u8"選択解除", myGrView);
    waitingSelectAction->setShortcut(tr("Esc"));
    waitingSelectAction->setShortcutContext(Qt::WidgetShortcut);
    connect(waitingSelectAction, &QAction::triggered, editAllOperation, &EditAllEventOperation::setWaitingMode);

    findAction = new QAction(u8"検索", myGrView);
    findAction->setIcon(QIcon(":/icons/search.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( findAction, &QAction::triggered, editAllOperation, &EditAllEventOperation::slotFindItems );
    findAction->setEnabled(true);

    deleteAction = new QAction(u8"削除", myGrView);
    deleteAction->setIcon(QIcon(":/icons/delete.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteAction, &QAction::triggered, editAllOperation, &EditAllEventOperation::slotDeleteItems );

    deleteIndependentAction = new QAction(u8"独立節点の削除", myGrView);
    deleteIndependentAction->setShortcut(tr("Ctrl+q"));
    deleteIndependentAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteIndependentAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotDeleteIndependentJoints );

    changeJointAction = new QAction(u8"座標値・属性の変更", myGrView);
    changeJointAction->setShortcut(tr("Ctrl+j"));
    changeJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeJointAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotShowChangeJointDialog );

    autoAttachAction = new QAction(u8"階・通り属性の自動付加", myGrView);
    autoAttachAction->setShortcut(tr("Ctrl+t"));
    autoAttachAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( autoAttachAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotShowAutoAttachDialog );

    moveAction = new QAction(u8"移動", myGrView);
    moveAction->setIcon(QIcon(":/icons/move.png"));
    moveAction->setShortcut(tr("Ctrl+m"));
    moveAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( moveAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotSetMoveWaitingMode );

    copyAction = new QAction(u8"コピー", myGrView);
    copyAction->setIcon(QIcon(":/icons/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( copyAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotCopyItems );

    pasteAction = new QAction(u8"貼り付け", myGrView);
    pasteAction->setIcon(QIcon(":/icons/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( pasteAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotSetPasteWaitingMode );

    changeSectionAction = new QAction(u8"符号の変更", myGrView);
    changeSectionAction->setShortcut(tr("Ctrl+s"));
    changeSectionAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeSectionAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotChangeSection );

    reverseJointAction = new QAction(u8"I,J端の入れ替え（節点構成の反転）", myGrView);
    reverseJointAction->setShortcut(tr("Ctrl+r"));
    reverseJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( reverseJointAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotReverseMemberJoint );

    changeAngleAction = new QAction(u8"コードアングルの変更", myGrView);
    changeAngleAction->setShortcut(tr("Ctrl+a"));
    changeAngleAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeAngleAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotChangeAngle );

    changeFaceAction = new QAction(u8"フェイス長さの変更", myGrView);
    changeFaceAction->setShortcut(tr("Ctrl+i"));
    changeFaceAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeFaceAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotChangeFace );

    changeShiftAction = new QAction(u8"寄り長さの変更", myGrView);
    changeShiftAction->setShortcut(tr("Ctrl+h"));
    changeShiftAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeShiftAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotChangeShift );

    changeOpenAction = new QAction(u8"壁開口の追加・変更", myGrView);
    changeOpenAction->setShortcut(tr("Ctrl+o"));
    changeOpenAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeOpenAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotChangeWallOpen );

    deleteOpenAction = new QAction(u8"壁開口の削除", myGrView);
    deleteOpenAction->setShortcut(tr("Ctrl+e"));
    deleteOpenAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteOpenAction, &QAction::triggered,
              editAllOperation, &EditAllEventOperation::slotDeleteWallOpen );


    actionList.append(waitingSelectAction);
    actionList.append(findAction);

    actionList.append(deleteAction);
    actionList.append(deleteIndependentAction);
    actionList.append(changeJointAction);
    actionList.append(autoAttachAction);

    actionList.append(changeSectionAction);
    actionList.append(reverseJointAction);

    actionList.append(moveAction);
    actionList.append(copyAction);
    actionList.append(pasteAction);

    actionList.append(changeAngleAction);
    actionList.append(changeFaceAction);
    actionList.append(changeShiftAction);

    actionList.append(changeOpenAction);
    actionList.append(deleteOpenAction);
}

void ManageEditAllActions::setActionStatus(QList<DATATYPE> &dtypes, bool isMoveWaiting,
                                           bool isReadyPaste, bool isActiveWaiting)
{
    if ( isMoveWaiting ) {
        Q_FOREACH ( QAction *act, actionList ) act->setEnabled(false);
        waitingSelectAction->setEnabled(true);
        findAction->setEnabled(true);
        return;
    }

    waitingSelectAction->setEnabled( !dtypes.isEmpty() );

    deleteAction->setEnabled( !dtypes.isEmpty() );

    bool isJointEnable = dtypes.contains(DATATYPE::TPJOINT);

    deleteIndependentAction->setEnabled( isJointEnable );
    changeJointAction->setEnabled( isJointEnable );
    autoAttachAction->setEnabled( isJointEnable );

    bool isAngleEnable = ( dtypes.contains(DATATYPE::TPCOLUMN) || dtypes.contains(DATATYPE::TPGIRDER) ||
                           dtypes.contains(DATATYPE::TPBRACE) || dtypes.contains(DATATYPE::TPBEAM) ) ;

    bool isFaceEnable = ( isAngleEnable || dtypes.contains(DATATYPE::TPDAMPER) || dtypes.contains(DATATYPE::TPISO) );

    bool isShiftEnable = ( isAngleEnable || dtypes.contains(DATATYPE::TPSLAB)
                           || dtypes.contains(DATATYPE::TPWALL) || dtypes.contains(DATATYPE::TPUWALL) ) ;

    bool isMemberEnable = ( isAngleEnable || isFaceEnable || isShiftEnable );

    bool isElementEnable = ( isMemberEnable || dtypes.contains(DATATYPE::TPJOINTLOAD)
                             || dtypes.contains(DATATYPE::TPMEMBERLOAD) || dtypes.contains(DATATYPE::TPSLABLOAD) );

    bool isWallEnable = ( dtypes.contains(DATATYPE::TPWALL) || dtypes.contains(DATATYPE::TPUWALL) ) ;

    changeSectionAction->setEnabled( isElementEnable );
    reverseJointAction->setEnabled( isElementEnable );

    moveAction->setEnabled( dtypes.contains(DATATYPE::TPJOINT) && isActiveWaiting );
    copyAction->setEnabled( dtypes.contains(DATATYPE::TPJOINT) && isActiveWaiting );
    pasteAction->setEnabled( isReadyPaste );

    changeAngleAction->setEnabled( isAngleEnable );
    changeFaceAction->setEnabled( isFaceEnable );
    changeShiftAction->setEnabled( isShiftEnable );

    changeOpenAction->setVisible( isWallEnable );
    deleteOpenAction->setVisible( isWallEnable );
}

void ManageEditAllActions::showContextMenu()
{
    currentMenu->exec(QCursor::pos());
}
} // namespace post3dapp
