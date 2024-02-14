#ifndef SECTIONBEAMFILTERDIALOG_H
#define SECTIONBEAMFILTERDIALOG_H

#include "section_filterdialog.h"

namespace post3dapp{
class SectionBeamFilterDialog : public SectionFilterDialog
{
    Q_OBJECT
public:
    SectionBeamFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);

protected:
    QVBoxLayout* createSwitchLayout() override;
};
} // namespace post3dapp
#endif // SECTIONBEAMFILTERDIALOG_H
