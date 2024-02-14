#include "tableinterface.h"
namespace CalculationMakerLib{

TableInterface::TableInterface(const QString &name) :
    CalculationMakerCommandInterface(name)
{
}

TableInterface::~TableInterface()
{
}
} // namespace CalculationMakerLib
