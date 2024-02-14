#include <QtGui>

#include "section_base_dialog.h"
#include "section_base_filterdialog.h"
#include "section_basesub_widget.h"

namespace post3dapp{
SectionBaseDialog::SectionBaseDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"柱脚 定義");
    createButtons(true);
    createTabWidget();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(tab_widget);
    this->setLayout(vlay);

    filterDialog = new SectionBaseFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidget, childWidgets){
        tableWidget->setFilterColumns(filters[tableWidget->getName()]);
        tableWidget->setTableViewBorderColumns(tableWidget->getSectionDataType(), filters[tableWidget->getName()]);
    }
}

void SectionBaseDialog::createTabWidget()
{
    tab_widget = new QTabWidget(this);
    s_widget = new SectionBaseSWidget(BaseSTable(), tab_widget);
    src_widget = new SectionBaseSRCWidget(BaseSRCTable(), tab_widget);
    childWidgets << s_widget << src_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ){
        tab_widget->addTab(wid, wid->getName());
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );
    }
    tab_widget->setIconSize(QSize(20, 20));
}
CustomTableModel* SectionBaseDialog::getSTableModel() const{
    return s_widget->getTableModel();
}
CustomTableModel* SectionBaseDialog::getSrcTableModel() const{
    return src_widget->getTableModel();
}
} // namespace post3dapp
