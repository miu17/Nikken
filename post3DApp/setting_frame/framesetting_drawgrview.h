#ifndef FRAMESETTINGDRAWGRVIEW_H
#define FRAMESETTINGDRAWGRVIEW_H

#include "customdraw_grview.h"

namespace post3dapp{
class BackGroundFloor;
struct FRAMEPOINTSVALUE;
struct FRAMEGROUPVALUE;

class FrameSettingDrawGrView: public CustomDrawGrView
{
    Q_OBJECT

public:

    explicit FrameSettingDrawGrView(QWidget *parent = nullptr);
    ~FrameSettingDrawGrView();

    void setDrawingData(const QList<FRAMEGROUPVALUE> &glist,
                        const QList<FRAMEPOINTSVALUE> &flist);
    void setCurrentGID(const QUuid &gid);

public slots:

    void fitWindow() override;
    void changeDrawingGID(const QString &);

protected:

    void drawBackground(QPainter *painter, const QRectF &rect) override;

private:


    QList<FRAMEGROUPVALUE> groupList;
    QList<FRAMEPOINTSVALUE> frameList;

    BackGroundFloor *backFloor;
    QString drawingFloor;

    QPointF minPoint;
    QPointF maxPoint;

    void filterGID();

    void calcMaxMinPoint();
    void initializeFittingRect();
    void updateFittingRect(const QPointF &);

};
} // namespace post3dapp

#endif
