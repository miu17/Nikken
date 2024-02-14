#ifndef MANAGEWALLACTIONS_H
#define MANAGEWALLACTIONS_H

#include <QObject>
class QAction;
class QMenu;

namespace post3dapp{
enum class DATATYPE;
class CustomDrawGrView;
class InputGrScene;
class WallEventOperation;

class ManageWallActions : public QObject
{
    Q_OBJECT

public:

    ManageWallActions(WallEventOperation *parent);

    void resetActions(bool isEditMode = true);
    void setActionStatus(DATATYPE dtype, bool isMoveWaiting,
                         bool isSelected, bool isMultiSelected, bool isMovable);
    void showContextMenu();

protected:

    WallEventOperation *wallOperation;
    CustomDrawGrView *myGrView;
    InputGrScene *myGrScene;

    void createActions();

    QList<QAction *> actionList;

    QAction *waitingSelectAction;   //　選択モード
    QAction *findAction;            // 検索

    QAction *deleteAction;          //　削除

    QAction *changeSectionAction;   //　断面変更 -> ダイアログ
    QAction *reverseJointAction;    // I,J端の入れ替え
    QAction *moveJointAction;       // 端点の移動
    QAction *changeShiftAction;     //　寄り変更 -> ダイアログ

    QAction *changeOpenAction;      //　開口の追加・変更 -> ダイアログ
    QAction *deleteOpenAction;      //　開口の削除

    QMenu *currentMenu;

};
} // namespace post3dapp
#endif
