#include "abstracttext.h"
#include "unified_inputdata.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextMats::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"    ■ 鋼材料               UNIT=N/MM2").append("\n");
    second.append(u8"                                                   +--PERMANENT--+ +--TEMPORARY--+ +---ULTIMATE--+").append("\n");
    second.append(u8"             NAME          F       E         G       COMP.   SHEAR   COMP.   SHEAR   COMP.   SHEAR").append("\n");
    second.append(u8"                                                     TENS.           TENS.           TENS.").append("\n");
    Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPMASTEEL)){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
