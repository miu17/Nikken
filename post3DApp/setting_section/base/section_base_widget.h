#ifndef SECTIONBASEWIDGET_H
#define SECTIONBASEWIDGET_H

#include "section_tablewidget.h"
namespace post3dapp{
class SectionBaseWidget : public SectionTableWidget
{
    Q_OBJECT

public:

    SectionBaseWidget( const SectionDataType &type, QWidget *parent = 0 );

protected :

    void createModels() override;
    void createViews() override;
    void createWidgets() override;


};
} // namespace post3dapp
#endif
