#include "abstracttext.h"
namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextFrame::write(const FixedData& fixedData) const{
    //PR066
    QString second;
    second.append(u8"                XXXXXX ( ANGLE = XXXX.XX )              XXXXXX ( ANGLE = XXXX.XX )              XXXXXX ( ANGLE = XXXX.XX )").append("\n");
    second.append(u8"FLOOR       JSEQ.    JNO. FL     GR1    GR2         JSEQ.    JNO. FL     GR1    GR2         JSEQ.    JNO. FL     GR1    GR2").append("\n");
    //Q_FOREACH(auto sec, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALL)){
        second.append(u8"XXXXXX      XXXXX  XXXXXX XXXXXX XXXXXX XXXXXX      XXXXX  XXXXXX XXXXXX XXXXXX XXXXXX      XXXXX  XXXXXX XXXXXX XXXXXX XXXXXX").append("\n");
    //}
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
