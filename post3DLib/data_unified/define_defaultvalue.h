#ifndef DEFINE_DEFAULTVALUE_H
#define DEFINE_DEFAULTVALUE_H

#include <QString>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
enum class TABLEDATATYPE {TB_STRING = 0, TB_INT, TB_DOUBLE, TB_BOOL};

struct POST3D_CLASS_DLLSPEC REGULATION{
    REGULATION(){};
    REGULATION(TABLEDATATYPE _type, const QVariant& _def = QVariant())
        :cellType(_type), defValue(_def){};
    TABLEDATATYPE cellType;
    QVariant defValue;

    static const QMap<QString, REGULATION> defaultFloor;
    static const QStringList defaultFloorScale;
    static const QMap<QString, REGULATION> defaultAnalysisFloor;
    static const QStringList defaultAnalysisFloorChildFloor;
    static const QMap<QString, REGULATION> defaultFrameGroup;
    static const QStringList defaultFrameGroupFrameType;
    static const QMap<QString, REGULATION> defaultFrameSubGroup;
    static const QMap<QString, REGULATION> defaultFrameXY;

    static const QMap<QString, REGULATION> defaultMaterial;
    static const QStringList defaultMaterialSymbol;
    static const QMap<QString, REGULATION> defaultColumnRc;
    static const QMap<QString, REGULATION> defaultColumnS;
    static const QStringList defaultColumnSSteelShape;
    static const QMap<QString, REGULATION> defaultColumnSrc;
    static const QStringList defaultColumnSrcSteelShape;
    static const QStringList defaultColumnSrcSteelShapeT;
    static const QMap<QString, REGULATION> defaultColumnCft;
    static const QStringList defaultColumnCftSteelShape;
    static const QMap<QString, REGULATION> defaultGirderRc;
    static const QStringList defaultGirderCantilever;
    static const QMap<QString, REGULATION> defaultGirderS;
    static const QStringList defaultGirderSSteelShape;
    static const QStringList defaultGirderSStiffner;
    static const QMap<QString, REGULATION> defaultGirderSrc;
    static const QStringList defaultGirderSrcSteelShape;
    static const QMap<QString, REGULATION> defaultBraceS;
    static const QStringList defaultBraceSSteelShape;
    static const QStringList defaultBraceSCompressiveResistance;
    static const QMap<QString, REGULATION> defaultBeamRc;
    static const QMap<QString, REGULATION> defaultBeamS;
    static const QStringList defaultBeamSSteelShape;
    static const QMap<QString, REGULATION> defaultBeamSrc;
    static const QStringList defaultBeamSrcSteelShape;
    static const QMap<QString, REGULATION> defaultWallRc;
    static const QStringList defaultWallRcType;
    static const QStringList defaultWallRcBarArrangementType;
    static const QMap<QString, REGULATION> defaultWallS;
    static const QStringList defaultWallSType;
    static const QMap<QString, REGULATION> defaultWallUnderGround;
    static const QMap<QString, REGULATION> defaultSlab;
    static const QStringList defaultSlabType;
    static const QMap<QString, REGULATION> defaultBaseS;
    static const QStringList defaultBaseSType;
    static const QMap<QString, REGULATION> defaultBaseSrc;
    static const QStringList defaultBaseSrcBplShape;
    static const QMap<QString, REGULATION> defaultIsolator;
    static const QStringList defaultIsolatorType;
    static const QStringList defaultIsolatorShearHysteresis;
    static const QStringList defaultIsolatorAxialHysteresis;
    static const QStringList defaultIsolatorRender;
    static const QMap<QString, REGULATION> defaultDamper;
    static const QStringList defaultDamperType;
    static const QList<QStringList> defaultDamperUnit;
    static const QMap<QString, REGULATION> defaultOpening;
    static const QStringList defaultOpeningType;
    static const QStringList defaultOpeningLrBase;
    static const QStringList defaultOpeningLr;
    static const QStringList defaultOpeningUdBase;
    static const QStringList defaultOpeningUd;
    static const QMap<QString, REGULATION> defaultJointLoad;
    static const QStringList defaultLoadType;
    static const QMap<QString, REGULATION> defaultMemberLoad;
    static const QStringList defaultMemberLoadType;
    static const QStringList defaultMemberLoadCoordinate;
    static const QStringList defaultMemberLoadDirection;
    static const QMap<QString, REGULATION> defaultSlabLoad;
    static const QMap<QString, REGULATION> defaultJointTable;
    static const QMap<QString, REGULATION> defaultMemberTable;
    static const QStringList defaultMemberTableElementList;
    static const QStringList defaultMemberTableFaceRule;
    static const QMap<QString, REGULATION> defaultLoadTable;
    static const QStringList defaultLoadTableElementList;

    static const QMap<QString, REGULATION> defaultIndivisualSettingType;
};




} // namespace post3dapp

#endif // DEFINE_DEFAULTVALUE_H
