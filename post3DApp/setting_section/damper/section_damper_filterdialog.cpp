#include "section_damper_filterdialog.h"
#include "define_sectiontable.h"
namespace post3dapp{
SectionDamperFilterDialog::SectionDamperFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    :SectionFilterDialog(tabelWidgets, parent)
{
    setWindowTitle(u8"表示設定　制振要素");
    createLayout();
}
} // namespace post3dapp
