#ifndef COLORPUSHBUTTON_H
#define COLORPUSHBUTTON_H

#include <QPushButton>

enum CBSTATUS{ CB_UNDEF=0, CB_EDIT, CB_DEF, CB_DISABLE, CB_DEFAULT };

class MenuPushButton : public QPushButton
{
    Q_OBJECT

public:

    MenuPushButton ( QWidget * parent = 0 )
        : QPushButton(parent) { changeButtonColor(CB_UNDEF);  }
    MenuPushButton ( const QString & text, QWidget * parent = 0 )
        : QPushButton(text,parent) { changeButtonColor(CB_UNDEF); }
    MenuPushButton ( const QIcon & icon, const QString & text, QWidget * parent = 0 )
        : QPushButton(icon,text,parent) { changeButtonColor(CB_UNDEF); }
    MenuPushButton ( const QString & text, CBSTATUS status, QWidget * parent = 0 )
        : QPushButton(text,parent) { changeButtonColor(status); }

public slots:

    void changeButtonColor( CBSTATUS status ) {
        switch(status){
            case CB_UNDEF:   setButtonColor(QColor(153,204,153)); break;
            case CB_EDIT:    setButtonColor(QColor(153,255, 51)); break;
            case CB_DEF:     setButtonColor(QColor(204,204, 51)); break;
            case CB_DISABLE: setButtonColor(QColor(133,174,174)); break;
            case CB_DEFAULT: setButtonColor(QColor(153,204,204)); break;
            default: setButtonColor(QColor(153,204,153)); break;
        }
    }

private:

    void setButtonColor(const QColor& back_col, const QColor& text_col=Qt::black){
        QPalette pal = this->palette();
        pal.setBrush(QPalette::Button,QBrush(back_col));
        pal.setBrush(QPalette::ButtonText,QBrush(text_col));
        this->setPalette(pal);
    }

};

#endif
