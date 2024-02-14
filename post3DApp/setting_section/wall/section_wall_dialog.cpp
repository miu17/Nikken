#include "section_wall_dialog.h"

#include <QtGui>
#include <QDebug>

#include "define_section_struct.h"
#include "section_wall_filterdialog.h"
#include "unified_inputdata.h"
#include "utility.h"

namespace post3dapp{
SectionWallDialog::SectionWallDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"壁 定義");

    createButtons(true);
    createTabWidget();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(tab_widget);
    this->setLayout(vlay);

    filterDialog = new SectionWallFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidget, childWidgets){
        tableWidget->setFilterColumns(filters[tableWidget->getName()]);
        tableWidget->setTableViewBorderColumns(reg_widget->getSectionDataType(), filters[tableWidget->getName()]);
    }
}

void SectionWallDialog::createTabWidget()
{
    tab_widget = new QTabWidget(this);
    reg_widget = new SectionWallRegularWidget(WallRegularTable(), tab_widget);
    steel_widget = new SectionWallSteelWidget(WallSteelTable(), tab_widget);
    childWidgets << reg_widget << steel_widget ;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ){
        tab_widget->addTab(wid, wid->getName());
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );
    }
    tab_widget->setIconSize(QSize(20, 20));
}



void SectionWallDialog::replaceMatchingWallName( const QString &str_a, const QString &str_b )
{
    reg_widget->replaceMatchingName(str_a, str_b);
}

void SectionWallDialog::replaceMatchingSWallName(const QString &str_a, const QString &str_b)
{
    steel_widget->replaceMatchingName(str_a, str_b);
}



CustomTableModel* SectionWallDialog::getRegularTableModel() const{
    return reg_widget->getTableModel();
}
CustomTableModel* SectionWallDialog::getSteelTableModel() const{
    return steel_widget->getTableModel();
}

} // namespace post3dapp
