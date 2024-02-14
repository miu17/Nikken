#ifndef OPERATIONCOLUMNPAGE_H
#define OPERATIONCOLUMNPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationColumnPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationColumnPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *columnStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
