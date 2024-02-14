#ifndef OPERATIONWALLPAGE_H
#define OPERATIONWALLPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationWallPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationWallPage( QWidget *parent = nullptr );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *wallStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
