#ifndef PERSCLDRAWWIDGET_H
#define PERSCLDRAWWIDGET_H

#include "interface_widget.h"

class QComboBox;
class QToolButton;

namespace post3dapp{
class PersCalcLoadGLViewer;
class PersCalcLoadScene;
class PersCalcLoadSettingDialog;
class ReferenceCombobox;


class PersCalcLoadWidget : public InterfaceWidget
{
    Q_OBJECT

public:

    PersCalcLoadWidget(QWidget *parent = nullptr);
    ~PersCalcLoadWidget();

private slots:

    void slotSavePixmap();
    void slotShowControlDialog(bool);
    void slotChangeViewSettings();
    void slotControlButtonUnChecked();
    void slotShowTextDialog();
    void slotClickedPrintButton();

    void resetCaseCombobox(int);
    void changeDrawingFloor(const QString &fl);
    void changeDrawingFrame(const QString &fr);
    void changeOutputCase();
    void initializePlanView();


protected:

private:

    PersCalcLoadScene *myScene;
    PersCalcLoadGLViewer *myGLView;
    PersCalcLoadSettingDialog *controlDialog;

    ReferenceCombobox *floorCombobox;
    ReferenceCombobox *frameCombobox;

//    QComboBox *outTypeCombobox;
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
