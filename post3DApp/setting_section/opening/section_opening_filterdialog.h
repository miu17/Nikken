#ifndef SECTIONOPENINGFILTERDIALOG_H
#define SECTIONOPENINGFILTERDIALOG_H

#include "section_filterdialog.h"
namespace post3dapp{
class SectionOpeningFilterDialog : public SectionFilterDialog
{
    Q_OBJECT
public:
    SectionOpeningFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);
};
} // namespace post3dapp
#endif // SECTIONOPENINGFILTERDIALOG_H
