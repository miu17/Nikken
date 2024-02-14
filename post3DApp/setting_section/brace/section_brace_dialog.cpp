#include "section_brace_dialog.h"

#include <QtGui>
#include <QDebug>

#include "section_brace_filterdialog.h"
#include "section_bracesub_widget.h"

namespace post3dapp{
SectionBraceDialog::SectionBraceDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"ブレース 定義");
    createButtons(true);
    s_widget = new SectionBraceSubWidget(BraceSTable());
    childWidgets << s_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ){
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );
    }

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(s_widget);
    this->setLayout(vlay);

    filterDialog = new SectionBraceFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidget, childWidgets){
        tableWidget->setFilterColumns(filters[tableWidget->getName()]);
        tableWidget->setTableViewBorderColumns(tableWidget->getSectionDataType(), filters[tableWidget->getName()]);
    }
}

CustomTableModel* SectionBraceDialog::getTableModel() const{
    return s_widget->getTableModel();
}

} // namespace post3dapp
