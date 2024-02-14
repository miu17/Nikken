#ifndef OPERATIONELEMENTWIDGET_H
#define OPERATIONELEMENTWIDGET_H

#include <QWidget>

class QStackedWidget;
class QComboBox;
class QButtonGroup;
namespace post3dapp{
class AbstractElementPage;

class OperationElementWidget : public QWidget
{
    Q_OBJECT

public:

    OperationElementWidget( QWidget *parent = 0 );

protected:

private slots:

    void slotChangeElementLayout(int);
    void slotChangeOperationMode(int);

signals:

private:

    QWidget *selectWidget;
    QComboBox *elementComb;
    QButtonGroup *elementOpMode;

    QStackedWidget *elementStackedWidget;
    QList<AbstractElementPage *> elementPageList;

    void createSelectArea();
    void createStackedPages();

};
} // namespace post3dapp
#endif
