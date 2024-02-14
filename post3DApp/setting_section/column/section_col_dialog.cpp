#include <QtGui>
#include <QDebug>

#include "section_col_dialog.h"
#include "section_col_filterdialog.h"
namespace post3dapp{
SectionColumnDialog::SectionColumnDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"柱 定義");

    createButtons(true);
    createTabWidget();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(tab_widget);
    this->setLayout(vlay);

    filterDialog = new SectionColumnFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidgets, childWidgets){
        tableWidgets->setFilterColumns(filters[tableWidgets->getName()]);
        tableWidgets->setTableViewBorderColumns(tableWidgets->getSectionDataType(), filters[tableWidgets->getName()]);
    }
}

void SectionColumnDialog::createTabWidget()
{
    tab_widget = new QTabWidget(this);
    rc_widget = new SectionColumnRCWidget(ColumnRCTable(), tab_widget);
    s_widget = new SectionColumnSWidget(ColumnSTable(), tab_widget);
    src_widget = new SectionColumnSRCWidget(ColumnSRCTable(), tab_widget);
    cft_widget = new SectionColumnCFTWidget(ColumnCFTTable(), tab_widget);
    childWidgets << rc_widget << s_widget << src_widget << cft_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ){
        tab_widget->addTab(wid, wid->getName());
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );
    }
    tab_widget->setIconSize(QSize(20, 20));
}

void SectionColumnDialog::replaceMatchingBaseName( const QString &str_a,
                                                          const QString &str_b )
{
    s_widget->replaceMatchingBaseName(str_a, str_b);
    src_widget->replaceMatchingBaseName(str_a, str_b);
    cft_widget->replaceMatchingBaseName(str_a, str_b);
}

CustomTableModel* SectionColumnDialog::getRcTableModel() const{
    return rc_widget->getTableModel();
}
CustomTableModel* SectionColumnDialog::getSTableModel() const{
    return s_widget->getTableModel();
}
CustomTableModel* SectionColumnDialog::getSrcTableModel() const{
    return src_widget->getTableModel();
}
CustomTableModel* SectionColumnDialog::getCftTableModel() const{
    return cft_widget->getTableModel();
}
} // namespace post3dapp
