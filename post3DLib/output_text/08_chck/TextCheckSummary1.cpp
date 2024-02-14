#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextCheckSummary1::write(const FixedData& fixedData) const{
    //PR501A

    QString second;
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};}
} // namespace post3dapp
