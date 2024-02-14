#include "analysis2dsetting_tabwidget.h"

#include "analysis2dsetting_graphwidget.h"
#include "analysis2dsetting_modelwidget.h"
#include "analysis2dsetting_totalwidget.h"
#include "define_analysis2dsettings.h"

namespace post3dapp{

Analysis2DSettingTabWidget::Analysis2DSettingTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    totalWidget = new Analysis2DSettingTotalWidget(u8"解析図(モデル図を除く)", "total");
    this->addTab(totalWidget, totalWidget->TabName);
    modelWidget = new Analysis2DSettingModelWidget(u8"モデル図", "model");
    this->addTab(modelWidget, modelWidget->TabName);
    graphWidget = new Analysis2DSettingGraphWidget(u8"グラフ", "graph");
    this->addTab(graphWidget, graphWidget->TabName);
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(pal);

}

void Analysis2DSettingTabWidget::initializeSettings(){
    this->totalWidget->initializeSettings();
    this->modelWidget->initializeSettings();
    this->graphWidget->initializeSettings();
}


Analysis2DViewSettings Analysis2DSettingTabWidget::makeSettings() const{
    Analysis2DViewSettings setting = Analysis2DViewSettings::getDefault();
    auto total = totalWidget->getSettings();
    foreach(auto key, total.keys()){
        setting.setSetting(static_cast<OUTFIGTYPE>(key), total.value(key));
    }
    auto model = modelWidget->getSettings();
    foreach(auto key, model.keys()){
        setting.setSetting(static_cast<OUTFIGTYPE>(key), model.value(key));
    }
    auto graph = graphWidget->getSettings();
    foreach(auto key, graph.keys()){
        setting.setSetting(static_cast<OUTFIGTYPE>(key), graph.value(key));
    }
    return setting;
}

void Analysis2DSettingTabWidget::applySettings(const Analysis2DViewSettings &settings)
{
    this->totalWidget->applySettings(settings);
    this->modelWidget->applySettings(settings);
    this->graphWidget->applySettings(settings);
}

} // namespace post3dapp
