#include "abstracttext.h"
#include "unified_inputdata.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextMatc::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8"    ■ コンクリ－ト材料     UNIT=N/MM2").append("\n");
    second.append(u8"                                                   +-------PERMANENT---------+ +--------TEMPORARY--------+ +----------ULTIMATE--------+").append("\n");
    second.append(u8"             NAME          F       E         G      COMP.  SHEAR     BOND       COMP.  SHEAR     BOND        COMP.  SHEAR  TENS.   BOND    N").append("\n");
    second.append(u8"                                                                   TOP GENERAL                 TOP GENERAL").append("\n");
    Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPMACONC)){
        second.append(sec).append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
