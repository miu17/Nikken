#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextCheckSummary2::write(const FixedData& fixedData) const{
    //PR501B

    QString second;
    second.append(u8"XXX ZZ- ZZ  リスト毎の検定比分布(曲げ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"").append("\n");
    //}
    second.append(u8"XXX ZZ- ZZ  リスト毎の検定比分布(せん断) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};}
} // namespace post3dapp
