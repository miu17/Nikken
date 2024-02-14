﻿#ifndef PLANCOPYINTERFACEGRSCENE_H
#define PLANCOPYINTERFACEGRSCENE_H

#include "input_grscene.h"
#include "copy_background_floor.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class UnifiedInputData;
class POST3D_CLASS_DLLSPEC PlanCopyInterfaceGrScene : public InputGrScene
{
    Q_OBJECT

public:

    PlanCopyInterfaceGrScene(LIMITSELECTTERM, UnifiedInputData*, const Input2DViewSettings& setting, bool _syncSetting, QObject *parent = nullptr );
    ~PlanCopyInterfaceGrScene();
    void setStatus(const QUuid &uid ) override;

    void setBackGround(const QString &fl, const QString &wk = "COMMON") override;
    GlobalPoint searchPointAttachment( QPointF );

    /* BackFloorに関する処理 */
    void drawBackGround(QPainter *paniter) override;
    QList<GlobalPoint> getCrossPoints() const override;
    QList<GlobalPoint> getDefinePoints() const override;
    void setCurrentFrames(const QList<QUuid> &) override;
    void clearCurrentAttach() override;

    QUuid floorID() const override;
    qreal floorHeight() const override;
    void completeGlobalXYZ(GlobalPoint &gp) override;

    void prepareBatchInputLines(DATATYPE) override;
    void prepareBatchInputPlanes(bool containsBeam = false) override;

    //void setOutfigRangeOption(OUTFIG_TYPE_OPTION range );
    void exportRenderBackGround(QPainter *painter, int nw, int nh, QSizeF unitPaperSize,
                                 qreal scale = 1.0, QPointF origin=QPointF(0,0),int dpi = 72);
    void exportRenderItems(QPainter *painter, int nw, int nh, QSizeF unitPaperSize,
                           qreal scale = 1.0, QPointF origin=QPointF(0,0), int dpi = 72);

    CopyBackGroundFloor* getBackGround() const override{return backGround;}
    FIGURE_TYPE_PROPERTY getFigureTypeProperty() const override;
    void SetUseScaledFont(bool useScaledFont) override
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

    QUuid myFloorID;
    CopyBackGroundFloor* backGround;
//    OUTFIG_TYPE_OPTION outfigRangeOption;

    QList<MemberData *> filterSceneMember( const QList<MemberData *> & ) const override;
    void createMemberItems(const QList<MemberData *> &) override;
    void changeMemberItems(const QList<MemberData *> &) override;

    void createHLineItem(MemberData *, unsigned int, unsigned int, const QList<QList<QPointF> > &,
                         const QList<int> &);
    void createVLineUpperItem(MemberData *, unsigned int, unsigned int, const QList<int> &);
    void createVLineDownItem(MemberData *, unsigned int, const QList<int> &);

    void createHPlaneItem(MemberData *, unsigned int, unsigned int, const QList<QList<QPointF> > &,
                          bool);
    void createVPlaneItem(MemberData *, unsigned int, unsigned int, const QList<QList<QPointF> > &,
                          bool);
    UnifiedInputData* copy;
};
} // namespace post3dapp
#endif // PLANCOPYINTERFACEGRSCENE_H
