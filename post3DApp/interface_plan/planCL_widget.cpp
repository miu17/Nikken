#include "planCL_widget.h"

#include <QtGui>

#include "custom_print_dialog.h"
#include "output_control.h"
#include "planCL_grscene.h"
#include "planCL_grview.h"
#include "reference_combobox.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "unified_output.h"
#include "unified_settings.h"

namespace post3dapp{

PlanCalcLoadWidget::PlanCalcLoadWidget(UnifiedInputData* input, QWidget *parent)
    : CustomDrawWidget(parent), copy(input)
{
    createSceneView();
    createButtons();
    createLayout();

    connect ( UnifiedInputData::getInInstance(), &UnifiedData::dataUpdated,
              this, &PlanCalcLoadWidget::updatePlanView );
    connect ( UnifiedInputData::getInInstance(), &UnifiedData::dataCleared,
              this, &PlanCalcLoadWidget::initializePlanView );

//    connect( UnifiedSettings::getInstance(), &UnifiedSettings::out2D_SettingChanged,
//             this, &PlanCalcLoadWidget::resetSceneView );

}

void PlanCalcLoadWidget::createSceneView()
{
    FIGURE_TYPE_PROPERTY figureTypeProperty;
    figureTypeProperty.viewtype = MODELVIEWTYPE::CALC2DMODEL;
    figureTypeProperty.scenetype = SCENETYPE::PLANSCENE;
    figureTypeProperty.viewdirection = OUTPUTDIR::ODIR_PLIN;
    figureTypeProperty.loadType = LoadType::LOADTYPE::LTYPE_DL;
    figureTypeProperty.loadSumType = LOADSUMTYPE::LOADSUM_ALL;

    planGrScene = new PlanCalcLoadGrScene(UnifiedSettings::getInstance()->getCalc2DViewSettings(), figureTypeProperty, true);
    connect(planGrScene, &CalcLoadGrScene::resetSceneView, this, &PlanCalcLoadWidget::resetSceneView);
    planGrView = new PlanCalcLoadGrView(planGrScene, this);
    planGrView->setMinimumHeight(300);
    planGrView->setMinimumWidth(200);
    connect( planGrView, &CustomDrawGrView::scaleChanged,
             this, &CustomDrawWidget::slotChangeSliderValue );

    planGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
}

void PlanCalcLoadWidget::createButtons()
{
    CustomDrawWidget::createButtons();

    floorCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::FixedInput, this);
    floorCombobox->setDefaultName(u8"（階の選択）");
    floorCombobox->setFixedWidth(110);

    connect(floorCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &PlanCalcLoadWidget::changeDrawingFloor);


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
            this, &PlanCalcLoadWidget::changeOutputCase);
    connect(outCaseBCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PlanCalcLoadWidget::changeOutputCase);
    connect(outCaseCCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PlanCalcLoadWidget::changeOutputCase);

    resetCaseCombobox(0);// LOADのコンボボックス消してるので、便宜的に0を渡している。LOAD系の出図増やす場合はindexを渡す

//    controlButton=new QToolButton(this);
//    controlButton->setAutoRaise(true);
//    controlButton->setIcon(QIcon(QPixmap(":/icons/control.png")));
//    controlButton->setToolTip(tr("Individual window display setting"));
//    controlButton->setIconSize(QSize(30,30));
//    controlButton->setFixedSize(30,30);

    printButton = new QToolButton(this);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setAutoRaise(true);
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));
    connect(printButton, &QAbstractButton::clicked,
            this, &PlanCalcLoadWidget::slotClickedPrintButton);

}
void PlanCalcLoadWidget::slotClickedPrintButton(){
    //TODO
    CustomPrintDialog* dialog = new CustomPrintDialog(planGrScene);
    if(dialog->exec()){

    }
    return;
}

CustomDrawGrView *PlanCalcLoadWidget::getView() const
{
    return planGrView;
}


void PlanCalcLoadWidget::createLayout()
{
    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(floorCombobox, 0);
    hlay1->addStretch();
    hlay1->addSpacing(10);
    hlay1->addWidget(zoomOutIcon, 0);
    hlay1->addWidget(zoomSlider, 0);
    hlay1->addWidget(zoomInIcon, 0);
    hlay1->addSpacing(5);
    hlay1->addWidget(fitButton, 0);

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

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 5, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay1, 0);
    vlay->addWidget(planGrView, 1);
    vlay->addLayout(hlay2, 0);

    setLayout(vlay);
}

void PlanCalcLoadWidget::resetCaseCombobox(int idx)
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

    QStringList strlistC = OutputControl::getInstance()->getOutfigStringList(LISTTYPE::INOUT);
    if ( strlistC.isEmpty() ) strlistC.append( u8"(条件C)" );
    outCaseCCombobox->addItems(strlistC);

    outCaseACombobox->blockSignals(false);
    outCaseBCombobox->blockSignals(false);
    outCaseCCombobox->blockSignals(false);

    resetSceneView();
}

void PlanCalcLoadWidget::changeDrawingFloor(const QString &fl)
{
    emit drawingFloorChanged(fl);
    resetSceneView();
}

void PlanCalcLoadWidget::changeOutputCase()
{
    qDebug() << outCaseACombobox->currentIndex() << outCaseBCombobox->currentIndex() << outCaseCCombobox->currentIndex();
    auto proprety = OutputControl::ChangeCalcLoadFigureTypeProperty(planGrScene->getFigureTypeProperty(),
                                                                    outCaseACombobox->currentIndex(),
                                                                    outCaseBCombobox->currentIndex(),
                                                                    outCaseCCombobox->currentIndex());
    planGrScene->updateFigureTypeProperty(proprety);
}

void PlanCalcLoadWidget::updatePlanView(DATATYPE type)
{
    /* 階の順番が変更された場合 → floorComboBoxのUpdate　→　currentIndexの変更
       →　PlanInterfaceDrawWidget::changeDrawingFloor()
       → PlanInterfaceGrView::setBackGroundFrame() により自動的にアップデートされる */

    if ( type != DATATYPE::TPFRAMEP ) return;
    resetSceneView();
}

void PlanCalcLoadWidget::resetSceneView()
{
    planGrScene->setStatus( floorCombobox->currentUuid());
}

void PlanCalcLoadWidget::initializePlanView()
{
    floorCombobox->setCurrentIndex(0);
}

} // namespace post3dapp
