#ifndef SECTIONCOLWIDGET_H
#define SECTIONCOLWIDGET_H

#include "section_tablewidget.h"
namespace post3dapp{
class SectionColumnWidget : public SectionTableWidget
{
    Q_OBJECT

public:

    SectionColumnWidget( const SectionDataType &type, QWidget *parent = 0 );

    void replaceMatchingBaseName( const QString &str_a, const QString &str_b );

protected:

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};
} // namespace post3dapp
#endif
