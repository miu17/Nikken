#include "section_uwall_dialog.h"

#include <QtGui>
#include <QDebug>

#include "define_section_struct.h"
#include "section_uwall_filterdialog.h"
#include "unified_inputdata.h"
#include "utility.h"

namespace post3dapp{
SectionUWallDialog::SectionUWallDialog(QWidget *parent )
    : SectionCustomDialog( parent )
{
    initializeWindowTitle(u8"地下外壁 定義");

    createButtons(true);

    ug_widget = new SectionUnderGroundWallWidget(WallUnderGroundTable());
    childWidgets << ug_widget;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ){
        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );
    }
//    createTabWidget();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(ug_widget);
    this->setLayout(vlay);

    filterDialog = new SectionUWallFilterDialog(childWidgets, this);
    QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
    Q_FOREACH(SectionTableWidget* tableWidget, childWidgets){
        tableWidget->setFilterColumns(filters[tableWidget->getName()]);
        tableWidget->setTableViewBorderColumns(ug_widget->getSectionDataType(), filters[tableWidget->getName()]);
    }
}

//void SectionUWallDialog::createTabWidget()
//{
//    tab_widget = new QTabWidget(this);
//    ug_widget = new SectionUnderGroundWallWidget(WallUnderGroundTable(), tab_widget);
//    childWidgets << ug_widget;
//    Q_FOREACH ( SectionTableWidget *wid, childWidgets ){
//        tab_widget->addTab(wid, wid->getName());
//        connect ( wid, &SectionTableWidget::sectionItemChanged, this, &SectionCustomDialog::slotModifiedStateOn );
//    }
//    tab_widget->setIconSize(QSize(20, 20));
//}
void SectionUWallDialog::replaceMatchingUWallName( const QString &str_a, const QString &str_b )
{
    ug_widget->replaceMatchingName(str_a, str_b);
}
CustomTableModel* SectionUWallDialog::getUnderGroundTableModel() const{
    return ug_widget->getTableModel();
}
} // namespace post3dapp
