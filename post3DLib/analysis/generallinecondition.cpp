#include "generallinecondition.h"

namespace post3dapp {

QString GeneralLineCondition::write() const{
    QString txt;
    txt.append(QString::number(number).rightJustified(10));
    if (isChoice){
        txt.append(QString::number(selectedNumber).rightJustified(10));
        txt.append(QString().rightJustified(10));
    }else{
        txt.append(QString::number(factor).rightJustified(20));
    }
    txt.append(keyword.leftJustified(10));
    txt.append(comment).append(" ");
    Q_FOREACH(auto choice, choices){
        txt.append(choice).append(" ");
    }
    txt.append("\n");
    return txt;
}

QSharedPointer<post3dapp::GeneralLineCondition> post3dapp::GeneralLineCondition::Create(const QString &str)
{
    QStringList strlist = str.split(",");
    int num = strlist.at(0).toInt();
    if (strlist.at(4) == QString()){
        return QSharedPointer<GeneralLineCondition>(new GeneralLineCondition(
                           num,
                           strlist.at(2),
                           false,
                           0,
                           strlist.at(1).toDouble(),
                           strlist.at(3)));
    }else{
        QList<QString> list;
        for (int i = 4; i < strlist.count(); i++){
            if (strlist.at(i).trimmed() != QString())
                list.append(strlist.at(i).trimmed());
        }
        return QSharedPointer<GeneralLineCondition>(new GeneralLineCondition(
                           num,
                           strlist.at(2),
                           true,
                           strlist.at(1).toInt(),
                           0,
                           strlist.at(3),
                           list));
    }
}

} // namespace post3dapp
