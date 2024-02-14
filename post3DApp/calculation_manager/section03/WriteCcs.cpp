#include "abstractwrite.h"
#include "nposk.h"
namespace post3dapp{
const QString WriteCcs::Command = u8"*CCS";
const QString WriteCcs::Japanese = u8"応力解析ケース";

WriteCcs::WriteCcs(const QStringList& list):AbstractWrite(list){};

QString WriteCcs::context(CalculationMakerLib::CalculationMaker *maker, const OutputPaperSetting& setting) const{
    QMap<QString, QList<STRESS>> stressMap;
    QString name;
    Q_FOREACH(QString text, list){
        if (text.startsWith(Command)){
            continue;
        }

        if (!text.startsWith("-")){
            name = text.left(10).trimmed();
        }

        QList<STRESS> stress;
        if (stressMap.contains(name)){
            stress = stressMap.value(name);
        }
        if (text.mid(10, 10).trimmed() == QString())
        {
            // error
        }else{
            STRESS s;
            s.rst = text.mid(10, 10).trimmed();
            s.coef = text.mid(20, 10).trimmed().toDouble();
            stress.append(s);
        }

        if (text.mid(30, 10).trimmed() != QString()){
            STRESS s;
            s.rst = text.mid(30, 10).trimmed();
            s.coef = text.mid(40, 10).trimmed().toDouble();
            stress.append(s);
        }
        stressMap.insert(name, stress);
    }

    Nposk::stressMap =stressMap;
    return QString();
}

} // namespace post3dapp


