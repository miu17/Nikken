#ifndef SECTIONnISOLATORSUBWIDGET_H
#define SECTIONnISOLATORSUBWIDGET_H

#include "section_isolator_widget.h"
namespace post3dapp{
class SectionIsolatorSubWidget : public SectionIsolatorWidget
{
    Q_OBJECT

public:

    SectionIsolatorSubWidget( const SectionDataType &type, QWidget *parent = 0 )
        : SectionIsolatorWidget( type, parent )
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

