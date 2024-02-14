#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrMemUlt::write(const FixedData& fixedData) const{
    //PR608

    QString second;
    second.append(u8"■ XXXXXX FRAME  GIRDER **").append("\n");
    second.append(u8"■ XXXXXX FRAME  COLUMN **").append("\n");
    second.append(u8"  SEQ.  LIST                                                                                    +---------------- REFERENCE ----------------+").append("\n");
    second.append(u8"        FL     FRAME  GR1    GR2     AX.V+U  BY-ST.I BY-ST.J SY-STEP   MZF.I    MZF.J      QUY    M0F.I    M0J.I     QU0    MBJ.I      QB0").append("\n");
    second.append(u8"        J1     J2            STATE   AX.ULT   COEF.   COEF.   COEF.    MYF.I    MYF.J      QUZ    M0F.J    M0J.J     QM0    MBJ.J     STATE").append("\n");
    second.append(u8"                           (DUF.AX)((V+U)/UL)( DUF.I)( DUF.J)( DUF.S)(VM/VMU) (VM/VMU) ( BETQ )                  (QU0/QM0)         (QU0/QB0)").append("\n");
    second.append(u8"                                      (KN)                            (KN*M)   (KN*M)    (KN*M)  (KN*M)   (KN*M)           (KN*M)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXXXXXXXXXXXX").append("\n");
        second.append(u8"        XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX    XXXX    XXXX    XXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX").append("\n");
        second.append(u8"        XXXXXX-XXXXXX       +-----+ XXXXXXXX XX.XXXX XX.XXXX XX.XXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX XXXXXXXX +------+").append("\n");
        second.append(u8"                            (XXX.XX)(XXX.XX) (XXX.XX)(XXX.XX)(XXX.XX)(XXX.XX) (XXX.XX) (XXX.XX)                   (XXX.XX)    MYQZ  (XXX.XX)").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  BRACE  **").append("\n");
    second.append(u8"  SEQ.  LIST").append("\n");
    second.append(u8"        FL     FRAME  GR1    GR2     AX.V+U  AY-STEP").append("\n");
    second.append(u8"        J1     J2                    AX.ULT   COEF.").append("\n");
    second.append(u8"                                   ((V+U)/UL)(DUF.AX)").append("\n");
    second.append(u8"                                      (KN)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXXXXXXXXXXXX").append("\n");
        second.append(u8"        XXXXXX XXXXXX XXXXXX XXXXXX XXXXXXXX    XXXX").append("\n");
        second.append(u8"        XXXXXX-XXXXXX               XXXXXXXX XX.XXXX").append("\n");
        second.append(u8"                                    (XXX.XX) (XXX.XX)").append("\n");
    //}
    second.append(u8"■ XXXXXX FRAME  WALL   **").append("\n");
    second.append(u8"  SEQ.  LIST").append("\n");
    second.append(u8"        FL     FRAME  GR1    GR2     QC-STEP QS-STEP BY-ST.B BY-ST.T   QU").append("\n");
    second.append(u8"        J1     J2     J3     J4       COEF.   COEF.   COEF.   COEF.    QULT").append("\n");
    second.append(u8"                                                                    (QU/QULT)").append("\n");
    second.append(u8"                                                                       (KN)").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"(XXXXX) XXXXXXXX").append("\n");
        second.append(u8"        XXXXXX XXXXXX XXXXXX XXXXXX     XXXX    XXXX    XXXX    XXXX XXXXXXXX").append("\n");
        second.append(u8"        XXXXXX-XXXXXX-XXXXXX-XXXXXX  XX.XXXX XX.XXXX XX.XXXX XX.XXXX XXXXXXXX").append("\n");
        second.append(u8"                                                                     (XXX.XX)").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
