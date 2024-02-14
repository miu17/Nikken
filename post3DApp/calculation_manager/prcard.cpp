#include "prcard.h"

Prcard::Prcard()
{

}

QStringList Prcard::getColumnList(){
    /*
XXX ZZ- ZZ  柱 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
柱符号  +-- 階 --+  仕上 構造 コンクリート 配筋    主筋   +-- フープ --+  鉄骨    +---- 鉄骨断面(y) -----+  +---- 鉄骨断面(z) -----+
                    重量 形式    Dy   Dz  タイプ ny nz D  ny nz  D        材種         A  x  B  x  t1 x t2       A  x  B  x  t1 x t2
    */
    return QStringList{};
}
QStringList Prcard::getGirderList(){
    /*
XXX ZZ- ZZ  大梁 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
大梁符号 +-- 階 --+   仕上  構造   コンクリート断面  配筋 +-上端主筋-+ +-下端主筋-+ +スターラップ+ 鉄骨   +------- 鉄骨断面 -----+
                      重量  形式    b     D  ハンチ タイプ  n  n  n D    n  n  n D    n D          材種        A  x  B  x t1 x t2
    */
    return QStringList{};
}
QStringList Prcard::getWallList(){
    /*
XXX ZZ- ZZ  壁 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
                            +-------- 壁配筋 --------+ +-------- 開口補強配筋 --------+ +-------- 開口補強配筋 --------+
壁符号  厚さ  仕上   配筋   +--タテ筋--+  +--ヨコ筋--+ +-タテ筋-+ +-ヨコ筋-+ +--斜筋--+ +-タテ筋-+ +-ヨコ筋-+ +--斜筋--+
              重量  タイプ   D   D         D   D          n  D       n  D       n  D       n  D       n  D       n  D
    */
    return QStringList{};
}
QStringList Prcard::getSteelWallList(){
    /*
XXX ZZ- ZZ  鋼板壁 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
鋼板壁符号  板厚  仕上  +------ タテ ------+  +------ ヨコ ------+
                  重量   ピッチ スチフナー材   ピッチ スチフナー材
    */
    return QStringList{};
}
QStringList Prcard::getFoundationWallList(){
    /*
XXX ZZ- ZZ  地下外壁 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
                               +------------------タテ筋------------------+ +----------ヨコ筋-----------+
地下外壁符号  厚さ   仕上      +--- 上部 ---+ +--- 中部 ---+ +--- 下部 ---+ +--- 端部 ---+ +--- 中央 ---+
                     重量        D    D         D   D          D   D          D   D          D   D
    */
    return QStringList{};
}
QStringList Prcard::getBraceList(){
    /*
XXX ZZ- ZZ  ブレース XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
ブレース符号  仕上                    +---------- 鉄骨寸法 ---------+ +-コンクリート断面-+ +--主筋--+ +---フープ---+
              重量  タイプ  鉄骨材種        A  x   B  x  t1  x  t2       b      D           n  D       n  D
    */
    return QStringList{};
}
QStringList Prcard::getBeamList(){
    /*
XXX ZZ- ZZ  小梁 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
小梁符号   仕上  構造   コンクリート断面   配筋  +- 上端主筋 -+ +- 下端主筋 -+ +スターラップ+  鉄骨   +------- 鉄骨断面 -----+
           重量  形式    b     D  ハンチ  タイプ   n  n  n  D     n  n  n  D     n  D          材種        A  x  B  x t1 x t2
    */
    return QStringList{};
}
QStringList Prcard::getSlabList(){
    /*
XXX ZZ- ZZ  スラブ XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
                                  +---------------短辺方向配筋---------------+ +---------------長辺方向配筋---------------+
スラブ符号  厚さ デッキ  仕上     +--Ａ部端部--+ +--Ａ部中央--+ +--Ｂ部端中--+ +--Ａ部端部--+ +--Ａ部中央--+ +--Ｂ部端中--+
                         重量      D   D          D   D          D   D          D   D          D   D          D   D
    */
    return QStringList{};
}
QStringList Prcard::getOpeningList(){
    /*
XXX ZZ- ZZ  開口 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
開口符号  +------ 参照開口符号 ------+  +-------------- 開口位置 --------------+  +---- 開口寸法 ---+   コメント
                                         横配置  :  X(mm)    縦配置  :  Y(mm)      幅(mm)  高さ(mm)
    */
    return QStringList{};
}
QStringList Prcard::getSteelBaseList(){
    /*
XXX ZZ- ZZ  S 露出柱脚 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
                                             /----------- ベースプレート -----------/ /--- アンカーボルト ----/
        NAME     MAKER      規格                 B      D     T    hy    hz  C材種     ny  nz  径     F     lb
                                               (MM)   (MM)  (MM)  (MM)  (MM)                   (N/mm2)   (mm)
(XXXXX) XXXXXXXX XXXXXXXXXX XXXXXXXXXXXXXXXX XXXX.X XXXX.X XXX.X XXX.X XXX.X XXXXXXXX XXX XXX XXX XXXX.X XXXX.X
   MNO.      I      J LIST             FL     FR     GR1    GR2    IorJ  BASE
(XXXXX) XXXXXX XXXXXX XXXXXXXXXXXXXXXX XXXXXX XXXXXX XXXXXX XXXXXX   X   XXXXXXX
                                         横配置  :  X(mm)    縦配置  :  Y(mm)      幅(mm)  高さ(mm)
    */
    return QStringList{};
}
QStringList Prcard::getSrcBaseList(){
    /*
XXX ZZ- ZZ  SRC非埋込柱脚リスト XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
                 /-------------------------------- ベースプレート ------------------------------/ /------- アンカーボルト ---------/
        NAME     形状          B      D     T   hy1   hy2   hz1   hz2   sml    B0    D0  C材種    配置    ny1 nz1 ny2 nz2 径      F
                             (MM)   (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)                                     (N/mm2)
(XXXXX) XXXXXXXX XXXXXXXXX XXXX.X XXXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXXXXXXX XXXXXXX XXX XXX XXX XXX XXX XXXX.X
   MNO.      I      J LIST             FL     FR     GR1    GR2    BASE
    */
    return QStringList{};
}
QStringList Prcard::getDamperList(){
    /*
XXX ZZ- ZZ  制振ダンパー XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
|       ダンパータイプ        |    TYPE    |      V1     |      V2     |      V3     |      V4     |      V5     |      V6     |      V7     |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| 線形減衰・剛性              | LINEAR     |   C(kNs/m)  |   K(kN/m)   |      -      |      -      |   (Fy(kN))  |    (Kp/K)   |   Kc(kN/m)  |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| 鋼材バイリニア（剛性入力）  | STEEL-BL-K |   K1(kN/m)  |   Fy+(kN)   |   K2(kN/m)  |    Fy-(kN)  |      -      |      -      |   Kc(kN/m)  |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| 鋼材バイリニア（断面積入力）| STEEL-BL-A |   E(kN/m2)  |    A(m2)    | σy(kN/m2)  |      α     |      β     |     Ep/E    |      γ     |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| 鋼材トリリニア（剛性入力）  | STEEL-TL-K |   K1(kN/m)  |   Fy1(kN)   |   K2(kN/m)  |    Fy2(kN)  |   K3(kN/m)  |      -      |   Kc(kN/m)  |
+-----------------------------+------------+-------------+-------------+-------------+-------------+------+------+-------------+------+------+
| 鋼材トリリニア（断面積入力）| STEEL-TL-A |   E(kN/m2)  |    A(m2)    | σy1(kN/m2) | σy2(kN/m2) |  α  |  β  |      γ     | E2/E | E3/E |
+-----------------------------+------------+-------------+-------------+-------------+-------------+------+------+-------------+------+------+
| オイル                      | OIL        |  C1(kNs/m)  |    C2/C1    |   Kd(kN/m)  |    δm(m)   |    Fy(kN)   |      -      |   Kc(kN/m)  |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| 粘性（速度の指数乗）        | VISCOS     |C(kN(s/m)^α)|      α     |   Kd(kN/m)  |    δm(m)   |      -      |      -      |   Kc(kN/m)  |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| 3M粘弾性                    | 3M-VEM     |     A(m2)   |     d(m)    |    TYPE     |    δm(m)   |    Fy(kN)   |      -      |   Kc(kN/m)  |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| HIDAX                       | HIDAX      |  C1(kNs/m)  |  C2(kNs/m)  | Cmin1(kNs/m)| Cmin2(kNs/m)|    Fy(kN)   |      -      |   Kc(kN/m)  |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| オイレス制震壁              | OLS-VW     |     A(m2)   |     d(m)    |   t(deg.)   |    δm(m)   |      -      |      -      |   Kc(kN/m)  |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| ADC制振壁                   | ADC-VW     |     A(m2)   |     h/w     |     d(m)    |    δm(m)   |   t(deg.)   |      e      |   Kc(kN/m)  |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
| 武田モデル要素              | TAKEDA     |   K1(kN/m)  |   K1(kN/m)  |   K1(kN/m)  |    Fy1(kN)  |    Fy2(kN)  |      -      |      -      |
+-----------------------------+------------+-------------+-------------+-------------+-------------+-------------+-------------+-------------+
        NAME     TYPE      M/S SHAPE    yVX   yVY   yVZ   AR   T1     RL    H2 DIR   FaT             KaT             V1             V2             V3
                                                              SEC      M     M        kN            kN/m
                                                                                                      V4             V5             V6             V7

(XXXXX) XXXXXXXX XXXXXXXXXX X XXXXXXX X.XXX X.XXX X.XXX X.XX X.XX XX.XXX X.XXX  X XXXXXX. XXXXXXXXXXXXXX XXXXXXXXXXXXXX XXXXXXXXXXXXXX XXXXXXXXXXXXXX
                                                                                          XXXXXXXXXXXXXX XXXXXXXXXXXXXX XXXXXXXXXXXXXX XXXXXXXXXXXXXX
   MNO. ---------------- I ----------------      J      K  NAME      TYPE        SHAPE
(XXXXX) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX XXXXXX         XXXXXXXX  XXXXXXXXXX  XXXXXXX
    */
    return QStringList{};
}
QStringList Prcard::getIsolatorList(){
    /*
XXX ZZ- ZZ  免震装置 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
+------------------------+-------+-------------+-------------+-------------+-------------+
|   せん断モデル         | TYPE  |      V1     |      V2     |      V3     |      V4     |
+------------------------+-------+-------------+-------------+-------------+-------------+
| 弾性                   | LIN   |   K(kN/m)   |      -      |      -      |      -      |
+------------------------+-------+-------------+-------------+-------------+-------------+
| リバーシブルバイリニア | R-BI  |   K1(kN/m)  |   K2(kN/m)  |    Fy(kN)   |      -      |
+------------------------+-------+-------------+-------------+-------------+-------------+
| ノーマル    バイリニア | N-BI  |   K1(kN/m)  |   K2(kN/m)  |    Fy(kN)   |      -      |
+------------------------+-------+-------------+-------------+-------------+-------------+
| SSB                    | SSB   |   Pv0(kN)   |    T2(sec)  |      -      |      -      |
+------------------------+-------+-------------+-------------+-------------+-------------+

+------------------------+-------+-------------+-------------+-------------+-------------+
|   軸モデル             | TYPE  |      V1     |      V2     |      V3     |      V4     |
+------------------------+-------+-------------+-------------+-------------+-------------+
|   弾性                 | LIN   |   K(kN/m)   |      -      |      -      |      -      |
+------------------------+-------+-------------+-------------+-------------+-------------+
| リバーシブルバイリニア | R-BI  |   K1(kN/m)  |   K2(kN/m)  |   Fy+(kN)   |   Fy-(kN)   |
+------------------------+-------+-------------+-------------+-------------+-------------+
                            /-------------------- せん断モデル --------------------/ /-------------------- 軸モデル --------------------/
        NAME      BAI    RR  M/S TYPE NS  DISP  HR             V1       V2       V3   R/M TYPE             V1       V2       V3       V4
                          M                  M
(XXXXX) XXXXXXXX XX.X X.XXX   X  XXXX XX X.XXX .XX XXXXXXXXXXXXXX XXXXXXXX XXXXXXXX    X  XXXX XXXXXXXXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
   MNO. ---------------- I ----------------      J  NAME
(XXXXX) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX XXXXXX  XXXXXXXX
    */
    return QStringList{};
}



QStringList Prcard::getList(){
    QStringList ret;
    ret.append(getColumnList());
    ret.append(getGirderList());
    ret.append(getWallList());
    ret.append(getSteelWallList());
    ret.append(getFoundationWallList());
    ret.append(getBraceList());
    ret.append(getBeamList());
    ret.append(getSlabList());
    ret.append(getOpeningList());
    ret.append(getSteelBaseList());
    ret.append(getSrcBaseList());
    ret.append(getDamperList());
    ret.append(getIsolatorList());
    return ret;
}


QStringList Prcard::getEstimate(){
    /*
    XXX ZZ- ZZ  数量／重量 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 05/13/02 20:35:48 ] XXXXXXXXXX
    ■ 固定荷重 (KN)
    階名      床     節点    大梁    小梁  大梁上  小梁上     柱     間柱     壁  ブレ－ス  層合計  LL面積  [/LL面積 ] 上階合計
                  追加荷重                追加荷重 追加荷重                                           (M2)   (KN/M2)
    XXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX
                                                           XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXXX XXXXXXX [XXXX.XX ] XXXXXXXXX
    ----------------------------------------------------------------------------------------------------------------------------
    TOTAL  XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXXX XXXXXXX [XXXX.XX ]
    ( % )  (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X)  (XXX.X)
    ■ 固定荷重の内訳 (KN)                                     BODY:く体   FL:各リスト仕上  FLL:床仕上リスト  FIRE:耐火被覆
           +------------------------ 当階 ----------------------------+ +------------ 下階 ------------+
    階名            床     節点           大梁    小梁  大梁上  小梁上     柱     間柱     壁  ブレ－ス  層合計  LL面積  [/LL面積 ] 上階合計
                         追加荷重                      追加荷重 追加荷重                                           (M2)   (KN/M2)
    XXXXXX BODY  XXXXXXX          BODY  XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX
           FL    XXXXXXX          FL    XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX
           FLL   XXXXXXX          FIRE  XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX
           TOTAL XXXXXXX XXXXXXX  TOTAL XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXXX XXXXXXX [XXXX.XX ] XXXXXXXXX
           ( % ) (XXX.X) (XXX.X)        (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X)  (XXX.X)
    -----------------------------------------------------------------------------------------------------------------------------------------
    TOTAL  BODY  XXXXXXX          BODY  XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX
           FL    XXXXXXX          FL    XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX
           FLL   XXXXXXX          FIRE  XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX
           TOTAL XXXXXXX XXXXXXX  TOTAL XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXXX XXXXXXX [XXXX.XX ]
           ( % ) (XXX.X) (XXX.X)        (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X)  (XXX.X)
    ■ コンクリ－ト体積 (M3)
    階名        床       大梁      小梁       柱      間柱        壁    ブレ－ス   層合計    LL面積  [ /LL面積 ]  上階合計
    XXXXXX XXXXXXX.X XXXXXXX.X XXXXXXX.X
                                         XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X [XXXXX.XX ] XXXXXXXXXX
    -----------------------------------------------------------------------------------------------------------------------
    TOTAL  XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXXXX [XXXXX.XX ]
    ( % )    ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )
    ■ 型枠面積 (M2)
    階名        床       大梁      小梁       柱      間柱        壁    ブレ－ス   層合計    LL面積  [ /LL面積 ]  上階合計
    XXXXXX XXXXXXX.X XXXXXXX.X XXXXXXX.X
                                         XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X [XXXXX.XX ] XXXXXXXXXX
    -----------------------------------------------------------------------------------------------------------------------
    TOTAL  XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXXXX [XXXXX.XX ]
    ( % )    ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )
    ■ 鉄骨重量 (KN)
    階名                 大梁      小梁       柱      間柱        壁    ブレ－ス   層合計    LL面積  [ /LL面積 ]  上階合計
    XXXXXX           XXXXXXX.X XXXXXXX.X
                                         XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X [XXXXX.XX ] XXXXXXXXXX
    -----------------------------------------------------------------------------------------------------------------------
    TOTAL            XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXXXX [XXXXX.XX ]
    ( % )              ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )
    ■ 鉄骨耐火被覆重量 (KN) ・ 鉄骨表面積 (M2)   ( S造,SC造 部分 )
                     +------ 当階 ------+ +--------------- 下階 ----------------+
    階名                 大梁     小梁        柱      間柱        壁    ブレ－ス   層合計    LL面積  [ /LL面積 ]  上階合計
    XXXXXX    重量   XXXXXXX.X XXXXXXX.X XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX [XXXXX.XX ] XXXXXXXXXX
              面積   XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X           [XXXXX.XX ] XXXXXXXXXX
        (重量/面積)    ( X.XXX ) ( X.XXX ) ( X.XXX ) ( X.XXX ) ( X.XXX ) ( X.XXX ) ( X.XXX )
    -----------------------------------------------------------------------------------------------------------------------
    TOTAL     重量   XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX [XXXXX.XX ]
             ( % )     ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )
              面積   XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX [XXXXX.XX ]
             ( % )     ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )
    ■ 節点鉛直荷重(応力解析用) (KN)
    階名    +----架構用-----+ +--- 地震用-----+ +---固定荷重----+ +架構用積載荷重-+ +- 地震用積載 --+ ( 固定荷重 DL )    (その他の架構用積載荷重)
            DL+LL1  [ /面積]  DL+LLE  [ /面積]     DL   [ /面積]    LL1   [ /面積]    LLE   [ /面積]   床梁                LL2      LL3      LL4
           (全体比%)         (全体比%)         (全体比%)          (DL比%)           (DL比%)
                    上階合計          上階合計          上階合計          上階合計          上階合計   柱壁等 上階合計
    XXXXXX XXXXXXXX [XXX.XX] XXXXXXXX [XXX.XX] XXXXXXXX [XXX.XX] XXXXXXXX [XXX.XX] XXXXXXXX [XXX.XX] XXXXXXXX          XXXXXXXX XXXXXXXX XXXXXXXX
            (XXX.X )          (XXX.X )          (XXX.X )          (XXX.X )          (XXX.X )
                    XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX XXXXXXXX XXXXXXXX
    ---------------------------------------------------------------------------------------------------------------------------------------------
     TOTAL          XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
    [ /面積]        [XXX.XX]          [XXX.XX]          [XXX.XX]          [XXX.XX]          [XXX.XX]          [XXX.XX] [XXX.XX] [XXX.XX] [XXX.XX]

    */
    return QStringList{};
}

QStringList Prcard::getAnalysis(){
    /*
    XXX ZZ- ZZ  解析条件 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

    XXX ZZ- ZZ  共通解析モデル条件 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

    XXX ZZ- ZZ  1次設計用解析モデル条件 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

    XXX ZZ- ZZ  1次設計用解析ケース XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

    XXX ZZ- ZZ  断面検討ケース XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

    XXX ZZ- ZZ  2次設計用解析モデル条件 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getFloor(){
    /*
    XXX ZZ- ZZ  階, 耐震計算ル－トXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    ■ FOOR GROUP   GL.FL=XXXXXX ( GL+ XX.XXXX(M) )   R.FL=XXXXXX
      FLNO.  NAME   TYPE  所属    形状H    解析H STR1 STR2 SEQ   SE-MASS   C-AREA  W-AREA1  W-AREA2 ZW-AREA1 ZW-AREA2     AREA  -- 層間変形計算用代表節点 --
                        (補助階)   (M)      (M)                    (KN)     (M2)     (M2)     (M2)     (M2)     (M2)     (M2)   ---- 当階 ---  ---- 上階 ---
    (XXXXXX) XXXXXX XXXX  ----  XX.XXXX XX.XXXX  XXX  XXX  XXX  XXXXXX.X XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX  XXXXXX XXXXXX  XXXXXX XXXXXX
                                                          TOTAL=XXXXXX.X
    XXX ZZ- ZZ  通り XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    ■ GRID GROUP   TYPE=XXXXXX   X0=XXXXX.XXXX(M)  Y0=XXXXX.XXXX(M)  ANGLE-S=XXXX.XX(DEG)  ANGLE-E=XXXX.XX(DEG)
      GRNO.  NAME         SPAN
                       (M,DEG)
    (XXXXXX) XXXXXX XXXXX.XXXX
    */
            return QStringList{};
        }

QStringList Prcard::getMaterial(){
    QStringList ret;
    /*
    XXX ZZ- ZZ  材料定数・許容応力度 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    ret.append(getSteelMaterial());
    ret.append(getConcreteMaterial());
    ret.append(getRebarMaterial());
    /*
    ■ 鉄筋
            NAME  MAT.NAME    AREA     PAI
                             (CM2)    (CM)
    (XXXXXX) XXX  XXXXXXXX XXXX.XX XXXX.XX
    */
    return ret;
}

QStringList Prcard::getSteelMaterial(){
    /*
    ■ 鋼材料               UNIT=N/MM2
                                                   +--PERMANENT--+ +--TEMPORARY--+ +---ULTIMATE--+
             NAME          F       E         G       COMP.   SHEAR   COMP.   SHEAR   COMP.   SHEAR
                                                     TENS.           TENS.           TENS.
    (     1) XXXXXXXX XXXXXX.X XXXXXXXX. XXXXXXXX. XXXXX.X XXXXX.X XXXXX.X XXXXX.X XXXXX.X XXXXX.X
    */
    return QStringList{};
}

QStringList Prcard::getConcreteMaterial(){
    /*
    ■ コンクリ－ト材料     UNIT=N/MM2
                                                   +-------PERMANENT---------+ +--------TEMPORARY--------+ +----------ULTIMATE--------+
             NAME          F       E         G      COMP.  SHEAR     BOND       COMP.  SHEAR     BOND        COMP.  SHEAR  TENS.   BOND    N
                                                                   TOP GENERAL                 TOP GENERAL
    (XXXXXX) XXXXXXXX XXXXXX.X XXXXXXXX. XXXXXXXX. XXX.XX XXX.XX XXX.XX XXX.XX XXX.XX XXX.XX XXX.XX XXX.XX  XXX.XX XXX.XX XXX.XX XXX.XX XX.X
    */
    return QStringList{};
}

QStringList Prcard::getRebarMaterial(){
    /*
    ■ 鉄筋材料             UNIT=N/MM2
                                 +--PERMANENT--+ +--TEMPORARY--+ +---ULTIMATE--+
             NAME        ( E )     COMP.   SHEAR   COMP.   SHEAR   COMP.   SHEAR
                                   TENS.           TENS.           TENS.
    (XXXXXX) XXXXXXXX XXXXXXXXX  XXXXX.X XXXXX.X XXXXX.X XXXXX.X XXXXX.X XXXXX.X
    */
    return QStringList{};
}

QStringList Prcard::getAList(){
    /*
    XXX ZZ- ZZ  大梁リスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    +------------------+-----------------------------------+-----------------------------------+-----------------------------------+
    |                  |  LEFT END                         |  CENTER                           |  RIGHT END                        |
    +------------------+-----------------------------------+-----------------------------------+-----------------------------------+
    | G1-1-3           |  SRC                              |  SRC                              |  SRC                              |
    | (NO.XXX)         |                                   |                                   |                                   |
    | BXD              |  1000X1000(1000X1000)  (FC21    ) |  1000X1000             (FC21    ) |  1000X1000             (FC21    ) |
    | STEEL            |  WH-1000X 200X 12X 25  (SN490   ) |  WH-1000X 200X 12X 25  (SN490   ) |  WH-1000X 200X 12X 25  (SN490   ) |
    | UPPER-BAR        |   2-D25( 75) 2-D25( 83) 2-D25( 91)|   2-D25( 75) 2-D25( 83) 2-D25( 91)|   2-D25( 75) 2-D25( 83) 2-D25( 91)|
    | LOWER-BAR        |   2-D25( 75) 2-D25( 83) 2-D25( 91)|   2-D25( 75) 2-D25( 83) 2-D25( 91)|   2-D25( 75) 2-D25( 83) 2-D25( 91)|
    | STIRRUP(PW)      |   2-D13@200( .28)                 |   2-D13@200( .28)                 |   2-D13@200( .28)                 |
    | D+(PT+),D-(PT-)  |         500( .32)       500( .32) |         500( .32)       500( .32) |         500( .32)       500( .32) |
    | I*(CM)  ,AF(CM2) |  XXXXXXXXXXXX.XX XXXXXXXXXXXX.XX  |  XXXXXXXXXXXX.XX XXXXXXXXXXXX.XX  |  XXXXXXXXXXXX.XX XXXXXXXXXXXX.XX  |
    | EI(KNM2),I(CM4E5)|  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |
    | GAS(KN) ,AS(CM2) |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |
    +------------------+-----------------------------------+-----------------------------------+-----------------------------------+


    ##    44   132    17     0              PR022      43
    XXX ZZ- ZZ  柱リスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    +------------------+-------------------------------------------------------+-------------------------------------------------------+
    |                  |  BOTTOM                                               |  TOP                                                  |
    +------------------+-------------------------------------------------------+-------------------------------------------------------+
    | C1-1-3           |  SRC                                                  |                                                       |
    | (NO.XXX)         |                                                       |                                                       |
    | DX X DY          |  1000X1000                                 (FC21    ) |  1000X1000                                 (FC21    ) |
    |                  |  +----Y DIRECTION----+ +----Z DIRECTION----+          |  +----Y DIRECTION----+ +----Z DIRECTION----+          |
    | STEEL            |  WH-1000X 200X 12X 25  WH-1000X 200X 12X 25(SN490   ) |  P -1000X  25 (CFT)                        (SN490   ) |
    | BAR(D,PT)        |  4-D25( 75)(XXXX, .83) 4-D25( 75)(XXXX, .83)          |  4-D25( 75)(  83, .25) 4-D25( 75)(  83, .25)          |
    | INNER-BAR        |  4-D25( 83)            4-D25( 83)                     |  4-D25( 83)            4-D25( 83)                     |
    | HOOP(PW) ,AT(CM2)|  2-D13@100( .28)       2-D13@100( .28)  XXXX.XX(X.XX) |  2-D13@100( .28)       2-D13@100( .28)  XXXX.XX(X.XX) |
    | IE,I*(CM),AF(CM2)|  XXX.XX XXX.XX XXX.XX  XXX.XX XXX.XX XXX.XX           |  XXXXXX XXXXXX XXXXXX  XXXXXX XXXXXX XXXXXX           |
    | EI(KNM2),I(CM4E5)| XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |
    | GAS(KN) ,AS(CM2) | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |
    | EAX,AX  ,GJ,J    | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |
    +------------------+-------------------------------------------------------+-------------------------------------------------------+


    ##    45   132    17     0              PR025      43
    XXX ZZ- ZZ  ブレ－スリスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX


    +------------------+-------------------------------------------------------+
    | C1-1-3           |  SRC                                                  |
    | (NO.XXX)         |                                                       |
    | DX X DY          |  1000X1000                                 (FC21    ) |
    |                  |  +----Y DIRECTION----+ +----Z DIRECTION----+          |
    | STEEL            |  WH-1000X 200X 12X 25  WH-1000X 200X 12X 25(SN490   ) |
    | BAR(D,PT)        |  4-D25( 75)(XXXX, .83) 4-D25( 75)(XXXX, .83)          |
    | INNER-BAR        |  4-D25( 83)            4-D25( 83)                     |
    | HOOP(PW) ,AT(CM2)|  2-D13@100( .28)       2-D13@100( .28)  XXXX.XX(X.XX) |
    | IE,I*(CM),AF(CM2)|  XXX.XX XXX.XX XXX.XX  XXX.XX XXX.XX XXX.XX           |
    | EI(KNM2),I(CM4E5)| XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |
    | GAS(KN) ,AS(CM2) | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |
    | EAX,AX  ,GJ,J    | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |
    +------------------+-------------------------------------------------------+


    ##    46   132     8     0
    XXX ZZ- ZZ  壁リスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    +----------+--------+----------+-----+--------------------------+
    | NAME     | THICK  | MATERIAL | S/W |        WALL-BAR       PW |
    |          |  (MM)  |          |     |                      (%) |
    +----------+--------+----------+-----+--------------------------+
    | XXXXXXXX | XXXX.X | XX       |  W  | V-BAR D10D13 @200 ( .36) |
    |          |        | XXXXXXXX |     | H-BAR D10D13 @200 ( .36) |
    +----------+--------+----------+-----+--------------------------+

    ##    47   132     5     0
    XXX ZZ- ZZ  開口リスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
             NAME     L/R B/T      X       Y      L0      H0
                                 (MM)    (MM)    (MM)    (MM)
    (XXXXXX) XXXXXXXX  X   X  XXXXX.X XXXXX.X XXXXX.X XXXXX.X
                       X   X  XXXXX.X XXXXX.X XXXXX.X XXXXX.X


    ##    51   132     7     0
    XXX ZZ- ZZ  S露出柱脚リスト XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
                                                 /----------- ベースプレート -----------/ /--- アンカーボルト ----/
            NAME     MAKER      規格                 B      D     T    hy    hz  C材種     ny  nz  径     F     lb
                                                   (MM)   (MM)  (MM)  (MM)  (MM)                   (N/mm2)   (mm)
    (XXXXX) XXXXXXXX XXXXXXXXXX XXXXXXXXXXXXXXXX XXXX.X XXXX.X XXX.X XXX.X XXX.X XXXXXXXX XXX XXX XXX XXXX.X XXXX.X
       MNO.      I      J LIST             FL     FR     GR1    GR2    IorJ  BASE
    (XXXXX) XXXXXX XXXXXX XXXXXXXXXXXXXXXX XXXXXX XXXXXX XXXXXX XXXXXX   X   XXXXXXX



    ##    52   132     7     0
    XXX ZZ- ZZ  SRC非埋込柱脚リスト XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
                     /-------------------------------- ベースプレート ------------------------------/ /------- アンカーボルト ---------/
            NAME     形状          B      D     T   hy1   hy2   hz1   hz2   sml    B0    D0  C材種    配置    ny1 nz1 ny2 nz2 径      F
                                 (MM)   (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)                                     (N/mm2)
    (XXXXX) XXXXXXXX XXXXXXXXX XXXX.X XXXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXXXXXXX XXXXXXX XXX XXX XXX XXX XXX XXXX.X
       MNO.      I      J LIST             FL     FR     GR1    GR2    BASE
    (XXXXX) XXXXXX XXXXXX XXXXXXXXXXXXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX
    */
            return QStringList{};
        }

QStringList Prcard::getJoint(){
    /*
    XXX ZZ- ZZ  節点 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
               NO. FL     GR1    GR2      0:FREE 1:FIX   X-COORD   Y-COORD   Z-COORD RGDFL    SEMASS FL     FR1    FR2    FR3    FR4
                                         XYZ RXYZ GXYZ     (M)       (M)       (M)             (KN)
    (XXXXX) XXXXXX-      -      -        XXX  XXX  XXX XXXX.XXXX XXXX.XXXX XXXX.XXXX XXXXXX XXXXXX.X XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX
    */
            return QStringList{};
        }
QStringList Prcard::getIndex(){
    /*
    XXX ZZ- ZZ  節点( MATRIX INDEX) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
               NO. FL     GR1    GR2      X-COORD   Y-COORD   Z-COORD     DX     DY     DZ     RX     RY     RZ     GX     GY     GZ
                                            (M)       (M)       (M)                 (*****:FIX  <0:DEPENDENT)
    (XXXXX) XXXXXX-      -      -       XXXX.XXXX XXXX.XXXX XXXX.XXXX  *****  *****  *****  *****  *****  *****  *****  *****  *****
    */
    return QStringList{};
}

QStringList Prcard::getD3(){
    /*
    XXX ZZ- ZZ  従属変位 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getD4(){
    /*
XXX ZZ- ZZ  剛床の特性 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
+------+--------+------------------+----------------------------+--------------+------------------+-------------+-----------------+
| SEQ. |  R.FL  |   CENTER JOINT   |   CENTER OF SEISMIC MASS   | SEISMIC MASS | ROTATIVE INERTIA | SEISMIC MASS| ROTATIVE INERTIA|
|  NO. |  NAME  | (SEQ.NO.)   NAME |   X(M)     Y(M)     Z(M)   |     (KN)     |      (kN*M2)     |     (t)     |      (t*M2)     |
+------+--------+------------------+----------------------------+--------------+------------------+-------------+-----------------+
| XXXX | XXXXXX | ( XXXXX ) XXXXXX | XXXX.XXX XXXX.XXX XXXX.XXX | XXXXXXXXXX.X | XXXXXXXXXXXXXX.X | XXXXXXXXX.X | XXXXXXXXXXXXX.X |
+------+--------+------------------+----------------------------+--------------+------------------+-------------+-----------------+                                                           TOTAL  XXXXXXXXXX.X
    */
    return QStringList{};
}

QStringList Prcard::getD4Joint(){
    /*
    XXX ZZ- ZZ  剛床節点 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getMember(){
    /*
XXX ZZ- ZZ  部材(柱,梁,ブレ－ス) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
 MSEQ.   I-JNT  J-JNT LIST-NAME
(XXXXX) XXXXXX XXXXXX XXXXXXXXXXXXXXXX
    */
    return QStringList{};
}


QStringList Prcard::getFrame(){
    /*
XXX ZZ- ZZ  架構の代表節点,計算角度 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
                XXXXXX ( ANGLE = XXXX.XX )              XXXXXX ( ANGLE = XXXX.XX )              XXXXXX ( ANGLE = XXXX.XX )
FLOOR       JSEQ.    JNO. FL     GR1    GR2         JSEQ.    JNO. FL     GR1    GR2         JSEQ.    JNO. FL     GR1    GR2
XXXXXX      XXXXX  XXXXXX XXXXXX XXXXXX XXXXXX      XXXXX  XXXXXX XXXXXX XXXXXX XXXXXX      XXXXX  XXXXXX XXXXXX XXXXXX XXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getWall(){
    /*
XXX ZZ- ZZ  耐力壁形状 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

XXX ZZ- ZZ  耐力壁周囲節点 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

XXX ZZ- ZZ  耐力壁周囲部材 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

XXX ZZ- ZZ  耐力壁周囲部材角度 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getZwall(){
    /*
XXX ZZ- ZZ  雑壁形状 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

XXX ZZ- ZZ  雑壁周囲節点 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

XXX ZZ- ZZ  雑壁周囲部材 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

XXX ZZ- ZZ  雑壁周囲部材角度 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getSlab(){
    /*
XXX ZZ- ZZ  スラブ形状 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getPanel(){
    /*
XXX ZZ- ZZ  パネル形状 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getSpringList(){
    /*
XXX ZZ- ZZ  ばねリスト XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getJointSpring(){
    /*
XXX ZZ- ZZ  節点ばね XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
SEQ.NO.   JNO. FL     GR1    GR2  AX/RT  PROP    V-X    V-Y    V-Z TYPE      K1         K2         K3         Y1+      Y1-      Y2+      Y2-
                                                                  AXIAL : (KN/M)     (KN/M)     (KN/M)       (KN)     (KN)     (KN)     (KN)
                                                                  ROT.  : (KN*M/RAD) (KN*M/RAD) (KN*M/RAD)   (KN*M)   (KN*M)   (KN*M)   (KN*M)
(XXXXX) XXXXXX-      -      -       RT XXXXXX XX.XXX XX.XXX XX.XXX
    */
    return QStringList{};
}

QStringList Prcard::getRlForSpring(){
    /*
XXX ZZ- ZZ  ばね剛域 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getAxialSpring(){
    /*
XXX ZZ- ZZ  軸ばね XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getShearSpring(){
    /*
XXX ZZ- ZZ  せん断ばね XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getMssSpring(){
    /*
XXX ZZ- ZZ  MSSばね XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getBsSpring(){
    /*
XXX ZZ- ZZ  曲げせん断ばね XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getTorqueSpring(){
    /*
XXX ZZ- ZZ  ねじればね XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getWallSpring(){
    /*
XXX ZZ- ZZ  壁式ばね XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getMemberEndSpring(){
    /*
XXX ZZ- ZZ  材端回転ばね XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
部材番号 節点番号I,J  階     -      通り   通り   符号        位置 タイプ    K1         K2         K3         Y1+      Y1-      Y2+      Y2-
                      階     架構   通り   通り                           (KN*M/RAD) (KN*M/RAD) (KN*M/RAD)   (KN*M)   (KN*M)   (KN*M)   (KN*M)
(XXXXX) XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXXXXXX XXX XXXXXX|----->
    */
    return QStringList{};
}

QStringList Prcard::getSeismicLoad(){
    /*
XXX ZZ- ZZ  地震荷重 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ 荷重名     XXXXXX           荷重タイプ XXX
                               GL-FLOOR = XXXXXX   ROOF-FLOOR = XXXXXX   ( XXXXXXXX HEIGHT )
   Z = -----   TC(S) = -----   RT = -----   T1(S) = -----   C0 = -----   Ki0= -----
+--------+---------+---------+-----+---------+--------+--------+--------+--------+-----------+
| FLOOR  | TOTAL.H | FLOOR.H | STR |   WI    |  ALPI  |   AI   |   CI   |   KI   |     QI    |
|        |   (M)   |   (M)   |     |  (KN)   |        |        |        |        |    (KN)   |
+--------+---------+---------+-----+---------+--------+--------+--------+--------+-----------+
| XXXXXX | XXX.XXX |         |     | XXXXX.X |        |        |        | XX.XXX |           |
|        |         |  XX.XXX | XXX |         | ------ | ------ | XX.XXX |        | XXXXXXX.X |
+--------+---------+---------+-----+---------+--------+--------+--------+--------+-----------+
+--------+---------+---------+-----+---------+--------+--------+--------+-----------+----+----+----+------+------+------+--------+-----------+
| FLOOR  | TOTAL.H | FLOOR.H | STR |   WI    |  ALPI  |   AI   |   CI   |    QUD    | FR | WR | BR |  DS  |  FE  |  FS  |   KI   |     QI    |
|        |   (M)   |   (M)   |     |  (KN)   |        |        |        |   (KN)    |    |    |    |      |      |      |        |    (KN)   |
+--------+---------+---------+-----+---------+--------+--------+--------+-----------+----+----+----+------+------+------+--------+-----------+
| XXXXXX | XXX.XXX |         |     | XXXXX.X |        |        |        |           |    |    |    |      |      |      | XX.XXX |           |
|        |         |  XX.XXX | XXX |         | XX.XXX | XX.XXX | XX.XXX | XXXXXXX.X | XX | XX | XX | X.XX | X.XX | X.XX |        | XXXXXXX.X |
+--------+---------+---------+-----+---------+--------+--------+--------+-----------+----+----+----+------+------+------+--------+-----------+
    */
    return QStringList{};
}

QStringList Prcard::getJointLoad(){
    /*
XXX ZZ- ZZ  節点荷重 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ 荷重名     XXXXXX
           NO. FL     GR1    GR2      X-COORD   Y-COORD   Z-COORD         FX          FX          FX          MX          MY          MZ
                                        (M)       (M)       (M)          (KN)        (KN)        (KN)       (KNm)       (KNm)       (KNm)
(XXXXX) XXXXXX-      -      -       XXXX.XXXX XXXX.XXXX XXXX.XXXX XXXXXXXX.XX XXXXXXXX.XX XXXXXXXX.XX XXXXXXXX.XX XXXXXXXX.XX XXXXXXXX.XX
    */
    return QStringList{};
}

QStringList Prcard::getFrameModel(){
    /*
XXX ZZ- ZZ  架構毎のモデル化条件 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ XXXXXX FRAME  GIRDER **                                                         KY'=EIZ*(IZ/IZ0)(+Z)/L    KZ'=EIY*(IY/IY0)(+Z)/L
■ XXXXXX FRAME  COLUMN **                                                         KY'=EIZ*(IZ/IZ0)(+Z)/L    KZ'=EIY*(IY/IY0)(+Z)/L
■ XXXXXX FRAME  BRACE  **
  SEQ.      I      J   FL    FRAME   GR1   GR2    LIST                         L(M)
          FLI   RLYI   RLYJ   MZQY   I-T-J  +W MODEL     MDL   EIZ  (IZ/IZ0)   EIZ'     GAY  (AY/AY0)    KY'     EAX  (AX/AX0)     GJ  (JX/JX0)
          FLJ   RLZI   RLZJ   MYQZ   I-T-J  +W                 EIY  (IY/IY0)   EIY'     GAZ  (AZ/AZ0)    KZ'
                 (M)    (M)                                (10-5KN*M2)               (10-6KN)       (10-4KN*M) (10-6KN)       (10-5KN*M2)
                                                                          Z-MS : NMS  W-LIST   ANGLE      L1      L2       Y0      Z0
                                                                                               (DEG)     (M)     (M)      (M)     (M)
(XXXXX) XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXXXXXXXXXX            XX.XXX
        XX.XXX XX.XXX XX.XXX  MZQY  +-----+ +W XXXXXXXXXX X XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX) XXX.XXX (XXX.XX)
        XX.XXX XX.XXX XX.XXX  MYQZ  +-----+ +W              XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX)XXXX.XXX
                                               XXXXXXXXXX X XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX) XXX.XXX (XXX.XX)
                                                            XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX)XXXX.XXX
                                                                        Z-MS-I : XXX XXXXXXXX XXXX.XX XXX.XXX XXX.XXX  XXX.XXX XXX.XXX
                                                                        Z-MS-J : XXX XXXXXXXX XXXX.XX XXX.XXX XXX.XXX  XXX.XXX XXX.XXX
■ XXXXXX FRAME  WALL **                                                          BETA'=GAMS*BETA
  SEQ.  FL     FRAME  GR1    GR2    STR  LIST
        J1     J2     J3     J4        L      H      T   OPEN    TYPE ROP  GAMS  GAMU  MODEL      E/P NMS NMS BETA  BETA' A/A0  I/I0  WG/WG0
                                      (M)    (M)    (M)                                                B   T
(XXXXX) XXXXXX XXXXXX XXXXXX XXXXXX XXX XXXXXXXX
        XXXXXX-XXXXXX-XXXXXX-XXXXXX XX.XXX XX.XXX XX.XXX XXXXXXXX XX X.XXX X.XXX X.XXX XXXXXXXXXX  X  XX  XX  XX.XX XX.XX XX.XX XX.XX XXX.XX
                                                                                       XXXXXXXXXX  X  XX  XX  XX.XX XX.XX XX.XX XX.XX XXX.XX
■ XXXXXX FRAME  Z-WALL **
        FL     GR1    GR2         LIST
        J1     J2     J3     J4        L      H      T   OPEN      ROP  MODEL      BETA  A/A0  I/I0
                                      (M)    (M)    (M)
(XXXXX) XXXXXX XXXXXX XXXXXX     XXXXXXXX
        XXXXXX-XXXXXX-XXXXXX-XXXXXX XX.XXX XX.XXX XX.XXX XXXXXXXX X.XXX XXXXXXXXXX XX.XX XX.XX XX.XX
                                                                        XXXXXXXXXX XX.XX XX.XX XX.XX
    */
    return QStringList{};
}

QStringList Prcard::getQuColumn(){
    /*
XXX ZZ- ZZ  柱のせん断耐力 ( MODEL-FILE : (DIR.1)XXXXXXXXXX (DIR.2)XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
        Y-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                      WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
        Z-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                      WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
                        +-----BOTTOM DIR.1----+  +-----BOTTOM DIR.2----+     +-----TOP DIR.1-------+  +-----TOP DIR.2-------+
                        +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+     +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+
          Q0  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          QU  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X

XXX ZZ- ZZ  柱のせん断耐力 ( MODEL-FILE : (DIR.1)XXXXXXXXXX (DIR.2)XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
          B X D          XXXXX  X  XXXXX      (XXXXXXXX)                      XXXXX  X  XXXXX      (XXXXXXXX)
          Y- BAR         4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1
             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX                        AT=XXX.XX(X.XX) D=XXXX J=XXXX
             HOOP PW(%)  2-D13-@100          PW= .23                          2-D10-@180          PW= .23
          Z- BAR         4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1
             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX                        AT=XXX.XX(X.XX) D=XXXX J=XXXX
             HOOP PW(%)  2-D13-@100          PW= .23                          2-D10-@180          PW= .23
                        +-----BOTTOM DIR.1----+  +-----BOTTOM DIR.2----+     +-----TOP DIR.1-------+  +-----TOP DIR.2-------+
                        +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+     +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+
          Q0  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          QE  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          M/QD           XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX
          PT  (%)        XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX
          SWY (N/MM2)    XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          S0  (N/MM2)    XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          B   (CM)       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX
          J   (CM)       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX
          QU  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X

XXX ZZ- ZZ  柱のせん断耐力 ( MODEL-FILE : (DIR.1)XXXXXXXXXX (DIR.2)XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
          B X D          XXXXX  X  XXXXX      (XXXXXXXX)                      XXXXX  X  XXXXX      (XXXXXXXX)
          Y- BAR         4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1
             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX                        AT=XXX.XX(X.XX) D=XXXX J=XXXX
             HOOP PW(%)  2-D13-@100          PW= .23                          2-D10-@180          PW= .23
             STEEL       WH-1000X 201X 12X 20 (XXXXXXXX)                      WH-1000X 201X 12X 20 (XXXXXXXX)
          Z- BAR         4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1
             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX                        AT=XXX.XX(X.XX) D=XXXX J=XXXX
             HOOP PW(%)  2-D13-@100          PW= .23                          2-D10-@180          PW= .23
             STEEL       WH-1000X 201X 12X 20 (XXXXXXXX)                      WH-1000X 201X 12X 20 (XXXXXXXX)
                        +-----BOTTOM DIR.1----+  +-----BOTTOM DIR.2----+     +-----TOP DIR.1-------+  +-----TOP DIR.2-------+
                        +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+     +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+
          Q0   (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          QE   (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          MBQD           XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X       XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X
          FS   (KN/MM2)  XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          B    (CM)      XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X       XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X
          B'   (CM)      XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X       XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X
          J    (CM)      XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X       XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X
          RSY  (KN/MM2)  XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          RQU1 (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          RQU2 (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          SQU1 (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          SQU2 (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          QU   (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X

XXX ZZ- ZZ  柱のせん断耐力 ( MODEL-FILE : (DIR.1)XXXXXXXXXX (DIR.2)XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SC-  L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
          STEEL          P -1000X 20          (XXXXXXXX)                      P -1000X201          (XXXXXXXX)
                        +-----BOTTOM DIR.1----+  +-----BOTTOM DIR.2----+     +-----TOP DIR.1-------+  +-----TOP DIR.2-------+
                        +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+     +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+
          Q0  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
          QU  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X
*/
    return QStringList{};
}

QStringList Prcard::getQuGirder(){
    /*
XXX ZZ- ZZ  大梁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
          STEEL          WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)
                        +--------- LEFT END ---------+     +--------- CENTER -----------+     +-------- RIGHT END ---------+
          Q0  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          QU  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X

XXX ZZ- ZZ  大梁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
          B X D          XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)
          UPPER-BAR      2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX
          LOWER-BAR      2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX
          STIRRUP PW(%)  2-D10-@180          PW= .23        2-D10-@180          PW= .23        2-D10-@180          PW= .23
                        +--------- LEFT END ---------+     +--------- CENTER -----------+     +-------- RIGHT END ---------+
          Q0  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          QE  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          M/QD                    XXXXXX.XX                          XXXXXX.XX                          XXXXXX.XX
          PT  (%)                 XXXXXX.XX                          XXXXXX.XX                          XXXXXX.XX
          SWY (KN/CM2)            XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          B   (CM)                XXXXXX.XX                          XXXXXX.XX                          XXXXXX.XX
          J   (CM)                XXXXXX.XX                          XXXXXX.XX                          XXXXXX.XX
          QU  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X

XXX ZZ- ZZ  大梁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
          B X D          XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)
          UPPER-BAR      2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX
          LOWER-BAR      2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX
          STIRRUP PW(%)  2-D10-@180          PW= .23        2-D10-@180          PW= .23        2-D10-@180          PW= .23
          STEEL          WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)
                        +--------- LEFT END ---------+     +--------- CENTER -----------+     +-------- RIGHT END ---------+
          Q0   (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          QE   (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          MBQD                    XXXXXX.XX                          XXXXXX.XX                          XXXXXXX.X
          FS   (KN/CM2)           XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          B    (CM)               XXXXXX.XX                          XXXXXX.XX                          XXXXXXX.X
          B'   (CM)               XXXXXX.XX                          XXXXXX.XX                          XXXXXXX.X
          J    (CM)               XXXXXX.XX                          XXXXXX.XX                          XXXXXXX.X
          RSY  (KN/CM2)           XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          RQU1 (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          RQU2 (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          SQU1 (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          SQU2 (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
          QU   (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X
    */
    return QStringList{};
}

QStringList Prcard::getNuBrace(){
    /*
XXX ZZ- ZZ  ブレ－スの軸耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SBR- L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
        Y-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
          W/T            B/F=XX.X  D/W=XX.X
        Z-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
          W/T            B/F=XX.X  D/W=XX.X
        COMPRESSION      LY=XXXXX. IY=XX.XX  RY=XXX.X
                         LZ=XXXXX. IZ=XX.XX  RZ=XXX.X

          NT  (KN)       XXXXXXX.X
          NC  (KN)       XXXXXXX.X
    */
    return QStringList{};
}

QStringList Prcard::getQuWall(){
    /*
XXX ZZ- ZZ  耐力壁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -S-    L=XX.XXX(M)  L0=XX.XXX(M)
   T X L0          200 X 5500          (XXXXXXXX)      QU =XXXXXX.X(KN)
   GAMMA          GAMMA=X.XXX                          TAU=XXXXX.XX(KN/CM2)

XXX ZZ- ZZ  耐力壁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -RC-   L=XX.XXX(M)  L0=XX.XXX(M)
   B X D, T X L   600 X 600 ( Y )      (XXXXXXXX)    200 X 5500           (XXXXXXXX)    600 X 600 ( Y )      (XXXXXXXX)
   BAR(Y,Z)       4-D25  4-D25  IN 4-D25  4-D25      2-D10D10-@200       PW=XX.XX(%)    4-D25  4-D25  IN 4-D25  4-D25
   AT,PT,GAMMA    AT=XXX.XX(CM2)   PT=XX.XX(%)       GAMMA=X.XXX                        AT=XXX.XX(CM2)   PT=XX.XX(%)
                 +--------- L COLUMN ---------+     +---------- WALL ------------+     +-------- R COLUMN ----------+
                  BE=XX.XXX(M)  DD=XX.XXX(M) JJ=XX.XXX(M) PT=XX.XX(%) SGMW=XXXXX.XX(KN/CM2) SGM0=XXXXX.XX(KN/CM2)
                  M/QD=X.XXX    PHAI=X.XXX                            QCR =XXXXXX.X(KN)     QU  =XXXXXX.X(KN)

XXX ZZ- ZZ  耐力壁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -SRC-  L=XX.XXX(M)  L0=XX.XXX(M)
   B X D, T X L   600 X 600(Y)         (XXXXXXXX)    200 X 5500           (XXXXXXXX)    600 X 600(Y)         (XXXXXXXX)
   HOOP,BAR-T     2-D13-@100           (PW=X.XX%)    2-D10D10-@200        (PS=X.XX%)    2-D13-@100           (PW=X.XX%)
        BAR-Y                                        2-D10D10-@200        (PS=X.XX%)
   STEEL(Y)       WH-1000X 201X 12X 20 (XXXXXXXX)                                       WH-1000X 201X 12X 20 (XXXXXXXX)
   STEEL(Z)       WH-1000X 201X 12X 20 (XXXXXXXX)                                       WH-1000X 201X 12X 20 (XXXXXXXX)
   GAMMA,C-FACT.  C-FACT.=X.XXX                      GAMMA=X.XXX                        C-FACT.=X.XXX
                 +--------- L COLUMN ---------+     +---------- WALL ------------+     +-------- R COLUMN ----------+
   QCU,QWU              QCU=XXXXXX.X(KN)                            QWU=XXXXXX.X(KN)         QCU=XXXXXX.X(KN)
    */
    return QStringList{};
}

QStringList Prcard::getPanelModel(){
    /*
XXX ZZ- ZZ  接合部パネルのモデル XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}


QStringList Prcard::getHfDistribution(){
    /*
XXX ZZ- ZZ  層せん断力分担率 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

+-------------------------------------------------------------------------------------------------+
|                CASE : XXXXXXXXXX      FILE : XXXXXXXXXX      FORCE ANGLE = XXXX.XX              |
+--------+--------------------------------------------------------+-------------------------------+
| FLOOR  |                    SHEAR FORCE (KN)                    |            RATIO (%)          |
|        |     TOTAL   COLUMN     WALL    BRACE   DAMPER    OTHER |  COL.  WALL  BRA. DAMP.  OTH. |
+--------+--------------------------------------------------------+-------------------------------+
| XXXXXX |                                                        |                               |
|        |  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X | XXX.X XXX.X XXX.X XXX.X XXX.X |
+--------+--------------------------------------------------------+-------------------------------+
    */
    return QStringList{};
}

QStringList Prcard::getStoryStiffness(){
    /*
XXX ZZ- ZZ  層剛性 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

+--------------------------------------------------------------------------------------------------------------------------------------------------------------+
|               CASE : XXXXXXXXXX      FILE : XXXXXXXXXX      FORCE ANGLE = XXXX.XX                                                                            |
+-------+--------+----------+------------++-------------------+---------+---------+---------+---------+------------++-------------------+---------+------------+
| FLOOR |  FL.H  |  SHEAR.F |    O.T.M   ||   CENTER OF S.F   | X-DISP. | Y-DISP. | THETA-Z |  DISP.  | STIFFNESS  ||   CENTER OF RIG.  |  DISP.  | STIFFNESS  |
|       |   (M)  |   (KN)   |   (KN*M)   ||   XC(M)    YC(M)  |  (MM)   |  (MM)   |  (MRAD) |  (MM)   |  (KN/CM)   ||   XK(M)    YK(M)  |  (MM)   |  (KN/CM)   |
+-------+--------+----------+------------++-------------------+---------+---------+---------+---------+------------++-------------------+---------+------------+
| XXXXXX|        |          | XXXXXXXXXX.||                   |         |         |         |         |            ||                   |         |            |
|       | XX.XXX | XXXXXX.X |            || XXXX.XXX XXXX.XXX | XXXX.XX | XXXX.XX | XX.XXXX | XXXX.XX | XXXXXXXX.X || XXXX.XXX XXXX.XXX | XXXX.XX | XXXXXXXX.X |
+-------+--------+----------+------------++-------------------+---------+---------+---------+---------+------------++-------------------+---------+------------+
    */
    return QStringList{};
}

QStringList Prcard::getMaximumDeform(){
    /*
XXX ZZ- ZZ  最大層間変形 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

+---------------------------------------------------------------------------------------------------------------------------+
|               CASE : XXXXXXXXXX      FILE : XXXXXXXXXX      FORCE ANGLE = XXXX.XX                                         |
+--------+---------------------------------------------------------------------------------------+--------------------------+
| 階名   | 形状階高 解析用階高 解析部材高さ 計算用階高 部材の水平変形 最大層間変形角の逆数 JUDGE |         POSITION         |
|        |    SH       AH          ZM       CH=ZM-AH+SH      δ             CH/δ                |                          |
|        |    (M)      (M)         (M)         (M)          (MM)                                 |   IMEM   GR1      GR2    |
+--------+---------------------------------------------------------------------------------------+--------------------------+
| XXXXXX |                                                                                       |                          |
|        |  XX.XXX    XX.XXX     XX.XXX       XX.XXX       XXXX.X         XXXXXXXXXXXX.  XXXXXXX | XXXXXX   XXXXXX   XXXXXX |
+--------+---------------------------------------------------------------------------------------+--------------------------+
    */
    return QStringList{};
}

QStringList Prcard::getStfFactor(){
    /*
XXX ZZ- ZZ  剛性率 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ 架構外雑壁剛性
■ 架構外雑壁剛性を 非考慮(階全体)
■ 架構外雑壁剛性を 考慮  (階全体)
■ 架構外雑壁剛性を 非考慮(指定領域のグループ)
■ 架構外雑壁剛性を 考慮  (指定領域のグループ)
■ 剛性率判定結果
+--------+---------------------------------------------------------------+---------------------------------------------------------------+
|        |              DIRECTION-1 ( ANGLE = XXXX.XX )                  |               DIRECTION-2 ( ANGLE = XXXX.XX )                 |
|        +---------------------------------------------------------------+---------------------------------------------------------------+
| FLOOR  |  SHEAR.F  STIFFNESS    DISP. FLOOR.H  H/DISP.  S.D.F   JUDGE  |   SHEAR.F  STIFFNESS   DISP. FLOOR.H  H/MXD    S.D.F  JUDGE   |
|        |   (KN)      (KN/CM)    (MM)    (M)                            |    (KN)      (KN/CM)   (MM)    (M)                            |
+--------+---------------------------------------------------------------+---------------------------------------------------------------+
| XXXXXX |                                                               |                                                               |
|        | XXXXXX.X XXXXXXXX.XX XXXXX.XX XX.XXX XXXXXXX. XXX.XXX XXXXXXX | XXXXXX.X XXXXXXXX.XX XXXXX.XX XX.XXX XXXXXXX. XXX.XXX XXXXXXX |
+--------+---------------------------------------------------------------+---------------------------------------------------------------+
+--------+--------+---------------------------------------------------------------+---------------------------------------------------------------+
|        |        |              DIRECTION-1 ( ANGLE = XXXX.XX )                  |               DIRECTION-2 ( ANGLE = XXXX.XX )                 |
|        |        +---------------------------------------------------------------+---------------------------------------------------------------+
| AREA   | FLOOR  |  SHEAR.F  STIFFNESS    DISP. FLOOR.H  H/DISP.  S.D.F   JUDGE  |   SHEAR.F  STIFFNESS   DISP. FLOOR.H  H/MXD    S.D.F  JUDGE   |
|        |        |   (KN)      (KN/CM)    (MM)    (M)                            |    (KN)      (KN/CM)   (MM)    (M)                            |
+--------+--------+---------------------------------------------------------------+---------------------------------------------------------------+
|        |        |                                                               |                                                               |
| XXXXXX | XXXXXX | XXXXXX.X XXXXXXXX.XX XXXXX.XX XX.XXX XXXXXXX. XXX.XXX XXXXXXX | XXXXXX.X XXXXXXXX.XX XXXXX.XX XX.XXX XXXXXXX. XXX.XXX XXXXXXX |
+--------+--------+---------------------------------------------------------------+---------------------------------------------------------------+
+--------+-------------------+---------------------------+---------------------------+
|        |                   | DIR-1 ( ANGLE = XXXX.XX ) | DIR-2 ( ANGLE = XXXX.XX ) |
|        |                   +---------------------------+---------------------------+
| FLOOR  |    AC      AWZ    |     KC           KZW      |     KC           KZW      |
|        |   (M2)     (M2)   |   (KN/CM)     (KN/CM/M2)  |   (KN/CM)     (KN/CM/M2)  |
+--------+-------------------+---------------------------+---------------------------+
| XXXXXX |                   |                           |                           |
|        | XXXX.XXX XXXX.XXX |  XXXXXXXX.X   XXXXXX.XXX  |  XXXXXXXX.X   XXXXXX.XXX  |
+--------+-------------------+---------------------------+---------------------------+
+--------+-----------------------------------------------------------+-----------------------------------------------------------+
|        |             DIRECTION-1 ( ANGLE = XXXX.XX )               |             DIRECTION-2 ( ANGLE = XXXX.XX )               |
|        +-----------------------------------------------------------+-----------------------------------------------------------+
| FLOOR  |  階全体     階全体 指定領域G  指定領域G  判定用    判定   |  階全体     階全体 指定領域G  指定領域G  判定用    判定   |
|        |雑壁非考慮 雑壁考慮 雑壁非考慮 雑壁考慮                    |雑壁非考慮 雑壁考慮 雑壁非考慮 雑壁考慮                    |
+--------+-----------------------------------------------------------+-----------------------------------------------------------+
| XXXXXX |                                                           |                                                           |
|        |  XXX.XXX   XXX.XXX   XXX.XXX   -------   XXX.XXX  XXXXXXX |  XXX.XXX   XXX.XXX   XXX.XXX   -------   XXX.XXX  XXXXXXX |
+--------+-----------------------------------------------------------+-----------------------------------------------------------+
    */
    return QStringList{};
}

QStringList Prcard::getEccFactor(){
    /*
XXX ZZ- ZZ  偏心率 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ 架構外雑壁剛性
■ 架構外雑壁剛性を 非考慮(階全体)
■ 架構外雑壁剛性を 考慮  (階全体)
■ 架構外雑壁剛性を 非考慮(指定領域)
■ 架構外雑壁剛性を 考慮  (指定領域)
■ 偏心率判定結果
+--------+-------------------+-------------------+-----------------------------------+-----------------------------------+-----------------------------------+
|        |                   |                   |                                   |  DIRECTION-1 ( ANGLE = XXXX.XX )  |  DIRECTION-2 ( ANGLE = XXXX.XX )  |
|        |                   |                   |                                   +-----------------------------------+-----------------------------------+
| FLOOR  |   CENTER OF S.F   |   CENTER OF RIG.  |    K1          K2          KR     |   RE1      E2      ECC.F.  JUDGE  |   RE2      E1      ECC.F.  JUDGE  |
|        |   XC(M)    YC(M)  |   XK(M)    YK(M)  | (KN/CM)     (KN/CM)    (KN*M2/CM) |   (M)      (M)                    |   (M)      (M)                    |
+--------+-------------------+-------------------+-----------------------------------+-----------------------------------+-----------------------------------+
| XXXXXX |                   |                   |                                   |                                   |                                   |
|        | XXXX.XXX XXXX.XXX | XXXX.XXX XXXX.XXX |XXXXXXXX.X XXXXXXXX.X XXXXXXXXXXX. | XXXX.XXX XXXX.XXX XXX.XXX XXXXXXX | XXXX.XXX XXXX.XXX XXX.XXX XXXXXXX |
+--------+-------------------+-------------------+-----------------------------------+-----------------------------------+-----------------------------------+
+--------+--------+-------------------+-------------------+-----------------------------------+---------------------------+---------------------------+
|        |        |                   |                   |                                   | DIR-1 ( ANGLE = XXXX.XX ) | DIR-2 ( ANGLE = XXXX.XX ) |
|        |        |                   |                   |                                   +---------------------------+---------------------------+
|  AREA  | FLOOR  |   CENTER OF S.F   |   CENTER OF RIG.  |    K1          K2          KR     |   RE1      E2      ECC.F. |   RE2      E1      ECC.F. |
|        |        |   XC(M)    YC(M)  |   XK(M)    YK(M)  | (KN/CM)     (KN/CM)    (KN*M2/CM) |   (M)      (M)            |   (M)      (M)            |
+--------+--------+-------------------+-------------------+-----------------------------------+---------------------------+---------------------------+
|        |        |                   |                   |                                   |                           |                           |
| XXXXXX | XXXXXX | XXXX.XXX XXXX.XXX | XXXX.XXX XXXX.XXX |XXXXXXXX.X XXXXXXXX.X XXXXXXXXXXX. | XXXX.XXX XXXX.XXX XXX.XXX | XXXX.XXX XXXX.XXX XXX.XXX |
+--------+--------+-------------------+-------------------+-----------------------------------+---------------------------+---------------------------+
+--------+-------------------+---------------------------+---------------------------+
|        |                   | DIR-1 ( ANGLE = XXXX.XX ) | DIR-2 ( ANGLE = XXXX.XX ) |
|        |                   +---------------------------+---------------------------+
| FLOOR  |    AC      AWZ    |     KC           KZW      |     KC           KZW      |
|        |   (M2)     (M2)   |   (KN/CM)     (KN/CM/M2)  |   (KN/CM)     (KN/CM/M2)  |
+--------+-------------------+---------------------------+---------------------------+
| XXXXXX |                   |                           |                           |
|        | XXXX.XXX XXXX.XXX |  XXXXXXXX.X   XXXXXX.XXX  |  XXXXXXXX.X   XXXXXX.XXX  |
+--------+-------------------+---------------------------+---------------------------+
+--------+-----------------------------------------------------------+-----------------------------------------------------------+
|        |             DIRECTION-1 ( ANGLE = XXXX.XX )               |             DIRECTION-2 ( ANGLE = XXXX.XX )               |
|        +-----------------------------------------------------------+-----------------------------------------------------------+
| FLOOR  |  階全体     階全体  指定領域  指定領域   判定用    判定   |  階全体     階全体  指定領域  指定領域   判定用    判定   |
|        |雑壁非考慮 雑壁考慮 雑壁非考慮 雑壁考慮                    |雑壁非考慮 雑壁考慮 雑壁非考慮 雑壁考慮                    |
+--------+-----------------------------------------------------------+-----------------------------------------------------------+
| XXXXXX |                                                           |                                                           |
|        |  XXX.XXX   XXX.XXX   XXX.XXX   -------   XXX.XXX  XXXXXXX |  XXX.XXX   XXX.XXX   XXX.XXX   -------   XXX.XXX  XXXXXXX |
+--------+-----------------------------------------------------------+-----------------------------------------------------------+
    */
    return QStringList{};
}

QStringList Prcard::getFrameStiffNess(){
    /*
XXX ZZ- ZZ  架構の層剛性 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

■ CASE : XXXXXXXXXX    FILE : XXXXXXXXXX    FORCE ANGLE : XXXX.XX

          XXXXXX ( ANGLE = XXXX.XX )    XXXXXX ( ANGLE = XXXX.XX )    XXXXXX ( ANGLE = XXXX.XX )   XXXXXX ( ANGLE = XXXX.XX )
FLOOR       SHEAR   DISP.     STIFF.      SHEAR   DISP.     STIFF.      SHEAR   DISP.     STIFF.     SHEAR   DISP.     STIFF.
             (KN)    (MM)    (KN/CM)       (KN)    (MM)    (KN/CM)       (KN)    (MM)    (KN/CM)      (KN)    (MM)    (KN/CM)

XXXXXX   XXXXXX.X XXXX.XX XXXXXXXX.X   XXXXXX.X XXXX.XX XXXXXXXX.X   XXXXXX.X XXXX.XX XXXXXXXX.X  XXXXXX.X XXXX.XX XXXXXXXX.X
    */
    return QStringList{};
}

QStringList Prcard::getFrDistribution(){
    /*
XXX ZZ- ZZ  架構の層せん断力分担 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
** XXXXXX FL  ( CASE : XXXXXXXXXX   FILE : XXXXXXXXXX   FORCE ANGLE : XXXX.XX )
+--------+--------+---------+------------------+------------------+------------------+------------------+------------------+------------------+
| FLOOR  | FRAME  |  ANGLE  | FRAME   (KN) (%) | BRACE   (KN) (%) | WALL    (KN) (%) | DAMPER  (KN) (%) | OTHER   (KN) (%) | TOTAL   (KN) (%) |
+--------+--------+---------+------------------+------------------+------------------+------------------+------------------+------------------+
| XXXXXX | XXXXXX | XXXX.XX |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |
+--------+--------+---------+------------------+------------------+------------------+------------------+------------------+------------------+
                       SUM   XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)
                       SHEAR XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)
    */
    return QStringList{};
}

QStringList Prcard::getFrCheckSummary(){
    /*
XXX ZZ- ZZ  断面検討結果の概要(架構毎の余裕率) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ XXXXXX FRAME  GIRDER **
■ XXXXXX FRAME  COLUMN **
■ XXXXXX FRAME  BRACE  **
  SEQ.      J1     J2 FL     FRAME  GR1    GR2            LIST     +-MD/MA( N-M INT.)-+     +-------QD/QA------+      ND/NA
                                                  CASE                (I)    (C)    (J)        (I)    (C)    (J)        (J)
(XXXXX) XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX-XXXXXX         XXXXXXXXXXXXXXXX
                                                  XXXXXXXXXX       XX.XXX XX.XXX XX.XXX XX  XX.XXX XX.XXX XX.XXX XX  XX.XXX
■ XXXXXX FRAME  WALL   **
  SEQ.      J1     J2     J3     J4 FL     FRAME  GR1    GR2    LIST
                                                                CASE              QD/QA
(XXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX-XXXXXX         XXXXXXXX
                                                                XXXXXXXXXX       XX.XXX XX
    */
    return QStringList{};
}

QStringList Prcard::getCheckSummary1(){
    /*
XXX ZZ- ZZ  部材毎の検定比 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getCheckSummary2(){
    /*
XXX ZZ- ZZ  リスト毎の検定比分布(曲げ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

XXX ZZ- ZZ  リスト毎の検定比分布(せん断) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getColumnCheck(){
    /*
XXX ZZ- ZZ  柱の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M) Lk=XX.XXX(M)
        y-鉄骨           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)  露出柱脚 XXXXXXXX   WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
          幅厚比         B/F=XX.X  D/W=XX.X               BPL XXXXxXXXXxXXX   B/F=XX.X  D/W=XX.X
          lb,I*,Af(R)    lb=XXXXX. I*=XXX.XX Af=XXX.X     CON XXXXXXXX        lb=XXXXX. I*=XXX.XX Af=XXX.X
          lb,I*,Af(L)    lb=XXXXX. I*=XXX.XX Af=XXX.X     ANB XXΦ XX x XX    lb=XXXXX. I*=XXX.XX Af=XXX.X
        z-鉄骨           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                      WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
          幅厚比         B/F=XX.X  D/W=XX.X                                   B/F=XX.X  D/W=XX.X
          lb,I*,Af(R)    lb=XXXXX. I*=XXX.XX Af=XXX.X                         lb=XXXXX. I*=XXX.XX Af=XXX.X
          lb,I*,Af(L)    lb=XXXXX. I*=XXX.XX Af=XXX.X                         lb=XXXXX. I*=XXX.XX Af=XXX.X
        圧縮検討用       ly=XXXXX. iy=XX.XX                                   ly=XXXXX. iy=XX.XX
                         lz=XXXXX. iz=XX.XX                                   lz=XXXXX. iz=XX.XX
                        +-------------------- 柱脚 --------------------+     +-------------------- 柱頭 --------------------+
  --- 曲げ軸力の検討 ---     PERM     TEMP     TEMP     TEMP     TEMP             PERM     TEMP     TEMP     TEMP     TEMP
       ft       (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       fc       (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       NL,  NS     (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT        XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT
       NAL, NAS    (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT        XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT
 z軸回 fb(R)    (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       fb(L)    (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       MzLJ, MzSJ  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MzL , MzS   (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MzAL, MzAS  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
      (MzAL0,MzAS0)(kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
 y軸回 fb(R)    (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       fb(L)    (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       MyLJ, MySJ  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MyL , MyS   (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MyAL, MyAS  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
      (MyAL0,MzAS0)(kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       検定比            XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       変動分検定比           --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
      耐力壁分担割増係数      --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
  ---- せん断の検討 ----
   y軸 QyL , QyS   (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       QyAL, QyAS  (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      (QyAL0,QyAS0)(kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
   z軸 QzL , QzS   (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       QzAL, QzAS  (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      (QzAL0,QzAS0)(kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       検定比            XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXXX.X XXXXXX.X

XXX ZZ- ZZ  柱の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
       B X D             XXXXX  X  XXXXX      (XXXXXXXX)                      XXXXX  X  XXXXX      (XXXXXXXX)
       Y- 鉄筋           4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1
          at,d,j         at=XXX.XX(X.XX) d=XXXX j=XXXX                        at=XXX.XX(X.XX) d=XXXX j=XXXX
          帯筋 pw(%)     2-D13-@100          pw= .23                          2-D10-@180          pw= .23
       Z- 鉄筋           4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1
          at,d,j         at=XXX.XX(X.XX) d=XXXX j=XXXX                        at=XXX.XX(X.XX) d=XXXX j=XXXX
          帯筋 pw(%)     2-D13-@100          pw= .23                          2-D10-@180          pw= .23
                        +-------------------- 柱脚 --------------------+     +-------------------- 柱頭 --------------------+
  --- 曲げ軸力の検討 ---     PERM     TEMP     TEMP     TEMP     TEMP             PERM     TEMP     TEMP     TEMP     TEMP
       NL,  NS     (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT        XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT
       NAL, NAS    (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT        XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT
 z軸回 MzLJ, MzSJ  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MzL , MzS   (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MzAL, MzAS  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
      (MzAL0,MzAS0)(kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
 y軸回 MyLJ, MySJ  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MyL , MyS   (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MyAL, MyAS  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
      (MyAL0,MzAS0)(kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       検定比            XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       変動分検定比           --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
      耐力壁分担割増係数      --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
  ---- せん断の検討 ----
   y軸 α1, α2               ---  X.X X.X  X.X X.X  X.X X.X  X.X X.X              ---  X.X X.X  X.X X.X  X.X X.X  X.X X.X
       α1*QyH   +QyL         --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       α2*MzY/L0+Qy0         --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       α                XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       QyL, QyL+QyH (kN) XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       QyL, QyS     (kN) XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       QyAL,QyAS    (kN) XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       QyAL,QyAS付着(kN) XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
   z軸 α1, α2               ---  X.X X.X  X.X X.X  X.X X.X  X.X X.X              ---  X.X X.X  X.X X.X  X.X X.X  X.X X.X
       α1*QzH   +QzL(kN)     --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       α2*MyY/L0+Qz0(kN)     --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       α                XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
       QzL, QzL+QzH (kN) XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       QzL, QzS     (kN) XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       QzAL,QzAS    (kN) XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       QzAL,QzAS付着(kN) XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
       検定比            XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXXX.X XXXXXX.X
       検定比   付着     XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXXX.X XXXXXX.X

XXX ZZ- ZZ  柱の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M) Lk=XX.XXX(M)
       B X D             XXXXX  X  XXXXX      (XXXXXXXX)                      XXXXX  X  XXXXX      (XXXXXXXX)
       Y- 鉄筋           4-D25( 75):Y1   2-D25( 91):Y1    非埋込柱脚          4-D25( 75):Y1   2-D25( 91):Y1    Lk/D=XXX.X 長柱
          at,d,j         at=XXX.XX(X.XX) d=XXXX j=XXXX     XXXXXXXX           at=XXX.XX(X.XX) d=XXXX j=XXXX
          帯筋 pw(%)     2-D13-@100          pw= .23       BPL XXXXxXXXXxXXX  2-D10-@180          pw= .23
          鉄骨           WH-1000X 201X 12X 20 (XXXXXXXX)   CON XXXXXXXX       WH-1000X 201X 12X 20 (XXXXXXXX)
          幅厚比,B'      B/F=XX.X  D/W=XX.X  B'=XXXXX      ANB XXΦ XX x XX   B/F=XX.X  D/W=XX.X  B'=XXXXX
       Z- 鉄筋           4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1
          at,d,j         at=XXX.XX(X.XX) d=XXXX j=XXXX                        at=XXX.XX(X.XX) d=XXXX j=XXXX
          帯筋 pw(%)     2-D13-@100          pw= .23                          2-D10-@180          pw= .23
          鉄骨           WH-1000X 201X 12X 20 (XXXXXXXX)                      WH-1000X 201X 12X 20 (XXXXXXXX)
          幅厚比,B'      B/F=XX.X  D/W=XX.X  B'=XXXXX                         B/F=XX.X  D/W=XX.X  B'=XXXXX
                        +-------------------- 柱脚 --------------------+     +-------------------- 柱頭 --------------------+
  --- 曲げ軸力の検討 ---     PERM     TEMP     TEMP     TEMP     TEMP             PERM     TEMP     TEMP     TEMP     TEMP
  軸力 NL,  NS     (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT        XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT
       NAL, NAS    (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT        XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT
 z軸回 MzLJ, MzSJ  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MzL , MzS   (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MzAL, MzAS  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
    ( MzAL0, MzAS0)(kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
 y軸回 MyLJ, MySJ  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MyL , MyS   (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MyAL, MyAS  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
    ( MyAL0, MyAS0)(kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
      検定比             XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
      変動分検定比            --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
      耐力壁分担割増係数      --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
  ---- せん断の検討 ----
  y軸 1+β               XXXXX.XX      ---      ---      ---      ---         XXXXX.XX      ---      ---      ---      ---
      α1*QyH   +QyL(kN)      --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      α2*MzY/L0+Qy0(kN)      --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
    ( α' ), α         (XXXXX.XX)XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX        (XXXXX.XX)XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
    (rQyAL),rQyAS1 (kN) (XXXXXX.X)XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X        (XXXXXX.X)XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
            rQyAS2 (kN)       --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      QyL, QyL+QyH (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      rMzAL,rMzAS (kNm)  XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
      sMzAL,sMzAS (kNm)  XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
      rQyL , rQyS  (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      rQyAL, rQyAS (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
    (rQyAL0,rQyAS0)(kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
     sQyL , sQyS   (kN)       --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
     sQyAL, sQyAS  (kN)       --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
    (sQyAL0,sQyAS0)(kN)       --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
  z軸 1+β               XXXXX.XX      ---      ---      ---      ---         XXXXX.XX      ---      ---      ---      ---
      α1*QzH   +QzL(kN)      --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      α2*MyY/L0+Qz0(kN)      --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
    ( α' ), α         (XXXXX.XX)XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX        (XXXXX.XX)XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
    (rQzAL),rQyAS1 (kN) (XXXXXX.X)XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X        (XXXXXX.X)XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
            rQyAS2 (kN)       --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      QzL ,QzL+QzH (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      rMyAL,rMyAS (kNm)  XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
      sMyAL,sMyAS (kNm)  XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
     rQzL , rQzS   (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
     rQzAL, rQzAS  (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
    (rQzAL0,rQzAS0)(kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
     sQzL , sQzS   (kN)       --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
     sQzAL, sQzAS  (kN)       --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
    (sQzAL0,sQzAS0)(kN)       --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
     検定比 RC           XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXXX.X XXXXXX.X
     検定比 S                 --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX XXXXXX.X XXXXXX.X

XXX ZZ- ZZ  柱の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -CFT- L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M) Lk=XX.XXX(M)
       鉄骨              P -1000X 20          (XXXXXXXX)  露出柱脚 XXXXXXXX   P -1000X201          (XXXXXXXX)
       D/T,As,σy        D/T=XX.X  As=XXX.X σy=XXXXX.X   BPL XXXXxXXXXxXXX   D/T=XX.X  AS=XXX.X σy=XXXXX.X  lk/D=XXX.X 長柱
       コンクリート      Fc =XXXX.X Ac=XXXX.X (XXXXXXXX)  CON XXXXXXXX        Fc =XXXX.X Ac=XXXX.X (XXXXXXXX) h0/D=XXX.X≦6
       拘束効果          cfc=XXXX.X cfc(短期)=XXXX.X      ANB XXΦ XX x XX    cfc=XXXX.X cfc(短期)=XXXX.X
                        +------------------ BOTTOM --------------------+     +-------------------- TOP ---------------------+
  --- 曲げ軸力の検討 ---     PERM     TEMP     TEMP     TEMP     TEMP             PERM     TEMP     TEMP     TEMP     TEMP
  軸力 NL,  NS     (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT        XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT
       NAL, NAS    (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT        XXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XTXXXXXX.XT
 z軸回 MzLJ, MzSJ  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MzL , MzS   (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MzAL, MzAS  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
    ( MzAL0, MzAS0)(kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
 y軸回 MyLJ, MySJ  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MyL , MyS   (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
       MyAL, MyAS  (kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
    ( MyAL0, MyAS0)(kNm) XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR        XXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XRXXXXXX.XR
   検定比                XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
   変動分検定比               --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
   耐力壁分担割増係数         --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX
  ---- せん断の検討 ----
  y軸  QyL ,  QyS  (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      sQyAL, sQyAS (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
    (sQyAL0,sQyAS0)(kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
  z軸  QzL ,  QzS  (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
      sQzAL, sQzAS (kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
    (sQzAL0,sQzAS0)(kN)  XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X XXXXXX.X
  検定比                 XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX XXXXXX.X XXXXXX.X
*/
    return QStringList{};
}

QStringList Prcard::getGirderCheck(){
    /*
XXX ZZ- ZZ  大梁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
       鉄骨              WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)
       幅厚比            B/F=XXX.X D/W=XXX.X                B/F=XXX.X D/W=XXX.X                B/F=XXX.X D/W=XXX.X
       lb,I*,Af(上)      lb=XXXXX. i*=XXX.XX Af=XXX.X       lb=XXXXX. i*=XXX.XX Af=XXX.X       lb=XXXXX. i*=XXX.XX Af=XXX.X
       lb,I*,Af(下)      lb=XXXXX. i*=XXX.XX Af=XXX.X       lb=XXXXX. i*=XXX.XX Af=XXX.X       lb=XXXXX. i*=XXX.XX Af=XXX.X
                        +----------- 左端 -----------+     +----------- 中央 -----------+     +----------- 右端 -----------+
  --- 曲げ軸力の検討 ---     PERM     TEMP     TEMP             PERM     TEMP     TEMP             PERM     TEMP     TEMP
   fb(N/mm2)(上端引張時) XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
   fb(N/mm2)(下端引張時) XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
       MLJ, MSJ (kNm)    XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU
       ML , MS  (kNm)    XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU
       MAL, MAS (kNm)    XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU
       検定比                 --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX
       変動分検定比           --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX
  ---- せん断の検討 ----
       QL , QL+QH (kN)   XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
       QL , QS    (kN)   XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
       QAL, QAS   (kN)   XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
       検定比                 --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX

XXX ZZ- ZZ  大梁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
        B X D            XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)
        上端主筋         2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
           at,d,j        at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX
        下端主筋         2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
           at,d,j        at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX
        あばら筋 pw(%)   2-D10-@180          pw= .23        2-D10-@180          pw= .23        2-D10-@180          pw= .23
                        +----------- 左端 -----------+     +----------- 中央 -----------+     +----------- 右端 -----------+
  --- 曲げ軸力の検討 ---     PERM     TEMP     TEMP             PERM     TEMP     TEMP             PERM     TEMP     TEMP
        MLJ,MSJ  (kNm)   XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU
        ML,MS    (kNm)   XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU
        MAL,MAS  (kNm)   XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU
        検定比                --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX
        変動分検定比          --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX
  ---- せん断の検討 ----
        α1, α2              ---  X.X X.X  X.X X.X              ---  X.X X.X  X.X X.X              ---  X.X X.X  X.X X.X
        α1*QH   +QL          --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
        α2*MY/L0+Q0          --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
        α               XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
        QL,  QL+QH  (kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
        QL,  QS     (kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
        QAL, QAS    (kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
        QAL, QAS付着(kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
        検定比           XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
        検定比  付着          --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX

XXX ZZ- ZZ  大梁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
         B X D           XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)
         上端主筋        2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
            at,d,j       at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX
         下端主筋        2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
            at,d,j       at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX
         あばら筋 pw(%)  2-D10-@180          pw= .23        2-D10-@180          pw= .23        2-D10-@180          pw= .23
         鉄骨            WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)
         幅厚比,B'       B/F=XX.X  D/W=XX.X  B'=XXXXX       B/F=XX.X  D/W=XX.X  B'=XXXXX       B/F=XX.X  D/W=XX.X  B'=XXXXX
                        +----------- 左端 -----------+     +----------- 中央 -----------+     +----------- 右端 -----------+
   --- 曲げ軸力の検討 --     PERM     TEMP     TEMP             PERM     TEMP     TEMP             PERM     TEMP     TEMP
         MLJ,MSJ   (kNm) XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU
         ML ,MS    (kNm) XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU
         MAL,MAS         XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU        XXXXXX.XUXXXXXX.XUXXXXXX.XU
         検定比               --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX
         変動分検定比         --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX
   ---- せん断の検討 ----
         QL,QL+QH  (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rMAL,rMAS (kNm) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         sMAL,sMAS (kNm) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
  [Root1,2] rMY/L0+(rQ0)      --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
  [Root1,2](rMY/MY)(Q0+2QH)   --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
         rQL,rQS   (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rα             XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
         rQA1      (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rQA2      (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rQA       (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         検定比 RC       XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
         sQD       (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         sQA       (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         検定比 S        XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX

XXX ZZ- ZZ  大梁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
    鉄骨                 WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)
    幅厚比               B/F=XXX.X D/W=XXX.X                B/F=XXX.X D/W=XXX.X                B/F=XXX.X D/W=XXX.X
    lb,I*,Af(上)         lb=XXXXX. i*=XXX.XX Af=XXX.X       lb=XXXXX. i*=XXX.XX Af=XXX.X       lb=XXXXX. i*=XXX.XX Af=XXX.X
    lb,I*,Af(下)         lb=XXXXX. i*=XXX.XX Af=XXX.X       lb=XXXXX. i*=XXX.XX Af=XXX.X       lb=XXXXX. i*=XXX.XX Af=XXX.X
    圧縮検討用           ly=XXXXX. iy=XX.XX  fcL=XXX.X      ly=XXXXX. iy=XX.XX  fcL=XXX.X      ly=XXXXX. iy=XX.XX  fcL=XXX.X
     fc(N/mm2)           lz=XXXXX. iz=XX.XX  fcS=XXX.X      lz=XXXXX. iz=XX.XX  fcS=XXX.X      lz=XXXXX. iz=XX.XX  fcS=XXX.X
    曲げ検討用  長期     z=XXX.X, XXX.X y=XXX.X, XXX.X      z=XXX.X, XXX.X y=XXX.X, XXX.X      z=XXX.X, XXX.X y=XXX.X, XXX.X
     fb(N/mm2)  短期     z=XXX.X, XXX.X y=XXX.X, XXX.X      z=XXX.X, XXX.X y=XXX.X, XXX.X      z=XXX.X, XXX.X y=XXX.X, XXX.X
                         +----------- 左端 -----------+     +----------- 中央 -----------+     +----------- 右端 -----------+
  --- 曲げ軸力の検討 --- XXXXXXXX XXXXXXXX XXXXXXXX         XXXXXXXX XXXXXXXX XXXXXXXX         XXXXXXXX XXXXXXXX XXXXXXXX
    NL  ,NS   (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MyL ,MyS  (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MzL ,MzS  (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    NAL ,NAS  (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MyAL,MyAS (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MzAL,MzAS (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    検定比               XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
    変動分検定比              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX
  ---- せん断の検討 ----
    QyL ,QyS  (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    QzL ,QzS  (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    QyAL,QyAS (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    QzAL,QzAS (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    検定比               XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX

XXX ZZ- ZZ  大梁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
        B X D            XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)
        上端主筋         2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
           at,d,j        at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX
        下端主筋         2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
           at,d,j        at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX
        あばら筋 pw(%)   2-D10-@180          pw= .23        2-D10-@180          pw= .23        2-D10-@180          pw= .23
                        +----------- 左端 -----------+     +----------- 中央 -----------+     +----------- 右端 -----------+
  --- 曲げ軸力の検討 ---     PERM     TEMP     TEMP             PERM     TEMP     TEMP             PERM     TEMP     TEMP
    NL  ,NS   (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MyL ,MyS  (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MzL ,MzS  (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    NAL ,NAS  (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MyAL,MyAS (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MzAL,MzAS (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    検定比               XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
    変動分検定比              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX
  ---- せん断の検討 ----
   y軸 α1, α2               ---  X.X X.X  X.X X.X              ---  X.X X.X  X.X X.X              ---  X.X X.X  X.X X.X
       α1*QyH   +QyL(kN)     --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
       α2*MzY/L0+Qy0(kN)     --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
       α                XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
       QyL, QyL+QyH (kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
       QyL, QyS     (kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
       QyAL,QyAS    (kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
       QyAL,QyAS付着(kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
   z軸 α1, α2               ---  X.X X.X  X.X X.X              ---  X.X X.X  X.X X.X              ---  X.X X.X  X.X X.X
       α1*QzH   +QzL(kN)     --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
       α2*MyY/L0+Qz0(kN)     --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
       α                XXXXX.XX XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
       QzL, QzL+QzH (kN) XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
       QzL, QzS     (kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
       QzAL,QzAS    (kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
       QzAL,QzAS付着(kN) XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
       検定比            XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
       検定比  付着      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X

XXX ZZ- ZZ  大梁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
         B X D           XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)
         上端主筋        2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
            at,d,j       at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX
         下端主筋        2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)
            at,d,j       at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX      at=XXX.XX(X.XX) d=XXXX j=XXXX
         あばら筋 pw(%)  2-D10-@180          pw= .23        2-D10-@180          pw= .23        2-D10-@180          pw= .23
         鉄骨            WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)
         幅厚比,B'       B/F=XX.X  D/W=XX.X  B'=XXXXX       B/F=XX.X  D/W=XX.X  B'=XXXXX       B/F=XX.X  D/W=XX.X  B'=XXXXX
                        +----------- 左端 -----------+     +----------- 中央 -----------+     +----------- 右端 -----------+
  --- 曲げ軸力の検討 ---     PERM     TEMP     TEMP             PERM     TEMP     TEMP             PERM     TEMP     TEMP
    NL  ,NS   (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MyL ,MyS  (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MzL ,MzS  (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    NAL ,NAS  (kN)       XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MyAL,MyAS (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    MzAL,MzAS (kNm)      XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    検定比               XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
    変動分検定比              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX              --- XXXXX.XX XXXXX.XX
   ---- せん断の検討 ----
    y軸  QL,QL+QH  (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rMzAL,rMzAS(kNm)XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         sMzAL,sMzAS(kNm)XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
  [Root1,2] rMY/L0+(rQ0)      --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
  [Root1,2](rMY/MY)(Q0+2QH)   --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
         rQL,rQS   (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rα             XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
         rQA1      (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rQA2      (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rQA       (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         sQD       (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         sQA       (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
    z軸  QL,QL+QH  (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rMyAL,rMyAS(kNm)XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         sMyAL,sMyAS(kNm)XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
  [Root1,2] rMY/L0+(rQ0)      --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
  [Root1,2](rMY/MY)(Q0+2QH)   --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X              --- XXXXXX.X XXXXXX.X
         rQL,rQS   (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rα             XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
         rQA1      (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rQA2      (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         rQA       (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         sQD       (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
         sQA       (kN)  XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
     検定比 RC           XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
     検定比 S            XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX         XXXXX.XX XXXXX.XX XXXXX.XX
*/
    return QStringList{};
}

QStringList Prcard::getBraceCheck(){
    /*
XXX ZZ- ZZ  ブレ－スの断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
        Y-鉄骨           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
          幅厚比         B/F=XX.X  D/W=XX.X
        Z-鉄骨           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
          幅厚比         B/F=XX.X  D/W=XX.X
        圧縮検討用       ly=XXXXX. iy=XX.XX
                         lz=XXXXX. iz=XX.XX

                             PERM     TEMP     TEMP
          ft    (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX
          fc    (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX
          NL,  NS  (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XT
          NAL, NAS (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XT
          検定比         XXXXX.XX XXXXX.XX XXXXX.XX
          変動分検定比        --- XXXXX.XX XXXXX.XX
    */
    return QStringList{};
}

QStringList Prcard::getWallCheck(){
    /*
XXX ZZ- ZZ  耐力壁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -S-    L=XX.XXX(M)  L0=XX.XXX(M)
   T X L           200 X 5500          (XXXXXXXX)
   γ              γ  =X.XXX
                 +----------- WALL -----------+
                      PERM     TEMP     TEMP
 QL, QH  (kN)     XXXXXX.X XXXXXX.X XXXXXX.X
 QL, QS  (kN)     XXXXXX.X XXXXXX.X XXXXXX.X
 QAL,QAS (kN)     XXXXXX.X XXXXXX.X XXXXXX.X
 τL,τS(N/mm2)   XXXXXX.X XXXXXX.X XXXXXX.X
 QD/QA            XXXXXX.X XXXXXX.X XXXXXX.X

XXX ZZ- ZZ  耐力壁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -RC-   L=XX.XXX(M)  LI=XX.XXX(M)
   B X D, T X L   600 X 600(Y)         (XXXXXXXX)    200 X 5500           (XXXXXXXX)    600 X 600(Y)         (XXXXXXXX)
   鉄筋           2-D13-@100           (pw=X.XX%)    2-D10D10-@200        (ps=X.XX%)    2-D13-@100           (pw=X.XX%)
   γ,柱有効係数  左柱係数 =X.XXX  γ=X.XXX H=XX.XXX(M) L0=XX.XXX(M) H0=XX.XXX(M) L0*H0=XX.XXXX(M^2)   右柱係数 =X.XXX
                 +----------- 左柱 ---------+     +------------ 壁 ------------+     +----------- 右柱 -----------+
                      PERM     TEMP     TEMP             PERM     TEMP     TEMP             PERM     TEMP     TEMP
   QL, QH  (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QL, QS  (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QA1     (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QW, QC  (kN)   XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
   QA2                 ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QAL,QAS (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   τL,τS (N/mm2)     ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QL/QAL ,QS/QAS      ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---

XXX ZZ- ZZ  耐力壁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -SRC-  L=XX.XXX(M)  LI=XX.XXX(M)
   B X D, T X L   600 X 600(Y)         (XXXXXXXX)    200 X 5500           (XXXXXXXX)    600 X 600(Y)         (XXXXXXXX)
   鉄筋           2-D13-@100           (pw=X.XX%)    2-D10D10-@200        (ps=X.XX%)    2-D13-@100           (pw=X.XX%)
   鉄骨(Y)        WH-1000X 201X 12X 20 (XXXXXXXX)                                       WH-1000X 201X 12X 20 (XXXXXXXX)
   鉄骨(Z)        WH-1000X 201X 12X 20 (XXXXXXXX)                                       WH-1000X 201X 12X 20 (XXXXXXXX)
   γ,柱有効係数  左柱係数 =X.XXX  γ=X.XXX H=XX.XXX(M) L0=XX.XXX(M) H0=XX.XXX(M) L0*H0=XX.XXXX(M^2)   右柱係数 =X.XXX
                 +----------- 左柱 ---------+     +------------ 壁 ------------+     +----------- 右柱 -----------+
                      PERM     TEMP     TEMP             PERM     TEMP     TEMP             PERM     TEMP     TEMP
   QL, QH  (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QL, QS  (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QA1                 ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QW, QC  (kN)   XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X
   QA2                 ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QAL,QAS (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   τL,τS (N/mm2)     ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---
   QL/QAL ,QS/QAS      ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---


    */
    return QStringList{};
}

QStringList Prcard::getStoryPlastInfo(){
    /*
XXX ZZ- ZZ  弾塑性解析結果の概要 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX

** ANALYSIS MODEL : XXXXXXXXXX   TOTAL STEP = XXXX     ENDING STATE : XXXXXXXXXXXXXXXXXXXXXXXX

** INITIAL STRESS XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
           XXXXXXXXXX  COEF.=XXXX.XX

** FRAME BENDING YIELDING, BRACE AXIAL YIELDING STEP ( ANGLE = XXXX.XX )

FLOOR  +---------FR.BEND.PLAST(c)----------+  +---------FR.BEND.YIELD(m)----------+  +---------BR.AXAL.YIELD(a)----------+
       STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE
              (KN)           (MM)                    (KN)           (MM)                    (KN)           (MM)

XXXXXX XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX

** FRAME SHEAR YIELDING STEP                         ( ANGLE = XXXX.XX )

FLOOR  +---------FR.SHEAR.PLAST(h)---------+  +---------FR.SHEAR.YELD(s)----------+  +---------PANEL.YELD(p)-------------+
       STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE
              (KN)           (MM)                   (KN)            (MM)                   (KN)            (MM)

XXXXXX XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX

** WALL YIELDING STEP                                ( ANGLE = XXXX.XX )

FLOOR  +---------WA.SHEAR.CRACK(H)---------+  +---------WA.SHEAR.YIELD(S)---------+  +----------WA.BEND.YIELD(M)---------+
       STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE
              (KN)           (MM)                   (KN)            (MM)                   (KN)            (MM)

XXXXXX XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX

** QU STEP, LAST STEP , INITIAL STIFFNESS            ( ANGLE = XXXX.XX )

FLOOR  +---------------QU STEP-------------+  +--------------LAST STEP------------+  +-------------INITIAL STIFFNESS---------------+
       STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.   ANGLE     STIFF.
              (KN)           (MM)                    (KN)           (MM)                    (KN)           (MM)             (KN/CM)

XXXXXX XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX XXXXXXXXX
    */
    return QStringList{};
}

QStringList Prcard::getQrCurve(){
    /*
XXX ZZ- ZZ  弾塑性解析経過 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ XXXXXX FL  ( ANGLE = XXXX.XX   HEIGHT = XXXXX.X (MM) )
FL   STEP   COEF    STATUS  SHEAR  X-DISP. Y-DISP.  THETA-Z F-DISP.   H/D    FRAME           BRACE           WALL               DK    DK/DK1
                  cmhsaHMS   (KN)   (MM)    (MM)     (MRAD)  (MM)            (KN)    (%)     (KN)    (%)     (KN)    (%)     (KN/CM)    (%)
XXXX XXXX XX.XXXX          XXXXXXX XXXX.XX XXXX.XX XXX.XXXX XXXX.XX XXXXXX XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXXX.X(XXXX.X)

    */
    return QStringList{};
}

QStringList Prcard::getFrQdCurve(){
    /*
XXX ZZ- ZZ  架構の弾塑性解析経過 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ XXXXXX FRAME   XXXXXX FL  ( FORCE ANGLE = XXXX.XX   FRAME ANGLE = XXXX.XX   HEIGHT = XXXXX.X (MM) )
FR   FL   STEP   COEF    STATUS  SHEAR   DISP.    H/D    FRAME           BRACE           WALL            TOTAL              DK    DK/DK1
                       cmhsaHMS   (KN)   (MM)            (KN)    (%)     (KN)    (%)     (KN)    (%)     (KN)    (%)     (KN/CM)    (%)
XXXX XXXX XXXX XX.XXXX          XXXXXXX XXXX.XX XXXXXX XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXXX.X(XXXX.X)
    */
    return QStringList{};
}

QStringList Prcard::getFrDistPlast(){
    /*
XXX ZZ- ZZ  弾塑性解析の架構の層せん断力分担 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ XXXXXX FL   STEP=XXXXX   COEF.=XXX.XXXX   ( FORCE ANGLE = XXXX.XX )
+-------------------+--------+--------+---------+------------------+------------------+------------------+------------------+
| STEP (   COEF.  ) | FLOOR  | FRAME  |  ANGLE  | FRAME   (KN) (%) | BRACE   (KN) (%) | WALL    (KN) (%) | TOTAL   (KN) (%) |
+-------------------+--------+--------+---------+------------------+------------------+------------------+------------------+
|XXXXX ( XXX.XXXX ) | XXXXXX | XXXXXX | XXXX.XX |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |XXXXXXXX (XXXX.X) |
+-------------------+--------+--------+---------+------------------+------------------+------------------+------------------+
                                           SUM   XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)
                                           SHEAR XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)  XXXXXXXX (XXXX.X)
    */
    return QStringList{};
}

QStringList Prcard::getQuNeed(){
    /*
XXX ZZ- ZZ  必要保有水平耐力と保有水平耐力の比較表 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■  XXXXXXXXXX   ( ANGLE = XXXX.XX )   DS : XXXXXXXXXX  QU : XXXXXXXXXX
+--------+-----+----------------------+----------------------+------+--------------------------+-----------+-----------+--------+------+
|        |解析 |   柱・梁群のランク   | 耐震要素群のランク   |耐震要|                          | 必要保有  |   保有    |        |      |
|  階名  |構造 |  FA  FB  FC  FD RANK |  WA  WB  WC  WD RANK |素分担|   DS   FS   FE    Ｑud   | 水平耐力  | 水平耐力  | Qu/Qun | 判定 |
|        |形式 |   %   %   %   %      |   %   %   %   %      |率 βu|                   (kN)   | Ｑun(kN)  |  Ｑu(kN)  |        |      |
+--------+-----+----------------------+----------------------+------+--------------------------+-----------+-----------+--------+------+
| XXXXXX |     |                      |                      |      |                          |           |           |        |      |
|        | XXX | XXX XXX XXX XXX  X   | XXX XXX XXX XXX  X   | X.XX | X.XX X.XX X.XX XXXXXXX.X | XXXXXXX.X | XXXXXXX.X | XXX.XX |  XX  |
+--------+-----+----------------------+----------------------+------+--------------------------+-----------+-----------+--------+------+
    */
    return QStringList{};
}

QStringList Prcard::getUltimateState(){
    /*
XXX ZZ- ZZ  想定終局せん断力 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
 ( ANGLE = XXXX.XX )
+--------+-----+-----------+-----------------------------------------+-----------------------------------------+
| FLOOR  | STR |           |            LAST STEP STATE              |           ASSUMED BROKEN STATE          |
|        |     |     QUN   |   FRAME    BRACE      WALL     TOTAL    |    FRAME  ( BRACE ) (  WALL ) ( TOTAL ) |
|        |     |    (KN)   |    (KN)     (KN)      (KN)      (KN)    |     (KN)     (KN)      (KN)      (KN)   |
+--------+-----+-----------+-----------------------------------------+-----------------------------------------+
| XXXXXX |     |           |                                         |                                         |
|        | XXX | XXXXXXX.X | XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X | XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X |
+--------+-----+-----------+-----------------------------------------+-----------------------------------------+
    */
    return QStringList{};
}

QStringList Prcard::getFrMemUlt(){
    /*
XXX ZZ- ZZ  架構毎の部材終局耐力 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ XXXXXX FRAME  GIRDER **
■ XXXXXX FRAME  COLUMN **
  SEQ.  LIST                                                                                    +---------------- REFERENCE ----------------+
        FL     FRAME  GR1    GR2     AX.V+U  BY-ST.I BY-ST.J SY-STEP   MZF.I    MZF.J      QUY    M0F.I    M0J.I     QU0    MBJ.I      QB0
        J1     J2            STATE   AX.ULT   COEF.   COEF.   COEF.    MYF.I    MYF.J      QUZ    M0F.J    M0J.J     QM0    MBJ.J     STATE
                           (DUF.AX)((V+U)/UL)( DUF.I)( DUF.J)( DUF.S)(VM/VMU) (VM/VMU) ( BETQ )                  (QU0/QM0)         (QU0/QB0)
                                      (KN)                            (KN*M)   (KN*M)    (KN*M)  (KN*M)   (KN*M)           (KN*M)
(XXXXX) XXXXXXXXXXXXXXXX
        XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX    XXXX    XXXX    XXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX
        XXXXXX-XXXXXX       +-----+ XXXXXXXX XX.XXXX XX.XXXX XX.XXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX +------+
                            (XXX.XX)(XXX.XX) (XXX.XX)(XXX.XX)(XXX.XX)(XXX.XX) (XXX.XX) (XXX.XX)                   (XXX.XX)    MYQZ  (XXX.XX)
■ XXXXXX FRAME  BRACE  **
  SEQ.  LIST
        FL     FRAME  GR1    GR2     AX.V+U  AY-STEP
        J1     J2                    AX.ULT   COEF.
                                   ((V+U)/UL)(DUF.AX)
                                      (KN)
(XXXXX) XXXXXXXXXXXXXXXX
        XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX    XXXX
        XXXXXX-XXXXXX               XXXXXXXX XX.XXXX
                                    (XXX.XX) (XXX.XX)
■ XXXXXX FRAME  WALL   **
  SEQ.  LIST
        FL     FRAME  GR1    GR2     QC-STEP QS-STEP BY-ST.B BY-ST.T   QU
        J1     J2     J3     J4       COEF.   COEF.   COEF.   COEF.    QULT
                                                                    (QU/QULT)
                                                                       (KN)
(XXXXX) XXXXXXXX
        XXXXXX XXXXXX XXXXXX XXXXXX     XXXX    XXXX    XXXX    XXXX XXXXXXXX
        XXXXXX-XXXXXX-XXXXXX-XXXXXX  XX.XXXX XX.XXXX XX.XXXX XX.XXXX XXXXXXXX
                                                                     (XXX.XX)
■ XXXXXX FRAME  COLUMN-GIRDER ULTIMATE BENDING MOMENT RATIO ( REFERENCE ) **
  SEQ.     NO. FL     GR1    GR2  NC NG  COL.BE   GIR.BE    C/G    COL.CMP  GIR.CMP    C/G
                                         (KN*M)   (KN*M)            (KN*M)   (KN*M)
    */
    return QStringList{};
}

QStringList Prcard::getFrRank(){
    /*
XXX ZZ- ZZ  架構毎の部材ランクの概要 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ XXXXXX FRAME  GIRDER **
  SEQ.  LIST                           ULT.SH                                                            +- RC -+ +---- S ----+ +--- SRC ---+
        FL     FRAME  GR1    GR2   IMD BEY.SH(UL/BEY) I STR RNK B   ALF  ALFB                             TAU/FC    B/F    D/W   N/N0 SM0/M0
        J1     J2     --- STATE ---    BRK.SH(UL/BRK) J STR RNK B   ALF  ALFB                             TAU/FC    B/F    D/W   N/N0 SM0/M0
(XXXXX) XXXXXXXXXXXXXXXX               XXXXXX
        XXXXXX XXXXXX XXXXXX XXXXXX XX XXXXXX(XXX.XX) I XXX (X) - XX.XX XX.XX                             XX.XXX XXX.XX XXX.XX XX.XXX XX.XXX
        XXXXXX-XXXXXX -- +-----+ --    XXXXXX(XXX.XX) J XXX (X) - XX.XX XX.XX                             XX.XXX XX.XXX XXX.XX XXX.XX XX.XXX
■ XXXXXX FRAME  COLUMN **
  SEQ.  LIST                           ULT.SH                                         +----------- RC ----------+ +---- S ----+ +- SRC(CFT)-+
        FL     FRAME  GR1    GR2   IMD BEY.SH(UL/BEY) I STR RNK B   ALF  ALFB  Y STRES H0/D SGM0/FC PT(%) TAU/FC    B/F    D/W  N/N0 SM/M(RU)
        J1     J2     --- STATE ---    BRK.SH(UL/BRK)                          Z STRES H0/D         PT(%)                            SM/M(RU)
                                   QRK                J STR RNK B   ALF  ALFB  Y STRES H0/D SGM0/FC PT(%) TAU/FC    B/F    D/W  N/N0 SM/M(RU)
                                                                               Z STRES H0/D         PT(%)                            SM/M(RU)
(XXXXX) XXXXXXXXXXXXXXXX               XXXXXX
        XXXXXX XXXXXX XXXXXX XXXXXX XX XXXXXX(XXX.XX) I XXX (X) - XX.XX XX.XX  Y  -- XX.XXX XX.XXX XX.XXX XX.XXX XXX.XX XXX.XX XX.XXX XX.XXX
        XXXXXX-XXXXXX -- +-----+ --    XXXXXX(XXX.XX)                          Z  -- XX.XXX        XX.XXX                             XX.XXX
                                  ( X )               J XXX (X) - XX.XX XX.XX  Y  -- XX.XXX XX.XXX XX.XXX XX.XXX XXX.XX XXX.XX XX.XXX XX.XXX
                                                                               Z  -- XX.XXX        XX.XXX                             XX.XXX
■ XXXXXX FRAME  BRACE  **
  SEQ.  J1     J2     FL     FRAME  GR1    GR2    LIST              RANK   RMD

(XXXXX) XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXXXXXXXXXX (   ) XXXX.X
■ XXXXXX FRAME  WALL   **
  SEQ.      J1     J2     J3     J4 FL     FRAME  GR1    GR2    LIST      RANK     QL        Q       QU  QU/(QL+Q)
                                                                                  (KN)     (KN)     (KN)
(XXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX ( X ) XXXXXXXX XXXXXXXX XXXXXXXX XX.XXX
■ XXXXXX FRAME  RC柱梁接合部パネルのせん断余裕度の検討 **
  SEQ.     NO. FL     GR1    GR2           形状  両側    σB     bj      D      1.1Qb      Qu      判定
                                                直交梁 (N/mm2)  (mm)   (mm)      (kN)     (kN)   (1.1Qb<=Qu?)
(XXXXX) XXXXXX                               X    XX   XXXX.X  XXXXX. XXXXX. XXXXXXXX. XXXXXXXX.    XX
    */
    return QStringList{};
}

QStringList Prcard::getFrMemR(){
    /*
XXX ZZ- ZZ  架構毎の部材角 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
■ XXXXXX FRAME  GIRDER **  UNIT= X 1000
■ XXXXXX FRAME  COLUMN **  UNIT= X 1000
■ XXXXXX FRAME  BRACE  **  UNIT= X 1000
  SEQ.  LIST
        FL     FRAME  GR1    GR2      DX/L0        RY.I      RZ.I      R.I         RY.J      RZ.J      R.J        DS/L0
        J1     J2            L0(M)  ( YIELD )   ( YIELD ) ( YIELD ) ( YIELD )   ( YIELD ) ( YIELD ) ( YIELD )   ( YIELD )
(XXXXX) XXXXXXXXXXXXXXXX
        XXXXXX XXXXXX XXXXXX XXXXXX  XXX.XXX     XXXXXXX   XXXXXXX   XXXXXXX     XXXXXXX   XXXXXXX   XXXXXXX     XXXXXXX
        XXXXXX-XXXXXX        XX.XXX (XXX.XXX)   (XXXXXXX) (XXXXXXX) (XXXXXXX)   (XXXXXXX) (XXXXXXX) (XXXXXXX)   (XXXXXXX)
■ XXXXXX FRAME  WALL   **
  SEQ.      J1     J2     J3     J4 FL     FRAME  GR1    GR2    LIST      RANK      Q       QU    QU/Q
(XXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX ( X ) XXXXXXXX XXXXXXXX XX.XXX
    */
    return QStringList{};
}

QStringList Prcard::getGirderRank(){
    /*
XXX ZZ- ZZ  大梁のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                           STEEL           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
           B/F=XX.X D/W=XX.X          MRANK=(-)  SRANK=(-)            W/T            B/F=XX.X  D/W=XX.X         MRANK=(-)  SRANK=(-)
          +------------------- LEFT ---------------------+                          +------------------- RIGHT --------------------+
           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK
MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)
ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)
                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~                                                  ~~~~~~~

XXX ZZ- ZZ  大梁のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
           XXXXX  X  XXXXX      (XXXXXXXX)                          B X D            XXXXX  X  XXXXX      (XXXXXXXX)
           2-D25( 75) 2-D25( 83) 2-D25( 91)                       UPPER-BAR          2-D25( 75) 2-D25( 83) 2-D25( 91)
           2-D25( 75) 2-D25( 83) 2-D25( 91)                       LOWER-BAR          2-D25( 75) 2-D25( 83) 2-D25( 91)
           TAU/FC=X.XXX               MRANK=(-)  SRANK=(-)         TAU/FC            TAU/FC=X.XXX               MRANK=(-)  SRANK=(-)
          +------------------- LEFT ---------------------+                          +------------------- RIGHT --------------------+
           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK
MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)
ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)
                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~                                                  ~~~~~~~

XXX ZZ- ZZ  大梁のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)
           XXXXX  X  XXXXX      (XXXXXXXX)                          B X D            XXXXX  X  XXXXX      (XXXXXXXX)
           2-D25( 75) 2-D25( 83) 2-D25( 91)                       UPPER-BAR          2-D25( 75) 2-D25( 83) 2-D25( 91)
           2-D25( 75) 2-D25( 83) 2-D25( 91)                       LOWER-BAR          2-D25( 75) 2-D25( 83) 2-D25( 91)
           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                          STEEL            WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
           N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)        N/N0,SM/M          N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)
          +------------------- LEFT ---------------------+                          +------------------- RIGHT --------------------+
           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK
MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)
ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)
                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~                                                  ~~~~~~~

*/
    return QStringList{};
}

QStringList Prcard::getColumnRnak(){
    /*
XXX ZZ- ZZ  柱のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                          Y-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
           B/F=XX.X D/W=XX.X                                          W/T            B/F=XX.X  D/W=XX.X
           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                          Z-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)
           B/F=XX.X D/W=XX.X          MRANK=(-)  SRANK=(-)            W/T            B/F=XX.X  D/W=XX.X         MRANK=(-)  SRANK=(-)
          +------------------ BOTTOM --------------------+                          +-------------------- TOP ---------------------+
           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK
MZ,QY(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)
ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)
                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~ ~~~~~~~                                          ~~~~~~~

XXX ZZ- ZZ  柱のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
           XXXXX  X  XXXXX      (XXXXXXXX)                          B X D            XXXXX  X  XXXXX      (XXXXXXXX)
           SGM/FC=X.XXX  TAU/FC=X.XXX                           SGM/FC,TAU/FC        SGM/FC=X.XXX  TAU/FC=X.XXX
           4-D25( 75):Y1   2-D25( 91):Y1                          Y- BAR             4-D25( 75):Y1   2-D25( 91):Y1
           H0/D=XX.XX  PT=X.XX(%)     MRANK=(-)  SRANK=(-)        Y- H0/D,PT         H0/D=XX.XX  PT=X.XX(%)     MRANK=(-)  SRANK=(-)
           4-D25( 75):Y1   2-D25( 91):Y1                          Z- BAR             4-D25( 75):Y1   2-D25( 91):Y1
           H0/D=XX.XX  PT=X.XX(%)     MRANK=(-)  SRANK=(-)        Z- H0/D,PT         H0/D=XX.XX  PT=X.XX(%)     MRANK=(-)  SRANK=(-)
          +------------------ BOTTOM --------------------+                          +-------------------- TOP ---------------------+
           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK
MZ,QY(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)
ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)
                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~ ~~~~~~~                                          ~~~~~~~

XXX ZZ- ZZ  柱のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
           XXXXX  X  XXXXX      (XXXXXXXX)                          B X D            XXXXX  X  XXXXX      (XXXXXXXX)
           4-D25( 75):Y1   2-D25( 91):Y1                          Y- BAR             4-D25( 75):Y1   2-D25( 91):Y1
           WH-1000X 201X 12X 20 (XXXXXXXX)                        Y- STEEL           WH-1000X 201X 12X 20 (XXXXXXXX)
           N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)        Y- N/N0,SM/M       N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)
           4-D25( 75):Y1   2-D25( 91):Y1                          Z- BAR             4-D25( 75):Y1   2-D25( 91):Y1
           WH-1000X 201X 12X 20 (XXXXXXXX)                        Z- STEEL           WH-1000X 201X 12X 20 (XXXXXXXX)
           N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)        Z- N/N0,SM/M       N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)
          +------------------ BOTTOM --------------------+                          +-------------------- TOP ---------------------+
           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK
MZ,QY(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)
ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)
                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~ ~~~~~~~                                          ~~~~~~~

XXX ZZ- ZZ  柱のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SC-  L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)
           P -1000X 20          (XXXXXXXX)                          STEEL            P -1000X201          (XXXXXXXX)
           D/T=XX.X   AS=XXXX.X  SGY=XX.XXX                         D/T,AS           D/T=XX.X   AS=XXXX.X  SGY=XX.XXX
           FC =XXXX.X AC=XXXX.X (XXXXXXXX)                         CONCRETE          FC=XXXX.XX AC=XXXX.X (XXXXXXXX)
           CFC=XX.XXX SFC=XX.XXX SFT=XX.XXX                        CONF.EFFECT       CFC=XX.XXX SFC=XX.XXX SFT=XX.XXX
           N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)        Y- N/N0,SM/M       N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)
          +------------------ BOTTOM --------------------+                          +-------------------- TOP ---------------------+
           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK
MZ,QY(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X
STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)
ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)
                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~ ~~~~~~~                                          ~~~~~~~
    */
    return QStringList{};
}

QStringList Prcard::getCwArea(){
    /*
XXX ZZ- ZZ  壁と柱の水平断面積 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/06/02 15:35:10 ] XXXXXXXXXX
 検討方向 X  [XXXX.X ]        計算ル－ト [ XXX ]     Z = X.XXX  T1= X.XXX (S)
                                                    ＲＣ造  ： A25 = (2.5Aw + 0.7 Ac+ 0.7Aw')/βZWAi , A18 = (1.8Aw + 1.8 Ac)/βZWAi
                                                    ＳＲＣ造： A25 = (2.5Aw + 1.0 Ac+ 0.7Aw')/βZWAi , A18 = (2.0Aw + 2.0 Ac)/βZWAi
+--------+------+----------+------+----------+----------+----------+----------+----------+-------+---------+---------+----------------+
|  階名  | 構造 |   CONC.  |  β  |    Aw    |    Ac    |    Aw'   | floor Wi | total W  |  Ai   |   A25   |   A18   |     判 定      |
|        | 形式 |   材料   |      |  (m**2)  |  (m**2)  |  (m**2)  |   (kN)   |   (kN)   |       |         |         |                |
+--------+------+----------+------+----------+----------+----------+----------+----------+-------+---------+---------+----------------+
| XXXXXX |      |          |      |          |          |          |XXXXXXXX. |          |       |         |         |                |
|        |  XXX | XXXXXXXX | X.XX | XXXXX.XX | XXXXX.XX | XXXXX.XX |          |XXXXXXXX. | X.XXX | XXX.XXX | XXX.XXX | XXXXXXXXXXXXXX |
+--------+------+----------+------+----------+----------+----------+----------+----------+-------+---------+---------+----------------+
    */
    return QStringList{};
}

QStringList Prcard::getMessage(){
    /*
XXX ZZ- ZZ  終了メッセ－ジ XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
+----------------------+----------------+----------------+----------------+----------------+----------------+
|                      |                |                |                |                |                |
|    サブシステム名    |   ＥＲＲＯＲ   | ＷＡＲＮＩＮＧ | ＮＯ　ＧＯＯＤ | ＣＡＵＴＩＯＮ |    実行状況    |
|                      |                |                |                |                |                |
|======================|================|================|================|================|================|
|                      |                |                |                |                |                |
|     建物形状入力     |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|       重量拾い       |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|    形状デ－タ変換    |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|  共通解析モデル作成  |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|       荷重作成       |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|    解析モデル作成    |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|       応力解析       |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
| 偏心率・剛性率等算定 |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|   Ｎ－Ｍ曲面の作成   |    XXXXX       |    XXXXX       |        -       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|       断面検討       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|  保有水平耐力の検討  |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
|    計算内容の確認    |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXXXXX    |
|                      |                |                |                |                |                |
+----------------------+----------------+----------------+----------------+----------------+----------------+
|                      |                |                |                |                |                |
|      合      計      |    XXXXX       |    XXXXX       |    XXXXX       |    XXXXX       |                |
|                      |                |                |                |                |                |
+----------------------+----------------+----------------+----------------+----------------+----------------+
                                        構造計算は問題なく終了しました
                                       インプットデ－タを見直して下さい
    */
    return QStringList{};
}
