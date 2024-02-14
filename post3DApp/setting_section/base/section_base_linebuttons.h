#ifndef SECTIONBASELINEBUTTONS_H
#define SECTIONBASELINEBUTTONS_H

#include "section_linebuttons.h"
namespace post3dapp{
class SectionBaseLineButtons : public SectionLineButtons
{
    Q_OBJECT
public:
    SectionBaseLineButtons(const SectionDataType &, const QList<bool> &, QWidget *parent = nullptr);

protected:
    void setStandardLineEdit() override;
};
} // namespace post3dapp
#endif // SECTIONBASELINEBUTTONS_H
