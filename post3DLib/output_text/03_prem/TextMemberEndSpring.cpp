#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextMemberEndSpring::write(const FixedData& fixedData) const{
    //PR130
    QString second;
    second.append(u8"部材番号 節点番号I,J  階     -      通り   通り   符号        位置 タイプ    K1         K2         K3         Y1+      Y1-      Y2+      Y2-").append("\n");
    second.append(u8"                      階     架構   通り   通り                           (KN*M/RAD) (KN*M/RAD) (KN*M/RAD)   (KN*M)   (KN*M)   (KN*M)   (KN*M)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXXXXXX XXX XXXXXX|----->").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
