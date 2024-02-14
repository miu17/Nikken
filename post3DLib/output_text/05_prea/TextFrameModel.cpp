#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrameModel::write(const FixedData& fixedData) const{
    //PR301
    QString second;
    second.append(u8"■ XXXXXX FRAME  GIRDER **                                                         KY'=EIZ*(IZ/IZ0)(+Z)/L    KZ'=EIY*(IY/IY0)(+Z)/L").append("\n");
    second.append(u8"■ XXXXXX FRAME  COLUMN **                                                         KY'=EIZ*(IZ/IZ0)(+Z)/L    KZ'=EIY*(IY/IY0)(+Z)/L").append("\n");
    second.append(u8"■ XXXXXX FRAME  BRACE  **").append("\n");
    second.append(u8"  SEQ.      I      J   FL    FRAME   GR1   GR2    LIST                         L(M)").append("\n");
    second.append(u8"          FLI   RLYI   RLYJ   MZQY   I-T-J  +W MODEL     MDL   EIZ  (IZ/IZ0)   EIZ'     GAY  (AY/AY0)    KY'     EAX  (AX/AX0)     GJ  (JX/JX0)").append("\n");
    second.append(u8"          FLJ   RLZI   RLZJ   MYQZ   I-T-J  +W                 EIY  (IY/IY0)   EIY'     GAZ  (AZ/AZ0)    KZ'").append("\n");
    second.append(u8"                 (M)    (M)                                (10-5KN*M2)               (10-6KN)       (10-4KN*M) (10-6KN)       (10-5KN*M2)").append("\n");
    second.append(u8"                                                                          Z-MS : NMS  W-LIST   ANGLE      L1      L2       Y0      Z0").append("\n");
    second.append(u8"                                                                                               (DEG)     (M)     (M)      (M)     (M)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXXXXXXXXXX            XX.XXX").append("\n");
        second.append(u8"        XX.XXX XX.XXX XX.XXX  MZQY  +-----+ +W XXXXXXXXXX X XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX) XXX.XXX (XXX.XX)").append("\n");
        second.append(u8"        XX.XXX XX.XXX XX.XXX  MYQZ  +-----+ +W              XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX)XXXX.XXX").append("\n");
        second.append(u8"                                               XXXXXXXXXX X XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX) XXX.XXX (XXX.XX)").append("\n");
        second.append(u8"                                                            XXX.XXX (XXX.XX)XXXX.XXX XXX.XXX (XXX.XX)XXXX.XXX").append("\n");
        second.append(u8"                                                                        Z-MS-I : XXX XXXXXXXX XXXX.XX XXX.XXX XXX.XXX  XXX.XXX XXX.XXX").append("\n");
        second.append(u8"                                                                        Z-MS-J : XXX XXXXXXXX XXXX.XX XXX.XXX XXX.XXX  XXX.XXX XXX.XXX").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  WALL **                                                          BETA'=GAMS*BETA").append("\n");
    second.append(u8"  SEQ.  FL     FRAME  GR1    GR2    STR  LIST").append("\n");
    second.append(u8"        J1     J2     J3     J4        L      H      T   OPEN    TYPE ROP  GAMS  GAMU  MODEL      E/P NMS NMS BETA  BETA' A/A0  I/I0  WG/WG0").append("\n");
    second.append(u8"                                      (M)    (M)    (M)                                                B   T").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX XXXXXX XXXXXX XXXXXX XXX XXXXXXXX").append("\n");
        second.append(u8"        XXXXXX-XXXXXX-XXXXXX-XXXXXX XX.XXX XX.XXX XX.XXX XXXXXXXX XX X.XXX X.XXX X.XXX XXXXXXXXXX  X  XX  XX  XX.XX XX.XX XX.XX XX.XX XXX.XX").append("\n");
        second.append(u8"                                                                                       XXXXXXXXXX  X  XX  XX  XX.XX XX.XX XX.XX XX.XX XXX.XX").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  Z-WALL **").append("\n");
    second.append(u8"        FL     GR1    GR2         LIST").append("\n");
    second.append(u8"        J1     J2     J3     J4        L      H      T   OPEN      ROP  MODEL      BETA  A/A0  I/I0").append("\n");
    second.append(u8"                                      (M)    (M)    (M)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX XXXXXX XXXXXX     XXXXXXXX").append("\n");
        second.append(u8"        XXXXXX-XXXXXX-XXXXXX-XXXXXX XX.XXX XX.XXX XX.XXX XXXXXXXX X.XXX XXXXXXXXXX XX.XX XX.XX XX.XX").append("\n");
        second.append(u8"                                                                        XXXXXXXXXX XX.XX XX.XX XX.XX").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
