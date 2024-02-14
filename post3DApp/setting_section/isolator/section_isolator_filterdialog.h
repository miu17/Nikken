#ifndef SECTIONISOLATORFILTERDIALOG_H
#define SECTIONISOLATORFILTERDIALOG_H

#include "section_filterdialog.h"
namespace post3dapp{
class SectionIsolatorFilterDialog : public SectionFilterDialog
{
    Q_OBJECT
public:
    SectionIsolatorFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);
};
} // namespace post3dapp
#endif // SECTIONISOLATORFILTERDIALOG_H
