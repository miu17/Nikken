#ifndef OPERATIONGIRDERPAGE_H
#define OPERATIONGIRDERPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationGirderPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationGirderPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *girderStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
