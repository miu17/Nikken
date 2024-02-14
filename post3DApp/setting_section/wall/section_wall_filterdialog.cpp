#include "section_wall_filterdialog.h"
#include "define_sectiontable.h"
namespace post3dapp{
SectionWallFilterDialog::SectionWallFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    :SectionFilterDialog(tabelWidgets, parent)
{
    setWindowTitle(u8"表示設定　壁");
    createLayout();
}
} // namespace post3dapp
