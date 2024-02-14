#ifndef MENUPUSHBUTTON_H
#define MENUPUSHBUTTON_H

#include <QWidget>

class QToolButton;
namespace post3dapp{
class ExpandLabel;

enum class CBSTATUS { CB_NORMAL = 0, CB_ACTIVE, CB_DEFINE, CB_COLOR,
                CB_DISABLE, CB_PLAY, CB_PLAYDONE, CB_DEFAULT
              };

class MenuButton : public QWidget
{
    Q_OBJECT

public:

    MenuButton ( const QString &text, CBSTATUS status = CBSTATUS::CB_NORMAL, QWidget *parent = 0 );

public slots:

    void changeButtonColor( CBSTATUS status );
    void signalButtonClicked();

signals:

    void clicked();

private:

    void resetButtonIcon();

    QToolButton *toolButton;
    ExpandLabel *textLabel;

    CBSTATUS currentStatus;

};
} // namespace post3dapp
#endif
