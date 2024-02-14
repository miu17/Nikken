#ifndef SECTIONWALLWIDGET_H
#define SECTIONWALLWIDGET_H

#include "section_tablewidget.h"
namespace post3dapp{
class SectionWallWidget : public SectionTableWidget
{
    Q_OBJECT

public:

    SectionWallWidget( const SectionDataType &type, QWidget *parent = 0 );

protected :

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};
} // namespace post3dapp
#endif
