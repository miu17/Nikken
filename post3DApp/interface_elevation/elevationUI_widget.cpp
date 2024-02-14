#include "elevationUI_widget.h"

#include <QtGui>
#include <QProxyStyle>

#include "custom_print_dialog.h"
#include "elevationUI_grscene.h"
#include "elevationUI_grview.h"
#include "reference_combobox.h"
#include "unified_inputdata.h"
#include "unified_settings.h"


namespace post3dapp{

ElevationInterfaceWidget::ElevationInterfaceWidget(QWidget *parent)
    : CustomDrawWidget(parent)
{
    createSceneView();
    createButtons();
    createLayout();

    connect ( UnifiedInputData::getInInstance(), &UnifiedData::dataUpdated,
              this, &ElevationInterfaceWidget::resetSceneView );
    connect ( UnifiedInputData::getInInstance(), &UnifiedData::dataCleared,
              this, &ElevationInterfaceWidget::initializePlanView );

//    connect(UnifiedSettings::getInstance(), &UnifiedSettings::in2D_SettingChanged,
//            this, &ElevationInterfaceWidget::resetSceneView);
}

void ElevationInterfaceWidget::createSceneView()
{
    elevationGrScene = new ElevationInterfaceGrScene(LIMITSELECTTERM(), UnifiedSettings::getInstance()->getInput2DViewSettings(), true, this);
    connect(elevationGrScene, &InputGrScene::resetSceneView, this, &ElevationInterfaceWidget::resetSceneView);
    elevationGrView = new ElevationInterfaceGrView(elevationGrScene, this);
    elevationGrView->setMinimumHeight(300);
    elevationGrView->setMinimumWidth(200);
    connect( elevationGrView, &CustomDrawGrView::scaleChanged,
             this,   &CustomDrawWidget::slotChangeSliderValue );

    elevationGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
}

void ElevationInterfaceWidget::createButtons()
{
    CustomDrawWidget::createButtons();

    frameCombobox = new ReferenceCombobox(DATATYPE::TPFRAMEP, UnifiedDataType::Input, this);
    frameCombobox->setDefaultName(u8"（通りの選択）");
    frameCombobox->setFixedWidth(110);

    connect(frameCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, &ElevationInterfaceWidget::changeDrawingFrame);

    printButton = new QToolButton(this);
    printButton->setAutoRaise(true);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));
    connect(printButton, &QAbstractButton::clicked,
            this, &ElevationInterfaceWidget::slotClickedPrintButton);

}
void ElevationInterfaceWidget::slotClickedPrintButton(){
    //TODO
    CustomPrintDialog* dialog = new CustomPrintDialog(elevationGrScene);
    if(dialog->exec()){

    }
    return;
}

CustomDrawGrView *ElevationInterfaceWidget::getView() const
{
    return elevationGrView;
}


void ElevationInterfaceWidget::createLayout()
{
    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(frameCombobox, 0);
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
    vlay->addWidget(elevationGrView, 1);

    setLayout(vlay);
}

void ElevationInterfaceWidget::changeDrawingFrame(const QString &fr)
{
    emit drawingFrameChanged(fr);
    resetSceneView();
}

void ElevationInterfaceWidget::resetSceneView()
{
    elevationGrScene->setBackGround(frameCombobox->currentText());
    elevationGrView->initializeEventOperation();
}

void ElevationInterfaceWidget::initializePlanView()
{
    frameCombobox->setCurrentIndex(0);
}

void ElevationInterfaceWidget::sendActiveStatusToScene()
{
    elevationGrScene->sendSelectedItems();
}

} // namespace post3dapp
