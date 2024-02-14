#ifndef SECTIONBRACEWIDGET_H
#define SECTIONBRACEWIDGET_H

#include "section_tablewidget.h"
namespace post3dapp{
class SectionBraceWidget : public SectionTableWidget
{
    Q_OBJECT

public:

    SectionBraceWidget( const SectionDataType &type, QWidget *parent = 0 );

protected:

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};
} // namespace post3dapp
#endif
