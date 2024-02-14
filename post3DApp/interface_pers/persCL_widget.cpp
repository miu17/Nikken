#include "persCL_widget.h"

#include <QtGui>

#include "custom_print_3d_dialog.h"
#include "define_output.h"
#include "output_control.h"
#include "persCL_glview.h"
#include "persCL_scene.h"
#include "persCL_settingdialog.h"
#include "reference_combobox.h"
#include "textcalcload_dialog.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "unified_settings.h"

namespace post3dapp{

PersCalcLoadWidget::PersCalcLoadWidget(QWidget *parent)
    : InterfaceWidget(parent)
{
    controlDialog = new PersCalcLoadSettingDialog(this);
    connect( controlDialog, &PersCalcLoadSettingDialog::viewSettingChanged, this, &PersCalcLoadWidget::slotChangeViewSettings );

    connect ( UnifiedInputData::getInInstance(), &UnifiedData::dataCleared,
              this, &PersCalcLoadWidget::initializePlanView );

    createSceneView();
    createButtons();

    connect (myGLView, &PersCalcLoadGLViewer::signalShowTextDialog, this, &PersCalcLoadWidget::slotShowTextDialog);
}

PersCalcLoadWidget::~PersCalcLoadWidget()
{
    delete myGLView;
    delete myScene;
}

void PersCalcLoadWidget::createSceneView()
{
    myScene = new PersCalcLoadScene(this);
    QGLWidget *share = UnifiedEditingData::getInstance()->getSingularGLWidget();
    myGLView = new PersCalcLoadGLViewer(myScene, this, share);

    myScene->initDisplayList();
}

void PersCalcLoadWidget::createButtons()
{
    // 上部　左

    floorCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::FixedInput, this);
    floorCombobox->setDefaultName(u8"（階の選択）");
    floorCombobox->setFixedWidth(110);

    connect(floorCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &PersCalcLoadWidget::changeDrawingFloor);

    frameCombobox = new ReferenceCombobox(DATATYPE::TPFRAMEP, UnifiedDataType::FixedInput, this);
    frameCombobox->setDefaultName(u8"（通りの選択）");
    frameCombobox->setFixedWidth(110);

    connect(frameCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &PersCalcLoadWidget::changeDrawingFrame);

//    outTypeCombobox = new QComboBox(this);
//    outTypeCombobox->setStyle( FactoryUnifiedData::getInstance(isOutput)->getWindowXPStyle() );
//    outTypeCombobox->setFixedWidth(140);

//    outTypeCombobox->addItems( OutputControl::getInstance()->outputTypeStringList(OUTFIGCATEGORY_LOAD) );

//    connect(outTypeCombobox, SIGNAL(currentIndexChanged(int)),
//            this, SLOT(resetCaseCombobox(int)));

    // 上部　右

    QToolButton *zoomInButton = new QToolButton(this);
    zoomInButton->setToolTip(u8"表示拡大");
    zoomInButton->setAutoRaise(true);
    zoomInButton->setIcon(QIcon(QPixmap(":/icons/zoomin.png")));
    zoomInButton->setIconSize(QSize(25, 25));
    zoomInButton->setFixedSize(30, 30);
    zoomInButton->setAutoRepeat(true);
    zoomInButton->setAutoRepeatInterval(33);
    zoomInButton->setAutoRepeatDelay(0);

    QToolButton *zoomOutButton = new QToolButton(this);
    zoomOutButton->setToolTip(u8"表示縮小");
    zoomOutButton->setAutoRaise(true);
    zoomOutButton->setIcon(QIcon(QPixmap(":/icons/zoomout.png")));
    zoomOutButton->setIconSize(QSize(25, 25));
    zoomOutButton->setFixedSize(30, 30);
    zoomOutButton->setAutoRepeat(true);
    zoomOutButton->setAutoRepeatInterval(33);
    zoomOutButton->setAutoRepeatDelay(0);

    connect ( zoomInButton, &QAbstractButton::clicked, myGLView, &PersCalcLoadGLViewer::zoomIn );
    connect ( zoomOutButton, &QAbstractButton::clicked, myGLView, &PersCalcLoadGLViewer::zoomOut );

    QToolButton *fitButton = new QToolButton(this);
    fitButton->setToolTip(u8"画面フィッティング");
    fitButton->setAutoRaise(true);
    fitButton->setIcon(QIcon(QPixmap(":/icons/fit.png")));
    fitButton->setFixedSize(30, 30);
    fitButton->setIconSize(QSize(25, 25));

    connect ( fitButton, &QAbstractButton::clicked, myGLView, &PersCalcLoadGLViewer::fitWindow );

    // 上部ボタンレイアウト

    QHBoxLayout *hlay_up = new QHBoxLayout;
    hlay_up->setSpacing(5);
//    hlay_up->addWidget(outTypeCombobox);
//    hlay_up->addSpacing(10);
    hlay_up->addWidget(floorCombobox);
    hlay_up->addWidget(frameCombobox);
    hlay_up->addStretch();
    hlay_up->addWidget(zoomOutButton);
    hlay_up->addWidget(zoomInButton);
    hlay_up->addSpacing(5);
    hlay_up->addWidget(fitButton);

    // 下部　左

    outCaseACombobox = new QComboBox(this);
    outCaseACombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    outCaseACombobox->setFixedWidth(100);

    outCaseBCombobox = new QComboBox(this);
    outCaseBCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    outCaseBCombobox->setFixedWidth(100);

    outCaseCCombobox = new QComboBox(this);
    outCaseCCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    outCaseCCombobox->setFixedWidth(100);

    connect(outCaseACombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PersCalcLoadWidget::changeOutputCase);
    connect(outCaseBCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PersCalcLoadWidget::changeOutputCase);
    connect(outCaseCCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PersCalcLoadWidget::changeOutputCase);

    resetCaseCombobox(0);//LOADのコンボボックス消してるので、便宜的に0を渡している。LOAD系の出図増やす場合はindexを渡す


    // 下部　右

    controlButton = new QToolButton(this);
    controlButton->setAutoRaise(true);
    controlButton->setIcon(QIcon(QPixmap(":/icons/control.png")));
    controlButton->setToolTip(u8"個別ウィンドウの表示設定");
    controlButton->setIconSize(QSize(30, 30));
    controlButton->setFixedSize(30, 30);

    controlButton->setCheckable(true);
    controlButton->setChecked(false);

    connect(controlButton, &QAbstractButton::toggled, this, &PersCalcLoadWidget::slotShowControlDialog);
    connect(controlDialog, &PersCalcLoadSettingDialog::windowClosed, this, &PersCalcLoadWidget::slotControlButtonUnChecked);

    printButton = new QToolButton(this);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setAutoRaise(true);
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));
    connect(printButton, &QAbstractButton::clicked,
            this, &PersCalcLoadWidget::slotClickedPrintButton);



    // 下部ボタンレイアウト

    QHBoxLayout *hlay_dn = new QHBoxLayout;
    hlay_dn->addWidget(outCaseACombobox, 0);
    hlay_dn->addSpacing(5);
    hlay_dn->addWidget(outCaseBCombobox, 0);
    hlay_dn->addSpacing(5);
    hlay_dn->addWidget(outCaseCCombobox, 0);
    hlay_dn->addStretch();
    hlay_dn->addSpacing(10);
    hlay_dn->addWidget(controlButton, 0);
    hlay_dn->addSpacing(5);
    hlay_dn->addWidget(printButton, 0);

    // 全体レイアウト

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 5, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay_up, 0);
    vlay->addWidget(myGLView, 1);
    vlay->addLayout(hlay_dn, 0);

    setLayout(vlay);
}

void PersCalcLoadWidget::slotSavePixmap()
{

}

void PersCalcLoadWidget::slotShowControlDialog(bool onoff)
{
    if (!onoff) {
        controlDialog->hide();
        return;
    }
    controlDialog->setCenterPosition( myGLView->camera()->revolveAroundPoint().x,
                                      myGLView->camera()->revolveAroundPoint().y,
                                      myGLView->camera()->revolveAroundPoint().z );
    controlDialog->setCurrentValues();
    controlDialog->show();
}

void PersCalcLoadWidget::slotChangeViewSettings()
{
    PERSSETTINGVALUES settings = controlDialog->currentViewSettings();
    myGLView->changeViewSettings(settings);
}

void PersCalcLoadWidget::slotControlButtonUnChecked()
{
    controlButton->setChecked(false);
}

void PersCalcLoadWidget::slotShowTextDialog()
{
    TextCalcLoadDialog dialog(this);
    QList<JointData*> jdlist;
    QList<MemberData*> mdlist;
    Q_FOREACH ( JointData *jd, myScene->currentSelectedJoints() ){
        jdlist.append(jd);
    }
    Q_FOREACH ( MemberData *md, myScene->currentSelectedMembers() ){
        mdlist.append( md);
    }
    dialog.setEditorText(  jdlist, mdlist );
    dialog.exec();
}

void PersCalcLoadWidget::slotClickedPrintButton()
{
    CustomPrint3dDialog* dialog = new CustomPrint3dDialog(myGLView);
    if (dialog->exec()){

    }
    return;
}

void PersCalcLoadWidget::resetCaseCombobox(int idx)
{
    outCaseACombobox->blockSignals(true);
    outCaseBCombobox->blockSignals(true);
    outCaseCCombobox->blockSignals(true);

    outCaseACombobox->clear();
    outCaseBCombobox->clear();
    outCaseCCombobox->clear();


    QStringList strlistA = OutputControl::getInstance()->getOutfigStringList(LISTTYPE::LOAD);
    if ( strlistA.isEmpty() ) strlistA.append( u8"(条件A)" );
    outCaseACombobox->addItems(strlistA);

    QStringList strlistB = OutputControl::getInstance()->getOutfigStringList(LISTTYPE::LOADTYPE);
    if ( strlistB.isEmpty() ) strlistB.append( u8"(条件B)" );
    outCaseBCombobox->addItems(strlistB);

    QStringList strlistC = OutputControl::getInstance()->getOutfigStringList(LISTTYPE::BUZAIJIKU);
    if ( strlistC.isEmpty() ) strlistC.append( u8"(条件C)" );
    outCaseCCombobox->addItems(strlistC);


    outCaseACombobox->blockSignals(false);
    outCaseBCombobox->blockSignals(false);
    outCaseCCombobox->blockSignals(false);
}

void PersCalcLoadWidget::changeDrawingFloor(const QString &fl)
{
    myScene->setLimitedFloor(fl);
}

void PersCalcLoadWidget::changeDrawingFrame(const QString &fr)
{
    myScene->setLimitedFrame(fr);
}

void PersCalcLoadWidget::changeOutputCase()
{
    LoadType::LOADTYPE indexA = static_cast<LoadType::LOADTYPE>(outCaseACombobox->currentIndex());
    LOADSUMTYPE indexB = static_cast<LOADSUMTYPE>(outCaseBCombobox->currentIndex());
    CALCOUTPUTDIR indexC = static_cast<CALCOUTPUTDIR>(outCaseCCombobox->currentIndex());

    myScene->changeCaseIndex(indexA,
                             indexB,
                             indexC);
    myGLView->update();
}

void PersCalcLoadWidget::initializePlanView()
{
    floorCombobox->setCurrentIndex(0);
    frameCombobox->setCurrentIndex(0);
//    outTypeCombobox->setCurrentIndex(0);
}

} // namespace post3dapp
