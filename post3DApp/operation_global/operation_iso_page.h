#ifndef OPERATIONISOPAGE_H
#define OPERATIONISOPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationIsoPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationIsoPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *isoStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
