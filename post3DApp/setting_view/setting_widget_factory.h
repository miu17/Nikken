#ifndef SETTINGWIDGETFACTORY_H
#define SETTINGWIDGETFACTORY_H

#include <QObject>

namespace post3dapp{
class BaseSettingWidget;
struct SETTINGDIALOGSETTINGS;

class SettingWidgetFactory : public QObject
{
    Q_OBJECT

public:

    static BaseSettingWidget* createSettingWidget(const SETTINGDIALOGSETTINGS& type);


public slots:

signals:


private:


    SettingWidgetFactory(){}


};
} // namespace post3dapp
#endif
