#ifndef MANAGEPOINTACTIONS_H
#define MANAGEPOINTACTIONS_H

#include <QObject>
#include "define_unifieddata.h"

class QAction;
class QMenu;

namespace post3dapp{
enum class DATATYPE;
class CustomDrawGrView;
class InputGrScene;
class PointEventOperation;

class ManagePointActions : public QObject
{
    Q_OBJECT

public:

    ManagePointActions(PointEventOperation *parent);

    void resetActions(bool isEditMode = true);
    void setActionStatus(DATATYPE dtype, bool isMoveWaiting, bool isItemSelected,
                         bool isMultiSelected, bool isItemDetected);
    void showContextMenu();

protected:

    PointEventOperation *pointOperation;
    CustomDrawGrView *myGrView;
    InputGrScene *myGrScene;

    void createActions();

    QList<QAction *> actionList;

    QAction *waitingSelectAction;       //　選択モード
    QAction *findAction;                // 検索

    QAction *deleteAction;              // 削除
    QAction *deleteIndependentAction;   // 独立節点の削除
    QAction *changeDialogAction;        // 座標値・属性の変更
    QAction *autoAttachAction;          // 階・通り属性の自動付加

    QAction *changeSectionAction;       // 符号の変更
    QAction *moveJointAction;           // 荷重点の移動

    QAction *moveAction;                // 移動
    QAction *copyAction;                // コピー
    QAction *pasteAction;               // 貼り付け

    QAction *divideMemberAction;        // 部材軸上の点の内分化

    QMenu *currentMenu;

    UnifiedDataType isOutput;

};
} // namespace post3dapp
#endif
