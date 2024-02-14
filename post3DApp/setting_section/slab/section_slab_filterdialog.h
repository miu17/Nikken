#ifndef SECTIONSLABFILTERDIALOG_H
#define SECTIONSLABFILTERDIALOG_H

#include "section_filterdialog.h"
namespace post3dapp{
class SectionSlabFilterDialog : public SectionFilterDialog
{
    Q_OBJECT
public:
    SectionSlabFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);
};
} // namespace post3dapp
#endif // SECTIONSLABFILTERDIALOG_H
