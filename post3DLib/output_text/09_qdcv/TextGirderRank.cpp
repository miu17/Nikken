#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextGirderRank::write(const FixedData& fixedData) const{
    //PR621

    QString second;
    second.append(u8"XXX ZZ- ZZ  大梁のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)").append("\n");
        second.append(u8"           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                           STEEL           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"           B/F=XX.X D/W=XX.X          MRANK=(-)  SRANK=(-)            W/T            B/F=XX.X  D/W=XX.X         MRANK=(-)  SRANK=(-)").append("\n");
        second.append(u8"          +------------------- LEFT ---------------------+                          +------------------- RIGHT --------------------+").append("\n");
        second.append(u8"           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK").append("\n");
        second.append(u8"MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)").append("\n");
        second.append(u8"ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)").append("\n");
        second.append(u8"                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~                                                  ~~~~~~~").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  大梁のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)").append("\n");
        second.append(u8"           XXXXX  X  XXXXX      (XXXXXXXX)                          B X D            XXXXX  X  XXXXX      (XXXXXXXX)").append("\n");
        second.append(u8"           2-D25( 75) 2-D25( 83) 2-D25( 91)                       UPPER-BAR          2-D25( 75) 2-D25( 83) 2-D25( 91)").append("\n");
        second.append(u8"           2-D25( 75) 2-D25( 83) 2-D25( 91)                       LOWER-BAR          2-D25( 75) 2-D25( 83) 2-D25( 91)").append("\n");
        second.append(u8"           TAU/FC=X.XXX               MRANK=(-)  SRANK=(-)         TAU/FC            TAU/FC=X.XXX               MRANK=(-)  SRANK=(-)").append("\n");
        second.append(u8"          +------------------- LEFT ---------------------+                          +------------------- RIGHT --------------------+").append("\n");
        second.append(u8"           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK").append("\n");
        second.append(u8"MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)").append("\n");
        second.append(u8"ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)").append("\n");
        second.append(u8"                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~                                                  ~~~~~~~").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  大梁のランク ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)").append("\n");
        second.append(u8"           XXXXX  X  XXXXX      (XXXXXXXX)                          B X D            XXXXX  X  XXXXX      (XXXXXXXX)").append("\n");
        second.append(u8"           2-D25( 75) 2-D25( 83) 2-D25( 91)                       UPPER-BAR          2-D25( 75) 2-D25( 83) 2-D25( 91)").append("\n");
        second.append(u8"           2-D25( 75) 2-D25( 83) 2-D25( 91)                       LOWER-BAR          2-D25( 75) 2-D25( 83) 2-D25( 91)").append("\n");
        second.append(u8"           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                          STEEL            WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"           N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)        N/N0,SM/M          N/N0=X.XXX  SM/M=X.XXX     MRANK=(-)  SRANK=(-)").append("\n");
        second.append(u8"          +------------------- LEFT ---------------------+                          +------------------- RIGHT --------------------+").append("\n");
        second.append(u8"           INITIAL    LAST     ULT   R.ULT     BRK   R.BRK   BEY.SH  BRK.SH  ULT.SH  INITIAL    LAST     ULT   R.ULT     BRK   R.BRK").append("\n");
        second.append(u8"MY,QZ(NOT)FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.X FXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.XFXXXXX.X").append("\n");
        second.append(u8"STATUS,QV                   BEND.Y           BEND.(COL.BR) FXXXXX.XFXXXXX.XFXXXXX.X                   BEND.Y           BEND.(COL.BR)").append("\n");
        second.append(u8"ALPHA,RANK                  XXX.XX          XXX.XX RANK(-)  IMD(--) GMD(--) QRNK(-)                   XXX.XX          XXX.XX RANK(-)").append("\n");
        second.append(u8"                                                   ~~~~~~~  ~~~~~~~ ~~~~~~~                                                  ~~~~~~~").append("\n");
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
