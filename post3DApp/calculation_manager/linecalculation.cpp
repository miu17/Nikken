#include "linecalculation.h"
namespace post3dapp{

bool LineCalculation::MatchKeyword(CalculationStatus status, const QString& text){
    if (premiseStatus.count() > 0 && !premiseStatus.contains(status))
        return false;

    return text.startsWith(key);
}
} // namespace post3dapp
