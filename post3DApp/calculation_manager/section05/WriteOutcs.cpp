﻿#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{
const QString WriteOutcs::Command = u8"*OUTCS";
const QString WriteOutcs::Japanese = u8"応力解析ケース";

WriteOutcs::WriteOutcs(const QStringList& list):AbstractWrite(list){};

QString WriteOutcs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QString line;
    line += prefix();
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
