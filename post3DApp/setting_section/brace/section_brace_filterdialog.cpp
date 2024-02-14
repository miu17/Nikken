#include "section_brace_filterdialog.h"
#include "define_sectiontable.h"
namespace post3dapp{
SectionBraceFilterDialog::SectionBraceFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    :SectionFilterDialog(tabelWidgets, parent)
{
    setWindowTitle(u8"表示設定　ブレース");
    createLayout();
}
} // namespace post3dapp
