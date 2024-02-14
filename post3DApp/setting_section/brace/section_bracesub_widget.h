#ifndef SECTIONBRACESUBWIDGET_H
#define SECTIONBRACESUBWIDGET_H

#include "section_brace_widget.h"
namespace post3dapp{
class SectionBraceSubWidget : public SectionBraceWidget
{
    Q_OBJECT

public:

    SectionBraceSubWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionBraceWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void reflectUnifiedData() override;
    QList<bool> getDefaultFilters() const override;
private:

    void createViews() override;

};
} // namespace post3dapp

#endif

