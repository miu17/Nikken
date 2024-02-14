#ifndef TYPICALCOLORSTYLE_H
#define TYPICALCOLORSTYLE_H

#include <QProxyStyle>
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class POST3D_CLASS_DLLSPEC TypicalColorStyle : public QProxyStyle
{
public:
    TypicalColorStyle();

    virtual void drawControl(ControlElement element, const QStyleOption *option,
                             QPainter *painter, const QWidget *widget = 0) const;
};

} // namespace post3dapp
#endif
