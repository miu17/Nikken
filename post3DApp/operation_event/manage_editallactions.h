#ifndef MANAGEEDITALLACTIONS_H
#define MANAGEEDITALLACTIONS_H

#include <QObject>

class QAction;
class QMenu;

namespace post3dapp{
enum class DATATYPE;
class CustomDrawGrView;
class EditAllEventOperation;
class InputGrScene;

class ManageEditAllActions : public QObject
{
    Q_OBJECT

public:

    ManageEditAllActions(EditAllEventOperation *parent);

    void resetActions();
//    void showContextMenu(QList<DATATYPE> &dtypes, bool isReadyPaste, bool isActiveWaiting);

    void setActionStatus(QList<DATATYPE> &dtypes, bool isMoveWaiting,
                         bool isReadyPaste, bool isActiveWaiting);
    void showContextMenu();

protected:

    EditAllEventOperation *editAllOperation;
    CustomDrawGrView *myGrView;
    InputGrScene *myGrScene;

    void createActions();

    QList<QAction *> actionList;

    QAction *waitingSelectAction;   //　選択モード
    QAction *findAction;            // 検索

    QAction *deleteAction;          //　削除
    QAction *deleteIndependentAction;  //　独立節点の削除
    QAction *changeJointAction;     // 節点属性変更
    QAction *autoAttachAction;      // 自動付加

    QAction *changeSectionAction;   //　断面変更 -> ダイアログ
    QAction *reverseJointAction;    // I,J端の入れ替え

    QAction *moveAction;            // 移動
    QAction *copyAction;            // コピー
    QAction *pasteAction;           // ペースト

    QAction *changeAngleAction;     // コードアングル変更 -> ダイアログ
    QAction *changeFaceAction;      // フェイス長さ変更 -> ダイアログ
    QAction *changeShiftAction;     // 寄り変更 -> ダイアログ

    QAction *changeOpenAction;      //　壁開口の変更 -> ダイアログ
    QAction *deleteOpenAction;      //　壁開口の削除

    QMenu *currentMenu;

};
} // namespace post3dapp
#endif
