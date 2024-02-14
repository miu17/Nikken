#include "elevationCI_widget.h"

#include <QtGui>
#include <QProxyStyle>

#include "custom_print_dialog.h"
#include "elevationCI_grscene.h"
#include "elevationCI_grview.h"
#include "reference_combobox.h"
#include "unified_inputdata.h"
#include "unified_settings.h"


namespace post3dapp{

ElevationCopyInterfaceWidget::ElevationCopyInterfaceWidget(UnifiedInputData* input, QWidget *parent)
    : CustomDrawWidget(parent), copy(input)
{
    createSceneView();
    createButtons();
    createLayout();

//    connect(UnifiedSettings::getInstance(), &UnifiedSettings::in2D_SettingChanged,
//            this, &ElevationCopyInterfaceWidget::resetSceneView);
}

void ElevationCopyInterfaceWidget::createSceneView()
{
    elevationGrScene = new ElevationCopyInterfaceGrScene(LIMITSELECTTERM(), copy, UnifiedSettings::getInstance()->getInput2DViewSettings(), this);
    connect(elevationGrScene, &InputGrScene::resetSceneView, this, &ElevationCopyInterfaceWidget::resetSceneView);
    elevationGrView = new ElevationCopyInterfaceGrView(elevationGrScene, this);
    elevationGrView->setMinimumHeight(300);
    elevationGrView->setMinimumWidth(200);
    connect( elevationGrView, &CustomDrawGrView::scaleChanged,
             this,   &CustomDrawWidget::slotChangeSliderValue );

    elevationGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
}

void ElevationCopyInterfaceWidget::createButtons()
{
    CustomDrawWidget::createButtons();

    frameCombobox = new ReferenceCombobox(DATATYPE::TPFRAMEP, UnifiedDataType::FixedInput, this);
    frameCombobox->setDefaultName(u8"（通りの選択）");
    frameCombobox->setFixedWidth(110);

    connect(frameCombobox, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
            this, QOverload<const QString&>::of(&ElevationCopyInterfaceWidget::changeDrawingFrame));

    printButton = new QToolButton(this);
    printButton->setAutoRaise(true);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));
    connect(printButton, &QAbstractButton::clicked,
            this, &ElevationCopyInterfaceWidget::slotClickedPrintButton);

}
void ElevationCopyInterfaceWidget::slotClickedPrintButton(){
    //TODO
    CustomPrintDialog* dialog = new CustomPrintDialog(elevationGrScene);
    if(dialog->exec()){

    }
    return;
}

CustomDrawGrView *ElevationCopyInterfaceWidget::getView() const
{
    return elevationGrView;
}


void ElevationCopyInterfaceWidget::createLayout()
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

void ElevationCopyInterfaceWidget::changeDrawingFrame(const QString &fr)
{
    emit drawingFrameChanged(fr);
    resetSceneView();
}

void ElevationCopyInterfaceWidget::resetSceneView()
{
    elevationGrScene->setBackGround(frameCombobox->currentText());
    elevationGrView->initializeEventOperation();
}

void ElevationCopyInterfaceWidget::initializePlanView()
{
    frameCombobox->setCurrentIndex(0);
}

void ElevationCopyInterfaceWidget::sendActiveStatusToScene()
{
    elevationGrScene->sendSelectedItems();
}

} // namespace post3dapp
