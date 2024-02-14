#ifndef SECTIONISOLATORLINEDRAW_H
#define SECTIONISOLATORLINEDRAW_H
#include "section_linedraw.h"
namespace post3dapp{
class SectionIsolatorLineDraw : public SectionLineDraw
{
    Q_OBJECT
public:
    SectionIsolatorLineDraw(const SectionDataType &type, QWidget *parent = 0 );
protected:
    void paintEvent( QPaintEvent * ) override;
    void drawOuterFrame(QPainter *painter) override;
private:
    void drawIsolatorGraph(QPainter *painter);
    void drawOneIsolatorGraph(QPainter *, const QPointF&, qreal, qreal, const SpringParameter&) const;
    void drawLinearGraph(const SpringParameter&, const QPointF&, qreal, qreal, QPainter*) const;
    void drawBilinearGraph(const SpringParameter&, const QPointF&, qreal, qreal, QPainter*) const;
    void drawTrilinearGraph(const SpringParameter&, const QPointF&, qreal, qreal, QPainter*) const;
    void drawReversibleBilinearGraph(const SpringParameter&, const QPointF&, qreal, qreal, QPainter*) const;
    void drawReversibleTrilinearGraph(const SpringParameter&, const QPointF&, qreal, qreal, QPainter*) const;
    void drawTensileReversibleBilinearGraph(const SpringParameter&, const QPointF&, qreal, qreal, QPainter*) const;
    void drawTensileReversibleTrilinearGraph(const SpringParameter&, const QPointF&, qreal, qreal, QPainter*) const;
    void drawOriginOrientedTrilinearGraph(const SpringParameter&, const QPointF&, qreal, qreal, QPainter*) const;
    static void drawReversibleBilinearGraph(qreal, qreal, qreal, qreal, const QPointF&, qreal, qreal, qreal, QPainter*);
    static void drawReversibleTrilinearGraph(qreal, qreal, qreal, qreal, qreal, qreal, qreal, const QPointF&, qreal, qreal, qreal,QPainter*);
};
} // namespace post3dapp
#endif // SECTIONISOLATORLINEDRAW_H
