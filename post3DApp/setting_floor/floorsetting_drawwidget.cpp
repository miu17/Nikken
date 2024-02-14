#include "floorsetting_drawwidget.h"

#include <QtGui>

#include "floorsetting_drawgrview.h"
#include "parameters.h"
#include "reference_combobox.h"

namespace post3dapp{

FloorSettingDrawWidget::FloorSettingDrawWidget(QWidget *parent)
    : CustomDrawWidget(parent)
{
    createSceneView();
    createButtons();
    createLayout();
}

void FloorSettingDrawWidget::setDrawingData(const QList<FLOORVALUES> &list)
{
    floorGrView->setDrawingData(list);
}

void FloorSettingDrawWidget::createSceneView()
{

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(PARAMETERS::floor_setting_scene_x_origin,
                        PARAMETERS::floor_setting_scene_y_origin,
                        PARAMETERS::floor_setting_scene_width, PARAMETERS::floor_setting_scene_height);

    floorGrView = new FloorSettingDrawGrView(this);
    floorGrView->setScene(scene);
    floorGrView->setMinimumHeight(300);
    floorGrView->setMinimumWidth(100);
    connect( floorGrView, &CustomDrawGrView::scaleChanged,
             this, &CustomDrawWidget::slotChangeSliderValue );

    floorGrView->fitInView(QRectF(0., 0., 5000., 5000.), Qt::KeepAspectRatio);
}

void FloorSettingDrawWidget::createLayout()
{
    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addStretch();
    hlay1->addSpacing(10);
    hlay1->addWidget(zoomOutIcon, 0);
    hlay1->addWidget(zoomSlider, 0);
    hlay1->addWidget(zoomInIcon, 0);
    hlay1->addSpacing(5);
    hlay1->addWidget(fitButton, 0);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->addWidget(floorGrView, 1);
    vlay->addLayout(hlay1, 0);

    setLayout(vlay);

}
CustomDrawGrView* FloorSettingDrawWidget::getView() const
{
    return floorGrView;
}

} // namespace post3dapp
