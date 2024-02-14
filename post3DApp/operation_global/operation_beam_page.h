#ifndef OPERATIONBEAMPAGE_H
#define OPERATIONBEAMPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationBeamPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationBeamPage( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeStackLayout(int);

signals:

private:

    QStackedWidget *beamStackedWidget;
    void createStackedLayout();

};
} // namespace post3dapp
#endif
