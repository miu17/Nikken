#include "section_slab_dialog.h"

#include <QtGui>
#include <QDebug>

#include "section_slab_filterdialog.h"
#include "section_slab_widget.h"

namespace post3dapp{
SectionSlabDialog::SectionSlabDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"スラブ 定義");
    createButtons(true);
    slab_widget = new SectionSlabWidget(SlabTable());
    childWidgets << slab_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets )
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(slab_widget);
    this->setLayout(vlay);

    filterDialog = new SectionSlabFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidgets, childWidgets){
        tableWidgets->setFilterColumns(filters[tableWidgets->getName()]);
        tableWidgets->setTableViewBorderColumns(tableWidgets->getSectionDataType(), filters[tableWidgets->getName()]);
    }
}

CustomTableModel* SectionSlabDialog::getTableModel() const{
    return slab_widget->getTableModel();
}

} // namespace post3dapp
