#ifndef OPERATIONMEMBERLOADPAGE_H
#define OPERATIONMEMBERLOADPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationMemberLoadPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationMemberLoadPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *memberLoadStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
