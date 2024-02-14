#ifndef SECTIONBASESUBWIDGET_H
#define SECTIONBASESUBWIDGET_H

#include "section_base_widget.h"
namespace post3dapp{
class SectionBaseSWidget : public SectionBaseWidget
{
    Q_OBJECT

public:

    SectionBaseSWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionBaseWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void recalcDefaultValue() override;
    void reflectUnifiedData() override;
    QString getName() const override {return u8"S造露出型";}
    QList<bool> getDefaultFilters() const override;

private:

    void createViews() override;

};

class SectionBaseSRCWidget : public SectionBaseWidget
{
    Q_OBJECT

public:

    SectionBaseSRCWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionBaseWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    QString getName() const override {return u8"SRC造非埋込型";}
    void reflectUnifiedData() override;
    QList<bool> getDefaultFilters() const override;
private:

};

} // namespace post3dapp

#endif

