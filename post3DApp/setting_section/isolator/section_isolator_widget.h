#ifndef SECTIONnISOLATORWIDGET_H
#define SECTIONnISOLATORWIDGET_H

#include "section_tablewidget.h"
namespace post3dapp{
class SectionIsolatorWidget : public SectionTableWidget
{
    Q_OBJECT

public:

    SectionIsolatorWidget( const SectionDataType &type, QWidget *parent = 0 );

protected:

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};
} // namespace post3dapp
#endif
