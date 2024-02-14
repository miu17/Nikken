#ifndef PERSINTERFACESCENE_H
#define PERSINTERFACESCENE_H

#include <QUuid>
#include "ascii_table.h"
#include "define_glsetting.h"
#include "define_input3dsettings.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
enum class DATATYPE;
enum class RENDERSHAPE;
class JointData;
class MemberData;
class SpringData;
struct XYZ;
struct LIMITSELECTTERM;

class POST3D_CLASS_DLLSPEC PersInterfaceScene : public QObject
{

    Q_OBJECT

public:

    PersInterfaceScene(ELEMENTTYPE, ADDEDITMODE, const Input3DViewSettings&, QObject *parent = 0 );
    ~PersInterfaceScene();

    bool isNotInitialized() const;

    qglviewer::Vec boundingMinVec() const
    {
        return boundMinVec;
    }
    qglviewer::Vec boundingMaxVec() const
    {
        return boundMaxVec;
    }

    void drawSolid( bool withname, bool useDisplayList, DRAWINGSTYLE dstyle, bool simplified = false );
    void drawMemberText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );
    void drawJointText( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );
    void drawGrid( qreal ox, qreal oy, qreal oz, qreal pitch, int cx, int cy );
    void drawPoints();
    void drawFrameScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );
    void drawFloorScale( const qglviewer::Vec &v_r, const qglviewer::Vec &v_u );

    void removeSelectedMemberID(GLuint id);
    void removeSelectedMember(MemberData *);
    void clearSelectedItems();

    DATATYPE currentDataType() const;
    QList<JointData *> getSceneJoints() const
    {
        return jointList;
    }
    QList<MemberData *> getSceneMembers() const
    {
        return memberList;
    }

    void setJointSelect( const QList<JointData *> & );
    void setMemberSelect( const QList<MemberData *> & );

    QList<DATATYPE> selectedDataTypes() const;

    QStringList selectedJointsXYZStrings() const;  // 節点数,X,Y,Z の4文字列　1文字列なら対象無し
    QString selectedMembersCodeAngle() const; // コードアングル　の文字列　空なら対象無し
    QStringList selectedMembersShiftValue() const; // shiftY,shiftZ の文字列　空なら対象無し
    QStringList selectedMembersFaceValue() const; // iFaceRule,iFaceLen,jFaceRule,jFaceLen

//    bool changeSelectedJointsAttachment(bool,bool,const QString& ,const QString&);

    void importCurrentStatus(ELEMENTTYPE, ADDEDITMODE);
    QList<JointData *> getSelectedJoints() const{return selectedJoints;}
    void appendSelectedJoint(JointData* jd);
    void removeSelectedJoint(JointData* jd);
    QList<MemberData *> getSelectedMembers() const{return selectedMembers;}
    void appendSelectedMember(MemberData* md);
    QList<SpringData *> getSelectedSprings() const{return selectedSprings;}
    QList<GLuint> getMemberDisplayList() const{return memberDisplayList;}

public slots:

    void initializeSelectItems();
    void clearCurrentData();
    void initDisplayList();

    void updateViewSettings();

signals:
    void dataUpdated();
    void selectedItemChanged();

private :
    Input3DViewSettings viewSettings;

    ELEMENTTYPE myElementType;
    ADDEDITMODE currentAddEditMode;
    AsciiTable ascii_table;

    QList<JointData *> jointList;
    QList<MemberData *> memberList;
    QList<INDEXPAIR> memberIndexList;
    QList<GLuint> memberDisplayList;

    QHash<MemberData *, ELEMENTSTATE> memberStates;
    QHash<MemberData *, QList<qglviewer::Vec> > closedMemberTriangles;

    qglviewer::Vec boundMinVec;
    qglviewer::Vec boundMaxVec;

    QList<JointData *> selectedJoints;
    QList<MemberData *> selectedMembers;
    QList<SpringData *> selectedSprings;

//    bool oneTime_useNoDisplayList;

    bool isMemberOkToDisplayLimited( MemberData *md ) const;
    bool isJointOkToDisplayLimited( JointData *jd ) const;

    ELEMENTSTATE checkLineMemberState( MemberData *md ) const;
    ELEMENTSTATE checkClosedMemberState( MemberData *md ) const;
    QList<qglviewer::Vec> divideClosedMember( MemberData *md ) const;

    void calcBoundingVec();

    void setLocalMatrix(qreal x1,  qreal y1,  qreal z1,
                        qreal x2,  qreal y2,  qreal z2,
                        qreal h0,  qreal h1,  qreal shy, qreal shz,
                        qreal angle, DATATYPE dtype , bool isVertical);

    void createLineMember(DATATYPE dtype, const QString &sectionName, ELEMENTSTATE state,
                          RENDERSHAPE stype,
                          qreal l1, qreal l2, qreal bb, qreal hh, DRAWINGSTYLE drtype,
                          bool isSelected, bool simplified = false ,qreal bbt=0.0,qreal hht=0.0);

    void createClosedMember(DATATYPE dtype, const QString &sectionName, ELEMENTSTATE state,
                            DRAWINGSTYLE drtype,
                            const QList<qglviewer::Vec> &plist, const QList<qglviewer::Vec> &tplist,
                            bool isSelected );

    void drawHShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawHShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawIShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawIShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawT1ShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawT1ShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawT2ShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawT2ShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawCShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawCShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawLShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawLShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawBShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawBShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawPShapeSolid( qreal l1, qreal l2, qreal dd, bool isColumn, bool simplified = false );
    void drawPShapeWire( qreal l1, qreal l2, qreal dd, bool isColumn, bool simplified = false );
    void drawCrossShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawCrossShapeWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawCrossHShapeSolid( qreal l1, qreal l2, qreal bb, qreal hh,qreal bbt,qreal hht, bool isColumn );
    void drawCrossHShapeWire( qreal l1, qreal l2, qreal bb, qreal hh,qreal bbt,qreal hht, bool isColumn );
//    void drawTriangleFromPlane( const QList<qglviewer::Vec> &plist );

    void drawXZText(const QString &name, qreal swidth, qreal shx, qreal shy);

    void setCurrentLineColor( bool isSelected, DATATYPE dtype, const QString &sectionName,
                              ELEMENTSTATE estate );
    void setCurrentSolidColor( bool isSelected, DATATYPE dtype, const QString &sectionName,
                               ELEMENTSTATE estate );

    ELEMENTTYPE dataTypeToElementType(DATATYPE dtype) const;
    QString dummyName(DATATYPE dtype) const;
    void setElementSolidColor(DATATYPE dtype);
    void setElementLineColor(DATATYPE dtype);
    void setStateSolidColor(ELEMENTSTATE state);
    void setStateLineColor(ELEMENTSTATE state);

    void createJointItems(const QList<JointData *> &);
    void removeJointItems(const QList<JointData *> &);
    void changeJointItems(const QList<JointData *> &);

    void createMemberItems(const QList<MemberData *> &);
    void removeMemberItems(const QList<MemberData *> &, DATATYPE);
    void changeMemberItems(const QList<MemberData *> &);


};
} // namespace post3dapp
#endif


