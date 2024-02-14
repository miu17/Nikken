#include "section_uwall_filterdialog.h"
#include "define_sectiontable.h"
namespace post3dapp{
SectionUWallFilterDialog::SectionUWallFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    :SectionFilterDialog(tabelWidgets, parent)
{
    setWindowTitle(u8"表示設定　地下外壁");
    createLayout();
}
} // namespace post3dapp
