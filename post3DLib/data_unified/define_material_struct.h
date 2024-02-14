#ifndef DEFINE_MATERIALSTRUCT_H
#define DEFINE_MATERIALSTRUCT_H

#include <QList>
#include <QString>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
/* タイプ定義 */

class POST3D_CLASS_DLLSPEC MaterialType
{
public:
    enum class Type {UNDEFINED = -1, CONCRETE = 0, STEEL, REINFORCE};
    static Type stringToEnum(const QString &str);
    static QString enumToString(const MaterialType::Type&);
};

/* 材料定義  */

struct POST3D_CLASS_DLLSPEC MATERIALVALUES {
    explicit MATERIALVALUES(
        MaterialType::Type type_ = MaterialType::Type::UNDEFINED,
        const QString &name_ = QString(),
        const QString &symbol_ = QString("D"),
        int rangefrom_ = 0,
        int rangeto_ = 0,
        qreal E_ = 0.0,
        qreal G_ = 0.0,
        qreal uw1_ = 0.0,
        qreal uw2_ = 0.0,
        qreal ratio_ = 0.0,
        qreal F_ = 0.0,
        qreal ft_l_ = 0.0,
        qreal ft_s_ = 0.0,
        qreal ft_u_ = 0.0,
        qreal fc_l_ = 0.0,
        qreal fc_s_ = 0.0,
        qreal fc_u_ = 0.0,
        qreal fs_l_ = 0.0,
        qreal fs_s_ = 0.0,
        qreal fs_u_ = 0.0,
        qreal fa_l_ = 0.0,
        qreal fa_s_ = 0.0,
        qreal fa_lu_ = 0.0,
        qreal fa_su_ = 0.0,
        qreal fa_u_ = 0.0 )
        : materialType(type_), materialName(name_),
          symbol(symbol_), barMin(rangefrom_), barMax(rangeto_),
          E(E_), G(G_), unitW1(uw1_), unitW2(uw2_), modularRatio(ratio_), F(F_),
          Ft_L(ft_l_), Ft_S(ft_s_), Ft_U(ft_u_),
          Fc_L(fc_l_), Fc_S(fc_s_), Fc_U(fc_u_),
          Fs_L(fs_l_), Fs_S(fs_s_), Fs_U(fs_u_),
          Fa_L(fa_l_), Fa_S(fa_s_), Fa_U(fa_u_),
          Fa_Lu(fa_lu_), Fa_Su(fa_su_){}

    MaterialType::Type materialType;    // 材料タイプ
    QString materialName;               // 材料符号
    QString symbol;                     // 鉄筋シンボル
    int barMin;                         // 鉄筋　使用範囲 Min
    int barMax;                         // 鉄筋　使用範囲 Max
    qreal E;                            // ヤング係数E N/mm2
    qreal G;                            // せん断弾性係数G N/mm2
    qreal unitW1;                       // 単位重量1 kN/m3
    qreal unitW2;                       // 単位重量2 kN/m3
    qreal modularRatio;                 // ヤング係数比 n
    qreal F;                            // F値 N/mm2
    qreal Ft_L;                         // 引張強度 長期 N/mm2
    qreal Ft_S;                         // 引張強度 短期 N/mm2
    qreal Ft_U;                         // 引張強度 終局 N/mm2
    qreal Fc_L;                         // 圧縮強度 長期 N/mm2
    qreal Fc_S;                         // 圧縮強度 短期 N/mm2
    qreal Fc_U;                         // 圧縮強度 終局 N/mm2
    qreal Fs_L;                         // せん断強度 長期 N/mm2
    qreal Fs_S;                         // せん断強度 短期 N/mm2
    qreal Fs_U;                         // せん断強度 終局 N/mm2
    qreal Fa_L;                         // 付着強度 長期 N/mm2
    qreal Fa_S;                         // 付着強度 短期 N/mm2
    qreal Fa_U;                         // 付着強度 終局 N/mm2
    qreal Fa_Lu;                        // 付着強度 長期 上端 N/mm2
    qreal Fa_Su;                        // 付着強度 短期 上端 N/mm2

    QString writeData() const;
};
} // namespace post3dapp
#endif
