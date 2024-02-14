#ifndef CALC_ESTIMATE_H
#define CALC_ESTIMATE_H

#include <QObject>

namespace post3dapp{
class CalculationEstimate : public QObject
{
    Q_OBJECT
public:
    CalculationEstimate();
    QString exec() const;
private:
    QString getDeadLoad() const;
    QString getDeadLoadDetail() const;
    QString getConcreteVolume() const;
    QString getShuttering() const;
    QString getSteel() const;
    QString getCovering() const;
    QString getJointLoadForAnalysis() const;
    QString getReinforcementByAnalysis() const;
    QString getReinforcementByInput() const;
};
} // namespace post3dapp
#endif // CALC_ESTIMATE_H
