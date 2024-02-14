#include "manage_persactions.h"

#include <QtGui>

#include "define_settings.h"
#include "persUI_glview.h"
namespace post3dapp{
ManagePersActions::ManagePersActions(PersInterfaceGLViewer *parent)
    : QObject(parent)
{
    myGLView = parent;
    createActions();

    currentMenu = new QMenu(myGLView);
    currentMenu->addActions(actionList);
    currentMenu->insertSeparator(deleteAction);
    currentMenu->insertSeparator(changeSectionAction);
    currentMenu->insertSeparator(changeAngleAction);
    currentMenu->insertSeparator(changeOpenAction);
}

void ManagePersActions::resetActions()
{
    QList<QAction *> actions = myGLView->actions();
    Q_FOREACH (QAction *act, actions)
        myGLView->removeAction(act);
    myGLView->addActions(actionList);
    findAction->setEnabled(true);
}

void ManagePersActions::createActions()
{
    waitingSelectAction = new QAction(u8"選択解除", myGLView);
    waitingSelectAction->setShortcut(tr("Esc"));
    connect ( waitingSelectAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::slotClearSelectedItems );

    findAction = new QAction(u8"検索", myGLView);
    findAction->setIcon(QIcon(":/icons/search.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( findAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalFindItems );
    findAction->setEnabled(true);

    deleteAction = new QAction(u8"削除", myGLView);
    deleteAction->setIcon(QIcon(":/icons/delete.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    connect ( deleteAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalDeleteItems );

    deleteIndependentAction = new QAction(u8"独立節点の削除", myGLView);
//    deleteIndependentAction->setShortcut(QKeySequence::Delete);
    connect ( deleteIndependentAction, &QAction::triggered, myGLView,
              &PersInterfaceGLViewer::signalDeleteIndependentJoints );

    changeJointAction = new QAction(u8"座標値・属性の変更", myGLView);
//    changeJointAction->setShortcut(QKeySequence::Delete);
    connect ( changeJointAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalShowChangeJointDialog );

    autoAttachAction = new QAction(u8"階・通り属性の自動付加", myGLView);
//    autoAttachAction->setShortcut(tr("Ctrl+t"));
//    autoAttachAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( autoAttachAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalShowAutoAttachDialog );

    changeSectionAction = new QAction(u8"符号の変更", myGLView);
//    changeSectionAction->setShortcut(tr("Ctrl+s"));
//    changeSectionAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeSectionAction, &QAction::triggered, myGLView,
              &PersInterfaceGLViewer::signalChangeSectionDialog );

    reverseJointAction = new QAction(u8"I,J端の入れ替え（節点構成の反転）", myGLView);
//    reverseJointAction->setShortcut(tr("Ctrl+r"));
//    reverseJointAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( reverseJointAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalReverseMemberJoint );

    changeAngleAction = new QAction(u8"コードアングルの変更", myGLView);
//    changeAngleAction->setShortcut(tr("Ctrl+a"));
//    changeAngleAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeAngleAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalShowChangeAngleDialog );

    changeFaceAction = new QAction(u8"フェイス長さの変更", myGLView);
//    changeFaceAction->setShortcut(tr("Ctrl+f"));
//    changeFaceAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeFaceAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalShowChangeFaceDialog );

    changeShiftAction = new QAction(u8"寄り長さの変更", myGLView);
//    changeShiftAction->setShortcut(tr("Ctrl+h"));
//    changeShiftAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeShiftAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalShowChangeShiftDialog );

    changeOpenAction = new QAction(u8"壁開口の追加・変更", myGLView);
//    changeOpenAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( changeOpenAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalChangeWallOpen );

    deleteOpenAction = new QAction(u8"壁開口の削除", myGLView);
//    deleteOpenAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( deleteOpenAction, &QAction::triggered, myGLView, &PersInterfaceGLViewer::signalDeleteWallOpen );


    actionList.append(waitingSelectAction);
    actionList.append(findAction);
    actionList.append(deleteAction);
    actionList.append(deleteIndependentAction);
    actionList.append(changeJointAction);
    actionList.append(autoAttachAction);
    actionList.append(changeSectionAction);
    actionList.append(reverseJointAction);
    actionList.append(changeAngleAction);
    actionList.append(changeFaceAction);
    actionList.append(changeShiftAction);
    actionList.append(changeOpenAction);
    actionList.append(deleteOpenAction);
}

void ManagePersActions::setActionStatus( const QList<DATATYPE> &dtypes  )
{
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

    bool isWallEnable = ( dtypes.contains(DATATYPE::TPWALL) || dtypes.contains(DATATYPE::TPUWALL) ) ;

    changeSectionAction->setEnabled( isMemberEnable );
    reverseJointAction->setEnabled( isMemberEnable );
    changeAngleAction->setEnabled( isAngleEnable );
    changeFaceAction->setEnabled( isFaceEnable );
    changeShiftAction->setEnabled( isShiftEnable );

    changeOpenAction->setVisible( isWallEnable );
    deleteOpenAction->setVisible( isWallEnable );
}

void ManagePersActions::showContextMenu()
{
    currentMenu->exec(QCursor::pos());
}
} // namespace post3dapp
