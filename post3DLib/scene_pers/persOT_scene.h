#ifndef PERSOUTPUTSCENE_H
#define PERSOUTPUTSCENE_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include <QGLViewer/qglviewer.h>
#include "ascii_table.h"
#include "define_draw_figure.h"

namespace post3dapp{
enum class OUTFIGTYPE;
enum class PERSSELECTMODE;
class JointData;
class MemberData;
class SpringData;
class PersOutputGLViewer;

class POST3D_CLASS_DLLSPEC PersOutputScene : public QObject
{

    Q_OBJECT

public:

    PersOutputScene( QObject *parent = 0 );
    ~PersOutputScene();

    void setChildViewer( PersOutputGLViewer *viewer );
    void clearSelectedItems();

    QList<JointData *> currentSelectedJoints() const
    {
        return selectedJoints;
    }
    QList<MemberData *> currentSelectedMembers() const
    {
        return selectedMembers;
    }
    QList<SpringData *> currentSelectedSprings() const
    {
        return selectedSprings;
    }
    bool existSelectedItems() const
    {
        return ( !selectedJoints.isEmpty() || !selectedMembers.isEmpty()|| !selectedSprings.isEmpty() );
    }

    void setLimitedFloor( const QString & );
    void setLimitedFrame( const QString & );
    void setFigureType( OUTFIGTYPE );
    void changeCaseIndex(int, int, int);

    OUTFIGTYPE getOutFigureType() const
    {
        return myFigureType;
    }

    qglviewer::Vec boundingMinVec() const
    {
        return boundMinVec;
    }
    qglviewer::Vec boundingMaxVec() const
    {
        return boundMaxVec;
    }

    void drawSolid();
    void drawMemberText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );
    void drawJointText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );
    void drawPoints();
    void drawFrameScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );
    void drawFloorScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );

    void drawOutputScale();

    void selectPoints(qglviewer::Camera *, const QRect &, PERSSELECTMODE mode);

    void appendSelectedMember(MemberData *);
    void removeSelectedMember(MemberData *);
    void appendSelectedJoint(JointData *);
    void removeSelectedJoint(JointData *);
    FIGURE_TYPE_PROPERTY getFigureTypeProperty() const{return figureTypeProperty;}

public slots:

    void clearCurrentData();
    void initDisplayList();

    void createJointItems(const QList<JointData *> &);
    void removeJointItems(const QList<JointData *> &);
    void changeJointItems(const QList<JointData *> &);

    void createMemberItems(const QList<MemberData *> &, bool updateOn = true);
    void removeMemberItems(const QList<MemberData *> &, DATATYPE, bool updateOn = true);
    void changeMemberItems(const QList<MemberData *> &);

signals:

    void selectionChanged();

private :

    PersOutputGLViewer *childViewer;

    AsciiTable ascii_table;

    QUuid limitedFloor;
    QUuid limitedFrame;
    OUTFIGTYPE myFigureType;
    FIGURE_TYPE_PROPERTY figureTypeProperty;
    int caseIndexA;
    int caseIndexB;
    int caseIndexC;

    QList<JointData *> jointList;
    QList<MemberData *> memberList;

    QHash<MemberData *, QList<qglviewer::Vec> > closedMemberTriangles;

    qglviewer::Vec boundMinVec;
    qglviewer::Vec boundMaxVec;

    QList<JointData *> selectedJoints;
    QList<MemberData *> selectedMembers;
    QList<SpringData *> selectedSprings;

    void calcBoundingVec();
    void reloadViewItems();

    bool isMemberOkToDisplayLimited( MemberData *md ) const;
    bool isJointOkToDisplayLimited( JointData *jd ) const;

    QList<qglviewer::Vec> divideClosedMember( MemberData *md ) const;

    void createLineMember(const QList<qglviewer::Vec> &plist );
    void createClosedMember(const QList<qglviewer::Vec> &tplist );

    void drawXZText(const QString &name, qreal swidth, qreal shx, qreal shy);

    ELEMENTTYPE dataTypeToElementType(DATATYPE dtype) const;
    QString dummyName(DATATYPE dtype) const;

    void recalcAnalysisJointsOfMember( MemberData * );

};
} // namespace post3dapp
#endif


