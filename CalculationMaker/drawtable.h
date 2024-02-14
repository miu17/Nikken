#ifndef DRAWTABLE_H
#define DRAWTABLE_H

#include <QPainter>
#include "tableinterface.h"

#ifdef CALCULATIONMAKER_LIB
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace CalculationMakerLib{

class CALCULATIONMAKER_CLASS_DLLSPEC DrawTable : public TableInterface
{
public:
    DrawTable(const QString &command, int rows) :
        TableInterface(command), m_rows(rows) {}
    int tableCount() { return m_rows; }
    int headerCount() { return 1; }
    void draw(QPainter*, const QPoint&, qreal, qreal, int, int);

    private:
        int m_rows;
};
} // namespace CalculationMakerLib

#endif // DRAWTABLE_H
