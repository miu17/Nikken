﻿#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteStt::Command = u8"*STT";
const QString WriteStt::Japanese = u8"STT";

WriteStt::WriteStt(const QStringList& list):AbstractWrite(list){};

QString WriteStt::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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