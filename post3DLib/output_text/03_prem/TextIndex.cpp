#include "abstracttext.h"
#include "unified_inputdata.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextIndex::write(const FixedData& fixedData) const{
    //PR062
    QString second;
    second.append(u8"               NO. FL     GR1    GR2      X-COORD   Y-COORD   Z-COORD     DX     DY     DZ     RX     RY     RZ     GX     GY     GZ").append("\n");
    second.append(u8"                                            (M)       (M)       (M)                 (*****:FIX  <0:DEPENDENT)").append("\n");
    Q_FOREACH(auto joint, UnifiedInputData::getInInstance()->jointListOfAll()){
        second.append(u8"    (XXXXX) XXXXXX-      -      -       XXXX.XXXX XXXX.XXXX XXXX.XXXX  *****  *****  *****  *****  *****  *****  *****  *****  *****").append("\n");
    }
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
