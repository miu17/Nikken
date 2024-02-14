#ifndef SECTIONBASEFILTERDIALOG_H
#define SECTIONBASEFILTERDIALOG_H

#include "section_filterdialog.h"
namespace post3dapp{
class SectionBaseFilterDialog : public SectionFilterDialog
{
    Q_OBJECT
public:
    SectionBaseFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);
};
} // namespace post3dapp
#endif // SECTIONBASEFILTERDIALOG_H
