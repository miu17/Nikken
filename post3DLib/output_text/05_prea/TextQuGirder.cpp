#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextQuGirder::write(const FixedData& fixedData) const{
    //AREAMM
    //PR321

    QString second;
    second.append(u8"XXX ZZ- ZZ  大梁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)").append("\n");
        second.append(u8"          STEEL          WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"                        +--------- LEFT END ---------+     +--------- CENTER -----------+     +-------- RIGHT END ---------+").append("\n");
        second.append(u8"          Q0  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          QU  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  大梁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)").append("\n");
        second.append(u8"          B X D          XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)").append("\n");
        second.append(u8"          UPPER-BAR      2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)").append("\n");
        second.append(u8"             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX").append("\n");
        second.append(u8"          LOWER-BAR      2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)").append("\n");
        second.append(u8"             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX").append("\n");
        second.append(u8"          STIRRUP PW(%)  2-D10-@180          PW= .23        2-D10-@180          PW= .23        2-D10-@180          PW= .23").append("\n");
        second.append(u8"                        +--------- LEFT END ---------+     +--------- CENTER -----------+     +-------- RIGHT END ---------+").append("\n");
        second.append(u8"          Q0  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          QE  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          M/QD                    XXXXXX.XX                          XXXXXX.XX                          XXXXXX.XX").append("\n");
        second.append(u8"          PT  (%)                 XXXXXX.XX                          XXXXXX.XX                          XXXXXX.XX").append("\n");
        second.append(u8"          SWY (KN/CM2)            XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          B   (CM)                XXXXXX.XX                          XXXXXX.XX                          XXXXXX.XX").append("\n");
        second.append(u8"          J   (CM)                XXXXXX.XX                          XXXXXX.XX                          XXXXXX.XX").append("\n");
        second.append(u8"          QU  (KN)                XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  大梁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)").append("\n");
        second.append(u8"          B X D          XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)    XXXXX  X  XXXXX      (XXXXXXXX)").append("\n");
        second.append(u8"          UPPER-BAR      2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)").append("\n");
        second.append(u8"             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX").append("\n");
        second.append(u8"          LOWER-BAR      2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)   2-D25( 75) 2-D25( 83) 2-D25( 91)").append("\n");
        second.append(u8"             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX      AT=XXX.XX(X.XX) D=XXXX J=XXXX").append("\n");
        second.append(u8"          STIRRUP PW(%)  2-D10-@180          PW= .23        2-D10-@180          PW= .23        2-D10-@180          PW= .23").append("\n");
        second.append(u8"          STEEL          WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)    WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"                        +--------- LEFT END ---------+     +--------- CENTER -----------+     +-------- RIGHT END ---------+").append("\n");
        second.append(u8"          Q0   (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          QE   (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          MBQD                    XXXXXX.XX                          XXXXXX.XX                          XXXXXXX.X").append("\n");
        second.append(u8"          FS   (KN/CM2)           XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          B    (CM)               XXXXXX.XX                          XXXXXX.XX                          XXXXXXX.X").append("\n");
        second.append(u8"          B'   (CM)               XXXXXX.XX                          XXXXXX.XX                          XXXXXXX.X").append("\n");
        second.append(u8"          J    (CM)               XXXXXX.XX                          XXXXXX.XX                          XXXXXXX.X").append("\n");
        second.append(u8"          RSY  (KN/CM2)           XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          RQU1 (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          RQU2 (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          SQU1 (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          SQU2 (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"          QU   (KN)               XXXXXXX.X                          XXXXXXX.X                          XXXXXXX.X").append("\n");
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
