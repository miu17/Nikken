#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrameStiffness::write(const FixedData& fixedData) const{
    //PR406

    QString second;
    second.append(u8"■ CASE : XXXXXXXXXX    FILE : XXXXXXXXXX    FORCE ANGLE : XXXX.XX").append("\n");
    second.append(u8"").append("\n");
    second.append(u8"          XXXXXX ( ANGLE = XXXX.XX )    XXXXXX ( ANGLE = XXXX.XX )    XXXXXX ( ANGLE = XXXX.XX )   XXXXXX ( ANGLE = XXXX.XX )").append("\n");
    second.append(u8"FLOOR       SHEAR   DISP.     STIFF.      SHEAR   DISP.     STIFF.      SHEAR   DISP.     STIFF.     SHEAR   DISP.     STIFF.").append("\n");
    second.append(u8"             (KN)    (MM)    (KN/CM)       (KN)    (MM)    (KN/CM)       (KN)    (MM)    (KN/CM)      (KN)    (MM)    (KN/CM)").append("\n");
    second.append(u8"").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXX   XXXXXX.X XXXX.XX XXXXXXXX.X   XXXXXX.X XXXX.XX XXXXXXXX.X   XXXXXX.X XXXX.XX XXXXXXXX.X  XXXXXX.X XXXX.XX XXXXXXXX.X").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
