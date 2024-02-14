#ifndef SECTIONDAMPERWIDGET_H
#define SECTIONDAMPERWIDGET_H

#include "section_tablewidget.h"
namespace post3dapp{
class SectionDamperWidget : public SectionTableWidget
{
    Q_OBJECT

public:

    SectionDamperWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionTableWidget( type, parent )
    {
        createModels();
        createViews();
        createWidgets();
    }

    void reflectUnifiedData() override;
    QList<bool> getDefaultFilters() const override;
protected :

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};
} // namespace post3dapp
#endif
