#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextMaximumDeform::write(const FixedData& fixedData) const{
    //PR403

    QString second;
    second.append(u8"+---------------------------------------------------------------------------------------------------------------------------+").append("\n");
    second.append(u8"|               CASE : XXXXXXXXXX      FILE : XXXXXXXXXX      FORCE ANGLE = XXXX.XX                                         |").append("\n");
    second.append(u8"--------+---------------------------------------------------------------------------------------+--------------------------+").append("\n");
    second.append(u8"| 階名   | 形状階高 解析用階高 解析部材高さ 計算用階高 部材の水平変形 最大層間変形角の逆数 JUDGE |         POSITION         |").append("\n");
    second.append(u8"|        |    SH       AH          ZM       CH=ZM-AH+SH      δ             CH/δ                |                          |").append("\n");
    second.append(u8"|        |    (M)      (M)         (M)         (M)          (MM)                                 |   IMEM   GR1      GR2    |").append("\n");
    second.append(u8"+--------+---------------------------------------------------------------------------------------+--------------------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX |                                                                                       |                          |").append("\n");
        second.append(u8"|        |  XX.XXX    XX.XXX     XX.XXX       XX.XXX       XXXX.X         XXXXXXXXXXXX.  XXXXXXX | XXXXXX   XXXXXX   XXXXXX |").append("\n");
        second.append(u8"+--------+---------------------------------------------------------------------------------------+--------------------------+").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
