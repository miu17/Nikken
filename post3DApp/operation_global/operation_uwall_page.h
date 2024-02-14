#ifndef OPERATIONUWALLPAGE_H
#define OPERATIONUWALLPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationUWallPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationUWallPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *uwallStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
