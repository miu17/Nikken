#ifndef TABLEINTERFACE_H
#define TABLEINTERFACE_H

#include "calculationmaker.h"
#include "calculationmakercommandinterface.h"

#ifdef CALCULATIONMAKER_LIB
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QPainter;
namespace CalculationMakerLib{

class CALCULATIONMAKER_CLASS_DLLSPEC TableInterface : public CalculationMakerCommandInterface
{
public:
    TableInterface(const QString &command);
    ~TableInterface();

    virtual int headerCount() = 0;
    virtual int tableCount() = 0;
    virtual void draw(QPainter *painter, const QPoint &topleft, qreal width, qreal rowheight, int start, int end) = 0;
};
} // namespace CalculationMakerLib

#endif // TABLEINTERFACE_H
