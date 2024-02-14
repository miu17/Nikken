#ifndef CUSTOMCHECKBUTTON_H
#define CUSTOMCHECKBUTTON_H

#include <QPushButton>
namespace post3dapp{
class CustomCheckButton : public QPushButton
{
    Q_OBJECT

public:

    CustomCheckButton ( QWidget *parent = 0 )
        : QPushButton(parent)
    {
        initialize();
    }
    CustomCheckButton ( const QString &text, QWidget *parent = 0 )
        : QPushButton(text, parent)
    {
        initialize();
    }
    CustomCheckButton ( const QIcon &icon, const QString &text, QWidget *parent = 0 )
        : QPushButton(icon, text, parent)
    {
        initialize();
    }

private slots:

    void changeColor( bool checked );
private:

    void initialize();

    QColor defaultBackColor;
    QColor defaultTextColor;
    QColor checkedBackColor;
    QColor checkedTextColor;

};
} // namespace post3dapp
#endif
