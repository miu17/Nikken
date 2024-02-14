#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextWallCheck::write(const FixedData& fixedData) const{
    //CODE+PR541

    QString second;
    second.append(u8"XXX ZZ- ZZ  耐力壁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -S-    L=XX.XXX(M)  L0=XX.XXX(M)").append("\n");
        second.append(u8"   T X L           200 X 5500          (XXXXXXXX)").append("\n");
        second.append(u8"   γ              γ  =X.XXX").append("\n");
        second.append(u8"                 +----------- WALL -----------+").append("\n");
        second.append(u8"                      PERM     TEMP     TEMP").append("\n");
        second.append(u8" QL, QH  (kN)     XXXXXX.X XXXXXX.X XXXXXX.X").append("\n");
        second.append(u8" QL, QS  (kN)     XXXXXX.X XXXXXX.X XXXXXX.X").append("\n");
        second.append(u8" QAL,QAS (kN)     XXXXXX.X XXXXXX.X XXXXXX.X").append("\n");
        second.append(u8" τL,τS(N/mm2)   XXXXXX.X XXXXXX.X XXXXXX.X").append("\n");
        second.append(u8" QD/QA            XXXXXX.X XXXXXX.X XXXXXX.X").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  耐力壁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -RC-   L=XX.XXX(M)  LI=XX.XXX(M)").append("\n");
        second.append(u8"   B X D, T X L   600 X 600(Y)         (XXXXXXXX)    200 X 5500           (XXXXXXXX)    600 X 600(Y)         (XXXXXXXX)").append("\n");
        second.append(u8"   鉄筋           2-D13-@100           (pw=X.XX%)    2-D10D10-@200        (ps=X.XX%)    2-D13-@100           (pw=X.XX%)").append("\n");
        second.append(u8"   γ,柱有効係数  左柱係数 =X.XXX  γ=X.XXX H=XX.XXX(M) L0=XX.XXX(M) H0=XX.XXX(M) L0*H0=XX.XXXX(M^2)   右柱係数 =X.XXX").append("\n");
        second.append(u8"                 +----------- 左柱 ---------+     +------------ 壁 ------------+     +----------- 右柱 -----------+").append("\n");
        second.append(u8"                      PERM     TEMP     TEMP             PERM     TEMP     TEMP             PERM     TEMP     TEMP").append("\n");
        second.append(u8"   QL, QH  (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QL, QS  (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QA1     (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QW, QC  (kN)   XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X").append("\n");
        second.append(u8"   QA2                 ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QAL,QAS (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   τL,τS (N/mm2)     ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QL/QAL ,QS/QAS      ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  耐力壁の断面検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXX ( XXXXXX-FL  XXXXXX-FRAME  XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX ) -SRC-  L=XX.XXX(M)  LI=XX.XXX(M)").append("\n");
        second.append(u8"   B X D, T X L   600 X 600(Y)         (XXXXXXXX)    200 X 5500           (XXXXXXXX)    600 X 600(Y)         (XXXXXXXX)").append("\n");
        second.append(u8"   鉄筋           2-D13-@100           (pw=X.XX%)    2-D10D10-@200        (ps=X.XX%)    2-D13-@100           (pw=X.XX%)").append("\n");
        second.append(u8"   鉄骨(Y)        WH-1000X 201X 12X 20 (XXXXXXXX)                                       WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"   鉄骨(Z)        WH-1000X 201X 12X 20 (XXXXXXXX)                                       WH-1000X 201X 12X 20 (XXXXXXXX)").append("\n");
        second.append(u8"   γ,柱有効係数  左柱係数 =X.XXX  γ=X.XXX H=XX.XXX(M) L0=XX.XXX(M) H0=XX.XXX(M) L0*H0=XX.XXXX(M^2)   右柱係数 =X.XXX").append("\n");
        second.append(u8"                 +----------- 左柱 ---------+     +------------ 壁 ------------+     +----------- 右柱 -----------+").append("\n");
        second.append(u8"                      PERM     TEMP     TEMP             PERM     TEMP     TEMP             PERM     TEMP     TEMP").append("\n");
        second.append(u8"   QL, QH  (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QL, QS  (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QA1                 ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QW, QC  (kN)   XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X         XXXXXX.X XXXXXX.X XXXXXX.X").append("\n");
        second.append(u8"   QA2                 ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QAL,QAS (kN)        ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   τL,τS (N/mm2)     ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"   QL/QAL ,QS/QAS      ---      ---      ---         XXXXXX.X XXXXXX.X XXXXXX.X              ---      ---      ---").append("\n");
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
