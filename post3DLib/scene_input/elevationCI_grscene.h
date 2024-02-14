#ifndef ELEVATIONCOPYINTERFACEGRSCENE_H
#define ELEVATIONCOPYINTERFACEGRSCENE_H

#include "background_frame.h"
#include "input_grscene.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class UnifiedInputData;
class POST3D_CLASS_DLLSPEC ElevationCopyInterfaceGrScene : public InputGrScene
{
    Q_OBJECT

public:

    ElevationCopyInterfaceGrScene(LIMITSELECTTERM, UnifiedInputData*, const Input2DViewSettings& setting, bool _syncSetting, QObject *parent = 0 );
    ~ElevationCopyInterfaceGrScene();

    void setStatus(const QUuid &uid );
    /* BackFloorに関する処理 */
    void setBackGround(const QString &fr, const QString &wk = "COMMON") override;
    void drawBackGround(QPainter *paniter) override;
    QList<GlobalPoint> getCrossPoints() const override;
    QList<GlobalPoint> getDefinePoints() const override;
    void setCurrentFrames(const QList<QUuid> &) override;
    void setCurrentFloors(const QList<QUuid> &) override;
    void clearCurrentAttach() override;

    QUuid frameID() const override;
    void completeGlobalXYZ(GlobalPoint &gp) override;

    void prepareBatchInputLines(DATATYPE) override;
    void prepareBatchInputPlanes(bool containsBeam = false) override;

    //void setOutfigRangeOption(OUTFIG_TYPE_OPTION range );
//    void exportRenderBackGround(QPainter *painter, int nw, int nh, QSizeF unitPaperSize,
//                                qreal scale = 1.0,QPointF origin = QPointF(0,0),int dpi = 72);
//    void exportRenderItems(QPainter *painter, int nw, int nh, QSizeF unitPaperSize,
//                           qreal scale = 1.0, QPointF origin = QPointF(0,0), int dpi = 72);
    BackGroundFrame* getBackGround() const override {return backGround;}
    FIGURE_TYPE_PROPERTY getFigureTypeProperty() const override;
    void SetUseScaledFont(bool useScaledFont)override
    {
        viewSettings.SetUseScaledFont(useScaledFont);
        backGround->SetUseScaledFont(useScaledFont);
    }

public slots:

    void resetItems() override;

    void createJointItems(const QList<JointData *> &) override;
//    void changeJointItems(const QList<JointData*>&);

signals:

protected:

private:

    QUuid myFrameID;
    BackGroundFrame *backGround;

    QList<MemberData *> filterSceneMember( const QList<MemberData *> & ) const override;
    void createMemberItems(const QList<MemberData *> &) override;
    void changeMemberItems(const QList<MemberData *> &) override;

    void createHLineItem(MemberData *, unsigned int, unsigned int, const QList<QList<QPointF> > &,
                         const QList<int> &, qreal, qreal);
    void createVLineUpperItem(MemberData *, unsigned int, unsigned int, const QList<int> &, qreal,
                              qreal);
    void createVLineDownItem(MemberData *, unsigned int, const QList<int> &, qreal, qreal);

    void createHPlaneItem(MemberData *, unsigned int, unsigned int, const QList<QList<QPointF> > &,
                          bool);
    void createVPlaneItem(MemberData *, unsigned int, unsigned int, const QList<QList<QPointF> > &,
                          bool);

    bool isClosedFrame() const;
    UnifiedInputData* copy;
   // OUTFIG_TYPE_OPTION outfigRangeOption;

};
} // namespace post3dapp

#endif // ELEVATIONCOPYINTERFACEGRSCENE_H
