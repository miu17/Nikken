#include "section_slab_filterdialog.h"
#include "define_sectiontable.h"
namespace post3dapp{
SectionSlabFilterDialog::SectionSlabFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    :SectionFilterDialog(tabelWidgets, parent)
{
    setWindowTitle(u8"表示設定　スラブ");
    createLayout();
}

} // namespace post3dapp
