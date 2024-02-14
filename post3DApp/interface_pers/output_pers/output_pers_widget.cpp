#include <QtWidgets>
#include <QDebug>
#include <QLabel>
#include <QtCore>
#include <QPushButton>
#include <QCheckBox>
#include "customdraw_grview.h"
#include <QtGui>

#include "output_pers_widget.h"
//#include "globaldata.h"
#include "calc_any_point_stress.h"
#include "cameravaluedialog.h"
#include "unified_editingdata.h"
#include "output_control.h"

namespace post3dapp{
OutputPersWidget::OutputPersWidget( QWidget* parent)
    : InterfaceWidget( parent )
{
    createSceneView();
    createButtons();
    createLayout();

    //    connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::dataUpdated,
    //            this, &OutputPersWidget::updateElevationView);
    //    connect(UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::dataCleared,
    //            this, &OutputPersWidget::initializePlanView);
    //    connect(UnifiedProject::getInstance(), &UnifiedProject::stressCombi_SettingChanged,
    //            this, &OutputPersWidget::resetSceneView);




}

void OutputPersWidget::createSceneView()
{
    singularGLWidget = new QGLWidget();
    singularGLWidget->setVisible(false);
    camdialog=new CameraValueDialog(this);
    camdialog->setWindowTitle(QString(QString::fromLocal8Bit("カメラアングルの設定")));
    connect ( camdialog->set_button, SIGNAL( clicked() ), this, SLOT( slotSetCameraValueFromDialog() ) );
    connect ( camdialog->get_button, SIGNAL( clicked() ), this, SLOT( slotGetCameraValueToDialog() ) );

    myPersScene =new OutputPersScene;
    myPersViewer=createPersViewer();

}
void OutputPersWidget::createButtons()
{
    qDebug()<<"OutputPersWidget::createButtons in";
    zoomInIcon = new QToolButton(this);
    zoomInIcon->setIcon(QIcon(QPixmap(":/icons/zoomin.png")));
    zoomInIcon->setIconSize(QSize(25, 25));
    zoomInIcon->setAutoRaise(true);
    zoomInIcon->setFixedSize(30, 30);
    zoomInIcon->setAutoRepeat(true);
    zoomInIcon->setAutoRepeatInterval(33);
    zoomInIcon->setAutoRepeatDelay(0);

    zoomOutIcon = new QToolButton(this);
    zoomOutIcon->setIcon(QIcon(QPixmap(":/icons/zoomout.png")));
    zoomOutIcon->setIconSize(QSize(25, 25));
    zoomOutIcon->setAutoRaise(true);
    zoomOutIcon->setFixedSize(30, 30);
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);

    zoomSlider = new QSlider(this);
    zoomSlider->setFixedSize(120, 30);
    zoomSlider->setOrientation(Qt::Horizontal);
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(400);
    zoomSlider->setValue(200);
    zoomSlider->setTickPosition(QSlider::TicksRight);

    //    connect ( zoomSlider, &QAbstractSlider::valueChanged, this, &CustomDrawWidget::slotChangeViewScale );
    //    connect ( zoomInIcon, &QAbstractButton::clicked, this, &CustomDrawWidget::zoomIn );
    //    connect ( zoomOutIcon, &QAbstractButton::clicked, this, &CustomDrawWidget::zoomOut );

    fitButton = new QToolButton(this);
    fitButton->setIcon(QIcon(QPixmap(":/icons/fit.png")));
    fitButton->setAutoRaise(true);
    fitButton->setFixedSize(30, 30);
    fitButton->setIconSize(QSize(25, 25));

    //    connect(fitButton, &QToolButton::clicked, getView(), &CustomDrawGrView::fitWindow);
    //    frameCombobox = new ReferenceCombobox(DATATYPE::TPFRAMEP, UnifiedDataType::Pstn, this);
    //    frameCombobox->setDefaultName(u8"（通りの選択）");
    //    frameCombobox->setFixedWidth(110);

    //    connect(frameCombobox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged),
    //            this, &ElevationOutputWidget::changeDrawingFrame);

    outTypeCombobox = new QComboBox(this);
    outTypeCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
    outTypeCombobox->setFixedWidth(200);

//    outTypeCombobox->addItems(OutputControl::getInstance()->outputTypeStringList(OUTFIGCATEGORY::PERS));
    outTypeCombobox->addItems(QStringList()<<u8"応力変形"<<u8"断面検討"<<u8"モード"<<u8"寄与率");

    //    connect(outTypeCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //            this, &ElevationOutputWidget::resetCaseCombobox);

    outCaseACombobox = new QComboBox(this);
    outCaseACombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
    outCaseACombobox->setFixedWidth(100);

    stepSpinBox = new QSpinBox(this);
    stepSpinBox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle());
    stepSpinBox->setFixedWidth(50);
    maxStepLabel = new QLabel(tr("/0"), this);

    //    connect(outCaseACombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //            this, &ElevationOutputWidget::changeOutputCase);
    //    connect(outCaseBCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //            this, &ElevationOutputWidget::changeOutputCaseB);
    //    connect(outCaseCCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //            this, &ElevationOutputWidget::changeOutputCase);
    //    connect(stepSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
    //            this, &ElevationOutputWidget::changeOutputCase);

    //    resetCaseCombobox(0);

    //    controlButton=new QToolButton(this);
    //    controlButton->setAutoRaise(true);
    //    controlButton->setIcon(QIcon(QPixmap(":/icons/control.png")));
    //    controlButton->setToolTip(tr("Individual window display setting"));
    //    controlButton->setIconSize(QSize(30,30));
    //    controlButton->setFixedSize(30,30);

    printButton = new QToolButton(this);
    printButton->setAutoRaise(true);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));
    //    connect(printButton, &QAbstractButton::clicked,
    //            this, &ElevationOutputWidget::slotClickedPrintButton);



    frontButton=new QPushButton(u8"前",this);
    frontButton->setFixedSize(QSize(25,25));
    rightButton=new QPushButton(u8"右",this);
    rightButton->setFixedSize(QSize(25,25));
    topButton=new QPushButton(u8"上",this);
    topButton->setFixedSize(QSize(25,25));
    isometricButton=new QPushButton(u8"斜",this);
    isometricButton->setFixedSize(QSize(25,25));
    oppositeButton=new QPushButton(u8"逆",this);
    oppositeButton->setFixedSize(QSize(25,25));

    settingButton=new QPushButton(u8"描画設定",this);
    settingButton->setFixedSize(QSize(60,25));
    limitButton=new QPushButton(u8"範囲指定",this);
    limitButton->setFixedSize(QSize(60,25));
    cameraButton=new QPushButton(u8"カメラ",this);
    cameraButton->setFixedSize(QSize(60,25));


    chk_m = new QCheckBox(u8"M");
    chk_n = new QCheckBox(u8"N");
    chk_q = new QCheckBox(u8"Q");
    chk_valueLabel = new QCheckBox(u8"数値");
    chk_disp = new QCheckBox(u8"変位");
    chk_reaction = new QCheckBox(u8"反力");
    chk_memberLabel = new QCheckBox(u8"符号");
    chk_yield = new QCheckBox(u8"塑性化");
    chk_legend = new QCheckBox(u8"凡例");
        comborendertype = new QComboBox(this);
        comborendertype->addItems(QStringList()<<u8"ポリゴン"<<u8"ライン"<<u8"面のみ");
        chkdrawcontour = new QCheckBox(u8"コンターカラー");


}
void OutputPersWidget::createLayout()
{
    QVBoxLayout* vlay = new QVBoxLayout;
    vlay->addWidget(createTopMenuWidget());
    vlay->addWidget(myPersViewer);
    vlay->addWidget(createBottomMenuWidget());
    this->setLayout(vlay);
}
QWidget* OutputPersWidget::createTopMenuWidget()
{
    //1段目：アプリ全般
    //タイプ
    //設定
    //範囲設定
    //カメラ
    //スナップショット

    //2段目：タイプごとのサブウィジェット
    //曲げ・軸・せん断の表示チェック
    //数値の表示チェック
    //反力のチェック
    //
    QWidget* wid1 = createTopMenuWidget1();
    QWidget* wid2 = createTopMenuWidget2();
    QVBoxLayout* vlay=new QVBoxLayout;
    vlay->addWidget(wid1);
    vlay->addWidget(wid2);
    QWidget* wid = new QWidget();
    wid->setLayout(vlay);
    return wid;



}
QWidget* OutputPersWidget::createTopMenuWidget1(){

    QHBoxLayout *hlay1 = new QHBoxLayout;
    //    hlay1->addWidget(frameCombobox, 0);
    //    hlay1->addSpacing(10);
    hlay1->addWidget(outTypeCombobox, 0);
    hlay1->addStretch();
    hlay1->addSpacing(10);
    hlay1->addWidget(settingButton, 0);
    hlay1->addWidget(limitButton, 0);
    hlay1->addWidget(cameraButton, 0);
    hlay1->addSpacing(10);

    hlay1->addWidget(topButton, 0);
    hlay1->addWidget(rightButton, 0);
    hlay1->addWidget(frontButton, 0);
    hlay1->addWidget(isometricButton, 0);
    hlay1->addWidget(oppositeButton, 0);
    hlay1->addSpacing(10);


    hlay1->addWidget(zoomOutIcon, 0);
    hlay1->addWidget(zoomSlider, 0);
    hlay1->addWidget(zoomInIcon, 0);
    hlay1->addSpacing(5);
    hlay1->addWidget(fitButton, 0);
    hlay1->addWidget(printButton, 0);

    QWidget* wid = new QWidget;
    wid->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    wid->setLayout(hlay1);
    return wid;
}
QWidget* OutputPersWidget::createTopMenuWidget2(){
    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(chk_m, 0);
    hlay1->addWidget(chk_n, 0);
    hlay1->addWidget(chk_q, 0);
    hlay1->addWidget(chk_valueLabel, 0);
    hlay1->addWidget(chk_disp, 0);
    hlay1->addWidget(chk_reaction, 0);
    hlay1->addWidget(chk_yield, 0);
    hlay1->addStretch();
    hlay1->addWidget(chkdrawcontour, 0);
    hlay1->addWidget(chk_legend, 0);
    hlay1->addWidget(chk_memberLabel, 0);
    hlay1->addWidget(comborendertype, 0);

    QWidget* wid = new QWidget;
    wid->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    wid->setLayout(hlay1);
    return wid;

}
QWidget* OutputPersWidget::createBottomMenuWidget()
{
    //    QComboBox *outCaseACombobox;
    //    QSpinBox *stepSpinBox;
    //    QLabel *maxStepLabel;

    //タイプごとのサブウィジェット。断面検討ケース、モード、応力ケース
    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->addWidget(outCaseACombobox, 0);
    hlay2->addSpacing(5);
    hlay2->addWidget(stepSpinBox, 0);
    hlay2->addSpacing(5);
    hlay2->addWidget(maxStepLabel, 0);
    hlay2->addStretch();
    hlay2->addSpacing(10);

    QWidget* wid = new QWidget;
    wid->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    wid->setLayout(hlay2);
    return wid;

}


//OutputPersWidget::OutputPersWidget( QWidget* parent)
//    : QWidget( parent )
//{



//    singularGLWidget = new QGLWidget();
//    singularGLWidget->setVisible(false);
//    myPersScene =new OutputPersScene;
//    //    plot2dwidget=NULL;
//    //    clearModel();//空の配列で初期化
//    camdialog=new CameraValueDialog(this);
//    camdialog->setWindowTitle(QString(QString::fromLocal8Bit("カメラアングルの設定")));
//    connect ( camdialog->set_button, SIGNAL( clicked() ), this, SLOT( slotSetCameraValueFromDialog() ) );
//    connect ( camdialog->get_button, SIGNAL( clicked() ), this, SLOT( slotGetCameraValueToDialog() ) );

//    menuWidget=createTopMenuWidget();
//    settingWidget=createSettingWidget();
//    myPersViewer=createPersViewer();

////    qDebug()<<"null";
////    pstn=nullptr;
////    npstn=nullptr;
////    qDebug()<<"nulled";

////    this->setModelSettings( MODEMSETTINGS() );

//    QSplitter *h_splitter = new QSplitter(Qt::Horizontal);
//    h_splitter->addWidget(settingWidget);
//    h_splitter->addWidget(myPersViewer);
//    h_splitter->setStyle(new QProxyStyle);

//    QVBoxLayout *lay=new QVBoxLayout;
//    lay->addWidget(menuWidget);
//    lay->addWidget(h_splitter);
//    this->setLayout(lay);

//    //    this->setWindowIcon(QIcon(":/icons/3DVisual.ico"));
////    this->setWindowTitle(QString::fromLocal8Bit("OutputPers");

////    connect ( openButton, SIGNAL( clicked() ), this, SLOT( slotFileOpen() ) );
////    connect ( readSettingButton, SIGNAL( clicked() ), this, SLOT( readSettingFile() ) );
////    connect ( saveSettingButton, SIGNAL( clicked() ), this, SLOT( saveSettingFile() ) );
////    connect ( renderButton, SIGNAL( clicked() ), this, SLOT( slotRenderView() ) );
////    connect ( frontButton, SIGNAL( clicked() ), this, SLOT( slotSetCameraFront() ) );
////    connect ( rightButton, SIGNAL( clicked() ), this, SLOT( slotSetCameraRight() ) );
////    connect ( topButton, SIGNAL( clicked() ), this, SLOT( slotSetCameraTop() ) );
////    connect ( isometricButton, SIGNAL( clicked() ), this, SLOT( slotSetCameraIsometric() ) );
////    connect ( oppositeButton, SIGNAL( clicked() ), this, SLOT( slotSetCameraOpposite() ) );
////    connect ( cameraButton, SIGNAL( clicked() ), this, SLOT( slotShowCameraDialog() ) );
////    connect ( fitButton, SIGNAL( clicked() ), this, SLOT( slotFitBoundingBox() ) );
////    connect ( executeButton, SIGNAL( clicked() ), this, SLOT( slotCreatePng() ) );
////    connect ( executeHQButton, SIGNAL( clicked() ), this, SLOT( slotCreateHQPng() ) );
////    connect ( plot2DButton, SIGNAL( clicked() ), this, SLOT( slotCreatePlot2DWidget() ) );
//}

//QWidget* OutputPersWidget::createTopMenuWidget()
//{
//    openButton=new QPushButton(QIcon(":/icons/open.png"),QString::fromLocal8Bit(" Open "),this);
//    openButton->setFixedSize(QSize(90,25));
//    openButton->setIconSize(QSize(16,16));
//    openButton->setToolTip(QString::fromLocal8Bit("ファイルを開く"));
//    readSettingButton=new QPushButton(QString::fromLocal8Bit("設定読込"),this);
//    //    readSettingButton->setFixedSize(QSize(90,25));
//    readSettingButton->setFixedHeight(25);
//    saveSettingButton=new QPushButton(QString::fromLocal8Bit("設定保存"),this);
//    saveSettingButton->setFixedSize(QSize(90,25));
//    frontButton=new QPushButton(QString::fromLocal8Bit("front"),this);
//    frontButton->setFixedSize(QSize(90,25));
//    rightButton=new QPushButton(QString::fromLocal8Bit("right"),this);
//    rightButton->setFixedSize(QSize(90,25));
//    topButton=new QPushButton(QString::fromLocal8Bit("top"),this);
//    topButton->setFixedSize(QSize(90,25));
//    isometricButton=new QPushButton(QString::fromLocal8Bit("isometric"),this);
//    isometricButton->setFixedSize(QSize(90,25));
//    oppositeButton=new QPushButton(QString::fromLocal8Bit("opposite"),this);
//    oppositeButton->setFixedSize(QSize(90,25));
//    cameraButton=new QPushButton(QString::fromLocal8Bit("カメラ設定"),this);
//    cameraButton->setFixedSize(QSize(90,25));
//    cameraButton->setVisible(false);
//    renderButton=new QPushButton(QIcon(":/icons/right.png"),QString::fromLocal8Bit("再描画"),this);
//    renderButton->setFixedSize(QSize(90,25));
//    renderButton->setIconSize(QSize(16,16));
//    renderButton->setToolTip(QString::fromLocal8Bit("再描画"));
//    fitButton=new QPushButton(QString::fromLocal8Bit("Fit"),this);
//    fitButton->setFixedSize(QSize(90,25));
//    fitButton->setToolTip(QString::fromLocal8Bit("Fit"));
//    plot2DButton=new QPushButton(QString::fromLocal8Bit("2DPlot"),this);
//    plot2DButton->setFixedSize(QSize(90,25));
//    plot2DButton->setVisible(false);

//    executeButton=new QPushButton(QString::fromLocal8Bit("PNG出力"),this);
//    executeButton->setFixedSize(QSize(90,25));
//    executeHQButton=new QPushButton(QString::fromLocal8Bit("高画質出力"),this);
//    executeHQButton->setFixedSize(QSize(90,25));

//    QHBoxLayout *hlay1=new QHBoxLayout;
//    hlay1->addWidget(openButton);
//    hlay1->addWidget(readSettingButton);
//    hlay1->addWidget(saveSettingButton);
//    hlay1->addSpacing(20);
//    hlay1->addWidget(frontButton);
//    hlay1->addWidget(rightButton);
//    hlay1->addWidget(topButton);
//    hlay1->addWidget(isometricButton);
//    hlay1->addWidget(oppositeButton);
//    hlay1->addWidget(fitButton);
//    hlay1->addWidget(cameraButton);
//    hlay1->addSpacing(20);
//    hlay1->addWidget(renderButton);
//    hlay1->addStretch();
//    hlay1->addWidget(plot2DButton);
//    hlay1->addWidget(executeButton);
//    hlay1->addWidget(executeHQButton);

//    QWidget* menu=new QWidget(this);
//    menu->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
//    menu->setLayout(hlay1);
//    return menu;
//}



//QWidget* OutputPersWidget::createSettingWidget()
//{
//    QTabWidget *tabWidget=new QTabWidget(this);
//    QWidget *modelSettingWidget=new QWidget(this);
//    QWidget *viewSettingWidget=new QWidget(this);
//    tabWidget->addTab(modelSettingWidget,QString::fromLocal8Bit("モデル設定"));
//    tabWidget->addTab(viewSettingWidget,QString::fromLocal8Bit("描画設定"));

//    //モデル設定
//    QGroupBox *selectload=new QGroupBox(QString::fromLocal8Bit("荷重ケース選択"),this);
//    QGroupBox *selectreact=new QGroupBox(QString::fromLocal8Bit("反力表示選択"),this);
//    QGroupBox *selectforce=new QGroupBox(QString::fromLocal8Bit("応力選択"),this);
//    QGroupBox *selectdisp=new QGroupBox(QString::fromLocal8Bit("表示変形倍率"),this);

//    //描画設定
//    QGroupBox *renderstyle=new QGroupBox(QString::fromLocal8Bit("描画形式"),this);
//    QGroupBox *selectgraphscalebox =new QGroupBox(QString::fromLocal8Bit("応力図スケール設定"),this);
//    QGroupBox *selectcolor=new QGroupBox(QString::fromLocal8Bit("コンターカラー設定"),this);
//    QGroupBox *displayscope=new QGroupBox(QString::fromLocal8Bit("表示範囲"),this);
//    QGroupBox *renderingsetting=new QGroupBox(QString::fromLocal8Bit("レンダリング"),this);
//    QGroupBox *selectoption=new QGroupBox(QString::fromLocal8Bit("オプション"),this);

//    selectcolor->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);

//    //selectload
//    //pstnを読んだ時に荷重ケース/最大ステップを読み込む
//    QGridLayout *layselectload=new QGridLayout;
//    //    QVBoxLayout *layselectload=new QVBoxLayout;

//    coefload1=new QLineEdit(this);
//    coefload1->setValidator(new QDoubleValidator(-9999.99,9999.99,2,this));
//    coefload1->setAlignment(Qt::AlignRight);
//    coefload1->setFixedWidth(80);
//    comboload1=new QComboBox(this);
//    comboload1->setFixedWidth(130);
//    coefload2=new QLineEdit(this);
//    coefload2->setValidator(new QDoubleValidator(-9999.99,9999.99,2,this));
//    coefload2->setAlignment(Qt::AlignRight);
//    coefload2->setFixedWidth(80);
//    comboload2=new QComboBox(this);
//    comboload2->setFixedWidth(130);
//    coefload3=new QLineEdit(this);
//    coefload3->setValidator(new QDoubleValidator(-9999.99,9999.99,2,this));
//    coefload3->setAlignment(Qt::AlignRight);
//    coefload3->setFixedWidth(80);
//    comboload3=new QComboBox(this);
//    comboload3->setFixedWidth(130);
//    coefload4=new QLineEdit(this);
//    coefload4->setValidator(new QDoubleValidator(-9999.99,9999.99,2,this));
//    coefload4->setAlignment(Qt::AlignRight);
//    coefload4->setFixedWidth(80);
//    comboload4=new QComboBox(this);
//    comboload4->setFixedWidth(130);

//    loadstep1=new QSpinBox(this);
//    loadstep2=new QSpinBox(this);
//    loadstep3=new QSpinBox(this);
//    loadstep4=new QSpinBox(this);
//    loadstep1->setMinimum(0);
//    loadstep2->setMinimum(0);
//    loadstep3->setMinimum(0);
//    loadstep4->setMinimum(0);
//    loadstep1->setFixedWidth(80);
//    loadstep2->setFixedWidth(80);
//    loadstep3->setFixedWidth(80);
//    loadstep4->setFixedWidth(80);

//    loadmaxstep1=new QLabel(QString::fromLocal8Bit("0"),this);
//    loadmaxstep2=new QLabel(QString::fromLocal8Bit("0"),this);
//    loadmaxstep3=new QLabel(QString::fromLocal8Bit("0"),this);
//    loadmaxstep4=new QLabel(QString::fromLocal8Bit("0"),this);

//    QLabel *labelloadcoef=new QLabel(QString::fromLocal8Bit("係数"),this);
//    QLabel *labelloadcase=new QLabel(QString::fromLocal8Bit("荷重ケース"),this);
//    QLabel *labelloadstep=new QLabel(QString::fromLocal8Bit("ステップ"),this);
//    QLabel *labelloadmaxstep=new QLabel(QString::fromLocal8Bit("　最大　"),this);
//    layselectload->addWidget(labelloadcoef,0,0);
//    layselectload->addWidget(labelloadcase,0,1);
//    layselectload->addWidget(labelloadstep,0,2);
//    layselectload->addWidget(labelloadmaxstep,0,3);
//    layselectload->setAlignment(labelloadcoef,Qt::AlignCenter);
//    layselectload->setAlignment(labelloadcase,Qt::AlignCenter);
//    layselectload->setAlignment(labelloadstep,Qt::AlignCenter);
//    layselectload->setAlignment(labelloadmaxstep,Qt::AlignCenter);
//    layselectload->addWidget(coefload1,1,0);
//    layselectload->addWidget(coefload2,2,0);
//    layselectload->addWidget(coefload3,3,0);
//    layselectload->addWidget(coefload4,4,0);
//    layselectload->addWidget(comboload1,1,1);
//    layselectload->addWidget(comboload2,2,1);
//    layselectload->addWidget(comboload3,3,1);
//    layselectload->addWidget(comboload4,4,1);
//    layselectload->addWidget(loadstep1,1,2);
//    layselectload->addWidget(loadstep2,2,2);
//    layselectload->addWidget(loadstep3,3,2);
//    layselectload->addWidget(loadstep4,4,2);
//    layselectload->addWidget(loadmaxstep1,1,3);
//    layselectload->addWidget(loadmaxstep2,2,3);
//    layselectload->addWidget(loadmaxstep3,3,3);
//    layselectload->addWidget(loadmaxstep4,4,3);
//    layselectload->setAlignment(loadmaxstep1,Qt::AlignRight);
//    layselectload->setAlignment(loadmaxstep2,Qt::AlignRight);
//    layselectload->setAlignment(loadmaxstep3,Qt::AlignRight);
//    layselectload->setAlignment(loadmaxstep4,Qt::AlignRight);
//    connect ( comboload1, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotComboLoad1Changed(int) ) );
//    connect ( comboload2, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotComboLoad2Changed(int) ) );
//    connect ( comboload3, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotComboLoad3Changed(int) ) );
//    connect ( comboload4, SIGNAL( currentIndexChanged(int) ), this, SLOT( slotComboLoad4Changed(int) ) );
//    connect ( loadstep1, SIGNAL( valueChanged(int) ), this, SLOT( slotLoadStep1Changed(int) ) );
//    connect ( loadstep2, SIGNAL( valueChanged(int) ), this, SLOT( slotLoadStep2Changed(int) ) );
//    connect ( loadstep3, SIGNAL( valueChanged(int) ), this, SLOT( slotLoadStep3Changed(int) ) );
//    connect ( loadstep4, SIGNAL( valueChanged(int) ), this, SLOT( slotLoadStep4Changed(int) ) );


//    //    QHBoxLayout *load1=new QHBoxLayout;
//    //    QHBoxLayout *load2=new QHBoxLayout;
//    //    load1->addWidget(coefload1);
//    //    load1->addWidget(comboload1);
//    //    load1->addStretch();
//    //    load1->addWidget(coefload2);
//    //    load1->addWidget(comboload2);
//    //    load2->addWidget(coefload3);
//    //    load2->addWidget(comboload3);
//    //    load2->addStretch();
//    //    load2->addWidget(coefload4);
//    //    load2->addWidget(comboload4);

//    //    layselectload->addLayout(load1);
//    //    layselectload->addLayout(load2);
//    selectload->setLayout(layselectload);


//    //selectforce  柱梁ブレース壁で分ける
//    QCheckBox *AxC=new QCheckBox(QString::fromLocal8Bit("N"),this);
//    QCheckBox *QyC=new QCheckBox(QString::fromLocal8Bit("Qy"),this);
//    QCheckBox *QzC=new QCheckBox(QString::fromLocal8Bit("Qz"),this);
//    QCheckBox *MxC=new QCheckBox(QString::fromLocal8Bit("Mx"),this);
//    QCheckBox *MyC=new QCheckBox(QString::fromLocal8Bit("My"),this);
//    QCheckBox *MzC=new QCheckBox(QString::fromLocal8Bit("Mz"),this);
//    selectforceC << AxC << QyC << QzC << MxC << MyC << MzC;

//    QHBoxLayout *layforceC=new QHBoxLayout;
//    layforceC->addWidget(AxC);
//    layforceC->addWidget(QyC);
//    layforceC->addWidget(QzC);
//    layforceC->addWidget(MxC);
//    layforceC->addWidget(MyC);
//    layforceC->addWidget(MzC);
//    QGroupBox *forceCbox =new QGroupBox(QString::fromLocal8Bit("柱"),this);
//    forceCbox->setLayout(layforceC);

//    QCheckBox *AxG=new QCheckBox(QString::fromLocal8Bit("N"),this);
//    QCheckBox *QyG=new QCheckBox(QString::fromLocal8Bit("Qy"),this);
//    QCheckBox *QzG=new QCheckBox(QString::fromLocal8Bit("Qz"),this);
//    QCheckBox *MxG=new QCheckBox(QString::fromLocal8Bit("Mx"),this);
//    QCheckBox *MyG=new QCheckBox(QString::fromLocal8Bit("My"),this);
//    QCheckBox *MzG=new QCheckBox(QString::fromLocal8Bit("Mz"),this);
//    selectforceG << AxG << QyG << QzG << MxG << MyG << MzG;

//    QHBoxLayout *layforceG=new QHBoxLayout;
//    layforceG->addWidget(AxG);
//    layforceG->addWidget(QyG);
//    layforceG->addWidget(QzG);
//    layforceG->addWidget(MxG);
//    layforceG->addWidget(MyG);
//    layforceG->addWidget(MzG);
//    QGroupBox *forceGbox =new QGroupBox(QString::fromLocal8Bit("梁"),this);
//    forceGbox->setLayout(layforceG);

//    QCheckBox *AxB=new QCheckBox(QString::fromLocal8Bit("N"),this);
//    QCheckBox *QyB=new QCheckBox(QString::fromLocal8Bit("Qy"),this);
//    QCheckBox *QzB=new QCheckBox(QString::fromLocal8Bit("Qz"),this);
//    QCheckBox *MxB=new QCheckBox(QString::fromLocal8Bit("Mx"),this);
//    QCheckBox *MyB=new QCheckBox(QString::fromLocal8Bit("My"),this);
//    QCheckBox *MzB=new QCheckBox(QString::fromLocal8Bit("Mz"),this);
//    selectforceB << AxB << QyB << QzB << MxB << MyB << MzB;

//    QHBoxLayout *layforceB=new QHBoxLayout;
//    layforceB->addWidget(AxB);
//    layforceB->addWidget(QyB);
//    layforceB->addWidget(QzB);
//    layforceB->addWidget(MxB);
//    layforceB->addWidget(MyB);
//    layforceB->addWidget(MzB);
//    QGroupBox *forceBbox =new QGroupBox(QString::fromLocal8Bit("ブレース"),this);
//    forceBbox->setLayout(layforceB);

//    QCheckBox *NW=new QCheckBox(QString::fromLocal8Bit("N"),this);
//    QCheckBox *QW=new QCheckBox(QString::fromLocal8Bit("Q"),this);
//    QCheckBox *MW=new QCheckBox(QString::fromLocal8Bit("M"),this);
//    selectforceW << NW << QW <<  MW;

//    QHBoxLayout *layforceW=new QHBoxLayout;
//    layforceW->addWidget(NW);
//    layforceW->addWidget(QW);
//    layforceW->addWidget(MW);
//    QGroupBox *forceWbox =new QGroupBox(QString::fromLocal8Bit("耐力壁"),this);
//    forceWbox->setLayout(layforceW);

//    //壁コンター消すときは以下
//    //    forceWbox->setVisible(false);
//    QLabel *labelnodeorface=new QLabel(QString::fromLocal8Bit("応力の種類："),this);
//    QRadioButton *rNode=new QRadioButton(QString::fromLocal8Bit("節点応力"),this);
//    QRadioButton *rFace=new QRadioButton(QString::fromLocal8Bit("フェイス応力"),this);
//    nodeorfacegroup=new QButtonGroup(this);
//    nodeorfacegroup->addButton(rNode,0);
//    nodeorfacegroup->addButton(rFace,1);
//    QHBoxLayout *nodeorface=new QHBoxLayout;
//    nodeorface->addWidget(labelnodeorface);
//    nodeorface->addWidget(rNode);
//    nodeorface->addWidget(rFace);

//    QLabel *labelvaluehandling=new QLabel(QString::fromLocal8Bit("複数応力選択時："),this);
//    QRadioButton *rMax=new QRadioButton(QString::fromLocal8Bit("最大"),this);
//    QRadioButton *rMin=new QRadioButton(QString::fromLocal8Bit("最小"),this);
//    QRadioButton *rAbs=new QRadioButton(QString::fromLocal8Bit("絶対値最大"),this);
//    valuecalcmethodgroup=new QButtonGroup(this);
//    valuecalcmethodgroup->addButton(rMax,0);
//    valuecalcmethodgroup->addButton(rMin,1);
//    valuecalcmethodgroup->addButton(rAbs,2);
//    QHBoxLayout *valuecalcmethod=new QHBoxLayout;
//    valuecalcmethod->addWidget(labelvaluehandling);
//    valuecalcmethod->addWidget(rAbs);
//    valuecalcmethod->addWidget(rMax);
//    valuecalcmethod->addWidget(rMin);

//    QVBoxLayout* layselectforce=new QVBoxLayout;
//    layselectforce->addWidget(forceCbox);
//    layselectforce->addWidget(forceGbox);
//    layselectforce->addWidget(forceBbox);
//    layselectforce->addWidget(forceWbox);
//    layselectforce->addLayout(nodeorface);
//    layselectforce->addLayout(valuecalcmethod);
//    selectforce->setLayout(layselectforce);

//    //reaction
//    chkreactnum = new QCheckBox(QString::fromLocal8Bit("数値"),this);
//    QLabel *labelreactscale=new QLabel(QString::fromLocal8Bit("表示倍率："),this);
//    reactscale =new QDoubleSpinBox(this);
//    reactscale->setMaximum(9999.999);
//    reactscale->setValue(1.0);
//    reactscale->setMinimum(0);
//    QRadioButton *rNone=new QRadioButton(QString::fromLocal8Bit("非表示"),this);
//    QRadioButton *rFx=new QRadioButton(QString::fromLocal8Bit("FX"),this);
//    QRadioButton *rFy=new QRadioButton(QString::fromLocal8Bit("FY"),this);
//    QRadioButton *rFz=new QRadioButton(QString::fromLocal8Bit("FZ"),this);
//    QRadioButton *rFxyz=new QRadioButton(QString::fromLocal8Bit("FXYZ"),this);
//    QRadioButton *rMx=new QRadioButton(QString::fromLocal8Bit("MX"),this);
//    QRadioButton *rMy=new QRadioButton(QString::fromLocal8Bit("MY"),this);
//    QRadioButton *rMz=new QRadioButton(QString::fromLocal8Bit("MZ"),this);
//    QRadioButton *rMxyz=new QRadioButton(QString::fromLocal8Bit("MXYZ"),this);
//    reactgroup=new QButtonGroup(this);
//    reactgroup->addButton(rNone,0);
//    reactgroup->addButton(rFx,1);
//    reactgroup->addButton(rFy,2);
//    reactgroup->addButton(rFz,3);
//    reactgroup->addButton(rFxyz,4);
//    reactgroup->addButton(rMx,5);
//    reactgroup->addButton(rMy,6);
//    reactgroup->addButton(rMz,7);
//    reactgroup->addButton(rMxyz,8);
//    QHBoxLayout *layreact1=new QHBoxLayout;
//    QHBoxLayout *layreact2=new QHBoxLayout;
//    QHBoxLayout *layreact3=new QHBoxLayout;
//    QVBoxLayout *layreact=new QVBoxLayout;
//    layreact1->addWidget(rNone);
//    layreact1->addStretch();
//    layreact1->addWidget(chkreactnum);
//    layreact1->addWidget(labelreactscale);
//    layreact1->addWidget(reactscale);
//    layreact2->addWidget(rFx);
//    layreact2->addWidget(rFy);
//    layreact2->addWidget(rFz);
//    layreact2->addWidget(rFxyz);
//    layreact3->addWidget(rMx);
//    layreact3->addWidget(rMy);
//    layreact3->addWidget(rMz);
//    layreact3->addWidget(rMxyz);
//    layreact->addLayout(layreact1);
//    layreact->addLayout(layreact2);
//    layreact->addLayout(layreact3);
//    selectreact->setLayout(layreact);


//    //selectdisp
//    QHBoxLayout *layselectdisp=new QHBoxLayout;
//    QLabel *labeldisp=new QLabel(QString::fromLocal8Bit("変形："),this);
//    QRadioButton *rbuttondon=new QRadioButton(QString::fromLocal8Bit("表示"));
//    QRadioButton *rbuttondoff=new QRadioButton(QString::fromLocal8Bit("非表示"));
//    dispgroup=new QButtonGroup(this);
//    dispgroup->addButton(rbuttondon,0);
//    dispgroup->addButton(rbuttondoff,1);
//    chkdispnum = new QCheckBox(QString::fromLocal8Bit("数値"),this);
//    QLabel *labeldispmag=new QLabel(QString::fromLocal8Bit("倍率："),this);
//    spindisp =new QSpinBox(this);
//    spindisp->setMaximum(9999);
//    spindisp->setValue(100);
//    spindisp->setMinimum(0);
//    layselectdisp->addWidget(labeldisp);
//    layselectdisp->addWidget(rbuttondon);
//    layselectdisp->addWidget(rbuttondoff);
//    layselectdisp->addWidget(chkdispnum);
//    layselectdisp->addStretch();
//    layselectdisp->addWidget(labeldispmag);
//    layselectdisp->addWidget(spindisp);
//    selectdisp->setLayout(layselectdisp);

//    //renderstyle
//    //LINE・POLYGON選択、応力図、色
//    QLabel *labelrendertype = new QLabel(QString::fromLocal8Bit("描画タイプ："),this);
//    comborendertype = new QComboBox(this);
//    QStringList strrendertype;
//    strrendertype<<QString::fromLocal8Bit("ポリゴン")<<QString::fromLocal8Bit("ライン")<<QString::fromLocal8Bit("面のみ");
//    comborendertype->addItems(strrendertype);
//    chknumlabel = new QCheckBox(QString::fromLocal8Bit("数値"),this);
//    chkdrawgraph = new QCheckBox(QString::fromLocal8Bit("応力図"),this);
//    chkdrawcontour = new QCheckBox(QString::fromLocal8Bit("コンターカラー"),this);

//    //応力図設定
//    QRadioButton *rAutoAll=new QRadioButton(QString::fromLocal8Bit("自動（全部材）"),this);
//    QRadioButton *rAutoDisplay=new QRadioButton(QString::fromLocal8Bit("自動（選択部材）"),this);
//    QRadioButton *rManual=new QRadioButton(QString::fromLocal8Bit("手動"),this);
//    selectgraphscalegroup=new QButtonGroup(this);
//    selectgraphscalegroup->addButton(rAutoAll,0);
//    selectgraphscalegroup->addButton(rAutoDisplay,1);
//    selectgraphscalegroup->addButton(rManual,2);
//    graphscale = new QLineEdit(this);
//    graphscale->setValidator(new QDoubleValidator(-99.99,99.99,2,this));
//    graphscale->setAlignment(Qt::AlignRight);
//    graphscale->setFixedWidth(100);
//    QLabel *labelgraphscale = new QLabel(QString::fromLocal8Bit("描画1mに対する値："),this);
//    QHBoxLayout * layselectgraphscale1=new QHBoxLayout;
//    QHBoxLayout * layselectgraphscale2=new QHBoxLayout;
//    layselectgraphscale1->addWidget(rAutoAll);
//    layselectgraphscale1->addWidget(rAutoDisplay);
//    layselectgraphscale2->addWidget(rManual);
//    layselectgraphscale2->addStretch();
//    layselectgraphscale2->addWidget(labelgraphscale);
//    layselectgraphscale2->addWidget(graphscale);
//    QVBoxLayout *layselectgraphscale =new QVBoxLayout;
//    layselectgraphscale->addLayout(layselectgraphscale1);
//    layselectgraphscale->addLayout(layselectgraphscale2);
//    selectgraphscalebox->setLayout(layselectgraphscale);

//    //コンターカラー設定
//    QRadioButton *rbuttonautoall =new QRadioButton(QString::fromLocal8Bit("自動（全部材）"),this);
//    QRadioButton *rbuttonautodisplay =new QRadioButton(QString::fromLocal8Bit("自動（選択部材）"),this);
//    QRadioButton *rbuttonmanual =new QRadioButton(QString::fromLocal8Bit("手動"),this);
//    //    QRadioButton *rbuttonnone =new QRadioButton(QString::fromLocal8Bit("コンターなし"),this);
//    selectdrawcontour=new QButtonGroup(this);
//    selectdrawcontour->addButton(rbuttonautoall,0);
//    selectdrawcontour->addButton(rbuttonautodisplay,1);
//    selectdrawcontour->addButton(rbuttonmanual,2);
//    //    selectLegend->addButton(rbuttonnone,3);
//    QHBoxLayout *selectcolorbuttons1 =new QHBoxLayout;
//    QHBoxLayout *selectcolorbuttons2 =new QHBoxLayout;
//    selectcolorbuttons1->addWidget(rbuttonautoall);
//    selectcolorbuttons1->addWidget(rbuttonautodisplay);
//    selectcolorbuttons2->addWidget(rbuttonmanual);
//    //    selectcolorbuttons2->addWidget(rbuttonnone);
//    QVBoxLayout *selectcolorbuttons = new QVBoxLayout;
//    selectcolorbuttons->addLayout(selectcolorbuttons1);
//    selectcolorbuttons->addLayout(selectcolorbuttons2);
//    QLabel *labelmin =new QLabel(QString::fromLocal8Bit("最小"),this);
//    linemin=new QLineEdit(this);
//    linemin->setValidator(new QDoubleValidator(-99999.99,99999.99,2,this));
//    linemin->setAlignment(Qt::AlignRight);
//    linemin->setFixedWidth(100);
//    QLabel *labelmax =new QLabel(QString::fromLocal8Bit("最大"),this);
//    linemax=new QLineEdit(this);
//    linemax->setValidator(new QDoubleValidator(-99999.99,99999.99,2,this));
//    linemax->setAlignment(Qt::AlignRight);
//    linemax->setFixedWidth(100);
//    QHBoxLayout *maxmin =new QHBoxLayout;
//    maxmin->addWidget(labelmin);
//    maxmin->addWidget(linemin);
//    maxmin->addWidget(labelmax);
//    maxmin->addWidget(linemax);
//    QGroupBox *manualbox =new QGroupBox(QString::fromLocal8Bit("手動境界値入力"),this);
//    manualbox->setLayout(maxmin);

//    QVBoxLayout *layselectcolor =new QVBoxLayout;
//    layselectcolor->addLayout(selectcolorbuttons);
//    layselectcolor->addWidget(manualbox);
//    selectcolor->setLayout(layselectcolor);

//    //描画形式全体
//    QHBoxLayout *layop1=new QHBoxLayout;
//    QHBoxLayout *layop2=new QHBoxLayout;
//    QVBoxLayout *laystyle=new QVBoxLayout;
//    //    layop1->addWidget(chknumlabel);
//    layop1->addWidget(labelrendertype);
//    layop1->addWidget(comborendertype);
//    layop2->addWidget(chkdrawgraph);
//    layop2->addWidget(chkdrawcontour);
//    layop2->addWidget(chknumlabel);//chknumlabel->setVisible(false);
//    laystyle->addLayout(layop1);
//    laystyle->addLayout(layop2);
//    laystyle->addWidget(selectgraphscalebox);
//    laystyle->addWidget(selectcolor);
//    renderstyle->setLayout(laystyle);

//    //displayscope
//    QLabel* floorLabel=new QLabel(QString::fromLocal8Bit("・階所属:"),this);
//    floorLabel->setFixedWidth(100);
//    QRadioButton* fl_noneButton=new QRadioButton(QString::fromLocal8Bit("制限なし"),this);
//    QRadioButton* fl_containButton=new QRadioButton(QString::fromLocal8Bit("全体所属"),this);
//    QRadioButton* fl_touchButton=new QRadioButton(QString::fromLocal8Bit("部分所属"),this);
//    limitedFloorGroup=new QButtonGroup(this);
//    limitedFloorGroup->addButton(fl_noneButton,0);
//    limitedFloorGroup->addButton(fl_containButton,1);
//    limitedFloorGroup->addButton(fl_touchButton,2);

//    QPushButton *selectedFloorButton=new QPushButton(QString::fromLocal8Bit("所属階"),this);
//    selectedFloorButton->setFixedSize(80,25);
//    limitedFloorLabel=new ElidedLabel(QString::fromLocal8Bit(""),this);
//    limitedFloorLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
//    limitedFloorLabel->setTextFormat(Qt::AutoText);

//    connect(selectedFloorButton, SIGNAL(clicked()), this, SLOT(showSelectFloorDialog()));

//    QLabel* frameLabel=new QLabel(QString::fromLocal8Bit("・通り所属:"),this);
//    frameLabel->setFixedWidth(100);
//    QRadioButton* fr_noneButton=new QRadioButton(QString::fromLocal8Bit("制限なし"),this);
//    QRadioButton* fr_containButton=new QRadioButton(QString::fromLocal8Bit("全体所属"),this);
//    QRadioButton* fr_touchButton=new QRadioButton(QString::fromLocal8Bit("部分所属"),this);
//    limitedFrameGroup=new QButtonGroup(this);
//    limitedFrameGroup->addButton(fr_noneButton,0);
//    limitedFrameGroup->addButton(fr_containButton,1);
//    limitedFrameGroup->addButton(fr_touchButton,2);

//    QPushButton* selectedFrameButton=new QPushButton(QString::fromLocal8Bit("所属通り"),this);
//    selectedFrameButton->setFixedSize(80,25);
//    limitedFrameLabel=new ElidedLabel(QString::fromLocal8Bit(""),this);
//    limitedFrameLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
//    limitedFrameLabel->setTextFormat(Qt::AutoText);

//    connect(selectedFrameButton, SIGNAL(clicked()), this, SLOT(showSelectFrameDialog()));

//    QLabel* sectionLabel=new QLabel(QString::fromLocal8Bit("・非表示断面:"),this);
//    sectionLabel->setFixedWidth(100);

//    QPushButton* selectedSectionButton=new QPushButton(QString::fromLocal8Bit("断面選択"),this);
//    selectedSectionButton->setFixedSize(80,25);
//    limitedSectionLabel=new ElidedLabel(QString::fromLocal8Bit(""),this);
//    limitedSectionLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
//    limitedSectionLabel->setTextFormat(Qt::AutoText);

//    connect(selectedSectionButton, SIGNAL(clicked()), this, SLOT(showSelectSectionDialog()));

//    QHBoxLayout *hlay_floor=new QHBoxLayout;
//    hlay_floor->setContentsMargins(0,0,0,0);
//    hlay_floor->addWidget(floorLabel);
//    hlay_floor->addSpacing(5);
//    hlay_floor->addWidget(fl_noneButton);
//    hlay_floor->addSpacing(5);
//    hlay_floor->addWidget(fl_containButton);
//    hlay_floor->addSpacing(5);
//    hlay_floor->addWidget(fl_touchButton);
//    hlay_floor->addStretch();
//    QHBoxLayout *hlay_sfloor=new QHBoxLayout;
//    hlay_sfloor->setContentsMargins(0,0,0,0);
//    hlay_sfloor->addSpacing(15);
//    hlay_sfloor->addWidget(selectedFloorButton);
//    hlay_sfloor->addWidget(limitedFloorLabel);

//    QHBoxLayout *hlay_frame=new QHBoxLayout;
//    hlay_frame->setContentsMargins(0,0,0,0);
//    hlay_frame->addWidget(frameLabel);
//    hlay_frame->addSpacing(5);
//    hlay_frame->addWidget(fr_noneButton);
//    hlay_frame->addSpacing(5);
//    hlay_frame->addWidget(fr_containButton);
//    hlay_frame->addSpacing(5);
//    hlay_frame->addWidget(fr_touchButton);
//    hlay_frame->addStretch();
//    QHBoxLayout *hlay_sframe=new QHBoxLayout;
//    hlay_sframe->setContentsMargins(0,0,0,0);
//    hlay_sframe->addSpacing(15);
//    hlay_sframe->addWidget(selectedFrameButton);
//    hlay_sframe->addWidget(limitedFrameLabel);

//    QHBoxLayout *hlay_section=new QHBoxLayout;
//    hlay_section->setContentsMargins(0,0,0,0);
//    hlay_section->addWidget(sectionLabel);
//    hlay_section->addStretch();
//    QHBoxLayout *hlay_ssection=new QHBoxLayout;
//    hlay_ssection->setContentsMargins(0,0,0,0);
//    hlay_ssection->addSpacing(15);
//    hlay_ssection->addWidget(selectedSectionButton);
//    hlay_ssection->addWidget(limitedSectionLabel);

//    QVBoxLayout *vlaydisp=new QVBoxLayout;
//    vlaydisp->addLayout(hlay_floor);
//    vlaydisp->addLayout(hlay_sfloor);
//    vlaydisp->addSpacing(4);
//    vlaydisp->addLayout(hlay_frame);
//    vlaydisp->addLayout(hlay_sframe);
//    vlaydisp->addSpacing(4);
//    vlaydisp->addLayout(hlay_section);
//    vlaydisp->addLayout(hlay_ssection);

//    displayscope->setLayout(vlaydisp);


//    //rendering
//    QLabel* renderLabel=new QLabel(QString::fromLocal8Bit("投影方法:"),this);
//    QRadioButton* re_orthoButton=new QRadioButton(QString::fromLocal8Bit("平行投影"),this);
//    QRadioButton* re_persButton=new QRadioButton(QString::fromLocal8Bit("透視投影"),this);
//    renderingGroup=new QButtonGroup(this);
//    renderingGroup->addButton(re_orthoButton,0);
//    renderingGroup->addButton(re_persButton,1);

//    QHBoxLayout *hlay_render=new QHBoxLayout;
//    hlay_render->setContentsMargins(0,0,0,0);
//    hlay_render->addWidget(renderLabel);
//    hlay_render->addSpacing(5);
//    hlay_render->addWidget(re_orthoButton);
//    hlay_render->addSpacing(5);
//    hlay_render->addWidget(re_persButton);
//    hlay_render->addStretch();
//    chksmooth = new QCheckBox(QString::fromLocal8Bit("アンチエイリアス"),this);
//    chkshade = new QCheckBox(QString::fromLocal8Bit("陰影"),this);
//    chkzwall = new QCheckBox(QString::fromLocal8Bit("雑壁"),this);
//    chkopening = new QCheckBox(QString::fromLocal8Bit("開口"),this);
//    QHBoxLayout *hlay_render2=new QHBoxLayout;
//    hlay_render2->addWidget(chksmooth);
//    hlay_render2->addWidget(chkshade);
//    hlay_render2->addWidget(chkzwall);
//    hlay_render2->addWidget(chkopening);

//    QVBoxLayout *vlay_camerarender=new QVBoxLayout;
//    vlay_camerarender->addLayout(hlay_render);
//    vlay_camerarender->addLayout(hlay_render2);
//    renderingsetting->setLayout(vlay_camerarender);


//    //options
//    chkdrawlegend = new QCheckBox(QString::fromLocal8Bit("凡例"),this);
//    chkfilename = new QCheckBox(QString::fromLocal8Bit("ファイル名"),this);
//    chkcoordinate = new QCheckBox(QString::fromLocal8Bit("座標系"),this);
//    chklogo = new QCheckBox(QString::fromLocal8Bit("CopyRight"),this);
//    chkdrawpoint = new QCheckBox(QString::fromLocal8Bit("節点"),this);
//    chkmemberlabel = new QCheckBox(QString::fromLocal8Bit("符号"),this);
//    chkdrawyield = new QCheckBox(QString::fromLocal8Bit("塑性化"),this);
//    QLabel *labelfontsize =new QLabel(QString::fromLocal8Bit("フォントサイズ："),this);
//    linefontsize=new QLineEdit(this);
//    linefontsize->setValidator(new QIntValidator(1,100,this));
//    linefontsize->setAlignment(Qt::AlignRight);
//    linefontsize->setFixedWidth(50);
//    linefontsize->setText(QString::fromLocal8Bit("20"));

//    QVBoxLayout *layoption=new QVBoxLayout;
//    QHBoxLayout *layoption1=new QHBoxLayout;
//    QHBoxLayout *layoption2=new QHBoxLayout;
//    //    QHBoxLayout *layoption3=new QHBoxLayout;
//    layoption1->addWidget(chkdrawlegend);
//    layoption1->addWidget(chkfilename);
//    layoption1->addWidget(chkcoordinate);
//    layoption1->addWidget(chklogo);
//    layoption2->addWidget(chkdrawpoint);
//    layoption2->addWidget(chkmemberlabel);
//    layoption2->addWidget(chkdrawyield);
//    //    layoption2->addStretch();
//    layoption2->addWidget(labelfontsize);
//    layoption2->addWidget(linefontsize);
//    layoption->addLayout(layoption1);
//    layoption->addLayout(layoption2);
//    //    layoption->addLayout(layoption3);
//    selectoption->setLayout(layoption);

//    //全体のレイアウト
//    QVBoxLayout *vlay1=new QVBoxLayout;//model
//    vlay1->setContentsMargins(5,0,0,0);
//    vlay1->addWidget(selectload);
//    vlay1->addWidget(selectforce);
//    vlay1->addWidget(selectreact);
//    vlay1->addWidget(selectdisp);
//    vlay1->addStretch();

//    QVBoxLayout *vlay2=new QVBoxLayout;//view
//    vlay2->setContentsMargins(5,0,0,0);
//    vlay2->addWidget(renderstyle);
//    vlay2->addWidget(displayscope);
//    vlay2->addWidget(renderingsetting);
//    vlay2->addWidget(selectoption);
//    vlay2->addStretch();

//    qDebug() << "setting widget";
//    tabWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
//    modelSettingWidget->setLayout(vlay1);
//    viewSettingWidget->setLayout(vlay2);
//    tabWidget->setMaximumSize(430,2000);

//    return tabWidget;
//}

OutputPersViewer *OutputPersWidget::createPersViewer()
{
    OutputPersViewer* render=new OutputPersViewer(myPersScene,this,singularGLWidget);
    render->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    qDebug() <<"persViewer";
    return render;
}


//void OutputPersWidget::clearModel()
//{
//    qDebug()<<"clearModel";
//    myPersScene->importData( QList<MemberData*>() ,QList<OpeningSet*>() );
//}

//void OutputPersWidget::importModel( const QList<MemberData*>& mdlist ,const QList<OpeningSet*>& oplist)
//{
//    myPersScene->importData(mdlist,oplist);
//    qDebug()<<"importModel";
//    myPersViewer->loadViewSettings(this->getModelSettings());
//    qDebug()<<"loadViewSettings";
//    setBoundingMinMax();
//    qDebug()<<"setBounding";
//    myPersViewer->update();
//    qDebug()<<"updateGL";
//}
//void OutputPersWidget::setBoundingMinMax(){
//    XYZ min,max;
//    myPersScene->initDisplayList();
//    bool isFirstPoint=true;
//    foreach(MemberData* md,myPersScene->getFilteredMembers()){
//        JointData* ij=md->iJoint();
//        if(isFirstPoint){
//            min.x=ij->xPos();
//            min.y=ij->yPos();
//            min.z=ij->zPos();
//            max.x=ij->xPos();
//            max.y=ij->yPos();
//            max.z=ij->zPos();
//            isFirstPoint=false;
//        }
//        if(ij->xPos()<min.x)min.x=ij->xPos();
//        if(ij->yPos()<min.y)min.y=ij->yPos();
//        if(ij->zPos()<min.z)min.z=ij->zPos();
//        if(ij->xPos()>max.x)max.x=ij->xPos();
//        if(ij->yPos()>max.y)max.y=ij->yPos();
//        if(ij->zPos()>max.z)max.z=ij->zPos();
//        JointData* jj=md->jJoint();
//        if(jj->xPos()<min.x)min.x=jj->xPos();
//        if(jj->yPos()<min.y)min.y=jj->yPos();
//        if(jj->zPos()<min.z)min.z=jj->zPos();
//        if(jj->xPos()>max.x)max.x=jj->xPos();
//        if(jj->yPos()>max.y)max.y=jj->yPos();
//        if(jj->zPos()>max.z)max.z=jj->zPos();
//    }
//    min.x-=10.0;
//    min.y-=10.0;
//    min.z-=10.0;
//    max.x+=10.0;
//    max.y+=10.0;
//    max.z+=10.0;
//    boundingMin=min;
//    boundingMax=max;
//    myPersViewer->setSceneCenter(qglviewer::Vec((max.x+min.x)/2.0,(max.y+min.y)/2.0,(max.z+min.z)/2.0));
//}

//void OutputPersWidget::slotFitBoundingBox(){
//    this->setBoundingMinMax();
//    myPersViewer->fitBoundingBox(boundingMin,boundingMax);
//    myPersViewer->update();
//}
//void OutputPersWidget::slotRenderView(){
//    bool drawContour=true;
//    bool drawLegend=true;
//    if(chkdrawlegend->isChecked()){
//        drawLegend=true;
//    }else{
//        drawLegend=false;
//    }
//    if(chkfilename->isChecked()){
//        myPersViewer->setDrawFileName(true);
//    }else{
//        myPersViewer->setDrawFileName(false);
//    }
//    //    if(drawContour==false)drawLegend=false;
//    myPersViewer->setDrawContour(drawContour);
//    myPersViewer->setDrawLegend(drawLegend);

//    qDebug()<<"drawing status set";
//    if(currentMemberList.empty()){myPersViewer->updateGL();return;}
//    qDebug()<<"import model";
//    importModel(currentMemberList,openingList);

//    handleNPstn* npstn = globalData::get()->npstn;
//    handlePstn* pstn = globalData::get()->pstn;
//    bool isNewPstn = globalData::get()->isNewPstn;

//    if(isNewPstn){
//        if(npstn->pstnReadStatus.value(handleNPstn::OFILE_MODEL)==handleNPstn::READING_STATUS::STATUS_SUCCESS
//                &&(npstn->pstnReadStatus.value(handleNPstn::OFILE_RST)==handleNPstn::READING_STATUS::STATUS_NOTEXIST
//                   ||npstn->pstnReadStatus.value(handleNPstn::OFILE_RST)==handleNPstn::READING_STATUS::STATUS_ERROR)){
//            myPersViewer->updateGL();
//            return;
//        }

//    }else{
//        if(pstn->successToReadModelFile && !pstn->successToReadSomeRstFile){myPersViewer->updateGL();return;}
//    }
//    qDebug()<<"update model";
//    QHash<QString,QList<qreal>> allSectionalForce;
//    QHash<QString,QList<qreal>> filteredSectionalForce;

//    QString loadcase1=comboload1->currentText();
//    QString loadcase2=comboload2->currentText();
//    QString loadcase3=comboload3->currentText();
//    QString loadcase4=comboload4->currentText();
//    qreal coef1=coefload1->displayText().toDouble();
//    qreal coef2=coefload2->displayText().toDouble();
//    qreal coef3=coefload3->displayText().toDouble();
//    qreal coef4=coefload4->displayText().toDouble();
//    QStringList loadcase;loadcase<<loadcase1<<loadcase2<<loadcase3<<loadcase4;
//    QList<qreal> coef;coef<<coef1<<coef2<<coef3<<coef4;
//    QList<int> loadstep;loadstep<<loadmaxstep1->text().toInt()<<loadmaxstep2->text().toInt()<<loadmaxstep3->text().toInt()<<loadmaxstep4->text().toInt();//最大最小は最終ステップの値を拾う
//    ValueMinMax minmax;
//    foreach(MemberData* md,currentMemberList){
//        QList<MemberData*> filteredMember=myPersScene->getFilteredMembers();//表示部材のみで最大最小を拾う場合
//        MemberForce mf;
//        if(nodeorfacegroup->checkedId()==0)mf=md->getCombinationNodeMemberForce(coef,loadcase,loadstep);
//        if(nodeorfacegroup->checkedId()==1)mf=md->getCombinationFaceMemberForce(coef,loadcase,loadstep);
//        if(md->dataType()==COLUMN){
//            if(selectforceC.at(0)->isChecked()){
//                allSectionalForce["N"]<<mf.N;
//                if(filteredMember.contains(md))filteredSectionalForce["N"]<<mf.N;
//            }
//            if(selectforceC.at(1)->isChecked()){
//                allSectionalForce["Qy"]<<mf.iQy;
//                allSectionalForce["Qy"]<<mf.jQy;
//                if(filteredMember.contains(md))filteredSectionalForce["Qy"]<<mf.iQy;
//                if(filteredMember.contains(md))filteredSectionalForce["Qy"]<<mf.jQy;
//            }
//            if(selectforceC.at(2)->isChecked()){
//                allSectionalForce["Qz"]<<mf.iQz;
//                allSectionalForce["Qz"]<<mf.jQz;
//                if(filteredMember.contains(md))filteredSectionalForce["Qz"]<<mf.iQz;
//                if(filteredMember.contains(md))filteredSectionalForce["Qz"]<<mf.jQz;
//            }
//            if(selectforceC.at(3)->isChecked()){
//                allSectionalForce["Mx"]<<mf.iMx;
//                allSectionalForce["Mx"]<<mf.jMx;
//                if(filteredMember.contains(md))filteredSectionalForce["Mx"]<<mf.iMx;
//                if(filteredMember.contains(md))filteredSectionalForce["Mx"]<<mf.jMx;
//            }
//            if(selectforceC.at(4)->isChecked()){
//                allSectionalForce["My"]<<mf.iMy;
//                allSectionalForce["My"]<<mf.cMy;
//                allSectionalForce["My"]<<mf.jMy;
//                if(filteredMember.contains(md)){
//                    filteredSectionalForce["My"]<<mf.iMy;
//                    filteredSectionalForce["My"]<<mf.cMy;
//                    filteredSectionalForce["My"]<<mf.jMy;
//                }
//            }
//            if(selectforceC.at(5)->isChecked()){
//                allSectionalForce["Mz"]<<mf.iMz;
//                allSectionalForce["Mz"]<<mf.cMz;
//                allSectionalForce["Mz"]<<mf.jMz;
//                if(filteredMember.contains(md)){
//                    filteredSectionalForce["Mz"]<<mf.iMz;
//                    filteredSectionalForce["Mz"]<<mf.cMz;
//                    filteredSectionalForce["Mz"]<<mf.jMz;
//                }
//            }
//        }else if(md->dataType()==DATATYPE::TPGIRDER){
//            if(selectforceG.at(0)->isChecked()){
//                allSectionalForce["N"]<<mf.N;
//                if(filteredMember.contains(md))filteredSectionalForce["N"]<<mf.N;
//            }
//            if(selectforceG.at(1)->isChecked()){
//                allSectionalForce["Qy"]<<mf.iQy;
//                allSectionalForce["Qy"]<<mf.jQy;
//                if(filteredMember.contains(md))filteredSectionalForce["Qy"]<<mf.iQy;
//                if(filteredMember.contains(md))filteredSectionalForce["Qy"]<<mf.jQy;
//            }
//            if(selectforceG.at(2)->isChecked()){
//                allSectionalForce["Qz"]<<mf.iQz;
//                allSectionalForce["Qz"]<<mf.jQz;
//                if(filteredMember.contains(md))filteredSectionalForce["Qz"]<<mf.iQz;
//                if(filteredMember.contains(md))filteredSectionalForce["Qz"]<<mf.jQz;
//            }
//            if(selectforceG.at(3)->isChecked()){
//                allSectionalForce["Mx"]<<mf.iMx;
//                allSectionalForce["Mx"]<<mf.jMx;
//                if(filteredMember.contains(md))filteredSectionalForce["Mx"]<<mf.iMx;
//                if(filteredMember.contains(md))filteredSectionalForce["Mx"]<<mf.jMx;
//            }
//            if(selectforceG.at(4)->isChecked()){
//                allSectionalForce["My"]<<mf.iMy;
//                allSectionalForce["My"]<<mf.cMy;
//                allSectionalForce["My"]<<mf.jMy;
//                if(filteredMember.contains(md)){
//                    filteredSectionalForce["My"]<<mf.iMy;
//                    filteredSectionalForce["My"]<<mf.cMy;
//                    filteredSectionalForce["My"]<<mf.jMy;
//                }
//            }
//            if(selectforceG.at(5)->isChecked()){
//                allSectionalForce["Mz"]<<mf.iMz;
//                allSectionalForce["Mz"]<<mf.cMz;
//                allSectionalForce["Mz"]<<mf.jMz;
//                if(filteredMember.contains(md)){
//                    filteredSectionalForce["Mz"]<<mf.iMz;
//                    filteredSectionalForce["Mz"]<<mf.cMz;
//                    filteredSectionalForce["Mz"]<<mf.jMz;
//                }
//            }
//        }else if(md->dataType()==BRACE){
//            if(selectforceB.at(0)->isChecked()){
//                allSectionalForce["N"]<<mf.N;
//                if(filteredMember.contains(md))filteredSectionalForce["N"]<<mf.N;
//            }
//            if(selectforceB.at(1)->isChecked()){
//                allSectionalForce["Qy"]<<mf.iQy;
//                allSectionalForce["Qy"]<<mf.jQy;
//                if(filteredMember.contains(md))filteredSectionalForce["Qy"]<<mf.iQy;
//                if(filteredMember.contains(md))filteredSectionalForce["Qy"]<<mf.jQy;
//            }
//            if(selectforceB.at(2)->isChecked()){
//                allSectionalForce["Qz"]<<mf.iQz;
//                allSectionalForce["Qz"]<<mf.jQz;
//                if(filteredMember.contains(md))filteredSectionalForce["Qz"]<<mf.iQz;
//                if(filteredMember.contains(md))filteredSectionalForce["Qz"]<<mf.jQz;
//            }
//            if(selectforceB.at(3)->isChecked()){
//                allSectionalForce["Mx"]<<mf.iMx;
//                allSectionalForce["Mx"]<<mf.jMx;
//                if(filteredMember.contains(md))filteredSectionalForce["Mx"]<<mf.iMx;
//                if(filteredMember.contains(md))filteredSectionalForce["Mx"]<<mf.jMx;
//            }
//            if(selectforceB.at(4)->isChecked()){
//                allSectionalForce["My"]<<mf.iMy;
//                allSectionalForce["My"]<<mf.cMy;
//                allSectionalForce["My"]<<mf.jMy;
//                if(filteredMember.contains(md)){
//                    filteredSectionalForce["My"]<<mf.iMy;
//                    filteredSectionalForce["My"]<<mf.cMy;
//                    filteredSectionalForce["My"]<<mf.jMy;
//                }
//            }
//            if(selectforceB.at(5)->isChecked()){
//                allSectionalForce["Mz"]<<mf.iMz;
//                allSectionalForce["Mz"]<<mf.cMz;
//                allSectionalForce["Mz"]<<mf.jMz;
//                if(filteredMember.contains(md)){
//                    filteredSectionalForce["Mz"]<<mf.iMz;
//                    filteredSectionalForce["Mz"]<<mf.cMz;
//                    filteredSectionalForce["Mz"]<<mf.jMz;
//                }
//            }
//        }else if(md->dataType()==WALL){
//            if(selectforceW.at(0)->isChecked()){
//                allSectionalForce["N"]<<mf.N;
//                if(filteredMember.contains(md))filteredSectionalForce["N"]<<mf.N;
//            }
//            if(selectforceW.at(1)->isChecked()){
//                allSectionalForce["Qy"]<<mf.iQy;
//                if(filteredMember.contains(md))filteredSectionalForce["Qy"]<<mf.iQy;
//            }
//            if(selectforceW.at(2)->isChecked()){
//                allSectionalForce["Mz"]<<mf.iMz;
//                allSectionalForce["Mz"]<<mf.jMz;
//                if(filteredMember.contains(md))filteredSectionalForce["Mz"]<<mf.iMz;
//                if(filteredMember.contains(md))filteredSectionalForce["Mz"]<<mf.jMz;
//            }

//        }
//    }
//    if(!filteredSectionalForce["N"].isEmpty()){qSort(filteredSectionalForce["N"].begin(),filteredSectionalForce["N"].end());}
//    else{filteredSectionalForce["N"]<<0.0;}
//    minmax.DisplayMin["N"]=filteredSectionalForce["N"].first();
//    minmax.DisplayMax["N"]=filteredSectionalForce["N"].last();
//    if(!filteredSectionalForce["Qy"].isEmpty()){qSort(filteredSectionalForce["Qy"].begin(),filteredSectionalForce["Qy"].end());}
//    else{filteredSectionalForce["Qy"]<<0.0;}
//    minmax.DisplayMin["Qy"]=filteredSectionalForce["Qy"].first();
//    minmax.DisplayMax["Qy"]=filteredSectionalForce["Qy"].last();
//    if(!filteredSectionalForce["Qz"].isEmpty()){qSort(filteredSectionalForce["Qz"].begin(),filteredSectionalForce["Qz"].end());}
//    else{filteredSectionalForce["Qz"]<<0.0;}
//    minmax.DisplayMin["Qz"]=filteredSectionalForce["Qz"].first();
//    minmax.DisplayMax["Qz"]=filteredSectionalForce["Qz"].last();
//    if(!filteredSectionalForce["Mx"].isEmpty()){qSort(filteredSectionalForce["Mx"].begin(),filteredSectionalForce["Mx"].end());}
//    else{filteredSectionalForce["Mx"]<<0.0;}
//    minmax.DisplayMin["Mx"]=filteredSectionalForce["Mx"].first();
//    minmax.DisplayMax["Mx"]=filteredSectionalForce["Mx"].last();
//    if(!filteredSectionalForce["My"].isEmpty()){qSort(filteredSectionalForce["My"].begin(),filteredSectionalForce["My"].end());}
//    else{filteredSectionalForce["My"]<<0.0;}
//    minmax.DisplayMin["My"]=filteredSectionalForce["My"].first();
//    minmax.DisplayMax["My"]=filteredSectionalForce["My"].last();
//    if(!filteredSectionalForce["Mz"].isEmpty()){qSort(filteredSectionalForce["Mz"].begin(),filteredSectionalForce["Mz"].end());}
//    else{filteredSectionalForce["Mz"]<<0.0;}
//    minmax.DisplayMin["Mz"]=filteredSectionalForce["Mz"].first();
//    minmax.DisplayMax["Mz"]=filteredSectionalForce["Mz"].last();

//    if(!allSectionalForce["N"].isEmpty()){qSort(allSectionalForce["N"].begin(),allSectionalForce["N"].end());}
//    else{allSectionalForce["N"]<<0.0;}
//    minmax.AllMin["N"]=allSectionalForce["N"].first();
//    minmax.AllMax["N"]=allSectionalForce["N"].last();
//    if(!allSectionalForce["Qy"].isEmpty()){qSort(allSectionalForce["Qy"].begin(),allSectionalForce["Qy"].end());}
//    else{allSectionalForce["Qy"]<<0.0;}
//    minmax.AllMin["Qy"]=allSectionalForce["Qy"].first();
//    minmax.AllMax["Qy"]=allSectionalForce["Qy"].last();
//    if(!allSectionalForce["Qz"].isEmpty()){qSort(allSectionalForce["Qz"].begin(),allSectionalForce["Qz"].end());}
//    else{allSectionalForce["Qz"]<<0.0;}
//    minmax.AllMin["Qz"]=allSectionalForce["Qz"].first();
//    minmax.AllMax["Qz"]=allSectionalForce["Qz"].last();
//    if(!allSectionalForce["Mx"].isEmpty()){qSort(allSectionalForce["Mx"].begin(),allSectionalForce["Mx"].end());}
//    else{allSectionalForce["Mx"]<<0.0;}
//    minmax.AllMin["Mx"]=allSectionalForce["Mx"].first();
//    minmax.AllMax["Mx"]=allSectionalForce["Mx"].last();
//    if(!allSectionalForce["My"].isEmpty()){qSort(allSectionalForce["My"].begin(),allSectionalForce["My"].end());}
//    else{allSectionalForce["My"]<<0.0;}
//    minmax.AllMin["My"]=allSectionalForce["My"].first();
//    minmax.AllMax["My"]=allSectionalForce["My"].last();
//    if(!allSectionalForce["Mz"].isEmpty()){qSort(allSectionalForce["Mz"].begin(),allSectionalForce["Mz"].end());}
//    else{allSectionalForce["Mz"]<<0.0;}
//    minmax.AllMin["Mz"]=allSectionalForce["Mz"].first();
//    minmax.AllMax["Mz"]=allSectionalForce["Mz"].last();

//    QList<qreal> allmin,allmax,filteredmin,filteredmax;

//    allmin<<minmax.AllMin["N"]<<minmax.AllMin["Qy"]<<minmax.AllMin["Qz"]<<minmax.AllMin["Mx"]<<minmax.AllMin["My"]<<minmax.AllMin["Mz"];
//    qSort(allmin.begin(),allmin.end());
//    minmax.AllMin["All"]=allmin.first();
//    allmax<<minmax.AllMax["N"]<<minmax.AllMax["Qy"]<<minmax.AllMax["Qz"]<<minmax.AllMax["Mx"]<<minmax.AllMax["My"]<<minmax.AllMax["Mz"];
//    qSort(allmax.begin(),allmax.end());
//    minmax.AllMax["All"]=allmax.last();

//    filteredmin<<minmax.DisplayMin["N"]<<minmax.DisplayMin["Qy"]<<minmax.DisplayMin["Qz"]<<minmax.DisplayMin["Mx"]<<minmax.DisplayMin["My"]<<minmax.DisplayMin["Mz"];
//    qSort(filteredmin.begin(),filteredmin.end());
//    minmax.DisplayMin["All"]=filteredmin.first();
//    filteredmax<<minmax.DisplayMax["N"]<<minmax.DisplayMax["Qy"]<<minmax.DisplayMax["Qz"]<<minmax.DisplayMax["Mx"]<<minmax.DisplayMax["My"]<<minmax.DisplayMax["Mz"];
//    qSort(filteredmax.begin(),filteredmax.end());
//    minmax.DisplayMax["All"]=filteredmax.last();

//    if(qAbs(minmax.AllMax["All"]-minmax.AllMin["All"])<0.00001){
//        minmax.AllMax["All"]=minmax.AllMax["All"]+1000.0;
//        minmax.AllMin["All"]=minmax.AllMin["All"]-1000.0;
//    }
//    if(qAbs(minmax.DisplayMax["All"]-minmax.DisplayMin["All"])<0.00001){
//        minmax.DisplayMax["All"]=minmax.DisplayMax["All"]+1000.0;
//        minmax.DisplayMin["All"]=minmax.DisplayMin["All"]-1000.0;
//    }

//    myPersViewer->setValueMinMax(minmax);//qDebug()<<minmax.AllMax["All"]<<minmax.AllMin["All"]<<minmax.DisplayMax["All"]<<minmax.DisplayMin["All"];
//    if(!chkdrawcontour->isChecked()){
//        drawContour=false;
//        myPersViewer->setDrawContour(false);
//    }else{
//        if(selectdrawcontour->checkedId()==0){//0 auto（全部材）,1 auto（表示部材のみ）,2 manual;
//            drawContour=true;
//            myPersViewer->setColorContourMinMax(minmax.AllMin["All"],minmax.AllMax["All"]);
//            qDebug()<<"all minmax="<<minmax.AllMin["All"]<<minmax.AllMax["All"];
//        }else if(selectdrawcontour->checkedId()==1){
//            drawContour=true;
//            myPersViewer->setColorContourMinMax(minmax.DisplayMin["All"],minmax.DisplayMax["All"]);
//            qDebug()<<"display minmax="<<minmax.DisplayMin["All"]<<minmax.DisplayMax["All"];
//        }else if(selectdrawcontour->checkedId()==2){
//            drawContour=true;
//            myPersViewer->setColorContourMinMax(linemin->text().toDouble(),linemax->text().toDouble());
//        }
//    }
//    myPersViewer->update();
//    glFlush();
//    qDebug()<<"Slotrenderview";

//}
//void OutputPersWidget::slotGetCameraValues()//persviewerからOutputPersWidgetのメンバ更新
//{
//    qglviewer::Vec p_vec=myPersViewer->camera()->position();
//    qglviewer::Vec u_vec=myPersViewer->camera()->upVector();
//    qglviewer::Vec d_vec=myPersViewer->camera()->viewDirection();
//    setCameraValues(XYZ(p_vec.x,p_vec.y,p_vec.z),XYZ(u_vec.x,u_vec.y,u_vec.z),XYZ(d_vec.x,d_vec.y,d_vec.z));
//}
//void OutputPersWidget::slotSetCameraValues()//OutputPersWidgetのメンバからpersviewer更新
//{
//    Analysis3DViewSettings modelsetting=getModelSettings();
////    CameraValues cam;
////    qglviewer::Vec p_vec=qglviewer::Vec(cam.cameraPos.x,cam.cameraPos.y,cam.cameraPos.z);
////    qglviewer::Vec u_vec=qglviewer::Vec(cam.cameraUpVec.x,cam.cameraUpVec.y,cam.cameraUpVec.z);
////    qglviewer::Vec d_vec=qglviewer::Vec(cam.cameraDir.x,cam.cameraDir.y,cam.cameraDir.z);
////    myPersViewer->camera()->setPosition(p_vec);
////    myPersViewer->camera()->setUpVector(u_vec);
////    myPersViewer->camera()->setViewDirection(d_vec);
//}
//void OutputPersWidget::slotSetCameraFront()
//{
//    qglviewer::Vec u_vec=qglviewer::Vec(0.0,0.0,1.0);
//    qglviewer::Vec d_vec=qglviewer::Vec(0.0,1.0,0.0);
//    myPersViewer->camera()->setUpVector(u_vec);
//    myPersViewer->camera()->setViewDirection(d_vec);
//    myPersViewer->fitBoundingBox(boundingMin,boundingMax);
//    myPersViewer->update();
//}
//void OutputPersWidget::slotSetCameraRight()
//{
//    qglviewer::Vec u_vec=qglviewer::Vec(0.0,0.0,1.0);
//    qglviewer::Vec d_vec=qglviewer::Vec(-1.0,0.0,0.0);
//    myPersViewer->camera()->setUpVector(u_vec);
//    myPersViewer->camera()->setViewDirection(d_vec);
//    myPersViewer->fitBoundingBox(boundingMin,boundingMax);
//    myPersViewer->update();
//}
//void OutputPersWidget::slotSetCameraTop()
//{
//    qglviewer::Vec u_vec=qglviewer::Vec(0.0,1.0,0.0);
//    qglviewer::Vec d_vec=qglviewer::Vec(0.0,0.0,-1.0);
//    myPersViewer->camera()->setUpVector(u_vec);
//    myPersViewer->camera()->setViewDirection(d_vec);
//    myPersViewer->fitBoundingBox(boundingMin,boundingMax);
//    myPersViewer->update();
//}
//void OutputPersWidget::slotSetCameraIsometric()
//{
//    qglviewer::Vec u_vec=qglviewer::Vec(-1.0,1.0,1.0);
//    qglviewer::Vec d_vec=qglviewer::Vec(-1.0,1.0,-1.0);
//    myPersViewer->camera()->setUpVector(u_vec);
//    myPersViewer->camera()->setViewDirection(d_vec);
//    myPersViewer->fitBoundingBox(boundingMin,boundingMax);
//    myPersViewer->update();
//}
//void OutputPersWidget::slotSetCameraOpposite()
//{
//    qglviewer::Vec p_vec=myPersViewer->camera()->position();
//    qglviewer::Vec u_vec=myPersViewer->camera()->upVector();
//    qglviewer::Vec d_vec=myPersViewer->camera()->viewDirection();
//    qglviewer::Vec c_vec=myPersViewer->camera()->sceneCenter();
//    p_vec=qglviewer::Vec(p_vec.x+2*(c_vec.x-p_vec.x),p_vec.y+2*(c_vec.y-p_vec.y),p_vec.z);
//    u_vec=qglviewer::Vec(-u_vec.x,-u_vec.y,u_vec.z);
//    d_vec=qglviewer::Vec(-d_vec.x,-d_vec.y,d_vec.z);
//    myPersViewer->camera()->setPosition(p_vec);
//    myPersViewer->camera()->setUpVector(u_vec);
//    myPersViewer->camera()->setViewDirection(d_vec);
//    myPersViewer->fitBoundingBox(boundingMin,boundingMax);
//    myPersViewer->update();
//}

//void OutputPersWidget::slotCreatePng()
//{
//    QString filename = QFileDialog::getSaveFileName( this, QString(), QString(),QString::fromLocal8Bit("Images(*.png)") );
//    if(filename.isEmpty())return;
//    QEventLoop loop;
//    QTimer::singleShot(500,&loop,SLOT(quit()));
//    loop.exec();
//    myPersViewer->update();//フレームバッファがうまく更新されないため便宜的にwaitを挟んで再描画してから保存。要原因究明
//    QTimer::singleShot(1000,&loop,SLOT(quit()));
//    loop.exec();

//    qDebug()<<"dpi=" << myPersViewer->logicalDpiX();
//    QImage image=myPersViewer->grabFrameBuffer(false);
//    //    int dpm = 300/0.0254;
//    //    image.setDotsPerMeterX(dpm);
//    //    image.setDotsPerMeterY(dpm);
//    image.save(filename);

//    return;


//}
//void OutputPersWidget::slotCreateHQPng()
//{
//    QString filename = QFileDialog::getSaveFileName( this, QString(), QString(),QString::fromLocal8Bit("Images(*.png)") );
//    if(filename.isEmpty())return;
//    QEventLoop loop;
//    myPersViewer->resize(myPersViewer->width()*4,myPersViewer->height()*4);
//    QTimer::singleShot(500,&loop,SLOT(quit()));
//    loop.exec();
//    myPersViewer->update();//フレームバッファがうまく更新されないため便宜的にwaitを挟んで再描画してから保存。要原因究明
//    QTimer::singleShot(1000,&loop,SLOT(quit()));
//    loop.exec();

//    qDebug()<<"dpi=" << myPersViewer->logicalDpiX();
//    QImage image=myPersViewer->grabFrameBuffer(false);
//    //    int dpm = 300/0.0254;
//    //    image.setDotsPerMeterX(dpm);
//    //    image.setDotsPerMeterY(dpm);
//    image.save(filename);
//    myPersViewer->resize(myPersViewer->width()/4,myPersViewer->height()/4);
//    myPersViewer->update();
//    return;


//}


//void OutputPersWidget::showSelectFloorDialog()
//{
//    QStringList limitList=limitedFloorLabel->text().split(",");
//    QStringList currentfl;
//    for(int i=0;i<currentFloors.count();i++){currentfl<<currentFloors.at(i).name;}
//    NameSelectDialog dialog(currentfl,limitList,this);
//    dialog.setWindowTitle(QString(QString::fromLocal8Bit("階の選択")));
//    if ( dialog.exec() ) {
//        limitList = dialog.selectedNames();
//        QString str= ( limitList.isEmpty() ) ? QString("") : limitList.join(",") ;
//        limitedFloorLabel->setText(str);
//    }
//}

//void OutputPersWidget::showSelectFrameDialog()
//{
//    QStringList limitList=limitedFrameLabel->text().split(",");
//    QStringList currentfr;
//    for(int i=0;i<currentFrames.count();i++){currentfr<<currentFrames.at(i).name;}
//    NameSelectDialog dialog(currentfr,limitList,this);
//    dialog.setWindowTitle(QString(QString::fromLocal8Bit("通りの選択")));
//    if ( dialog.exec() ) {
//        limitList = dialog.selectedNames();
//        QString str= ( limitList.isEmpty() ) ? QString("") : limitList.join(",") ;
//        limitedFrameLabel->setText(str);
//    }
//}

//void OutputPersWidget::showSelectSectionDialog()
//{
//    QStringList limitList=limitedSectionLabel->text().split(",");
//    QStringList currentsec;
//    for(int i=0;i<currentSectionList.count();i++){currentsec<<currentSectionList.at(i)->sectionName;}
//    NameSelectDialog dialog(currentsec,limitList,this);
//    dialog.setWindowTitle(QString(QString::fromLocal8Bit("断面の選択")));
//    if ( dialog.exec() ) {
//        limitList = dialog.selectedNames();
//        QString str= ( limitList.isEmpty() ) ? QString("") : limitList.join(",") ;
//        limitedSectionLabel->setText(str);
//    }
//}
//void OutputPersWidget::slotShowCameraDialog()
//{
//    camdialog->show();
//}
//void OutputPersWidget::slotSetCameraValueFromDialog(){
//    //OutputPersWidgetのcameravalueを更新
//    setCameraValues(camdialog->getCameraViewPointVector(),camdialog->getCameraUpwardVector(),camdialog->getCameraFocusVector());
//    //persviewerのcameravalueを更新
//    slotSetCameraValues();
//    slotRenderView();
//}
//void OutputPersWidget::slotGetCameraValueToDialog(){

//    slotGetCameraValues();
//    camdialog->setCameraViewPointVector(cameraViewPointVector);
//    camdialog->setCameraUpwardVector(cameraUpwardVector);
//    camdialog->setCameraFocusVector(cameraFocusVector);
//}

//Analysis3DViewSettings OutputPersWidget::getModelSettings() const
//{

//    QHash<QString,QList<bool> >selectforce;
//    selectforce["C"]<<selectforceC.at(0)->isChecked()<<selectforceC.at(1)->isChecked()<<selectforceC.at(2)->isChecked()
//                   <<selectforceC.at(3)->isChecked()<<selectforceC.at(4)->isChecked()<<selectforceC.at(5)->isChecked();
//    selectforce["G"]<<selectforceG.at(0)->isChecked()<<selectforceG.at(1)->isChecked()<<selectforceG.at(2)->isChecked()
//                   <<selectforceG.at(3)->isChecked()<<selectforceG.at(4)->isChecked()<<selectforceG.at(5)->isChecked();
//    selectforce["B"]<<selectforceB.at(0)->isChecked()<<selectforceB.at(1)->isChecked()<<selectforceB.at(2)->isChecked()
//                   <<selectforceB.at(3)->isChecked()<<selectforceB.at(4)->isChecked()<<selectforceB.at(5)->isChecked();
//    selectforce["W"]<<selectforceW.at(0)->isChecked()<<selectforceW.at(1)->isChecked()<<selectforceW.at(2)->isChecked();
//    QString title,filename;


//    handleNPstn* npstn = globalData::get()->npstn;
//    handlePstn* pstn = globalData::get()->pstn;
//    bool isNewPstn = globalData::get()->isNewPstn;

//    if(isNewPstn){
//        if(&npstn==NULL){
//            title=QString();
//            filename=QString();
//        }else{
//            title=npstn->model.title;
//            filename=npstn->getFileName();
//        }
//    }else{
//        if(&pstn==NULL){
//            title=QString();
//            filename=QString();
//        }else{
//            title=pstn->model.title;
//            filename=pstn->getFileName();
//        }
//    }
//    QList<qreal> coefload;
//    coefload<<coefload1->displayText().toDouble()<<
//              coefload2->displayText().toDouble()<<
//              coefload3->displayText().toDouble()<<
//              coefload4->displayText().toDouble();
//    QStringList loadcase;
//    loadcase<<comboload1->currentText()<<
//              comboload2->currentText()<<
//              comboload3->currentText()<<
//              comboload4->currentText();
//    QList<int> loadstep,loadmaxstep;
//    loadstep<<loadstep1->value()<<loadstep2->value()<<loadstep3->value()<<loadstep4->value();
//    loadmaxstep<<loadmaxstep1->text().toInt()<<loadmaxstep2->text().toInt()<<loadmaxstep3->text().toInt()<<loadmaxstep4->text().toInt();

//    return Analysis3DViewSettings(
//                title,
//                filename,
//                QColor(Qt::white),//background color
//                1,//line weight
//                comborendertype->currentIndex(),
//                spindisp->value(),
//                limitedFloorGroup->checkedId(),
//                limitedFloorLabel->text().split(","),
//                limitedFrameGroup->checkedId(),
//                limitedFrameLabel->text().split(","),
//                limitedSectionLabel->text().split(","),
//                cameraViewPointVector,
//                cameraUpwardVector,
//                cameraFocusVector,
//                selectdrawcontour->checkedId(),
//                linemin->displayText().toDouble(),
//                linemax->displayText().toDouble(),
//                selectforce,
//                nodeorfacegroup->checkedId(),
//                valuecalcmethodgroup->checkedId(),
//                dispgroup->checkedId(),
//                loadcase,
//                coefload,
//                loadstep,
//                loadmaxstep,
//                chkreactnum->isChecked(),
//                reactgroup->checkedId(),
//                reactscale->value(),
//                chknumlabel->isChecked(),
//                chkdrawgraph->isChecked(),
//                chkdrawcontour->isChecked(),
//                selectgraphscalegroup->checkedId(),
//                graphscale->displayText().toDouble(),
//                chkdrawlegend->isChecked(),
//                chkfilename->isChecked(),
//                linefontsize->displayText().toInt(),
//                chkmemberlabel->isChecked(),
//                chkcoordinate->isChecked(),
//                chklogo->isChecked(),
//                chkdrawpoint->isChecked(),
//                chkzwall->isChecked(),
//                chkopening->isChecked(),
//                chkdrawyield->isChecked(),
//                chksmooth->isChecked(),
//                chkshade->isChecked(),
//                renderingGroup->checkedId(),
//                chkdispnum->isChecked()
//                );
//}
//void OutputPersWidget::setModelSettings( const Analysis3DViewSettings & values ){
//    this->setWindowTitle(QString::fromLocal8Bit("3DVisual  ")+version+QString::fromLocal8Bit(" : ")+values.title);
//    comborendertype->setCurrentIndex(values.drawStyle);
//    spindisp->setValue(values.drawRatioXYZ);
//    selectdrawcontour->button(values.selectDrawContour)->setChecked(true);
//    linemin->setText(QString::number(values.manualLegendMin));
//    linemax->setText(QString::number(values.manualLegendMax));
//    for(int i=0;i<6;i++){
//        selectforceC.at(i)->setChecked(values.selectForce["C"].at(i));
//        selectforceG.at(i)->setChecked(values.selectForce["G"].at(i));
//        selectforceB.at(i)->setChecked(values.selectForce["B"].at(i));
//    }
//    for(int i=0;i<3;i++){
//        selectforceW.at(i)->setChecked(values.selectForce["W"].at(i));
//    }
//    nodeorfacegroup->button(values.nodeOrFace)->setChecked(true);
//    valuecalcmethodgroup->button(values.valueCalcMethod)->setChecked(true);
//    dispgroup->button(values.selectDisp)->setChecked(true);
//    comboload1->setCurrentIndex(comboload1->findText(values.loadCase.at(0)));
//    comboload2->setCurrentIndex(comboload2->findText(values.loadCase.at(1)));
//    comboload3->setCurrentIndex(comboload3->findText(values.loadCase.at(2)));
//    comboload4->setCurrentIndex(comboload4->findText(values.loadCase.at(3)));
//    coefload1->setText(QString::number(values.coefload.at(0),'f',2));
//    coefload2->setText(QString::number(values.coefload.at(1),'f',2));
//    coefload3->setText(QString::number(values.coefload.at(2),'f',2));
//    coefload4->setText(QString::number(values.coefload.at(3),'f',2));
//    loadstep1->setValue(values.loadStep.at(0));
//    loadstep2->setValue(values.loadStep.at(1));
//    loadstep3->setValue(values.loadStep.at(2));
//    loadstep4->setValue(values.loadStep.at(3));
//    //    loadmaxstep1->setText(QString::number(values.loadMaxStep.at(0)));
//    //    loadmaxstep2->setText(QString::number(values.loadMaxStep.at(1)));
//    //    loadmaxstep3->setText(QString::number(values.loadMaxStep.at(2)));
//    //    loadmaxstep4->setText(QString::number(values.loadMaxStep.at(3)));
//    chkreactnum->setChecked(values.drawReactNumber);
//    reactscale->setValue(values.reactScale);
//    reactgroup->button(values.selectReact)->setChecked(true);
//    limitedFloorGroup->button(values.limitedFloorID)->setChecked(true);
//    limitedFloorLabel->setText(values.limitedFloors.join(","));
//    limitedFrameGroup->button(values.limitedFrameID)->setChecked(true);
//    limitedFrameLabel->setText(values.limitedFrames.join(","));
//    limitedSectionLabel->setText(values.limitedSection.join(","));
//    chknumlabel->setChecked(values.drawNumberLabel);
//    chkdrawgraph->setChecked(values.drawGraph);
//    chkdrawcontour->setChecked(values.drawContour);
//    selectgraphscalegroup->button(values.selectGraphScale)->setChecked(true);
//    graphscale->setText(QString::number(values.graphScale,'f',2));
//    chkdrawlegend->setChecked(values.drawLegend);
//    chkfilename->setChecked(values.drawFileName);
//    linefontsize->setText(QString::number(values.fontSize));
//    chkmemberlabel->setChecked(values.drawMemberLabel);
//    chkcoordinate->setChecked(values.drawCoordinate);
//    chklogo->setChecked(values.drawLogo);
//    chkdrawpoint->setChecked(values.drawPoint);
//    chkzwall->setChecked(values.drawZWall);
//    chkopening->setChecked(values.drawOpening);
//    chkdrawyield->setChecked(values.drawYield);
//    chksmooth->setChecked(values.smoothing);
//    chkshade->setChecked(values.shade);
//    renderingGroup->button(values.renderType)->setChecked(true);
//    chkdispnum->setChecked(values.drawDispNumber);
//    qDebug()<<"setvalue done";
//}

//void OutputPersWidget::setCameraValues(XYZ p_vec,XYZ u_vec,XYZ d_vec){
//    cameraViewPointVector=p_vec;
//    cameraUpwardVector=u_vec;
//    cameraFocusVector=d_vec;
//}

//void OutputPersWidget::saveSettingFile(){
//    //readdSettingFileの書式で書き出す
//    QString filename = QFileDialog::getSaveFileName( this, QString(), QString(),QString::fromLocal8Bit("Text(*.txt)") );
//    if(filename.isEmpty())return;
//    QFile file(filename);

//    if ( ! file.open(QIODevice::WriteOnly) ){
//        QMessageBox::warning( this, "WARNING","Failed to open" );
//        return;
//    }

//    QTextStream out(&file);
//    QString line;
//    Analysis3DViewSettings setting=getModelSettings();
//    line="SET_RSTCASE1,"+QString::number(setting.coefload.at(0),'f',2)+","+setting.loadCase.at(0)+","+QString::number(setting.loadStep.at(0));
//    out<<line<<endl;
//    line="SET_RSTCASE2,"+QString::number(setting.coefload.at(1),'f',2)+","+setting.loadCase.at(1)+","+QString::number(setting.loadStep.at(1));
//    out<<line<<endl;
//    line="SET_RSTCASE3,"+QString::number(setting.coefload.at(2),'f',2)+","+setting.loadCase.at(2)+","+QString::number(setting.loadStep.at(2));
//    out<<line<<endl;
//    line="SET_RSTCASE4,"+QString::number(setting.coefload.at(3),'f',2)+","+setting.loadCase.at(3)+","+QString::number(setting.loadStep.at(3));
//    out<<line<<endl;
//    QString forceC,forceG,forceB,forceW;
//    for(int i=0;i<6;i++){
//        setting.selectForce["C"].at(i)==true ? forceC+="1," : forceC+="0,";
//        setting.selectForce["G"].at(i)==true ? forceG+="1," : forceG+="0,";
//        setting.selectForce["B"].at(i)==true ? forceB+="1," : forceB+="0,";
//    }
//    for(int i=0;i<3;i++){
//        setting.selectForce["W"].at(i)==true ? forceW+="1," : forceW+="0,";
//    }
//    forceC.chop(1);
//    forceG.chop(1);
//    forceB.chop(1);
//    line="SET_SELECT_FORCE_COLUMN,"+forceC;
//    out<<line<<endl;
//    line="SET_SELECT_FORCE_DATATYPE::TPGIRDER,"+forceG;
//    out<<line<<endl;
//    line="SET_SELECT_FORCE_BRACE,"+forceB;
//    out<<line<<endl;
//    line="SET_SELECT_FORCE_WALL,"+forceW;
//    out<<line<<endl;
//    line="SET_SELECT_FORCETYPE,"+QString::number(setting.nodeOrFace);
//    out<<line<<endl;
//    line="SET_SELECT_MULTIFORCE,"+QString::number(setting.valueCalcMethod);
//    out<<line<<endl;
//    line="SET_SELECT_REACTION,"+QString::number(setting.selectReact)+","+QString::number(setting.drawReactNumber==true?1:0)+","+QString::number(setting.reactScale,'f',2);
//    out<<line<<endl;
//    line="SET_SELECT_DISP,"+QString::number(setting.selectDisp)+","+QString::number((int)setting.drawRatioXYZ);
//    out<<line<<endl;
//    line="SET_RENDERTYPE,"+QString::number(setting.drawStyle);
//    out<<line<<endl;
//    line="SET_RENDERGRAPH,"+QString::number(setting.drawGraph==true?1:0);
//    out<<line<<endl;
//    line="SET_RENDERCOLOR,"+QString::number(setting.drawContour==true?1:0);
//    out<<line<<endl;
//    line="SET_RENDERNUMBER,"+QString::number(setting.drawNumberLabel==true?1:0);
//    out<<line<<endl;
//    line="SET_GRAPHSCALE,"+QString::number(setting.selectGraphScale)+","+QString::number(setting.graphScale,'f',2);
//    out<<line<<endl;
//    line="SET_COLORSCALE,"+QString::number(setting.selectDrawContour)+","+QString::number(setting.manualLegendMin,'f',2)+","+QString::number(setting.manualLegendMax,'f',2);
//    out<<line<<endl;

//    QStringList strlist;
//    line="SET_DISPLAYSCOPE_FLOOR,"+QString::number(setting.limitedFloorID);
//    strlist=setting.limitedFloors;
//    for(int i=0;i<strlist.count();i++){
//        line+=",";
//        line+=strlist.at(i);
//    }
//    out<<line<<endl;
//    line="SET_DISPLAYSCOPE_FRAME,"+QString::number(setting.limitedFrameID);
//    strlist=setting.limitedFrames;
//    for(int i=0;i<strlist.count();i++){
//        line+=",";
//        line+=strlist.at(i);
//    }
//    out<<line<<endl;
//    line="SET_DISPLAYSCOPE_SECTION";
//    strlist=setting.limitedSection;
//    for(int i=0;i<strlist.count();i++){
//        line+=",";
//        line+=strlist.at(i);
//    }
//    out<<line<<endl;
//    line="SET_RENDER_RENDERTYPE,"+QString::number(setting.renderType);
//    out<<line<<endl;
//    line="SET_RENDER_SMOOTH,"+QString::number(setting.smoothing==true?1:0);
//    out<<line<<endl;
//    line="SET_RENDER_SHADE,"+QString::number(setting.shade==true?1:0);
//    out<<line<<endl;
//    line="SET_DRAW_DISPNUM,"+QString::number(setting.drawDispNumber==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_LEGEND,"+QString::number(setting.drawLegend==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_FILENAME,"+QString::number(setting.drawFileName==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_ZWALL,"+QString::number(setting.drawZWall==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_ZWALL,"+QString::number(setting.drawZWall==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_OPENING,"+QString::number(setting.drawOpening==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_COORDINATE,"+QString::number(setting.drawCoordinate==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_LOGO,"+QString::number(setting.drawLogo==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_NODE,"+QString::number(setting.drawPoint==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_LABEL,"+QString::number(setting.drawMemberLabel==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_YIELD,"+QString::number(setting.drawYield==true?1:0);
//    out<<line<<endl;
//    line="SET_OPTION_FONTSIZE,"+QString::number(setting.fontSize);
//    out<<line<<endl;
//    slotGetCameraValues();
//    line="SET_CAMERA_VIEWPOINTVECTOR,"+QString::number(cameraViewPointVector.x,'f',2)+","+QString::number(cameraViewPointVector.y,'f',2)+","+QString::number(cameraViewPointVector.z,'f',2);
//    out<<line<<endl;
//    line="SET_CAMERA_UPWARDVECTOR,"+QString::number(cameraUpwardVector.x,'f',2)+","+QString::number(cameraUpwardVector.y,'f',2)+","+QString::number(cameraUpwardVector.z,'f',2);
//    out<<line<<endl;
//    line="SET_CAMERA_FOCUSVECTOR,"+QString::number(cameraFocusVector.x,'f',2)+","+QString::number(cameraFocusVector.y,'f',2)+","+QString::number(cameraFocusVector.z,'f',2);
//    out<<line<<endl;
//    //    settingのcameraPosとか更新されてない/使ってない
//    //    line="SET_CAMERA_VIEWPOINTVECTOR,"+QString::number(setting.cameraPos.x,'f',2)+","+QString::number(setting.cameraPos.y,'f',2)+","+QString::number(setting.cameraPos.z,'f',2);
//    //    out<<line<<endl;
//    //    line="SET_CAMERA_UPWARDVECTOR,"+QString::number(setting.cameraUpVec.x,'f',2)+","+QString::number(setting.cameraUpVec.y,'f',2)+","+QString::number(setting.cameraUpVec.z,'f',2);
//    //    out<<line<<endl;
//    //    line="SET_CAMERA_FOCUSVECTOR,"+QString::number(setting.cameraDir.x,'f',2)+","+QString::number(setting.cameraDir.y,'f',2)+","+QString::number(setting.cameraDir.z,'f',2);
//    //    out<<line<<endl;
//    line="SET_CAMERA";
//    out<<line<<endl;
//    line="RENDER";
//    out<<line<<endl;
//    line="END";
//    out<<line<<endl;
//    line=QString::fromLocal8Bit("*****************************************************************************");
//    out<<line<<endl;
//    line=QString::fromLocal8Bit("*****SET_CAMERA_FRONT/RIGHT/TOP/ISOMETRIC/OPPOSITE/FITでプリセットのカメラアングル呼び出し");
//    out<<line<<endl;
//    line=QString::fromLocal8Bit("*****WAIT,msecで一時停止");
//    out<<line<<endl;
//    line=QString::fromLocal8Bit("*****SAVEPNG,filenameで画像保存(実行ファイルからの相対パスもしくはフルパス)");
//    out<<line<<endl;
//    line=QString::fromLocal8Bit("*****一部の設定を変えてRENDER,SAVEPNGとすることで連続して画像を描画できます");
//    out<<line<<endl;
//    line=QString::fromLocal8Bit("*****************************************************************************");
//    out<<line<<endl;


//    file.close();

//}

//void OutputPersWidget::readSettingFile(){
//    QString filename = QFileDialog::getOpenFileName( this, QString(), QString(),QString::fromLocal8Bit("Text(*.txt)") );
//    if(filename.isEmpty())return;
//    QFile file(filename);

//    if ( ! file.open(QIODevice::ReadOnly) ){
//        QMessageBox::warning( this, "WARNING","Failed to open" );
//        return;
//    }

//    QTextStream in(&file);
//    QString line;
//    QStringList datalist;

//    while(!in.atEnd()){
//        line=in.readLine();
//        if(line.isEmpty())continue;
//        if(line.left(1)==QString::fromLocal8Bit("*"))continue;
//        if(line.left(3)==QString::fromLocal8Bit("END"))break;
//        datalist=line.split(QString::fromLocal8Bit(","));
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RSTCASE1")){
//            if(datalist.count()<3)continue;
//            coefload1->setText(datalist.at(1));
//            comboload1->setCurrentIndex(comboload1->findText(datalist.at(2)));
//            if(datalist.count()==4)loadstep1->setValue(datalist.at(3).toInt());
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RSTCASE2")){
//            if(datalist.count()<3)continue;
//            coefload2->setText(datalist.at(1));
//            comboload2->setCurrentIndex(comboload2->findText(datalist.at(2)));
//            if(datalist.count()==4)loadstep2->setValue(datalist.at(3).toInt());
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RSTCASE3")){
//            if(datalist.count()<3)continue;
//            coefload3->setText(datalist.at(1));
//            comboload3->setCurrentIndex(comboload3->findText(datalist.at(2)));
//            if(datalist.count()==4)loadstep3->setValue(datalist.at(3).toInt());
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RSTCASE4")){
//            if(datalist.count()<3)continue;
//            coefload4->setText(datalist.at(1));
//            comboload4->setCurrentIndex(comboload4->findText(datalist.at(2)));
//            if(datalist.count()==4)loadstep4->setValue(datalist.at(3).toInt());
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_SELECT_FORCE_COLUMN")){
//            if(datalist.count()<7)continue;
//            for(int i=1;i<7;i++){
//                bool selectforce=false;
//                if(datalist.at(i)=="1")selectforce=true;
//                selectforceC.at(i-1)->setChecked(selectforce);
//            }
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_SELECT_FORCE_DATATYPE::TPGIRDER")){
//            if(datalist.count()<7)continue;
//            for(int i=1;i<7;i++){
//                bool selectforce=false;
//                if(datalist.at(i)=="1")selectforce=true;
//                selectforceG.at(i-1)->setChecked(selectforce);
//            }
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_SELECT_FORCE_BRACE")){
//            if(datalist.count()<7)continue;
//            for(int i=1;i<7;i++){
//                bool selectforce=false;
//                if(datalist.at(i)=="1")selectforce=true;
//                selectforceB.at(i-1)->setChecked(selectforce);
//            }
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_SELECT_FORCE_WALL")){
//            if(datalist.count()<4)continue;
//            for(int i=1;i<4;i++){
//                bool selectforce=false;
//                if(datalist.at(i)=="1")selectforce=true;
//                selectforceW.at(i-1)->setChecked(selectforce);
//            }
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_SELECT_FORCETYPE")){
//            if(datalist.count()<2)continue;
//            nodeorfacegroup->button(datalist.at(1).toInt())->setChecked(true);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_SELECT_MULTIFORCE")){
//            if(datalist.count()<2)continue;
//            valuecalcmethodgroup->button(datalist.at(1).toInt())->setChecked(true);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_SELECT_REACTION")){
//            if(datalist.count()<4)continue;
//            bool drawreactnumber=false;
//            if(datalist.at(2)=="1")drawreactnumber=true;
//            chkreactnum->setChecked(drawreactnumber);
//            reactscale->setValue(datalist.at(3).toDouble());
//            reactgroup->button(datalist.at(1).toInt())->setChecked(true);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_SELECT_DISP")){
//            if(datalist.count()<3)continue;
//            dispgroup->button(datalist.at(1).toInt())->setChecked(true);
//            spindisp->setValue(datalist.at(2).toInt());
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RENDERTYPE")){
//            if(datalist.count()<2)continue;
//            comborendertype->setCurrentIndex(datalist.at(1).toInt());
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RENDERGRAPH")){
//            if(datalist.count()<2)continue;
//            bool drawgraph=false;
//            if(datalist.at(1)=="1")drawgraph=true;
//            chkdrawgraph->setChecked(drawgraph);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RENDERCOLOR")){
//            if(datalist.count()<2)continue;
//            bool drawcontour=false;
//            if(datalist.at(1)=="1")drawcontour=true;
//            chkdrawcontour->setChecked(drawcontour);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RENDENUMBER")){
//            if(datalist.count()<2)continue;
//            bool drawnumber=false;
//            if(datalist.at(1)=="1")drawnumber=true;
//            chknumlabel->setChecked(drawnumber);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_GRAPHSCALE")){
//            if(datalist.count()<2)continue;
//            selectgraphscalegroup->button(datalist.at(1).toInt())->setChecked(true);
//            if(datalist.count()<3)continue;//スケールが書いてある場合
//            graphscale->setText(datalist.at(2));
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_COLORSCALE")){
//            if(datalist.count()<2)continue;
//            selectdrawcontour->button(datalist.at(1).toInt())->setChecked(true);
//            if(datalist.count()<4)continue;//スケールが書いてある場合
//            linemin->setText(datalist.at(2));
//            linemax->setText(datalist.at(3));
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_DISPLAYSCOPE_FLOOR")){
//            if(datalist.count()<2)continue;
//            limitedFloorGroup->button(datalist.at(1).toInt())->setChecked(true);
//            if(datalist.count()<3)continue;
//            QString str=datalist.at(2);
//            if(datalist.count()>3){
//                for(int i=3;i<datalist.count();i++){
//                    str=str+","+datalist.at(i);
//                }
//            }
//            limitedFloorLabel->setText(str);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_DISPLAYSCOPE_FRAME")){
//            if(datalist.count()<2)continue;
//            limitedFrameGroup->button(datalist.at(1).toInt())->setChecked(true);
//            if(datalist.count()<3)continue;
//            QString str=datalist.at(2);
//            if(datalist.count()>3){
//                for(int i=3;i<datalist.count();i++){
//                    str=str+","+datalist.at(i);
//                }
//            }
//            limitedFrameLabel->setText(str);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_DISPLAYSCOPE_SECTION")){
//            if(datalist.count()<2)continue;
//            QString str=datalist.at(1);
//            if(datalist.count()>2){
//                for(int i=2;i<datalist.count();i++){
//                    str=str+","+datalist.at(i);
//                }
//            }
//            limitedSectionLabel->setText(str);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RENDER_RENDERTYPE")){
//            if(datalist.count()<2)continue;
//            renderingGroup->button(datalist.at(1).toInt())->setChecked(true);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RENDER_SMOOTH")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chksmooth->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_DRAW_DISPNUM")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkdispnum->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_RENDER_SHADE")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkshade->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_LEGEND")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkdrawlegend->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_FILENAME")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkfilename->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_ZWALL")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkzwall->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_OPENING")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkopening->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_COORDINATE")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkcoordinate->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_LOGO")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chklogo->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_NODE")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkdrawpoint->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_LABEL")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkmemberlabel->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_YIELD")){
//            if(datalist.count()<2)continue;
//            bool flag=false;
//            if(datalist.at(1)=="1")flag=true;
//            chkdrawyield->setChecked(flag);
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_OPTION_FONTSIZE")){
//            if(datalist.count()<2)continue;
//            linefontsize->setText(datalist.at(1));
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA_VIEWPOINTVECTOR")){
//            if(datalist.count()<4)continue;
//            cameraViewPointVector=XYZ(datalist.at(1).toDouble(),datalist.at(2).toDouble(),datalist.at(3).toDouble());
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA_UPWARDVECTOR")){
//            if(datalist.count()<4)continue;
//            cameraUpwardVector=XYZ(datalist.at(1).toDouble(),datalist.at(2).toDouble(),datalist.at(3).toDouble());
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA_FOCUSVECTOR")){
//            if(datalist.count()<4)continue;
//            cameraFocusVector=XYZ(datalist.at(1).toDouble(),datalist.at(2).toDouble(),datalist.at(3).toDouble());
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA")){
//            //OutputPersWidgetのcameravalueを更新
//            setCameraValues(cameraViewPointVector,cameraUpwardVector,cameraFocusVector);
//            //persviewerのcameravalueを更新
//            slotSetCameraValues();
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA_FRONT")){
//            slotSetCameraFront();
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA_TOP")){
//            slotSetCameraTop();
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA_RIGHT")){
//            slotSetCameraRight();
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA_ISOMETRIC")){
//            slotSetCameraIsometric();
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA_OPPOSITE")){
//            slotSetCameraOpposite();
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SET_CAMERA_FIT")){
//            slotFitBoundingBox();
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("RENDER")){
//            slotRenderView();
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("WAIT")){
//            if(datalist.count()<2)continue;
//            QEventLoop loop;
//            QTimer::singleShot(datalist.at(1).toInt(),&loop,SLOT(quit()));
//            loop.exec();
//            continue;
//        }
//        if(datalist.at(0)==QString::fromLocal8Bit("SAVEPNG")){
//            //フレームバッファがうまく更新されないため便宜的にwaitを挟んで再描画してから保存
//            QEventLoop loop;
//            QTimer::singleShot(500,&loop,SLOT(quit()));
//            loop.exec();
//            myPersViewer->update();
//            QTimer::singleShot(500,&loop,SLOT(quit()));
//            loop.exec();

//            if(datalist.count()<2)continue;
//            if(datalist.count()==3){//3つめのデータが入力されていた場合、その値分waitする
//                QTimer::singleShot(datalist.at(2).toInt(),&loop,SLOT(quit()));
//                loop.exec();
//            }
//            QString filename = datalist.at(1);
//            if(filename.isEmpty())return;
//            QImage image=myPersViewer->grabFrameBuffer(false);
//            image.save(filename);
//            continue;
//        }
//    }
//}

//void OutputPersWidget::slotComboLoad1Changed(int index){
//    //loadcaseのmaxstepを取得してmaxstep値を書き換え
//    //stepのvalidatorを0～maxstepに書き換え
//    //stepの値を書き換え
//    QString loadcase=comboload1->itemText(index);
//    int maxstep;

//    handleNPstn* npstn = globalData::get()->npstn;
//    handlePstn* pstn = globalData::get()->pstn;
//    bool isNewPstn = globalData::get()->isNewPstn;

//    if(isNewPstn){
//        loadcase.isEmpty() ? maxstep=0 : maxstep=npstn->rst[loadcase].count();
//    }else{
//        loadcase.isEmpty() ? maxstep=0 : maxstep=pstn->rst[loadcase].count();
//    }
//    loadmaxstep1->setText(QString::number(maxstep));
//    loadstep1->setMaximum(maxstep);
//    loadstep1->setValue(maxstep);
//    return;
//}

//void OutputPersWidget::slotLoadStep1Changed(int step){
//    //stepがmaxstep以上のときはmaxstep、0未満のときは0を返す
//    //再描画
//    if(step > loadmaxstep1->text().toInt())step=loadmaxstep1->text().toInt();
//    if(step < 0)step=0;
//    loadstep1->setValue(step);
//    myPersViewer->loadViewSettings(this->getModelSettings());
//    myPersViewer->update();
//    return;
//}



//void OutputPersWidget::closeEvent(QCloseEvent *event)
//{
//    QMessageBox msg(this);
//    msg.setWindowTitle(QString::fromLocal8Bit("終了確認"));
//    msg.setText(QString::fromLocal8Bit("終了していいですか？"));
//    msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
//    msg.setDefaultButton(QMessageBox::Yes);
//    msg.button( QMessageBox::Yes )->setText(QString::fromLocal8Bit("終了"));
//    msg.button( QMessageBox::Cancel )->setText(QString::fromLocal8Bit("キャンセル"));
//    int ret = msg.exec();
//    if( ret== QMessageBox::Cancel ){ event->ignore(); return; }
//    event->accept();
//}
}
