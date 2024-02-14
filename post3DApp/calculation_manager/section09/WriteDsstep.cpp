#include "abstractwrite.h"
#include "abstracttext.h"
#include "fixed_data.h"
namespace post3dapp{

const QString WriteDsstep::Command = u8"*DSSTEP";
const QString WriteDsstep::Japanese = u8"構造特性係数Ds算定のための荷重増分解析の経過";

WriteDsstep::WriteDsstep(const QStringList& list):AbstractWrite(list){};

QString WriteDsstep::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
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
