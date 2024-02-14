#ifndef PERSOUTDRAWWIDGET_H
#define PERSOUTDRAWWIDGET_H

#include "interface_widget.h"

class QComboBox;
class QToolButton;

namespace post3dapp{
class ReferenceCombobox;
class PersOutputScene;
class PersOutputGLViewer;
class PersOutputSettingDialog;
class PersOutputWidget : public InterfaceWidget
{
    Q_OBJECT

public:

    PersOutputWidget(QWidget *parent = nullptr);
    ~PersOutputWidget();

private slots:

    void slotSavePixmap();
    void slotShowControlDialog(bool);
    void slotChangeViewSettings();
    void slotControlButtonUnChecked();

    void resetCaseCombobox(int);
    void changeDrawingFloor(const QString &fl);
    void changeDrawingFrame(const QString &fr);
    void changeOutputCase();
    void resetSceneView();
    void initializePlanView();

    void slotShowTextDialog();

protected:

private:

    PersOutputScene *myScene;
    PersOutputGLViewer *myGLView;
    PersOutputSettingDialog *controlDialog;

    ReferenceCombobox *floorCombobox;
    ReferenceCombobox *frameCombobox;

    QComboBox *outTypeCombobox;
    QComboBox *outCaseACombobox;
    QComboBox *outCaseBCombobox;
    QComboBox *outCaseCCombobox;

    QToolButton *controlButton;
    QToolButton *printButton;

    void createSceneView();
    void createButtons();

};

} // namespace post3dapp
#endif
