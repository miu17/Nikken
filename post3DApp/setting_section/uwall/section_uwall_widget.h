#ifndef SECTIONUWALLWIDGET_H
#define SECTIONUWALLWIDGET_H

#include "section_tablewidget.h"
namespace post3dapp{
class SectionUnderGroundWallWidget : public SectionTableWidget
{
    Q_OBJECT

public:

    SectionUnderGroundWallWidget( const SectionDataType &type, QWidget *parent = 0 );

    void reflectUnifiedData() override;
    QString getName() const override {return u8"地下外壁断面";}
    QList<bool> getDefaultFilters() const override;

protected :

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};
} // namespace post3dapp
#endif
