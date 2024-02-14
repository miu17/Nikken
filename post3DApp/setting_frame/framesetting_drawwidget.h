#ifndef FRAMESETTINGDRAWWIDGET_H
#define FRAMESETTINGDRAWWIDGET_H

#include "customdraw_widget.h"

class QComboBox;
class QPushButton;

namespace post3dapp{
struct FRAMEGROUPVALUE;
struct FRAMEPOINTSVALUE;
class FrameSettingDrawGrView;
class FrameSettingDrawWidget : public CustomDrawWidget
{
    Q_OBJECT

public:

    explicit FrameSettingDrawWidget(QWidget *parent = 0);

    void setDrawingData(const QList<FRAMEGROUPVALUE> &groupList,
                        const QList<FRAMEPOINTSVALUE> &frameList);
    void setCurrentGID(const QUuid &gid);

private slots:

    void signalLinkStatus(bool);
    void changeDrawingStatus();

signals:

    void linkStatusChanged(bool);

private:

    FrameSettingDrawGrView *frameGrView;

    QComboBox *floorCombobox;
    QPushButton *linkButton;

    CustomDrawGrView* getView() const override;
    void createSceneView() override;
    void createButtons() override;
    void createLayout() override;

};
} // namespace post3dapp

#endif
