#ifndef MANAGELINEACTIONS_H
#define MANAGELINEACTIONS_H

#include <QObject>

class QAction;
class QMenu;

namespace post3dapp{
enum class DATATYPE;
class CustomDrawGrView;
class InputGrScene;
class LineEventOperation;

class ManageLineActions : public QObject
{
    Q_OBJECT

public:

    ManageLineActions(LineEventOperation *parent);

    void resetActions(bool isEditMode = true);
    void setActionStatus(DATATYPE dtype, bool isMoveWaiting,
                         bool isSelected, bool isMultiSelected, bool isMovable);

    void showContextMenu();

protected:

    LineEventOperation *lineOperation;
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

    QAction *divideAction;          // 部材の分割 -> ダイアログ
    QAction *uniteAction;           // 部材の一本化

    QAction *changeAngleAction;     //　コードアングル変更 -> ダイアログ
    QAction *changeFaceAction;      //　フェイス長さ変更 -> ダイアログ
    QAction *changeShiftAction;     //　寄り変更 -> ダイアログ

    QMenu *currentMenu;

};
} // namespace post3dapp
#endif
