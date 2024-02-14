#ifndef MANAGEPERSACTIONS_H
#define MANAGEPERSACTIONS_H

#include <QtWidgets>
#include <QAction>
#include <QObject>
#include "define_unifieddata.h"
namespace post3dapp{
class PersInterfaceGLViewer;

class ManagePersActions : public QObject
{
    Q_OBJECT

public:

    ManagePersActions(PersInterfaceGLViewer *parent);

    void resetActions();

//    void showContextMenu(const QList<DATATYPE> &dtypes);

    void setActionStatus(const QList<DATATYPE> &dtypes);
    void showContextMenu();

protected:

    PersInterfaceGLViewer *myGLView;

    void createActions();

    QList<QAction *> actionList;

    QAction *waitingSelectAction;               //　選択モード
    QAction *findAction;                        // 検索

    QAction *deleteAction;                      //　削除
    QAction *deleteIndependentAction;           //　独立節点の削除
    QAction *changeJointAction;                 // 節点属性変更
    QAction *autoAttachAction;                  // 自動付加

    QAction *changeSectionAction;               //　断面変更 -> ダイアログ
    QAction *reverseJointAction;                // I,J端の入れ替え

    QAction *changeAngleAction;                 //　コードアングル変更 -> ダイアログ
    QAction *changeFaceAction;                  //　フェイス長さ変更 -> ダイアログ
    QAction *changeShiftAction;                 //　寄り変更 -> ダイアログ

    QAction *changeOpenAction;                  //　壁開口の変更 -> ダイアログ
    QAction *deleteOpenAction;                  //　壁開口の削除

    QMenu *currentMenu;

};
} // namespace post3dapp
#endif
