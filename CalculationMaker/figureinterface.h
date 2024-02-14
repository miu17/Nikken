#ifndef FIGUREINTERFACE_H
#define FIGUREINTERFACE_H

#include "calculationmaker.h"
#include "calculationmakercommandinterface.h"

#ifdef CALCULATIONMAKER_LIB
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_IMPORT
#endif


class QPainter;
namespace CalculationMakerLib{

class CALCULATIONMAKER_CLASS_DLLSPEC FigureInterface : public CalculationMakerCommandInterface
{
public:
    FigureInterface(const QString &command);
    ~FigureInterface();

    virtual void draw(QPainter *painter, const QRect &rect, const QString &floor, post3dapp::PAPERDIVIDE separete, int index) = 0;
    virtual bool isPlan() = 0;
};
} // namespace CalculationMakerLib

#endif // FIGUREINTERFACE_H
