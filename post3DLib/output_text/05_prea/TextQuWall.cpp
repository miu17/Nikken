#include "abstracttext.h"

namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextQuWall::write(const FixedData& fixedData) const{
    //AREAMM
    //PR351
    QString second;
    second.append(u8"XXX ZZ- ZZ  耐力壁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -S-    L=XX.XXX(M)  L0=XX.XXX(M)").append("\n");
        second.append(u8"   T X L0          200 X 5500          (XXXXXXXX)      QU =XXXXXX.X(KN)").append("\n");
        second.append(u8"   GAMMA          GAMMA=X.XXX                          TAU=XXXXX.XX(KN/CM2)").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  耐力壁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -RC-   L=XX.XXX(M)  L0=XX.XXX(M)").append("\n");
        second.append(u8"   B X D, T X L   600 X 600 ( Y )      (XXXXXXXX)    200 X 5500           (XXXXXXXX)    600 X 600 ( Y )      (XXXXXXXX)").append("\n");
        second.append(u8"   BAR(Y,Z)       4-D25  4-D25  IN 4-D25  4-D25      2-D10D10-@200       PW=XX.XX(%)    4-D25  4-D25  IN 4-D25  4-D25").append("\n");
        second.append(u8"   AT,PT,GAMMA    AT=XXX.XX(CM2)   PT=XX.XX(%)       GAMMA=X.XXX                        AT=XXX.XX(CM2)   PT=XX.XX(%)").append("\n");
        second.append(u8"                 +--------- L COLUMN ---------+     +---------- WALL ------------+     +-------- R COLUMN ----------+").append("\n");
        second.append(u8"                  BE=XX.XXX(M)  DD=XX.XXX(M) JJ=XX.XXX(M) PT=XX.XX(%) SGMW=XXXXX.XX(KN/CM2) SGM0=XXXXX.XX(KN/CM2)").append("\n");
        second.append(u8"                  M/QD=X.XXX    PHAI=X.XXX                            QCR =XXXXXX.X(KN)     QU  =XXXXXX.X(KN)").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  耐力壁のせん断耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -SRC-  L=XX.XXX(M)  L0=XX.XXX(M)").append("\n");
        second.append(u8"   B X D, T X L   600 X 600(Y)         (XXXXXXXX)    200 X 5500           (XXXXXXXX)    600 X 600(Y)         (XXXXXXXX)").append("\n");
        second.append(u8"   HOOP,BAR-T     2-D13-@100           (PW=X.XX%)    2-D10D10-@200        (PS=X.XX%)    2-D13-@100           (PW=X.XX%)").append("\n");
        second.append(u8"        BAR-Y                                        2-D10D10-@200        (PS=X.XX%)").append("\n");
        second.append(u8"   STEEL(Y)       WH-1000X 201X 12X 20 (XXXXXXXX)                                       WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"   STEEL(Z)       WH-1000X 201X 12X 20 (XXXXXXXX)                                       WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"   GAMMA,C-FACT.  C-FACT.=X.XXX                      GAMMA=X.XXX                        C-FACT.=X.XXX").append("\n");
        second.append(u8"                 +--------- L COLUMN ---------+     +---------- WALL ------------+     +-------- R COLUMN ----------+").append("\n");
        second.append(u8"   QCU,QWU              QCU=XXXXXX.X(KN)                            QWU=XXXXXX.X(KN)         QCU=XXXXXX.X(KN)").append("\n");
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}

} // namespace post3dapp
