#ifndef OPERATIONPARTSWIDGET_H
#define OPERATIONPARTSWIDGET_H

#include <QWidget>
#include "unified_settings.h"

class QVBoxLayout;
class QHBoxLayout;
class QStackedWidget;
class QButtonGroup;
class QComboBox;
class QLabel;
class QCheckBox;
class QSpinBox;
class QRadioButton;
class QScrollArea;

namespace post3dapp{
/*個別要素の抽象ページ*/
class AbstractElementPage : public QWidget
{
    Q_OBJECT

public:

    AbstractElementPage( QWidget *parent = 0 );

public slots:

    virtual void slotChangeAddEditLayout(int);
    void signalCurrentMode(int);
    void signalCurrentInputName(const QString &);

protected:

    ELEMENTTYPE myElementType;
    QStackedWidget *addEditWidget;
    QWidget *myAddWidget;
    QWidget *myEditWidget;
    QButtonGroup *myButtonGroup;
};


/*個別要素ページの構成ウィジェット*/
class AbstractPageParts : public QWidget
{
    Q_OBJECT

public:

    AbstractPageParts( QWidget *parent = 0 );

public slots:



protected:


};


/*候補点の設定*/
class SnapJointWidget : public QWidget
{
    Q_OBJECT

public:

    SnapJointWidget( QWidget *parent = 0 );

public slots:

    void setWidgetStatus(SNAPSTATE);

private slots:

    void resetDivButtonStatus();
    void uploadSettingStatus();

protected:

    QLabel *snap_label;
    QCheckBox *grid_check;
    QLabel *grid_label;
    QSpinBox *gpitch_spin;
    QCheckBox *cross_check;
    QCheckBox *framedef_check;
    QCheckBox *div_check;
    QCheckBox *divuni_check;
    QLabel *divuni_label;
    QSpinBox *divuni_spin;
    QCheckBox *divdis_check;
    QLabel *divdisI_label;
    QSpinBox *divdisI_spin;
    QLabel *divdisJ_label;
    QSpinBox *divdisJ_spin;
    QLabel* crossoffset_label;
    QSpinBox *crossoffset_spin;

    bool signalOn;

};


/*追加・移動時の設定*/
class AddMoveWidget : public QWidget
{
    Q_OBJECT

public:

    AddMoveWidget( QWidget *parent = 0 );

public slots:

    void setWidgetStatus(ADDMOVEACTION);

private slots:

    void uploadSettingStatus();

protected:

    QButtonGroup *am_group;
    bool signalOn;

};


/*追加・移動された時の設定*/

class AddedMovedWidget : public QWidget
{
    Q_OBJECT

public:

    AddedMovedWidget( QWidget *parent = 0 );

public slots:

    void setWidgetStatus(ADDMOVEDACTION);

private slots:

    void uploadSettingStatus();

protected:

    QButtonGroup *amd_group;
    bool signalOn;

};


/*削除時の設定*/
class DeleteWidget : public QWidget
{
    Q_OBJECT

public:

    DeleteWidget( QWidget *parent = 0 );

public slots:

    void setWidgetStatus(DELETEACTION);

private slots:

    void uploadSettingStatus();

protected:

    QButtonGroup *del_group;
    bool signalOn;

};


/*節点一括追加*/
class JointBatchWidget : public QWidget
{
    Q_OBJECT

public:

    JointBatchWidget( QWidget *parent = 0 );

private slots:

    void uploadSettingStatus();

protected:

    QButtonGroup *batch_group;

    QLabel *grid_label;
    QSpinBox *gpitch_spin;

    bool signalOn;

    void setWidgetStatus(JOINTBATCHACTION, int value = 0);
};

/*スピンボックス*/
class OperationSpinBox : public QSpinBox
{
    Q_OBJECT

public:

    OperationSpinBox( ADDEDITMODE mode, QWidget *parent = 0 );

private slots:

    void uploadSettingStatus();

protected:

    ADDEDITMODE myMode;

    bool signalOn;

    void setWidgetStatus(int);
};

/*削除時の設定*/
class TargetAreaWidget : public QWidget
{
    Q_OBJECT

public:

    TargetAreaWidget( QWidget *parent = 0 );

public slots:

    void setWidgetStatus(TARGETACTION);

private slots:

    void uploadSettingStatus();

protected:

    QButtonGroup *tar_group;
    bool signalOn;

};
} // namespace post3dapp
#endif
