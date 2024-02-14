#include "section_damper_dialog.h"

#include <QtGui>
#include <QDebug>

#include "section_damper_filterdialog.h"
#include "section_damper_widget.h"
namespace post3dapp{
SectionDamperDialog::SectionDamperDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"ダンパー要素 定義");

    createButtons(true);

    damper_widget = new SectionDamperWidget(DamperTable());
    childWidgets << damper_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets )
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(damper_widget);
    this->setLayout(vlay);

    filterDialog = new SectionDamperFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidgets, childWidgets){
        tableWidgets->setFilterColumns(filters[tableWidgets->getName()]);
        tableWidgets->setTableViewBorderColumns(tableWidgets->getSectionDataType(), filters[tableWidgets->getName()]);
    }
}

CustomTableModel* SectionDamperDialog::getTableModel() const{
    return damper_widget->getTableModel();
}

} // namespace post3dapp
