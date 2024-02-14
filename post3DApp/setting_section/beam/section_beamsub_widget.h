#ifndef SECTIONBEAMSUBWIDGET_H
#define SECTIONBEAMSUBWIDGET_H

#include "section_beam_widget.h"
namespace post3dapp{
class SectionBeamRCWidget : public SectionBeamWidget
{
    Q_OBJECT

public:

    SectionBeamRCWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionBeamWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void recalcDefaultValue() override;
    void reflectUnifiedData() override;
    QString getName() const override {return u8" ＲＣ断面";};
    QList<bool> getDefaultFilters() const override;

private:

};

class SectionBeamSWidget : public SectionBeamWidget
{
    Q_OBJECT

public:

    SectionBeamSWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionBeamWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }
    void reflectUnifiedData() override;
    QString getName() const override {return u8"  Ｓ断面";};
    QList<bool> getDefaultFilters() const override;

private:

    void createViews() override;

};

class SectionBeamSRCWidget : public SectionBeamWidget
{
    Q_OBJECT

public:

    SectionBeamSRCWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionBeamWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void recalcDefaultValue() override;
    void reflectUnifiedData() override;
    QString getName() const override {return u8"ＳＲＣ断面";};
    QList<bool> getDefaultFilters() const override;

private:

    void createViews() override;

};
} // namespace post3dapp

#endif

