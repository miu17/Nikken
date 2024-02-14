#ifndef SECTIONBEAMLINEDRAW_H
#define SECTIONBEAMLINEDRAW_H

#include "section_linedraw.h"
namespace post3dapp{
class SectionBeamLineDraw : public SectionLineDraw
{
    Q_OBJECT
public:
    SectionBeamLineDraw(const SectionDataType &type, QWidget *parent = 0);
protected:
    void paintEvent( QPaintEvent * ) override;
    void drawOuterFrame(QPainter *painter) override;
};
} // namespace post3dapp
#endif // SECTIONBEAMLINEDRAW_H
