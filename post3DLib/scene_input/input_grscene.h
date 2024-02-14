#ifndef INPUTGRSCENE_H
#define INPUTGRSCENE_H

#include <QUuid>
#include "customdraw_grscene.h"
#include "define_input2dsettings.h"
#include "define_unifieddata.h" // LIMITSELECTTERMが実体

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
enum ELEMENTTYPE;
enum class DATATYPE;
enum class ITEMDIVRULE;
enum class SCENETYPE;
class CustomDrawItem;
class CustomDrawHLineItem;
class CustomDrawHPlaneItem;
class CustomDrawVLineItem;
class CustomDrawVPlaneItem;
class GlobalPoint;
class JointData;
class JointItem;
class JointLoadItem;
class MemberData;
class SpringData;
struct JOINTGROUP;
struct GLOBALLINE;

class POST3D_CLASS_DLLSPEC InputGrScene : public CustomDrawGrScene
{
    Q_OBJECT

public:

    InputGrScene(LIMITSELECTTERM term, SCENETYPE stype, const Input2DViewSettings& setting, bool _syncSetting,  QObject *parent = nullptr);
    ~InputGrScene();

    /* BackGround に関する処理 */
    virtual void setBackGround(const QString &, const QString &) = 0;
    virtual void drawBackGround(QPainter * ) = 0;

    virtual QList<GlobalPoint> getCrossPoints() const
    {
        return QList<GlobalPoint>();
    }
    virtual QList<GlobalPoint> getDefinePoints() const
    {
        return QList<GlobalPoint>();
    }
    QList<GLOBALLINE> getBatchInputLines() const;
    QList<QPolygonF> getBatchInputPlanes() const;

    virtual void setCurrentFrames(const QList<QUuid> &) = 0;
    virtual void setCurrentFloors(const QList<QUuid> &) {}
    virtual void clearCurrentAttach() = 0;

    virtual QUuid floorID() const
    {
        return QUuid();    // PLAN only
    }
    virtual QUuid frameID() const
    {
        return QUuid();    // ELEVATION only
    }
    virtual qreal floorHeight() const
    {
        return 0.0;    // PLAN only
    }
    virtual void completeGlobalXYZ(GlobalPoint &) = 0;

    SCENETYPE sceneType()
    {
        return mySceneType;
    }

    virtual void setStatus(const QUuid &uid ) = 0;

    QList<JointData *> selectedJoints(ELEMENTTYPE) const;
    QList<SpringData *> selectedSprings() const;
    MemberData *selectedMember() const;
    QList<MemberData *> selectedMembers() const;
    QList<GlobalPoint> selectedPoints() const;

    void setJointSelect( const QList<JointData *> & );
    void setMemberSelect( const QList<MemberData *> & );

    bool isColumnSelected() const;
    bool isWallSelected() const;
    bool isWallOpenSelected() const;

    bool isExistedJoint( JointData * ) const;
    bool isExistedMember( MemberData * ) const;


    QList<QLineF> getMoveWaitingLines(QGraphicsItem *b_item, QPointF, ELEMENTTYPE);
    JOINTGROUP selectedMemberJoints(QPointF) const;

    void searchNearerJointItem(QPointF, GlobalPoint &, qreal &);
    JointData *searchNearlyJoint(QPointF, qreal eps = 1.0);
    QList<JointData *> searchBetweenJoints(JointData *, JointData * );
    QList<JointData *> searchBetweenJoints(const QList<JointData *> & );
    QList<JointData *> searchBetweenPoints(QPointF, QPointF );

    bool searchDivideMemberPoint(QPointF, QPointF &, qreal &, bool, int, bool, int, int);
    QList<MemberData *> searchContainMembers(JointData *, bool exceptJoint = false);
    QList<MemberData *> searchContainMembers(QPointF, bool exceptJoint = false);

    void setItemSelectable(DATATYPE, bool ok = true);
    void setInputItemSelectable(DATATYPE);
    virtual void prepareBatchInputLines(DATATYPE) = 0;
    virtual void prepareBatchInputPlanes(bool) = 0;

    // 全編集時の選択設定
    void setItemSelectLimited(LIMITSELECTTERM);
    void setMemberSelectLimited( const QList<MemberData *> &mdlist );

    JointData *jointItemToJointData(JointItem *item) const
    {
        return ( jointAndPoint.contains(item) ) ? jointAndPoint.value(item) : nullptr ;
    }

    /* 2つ目の引数をtrueの場合、通り起点のJointItemを得る（JointItemが複数の場合） */
    JointItem *jointDataToJointItem(JointData *jd, bool isFirst = true) const;
    /* 2つ目の引数に近いJointItemを得る（JointItemが複数の場合） */
    JointItem *jointDataToJointItem(JointData *jd, QPointF) const;

    /* 2つ目の引数が無い場合は通り起点のitemPositionを、指定点がある場合はそれに近いitemPositionを返す */
    QPointF itemPosition(JointData *jd) const;
    QPointF itemPosition(JointData *jd, QPointF) const;

    JointData *jointLoadItemToJointData(JointLoadItem *item) const;

    QList<QPointF> itemToDividedPointList(CustomDrawItem *, ITEMDIVRULE, qreal, qreal r2 = 0.0) const;

    CustomDrawHLineItem *searchNearlyHItem(QPointF pp, qreal min_len = 1.0) const;
    QList<QPointF> searchCrossPointsToLine(const QLineF &) const;

    QPolygonF searchContainPlane(QPointF pp) const;
    QList<JointData *> polygonToBatchJoints(const QPolygonF poly) const;

//    virtual void exportRenderBackGround(QPainter *painter, int nw, int nh, QSizeF unitPaperSize,
//                                 qreal scale = 1.0, QPointF origin=QPointF(0,0),int dpi = 72) = 0;
//    virtual void exportRenderItems(QPainter *painter, int nw, int nh, QSizeF unitPaperSize,
//                           qreal scale = 1.0, QPointF origin=QPointF(0,0), int dpi = 72) = 0;

    Input2DViewSettings* getViewSettings() {return &viewSettings;}
public slots:

    virtual void resetItems() = 0;

    virtual void createJointItems(const QList<post3dapp::JointData *> &) = 0;
    virtual void removeJointItems(const QList<post3dapp::JointData *> &);
    virtual void changeJointItems(const QList<post3dapp::JointData *> &);

    virtual void filterCreateMember(const QList<post3dapp::MemberData *> &);
    virtual void filterRemoveMember(const QList<post3dapp::MemberData *> &, post3dapp::DATATYPE);
    virtual void filterChangeMember(const QList<post3dapp::MemberData *> &);

//    void sendSelectedItems();
    void updateViewSettings();
    void slotSelectTermChanged(const LIMITSELECTTERM&);
    void slotOperationSettiingChanged(const OPERATIONSETTING&);

signals:

    void itemSelectChanged();
    void resetSceneView();
    void sendSelectedItems();

protected:
    Input2DViewSettings viewSettings;

    SCENETYPE mySceneType;
    LIMITSELECTTERM myselectTerm;
    ELEMENTTYPE myElementType;
    ADDEDITMODE myMode;


    QHash<JointItem *, JointData *> jointAndPoint;
    QHash<CustomDrawHLineItem *, MemberData *> memberAndHGirder;
    QHash<CustomDrawVLineItem *, MemberData *> memberAndVGirder;
    QHash<CustomDrawHLineItem *, MemberData *> memberAndHColumn;
    QHash<CustomDrawVLineItem *, MemberData *> memberAndVColumn;
    QHash<CustomDrawVLineItem *, MemberData *> memberAndOffVColumn;
    QHash<CustomDrawHLineItem *, MemberData *> memberAndHBrace;
    QHash<CustomDrawVLineItem *, MemberData *> memberAndVBrace;
    QHash<CustomDrawHLineItem *, MemberData *> memberAndHBeam;
    QHash<CustomDrawVLineItem *, MemberData *> memberAndVBeam;
    QHash<CustomDrawHPlaneItem *, MemberData *> memberAndHSlab;
    QHash<CustomDrawVPlaneItem *, MemberData *> memberAndVSlab;
    QHash<CustomDrawHPlaneItem *, MemberData *> memberAndHWall;
    QHash<CustomDrawVPlaneItem *, MemberData *> memberAndVWall;
    QHash<CustomDrawHPlaneItem *, MemberData *> memberAndHUWall;
    QHash<CustomDrawVPlaneItem *, MemberData *> memberAndVUWall;

    QHash<CustomDrawHLineItem *, MemberData *> memberAndHDamper;
    QHash<CustomDrawVLineItem *, MemberData *> memberAndVDamper;
    QHash<CustomDrawHLineItem *, MemberData *> memberAndHIsolator;
    QHash<CustomDrawVLineItem *, MemberData *> memberAndVIsolator;

//    QHash<CustomDrawHLineItem*,MemberData*> memberAndHSpring;
//    QHash<CustomDrawVLineItem*,MemberData*> memberAndVSpring;

    QHash<JointLoadItem *, MemberData *> memberAndJointLoad;
    QHash<CustomDrawHLineItem *, MemberData *> memberAndMemberLoad;
    QHash<CustomDrawHPlaneItem *, MemberData *> memberAndSlabLoad;

    QList<GLOBALLINE> batchInputLines;
    QList<QList<JointItem *> > batchInputPlanes;

    virtual QList<MemberData *> filterSceneMember( const QList<MemberData *> & ) const
    {
        return QList<MemberData *>();
    }

    virtual void createMemberItems(const QList<MemberData *> &) = 0;
    virtual void removeMemberItem(MemberData *, DATATYPE);
    virtual void removeMemberItems(const QList<MemberData *> &, DATATYPE);
    virtual void changeMemberItems(const QList<MemberData *> &) = 0;


    bool isSelectableJoint(JointData *, const LIMITSELECTTERM &);
    bool isSelectableMember(MemberData *, const LIMITSELECTTERM &);


};

} // namespace post3dapp
#endif


