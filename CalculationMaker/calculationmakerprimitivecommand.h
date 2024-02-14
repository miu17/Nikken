#ifndef CALCULATIONMAKERPRIMITIVECOMMAND_H
#define CALCULATIONMAKERPRIMITIVECOMMAND_H

#include "calculationmaker.h"
#include "calculationmakercommandinterface.h"

class CalculationMakerPrimitiveCommand : public CalculationMakerCommandInterface
{
public:
    CalculationMakerPrimitiveCommand(const QString &command);

    virtual void draw(QPainter *) = 0;
};

#endif // CALCULATIONMAKERPRIMITIVECOMMAND_H
