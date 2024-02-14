#ifndef OPERATIONSLABPAGE_H
#define OPERATIONSLABPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationSlabPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationSlabPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *slabStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
