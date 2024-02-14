#ifndef PLANEEVENTOPERATION_H
#define PLANEEVENTOPERATION_H

#include "eventoperation_abstract.h"
#include "manage_jointdata.h"

namespace post3dapp{
enum TARGETACTION;
class ManagePlaneActions;

class PlaneEventOperation : public AbstractEventOperation
{
    Q_OBJECT

public:

    PlaneEventOperation(CustomDrawGrView *view);

    void execDrawForeground(QPainter *, const QRect &) override;
    void execMousePress(QMouseEvent * ) override;
    void execMouseRelease(QMouseEvent * ) override;
    void execKeyPress(QKeyEvent *) override;
    void execDoubleClick(QMouseEvent *) override;

    void initializeSettings() override;

    void toggleViewMode() override;
    void changePosition(const QPoint &) override;
    bool existNearlyPoint(QPoint ) override;
    void changeBatchRect(QPoint, QPoint) override;

    bool isPositionSnapOn() const override;
    bool isBatchInputMode() const override;
    bool isSelectItemMode() const override;

public slots:

    void reloadSettings() override;
    void slotChangeActionStatus() override;
    void setWaitingMode() override;

    void slotSetMoveWaitingMode();
    void slotFindItems();
    void slotDeleteItems();
    void slotChangeSection();
    void slotReverseMemberJoint();
    void slotChangeShift();

    void slotChangeWallOpen();
    void slotDeleteWallOpen();

private:

    ManagePlaneActions *manageActions;

    bool onMoveWaiting;
    QList<GlobalPoint> previousHitGP;

    TARGETACTION currentTarget;
    QList<QPolygonF> batchPlanes;
    QList<QPolygonF> tmpBatchPlanes;

    void createMemberFromPoints();

    bool setMoveWaitingJoints( QPoint );
    JOINTGROUP waitingJoints;
    void moveMemberJoints();

    bool isPointHitMode() const;
    bool isBatchMode() const;
    bool isSelectHitMode() const;
    bool isOneHitMode() const;
    bool isEditMode() const;

    bool isMoveWaiting() const;
    void setMoveWaitingOn(bool);

    void refreshBatchPlanes();
    void selectBatchPlanes(const QRectF &);
    void combineBatchPlanes();
    void batchPlanesToPlaneItems(bool messageOn = true);
    void selectItemToLoadItem();
};
} // namespace post3dapp
#endif

