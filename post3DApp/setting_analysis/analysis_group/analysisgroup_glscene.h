#ifndef ANALYSISGROUPGLSCENE_H
#define ANALYSISGROUPGLSCENE_H

#include <QUuid>
#include "ascii_table.h"
#include "define_glsetting.h"

namespace post3dapp{

enum class DATATYPE;
enum ELEMENTTYPE;
class JointData;
class MemberData;
class AnalysisGroupGLViewer;

class AnalysisGroupGLScene : public QObject
{

    Q_OBJECT

public:

    AnalysisGroupGLScene( QObject* parent=0 );
    ~AnalysisGroupGLScene();

    void setSelectedItems( const QList<JointData*>&, const QList<MemberData*> & );
    void clearSelectedItems();

    QList<JointData*> currentSelectedJoints() const {return selectedJoints; }
    QList<MemberData*> currentSelectedMembers() const {return selectedMembers; }

    void setLimitedFloor( const QString & );
    void setLimitedFrame( const QString & );
    void setLimitedElement( ELEMENTTYPE );

    qglviewer::Vec boundingMinVec(){ return boundMinVec; }
    qglviewer::Vec boundingMaxVec(){ return boundMaxVec; }

    void drawSolid( bool withname );
    void drawMemberText( const qglviewer::Vec& v_r, const qglviewer::Vec& v_u );
    void drawJointText( const qglviewer::Vec& v_r, const qglviewer::Vec& v_u );
    void drawPoints();
    void drawFrameScale( const qglviewer::Vec& v_r, const qglviewer::Vec& v_u );
    void drawFloorScale( const qglviewer::Vec& v_r, const qglviewer::Vec& v_u );

    void selectPoints(qglviewer::Camera*, const QRect&, PERSSELECTMODE mode);

    void appendSelectedMember(MemberData*);
    void removeSelectedMember(MemberData*);
    void appendSelectedJoint(JointData*);
    void removeSelectedJoint(JointData*);

public slots:

    void clearCurrentData();
    void initDisplayList();

    void createJointItems(const QList<JointData*>&);
    void removeJointItems(const QList<JointData*>&);
    void changeJointItems(const QList<JointData*>&);

    void createMemberItems(const QList<MemberData*>&,bool updateOn=true);
    void removeMemberItems(const QList<MemberData*>&,DATATYPE,bool updateOn=true);
    void changeMemberItems(const QList<MemberData*>&);

signals:

    void selectionChanged();

private :
    ELEMENTTYPE myElementType;
    AsciiTable ascii_table;

    QUuid displayLimitedFloor;
    QUuid displayLimitedFrame;

    QList<JointData*> jointList;
    QList<MemberData*> memberList;

    QHash<MemberData*,QList<qglviewer::Vec> > closedMemberTriangles;

    qglviewer::Vec boundMinVec;
    qglviewer::Vec boundMaxVec;

    QList<JointData*> selectedJoints;
    QList<MemberData*> selectedMembers;

    void calcBoundingVec();
    void reloadViewItems();

    bool isMemberOkToDisplayLimited( MemberData* md ) const;
    bool isJointOkToDisplayLimited( JointData* jd ) const;

    QList<qglviewer::Vec> divideClosedMember( MemberData* md ) const;

    void createLineMember(const QList<qglviewer::Vec> &plist );
    void createClosedMember(const QList<qglviewer::Vec> &tplist );

    void drawXZText(const QString& name, qreal swidth, qreal shx, qreal shy);

    ELEMENTTYPE dataTypeToElementType(DATATYPE dtype) const;
    QString dummyName(DATATYPE dtype) const;

    void recalcAnalysisJointsOfMember( MemberData* );

    // カラー設定
    const bool defaultLineVisible = true;
    const bool defaultPlaneVisible = true;
    const QColor defaultLineColor = Qt::white;
    const QColor defaultPlaneColor = Qt::blue;
    const QColor defaultSelectColor = Qt::green;
    const bool defaultIsNameVisible = true;
    const int defaultNameSize = 4;
    const QColor defaultNameColor = Qt::blue;
    const bool defaultIsJointNumberVisible = true;
    const QColor defaultJnoColor = Qt::yellow;
    const int defaultJnoSize = 4;
    const bool defaultIsJoinVisible = true;
    const QColor defaultJointColor = Qt::darkRed;
    const bool defaultIsFrScaleVisible = true;
    const QColor defaultScaleColor = Qt::darkMagenta;
    const bool defaultIsFlScaleVisible = true;
    const QPointF defaultFloorScaleXY = QPointF(-5.0, -5.0);
};
} // namespace post3dapp

#endif


