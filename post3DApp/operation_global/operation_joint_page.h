#ifndef OPERATIONJOINTPAGE_H
#define OPERATIONJOINTPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationJointPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationJointPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *jointStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
