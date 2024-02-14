#ifndef OPERATIONBRACEPAGE_H
#define OPERATIONBRACEPAGE_H

#include "operation_widget_parts.h"

namespace post3dapp{
class OperationBracePage : public AbstractElementPage
{
    Q_OBJECT

public:

    OperationBracePage( QWidget *parent = 0 );

protected:

private slots:

    virtual void slotChangeAddEditLayout(int);

signals:

private:

};
} // namespace post3dapp
#endif
