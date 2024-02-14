#ifndef SECTIONWALLSUBWIDGET_H
#define SECTIONWALLSUBWIDGET_H

#include "section_wall_widget.h"
namespace post3dapp{
class SectionWallRegularWidget : public SectionWallWidget
{
    Q_OBJECT

public:

    SectionWallRegularWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionWallWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void reflectUnifiedData() override;
    QString getName() const override {return u8"壁断面";}
    QList<bool> getDefaultFilters() const override;
private:

    void createViews() override;

};

class SectionWallSteelWidget : public SectionWallWidget{
    Q_OBJECT

public:

    SectionWallSteelWidget( const SectionDataType &type, QWidget *parent = 0)
        : SectionWallWidget(type, parent)
    {
        createModels();
        createViews();
        createWidgets();
    }

    void reflectUnifiedData() override;
    QString getName() const override {return u8"鋼板壁断面";}
    QList<bool> getDefaultFilters() const override;
};



} // namespace post3dapp

#endif

