#ifndef SECTIONBASELINEDRAW_H
#define SECTIONBASELINEDRAW_H

#include "section_linedraw.h"
namespace post3dapp{
class SectionBaseLineDraw : public SectionLineDraw
{
    Q_OBJECT
public:
    SectionBaseLineDraw(const SectionDataType &type, QWidget *parent = 0);

protected:
    void paintEvent( QPaintEvent * ) override;
    void drawOuterFrame(QPainter *painter) override;

private:
    void drawBasePlate(QPainter *painter);
};
} // namespace post3dapp
#endif // SECTIONBASELINEDRAW_H
