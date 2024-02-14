#include "section_base_filterdialog.h"
#include "define_sectiontable.h"
namespace post3dapp{
SectionBaseFilterDialog::SectionBaseFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    :SectionFilterDialog(tabelWidgets, parent)
{
    setWindowTitle(u8"表示設定　柱脚");
    createLayout();
}


} // namespace post3dapp
