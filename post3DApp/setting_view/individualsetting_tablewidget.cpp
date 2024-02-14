#include "individualsetting_tablewidget.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QUndoStack>

#include "customtable_view.h"
#include "sortfilter_proxymodel.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_delegate.h"
#include "typicalcolor_table.h"
#include "unified_settings.h"

namespace post3dapp{
IndividualSettingTableWidget::IndividualSettingTableWidget( const CustomTableDataType &type,
                                                            QWidget *parent )
    : CustomTableWidget( type, parent )
{
    myMenu = new QMenu(this);
    undoStack = new QUndoStack(this);

    createActions();
    createButtons();
    createModels();
    createViews();
    createWidgets();
    setFilterWidgetsVisible(false);
}

void IndividualSettingTableWidget::createModels()
{
    myTableModel = new IndividualSettingTableModel(myType, this);
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel = new QItemSelectionModel(myProxyModel);

    connect ( myTableModel, &CustomTableModel::rowCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxValue );
    connect ( myTableModel, &CustomTableModel::dataCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxMinimum );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &CustomTableWidget::slotEditItem );
}

void IndividualSettingTableWidget::createViews()
{
    myTableView = new CustomTableView(this);
    myTableView->setFirstColumn(myType.getTaggedColumn("LABEL"));
    myTableView->setLastColumn(myType.getTaggedColumn("ALPHA"));
    myTableView->horizontalHeader()->setMinimumHeight(45);

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth(myType.getTaggedColumn("NO"), 60);
    myTableView->setColumnWidth(myType.getTaggedColumn("LABEL"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("LINECOLOR"), 150);
    myTableView->setColumnWidth(myType.getTaggedColumn("FILLCOLOR"), 150);
    myTableView->setColumnWidth(myType.getTaggedColumn("ALPHA"), 80);

    myTableView->setItemDelegateForColumn(myType.getTaggedColumn("LINECOLOR"), new TypicalColorDelegate);
    myTableView->setItemDelegateForColumn(myType.getTaggedColumn("FILLCOLOR"), new TypicalColorDelegate);

    connect ( myTableView, QOverload<>::of(&CustomTableView::delKeyClicked), this, &CustomTableWidget::del );
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &CustomTableWidget::slotRowActionEnable );
}

void IndividualSettingTableWidget::createWidgets()
{
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout, 0);
    vlay->addWidget(myTableView, 1);
    this->setLayout(vlay);
}

QMap<QString, COLORBRUSHSETTING> IndividualSettingTableWidget::getColorTableData() const
{
    QMap<QString, COLORBRUSHSETTING> colorHash;
    int count = myTableModel->rowCount();
    for ( int i = 0 ; i < count ; i++ ) {
        QList<QVariant> rowData = myTableModel->getRowData(i);
        QString name = rowData.at(myType.getTaggedColumn("LABEL")).toString();
        if ( !name.isEmpty() ) {
            QString str1 = rowData.at(myType.getTaggedColumn("LINECOLOR")).toString();
            QString str2 = rowData.at(myType.getTaggedColumn("FILLCOLOR")).toString();
            QString str3 = rowData.at(myType.getTaggedColumn("ALPHA")).toString();
            QColor col1 = ( str1.isEmpty() ) ? Qt::transparent :
                          TypicalColorTable::nameToColor(str1);
            QColor col2 = ( str2.isEmpty() ) ? Qt::transparent :
                          TypicalColorTable::nameToColor(str2);
            if ( col1 == Qt::transparent && col2 == Qt::transparent ) continue;
            if ( !str2.isEmpty() && !str3.isEmpty() ) {
                bool ok = true;
                int alpha = str3.toInt(&ok);
                if ( ok ) col2.setAlpha( alpha );
            }
            colorHash.insert(name, COLORBRUSHSETTING(col1, QBrush(col2)));
        }
    }
    return colorHash;
}

QList<QList<QVariant> > IndividualSettingTableWidget::getIndividualData() const
{
    QList<QList<QVariant> > colorList;
    for ( int i = 0 ; i < myTableModel->rowCount() ; i++ ) {
        QList<QVariant> rowData = myTableModel->getRowData(i);
        QString name = rowData.at(myType.getTaggedColumn("LABEL")).toString();
        if ( !name.isEmpty() ) colorList.append(rowData);
    }
    return colorList;
}

void IndividualSettingTableWidget::setIndividualData( const QList<QList<QVariant> > &dlist )
{
    myTableModel->initializeModel();
    if ( dlist.count() < 1 ) {
        undoStack->clear();
        return;
    }
    int count = dlist.count();
    myTableModel->setRowCount(count);
    for ( int i = 0 ; i < dlist.count() ; i++ ) {
        QList<QVariant> list = dlist.at(i);
        myTableModel->setRowDataDirect(i, list);
    }
    undoStack->clear();
}

QVariant IndividualSettingTableModel::data( const QModelIndex &index, int role ) const
{
    int row = index.row();
    int col = index.column();

    if ( col == myType.getTaggedColumn("LINECOLOR")
            || col == myType.getTaggedColumn("FILLCOLOR") ) {

        if (role == Qt::TextAlignmentRole) {
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        }

        if ( role == Qt::DecorationRole ) {
            return TypicalColorTable::nameToColor( allItems.at(row, col).toString() );
        }

    }

    return CustomTableModel::data(index, role);
}
} // namespace post3dapp
