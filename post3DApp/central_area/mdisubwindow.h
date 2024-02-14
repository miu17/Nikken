#ifndef MDISUBWINDOW_H
#define MDISUBWINDOW_H

#include <QMdiSubWindow>
#include <QUuid>
#include "define_unifieddata.h"

namespace post3dapp {
class InterfaceWidget;

class MdiSubWindow : public QMdiSubWindow
{
    Q_OBJECT

public:

    MdiSubWindow( const SUBWINSTATUS &, QWidget *parent = 0 );
    ~MdiSubWindow();

//    bool uploadID();

    void sendActiveStatusToChildWidget();
    SUBWINSTATUS subWindowStatus() const
    {
        return mySubWinStatus;
    }

public slots:

    void changeCurrentID(const QString &);

protected:

private:

    void setInterfaceWidget();
    const SUBWINSTATUS mySubWinStatus;
    QUuid uuid; // frame or floor
    InterfaceWidget *myInterfaceWidget;

    void resetTitle();
};
} // namespace post3dapp

#endif

