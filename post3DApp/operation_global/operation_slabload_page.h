#ifndef OPERATIONSLABLOADPAGE_H
#define OPERATIONSLABLOADPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationSlabLoadPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationSlabLoadPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *slabLoadStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
