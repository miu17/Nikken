#include "persOT_widget.h"

#include <QtGui>

#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "manage_springdata.h"
#include "output_control.h"
#include "persOT_glview.h"
#include "persOT_scene.h"
#include "persOT_settingdialog.h"
#include "reference_combobox.h"
#include "textoutput_dialog.h"
#include "unified_analysisdata.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "unified_settings.h"

namespace post3dapp{
PersOutputWidget::PersOutputWidget(QWidget *parent)
    : InterfaceWidget(parent)
{
    controlDialog = new PersOutputSettingDialog(this);
    connect( controlDialog, &PersOutputSettingDialog::viewSettingChanged, this, &PersOutputWidget::slotChangeViewSettings );

    connect ( UnifiedFixedData::getInstance()->getAnalysisData(), &UnifiedData::dataCleared,
              this, &PersOutputWidget::initializePlanView );

    createSceneView();
    createButtons();

    connect (myGLView, &PersOutputGLViewer::signalShowTextDialog, this, &PersOutputWidget::slotShowTextDialog);
}

PersOutputWidget::~PersOutputWidget()
{
    delete myGLView;
    delete myScene;
}

void PersOutputWidget::createSceneView()
{
    myScene = new PersOutputScene(this);
    QGLWidget *share = UnifiedEditingData::getInstance()->getSingularGLWidget();
    myGLView = new PersOutputGLViewer(myScene, this, share);
    myScene->setChildViewer(myGLView);

    myScene->initDisplayList();
}

void PersOutputWidget::createButtons()
{
    // 上部　左

    floorCombobox = new ReferenceCombobox(DATATYPE::TPAFLOOR, UnifiedDataType::Pstn, this);
    floorCombobox->setDefaultName(u8"（階の選択）");
    floorCombobox->setFixedWidth(110);

    connect(floorCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &PersOutputWidget::changeDrawingFloor);

    frameCombobox = new ReferenceCombobox(DATATYPE::TPFRAMEP, UnifiedDataType::Pstn, this);
    frameCombobox->setDefaultName(u8"（通りの選択）");
    frameCombobox->setFixedWidth(110);

    connect(frameCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &PersOutputWidget::changeDrawingFrame);

    outTypeCombobox = new QComboBox(this);
    outTypeCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    outTypeCombobox->setFixedWidth(140);

    outTypeCombobox->addItems( OutputControl::getInstance()->outputTypeStringList(OUTFIGCATEGORY::PERS) );

    connect(outTypeCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PersOutputWidget::resetCaseCombobox);

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

    connect ( zoomInButton, &QAbstractButton::clicked, myGLView, &PersOutputGLViewer::zoomIn );
    connect ( zoomOutButton, &QAbstractButton::clicked, myGLView, &PersOutputGLViewer::zoomOut );

    QToolButton *fitButton = new QToolButton(this);
    fitButton->setToolTip(u8"画面フィッティング");
    fitButton->setAutoRaise(true);
    fitButton->setIcon(QIcon(QPixmap(":/icons/fit.png")));
    fitButton->setFixedSize(30, 30);
    fitButton->setIconSize(QSize(25, 25));

    connect ( fitButton, &QAbstractButton::clicked, myGLView, &PersOutputGLViewer::fitWindow );

    // 上部ボタンレイアウト

    QHBoxLayout *hlay_up = new QHBoxLayout;
    hlay_up->setSpacing(5);
    hlay_up->addWidget(outTypeCombobox);
    hlay_up->addSpacing(10);
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
            this, &PersOutputWidget::changeOutputCase);
    connect(outCaseBCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PersOutputWidget::changeOutputCase);
    connect(outCaseCCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PersOutputWidget::changeOutputCase);

    resetCaseCombobox(0);


    // 下部　右

    controlButton = new QToolButton(this);
    controlButton->setAutoRaise(true);
    controlButton->setIcon(QIcon(QPixmap(":/icons/control.png")));
    controlButton->setToolTip(u8"個別ウィンドウの表示設定");
    controlButton->setIconSize(QSize(30, 30));
    controlButton->setFixedSize(30, 30);

    printButton = new QToolButton(this);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setAutoRaise(true);
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));

    controlButton->setCheckable(true);
    controlButton->setChecked(false);

    connect(controlButton, &QAbstractButton::toggled, this, &PersOutputWidget::slotShowControlDialog);
    connect(controlDialog, &PersOutputSettingDialog::windowClosed, this, &PersOutputWidget::slotControlButtonUnChecked);


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

void PersOutputWidget::slotSavePixmap()
{

}

void PersOutputWidget::slotShowControlDialog(bool onoff)
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

void PersOutputWidget::slotChangeViewSettings()
{
    PERSSETTINGVALUES settings = controlDialog->currentViewSettings();
    myGLView->changeViewSettings(settings);
}

void PersOutputWidget::slotControlButtonUnChecked()
{
    controlButton->setChecked(false);
}

void PersOutputWidget::resetCaseCombobox(int idx)
{
    outCaseACombobox->blockSignals(true);
    outCaseBCombobox->blockSignals(true);
    outCaseCCombobox->blockSignals(true);

    outCaseACombobox->clear();
    outCaseBCombobox->clear();
    outCaseCCombobox->clear();


    OutputControl* control = OutputControl::getInstance();
    LISTTYPE ltype = control->listType(idx,OUTFIGCATEGORY::PERS);

    QStringList strlistA = control->getOutfigStringList(LISTTYPE::INOUT);
    if ( strlistA.isEmpty() ) strlistA.append( u8"(条件A)" );
    outCaseACombobox->addItems(strlistA);

    QStringList strlistB = control->getOutfigStringList(LISTTYPE::BUZAIJIKU);
    if ( strlistB.isEmpty() ) strlistB.append( u8"(条件B)" );
    outCaseBCombobox->addItems(strlistB);

    QStringList strlistC = control->getOutfigStringList(ltype);
    if ( strlistC.isEmpty() ) strlistC.append( u8"(条件C)" );
    outCaseCCombobox->addItems(strlistC);


    outCaseACombobox->blockSignals(false);
    outCaseBCombobox->blockSignals(false);
    outCaseCCombobox->blockSignals(false);

    resetSceneView();
}

void PersOutputWidget::changeDrawingFloor(const QString &fl)
{
    myScene->setLimitedFloor(fl);
}

void PersOutputWidget::changeDrawingFrame(const QString &fr)
{
    myScene->setLimitedFrame(fr);
}

void PersOutputWidget::changeOutputCase()
{
    myScene->changeCaseIndex(outCaseACombobox->currentIndex(),
                             outCaseBCombobox->currentIndex(),
                             outCaseCCombobox->currentIndex());
}

void PersOutputWidget::resetSceneView()
{
    QList<OUTFIGTYPE> typelist = OutputControl::getInstance()->outputTypeList(OUTFIGCATEGORY::PERS);
    OUTFIGTYPE otype = typelist.at(outTypeCombobox->currentIndex());
    myScene->setFigureType( otype );
}

void PersOutputWidget::initializePlanView()
{
    floorCombobox->setCurrentIndex(0);
    frameCombobox->setCurrentIndex(0);
    outTypeCombobox->setCurrentIndex(0);
}

void PersOutputWidget::slotShowTextDialog()
{
    TextOutputDialog dialog(this);

    QList<QUuid> jdlist, mdlist,sdlist;
    Q_FOREACH ( JointData *jd, myScene->currentSelectedJoints() ) jdlist.append( jd->getUuid() );
    Q_FOREACH ( MemberData *md, myScene->currentSelectedMembers() ) mdlist.append( md->getUuid() );
    Q_FOREACH ( SpringData *sd, myScene->currentSelectedSprings() ) sdlist.append( sd->getUuid() );

    dialog.setEditorText( myScene->getFigureTypeProperty(), jdlist, mdlist,QUuid() );
    dialog.exec();
}
} // namespace post3dapp
