#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextMember::write(const FixedData& fixedData) const{
    QString second;
    second.append(u8" MSEQ.   I-JNT  J-JNT LIST-NAME").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX XXXXXX XXXXXXXXXXXXXXXX").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
