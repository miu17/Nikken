#include "section_gir_dialog.h"

#include <QtGui>
#include <QDebug>

#include "section_gir_filterdialog.h"
#include "section_girsub_widget.h"

namespace post3dapp{
SectionGirderDialog::SectionGirderDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"大梁 定義");
    createButtons(true);
    createTabWidget();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(tab_widget);
    this->setLayout(vlay);

    filterDialog = new SectionGirderFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidget, childWidgets){
        tableWidget->setFilterColumns(filters[tableWidget->getName()]);
        tableWidget->setTableViewBorderColumns(rc_widget->getSectionDataType(), filters[tableWidget->getName()]);
    }
}

void SectionGirderDialog::createTabWidget()
{
    tab_widget = new QTabWidget(this);
    rc_widget = new SectionGirderRCWidget(GirderRCTable(), tab_widget);
    s_widget = new SectionGirderSWidget(GirderSTable(), tab_widget);
    src_widget = new SectionGirderSRCWidget(GirderSRCTable(), tab_widget);
    childWidgets << rc_widget << s_widget << src_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ){
        tab_widget->addTab(wid, wid->getName());
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );
    }
    tab_widget->setIconSize(QSize(20, 20));
}

CustomTableModel* SectionGirderDialog::getRcTableModel() const{
    return rc_widget->getTableModel();
}
CustomTableModel* SectionGirderDialog::getSTableModel() const{
    return s_widget->getTableModel();
}
CustomTableModel* SectionGirderDialog::getSrcTableModel() const{
    return src_widget->getTableModel();
}
} // namespace post3dapp
