#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextQuColumn::write(const FixedData& fixedData) const{
    //AREAMM
    //PR331

    QString second;
    second.append(u8"XXX ZZ- ZZ  柱のせん断耐力 ( MODEL-FILE : (DIR.1)XXXXXXXXXX (DIR.2)XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)").append("\n");
        second.append(u8"        Y-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                      WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"        Z-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)                      WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"                        +-----BOTTOM DIR.1----+  +-----BOTTOM DIR.2----+     +-----TOP DIR.1-------+  +-----TOP DIR.2-------+").append("\n");
        second.append(u8"                        +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+     +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+").append("\n");
        second.append(u8"          Q0  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          QU  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  柱のせん断耐力 ( MODEL-FILE : (DIR.1)XXXXXXXXXX (DIR.2)XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -RC-  L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)").append("\n");
        second.append(u8"          B X D          XXXXX  X  XXXXX      (XXXXXXXX)                      XXXXX  X  XXXXX      (XXXXXXXX)").append("\n");
        second.append(u8"          Y- BAR         4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1").append("\n");
        second.append(u8"             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX                        AT=XXX.XX(X.XX) D=XXXX J=XXXX").append("\n");
        second.append(u8"             HOOP PW(%)  2-D13-@100          PW= .23                          2-D10-@180          PW= .23").append("\n");
        second.append(u8"          Z- BAR         4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1").append("\n");
        second.append(u8"             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX                        AT=XXX.XX(X.XX) D=XXXX J=XXXX").append("\n");
        second.append(u8"             HOOP PW(%)  2-D13-@100          PW= .23                          2-D10-@180          PW= .23").append("\n");
        second.append(u8"                        +-----BOTTOM DIR.1----+  +-----BOTTOM DIR.2----+     +-----TOP DIR.1-------+  +-----TOP DIR.2-------+").append("\n");
        second.append(u8"                        +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+     +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+").append("\n");
        second.append(u8"          Q0  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          QE  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          M/QD           XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX").append("\n");
        second.append(u8"          PT  (%)        XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX").append("\n");
        second.append(u8"          SWY (N/MM2)    XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          S0  (N/MM2)    XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          B   (CM)       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX").append("\n");
        second.append(u8"          J   (CM)       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX       XXXXXX.XX   XXXXXX.XX    XXXXXX.XX   XXXXXX.XX").append("\n");
        second.append(u8"          QU  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  柱のせん断耐力 ( MODEL-FILE : (DIR.1)XXXXXXXXXX (DIR.2)XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SRC- L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)").append("\n");
        second.append(u8"          B X D          XXXXX  X  XXXXX      (XXXXXXXX)                      XXXXX  X  XXXXX      (XXXXXXXX)").append("\n");
        second.append(u8"          Y- BAR         4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1").append("\n");
        second.append(u8"             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX                        AT=XXX.XX(X.XX) D=XXXX J=XXXX").append("\n");
        second.append(u8"             HOOP PW(%)  2-D13-@100          PW= .23                          2-D10-@180          PW= .23").append("\n");
        second.append(u8"             STEEL       WH-1000X 201X 12X 20 (XXXXXXXX)                      WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"          Z- BAR         4-D25( 75):Y1   2-D25( 91):Y1                        4-D25( 75):Y1   2-D25( 91):Y1").append("\n");
        second.append(u8"             AT,D,J      AT=XXX.XX(X.XX) D=XXXX J=XXXX                        AT=XXX.XX(X.XX) D=XXXX J=XXXX").append("\n");
        second.append(u8"             HOOP PW(%)  2-D13-@100          PW= .23                          2-D10-@180          PW= .23").append("\n");
        second.append(u8"             STEEL       WH-1000X 201X 12X 20 (XXXXXXXX)                      WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"                        +-----BOTTOM DIR.1----+  +-----BOTTOM DIR.2----+     +-----TOP DIR.1-------+  +-----TOP DIR.2-------+").append("\n");
        second.append(u8"                        +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+     +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+").append("\n");
        second.append(u8"          Q0   (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          QE   (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          MBQD           XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X       XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          FS   (KN/MM2)  XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          B    (CM)      XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X       XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          B'   (CM)      XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X       XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          J    (CM)      XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X       XXXXXX.XX   XXXXXX.XX    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          RSY  (KN/MM2)  XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          RQU1 (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          RQU2 (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          SQU1 (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          SQU2 (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          QU   (KN)      XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  柱のせん断耐力 ( MODEL-FILE : (DIR.1)XXXXXXXXXX (DIR.2)XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SC-  L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)").append("\n");
        second.append(u8"          STEEL          P -1000X 20          (XXXXXXXX)                      P -1000X201          (XXXXXXXX)").append("\n");
        second.append(u8"                        +-----BOTTOM DIR.1----+  +-----BOTTOM DIR.2----+     +-----TOP DIR.1-------+  +-----TOP DIR.2-------+").append("\n");
        second.append(u8"                        +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+     +--X DIR--+ +--Y DIR--+  +--X DIR--+ +--Y DIR--+").append("\n");
        second.append(u8"          Q0  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"          QU  (KN)       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X       XXXXXXX.X   XXXXXXX.X    XXXXXXX.X   XXXXXXX.X").append("\n");
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
