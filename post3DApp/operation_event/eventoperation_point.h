#ifndef POINTEVENTOPERATION_H
#define POINTEVENTOPERATION_H

#include "eventoperation_abstract.h"
namespace post3dapp{
class ChangePlaneJointDialog;
class JointData;
class JointItem;
class ManagePointActions;
struct PLANEJOINTVALUES;

class PointEventOperation : public AbstractEventOperation
{
    Q_OBJECT

public:

    PointEventOperation(CustomDrawGrView *view);

    void execDrawForeground(QPainter *, const QRect &) override;
    void execMousePress(QMouseEvent * ) override;
    void execMouseRelease(QMouseEvent * ) override;
    void execKeyPress(QKeyEvent *) override;

    void initializeSettings() override;

    void toggleViewMode() override;
    void positionSnap(const QPoint &, bool searchJoint = false) override;
    bool existNearlyPoint(QPoint ) override;
    void changeBatchRect(QPoint, QPoint) override;

    bool isPositionSnapOn() const override;
    bool isBatchInputMode() const override;
    bool isSelectItemMode() const override;

    bool changeSelectedJointsGlobalXYZ(bool, bool, bool, qreal, qreal, qreal) override;
    bool changeSelectedJointsGlobalDelta(bool, bool, bool, qreal, qreal, qreal) override;
    bool changeSelectedJointsLocalXYZ(bool, bool, bool, bool, qreal, qreal, qreal) override;
    bool changeSelectedJointsLocalDelta(bool, bool, bool, bool, qreal, qreal, qreal) override;
    bool changeSelectedJointsAttachment(bool, bool, const QString &, const QString &) override;

public slots:

    void reloadSettings() override;
    void slotChangeActionStatus() override;
    void setWaitingMode() override;

    void refreshBatchPoints();

//    void slotSetSelectWaitingMode();
    void slotSetMoveWaitingMode();
    void slotFindItems();
    void slotDeleteItems();
    void slotDeleteIndependentJoints();
    void slotCopyItems();
    void slotSetPasteWaitingMode();
    void slotShowChangeJointDialog();
    void slotShowAutoAttachDialog();
    void slotChangeSection();
    void slotDivideMember();

private:

    ManagePointActions *manageActions;

    ChangePlaneJointDialog *changeJointDialog;

    bool onMoveWaiting;
    bool onPasteWaiting;

    bool isPointHitMode() const;
    bool isBatchMode() const;
    bool isSelectHitMode() const;
    bool isEditMode() const;

    bool isActionWaiting() const;
    bool isMoveWaiting() const;
    bool isPasteWaiting() const;
    void setMoveWaitingOn(bool);
    void setPasteWaitingOn(bool);

    QList<QPointF> batchPoints;
    QList<QPointF> tmpBatchPoints;

    void selectBatchPoints(const QRectF &);
    void combineBatchPoints();
    void batchPointsToJointItems();
    void selectItemToLoadItem();

    JointItem *copySourceItem;
    JointData *moveSourceJoint;
    bool setWaitingPoints( QPoint );
//    QList<QPointF> waitingPoints;
//    QList<JOINTGROUP> waitingLines;

    void moveJoints();
    void pasteJoints();
    void moveMemberJoints();

    PLANEJOINTVALUES getChangeJointDialogValues( const QList<GlobalPoint> & ) const;

};
} // namespace post3dapp
#endif

