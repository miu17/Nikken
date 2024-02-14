#include "drawtable.h"
namespace CalculationMakerLib{


void DrawTable::draw(QPainter *painter, const QPoint &topleft, qreal width, qreal rowheight, int start, int end)
{
    painter->setPen(Qt::blue);
    for (int i = 0; i < headerCount(); ++i) {
        int y = topleft.y() + rowheight * i;
        QRect r(topleft.x(), y, width, rowheight);
        painter->drawRect(r);
        painter->drawText(r, u8"HEADER");
    }
    painter->setPen(Qt::black);
    for (int i = start; i <= end; ++i) {
        int y = topleft.y() + rowheight * (i - start + headerCount());
        QRect r(topleft.x(), y, width, rowheight);
        painter->drawRect(r);
        QString str = QString::fromLocal8Bit("Item %1").arg(i);
        painter->drawText(r, str);
    }
}
} // namespace CalculationMakerLib
