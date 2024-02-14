#ifndef SECTIONCOLUMNFILTERDIALOG_H
#define SECTIONCOLUMNFILTERDIALOG_H

#include "section_filterdialog.h"
namespace post3dapp{
class SectionColumnFilterDialog : public SectionFilterDialog
{
    Q_OBJECT
public:
    SectionColumnFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);

protected:
    QVBoxLayout* createSwitchLayout() override;
};
} // namespace post3dapp
#endif // SECTIONCOLUMNFILTERDIALOG_H
