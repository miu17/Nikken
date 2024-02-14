#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextStoryStiffness::write(const FixedData& fixedData) const{
    //PR402

    QString second;
    second.append(u8"+--------------------------------------------------------------------------------------------------------------------------------------------------------------+").append("\n");
    second.append(u8"|               CASE : XXXXXXXXXX      FILE : XXXXXXXXXX      FORCE ANGLE = XXXX.XX                                                                            |").append("\n");
    second.append(u8"+-------+--------+----------+------------++-------------------+---------+---------+---------+---------+------------++-------------------+---------+------------+").append("\n");
    second.append(u8"| FLOOR |  FL.H  |  SHEAR.F |    O.T.M   ||   CENTER OF S.F   | X-DISP. | Y-DISP. | THETA-Z |  DISP.  | STIFFNESS  ||   CENTER OF RIG.  |  DISP.  | STIFFNESS  |").append("\n");
    second.append(u8"|       |   (M)  |   (KN)   |   (KN*M)   ||   XC(M)    YC(M)  |  (MM)   |  (MM)   |  (MRAD) |  (MM)   |  (KN/CM)   ||   XK(M)    YK(M)  |  (MM)   |  (KN/CM)   |").append("\n");
    second.append(u8"+-------+--------+----------+------------++-------------------+---------+---------+---------+---------+------------++-------------------+---------+------------+").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"| XXXXXX|        |          | XXXXXXXXXX.||                   |         |         |         |         |            ||                   |         |            |").append("\n");
        second.append(u8"|       | XX.XXX | XXXXXX.X |            || XXXX.XXX XXXX.XXX | XXXX.XX | XXXX.XX | XX.XXXX | XXXX.XX | XXXXXXXX.X || XXXX.XXX XXXX.XXX | XXXX.XX | XXXXXXXX.X |").append("\n");
        second.append(u8"+-------+--------+----------+------------++-------------------+---------+---------+---------+---------+------------++-------------------+---------+------------+").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
