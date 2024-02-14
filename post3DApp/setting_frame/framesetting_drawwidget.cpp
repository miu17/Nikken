#include "framesetting_drawwidget.h"

#include <QtGui>

#include "framesetting_drawgrview.h"
#include "parameters.h"
#include "reference_combobox.h"

namespace post3dapp{

FrameSettingDrawWidget::FrameSettingDrawWidget(QWidget *parent)
    : CustomDrawWidget(parent)
{
    createSceneView();
    createButtons();
    createLayout();
}

void FrameSettingDrawWidget::setDrawingData(const QList<FRAMEGROUPVALUE> &groupList, const QList<FRAMEPOINTSVALUE> &frameList)
{
    frameGrView->setDrawingData(groupList, frameList);
}

void FrameSettingDrawWidget::setCurrentGID(const QUuid &gid)
{
    frameGrView->setCurrentGID(gid);
}

void FrameSettingDrawWidget::createSceneView()
{

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(PARAMETERS::scene_x_origin, PARAMETERS::scene_y_origin,
                        PARAMETERS::scene_width, PARAMETERS::scene_height);
    frameGrView = new FrameSettingDrawGrView(this);
    frameGrView->setScene(scene);
    frameGrView->setMinimumHeight(300);
    frameGrView->setMinimumWidth(200);
    connect( frameGrView, &CustomDrawGrView::scaleChanged,
             this, &CustomDrawWidget::slotChangeSliderValue );

    frameGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
}

void FrameSettingDrawWidget::createButtons()
{

    CustomDrawWidget::createButtons();

    linkButton = new QPushButton(u8"描画", this);
    linkButton->setCheckable(true);
    linkButton->setChecked(true);
    linkButton->setFixedWidth(40);

    connect(linkButton, &QAbstractButton::toggled, this, &FrameSettingDrawWidget::signalLinkStatus);

    floorCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::Input, this);
    floorCombobox->setFixedWidth(110);

    connect(floorCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &FrameSettingDrawWidget::changeDrawingStatus);
}

void FrameSettingDrawWidget::createLayout()
{

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->setSpacing(3);
    hlay1->addWidget(linkButton, 0);
    hlay1->addWidget(floorCombobox, 0);
    hlay1->addStretch();
//    hlay1->addSpacing(10);
    hlay1->addWidget(zoomOutIcon, 0);
    hlay1->addWidget(zoomSlider, 0);
    hlay1->addWidget(zoomInIcon, 0);
    hlay1->addSpacing(5);
    hlay1->addWidget(fitButton, 0);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->addWidget(frameGrView, 1);
    vlay->addLayout(hlay1, 0);

    setLayout(vlay);

}

void FrameSettingDrawWidget::signalLinkStatus(bool chk)
{
    emit linkStatusChanged(chk);
}

void FrameSettingDrawWidget::changeDrawingStatus()
{
    frameGrView->changeDrawingGID( floorCombobox->currentText() );
}

CustomDrawGrView *FrameSettingDrawWidget::getView() const
{
    return frameGrView;
}

} // namespace post3dapp
