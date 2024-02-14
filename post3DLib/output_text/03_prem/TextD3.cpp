#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextD3::write(const FixedData& fixedData) const{
    QString second;
    /*
    Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"").append("\n");
    }
    */
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
