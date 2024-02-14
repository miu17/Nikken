#include "abstracttext.h"
#include "fortranFunc.h"
#include "unified_inputdata.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextJnt::write(const FixedData& fixedData) const{
    //PR061
    QString second;
    second.append(u8"               NO. FL     GR1    GR2      0:FREE 1:FIX   X-COORD   Y-COORD   Z-COORD RGDFL    SEMASS FL     FR1    FR2    FR3    FR4").append("\n");
    //second.append(u8"               NO. FL     GR1    GR2      X-COORD   Y-COORD   Z-COORD     DX     DY     DZ     RX     RY     RZ     GX     GY     GZ").append("\n");
    //second.append(u8"                                            (M)       (M)       (M)                 (*****:FIX  <0:DEPENDENT)").append("\n");
    /*
    Q_FOREACH(auto joint, fixedData.getPstnData()->getCmdlData().namej){
        //second.append(u8"    (XXXXX) XXXXXX-      -      -        XXX  XXX  XXX XXXX.XXXX XXXX.XXXX XXXX.XXXX XXXXXX XXXXXX.X XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX").append("\n");
        second.append(joint).append("\n");

    }
    */
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
