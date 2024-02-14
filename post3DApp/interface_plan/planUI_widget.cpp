#include "planUI_widget.h"

#include <QtGui>
#include <QProxyStyle>

#include "custom_print_dialog.h"
#include "planUI_grscene.h"
#include "planUI_grview.h"
#include "reference_combobox.h"
#include "unified_inputdata.h"
#include "unified_settings.h"

namespace post3dapp{


PlanInterfaceWidget::PlanInterfaceWidget(QWidget *parent)
    : CustomDrawWidget(parent)
{
    createSceneView();
    createButtons();
    createLayout();

    connect(UnifiedInputData::getInInstance(), &UnifiedInputData::dataUpdated, this, &PlanInterfaceWidget::resetSceneView);
    connect(UnifiedInputData::getInInstance(), &UnifiedInputData::dataCleared, this, &PlanInterfaceWidget::initializePlanView);
//    connect(UnifiedSettings::getInstance(), &UnifiedSettings::in2D_SettingChanged, this, &PlanInterfaceWidget::resetSceneView);
}

void PlanInterfaceWidget::createSceneView()
{
    qDebug()<<"PlanInterfaceWidget::createSceneView : in";

    planGrScene = new PlanInterfaceGrScene(LIMITSELECTTERM(), UnifiedSettings::getInstance()->getInput2DViewSettings(),this);
    qDebug()<<"PlanInterfaceWidget::createSceneView : new scene";
    connect(planGrScene, &InputGrScene::resetSceneView, this, &PlanInterfaceWidget::resetSceneView);
    planGrView = new PlanInterfaceGrView(planGrScene, this);
    planGrView->setMinimumHeight(300);
    planGrView->setMinimumWidth(200);
    connect(planGrView, &CustomDrawGrView::scaleChanged, this, &PlanInterfaceWidget::slotChangeSliderValue);

    planGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
}

void PlanInterfaceWidget::createButtons()
{
    CustomDrawWidget::createButtons();

    floorCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::Input, this);
    floorCombobox->setDefaultName(u8"（階の選択）");
    floorCombobox->setFixedWidth(110);
    connect(floorCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &PlanInterfaceWidget::changeDrawingFloor);

    printButton = new QToolButton(this);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setAutoRaise(true);
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));
    connect(printButton, &QToolButton::clicked, this, &PlanInterfaceWidget::slotClickedPrintButton);
}
void PlanInterfaceWidget::slotClickedPrintButton(){
    //TODO
    CustomPrintDialog* dialog = new CustomPrintDialog(planGrScene);
    if(dialog->exec()){

    }
    return;
}

CustomDrawGrView *PlanInterfaceWidget::getView() const
{
    return planGrView;
}


void PlanInterfaceWidget::createLayout()
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

void PlanInterfaceWidget::changeDrawingFloor(const QString &fl)
{
    emit drawingFloorChanged(fl);
    resetSceneView();
}

void PlanInterfaceWidget::resetSceneView()
{
    planGrScene->setBackGround(floorCombobox->currentText());
    planGrView->initializeEventOperation();
}

void PlanInterfaceWidget::initializePlanView()
{
    floorCombobox->setCurrentIndex(0);
}

void PlanInterfaceWidget::sendActiveStatusToScene()
{
    planGrScene->sendSelectedItems();
}
} // namespace post3dapp
