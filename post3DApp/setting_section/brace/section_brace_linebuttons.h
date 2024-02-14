#ifndef SECTIONBRACELINEBUTTONS_H
#define SECTIONBRACELINEBUTTONS_H

#include "section_linebuttons.h"
namespace post3dapp{
class SectionBraceLineButtons : public SectionLineButtons
{
    Q_OBJECT
public:
    SectionBraceLineButtons( const SectionDataType &, const QList<bool> &, QWidget *parent = nullptr );

protected:
    void setStandardLineEdit() override;
};
} // namespace post3dapp
#endif // SECTIONBRACELINEBUTTONS_H
