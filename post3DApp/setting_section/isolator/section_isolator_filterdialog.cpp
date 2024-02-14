#include "section_isolator_filterdialog.h"
#include "define_sectiontable.h"
namespace post3dapp{
SectionIsolatorFilterDialog::SectionIsolatorFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    :SectionFilterDialog(tabelWidgets, parent)
{
    setWindowTitle(u8"免震要素 定義");
    createLayout();
}
} // namespace post3dapp
