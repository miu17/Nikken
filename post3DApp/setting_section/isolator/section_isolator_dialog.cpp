#include "section_isolator_dialog.h"

#include <QtGui>
#include <QDebug>

#include "section_isolator_filterdialog.h"
#include "section_isolatorsub_widget.h"

namespace post3dapp{
SectionIsolatorDialog::SectionIsolatorDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"免震要素 定義");
    createButtons(false);

    isol_widget = new SectionIsolatorSubWidget(IsolatorTable());
    childWidgets << isol_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets )
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(isol_widget);
    this->setLayout(vlay);

    filterDialog = new SectionIsolatorFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidget, childWidgets){
        tableWidget->setFilterColumns(filters[tableWidget->getName()]);
        tableWidget->setTableViewBorderColumns(tableWidget->getSectionDataType(), filters[tableWidget->getName()]);
    }
}

CustomTableModel* SectionIsolatorDialog::getTableModel() const{
    return isol_widget->getTableModel();
}

} // namespace post3dapp
