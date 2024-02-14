#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextQdCurve::write(const FixedData& fixedData) const{
    //PR603

    QString second;
    second.append(u8"XXX ZZ- ZZ  弾塑性解析経過 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"■ XXXXXX FL  ( ANGLE = XXXX.XX   HEIGHT = XXXXX.X (MM) )").append("\n");
    second.append(u8"FL   STEP   COEF    STATUS  SHEAR  X-DISP. Y-DISP.  THETA-Z F-DISP.   H/D    FRAME           BRACE           WALL               DK    DK/DK1").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXX XXXX XX.XXXX          XXXXXXX XXXX.XX XXXX.XX XXX.XXXX XXXX.XX XXXXXX XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXX(XXXX.X) XXXXXXXX.X(XXXX.X)").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
