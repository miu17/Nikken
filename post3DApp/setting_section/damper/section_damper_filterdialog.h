#ifndef SECTIONDAMPERFILTERDIALOG_H
#define SECTIONDAMPERFILTERDIALOG_H

#include "section_filterdialog.h"
namespace post3dapp{
class SectionDamperFilterDialog : public SectionFilterDialog
{
    Q_OBJECT
public:
    SectionDamperFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);
};
} // namespace post3dapp
#endif // SECTIONDAMPERFILTERDIALOG_H
