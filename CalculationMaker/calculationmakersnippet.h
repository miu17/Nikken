#ifndef CALCULATIONMAKERSNIPPET_H
#define CALCULATIONMAKERSNIPPET_H

#include "calculationmakercommandinterface.h"
namespace CalculationMakerLib{
class CalculationMaker;

class CalculationMakerSnippet : public CalculationMakerCommandInterface
{
public:
    CalculationMakerSnippet(const QString &command);
    virtual QString expandText(CalculationMaker *maker, const QString &arg) = 0;
};

class CalculationMakerPgSnippet : public CalculationMakerSnippet
{
public:
    CalculationMakerPgSnippet();
public:
    QString expandText(CalculationMaker *maker, const QString &arg);
};

class CalculationMakerDateSnippet : public CalculationMakerSnippet
{
public:
    CalculationMakerDateSnippet();
public:
    QString expandText(CalculationMaker *maker, const QString &arg);
};

class CalculationMakerProjectNameSnippet : public CalculationMakerSnippet
{
public:
    CalculationMakerProjectNameSnippet();
public:
    QString expandText(CalculationMaker *maker, const QString &arg);
};

class CalculationMakerStructureTypeSnippet : public CalculationMakerSnippet
{
public:
    CalculationMakerStructureTypeSnippet();
public:
    QString expandText(CalculationMaker *maker, const QString &arg);
};
} // namespace CalculationMakerLib

#endif // CALCULATIONMAKERSNIPPET_H
