#include "section_beam_dialog.h"

#include <QtGui>
#include <QDebug>

#include "section_beam_filterdialog.h"
#include "section_beamsub_widget.h"

namespace post3dapp{
SectionBeamDialog::SectionBeamDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"小梁 定義");
    createButtons(true);
    createTabWidget();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(tab_widget);
    this->setLayout(vlay);

    filterDialog = new SectionBeamFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidget, childWidgets){
        tableWidget->setFilterColumns(filters[tableWidget->getName()]);
        tableWidget->setTableViewBorderColumns(tableWidget->getSectionDataType(), filters[tableWidget->getName()]);
    }
}

void SectionBeamDialog::createTabWidget()
{
    tab_widget = new QTabWidget(this);
    rc_widget = new SectionBeamRCWidget(BeamRCTable(), tab_widget);
    s_widget = new SectionBeamSWidget(BeamSTable(), tab_widget);
    src_widget = new SectionBeamSRCWidget(BeamSRCTable(), tab_widget);
    childWidgets << rc_widget << s_widget << src_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ){
        tab_widget->addTab(wid, wid->getName());
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );
    }
    tab_widget->setIconSize(QSize(20, 20));
}
CustomTableModel* SectionBeamDialog::getRcTableModel() const{
    return rc_widget->getTableModel();
}
CustomTableModel* SectionBeamDialog::getSTableModel() const{
    return s_widget->getTableModel();
}
CustomTableModel* SectionBeamDialog::getSrcTableModel() const{
    return src_widget->getTableModel();
}

} // namespace post3dapp
