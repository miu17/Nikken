#ifndef ANALYSISGROUPGLVIEWER_H
#define ANALYSISGROUPGLVIEWER_H

#include "analysisgroup_glscene.h"
namespace post3dapp{

class AnalysisGroupGLViewer : public QGLViewer
{

    Q_OBJECT

public:

    AnalysisGroupGLViewer( AnalysisGroupGLScene* s, QWidget* parent );

    void clearFloorAttachment();
    bool appendFloorAttachment(const QString &);
    void clearFrameAttachment();
    bool appendFrameAttachment(const QString &);

    void changeViewSettings(const PERSANALYSISSETTINGS &settings);

    void setLimitedElement(ELEMENTTYPE type){myScene->setLimitedElement(type);}
    void setSelectedJointItems(const QList<JointData*> &joints);
    void setSelectedMemberItems(const QList<MemberData*> &members);
    QList<JointData*> currentSelectedJoints()const {return myScene->currentSelectedJoints();}
    QList<MemberData*> currentSelectedMembers()const{return myScene->currentSelectedMembers();}

public slots:

    void zoomIn();
    void zoomOut();
    void fitWindow();
    void slotClearSelectedItems();

    void slotChangeLimitedFloor(const QString &);
    void slotChangeLimitedFrame(const QString &);

signals:

protected :

    void init() override;
    void draw() override;
    void fastDraw() override;
    void drawWithNames() override;
    void postSelection(const QPoint& point) override;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private:

    AnalysisGroupGLScene* myScene;
    QRect selectRect;

    PERSANALYSISSETTINGS myViewSettings;
    PERSSELECTMODE selectMode;

    void drawSelectionRectangle() const;

};
} // namespace post3dapp

#endif

