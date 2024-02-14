#ifndef LINEEVENTOPERATION_H
#define LINEEVENTOPERATION_H

#include "eventoperation_abstract.h"
#include "manage_jointdata.h"

namespace post3dapp{
struct GLOBALLINE;
class ManageLineActions;
class CustomDrawHLineItem;

class LineEventOperation : public AbstractEventOperation
{
    Q_OBJECT

public:

    LineEventOperation(CustomDrawGrView *view);


    void execDrawForeground(QPainter *, const QRect &) override;
    void execMousePress(QMouseEvent * ) override;
    void execMouseRelease(QMouseEvent * ) override;
    void execKeyPress(QKeyEvent *) override;

    void initializeSettings() override;

    void toggleViewMode() override;

    void positionSnap(const QPoint &cur_p, bool searchJoint) override;
    bool existNearlyPoint(QPoint ) override;
    void changeBatchRect(QPoint, QPoint) override;

    bool isPositionSnapOn() const override;
    bool isBatchInputMode() const override;
    bool isSelectItemMode() const override;

public slots:

    void reloadSettings() override;
    void slotChangeActionStatus() override;
    void setWaitingMode() override;

    void refreshBatchLines();
    void refreshBeamInputStatus();

    void slotSetMoveWaitingMode();
    void slotFindItems();
    void slotDeleteItems();
    void slotChangeSection();
    void slotReverseMemberJoint();
    void slotUniteMembers();
    void slotChangeAngle();
    void slotChangeShift();
    void slotChangeFace();
    void slotDivideMember();

private:

    ManageLineActions *manageActions;

    bool onMoveWaiting;
    bool onePointHit;
    GlobalPoint firstHitGP;

    bool isPointHitMode() const;
    bool isBatchMode() const;
    bool isSelectHitMode() const;
    bool isEditMode() const;
    bool isBeamInputMode() const;

    bool isMoveWaiting() const;
    bool isPlanOneHit() const;
    void setMoveWaitingOn(bool);

    QList<GLOBALLINE> batchLines;
    QList<GLOBALLINE> tmpBatchLines;

    void selectBatchLines(const QRectF &);
    void combineBatchLines();
    void batchLinesToMemberItems();
    void selectItemToLoadItem();

    bool oneLineItemHit;
    CustomDrawHLineItem *hitLineItem;

    bool setMoveWaitingJoints( QPoint );
    JOINTGROUP waitingJoints;
    void moveMemberJoints();

    void appendEquallyBeam(const QPoint &);
    void appendParallelBeam(const QPoint &);
    void appendVerticalBeam(const QPoint &);
    void appendExtendBeam(const QPoint &);

};
} // namespace post3dapp
#endif

