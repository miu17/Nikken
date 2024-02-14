#ifndef SECTIONBEAMWIDGET_H
#define SECTIONBEAMWIDGET_H

#include "section_tablewidget.h"
namespace post3dapp{
class SectionBeamWidget : public SectionTableWidget
{
    Q_OBJECT

public:

    SectionBeamWidget( const SectionDataType &type, QWidget* parent=0 );

protected :

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};
} // namespace post3dapp
#endif
