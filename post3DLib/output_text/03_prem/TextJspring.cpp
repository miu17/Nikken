#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextJspring::write(const FixedData& fixedData) const{
    //PR122
    QString second;
    second.append(u8"SEQ.NO.   JNO. FL     GR1    GR2  AX/RT  PROP    V-X    V-Y    V-Z TYPE      K1         K2         K3         Y1+      Y1-      Y2+      Y2-").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
