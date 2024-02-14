#ifndef OUTPUT_PERS_WIDGET_H
#define OUTPUT_PERS_WIDGET_H

//基本方針：npstnへの依存部分は一か所にまとめる。
//形状はUnifiedDataから持ってくる。SHAPEなどもそちらから引いてくる。

//InterfaceWidgetになじむ形で再構成。画面モック　→　骨組み描画　→　断面　→　応力　→　凡例　→　イベント関係。このあたりで設定系を一旦接続
//→　断面検討　→　モード→寄与率など。　ファイルやクラス、includeの整理は後回し

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QVector3D>
#include <QPushButton>
#include "qgl.h"
#include "manage_floordata.h"
#include "manage_framedata.h"
#include "define_outputpers.h"
#include "elided_label.h"
#include "output_pers_viewer.h"
#include "nameselect_dialog.h"
#include "cameravaluedialog.h"
#include "handle_npstn.h"
#include "customdraw_widget.h"

//#include "plot2d_widget.h"
//#include "convert_bld3d_to_post3d.h"

#include "strokefont.h"
namespace post3dapp{

class OutputPersWidget : public InterfaceWidget
{
    Q_OBJECT

public:

    OutputPersWidget( QWidget* parent=nullptr );
    void createSceneView();
    void createButtons();
    void createLayout();
    MODELSETTINGS getWidgetState()const{return MODELSETTINGS();}
//    CustomDrawGrView* getView()const override;
    QToolButton *fitButton;
    QToolButton *zoomInIcon;
    QToolButton *zoomOutIcon;
    QSlider *zoomSlider;
    QComboBox *outTypeCombobox;
    QToolButton *printButton;
        QComboBox *outCaseACombobox;
        QSpinBox *stepSpinBox;
        QLabel *maxStepLabel;

//    void clearModel();
//    void importModel(const QList<MemberData*>& , const QList<OpeningSet *> &oplist);

//    void setModelSettings( const Analysis3DViewSettings & );
//    Analysis3DViewSettings getModelSettings() const;
//    void setCameraValues(XYZ,XYZ,XYZ);

//protected:

//    void closeEvent(QCloseEvent *);

//public slots:

//    void slotRenderView();
//    void slotFitBoundingBox();
//    void slotCreatePng();
//    void slotCreateHQPng();
//    void slotFileOpen();
//    void readSettingFile();
//    void saveSettingFile();

////    void slotExportViewSettings();
//    void slotGetCameraValues();
//    void slotSetCameraValues();
//    void slotSetCameraFront();
//    void slotSetCameraRight();
//    void slotSetCameraTop();
//    void slotSetCameraIsometric();
//    void slotSetCameraOpposite();
//    void slotShowCameraDialog();
//    void showSelectFloorDialog();
//    void showSelectFrameDialog();
//    void showSelectSectionDialog();

//    void slotSetCameraValueFromDialog();
//    void slotGetCameraValueToDialog();

//    void slotComboLoad1Changed(int index);

//    void slotLoadStep1Changed(int step);


signals:

private:
    CameraValueDialog* camdialog;
    QGLWidget* singularGLWidget;
    QStyle* plastiqueStyle;

//    QWidget *menuWidget;
//    QWidget *settingWidget;
    OutputPersScene *myPersScene;
    OutputPersViewer *myPersViewer;

//    QPushButton *openButton;
//    QPushButton *readSettingButton;
//    QPushButton *saveSettingButton;
    QPushButton *frontButton;
    QPushButton *rightButton;
    QPushButton *topButton;
    QPushButton *isometricButton;
    QPushButton *oppositeButton;
    QPushButton *cameraButton;
//    QPushButton *executeButton;
//    QPushButton *executeHQButton;
//    QPushButton* renderButton;
//    QPushButton* plot2DButton;
    QPushButton *limitButton;
    QPushButton *settingButton;


    void slotImportPstnData();
    void slotImportNewPstnData(bool isPost3d = false);

    QList<SectionValues*> currentSectionList;
    QList<JointData*>  currentJointList;
    QList<MemberData*> currentMemberList;
    QList<OpeningSet*> openingList;

    QList<FLOORVALUES> currentFloors;
    QList<FRAMEPOINTSVALUE> currentFrames;


//    QList<QCheckBox*> selectforceC;
//    QList<QCheckBox*> selectforceG;
//    QList<QCheckBox*> selectforceB;
//    QList<QCheckBox*> selectforceW;
//    QButtonGroup *nodeorfacegroup;
//    QButtonGroup *valuecalcmethodgroup;

//    QButtonGroup *reactgroup;
//    QCheckBox *chkreactnum;
//    QDoubleSpinBox *reactscale;

//    QButtonGroup *dispgroup;
//    QSpinBox *spindisp;
//    QCheckBox *chkdispnum;

//    QLineEdit *coefload1;
//    QLineEdit *coefload2;
//    QLineEdit *coefload3;
//    QLineEdit *coefload4;
//    QComboBox *comboload1;
//    QComboBox *comboload2;
//    QComboBox *comboload3;
//    QComboBox *comboload4;
//    QSpinBox *loadstep1;
//    QSpinBox *loadstep2;
//    QSpinBox *loadstep3;
//    QSpinBox *loadstep4;
//    QLabel *loadmaxstep1;
//    QLabel *loadmaxstep2;
//    QLabel *loadmaxstep3;
//    QLabel *loadmaxstep4;

    QButtonGroup* limitedFloorGroup;
    ElidedLabel* limitedFloorLabel;
    QButtonGroup* limitedFrameGroup;
    ElidedLabel* limitedFrameLabel;
    ElidedLabel* limitedSectionLabel;

    XYZ cameraViewPointVector;
    XYZ cameraUpwardVector;
    XYZ cameraFocusVector;
    XYZ boundingMin;
    XYZ boundingMax;

//    QButtonGroup* renderingGroup;
//    QCheckBox *chksmooth;
//    QCheckBox *chkshade;

//    QCheckBox *chknumlabel;
//    QCheckBox *chkdrawpoint;
//    QCheckBox *chkmemberlabel;
//    QCheckBox *chkcoordinate;
//    QCheckBox *chklogo;
//    QCheckBox *chkzwall;
//    QCheckBox *chkopening;
//    QCheckBox *chkdrawgraph;
//    QCheckBox *chkdrawyield;
//    QButtonGroup *selectgraphscalegroup;
//    QLineEdit *graphscale;
    QCheckBox *chkdrawcontour;
//    QButtonGroup *selectdrawcontour;
//    QLineEdit *linemin;
//    QLineEdit *linemax;
//    QCheckBox *chkdrawlegend;
//    QCheckBox *chkfilename;
//    QLineEdit *linefontsize;
    QComboBox *comborendertype;

    QCheckBox *chk_m;
    QCheckBox *chk_n;
    QCheckBox *chk_q;
    QCheckBox *chk_valueLabel;
    QCheckBox *chk_disp;
    QCheckBox *chk_reaction;
    QCheckBox *chk_legend;
    QCheckBox *chk_yield;
    QCheckBox *chk_memberLabel;


    QWidget* createTopMenuWidget();
    QWidget* createTopMenuWidget1();
    QWidget* createTopMenuWidget2();
    QWidget* createBottomMenuWidget();
//    QWidget* createSettingWidget();
    OutputPersViewer* createPersViewer();

    void dataInitialize();
    void setBoundingMinMax();
};
}
#endif
