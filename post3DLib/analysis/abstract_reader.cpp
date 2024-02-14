#include "abstract_reader.h"

#include "define_draw_figure.h"
namespace post3dapp{
AbstractReader::AbstractReader()
{

}

QString AbstractReader::MakeGroup(const QString& line, const QString& lastDefinition, QHash<QString, QList<QString>>* group) const
{
    QString groupName = line.left(10).trimmed();
    if (groupName == "-")
        groupName = lastDefinition;
    QList<QString> tmp;
    for(int i = 1; i * 10 < line.length(); i++){
        QString name = line.mid(i * 10, 10).trimmed();
        if (name == QString{}) continue;
        tmp.append(name);
    }
    if (group->contains(groupName)){
        auto value = group->value(groupName);
        value.append(tmp);
        group->insert(groupName, value);
    }
    else{
        group->insert(groupName, tmp);
    }
    return groupName; // lastDefinition
}

QList<QString> AbstractReader::ParseNameType(const QString& line, const QHash<QString, QList<QString>>& group) const
{
    QList<QString> nameList;
    for (int i = 0; i * 10 < line.length(); i++){
        QString name = line.mid(i * 10, 10).trimmed();
        if (name == QString{}) continue;
        if (group.contains(name)){
            Q_FOREACH(auto value, group[name]){
                nameList.append(value);
            }
        }
        else{
            nameList.append(name);
        }
    }
    return nameList;
}

QList<QString> AbstractReader::ParseHyphenType(const QString &line, QString& lastDefinition, const QHash<QString, QList<QString>> &group) const
{
    QString stressName = line.left(10).trimmed();
    if (stressName == "-"){
        stressName = lastDefinition;
    }else{
        lastDefinition = stressName;
    }

    QList<QString> nameList;
    for(int i = 1; i * 10 < line.length(); i++){
        QString name = line.mid(i * 10, 10).trimmed();
        if (name == QString{}) continue;
        if (group.contains(name)){
            Q_FOREACH(auto value, group[name]){
                nameList.append(value);
            }
        }
        else{
            nameList.append(name);
        }
    }
    return nameList;

}

QString AbstractReader::ParseCcsType(const QString& line, const QString& lastDefinition, QHash<QString, QList<STRESS>>* combinationStress) const
{
    QString combinationStressName = line.left(10).trimmed();
    if (combinationStressName == "-")
        combinationStressName = lastDefinition;

    for(int i = 1; i * 20 - 10 < line.length(); i++){
        QString name = line.mid(i * 20 - 10, 10).trimmed();
        if (name == QString{}) continue;
        QString ratio = line.mid(i * 20, 10).trimmed();
        if (ratio == QString{}) continue;
        STRESS stress;
        stress.rst = name;
        stress.coef = ratio.toDouble();
        if (combinationStress->contains(combinationStressName)){
            auto value = combinationStress->value(combinationStressName);
            value.append(stress);
            combinationStress->insert(combinationStressName, value);
        }
        else{
            combinationStress->insert(combinationStressName, QList<STRESS>{stress});
        }
    }

    return combinationStressName;
}

QString AbstractReader::ParseCase(const QString& line) const{
    return line.mid(10, 10).trimmed();
}


}
