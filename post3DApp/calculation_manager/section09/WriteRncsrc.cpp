﻿#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteRncsrc::Command = u8"*RNCSRC";
const QString WriteRncsrc::Japanese = u8"部材種別（ SRC 柱 ）";

WriteRncsrc::WriteRncsrc(const QStringList& list):AbstractWrite(list){};

QString WriteRncsrc::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
