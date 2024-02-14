#include "vhline.h"
namespace post3dapp{
VFLine::VFLine(const QColor& color, QWidget *parent)
    :QFrame(parent), myColor(color)
{
    setFrameStyle(QFrame::VLine | QFrame::Raised);
    updateColor();
}

void VFLine::setColor(const QColor& color){
    myColor = color;
    updateColor();
}

void VFLine::updateColor()
{
    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(myColor));
    pal.setBrush(QPalette::Button, QBrush(myColor));
    setPalette(pal);
}

HFLine::HFLine(const QColor &color, QWidget *parent)
    : VFLine(color, parent)
{
    setFrameStyle(QFrame::HLine | QFrame::Raised);
    updateColor();
}

} // namespace post3dapp
