#ifndef SECTION_WALL_FILTERDIALOG_H
#define SECTION_WALL_FILTERDIALOG_H

#include "section_filterdialog.h"
namespace post3dapp{
class SectionWallFilterDialog : public SectionFilterDialog
{
    Q_OBJECT

public:
    SectionWallFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);
};
} // namespace post3dapp
#endif // SECTION_WALL_FILTERDIALOG_H
