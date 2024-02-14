#ifndef SECTIONGIRWIDGET_H
#define SECTIONGIRWIDGET_H

#include "section_tablewidget.h"
namespace post3dapp{
class SectionGirderWidget : public SectionTableWidget
{
    Q_OBJECT

public:

    SectionGirderWidget( const SectionDataType &type, QWidget *parent = 0 );

protected :

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};
} // namespace post3dapp
#endif
