#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextJointLoad::write(const FixedData& fixedData) const{
    //PR203
    QString second;
    second.append(u8"■ 荷重名     XXXXXX").append("\n");
    second.append(u8"           NO. FL     GR1    GR2      X-COORD   Y-COORD   Z-COORD         FX          FX          FX          MX          MY          MZ").append("\n");
    second.append(u8"                                        (M)       (M)       (M)          (KN)        (KN)        (KN)       (KNm)       (KNm)       (KNm)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX-      -      -       XXXX.XXXX XXXX.XXXX XXXX.XXXX XXXXXXXX.XX XXXXXXXX.XX XXXXXXXX.XX XXXXXXXX.XX XXXXXXXX.XX XXXXXXXX.XX").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
