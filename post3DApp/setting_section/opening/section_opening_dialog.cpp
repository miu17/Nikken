#include "section_opening_dialog.h"

#include <QtGui>
#include <QDebug>

#include "section_opening_filterdialog.h"
#include "section_opening_widget.h"

namespace post3dapp{
SectionOpeningDialog::SectionOpeningDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"開口 定義");

    createButtons(false);

    opening_widget = new SectionOpeningWidget(OpeningTable());
    childWidgets << opening_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets )
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(opening_widget);
    this->setLayout(vlay);

    filterDialog = new SectionOpeningFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidget, childWidgets){
        tableWidget->setFilterColumns(filters[tableWidget->getName()]);
        tableWidget->setTableViewBorderColumns(tableWidget->getSectionDataType(), filters[tableWidget->getName()]);
    }
}

CustomTableModel* SectionOpeningDialog::getTableModel() const{
    return opening_widget->getTableModel();
}

} // namespace post3dapp
