#include "custom_checkbutton.h"
namespace post3dapp{
void CustomCheckButton::initialize()
{
    setCheckable(true);
//    defaultBackColor=QColor(190,210,230);
    defaultBackColor = QColor(210, 230, 200);
    defaultTextColor = QColor(100, 100, 100);
//    checkedBackColor=QColor(255,127, 80);
//    checkedTextColor=QColor(100,  0,  0);
    checkedBackColor = QColor(182, 221, 132);
    checkedTextColor = QColor(  0, 50,  0);
    changeColor(false);
    connect(this, &QAbstractButton::toggled, this, &CustomCheckButton::changeColor);
}

void CustomCheckButton::changeColor( bool checked )
{
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Button, QBrush( checked ? checkedBackColor : defaultBackColor ));
    pal.setBrush(QPalette::ButtonText, QBrush( checked ? checkedTextColor : defaultTextColor ));
    this->setPalette(pal);
}
} // namespace post3dapp
