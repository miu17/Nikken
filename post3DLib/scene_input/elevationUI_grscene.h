#ifndef ELEVATIONUIGRSCENE_H
#define ELEVATIONUIGRSCENE_H

#include "input_grscene.h"
#include "background_frame.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class POST3D_CLASS_DLLSPEC ElevationInterfaceGrScene : public InputGrScene
{
    Q_OBJECT

public:

    ElevationInterfaceGrScene(LIMITSELECTTERM, const Input2DViewSettings& setting, bool _syncSetting, QObject *parent = 0 );
    ~ElevationInterfaceGrScene();

    void setStatus(const QUuid &uid )override;
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
//                                qreal scale = 1.0,QPointF origin = QPointF(0,0),int dpi = 72)override;
//    void exportRenderItems(QPainter *painter, int nw, int nh, QSizeF unitPaperSize,
//                           qreal scale = 1.0, QPointF origin = QPointF(0,0), int dpi = 72)override;

    BackGroundFrame* getBackGround() const override{return backGround;}
    FIGURE_TYPE_PROPERTY getFigureTypeProperty() const override;
    void SetUseScaledFont(bool useScaledFont) override
    {
        viewSettings.SetUseScaledFont(useScaledFont);
        backGround->SetUseScaledFont(useScaledFont);
    }

public slots:

    void resetItems() override;

    void createJointItems(const QList<post3dapp::JointData *> &) override;
//    void changeJointItems(const QList<JointData*>&);

signals:

protected:

private:
    BackGroundFrame* backGround;
    QUuid myFrameID;

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
   // OUTFIG_TYPE_OPTION outfigRangeOption;

};
} // namespace post3dapp
#endif


