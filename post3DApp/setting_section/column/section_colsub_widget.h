#ifndef SECTIONCOLSUBWIDGET_H
#define SECTIONCOLSUBWIDGET_H

#include "section_col_widget.h"
namespace post3dapp{
class SectionColumnRCWidget : public SectionColumnWidget
{
    Q_OBJECT

public:

    SectionColumnRCWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionColumnWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void recalcDefaultValue() override;
    void reflectUnifiedData() override;
    QString getName() const override { return u8" ＲＣ断面";}
    QList<bool> getDefaultFilters() const override;

private:

};

class SectionColumnSWidget : public SectionColumnWidget
{
    Q_OBJECT

public:

    SectionColumnSWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionColumnWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void reflectUnifiedData() override;
    QString getName() const override { return u8"  Ｓ断面";}
    QList<bool> getDefaultFilters() const override;
private:

    void createViews() override;

};

class SectionColumnSRCWidget : public SectionColumnWidget
{
    Q_OBJECT

public:

    SectionColumnSRCWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionColumnWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void recalcDefaultValue() override;
    void reflectUnifiedData() override;
    QString getName() const override { return u8"ＳＲＣ断面";}
    QList<bool> getDefaultFilters() const override;

private:

    void createViews() override;

};

class SectionColumnCFTWidget : public SectionColumnWidget
{
    Q_OBJECT

public:

    SectionColumnCFTWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionColumnWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }
    void reflectUnifiedData() override;
    QString getName() const override { return u8"ＣＦＴ断面";}
    QList<bool> getDefaultFilters() const override;

private:

    void createViews() override;

};
} // namespace post3dapp
#endif

