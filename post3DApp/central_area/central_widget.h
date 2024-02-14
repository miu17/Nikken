#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>

class QComboBox;
class QMdiSubWindow;
class QMdiArea;
class QToolButton;

namespace post3dapp{
class MessageDialog;
class PropertyDialog;
struct SUBWINSTATUS;

class CentralWidget : public QWidget
{
    Q_OBJECT

public:

    CentralWidget( QWidget *parent = 0 );

    QPoint positionAtRightTop() const;

public slots:

    void slotAddSubWindow();
    void sendActiveStatusToSubWindow(QMdiSubWindow *);
    void slotWindowDestroy();

    void slotChangeTypeComboBox();

//    void slotShowViewSettingDialog(bool);
//    void slotViewSettingButtonUnChecked();
    void slotShowMessageDialog(bool);
    void slotMessageButtonUnChecked();
    void slotShowPropertyDialog(bool);
    void slotPropertyButtonUnChecked();

    void slotUndoButtonEnable(bool);
    void slotRedoButtonEnable(bool);

private:

    QMdiArea *myMDIArea;
    QWidget *windowMakeBar;

//    ViewSettingDialog *viewSettingDialog;
    //MessageDialogとPropertyDialog、UndoRedoはoutputと共用化可能か？
    MessageDialog *messageDialog;
    PropertyDialog *propertyDialog;
//    QToolButton *viewSettingButton;
    QToolButton *messageButton;
    QToolButton *propertyButton;

    QToolButton *undoButton;
    QToolButton *redoButton;

    QComboBox *inoutComboBox;
    QComboBox *typeComboBox;
//    FloorFrameCombobox *nameComboBox;

    void createWindowMakeBar();
    void createSettingDialogs();

    bool checkSubWindowType();
    bool existPersWindow() const;
    SUBWINSTATUS currentSubWindowType() const;

};

} // namespace post3dapp
#endif

