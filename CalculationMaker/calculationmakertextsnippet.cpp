
#include "calculationmakersnippet.h"
#include "calculationmaker.h"
#include "calculationmakercontext.h"

#include <QDateTime>
namespace CalculationMakerLib{

CalculationMakerSnippet::CalculationMakerSnippet(const QString &command) :
    CalculationMakerCommandInterface(command)
{
}

CalculationMakerPgSnippet::CalculationMakerPgSnippet() :
    CalculationMakerSnippet(u8"\\pg")
{
}

QString CalculationMakerPgSnippet::expandText(CalculationMaker *maker, const QString &arg)
{
    if (!arg.isEmpty()) {
        setError("Invalid argument");
    }
    return QString::number(maker->context()->currentPage());
}

CalculationMakerDateSnippet::CalculationMakerDateSnippet() :
    CalculationMakerSnippet(u8"\\date")
{
}

QString CalculationMakerDateSnippet::expandText(CalculationMaker *maker, const QString &arg)
{
    Q_UNUSED(maker);
    return QDateTime::currentDateTime().toString(arg);
}

CalculationMakerProjectNameSnippet::CalculationMakerProjectNameSnippet() :
    CalculationMakerSnippet(u8"\\projectname")
{
}

QString CalculationMakerProjectNameSnippet::expandText(CalculationMaker *maker, const QString &arg)
{
    if (!arg.isEmpty()) {
        setError("Invalid argument");
    }
    return maker->projectName;
}


CalculationMakerStructureTypeSnippet::CalculationMakerStructureTypeSnippet() :
    CalculationMakerSnippet(u8"\\structuretype")
{
}

QString CalculationMakerStructureTypeSnippet::expandText(CalculationMaker *maker, const QString &arg)
{
    if (!arg.isEmpty()) {
        setError("Invalid argument");
    }
    return maker->structureType;
}
} // namespace CalculationMakerLib
