#ifndef SECTIONCOLUMNLINEDRAW_H
#define SECTIONCOLUMNLINEDRAW_H

#include "section_linedraw.h"
namespace post3dapp{
class SectionColumnLineDraw : public SectionLineDraw
{
    Q_OBJECT
public:
    SectionColumnLineDraw(const SectionDataType &type, QWidget *parent = 0);
protected:
    void paintEvent( QPaintEvent * ) override;
    void drawOuterFrame(QPainter *painter) override;
private:
    void drawRcColumnSection(QPainter *painter);
    void drawSteelColumnSection(QPainter *painter);
    void drawSrcSteelColumnSection(QPainter *painter);
    void drawSrcColumnSection(QPainter *painter);
    void drawCftColumnSection(QPainter *painter);
    void drawRcOneColumn(QPainter *painter, const LinePartsParameterSRC&, qreal, qreal) const;
    void drawSrcSteelOneColumn(QPainter *painter, const LinePartsParameterSRC&, qreal, qreal);
    void drawSteelOneColumn(QPainter *painter, const LinePartsParameterSRC&, qreal, qreal);
};
} // namespace post3dapp
#endif // SECTIONCOLUMNLINEDRAW_H
