#ifndef SECTION_UWALL_FILTERDIALOG_H
#define SECTION_UWALL_FILTERDIALOG_H

#include "section_filterdialog.h"
namespace post3dapp{
class SectionUWallFilterDialog : public SectionFilterDialog
{
    Q_OBJECT

public:
    SectionUWallFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);
};
} // namespace post3dapp
#endif // SECTION_WALL_FILTERDIALOG_H
