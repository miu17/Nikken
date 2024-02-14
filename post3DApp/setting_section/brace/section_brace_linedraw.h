#ifndef SECTIONBRACELINEDRAW_H
#define SECTIONBRACELINEDRAW_H
#include "section_linedraw.h"
namespace post3dapp{
class SectionBraceLineDraw : public SectionLineDraw
{
    Q_OBJECT
public:
    SectionBraceLineDraw(const SectionDataType &type, QWidget *parent = 0);
protected:
    void paintEvent( QPaintEvent * ) override;
    void drawOuterFrame(QPainter *painter) override;
};
} // namespace post3dapp
#endif // SECTIONBRACELINEDRAW_H
