#include "abstracttext.h"

#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsmen::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"|   せん断モデル         | TYPE  |      V1     |      V2     |      V3     |      V4     |").append("\n");
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| 弾性                   | LIN   |   K(kN/m)   |      -      |      -      |      -      |").append("\n");
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| リバーシブルバイリニア | R-BI  |   K1(kN/m)  |   K2(kN/m)  |    Fy(kN)   |      -      |").append("\n");
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| ノーマル    バイリニア | N-BI  |   K1(kN/m)  |   K2(kN/m)  |    Fy(kN)   |      -      |").append("\n");
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| SSB                    | SSB   |   Pv0(kN)   |    T2(sec)  |      -      |      -      |").append("\n");
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"").append("\n");
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"|   軸モデル             | TYPE  |      V1     |      V2     |      V3     |      V4     |").append("\n");
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"|   弾性                 | LIN   |   K(kN/m)   |      -      |      -      |      -      |").append("\n");
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| リバーシブルバイリニア | R-BI  |   K1(kN/m)  |   K2(kN/m)  |   Fy+(kN)   |   Fy-(kN)   |").append("\n");
    second.append(u8"+------------------------+-------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"                            /-------------------- せん断モデル --------------------/ /-------------------- 軸モデル --------------------/").append("\n");
    second.append(u8"        NAME      BAI    RR  M/S TYPE NS  DISP  HR             V1       V2       V3   R/M TYPE             V1       V2       V3       V4").append("\n");
    second.append(u8"                          M                  M").append("\n");
    second.append(u8"   MNO. ---------------- I ----------------      J  NAME").append("\n");
    Q_FOREACH(auto sec, fixedData.getIsoList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
