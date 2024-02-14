#ifndef VHLINE_H
#define VHLINE_H

#include <QFrame>

namespace post3dapp{
class VFLine :  public QFrame
{

public:

    VFLine(const QColor &color = Qt::black, QWidget *parent = 0);
    void setColor(const QColor &color);

protected:
    QColor myColor;
    void updateColor();
};


class HFLine :  public VFLine
{

public:
    HFLine(const QColor &color = Qt::black, QWidget *parent = 0);
};

} // namespace post3dapp
#endif
