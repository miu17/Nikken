#include "calculationmakercommandinterface.h"
namespace CalculationMakerLib{

CalculationMakerCommandInterface::CalculationMakerCommandInterface(const QString &command) :
    m_command(command)
{
}

CalculationMakerCommandInterface::~CalculationMakerCommandInterface()
{
}

QString CalculationMakerCommandInterface::command() const
{
    return m_command;
}

QString CalculationMakerCommandInterface::errorString() const
{
    return m_error;
}

void CalculationMakerCommandInterface::setError(const QString &message)
{
    m_error = message;
}
} // namespace CalculationMakerLib
