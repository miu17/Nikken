#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextColumnRank::write(const FixedData& fixedData) const{
    //PR631

    QString second;
    second.append(u8"XXX ZZ- ZZ  柱のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)").append("\n");
        second.append(u8"           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                          Y-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"           B/F=XX.X D/W=XX.X                                          W/T            B/F=XX.X  D/W=XX.X").append("\n");
        second.append(u8"           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                          Z-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"           B/F=XX.X D/W=XX.X          MRANK=(-)  SRANK=(-)            W/T            B/F=XX.X  D/W=XX.X         MRANK=(-)  SRANK=(-)").append("\n");
        second.append(u8"          +------------------ BOTTOM --------------------+                          +-------------------- TOP ---------------------+").append("\n");
        second.append(u8"           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK").append("\n");
        second.append(u8"MZ,QY(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)").append("\n");
        second.append(u8"ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)").append("\n");
        second.append(u8"                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~ ~~~~~~~                                          ~~~~~~~").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  柱のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)").append("\n");
        second.append(u8"           XXXXX  X  XXXXX      (XXXXXXXX)                          B X D            XXXXX  X  XXXXX      (XXXXXXXX)").append("\n");
        second.append(u8"           SGM/FC=X.XXX  TAU/FC=X.XXX                           SGM/FC,TAU/FC        SGM/FC=X.XXX  TAU/FC=X.XXX").append("\n");
        second.append(u8"           4-D25( 75):Y1   2-D25( 91):Y1                          Y- BAR             4-D25( 75):Y1   2-D25( 91):Y1").append("\n");
        second.append(u8"           H0/D=XX.XX  PT=X.XX(%)     MRANK=(-)  SRANK=(-)        Y- H0/D,PT         H0/D=XX.XX  PT=X.XX(%)     MRANK=(-)  SRANK=(-)").append("\n");
        second.append(u8"           4-D25( 75):Y1   2-D25( 91):Y1                          Z- BAR             4-D25( 75):Y1   2-D25( 91):Y1").append("\n");
        second.append(u8"           H0/D=XX.XX  PT=X.XX(%)     MRANK=(-)  SRANK=(-)        Z- H0/D,PT         H0/D=XX.XX  PT=X.XX(%)     MRANK=(-)  SRANK=(-)").append("\n");
        second.append(u8"          +------------------ BOTTOM --------------------+                          +-------------------- TOP ---------------------+").append("\n");
        second.append(u8"           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK").append("\n");
        second.append(u8"MZ,QY(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)").append("\n");
        second.append(u8"ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)").append("\n");
        second.append(u8"                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~ ~~~~~~~                                          ~~~~~~~").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  柱のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)").append("\n");
        second.append(u8"           XXXXX  X  XXXXX      (XXXXXXXX)                          B X D            XXXXX  X  XXXXX      (XXXXXXXX)").append("\n");
        second.append(u8"           4-D25( 75):Y1   2-D25( 91):Y1                          Y- BAR             4-D25( 75):Y1   2-D25( 91):Y1").append("\n");
        second.append(u8"           WH-1000X 201X 12X 20 (XXXXXXXX)                        Y- STEEL           WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"           N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)        Y- N/N0,SM/M       N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)").append("\n");
        second.append(u8"           4-D25( 75):Y1   2-D25( 91):Y1                          Z- BAR             4-D25( 75):Y1   2-D25( 91):Y1").append("\n");
        second.append(u8"           WH-1000X 201X 12X 20 (XXXXXXXX)                        Z- STEEL           WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"           N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)        Z- N/N0,SM/M       N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)").append("\n");
        second.append(u8"          +------------------ BOTTOM --------------------+                          +-------------------- TOP ---------------------+").append("\n");
        second.append(u8"           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK").append("\n");
        second.append(u8"MZ,QY(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)").append("\n");
        second.append(u8"ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)").append("\n");
        second.append(u8"                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~ ~~~~~~~                                          ~~~~~~~").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  柱のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SC-  L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)").append("\n");
        second.append(u8"           P -1000X 20          (XXXXXXXX)                          STEEL            P -1000X201          (XXXXXXXX)").append("\n");
        second.append(u8"           D/T=XX.X   AS=XXXX.X  SGY=XX.XXX                         D/T,AS           D/T=XX.X   AS=XXXX.X  SGY=XX.XXX").append("\n");
        second.append(u8"           FC =XXXX.X AC=XXXX.X (XXXXXXXX)                         CONCRETE          FC=XXXX.XX AC=XXXX.X (XXXXXXXX)").append("\n");
        second.append(u8"           CFC=XX.XXX SFC=XX.XXX SFT=XX.XXX                        CONF.EFFECT       CFC=XX.XXX SFC=XX.XXX SFT=XX.XXX").append("\n");
        second.append(u8"           N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)        Y- N/N0,SM/M       N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)").append("\n");
        second.append(u8"          +------------------ BOTTOM --------------------+                          +-------------------- TOP ---------------------+").append("\n");
        second.append(u8"           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK").append("\n");
        second.append(u8"MZ,QY(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)").append("\n");
        second.append(u8"ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)").append("\n");
        second.append(u8"                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~ ~~~~~~~                                          ~~~~~~~").append("\n");
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
