#include "abstracttext.h"
#include "define_section_struct.h"
#include "unified_inputdata.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextMatr::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"    ■ 鉄筋材料             UNIT=N/MM2").append("\n");
    second.append(u8"                                 +--PERMANENT--+ +--TEMPORARY--+ +---ULTIMATE--+").append("\n");
    second.append(u8"             NAME        ( E )     COMP.   SHEAR   COMP.   SHEAR   COMP.   SHEAR").append("\n");
    second.append(u8"                                   TENS.           TENS.           TENS.").append("\n");
    Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(sec.sectionName).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
