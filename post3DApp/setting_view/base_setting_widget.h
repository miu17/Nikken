#ifndef BASESETTINGWIDGET_H
#define BASESETTINGWIDGET_H

#include <QWidget>
namespace post3dapp {

class BaseSettingWidget:public QWidget
{
    Q_OBJECT
public:
    BaseSettingWidget(QWidget* parent=nullptr);
};
}
#endif // BASESETTINGWIDGET_H
