#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrQdCurve::write(const FixedData& fixedData) const{
    //PR604

    QString second;
    second.append(u8"XXX ZZ- ZZ  架構の弾塑性解析経過 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"■ XXXXXX FRAME   XXXXXX FL  ( FORCE ANGLE = XXXX.XX   FRAME ANGLE = XXXX.XX   HEIGHT = XXXXX.X (MM) )").append("\n");
    second.append(u8"FR   FL   STEP   COEF    STATUS  SHEAR   DISP.    H/D    FRAME           BRACE           WALL            TOTAL              DK    DK/DK1").append("\n");
    second.append(u8"                       cmhsaHMS   (KN)   (MM)            (KN)    (%)     (KN)    (%)     (KN)    (%)     (KN)    (%)     (KN/CM)    (%)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXX XXXX XXXX XX.XXXX          XXXXXXX XXXX.XX XXXXXX XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXXX.X(XXXX.X)").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
