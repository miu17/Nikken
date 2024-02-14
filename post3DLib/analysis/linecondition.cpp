#include "linecondition.h"

#include <QTextStream>

#include "fixed_data.h"
#include "manage_jointdata.h"
#include "textdataunit.h"
#include "unified_inputdata.h"

namespace post3dapp {

bool LineCondition::MatchKeyword(ANALYSISSTEP step, LINESTATUSTYPE status, const QString& text){
    if (premiseTypes.count() > 0 && !premiseTypes.contains(status))
        return false;

    if (secondKeywordIndex == -1)
        if (text.startsWith("=====") || text.startsWith("/=====")){
            return text.startsWith(key);
        }else{
            return step == myStep && text.startsWith(key);
        }
    else{
        if (step != myStep || !text.startsWith(key))
            return false;

        if (dataUnits.count() <= secondKeywordIndex)
            return false;

        auto unit = dataUnits.at(secondKeywordIndex);
        QString txt = text.mid(unit.startCol -1, unit.endCol - unit.startCol + 1);
        return unit.match(txt);
    }
}

QString TwoPointLineCondition::convert(const QString& txt, QSet<QString>* jointSet, QSet<QString>* memberSet, QString* errorMessage) const{
    Q_UNUSED(errorMessage);
    // 範囲指定は加減算しないので、そのまま書き込む
    const QString floor1 = txt.mid(8, 6).trimmed();
    const QString gridA1 = txt.mid(14, 6).trimmed();
    const QString gridB1 = txt.mid(20, 6).trimmed();
    auto uuid1 = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor1, gridA1, gridB1);
    auto jointId1 = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid1);

    const QString floor2 = txt.mid(26, 6).trimmed();
    const QString gridA2 = txt.mid(32, 6).trimmed();
    const QString gridB2 = txt.mid(38, 6).trimmed();
    auto uuid2 = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor2, gridA2, gridB2);
    auto jointId2 = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid2);

    jointSet->clear();
    memberSet->clear();
    return jointId1.rightJustified(10) + jointId2.rightJustified(10);
}

QString DkLineCondition::convert(const QString& txt, QSet<QString>* jointSet, QSet<QString>* memberSet, QString* errorMessage) const{
    Q_UNUSED(jointSet);
    Q_UNUSED(memberSet);
    Q_UNUSED(errorMessage);
    // 範囲指定は加減算しないので、そのまま書き込む
    const QString floor1 = txt.mid(8, 6).trimmed();
    QString floor2 = txt.mid(20, 6).trimmed();
    if (floor2 == QString())
        floor2 = floor1;
    const QString grid1 = txt.mid(26, 6).trimmed();
    const QString grid2 = txt.mid(32, 6).trimmed();
    auto uuid = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor2, grid1, grid2);
    QString joint = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid);
    return QString("        ") + floor1.leftJustified(6) + joint.rightJustified(6);
}

QString D3LineCondition::convert(const QString& txt, QSet<QString>* jointSet, QSet<QString>* memberSet, QString* errorMessage) const{
    Q_UNUSED(jointSet);
    Q_UNUSED(memberSet);
    Q_UNUSED(errorMessage);
    const QString floor1 = txt.mid(2, 6).trimmed();
    const QString gridA1 = txt.mid(8, 6).trimmed();
    const QString girdB1 = txt.mid(14, 6).trimmed();
    const QString direction1 = txt.mid(20, 1);
    auto uuid1 = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor1, gridA1, girdB1);
    QString joint1 = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid1);

    const QString floor2 = txt.mid(22, 6).trimmed();
    const QString gridA2 = txt.mid(28, 6).trimmed();
    const QString gridB2 = txt.mid(34, 6).trimmed();
    const QString direction2 = txt.mid(40, 1);
    const QString factor2 = txt.mid(41, 9);
    auto uuid2 = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor2, gridA2, gridB2);
    QString joint2 = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid2);

    QString ret = u8"D3" + joint1.rightJustified(6) + direction1 + joint2.rightJustified(6) + direction2 + factor2;

    const QString floor3 = txt.mid(52, 6).trimmed();
    const QString gridA3 = txt.mid(58, 6).trimmed();
    const QString gridB3 = txt.mid(64, 6).trimmed();
    const QString direction3 = txt.mid(70, 1);
    const QString factor3 = txt.mid(71, 9);
    if (floor3 != QString()){
        auto uuid3 = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor3, gridA3, gridB3);
        QString joint3 = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid3);
        ret += joint3.rightJustified(6) + direction3 + factor3;
    }

    return ret;
}

QString D3LineCondition2::convert(const QString& txt, QSet<QString>* jointSet, QSet<QString>* memberSet, QString* errorMessage) const
{
    Q_UNUSED(jointSet);
    Q_UNUSED(memberSet);
    Q_UNUSED(errorMessage);
    const QString floor2 = txt.mid(22, 6).trimmed();
    const QString gridA2 = txt.mid(28, 6).trimmed();
    const QString gridB2 = txt.mid(34, 6).trimmed();
    const QString direction2 = txt.mid(40, 1);
    const QString factor2 = txt.mid(41, 9);
    auto uuid2 = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor2, gridA2, gridB2);
    QString joint2 = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid2);

    QString ret = QString("").leftJustified(10) + joint2.rightJustified(6) + direction2 + factor2;

    const QString floor3 = txt.mid(52, 6).trimmed();
    const QString gridA3 = txt.mid(58, 6).trimmed();
    const QString gridB3 = txt.mid(64, 6).trimmed();
    const QString direction3 = txt.mid(70, 1);
    const QString factor3 = txt.mid(71, 9);
    if (floor3 != QString()){
        auto uuid3 = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor3, gridA3, gridB3);
        QString joint3 = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid3);
        ret += joint3.rightJustified(6) + direction3 + factor3;
    }

    return ret;
}

QString D3TLineCondition::convert(const QString& txt, QSet<QString>* jointSet, QSet<QString>* memberSet, QString* errorMessage) const
{
    Q_UNUSED(jointSet);
    Q_UNUSED(memberSet);
    Q_UNUSED(errorMessage);
    // 後で差し引くためにjointSetにjointを追加
    const QString floor = txt.mid(22, 6).trimmed();
    const QString gridA = txt.mid(28, 6).trimmed();
    const QString gridB = txt.mid(34, 6).trimmed();
    const QString direction = txt.mid(40, 1);
    const QString factor = u8"1.0";
    auto uuid = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor, gridA, gridB);
    QString joint = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid);
    jointSet->insert(joint);
    return QString(u8"D3").leftJustified(10) + joint.rightJustified(6) + direction + factor;
}

QString MaLineCondition::convert(const QString& txt, QSet<QString>* jointSet, QSet<QString>* memberSet, QString* errorMessage) const{
    Q_UNUSED(jointSet);
    Q_UNUSED(memberSet);
    Q_UNUSED(errorMessage);
    const QString number = txt.mid(2, 6).trimmed();
    const QString floor = txt.mid(8, 6).trimmed();
    const QString gridA = txt.mid(14, 6).trimmed();
    const QString gridB = txt.mid(20, 6).trimmed();
    QUuid uuid;
    if (number != QString()){
        int no = number.toInt();
        uuid = UnifiedFixedData::getInstance()->getInputData()->jointListOfAll().at(no)->getUuid();
    }
    else{
        uuid = UnifiedFixedData::getInstance()->getInputData()->getJointId(floor, gridA, gridB);
    }
    QString joint = UnifiedFixedData::getInstance()->getAnalysisJointName(uuid);
    return u8"MA" + joint.rightJustified(6);
}

} // namespace post3dapp
