#ifndef OPERATIONDAMPERPAGE_H
#define OPERATIONDAMPERPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationDamperPage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationDamperPage( QWidget *parent = 0 );

protected:

private slots:

    virtual void slotChangeAddEditLayout(int);

signals:

private:

};
} // namespace post3dapp
#endif
