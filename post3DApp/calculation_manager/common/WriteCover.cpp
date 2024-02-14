#include "abstractwrite.h"
namespace post3dapp{
const QString WriteCover::Command = u8"*COVER";
const QString WriteCover::Japanese = u8"構造計算書（その１）表紙";

WriteCover::WriteCover(const QStringList& list):AbstractWrite(list){};

QString WriteCover::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const {
    Q_UNUSED(setting)
    Q_UNUSED(maker)
    QString line;
    line += begin();
    line += footer(u8"OFF");
    line += newPage();
    line += u8"\\vspace{40.0} \n";
    line += title(u8"構造計算書（その１）");
    line += u8"\\vspace{40.0} \n";
    line += u8"\\subtitle{\\projectname{}} \n";
    line += u8"\\linespacing{5.0}  # 次の\\beginまでlinespacingが5.0となる \n";
    line += u8"\\moverxy{30.0,180.0} \\indentL{0.0} \\indentR{100.0} \n";
    line += u8"\\text{建築設計事務所名：} \n";
    line += text(u8"（確認申請上の設計者）：");
    line += text(u8"担当者名：");
    line += text(u8"連絡先・電話番号：");
    line += u8"\\moverxy{80.0,180.0} \\indentL{60.0} \\indentR{0.0} \n";
    line += u8"\\alignment{LEFT} \n";
    line += text(u8"株式会社 日建設計");
    line += u8"\\includelinetext{TEXT01} \n";
    line += u8"\\includelinetext{TEXT02} \n";
    line += text(u8"TEL 03-5226-3030 FAX 03-5226-3042");
    line += u8"\\picture{180.0,260.0,20.0,20.0,\"D:\\Users\\03551.NIKKEN\\Documents\\GitHub\\CalculationManager\\Sample\\sakai\\qt-logo.png\"} \n";
    line += end();
    return line;
}

} // namespace post3dapp
