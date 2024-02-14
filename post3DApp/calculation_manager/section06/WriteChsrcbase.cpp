﻿#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteChsrcbase::Command = u8"*CHSRCBASE";
const QString WriteChsrcbase::Japanese = u8"SRC非埋込柱脚のせん断検討";

WriteChsrcbase::WriteChsrcbase(const QStringList& list):AbstractWrite(list){};

QString WriteChsrcbase::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
    line += u8"\\indentL{0.0}\n";
    line += u8"\\linespacing{0.1} \n";
    line += font(u8"ＭＳ 明朝", u8"6.0");
    /*
    auto second = AbstractText::CreateAndWrite(*UnifiedFixedData::getInstance(), list);
    QStringList osubh = second.second.split("\n");
    for (int i = 0; i < osubh.count(); i++){
        line += text(osubh.at(i));
    }
    */
    line += suffix();
    return line;

}
} // namespace post3dapp
