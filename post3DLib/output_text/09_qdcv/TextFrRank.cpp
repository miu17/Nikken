#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrRank::write(const FixedData& fixedData) const{
    //PR609

    QString second;
    second.append(u8"■ XXXXXX FRAME  GIRDER **").append("\n");
    second.append(u8"  SEQ.  LIST                           ULT.SH                                                            +- RC -+ +---- S ----+ +--- SRC ---+").append("\n");
    second.append(u8"        FL     FRAME  GR1    GR2   IMD BEY.SH(UL/BEY) I STR RNK B   ALF  ALFB                             TAU/FC    B/F    D/W   N/N0 SM0/M0").append("\n");
    second.append(u8"        J1     J2     --- STATE ---    BRK.SH(UL/BRK) J STR RNK B   ALF  ALFB                             TAU/FC    B/F    D/W   N/N0 SM0/M0").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXXXXXXXXXXXX               XXXXXX").append("\n");
        second.append(u8"        XXXXXX XXXXXX XXXXXX XXXXXX XX XXXXXX(XXX.XX) I XXX (X) - XX.XX XX.XX                             XX.XXX XXX.XX XXX.XX XX.XXX XX.XXX").append("\n");
        second.append(u8"        XXXXXX-XXXXXX -- +-----+ --    XXXXXX(XXX.XX) J XXX (X) - XX.XX XX.XX                             XX.XXX XX.XXX XXX.XX XXX.XX XX.XXX").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  COLUMN **").append("\n");
    second.append(u8"  SEQ.  LIST                           ULT.SH                                         +----------- RC ----------+ +---- S ----+ +- SRC(CFT)-+").append("\n");
    second.append(u8"        FL     FRAME  GR1    GR2   IMD BEY.SH(UL/BEY) I STR RNK B   ALF  ALFB  Y STRES H0/D SGM0/FC PT(%) TAU/FC    B/F    D/W  N/N0 SM/M(RU)").append("\n");
    second.append(u8"        J1     J2     --- STATE ---    BRK.SH(UL/BRK)                          Z STRES H0/D         PT(%)                            SM/M(RU)").append("\n");
    second.append(u8"                                   QRK                J STR RNK B   ALF  ALFB  Y STRES H0/D SGM0/FC PT(%) TAU/FC    B/F    D/W  N/N0 SM/M(RU)").append("\n");
    second.append(u8"                                                                               Z STRES H0/D         PT(%)                            SM/M(RU)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXXXXXXXXXXXX               XXXXXX").append("\n");
        second.append(u8"        XXXXXX XXXXXX XXXXXX XXXXXX XX XXXXXX(XXX.XX) I XXX (X) - XX.XX XX.XX  Y  -- XX.XXX XX.XXX XX.XXX XX.XXX XXX.XX XXX.XX XX.XXX XX.XXX").append("\n");
        second.append(u8"        XXXXXX-XXXXXX -- +-----+ --    XXXXXX(XXX.XX)                          Z  -- XX.XXX        XX.XXX                             XX.XXX").append("\n");
        second.append(u8"                                  ( X )               J XXX (X) - XX.XX XX.XX  Y  -- XX.XXX XX.XXX XX.XXX XX.XXX XXX.XX XXX.XX XX.XXX XX.XXX").append("\n");
        second.append(u8"                                                                               Z  -- XX.XXX        XX.XXX                             XX.XXX").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  BRACE  **").append("\n");
    second.append(u8"  SEQ.  J1     J2     FL     FRAME  GR1    GR2    LIST              RANK   RMD").append("\n");
    second.append(u8"").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXXXXXXXXXX (   ) XXXX.X").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  WALL   **").append("\n");
    second.append(u8"  SEQ.      J1     J2     J3     J4 FL     FRAME  GR1    GR2    LIST      RANK     QL        Q       QU  QU/(QL+Q)").append("\n");
    second.append(u8"                                                                                  (KN)     (KN)     (KN)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX-XXXXXX-XXXXXX-XXXXXX XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX ( X ) XXXXXXXX XXXXXXXX XXXXXXXX XX.XXX").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  RC柱梁接合部パネルのせん断余裕度の検討 **").append("\n");
    second.append(u8"  SEQ.     NO. FL     GR1    GR2           形状  両側    σB     bj      D      1.1Qb      Qu      判定").append("\n");
    second.append(u8"                                                直交梁 (N/mm2)  (mm)   (mm)      (kN)     (kN)   (1.1Qb<=Qu?)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXX                               X    XX   XXXX.X  XXXXX. XXXXX. XXXXXXXX. XXXXXXXX.    XX").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
