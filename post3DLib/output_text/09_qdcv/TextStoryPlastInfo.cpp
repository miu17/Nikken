#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextStoryPlastInfo::write(const FixedData& fixedData) const{
    //PR601

    QString second;
    second.append(u8"XXX ZZ- ZZ  弾塑性解析結果の概要 ( STRESS-FILE : XXXXXXXXXX ) XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX").append("\n");
    second.append(u8"** INITIAL STRESS XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX").append("\n");
    second.append(u8"           XXXXXXXXXX  COEF.=XXXX.XX").append("\n");
    second.append(u8"").append("\n");
    second.append(u8"** FRAME BENDING YIELDING, BRACE AXIAL YIELDING STEP ( ANGLE = XXXX.XX )").append("\n");
    second.append(u8"").append("\n");
    second.append(u8"FLOOR  +---------FR.BEND.PLAST(c)----------+  +---------FR.BEND.YIELD(m)----------+  +---------BR.AXAL.YIELD(a)----------+").append("\n");
    second.append(u8"       STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE").append("\n");
    second.append(u8"              (KN)           (MM)                    (KN)           (MM)                    (KN)           (MM)").append("\n");
    second.append(u8"").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXX XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX").append("\n");
    //}
    second.append(u8"** FRAME SHEAR YIELDING STEP                         ( ANGLE = XXXX.XX )").append("\n");
    second.append(u8"").append("\n");
    second.append(u8"FLOOR  +---------FR.SHEAR.PLAST(h)---------+  +---------FR.SHEAR.YELD(s)----------+  +---------PANEL.YELD(p)-------------+").append("\n");
    second.append(u8"       STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE").append("\n");
    second.append(u8"              (KN)           (MM)                   (KN)            (MM)                   (KN)            (MM)").append("\n");
    second.append(u8"").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXX XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX").append("\n");
    //}
    second.append(u8"** WALL YIELDING STEP                                ( ANGLE = XXXX.XX )").append("\n");
    second.append(u8"").append("\n");
    second.append(u8"FLOOR  +---------WA.SHEAR.CRACK(H)---------+  +---------WA.SHEAR.YIELD(S)---------+  +----------WA.BEND.YIELD(M)---------+").append("\n");
    second.append(u8"       STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE").append("\n");
    second.append(u8"              (KN)           (MM)                   (KN)            (MM)                   (KN)            (MM)").append("\n");
    second.append(u8"").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXX XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX").append("\n");
    //}
    second.append(u8"** QU STEP, LAST STEP , INITIAL STIFFNESS            ( ANGLE = XXXX.XX )").append("\n");
    second.append(u8"").append("\n");
    second.append(u8"FLOOR  +---------------QU STEP-------------+  +--------------LAST STEP------------+  +-------------INITIAL STIFFNESS---------------+").append("\n");
    second.append(u8"       STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.    ANGLE   STEP  SHEAR   COEF.   DISP.   ANGLE     STIFF.").append("\n");
    second.append(u8"              (KN)           (MM)                    (KN)           (MM)                    (KN)           (MM)             (KN/CM)").append("\n");
    second.append(u8"").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXX XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX  XXXX XXXXXXX XX.XXXX XXXX.XX 1/XXXXXX XXXXXXXXX").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
