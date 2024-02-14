#ifndef OPERATIONJOINTLOADPAGE_H
#define OPERATIONJOINTLOADPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationJointLoadPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationJointLoadPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *jointLoadStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
