#ifndef CALCULATIONREADER_H
#define CALCULATIONREADER_H

namespace post3dapp{
enum class CalculationStatus;
class LineCalculation;

class CalculationReader
{
public:
    CalculationReader(){};
    static LineCalculation* CheckLineCondition(const QString&, CalculationStatus, bool*);
    static LineCalculation* MatchLineCondition(const QString&, CalculationStatus, bool*);
    static bool hasFormatError(const QString&);
private:
    static QList<LineCalculation*> conditions;
};
} // namespace post3dapp

#endif // CALCULATIONREADER_H
