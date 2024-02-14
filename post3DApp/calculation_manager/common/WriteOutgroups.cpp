#include "abstractwrite.h"
#include "nposk.h"
namespace post3dapp{
const QString WriteOutgroups::Command = u8"*OUTGROUPS";
const QString WriteOutgroups::Japanese = u8"通りグループ指定";

WriteOutgroups::WriteOutgroups(const QStringList& list):AbstractWrite(list){};

QString WriteOutgroups::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    Q_UNUSED(setting)
    for(int i = 1; i < list.count(); i++){
        QStringList grids;
        QString group;
        for (int j = 0; j < 8; j++){
            QString split = list.at(i).mid(j * 10, 10).trimmed();
            if (split == QString{}) continue;
            if (j == 0)
                group = split;
            else
                grids << split;
        }

        if (Nposk::outGroupElevation.contains(group)){
            auto old = Nposk::outGroupElevation.take(group);
            old.append(grids);
            Nposk::outGroupElevation.insert(group, old);
        }
        else{
            Nposk::outGroupElevation.insert(group, grids);
        }
    }

    for (int i = 0; i < Nposk::outGroupElevation.count(); i++){
        auto key = Nposk::outGroupElevation.keys().at(i);
        auto value = Nposk::outGroupElevation.values().at(i);
        maker->outGroups.insert(key, value);
    }

    return QString();
}
} // namespace post3dapp
