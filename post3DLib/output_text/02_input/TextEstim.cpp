#include "abstracttext.h"

#include "fixed_data.h"
#include "unified_inputdata.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextEstim::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"    ■ 固定荷重 (KN)").append("\n");
    second.append(u8"    階名      床     節点    大梁    小梁  大梁上  小梁上     柱     間柱     壁  ブレ－ス  層合計  LL面積  [/LL面積 ] 上階合計").append("\n");
    second.append(u8"                  追加荷重                追加荷重 追加荷重                                           (M2)   (KN/M2)").append("\n");
    Q_FOREACH(auto floor, fixedData.getInputData()->getFloorList()){
        second.append(floor.floorName).append("\n");
    }
    second.append(u8"    ----------------------------------------------------------------------------------------------------------------------------").append("\n");
    second.append(u8"    TOTAL  XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXXX XXXXXXX [XXXX.XX ]").append("\n");
    second.append(u8"    ( % )  (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X)  (XXX.X)").append("\n");
    second.append(u8"    ■ 固定荷重の内訳 (KN)                                     BODY:く体   FL:各リスト仕上  FLL:床仕上リスト  FIRE:耐火被覆").append("\n");
    second.append(u8"           +------------------------ 当階 ----------------------------+ +------------ 下階 ------------+").append("\n");
    second.append(u8"    階名            床     節点           大梁    小梁  大梁上  小梁上     柱     間柱     壁  ブレ－ス  層合計  LL面積  [/LL面積 ] 上階合計").append("\n");
    second.append(u8"                         追加荷重                      追加荷重 追加荷重                                           (M2)   (KN/M2)").append("\n");
    Q_FOREACH(auto floor, fixedData.getInputData()->getFloorList()){
        second.append(u8"    XXXXXX BODY  XXXXXXX          BODY  XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX").append("\n");
        second.append(u8"           FL    XXXXXXX          FL    XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX").append("\n");
        second.append(u8"           FLL   XXXXXXX          FIRE  XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX").append("\n");
        second.append(u8"           TOTAL XXXXXXX XXXXXXX  TOTAL XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXXX XXXXXXX [XXXX.XX ] XXXXXXXXX").append("\n");
        second.append(u8"           ( % ) (XXX.X) (XXX.X)        (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X)  (XXX.X)").append("\n");
        second.append(u8"    -----------------------------------------------------------------------------------------------------------------------------------------").append("\n");
    }
    second.append(u8"    TOTAL  BODY  XXXXXXX          BODY  XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX").append("\n");
    second.append(u8"           FL    XXXXXXX          FL    XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX").append("\n");
    second.append(u8"           FLL   XXXXXXX          FIRE  XXXXXXX XXXXXXX                 XXXXXXX XXXXXXX XXXXXXX XXXXXXX").append("\n");
    second.append(u8"           TOTAL XXXXXXX XXXXXXX  TOTAL XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXX XXXXXXXX XXXXXXX [XXXX.XX ]").append("\n");
    second.append(u8"           ( % ) (XXX.X) (XXX.X)        (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X) (XXX.X)  (XXX.X)").append("\n");
    second.append(u8"    ■ コンクリ－ト体積 (M3)").append("\n");
    second.append(u8"    階名        床       大梁      小梁       柱      間柱        壁    ブレ－ス   層合計    LL面積  [ /LL面積 ]  上階合計").append("\n");
    Q_FOREACH(auto floor, fixedData.getInputData()->getFloorList()){
        second.append(u8"    XXXXXX XXXXXXX.X XXXXXXX.X XXXXXXX.X").append("\n");
        second.append(u8"                                         XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X [XXXXX.XX ] XXXXXXXXXX").append("\n");
    }
    second.append(u8"    -----------------------------------------------------------------------------------------------------------------------").append("\n");
    second.append(u8"    TOTAL  XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXXXX [XXXXX.XX ]").append("\n");
    second.append(u8"    ( % )    ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )").append("\n");
    second.append(u8"    ■ 型枠面積 (M2)").append("\n");
    second.append(u8"    階名        床       大梁      小梁       柱      間柱        壁    ブレ－ス   層合計    LL面積  [ /LL面積 ]  上階合計").append("\n");
    Q_FOREACH(auto floor, fixedData.getInputData()->getFloorList()){
        second.append(u8"    XXXXXX XXXXXXX.X XXXXXXX.X XXXXXXX.X").append("\n");
        second.append(u8"                                         XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X [XXXXX.XX ] XXXXXXXXXX").append("\n");
    }
    second.append(u8"    -----------------------------------------------------------------------------------------------------------------------").append("\n");
    second.append(u8"    TOTAL  XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXXXX [XXXXX.XX ]").append("\n");
    second.append(u8"    ( % )    ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )").append("\n");
    second.append(u8"    ■ 鉄骨重量 (KN)").append("\n");
    second.append(u8"    階名                 大梁      小梁       柱      間柱        壁    ブレ－ス   層合計    LL面積  [ /LL面積 ]  上階合計").append("\n");
    Q_FOREACH(auto floor, fixedData.getInputData()->getFloorList()){
        second.append(u8"    XXXXXX           XXXXXXX.X XXXXXXX.X").append("\n");
        second.append(u8"                                         XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X [XXXXX.XX ] XXXXXXXXXX").append("\n");
    }
    second.append(u8"    -----------------------------------------------------------------------------------------------------------------------").append("\n");
    second.append(u8"    TOTAL            XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXXXX [XXXXX.XX ]").append("\n");
    second.append(u8"    ( % )              ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )").append("\n");
    second.append(u8"    ■ 鉄骨耐火被覆重量 (KN) ・ 鉄骨表面積 (M2)   ( S造,SC造 部分 )").append("\n");
    second.append(u8"                     +------ 当階 ------+ +--------------- 下階 ----------------+").append("\n");
    second.append(u8"    階名                 大梁     小梁        柱      間柱        壁    ブレ－ス   層合計    LL面積  [ /LL面積 ]  上階合計").append("\n");
    Q_FOREACH(auto floor, fixedData.getInputData()->getFloorList()){
        second.append(u8"    XXXXXX    重量   XXXXXXX.X XXXXXXX.X XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX [XXXXX.XX ] XXXXXXXXXX").append("\n");
        second.append(u8"              面積   XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X XXXXXXX.X           [XXXXX.XX ] XXXXXXXXXX").append("\n");
        second.append(u8"        (重量/面積)    ( X.XXX ) ( X.XXX ) ( X.XXX ) ( X.XXX ) ( X.XXX ) ( X.XXX ) ( X.XXX )").append("\n");
    }
    second.append(u8"    -----------------------------------------------------------------------------------------------------------------------").append("\n");
    second.append(u8"    TOTAL     重量   XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX [XXXXX.XX ]").append("\n");
    second.append(u8"             ( % )     ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )").append("\n");
    second.append(u8"              面積   XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX XXXXXXXXX [XXXXX.XX ]").append("\n");
    second.append(u8"             ( % )     ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X ) ( XXX.X )").append("\n");
    second.append(u8"    ■ 節点鉛直荷重(応力解析用) (KN)").append("\n");
    second.append(u8"    階名    +----架構用-----+ +--- 地震用-----+ +---固定荷重----+ +架構用積載荷重-+ +- 地震用積載 --+ ( 固定荷重 DL )    (その他の架構用積載荷重)").append("\n");
    second.append(u8"            DL+LL1  [ /面積]  DL+LLE  [ /面積]     DL   [ /面積]    LL1   [ /面積]    LLE   [ /面積]   床梁                LL2      LL3      LL4").append("\n");
    second.append(u8"           (全体比%)         (全体比%)         (全体比%)          (DL比%)           (DL比%)").append("\n");
    second.append(u8"                    上階合計          上階合計          上階合計          上階合計          上階合計   柱壁等 上階合計").append("\n");
    Q_FOREACH(auto floor, fixedData.getInputData()->getFloorList()){
        second.append(u8"            (XXX.X )          (XXX.X )          (XXX.X )          (XXX.X )          (XXX.X )").append("\n");
        second.append(u8"                    XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX XXXXXXXX XXXXXXXX").append("\n");
    }
    second.append(u8"    ---------------------------------------------------------------------------------------------------------------------------------------------").append("\n");
    second.append(u8"     TOTAL          XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX          XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX").append("\n");
    second.append(u8"    [ /面積]        [XXX.XX]          [XXX.XX]          [XXX.XX]          [XXX.XX]          [XXX.XX]          [XXX.XX] [XXX.XX] [XXX.XX] [XXX.XX]").append("\n");
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
