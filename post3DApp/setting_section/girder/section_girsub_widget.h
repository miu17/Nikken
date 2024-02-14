#ifndef SECTIONGIRSUBWIDGET_H
#define SECTIONGIRSUBWIDGET_H

#include "section_gir_widget.h"
namespace post3dapp{
class SectionGirderRCWidget : public SectionGirderWidget
{
    Q_OBJECT

public:

    SectionGirderRCWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionGirderWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void recalcDefaultValue() override;
    void reflectUnifiedData() override;
    QString getName() const override {return u8" ＲＣ断面";}
    QList<bool> getDefaultFilters() const override;
private:

};

class SectionGirderSWidget : public SectionGirderWidget
{
    Q_OBJECT

public:

    SectionGirderSWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionGirderWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void reflectUnifiedData() override;
    QString getName() const override {return u8"  Ｓ断面";}
    QList<bool> getDefaultFilters() const override;
private:

    void createViews() override;

};

class SectionGirderSRCWidget : public SectionGirderWidget
{
    Q_OBJECT

public:

    SectionGirderSRCWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionGirderWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void recalcDefaultValue() override;
    void reflectUnifiedData() override;
    QString getName() const override {return u8"ＳＲＣ断面";}
    QList<bool> getDefaultFilters() const override;
private:

    void createViews() override;

};
} // namespace post3dapp

#endif

