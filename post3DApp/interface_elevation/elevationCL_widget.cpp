#include "elevationCL_widget.h"

#include <QtGui>

#include "custom_print_dialog.h"
#include "elevationCL_grscene.h"
#include "elevationCL_grview.h"
#include "output_control.h"
#include "reference_combobox.h"
#include "unified_editingdata.h"
#include "unified_output.h"
#include "unified_settings.h"

namespace post3dapp{

ElevationCalcLoadWidget::ElevationCalcLoadWidget(UnifiedInputData* input, QWidget *parent)
    : CustomDrawWidget(parent), copy(input)
{
    createSceneView();
    createButtons();
    createLayout();

//    connect( UnifiedSettings::getInstance(), &UnifiedSettings::out2D_SettingChanged,
//             this, &ElevationCalcLoadWidget::resetSceneView );

}

void ElevationCalcLoadWidget::createSceneView()
{
    FIGURE_TYPE_PROPERTY figureTypeProperty;
    figureTypeProperty.viewtype = MODELVIEWTYPE::CALC2DMODEL;
    figureTypeProperty.scenetype = SCENETYPE::ELEVATIONSCENE;
    figureTypeProperty.viewdirection = OUTPUTDIR::ODIR_PLIN;
    figureTypeProperty.loadType = LoadType::LOADTYPE::LTYPE_DL;
    figureTypeProperty.loadSumType = LOADSUMTYPE::LOADSUM_ALL;

    elevationGrScene = new ElevationCalcLoadGrScene(UnifiedSettings::getInstance()->getCalc2DViewSettings(), figureTypeProperty, this);
    connect(elevationGrScene, &CalcLoadGrScene::resetSceneView, this, &ElevationCalcLoadWidget::resetSceneView);
    elevationGrView = new ElevationCalcLoadGrView(elevationGrScene, this);
    elevationGrView->setMinimumHeight(300);
    elevationGrView->setMinimumWidth(200);
    connect( elevationGrView, &CustomDrawGrView::scaleChanged,
             this,   &CustomDrawWidget::slotChangeSliderValue );

    elevationGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
}

void ElevationCalcLoadWidget::createButtons()
{
    qDebug() << "createbuttons";
    CustomDrawWidget::createButtons();

    frameCombobox = new ReferenceCombobox(DATATYPE::TPFRAMEP, UnifiedDataType::FixedInput, this);
    frameCombobox->setDefaultName(u8"（通りの選択）");
    frameCombobox->setFixedWidth(110);

    connect(frameCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, QOverload<const QString&>::of(&ElevationCalcLoadWidget::changeDrawingFrame));

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
            this, &ElevationCalcLoadWidget::changeOutputCase);
    connect(outCaseBCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ElevationCalcLoadWidget::changeOutputCase);
    connect(outCaseCCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ElevationCalcLoadWidget::changeOutputCase);

    resetCaseCombobox(0);//LOADのコンボボックス消してるので、便宜的に0を渡している。LOAD系の出図増やす場合はindexを渡す

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
    connect(printButton, &QAbstractButton::clicked,
            this, &ElevationCalcLoadWidget::slotClickedPrintButton);
}
void ElevationCalcLoadWidget::slotClickedPrintButton(){
    //TODO
    CustomPrintDialog* dialog = new CustomPrintDialog(elevationGrScene);
    if(dialog->exec()){

    }
    return;
}

CustomDrawGrView *ElevationCalcLoadWidget::getView() const
{
    return elevationGrView;
}

void ElevationCalcLoadWidget::createLayout()
{
    qDebug() << "createlayout";

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(frameCombobox, 0);
    hlay1->addStretch();
    hlay1->addSpacing(10);
    hlay1->addWidget(zoomOutIcon, 0);
    hlay1->addWidget(zoomSlider, 0);
    hlay1->addWidget(zoomInIcon, 0);
    hlay1->addSpacing(5);
    hlay1->addWidget(fitButton, 0);

    qDebug() << "set hlay1";
    QHBoxLayout *hlay2 = new QHBoxLayout;
    hlay2->addWidget(outCaseACombobox, 0);
    hlay2->addSpacing(5);
    hlay2->addWidget(outCaseBCombobox, 0);
    hlay2->addSpacing(5);
    hlay2->addWidget(outCaseCCombobox, 0);
    hlay2->addStretch();
    hlay2->addSpacing(10);
//    hlay2->addWidget(controlButton,0);
//    hlay2->addSpacing(5);
    hlay2->addWidget(printButton, 0);
    qDebug() << "set hlay2";

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 5, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay1, 0);
    vlay->addWidget(elevationGrView, 1);
    vlay->addLayout(hlay2, 0);

    setLayout(vlay);
}

void ElevationCalcLoadWidget::resetCaseCombobox(int idx)
{
    qDebug() << "createcasecombobox";

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

    QStringList strlistC = OutputControl::getInstance()->getOutfigStringList(LISTTYPE::INOUT);
    if ( strlistC.isEmpty() ) strlistC.append( u8"(条件C)" );
    outCaseCCombobox->addItems(strlistC);


    outCaseACombobox->blockSignals(false);
    outCaseBCombobox->blockSignals(false);
    outCaseCCombobox->blockSignals(false);

    resetSceneView();
}

void ElevationCalcLoadWidget::changeDrawingFrame(const QString &fr)
{
    emit drawingFrameChanged(fr);
    resetSceneView();
}

void ElevationCalcLoadWidget::changeOutputCase()
{
    qDebug() << "changeoutputcase";
    auto proprety = OutputControl::ChangeCalcLoadFigureTypeProperty(elevationGrScene->getFigureTypeProperty(),
                                                                    outCaseACombobox->currentIndex(),
                                                                    outCaseBCombobox->currentIndex(),
                                                                    outCaseCCombobox->currentIndex());
    elevationGrScene->updateFigureTypeProperty(proprety);
}

void ElevationCalcLoadWidget::resetSceneView()
{

    elevationGrScene->setStatus( frameCombobox->currentUuid() );
}

void ElevationCalcLoadWidget::initializePlanView()
{
    frameCombobox->setCurrentIndex(0);
}

} // namespace post3dapp
