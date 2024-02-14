#ifndef PERSCLSCENE_H
#define PERSCLSCENE_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include <QGLViewer/qglviewer.h>
#include "ascii_table.h"
#include "define_output.h"
#include "define_unifieddata.h"

namespace post3dapp{
enum ELEMENTTYPE;
enum class DATATYPE;
enum class OUTFIGTYPE;
enum class PERSSELECTMODE;
class JointData;
class MemberData;
class PersCalcLoadGLViewer;

class POST3D_CLASS_DLLSPEC PersCalcLoadScene : public QObject
{

    Q_OBJECT

public:

    PersCalcLoadScene( QObject *parent = nullptr );
    ~PersCalcLoadScene();

    void clearSelectedItems();

    QList<JointData *> currentSelectedJoints() const
    {
        return selectedJoints;
    }
    QList<MemberData *> currentSelectedMembers() const
    {
        return selectedMembers;
    }
    bool existSelectedItems() const
    {
        return ( !selectedJoints.isEmpty() || !selectedMembers.isEmpty() );
    }

    void setLimitedFloor( const QString & );
    void setLimitedFrame( const QString & );
    void changeCaseIndex(LoadType::LOADTYPE, LOADSUMTYPE, CALCOUTPUTDIR);

    qglviewer::Vec boundingMinVec()
    {
        return boundMinVec;
    }
    qglviewer::Vec boundingMaxVec()
    {
        return boundMaxVec;
    }

    void drawSolid();
    void drawMemberText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );
    void drawJointText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );
    void drawPoints();
    void drawFrameScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );
    void drawFloorScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );

    void selectPoints(qglviewer::Camera *, const QRect &, PERSSELECTMODE mode);

    void appendSelectedMember(MemberData *);
    void removeSelectedMember(MemberData *);
    void appendSelectedJoint(JointData *);
    void removeSelectedJoint(JointData *);

public slots:

    void clearCurrentData();
    void initDisplayList();

    void createMemberItems(const QList<MemberData *> &, bool updateOn = true);

signals:

    void selectionChanged();

private :

//    PersCalcLoadGLViewer *childViewer;

    AsciiTable ascii_table;

    QUuid limitedFloor;
    QUuid limitedFrame;
    OUTFIGTYPE myFigureType;
    LoadType::LOADTYPE indexA;
    LOADSUMTYPE indexB;
    CALCOUTPUTDIR indexC;

    QList<JointData *> jointList;
    QList<MemberData *> memberList;

    QHash<MemberData *, QList<qglviewer::Vec> > closedMemberTriangles;

    qglviewer::Vec boundMinVec;
    qglviewer::Vec boundMaxVec;

    QList<JointData *> selectedJoints;
    QList<MemberData *> selectedMembers;

    void calcBoundingVec();
    void reloadViewItems();

    bool isMemberOkToDisplayLimited( MemberData *md ) const;
    bool isJointOkToDisplayLimited( JointData *jd ) const;

    QList<qglviewer::Vec> divideClosedMember( MemberData *md ) const;

    void createClosedMember(const QList<qglviewer::Vec> &tplist );

    void drawXZText(const QString &name, qreal swidth, qreal shx, qreal shy);

    QString dummyName(DATATYPE dtype) const;

    void recalcAnalysisJointsOfMember( MemberData * );
};
} // namespace post3dapp
#endif


