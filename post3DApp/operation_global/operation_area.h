#ifndef OPERATIONAREA_H
#define OPERATIONAREA_H

#include <QScrollArea>

namespace post3dapp{
class OperationElementWidget;
class OperationEditAllWidget;
class OperationChangeAllWidget;

class OperationArea : public QScrollArea
{
    Q_OBJECT

public:

    OperationArea( QWidget *parent = 0 );

protected:

private slots:

signals:

private:

    OperationElementWidget *elementWidget;
    OperationEditAllWidget *editAllWidget;
    OperationChangeAllWidget *changeAllWidget;

};
} // namespace post3dapp
#endif
