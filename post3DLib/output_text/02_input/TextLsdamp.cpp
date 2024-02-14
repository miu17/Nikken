#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextLsdamp::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"|       ダンパータイプ        |    TYPE    |      V1     |      V2     |      V3     |      V4     |      V5     |      V6     |      V7     |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| 線形減衰・剛性              | LINEAR     |   C(kNs/m)  |   K(kN/m)   |      -      |      -      |   (Fy(kN))  |    (Kp/K)   |   Kc(kN/m)  |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| 鋼材バイリニア（剛性入力）  | STEEL-BL-K |   K1(kN/m)  |   Fy+(kN)   |   K2(kN/m)  |    Fy-(kN)  |      -      |      -      |   Kc(kN/m)  |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| 鋼材バイリニア（断面積入力）| STEEL-BL-A |   E(kN/m2)  |    A(m2)    | σy(kN/m2)  |      α     |      β     |     Ep/E    |      γ     |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| 鋼材トリリニア（剛性入力）  | STEEL-TL-K |   K1(kN/m)  |   Fy1(kN)   |   K2(kN/m)  |    Fy2(kN)  |   K3(kN/m)  |      -      |   Kc(kN/m)  |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+------+------+-------------+------+------+").append("\n");
    second.append(u8"| 鋼材トリリニア（断面積入力）| STEEL-TL-A |   E(kN/m2)  |    A(m2)    | σy1(kN/m2) | σy2(kN/m2) |  α  |  β  |      γ     | E2/E | E3/E |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+------+------+-------------+------+------+").append("\n");
    second.append(u8"| オイル                      | OIL        |  C1(kNs/m)  |    C2/C1    |   Kd(kN/m)  |    δm(m)   |    Fy(kN)   |      -      |   Kc(kN/m)  |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| 粘性（速度の指数乗）        | VISCOS     |C(kN(s/m)^α)|      α     |   Kd(kN/m)  |    δm(m)   |      -      |      -      |   Kc(kN/m)  |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| 3M粘弾性                    | 3M-VEM     |     A(m2)   |     d(m)    |    TYPE     |    δm(m)   |    Fy(kN)   |      -      |   Kc(kN/m)  |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| HIDAX                       | HIDAX      |  C1(kNs/m)  |  C2(kNs/m)  | Cmin1(kNs/m)| Cmin2(kNs/m)|    Fy(kN)   |      -      |   Kc(kN/m)  |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| オイレス制震壁              | OLS-VW     |     A(m2)   |     d(m)    |   t(deg.)   |    δm(m)   |      -      |      -      |   Kc(kN/m)  |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| ADC制振壁                   | ADC-VW     |     A(m2)   |     h/w     |     d(m)    |    δm(m)   |   t(deg.)   |      e      |   Kc(kN/m)  |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"| 武田モデル要素              | TAKEDA     |   K1(kN/m)  |   K1(kN/m)  |   K1(kN/m)  |    Fy1(kN)  |    Fy2(kN)  |      -      |      -      |").append("\n");
    second.append(u8"+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+").append("\n");
    second.append(u8"        NAME     TYPE      M/S SHAPE    yVX   yVY   yVZ   AR   T1     RL    H2 DIR   FaT             KaT             V1             V2             V3").append("\n");
    second.append(u8"                                                              SEC      M     M        kN            kN/m").append("\n");
    second.append(u8"                                                                                                      V4             V5             V6             V7").append("\n");
    second.append(u8"   MNO. ---------------- I ----------------      J      K  NAME      TYPE        SHAPE").append("\n");
    Q_FOREACH(auto sec, fixedData.getDamperList()){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
