#include "section_opening_filterdialog.h"
#include "define_sectiontable.h"
namespace post3dapp{
SectionOpeningFilterDialog::SectionOpeningFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    :SectionFilterDialog(tabelWidgets, parent)
{
    setWindowTitle(u8"表示設定　開口");
    createLayout();
}
} // namespace post3dapp

