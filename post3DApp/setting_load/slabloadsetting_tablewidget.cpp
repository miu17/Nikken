#include "slabloadsetting_tablewidget.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QUndoStack>

#include "customtable_model.h"
#include "customtable_view.h"
#include "sortfilter_proxymodel.h"
#include "unified_inputdata.h"

namespace post3dapp{
SlabLoadSettingTableWidget::SlabLoadSettingTableWidget( const CustomTableDataType &type,
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
//    setUndoOnOff(false);
}

void SlabLoadSettingTableWidget::createModels()
{
    localModel = new CustomTableModel(myType, this);
    myTableModel = localModel;
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel = new QItemSelectionModel(myProxyModel);

    connect ( myTableModel, &CustomTableModel::rowCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxValue );
    connect ( myTableModel, &CustomTableModel::dataCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxMinimum );
    connect ( myTableModel, &QAbstractItemModel::dataChanged,
              this, &SlabLoadSettingTableWidget::slotManageTableData );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &CustomTableWidget::slotEditItem );
}

void SlabLoadSettingTableWidget::createViews()
{
    myTableView = new CustomTableView(this);
    myTableView->setFirstColumn(myType.getTaggedColumn("NAME"));
    myTableView->setLastColumn(myType.getTaggedColumn("COMMENT"));

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth(myType.getTaggedColumn("NO"), 60);
    myTableView->setColumnWidth(myType.getTaggedColumn("NAME"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("DL"), 110);
    myTableView->setColumnWidth(myType.getTaggedColumn("LL0"), 110);
    myTableView->setColumnWidth(myType.getTaggedColumn("LL1"), 110);
    myTableView->setColumnWidth(myType.getTaggedColumn("LL2"), 110);
    myTableView->setColumnWidth(myType.getTaggedColumn("LL3"), 110);
    myTableView->setColumnWidth(myType.getTaggedColumn("LL4"), 110);
    myTableView->setColumnWidth(myType.getTaggedColumn("LLE"), 110);
    myTableView->setColumnWidth(myType.getTaggedColumn("COMMENT"), 150);

    connect ( myTableView, QOverload<>::of(&CustomTableView::delKeyClicked), this, &CustomTableWidget::del );
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &CustomTableWidget::slotRowActionEnable );
}

void SlabLoadSettingTableWidget::createWidgets()
{
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout, 0);
    vlay->addWidget(myTableView, 1);
    this->setLayout(vlay);
}

void SlabLoadSettingTableWidget::slotDeleteLine()
{
    CustomTableWidget::slotDeleteLine();
    emit sectionItemChanged();
}

void SlabLoadSettingTableWidget::modelUpload()
{
    localModel->keepCurrentData();
    undoStack->clear();
}

void SlabLoadSettingTableWidget::modelRewind()
{
    localModel->backPreviousData();
    undoStack->clear();
    emit sectionItemChanged();
}

void SlabLoadSettingTableWidget::slotManageTableData( const QModelIndex &idx_a,
                                                      const QModelIndex &idx_b)
{
    Q_UNUSED(idx_a);
    if ( idx_b.column() > 0 ) emit sectionItemChanged();
}

QStringList SlabLoadSettingTableWidget::checkOverlapNames() const
{
    return QStringList();
}

QList<SLABLOADVALUES> SlabLoadSettingTableWidget::getValueList() const
{
    QList<SLABLOADVALUES> sloadList;

    for ( int i = 0; i < myTableModel->rowCount() ; i++ ) {

        QVariant var1 = myTableModel->getData(i, myType.getTaggedColumn("NAME"));
        QVariant var2 = myTableModel->getData(i, myType.getTaggedColumn("DL"));
        QVariant var3 = myTableModel->getData(i, myType.getTaggedColumn("LL0"));
        QVariant var4 = myTableModel->getData(i, myType.getTaggedColumn("LL1"));
        QVariant var5 = myTableModel->getData(i, myType.getTaggedColumn("LL2"));
        QVariant var6 = myTableModel->getData(i, myType.getTaggedColumn("LL3"));
        QVariant var7 = myTableModel->getData(i, myType.getTaggedColumn("LL4"));
        QVariant var8 = myTableModel->getData(i, myType.getTaggedColumn("LLE"));
        QVariant var9 = myTableModel->getData(i, myType.getTaggedColumn("COMMENT"));

        if ( !var1.isValid() ) continue;

        sloadList.append(SLABLOADVALUES(var1.toString(), var9.toString(),
                                        var2.toDouble(), var3.toDouble(),
                                        var4.toDouble(), var5.toDouble(),
                                        var6.toDouble(), var7.toDouble(),
                                        var8.toDouble()));
    }
    return sloadList;
}

QStringList SlabLoadSettingTableWidget::getSectionNameList() const
{
    QList<QVariant> vlist = myTableModel->getColumnData(myType.getTaggedColumn("NAME"));
    QStringList namelist;
    Q_FOREACH (QVariant var, vlist) {
        QString str = var.toString();
        if ( !namelist.contains(str) && !str.isEmpty() ) namelist.append(str);
    }
    return namelist;
}

void SlabLoadSettingTableWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    QList<SLABLOADVALUES> list = UnifiedInputData::getInInstance()->getSlabLoadList();
    Q_FOREACH(SLABLOADVALUES ml, list){
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(ml.loadName));
        data.append(QVariant(QString::number(ml.sDL)));
        data.append(QVariant(QString::number(ml.sLL0)));
        data.append(QVariant(QString::number(ml.sLL1)));
        data.append(QVariant(QString::number(ml.sLL2)));
        data.append(QVariant(QString::number(ml.sLL3)));
        data.append(QVariant(QString::number(ml.sLL4)));
        data.append(QVariant(QString::number(ml.sLLE)));
        data.append(QVariant(ml.notes));
        temp.appendRowData(data);
        i++;
    }

    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    undoStack->clear();
}

bool SlabLoadSettingTableWidget::readTextFile(QTextStream &infile, bool flg)
{
    if ( ! myTableModel->readListTextData(infile, flg) ) return false;
    emit sectionItemChanged();
    undoStack->clear();
    return true;
}

} // namespace post3dapp
