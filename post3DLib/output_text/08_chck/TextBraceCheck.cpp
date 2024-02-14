#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextBraceCheck::write(const FixedData& fixedData) const{
    //CODE+PR531

    QString second;
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXXXXXXXXXXXX ( XXXXXX-FL XXXXXX-FRAME XXXXXX-XXXXXX (XXXXXX) XXXXXX-XXXXXX ) -S-   L=XX.XXX L0=XX.XXX(M)  FL=X.XXXX FR=X.XXXX(M)").append("\n");
        second.append(u8"        Y-鉄骨           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"          幅厚比         B/F=XX.X  D/W=XX.X").append("\n");
        second.append(u8"        Z-鉄骨           WH-HHHHXBBBBXWWWXFFF (XXXXXXXX)").append("\n");
        second.append(u8"          幅厚比         B/F=XX.X  D/W=XX.X").append("\n");
        second.append(u8"        圧縮検討用       ly=XXXXX. iy=XX.XX").append("\n");
        second.append(u8"                         lz=XXXXX. iz=XX.XX").append("\n");
        second.append(u8"").append("\n");
        second.append(u8"                             PERM     TEMP     TEMP").append("\n");
        second.append(u8"          ft    (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX").append("\n");
        second.append(u8"          fc    (N/mm2)  XXXXX.XX XXXXX.XX XXXXX.XX").append("\n");
        second.append(u8"          NL,  NS  (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XT").append("\n");
        second.append(u8"          NAL, NAS (kN)  XXXXXX.XTXXXXXX.XTXXXXXX.XT").append("\n");
        second.append(u8"          検定比         XXXXX.XX XXXXX.XX XXXXX.XX").append("\n");
        second.append(u8"          変動分検定比        --- XXXXX.XX XXXXX.XX").append("\n");
        second.append(u8"").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
