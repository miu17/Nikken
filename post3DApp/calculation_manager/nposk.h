#ifndef NPOSK_H
#define NPOSK_H

#include <QObject>

namespace CalculationMakerLib{
class CalculationMaker;
}

namespace post3dapp{
class AbstractWrite;
class CustomDrawGrScene;
class FIGURE_PROPERTY;
class OutputPaperSetting;
class Section01;
class Section02;
class Section03;
class Section04;
class Section05;
class Section06;
class Section07;
class Section08;
class Section09;
class Section10;
class Section11;
class WriteContents;
class WriteContents2;
class WriteCover;
class WriteEnd;
class WriteOrganize;
struct STRESS;
struct PAPERSETTING;

class Nposk : public QObject
{
    Q_OBJECT
public:
    Nposk(){};
    ~Nposk();
    bool parseAll(CalculationMakerLib::CalculationMaker*, const QStringList&);

    QString getResorce() const {return resorce.join("\n");}

    bool context(CalculationMakerLib::CalculationMaker*, const OutputPaperSetting&);
    QString common();
    bool check(const OutputPaperSetting&);
    static QMap<QString, QStringList> outGroupPlan;
    static QMap<QString, QStringList> outGroupElevation;
    static QMap<QString, QList<STRESS>> stressMap;
    static QMap<QString, FIGURE_PROPERTY> planOption;
    static QMap<QString, PAPERSETTING> planPaperSetting;
    static QMap<QString, FIGURE_PROPERTY> elevationOption;
    static QMap<QString, PAPERSETTING> elevationPaperSetting;
signals:

private:
    QString parseList(CalculationMakerLib::CalculationMaker*, QStringList);
    QMap<QString, AbstractWrite *> writeCommands;
    Section01 *section01 = nullptr;
    Section02 *section02 = nullptr;
    Section03 *section03 = nullptr;
    Section04 *section04 = nullptr;
    Section05 *section05 = nullptr;
    Section06 *section06 = nullptr;
    Section07 *section07 = nullptr;
    Section08 *section08 = nullptr;
    Section09 *section09 = nullptr;
    Section10 *section10 = nullptr;
    Section11 *section11 = nullptr;
    WriteContents *contents = nullptr;
    WriteContents2 *contents2 = nullptr;
    WriteCover *cover = nullptr;
    WriteEnd *end = nullptr;
    WriteOrganize *organize = nullptr;

    QStringList resorce;
};
} // namespace post3dapp

#endif // NPOSK_H
