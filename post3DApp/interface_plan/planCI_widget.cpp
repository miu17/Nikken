#include "planCI_widget.h"

#include <QtGui>
#include <QProxyStyle>

#include "custom_print_dialog.h"
#include "planCI_grscene.h"
#include "planCI_grview.h"
#include "reference_combobox.h"
#include "unified_inputdata.h"
#include "unified_settings.h"

namespace post3dapp{


PlanCopyInterfaceWidget::PlanCopyInterfaceWidget(UnifiedInputData* input, QWidget *parent)
    : CustomDrawWidget(parent), copy(input)
{
    createSceneView();
    createButtons();
    createLayout();

//    connect( UnifiedSettings::getInstance(), &UnifiedSettings::in2D_SettingChanged,
//             this, &PlanCopyInterfaceWidget::resetSceneView );
}

void PlanCopyInterfaceWidget::createSceneView()
{
    planGrScene = new PlanCopyInterfaceGrScene(LIMITSELECTTERM(), copy, UnifiedSettings::getInstance()->getInput2DViewSettings(), this);
    connect(planGrScene, &InputGrScene::resetSceneView, this, &PlanCopyInterfaceWidget::resetSceneView);
    planGrView = new PlanCopyInterfaceGrView(planGrScene, this);
    planGrView->setMinimumHeight(300);
    planGrView->setMinimumWidth(200);
    connect( planGrView, &CustomDrawGrView::scaleChanged,
             this, &CustomDrawWidget::slotChangeSliderValue );

    planGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
}

void PlanCopyInterfaceWidget::createButtons()
{
    CustomDrawWidget::createButtons();

    floorCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::FixedInput, this);
    floorCombobox->setDefaultName(u8"（階の選択）");
    floorCombobox->setFixedWidth(110);

    connect(floorCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &PlanCopyInterfaceWidget::changeDrawingFloor);

    printButton = new QToolButton(this);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setAutoRaise(true);
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));
    connect(printButton, &QAbstractButton::clicked,
            this, &PlanCopyInterfaceWidget::slotClickedPrintButton);
}
void PlanCopyInterfaceWidget::slotClickedPrintButton(){
    //TODO
    CustomPrintDialog* dialog = new CustomPrintDialog(planGrScene);
    if(dialog->exec()){

    }
    return;
}

CustomDrawGrView *PlanCopyInterfaceWidget::getView() const
{
    return planGrView;
}


void PlanCopyInterfaceWidget::createLayout()
{
    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(floorCombobox, 0);
    hlay1->addSpacing(5);
    hlay1->addWidget(printButton, 0);
    hlay1->addStretch();
    hlay1->addSpacing(10);
    hlay1->addWidget(zoomOutIcon, 0);
    hlay1->addWidget(zoomSlider, 0);
    hlay1->addWidget(zoomInIcon, 0);
    hlay1->addSpacing(5);
    hlay1->addWidget(fitButton, 0);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 5, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay1, 0);
    vlay->addWidget(planGrView, 1);

    setLayout(vlay);
}

void PlanCopyInterfaceWidget::changeDrawingFloor(const QString &fl)
{
    emit drawingFloorChanged(fl);
    resetSceneView();
}

void PlanCopyInterfaceWidget::updatePlanView(DATATYPE type)
{
    /* 階の順番が変更された場合　→　floorComboBoxのUpdate　→　currentIndexの変更
       →　PlanInterfaceDrawWidget::changeDrawingFloor()
       → PlanInterfaceGrView::setBackGroundFrame() により自動的にアップデートされる */

    if (type != DATATYPE::TPFRAMEP) return;
    resetSceneView();
}

void PlanCopyInterfaceWidget::resetSceneView()
{
    planGrScene->setBackGround(floorCombobox->currentText());
    planGrView->initializeEventOperation();
}

void PlanCopyInterfaceWidget::initializePlanView()
{
    floorCombobox->setCurrentIndex(0);
}

void PlanCopyInterfaceWidget::sendActiveStatusToScene()
{
    planGrScene->sendSelectedItems();
}
} // namespace post3dapp
