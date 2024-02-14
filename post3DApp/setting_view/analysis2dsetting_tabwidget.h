#ifndef ANALYSIS2DSETTING_TABWIDGET_H
#define ANALYSIS2DSETTING_TABWIDGET_H

#include <QTabWidget>

namespace post3dapp{
class Analysis2DViewSettings;
class Analysis2DSettingGraphWidget;
class Analysis2DSettingModelWidget;
class Analysis2DSettingTotalWidget;

class Analysis2DSettingTabWidget : public QTabWidget
{
public:
    Analysis2DSettingTabWidget(QWidget *parent = 0);
    Analysis2DViewSettings makeSettings() const;
    void applySettings(const Analysis2DViewSettings&);

public slots:
    void initializeSettings();

private:
    Analysis2DSettingTotalWidget *totalWidget;
    Analysis2DSettingModelWidget *modelWidget;
    Analysis2DSettingGraphWidget *graphWidget;

};
} // namespace post3dapp

#endif // ANALYSIS2DSETTING_TABWIDGET_H
