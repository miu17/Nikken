#ifndef SECTIONBRACEFILTERDIALOG_H
#define SECTIONBRACEFILTERDIALOG_H

#include "section_filterdialog.h"
namespace post3dapp{
class SectionBraceFilterDialog : public SectionFilterDialog
{
    Q_OBJECT
public:
    SectionBraceFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);
};
} // namespace post3dapp
#endif // SECTIONBRACEFILTERDIALOG_H
