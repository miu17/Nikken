#ifndef EDITALLEVENTOPERATION_H
#define EDITALLEVENTOPERATION_H

#include "eventoperation_abstract.h"
namespace post3dapp{
class JointItem;
class ManageEditAllActions;
class ChangePlaneJointDialog;
struct PLANEJOINTVALUES;

class EditAllEventOperation : public AbstractEventOperation
{
    Q_OBJECT

public:

    EditAllEventOperation(CustomDrawGrView *view);

    void execDrawForeground(QPainter *, const QRect &) override;
    void execMousePress(QMouseEvent * ) override;
    void execMouseRelease(QMouseEvent * ) override;
    void execKeyPress(QKeyEvent *) override;

    void initializeSettings() override;

    void toggleViewMode() override;

    void positionSnap(const QPoint &cur_p, bool searchJoint) override;
    bool existNearlyPoint(QPoint ) override;

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

    void slotFindItems();
    void slotDeleteItems();
    void slotSetMoveWaitingMode();
    void slotCopyItems();
    void slotSetPasteWaitingMode();

    void slotChangeSection();
    void slotReverseMemberJoint();
    void slotChangeAngle();
    void slotChangeShift();
    void slotChangeFace();

    void slotDeleteIndependentJoints();
    void slotShowChangeJointDialog();
    void slotShowAutoAttachDialog();

    void slotChangeWallOpen();
    void slotDeleteWallOpen();


private:

    ManageEditAllActions *manageActions;

    ChangePlaneJointDialog *changeJointDialog;

    bool onMoveWaiting;
    bool onPasteWaiting;

    GlobalPoint firstHitGP;
    JointItem *copySourceItem;

    bool setWaitingPoints( QPoint );

    bool isMoveWaiting() const;
    void setMoveWaitingOn(bool);
    bool isPasteWaiting() const;
    void setPasteWaitingOn(bool);

    void moveJoints();
    void pasteCopyBuffer();

    PLANEJOINTVALUES getChangeJointDialogValues( const QList<GlobalPoint> & ) const;

};
} // namespace post3dapp
#endif

