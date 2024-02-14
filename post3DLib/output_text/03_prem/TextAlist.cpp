#include "abstracttext.h"
#include "fixed_data.h"
#include "define_section_struct.h"
#include "unified_inputdata.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextAlist::write(const FixedData& fixedData) const{
    //GIRDER(PR043)
    //COLUMN(PR044)
    //BRACE(PR045)
    //WALL(PR046)
    //OPEN(PR047)


    QString second;
    second.append(u8"    XXX ZZ- ZZ  大梁リスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"    +------------------+-----------------------------------+-----------------------------------+-----------------------------------+").append("\n");
    second.append(u8"    |                  |  LEFT END                         |  CENTER                           |  RIGHT END                        |").append("\n");
    Q_FOREACH(auto sec, fixedData.getInputData()->getSectionValueList(DATATYPE::TPGIRDER)){
        second.append(u8"    | G1-1-3           |  SRC                              |  SRC                              |  SRC                              |").append("\n");
        second.append(u8"    | (NO.XXX)         |                                   |                                   |                                   |").append("\n");
        second.append(u8"    | BXD              |  1000X1000(1000X1000)  (FC21    ) |  1000X1000             (FC21    ) |  1000X1000             (FC21    ) |").append("\n");
        second.append(u8"    | STEEL            |  WH-1000X 200X 12X 25  (SN490   ) |  WH-1000X 200X 12X 25  (SN490   ) |  WH-1000X 200X 12X 25  (SN490   ) |").append("\n");
        second.append(u8"    | UPPER-BAR        |   2-D25( 75) 2-D25( 83) 2-D25( 91)|   2-D25( 75) 2-D25( 83) 2-D25( 91)|   2-D25( 75) 2-D25( 83) 2-D25( 91)|").append("\n");
        second.append(u8"    | LOWER-BAR        |   2-D25( 75) 2-D25( 83) 2-D25( 91)|   2-D25( 75) 2-D25( 83) 2-D25( 91)|   2-D25( 75) 2-D25( 83) 2-D25( 91)|").append("\n");
        second.append(u8"    | STIRRUP(PW)      |   2-D13@200( .28)                 |   2-D13@200( .28)                 |   2-D13@200( .28)                 |").append("\n");
        second.append(u8"    | D+(PT+),D-(PT-)  |         500( .32)       500( .32) |         500( .32)       500( .32) |         500( .32)       500( .32) |").append("\n");
        second.append(u8"    | I*(CM)  ,AF(CM2) |  XXXXXXXXXXXX.XX XXXXXXXXXXXX.XX  |  XXXXXXXXXXXX.XX XXXXXXXXXXXX.XX  |  XXXXXXXXXXXX.XX XXXXXXXXXXXX.XX  |").append("\n");
        second.append(u8"    | EI(KNM2),I(CM4E5)|  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |").append("\n");
        second.append(u8"    | GAS(KN) ,AS(CM2) |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |  XXXXXXXXXXX.XXX XXXXXXXXXXX.XXX  |").append("\n");
        second.append(u8"    +------------------+-----------------------------------+-----------------------------------+-----------------------------------+").append("\n");
    }
    second.append(u8"    XXX ZZ- ZZ  柱リスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"    +------------------+-------------------------------------------------------+-------------------------------------------------------+").append("\n");
    second.append(u8"    |                  |  BOTTOM                                               |  TOP                                                  |").append("\n");
    second.append(u8"    +------------------+-------------------------------------------------------+-------------------------------------------------------+").append("\n");
    Q_FOREACH(auto sec, fixedData.getInputData()->getSectionValueList(DATATYPE::TPCOLUMN)){
        second.append(u8"    | C1-1-3           |  SRC                                                  |                                                       |").append("\n");
        second.append(u8"    | (NO.XXX)         |                                                       |                                                       |").append("\n");
        second.append(u8"    | DX X DY          |  1000X1000                                 (FC21    ) |  1000X1000                                 (FC21    ) |").append("\n");
        second.append(u8"    |                  |  +----Y DIRECTION----+ +----Z DIRECTION----+          |  +----Y DIRECTION----+ +----Z DIRECTION----+          |").append("\n");
        second.append(u8"    | STEEL            |  WH-1000X 200X 12X 25  WH-1000X 200X 12X 25(SN490   ) |  P -1000X  25 (CFT)                        (SN490   ) |").append("\n");
        second.append(u8"    | BAR(D,PT)        |  4-D25( 75)(XXXX, .83) 4-D25( 75)(XXXX, .83)          |  4-D25( 75)(  83, .25) 4-D25( 75)(  83, .25)          |").append("\n");
        second.append(u8"    | INNER-BAR        |  4-D25( 83)            4-D25( 83)                     |  4-D25( 83)            4-D25( 83)                     |").append("\n");
        second.append(u8"    | HOOP(PW) ,AT(CM2)|  2-D13@100( .28)       2-D13@100( .28)  XXXX.XX(X.XX) |  2-D13@100( .28)       2-D13@100( .28)  XXXX.XX(X.XX) |").append("\n");
        second.append(u8"    | IE,I*(CM),AF(CM2)|  XXX.XX XXX.XX XXX.XX  XXX.XX XXX.XX XXX.XX           |  XXXXXX XXXXXX XXXXXX  XXXXXX XXXXXX XXXXXX           |").append("\n");
        second.append(u8"    | EI(KNM2),I(CM4E5)| XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |").append("\n");
        second.append(u8"    | GAS(KN) ,AS(CM2) | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |").append("\n");
        second.append(u8"    | EAX,AX  ,GJ,J    | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |").append("\n");
        second.append(u8"    +------------------+-------------------------------------------------------+-------------------------------------------------------+").append("\n");
    }
    second.append(u8"    XXX ZZ- ZZ  ブレ－スリスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"    +------------------+-------------------------------------------------------+").append("\n");
    Q_FOREACH(auto sec, fixedData.getInputData()->getSectionValueList(DATATYPE::TPBRACE)){
        second.append(u8"    | C1-1-3           |  SRC                                                  |").append("\n");
        second.append(u8"    | (NO.XXX)         |                                                       |").append("\n");
        second.append(u8"    | DX X DY          |  1000X1000                                 (FC21    ) |").append("\n");
        second.append(u8"    |                  |  +----Y DIRECTION----+ +----Z DIRECTION----+          |").append("\n");
        second.append(u8"    | STEEL            |  WH-1000X 200X 12X 25  WH-1000X 200X 12X 25(SN490   ) |").append("\n");
        second.append(u8"    | BAR(D,PT)        |  4-D25( 75)(XXXX, .83) 4-D25( 75)(XXXX, .83)          |").append("\n");
        second.append(u8"    | INNER-BAR        |  4-D25( 83)            4-D25( 83)                     |").append("\n");
        second.append(u8"    | HOOP(PW) ,AT(CM2)|  2-D13@100( .28)       2-D13@100( .28)  XXXX.XX(X.XX) |").append("\n");
        second.append(u8"    | IE,I*(CM),AF(CM2)|  XXX.XX XXX.XX XXX.XX  XXX.XX XXX.XX XXX.XX           |").append("\n");
        second.append(u8"    | EI(KNM2),I(CM4E5)| XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |").append("\n");
        second.append(u8"    | GAS(KN) ,AS(CM2) | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |").append("\n");
        second.append(u8"    | EAX,AX  ,GJ,J    | XXXXXXXX.X XXXXXX.XXX XXXXXXXX.X XXXXXX.XXX           |").append("\n");
        second.append(u8"    +------------------+-------------------------------------------------------+").append("\n");
    }
    second.append(u8"    XXX ZZ- ZZ  壁リスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"    +----------+--------+----------+-----+--------------------------+").append("\n");
    second.append(u8"    | NAME     | THICK  | MATERIAL | S/W |        WALL-BAR       PW |").append("\n");
    second.append(u8"    |          |  (MM)  |          |     |                      (%) |").append("\n");
    second.append(u8"    +----------+--------+----------+-----+--------------------------+").append("\n");
    Q_FOREACH(auto sec, fixedData.getInputData()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"    | XXXXXXXX | XXXX.X | XX       |  W  | V-BAR D10D13 @200 ( .36) |").append("\n");
        second.append(u8"    |          |        | XXXXXXXX |     | H-BAR D10D13 @200 ( .36) |").append("\n");
        second.append(u8"    +----------+--------+----------+-----+--------------------------+").append("\n");
    }
    second.append(u8"    XXX ZZ- ZZ  開口リスト(解析情報) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"             NAME     L/R B/T      X       Y      L0      H0").append("\n");
    second.append(u8"                                 (MM)    (MM)    (MM)    (MM)").append("\n");
    Q_FOREACH(auto sec, fixedData.getInputData()->getSectionValueList(DATATYPE::TPWALLOPEN)){
        second.append(u8"    (XXXXXX) XXXXXXXX  X   X  XXXXX.X XXXXX.X XXXXX.X XXXXX.X").append("\n");
        second.append(u8"                       X   X  XXXXX.X XXXXX.X XXXXX.X XXXXX.X").append("\n");
    }
    second.append(u8"    XXX ZZ- ZZ  S露出柱脚リスト XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"                                                 /----------- ベースプレート -----------/ /--- アンカーボルト ----/").append("\n");
    second.append(u8"            NAME     MAKER      規格                 B      D     T    hy    hz  C材種     ny  nz  径     F     lb").append("\n");
    second.append(u8"                                                   (MM)   (MM)  (MM)  (MM)  (MM)                   (N/mm2)   (mm)").append("\n");
    second.append(u8"       MNO.      I      J LIST             FL     FR     GR1    GR2    IorJ  BASE").append("\n");
    Q_FOREACH(auto sec, fixedData.getInputData()->getSectionValueList(DATATYPE::TPBASE)){
        if (sec.sectionType != STRUCTTYPE::STP_S)
            continue;
        second.append(u8"    (XXXXX) XXXXXXXX XXXXXXXXXX XXXXXXXXXXXXXXXX XXXX.X XXXX.X XXX.X XXX.X XXX.X XXXXXXXX XXX XXX XXX XXXX.X XXXX.X").append("\n");
        second.append(u8"    (XXXXX) XXXXXX XXXXXX XXXXXXXXXXXXXXXX XXXXXX XXXXXX XXXXXX XXXXXX   X   XXXXXXX").append("\n");
    }
    second.append(u8"    XXX ZZ- ZZ  SRC非埋込柱脚リスト XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"                     /-------------------------------- ベースプレート ------------------------------/ /------- アンカーボルト ---------/").append("\n");
    second.append(u8"            NAME     形状          B      D     T   hy1   hy2   hz1   hz2   sml    B0    D0  C材種    配置    ny1 nz1 ny2 nz2 径      F").append("\n");
    second.append(u8"                                 (MM)   (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)  (MM)                                     (N/mm2)").append("\n");
    second.append(u8"       MNO.      I      J LIST             FL     FR     GR1    GR2    BASE").append("\n");
    Q_FOREACH(auto sec, fixedData.getInputData()->getSectionValueList(DATATYPE::TPBASE)){
        if (sec.sectionType != STRUCTTYPE::STP_SRC)
            continue;
        second.append(u8"    (XXXXX) XXXXXXXX XXXXXXXXX XXXX.X XXXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXX.X XXXXXXXX XXXXXXX XXX XXX XXX XXX XXX XXXX.X").append("\n");
        second.append(u8"    (XXXXX) XXXXXX XXXXXX XXXXXXXXXXXXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX").append("\n");
    }

    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
