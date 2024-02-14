#include "abstractwrite.h"
#include "nposk.h"
namespace post3dapp{
const QString WriteOutgroupp::Command = u8"*OUTGROUPP";
const QString WriteOutgroupp::Japanese = u8"階グループ指定";

WriteOutgroupp::WriteOutgroupp(const QStringList& list):AbstractWrite(list){};

QString WriteOutgroupp::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    Q_UNUSED(setting)
    for(int i = 1; i < list.count(); i++){
        QStringList floors;
        QString group;
        for (int j = 0; j < 8; j++){
            QString split = list.at(i).mid(j * 10, 10).trimmed();
            if (split == QString{}) continue;
            if (j == 0)
                group = split;
            else
                floors << split;
        }

        if (Nposk::outGroupPlan.contains(group)){
            auto old = Nposk::outGroupPlan.take(group);
            old.append(floors);
            Nposk::outGroupPlan.insert(group, old);
        }
        else{
            Nposk::outGroupPlan.insert(group, floors);
        }
    }

    for (int i = 0; i < Nposk::outGroupPlan.count(); i++){
        auto key = Nposk::outGroupPlan.keys().at(i);
        auto value = Nposk::outGroupPlan.values().at(i);
        maker->outGroups.insert(key, value);
    }

    return QString();
}
} // namespace post3dapp
