#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextNuBrace::write(const FixedData& fixedData) const{
    //AREAMM
    //PR341
    QString second;
    second.append(u8"XXX ZZ- ZZ  ブレ－スの軸耐力 ( MODEL-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -SBR- L=XX.XXX(M) L0=XX.XXX(M)  FB=X.XXXX FT=X.XXXX(M)").append("\n");
        second.append(u8"        Y-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"          W/T            B/F=XX.X  D/W=XX.X").append("\n");
        second.append(u8"        Z-STEEL          WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"          W/T            B/F=XX.X  D/W=XX.X").append("\n");
        second.append(u8"        COMPRESSION      LY=XXXXX. IY=XX.XX  RY=XXX.X").append("\n");
        second.append(u8"                         LZ=XXXXX. IZ=XX.XX  RZ=XXX.X").append("\n");
        second.append(u8"").append("\n");
        second.append(u8"          NT  (KN)       XXXXXXX.X").append("\n");
        second.append(u8"          NC  (KN)       XXXXXXX.X").append("\n");
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
