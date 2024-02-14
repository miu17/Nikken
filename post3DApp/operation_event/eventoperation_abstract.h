#ifndef ABSTRACTEVENTOPERATION_H
#define ABSTRACTEVENTOPERATION_H

#include <QObject>
#include "define_settings.h"
#include "global_point.h"

class QMouseEvent;
namespace post3dapp{
enum class DATATYPE;
class CustomDrawGrView;
class InputGrScene;
class JointData;


class AbstractEventOperation : public QObject
{
    Q_OBJECT

public:

    AbstractEventOperation(CustomDrawGrView *view);

    virtual void execDrawForeground(QPainter *, const QRect &);
    virtual void execDrawBackground(QPainter *, const QRect &);
    virtual void execMousePress(QMouseEvent * );
    virtual void execMouseRelease(QMouseEvent * );
    virtual void execMouseMove(QMouseEvent *);
    virtual void execKeyPress(QKeyEvent *);
    virtual void execDoubleClick(QMouseEvent *) { }

    virtual void initializeSettings();
    virtual void setWaitingMode();
    virtual void setViewModeOn(bool ok = true);
    virtual void toggleViewMode();

    virtual bool existNearlyPoint(QPoint )
    {
        return false;
    }
    virtual void changeBatchRect(QPoint, QPoint ) { }
    virtual void changePosition(const QPoint &);
    virtual void positionSnap(const QPoint &, bool searchJoint = false);
    virtual QPointF currentPosition() const;

    virtual void hideRubberBand(bool );
    virtual bool isHiddenRubberBand() const;

    virtual bool isViewModeOn() const;
    virtual bool isPositionSnapOn() const;
    virtual bool isBatchInputMode() const;
    virtual bool isSelectItemMode() const;

    CustomDrawGrView *view() const;
    InputGrScene *scene() const;

    virtual bool changeSelectedJointsGlobalXYZ(bool, bool, bool, qreal, qreal, qreal)
    {
        return false;
    }
    virtual bool changeSelectedJointsGlobalDelta(bool, bool, bool, qreal, qreal, qreal)
    {
        return false;
    }
    virtual bool changeSelectedJointsLocalXYZ(bool, bool, bool, bool, qreal, qreal, qreal)
    {
        return false;
    }
    virtual bool changeSelectedJointsLocalDelta(bool, bool, bool, bool, qreal, qreal, qreal)
    {
        return false;
    }
    virtual bool changeSelectedJointsAttachment(bool, bool, const QString &, const QString &)
    {
        return false;
    }

public slots:

    virtual void reloadSettings();
    virtual void slotChangeActionStatus() { }

protected:

    CustomDrawGrView *myGrView;
    InputGrScene *myGrScene;

    ELEMENTTYPE myElementType;
    ADDEDITMODE currentAddEditMode;

    bool viewMode;
    bool mousePressed;
    bool onRubberHide;
    Qt::KeyboardModifiers currentModifier;

    GlobalPoint currentGP;
    QString positionText;

    virtual bool isMousePressed() const;
    virtual void mousePressOn(bool ok = true);

    void searchNearerCrossPoint(QPointF, GlobalPoint &, qreal &, qreal offset = 0.0);
    void searchNearerDefinePoint(QPointF, GlobalPoint &, qreal &);
    void searchNearerGridPoint(QPointF, int, GlobalPoint &, qreal &);

    void changePositionText();
    QList<GlobalPoint> scenePointsToGlobalPoints(const QList<QPointF> &);

    bool warningJointBatchInput();
    bool warningMemberBatchInput();
    bool warningLoadBatchInput();
    DATATYPE currentDataType() const;

    void divideMembersByJoints(const QList<JointData *> &, ADDMOVEACTION );

    void showInformationMessage(const QString &);
    void showWarningMessage(const QString &);
    void warningInvalidValue();

    UnifiedDataType isOutput;
};
} // namespace post3dapp
#endif
