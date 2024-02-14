#ifndef SECTIONGIRDERLINEDRAW_H
#define SECTIONGIRDERLINEDRAW_H

#include "section_linedraw.h"
namespace post3dapp{
class SectionGirderLineDraw : public SectionLineDraw
{
    Q_OBJECT
public:
    SectionGirderLineDraw(const SectionDataType &type, QWidget *parent = 0);
protected:
    void paintEvent( QPaintEvent * ) override;
    void drawOuterFrame(QPainter *painter) override;
};
} // namespace post3dapp
#endif // SECTIONGIRDERLINEDRAW_H
