#ifndef FLOORSETTINGDRAWWIDGET_H
#define FLOORSETTINGDRAWWIDGET_H

#include "customdraw_widget.h"

class QComboBox;

namespace post3dapp{
struct FLOORVALUES;
class CustomDrawGrView;
class FloorSettingDrawGrView;

class FloorSettingDrawWidget : public CustomDrawWidget
{
    Q_OBJECT

public:

    explicit FloorSettingDrawWidget(QWidget *parent = 0);

    void setDrawingData(const QList<FLOORVALUES> &list);

private slots:

private:

    FloorSettingDrawGrView *floorGrView;

    CustomDrawGrView* getView() const override;
    void createSceneView() override;
    void createLayout() override;

};
} // namespace post3dapp

#endif
