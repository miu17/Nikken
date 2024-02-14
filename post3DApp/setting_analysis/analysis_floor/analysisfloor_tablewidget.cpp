#include "analysisfloor_tablewidget.h"

#include <QtGui>
#include <QAction>
#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QSpinBox>
#include <QUndoStack>

#include "analysisfloor_tablemodel.h"
#include "customtable_commands.h"
#include "customtable_view.h"
#include "refercombobox_delegate.h"
#include "sortfilter_proxymodel.h"

namespace post3dapp{
AnalysisFloorTableWidget::AnalysisFloorTableWidget( const CustomTableDataType &type,
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
    mySpinBox->setValue(0);
    undoStack->clear();

}

void AnalysisFloorTableWidget::createActions()
{
    CustomTableWidget::createActions();

    deleteAction->setEnabled(false);
    insertAction->setEnabled(false);
    upAction->setEnabled(false);
    downAction->setEnabled(false);
    deleteAction->setVisible(false);
    insertAction->setVisible(false);
    upAction->setVisible(false);
    downAction->setVisible(false);
}

void AnalysisFloorTableWidget::createModels()
{
    myTableModel = new AnalysisFloorTableModel(myType, this);
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel = new QItemSelectionModel(myProxyModel);

    connect ( myTableModel, &CustomTableModel::rowCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxValue );
    connect ( myTableModel, &CustomTableModel::dataCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxMinimum );
    connect ( myTableModel, &QAbstractItemModel::dataChanged,
              this, &AnalysisFloorTableWidget::slotManageTableData );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &CustomTableWidget::slotEditItem );
}


void AnalysisFloorTableWidget::createViews()
{

    myTableView = new CustomTableView;
    myTableView->setFirstColumn(myType.getTaggedColumn("FLOOR"));
    myTableView->setLastColumn(myType.getTaggedColumn("PARENTFLOOR"));

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth(myType.getTaggedColumn("NO"), 60);
    myTableView->setColumnWidth(myType.getTaggedColumn("FLOOR"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("LEVEL"), 100);
    myTableView->setColumnWidth(myType.getTaggedColumn("CHILDFLOOR"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("PARENTFLOOR"), 80);

    QHash<int, QStringList> lmlist = myType.getLimitedStrings();
    Q_FOREACH (int col, lmlist.keys()) {
        ComboBoxDelegate *deleg = new ComboBoxDelegate(lmlist.value(col));
        myTableView->setItemDelegateForColumn(col, deleg);
    }

    QHash<int, DATATYPE> refcol = myType.getReferenceColumns();
    Q_FOREACH(int cc, refcol.keys()){
        ReferenceComboBoxDelegate *deleg = new ReferenceComboBoxDelegate(refcol.value(cc));
        myTableView->setItemDelegateForColumn(cc, deleg);
    }

    connect ( myTableView, QOverload<>::of(&CustomTableView::delKeyClicked), this, &AnalysisFloorTableWidget::del );
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &CustomTableWidget::slotRowActionEnable );
}

void AnalysisFloorTableWidget::createWidgets()
{
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

//    vlay->addLayout(topLayout,0);
    vlay->addWidget(myTableView, 1);
    this->setLayout(vlay);
}


void AnalysisFloorTableWidget::modelUpload()
{
    myTableModel->setEditOn(false);
    myTableModel->keepCurrentData();
    undoStack->clear();
}
/*
void AnalysisFloorTableWidget::modelEdit()
{
    myTableModel->setEditOn(true);
}
*/
void AnalysisFloorTableWidget::modelRewind()
{
    myTableModel->backPreviousData();
    myTableModel->setEditOn(false);
    undoStack->clear();
    emit floorItemChanged();
}

void AnalysisFloorTableWidget::paste()
{
    QModelIndex current_idx = myTableView->currentIndex();
    QString str = QApplication::clipboard()->text();
    QUndoCommand *pasteCommand = new PasteCommand(myProxyModel, current_idx, str);
    undoStack->push(pasteCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }
}

void AnalysisFloorTableWidget::del()
{
    if (!mySelectionModel->hasSelection()) return;
    QModelIndexList idxlist = mySelectionModel->selectedIndexes();

    QUndoCommand *deleteCommand = new DeleteCommand(myProxyModel, idxlist);
    undoStack->push(deleteCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }
}

void AnalysisFloorTableWidget::slotManageTableData(const QModelIndex &, const QModelIndex &idx)
{
//    qDebug() << "slotManageTableData";
    if ( idx.column() > 0 ) emit floorItemChanged();
}

bool AnalysisFloorTableWidget::checkAnalysisFloor() const
{
    QStringList namelist;
    for ( int i = 0; i < myTableModel->rowCount(); i++ ) {
        QList<QVariant> varlist = myTableModel->getRowData(i);
        QString str = varlist.at( myType.getTaggedColumn("FLOOR") ).toString();
        if ( namelist.contains(str) ) return false;
        namelist.append(str);
    }
    return true;
}


void AnalysisFloorTableWidget::updateFloorTable()
{
    QHash<QString, QList<QVariant> > org_data;
    for ( int i = 0; i < myTableModel->rowCount(); i++ ) {
        QList<QVariant> varlist = myTableModel->getRowData(i);
        QString name = varlist.at( myType.getTaggedColumn("FLOOR") ).toString();
        org_data.insert( name, varlist );
    }
    for ( int i = 0; i < myTableModel->rowCount(); i++ ) {
        myTableModel->removeRow(i);
    }

    QStringList fllist = UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPFLOOR);
    for ( int i = 0; i < fllist.count(); i++ ) {
        QList<QVariant> varlist;
        QString name = fllist.at(i);
        if ( org_data.contains(name) ) {
            varlist = org_data.value(name);
        } else {
            varlist << QVariant() << QVariant(name) << QVariant() << QVariant() << QVariant();
        }
        myTableModel->setRowDataDirect(i, varlist, false);
    }
    mySpinBox->setValue(fllist.count());
    undoStack->clear();

    myTableModel->keepCurrentData();
}

QList<FLOORVALUES> AnalysisFloorTableWidget::getAnalysisFloorList() const
{
    QList<FLOORVALUES> floorList;

    for (int i = 0; i < myTableModel->rowCount() ; i++) {
        QString floorName = myTableModel->getData(i, myType.getTaggedColumn("FLOOR")).toString();
        double ajustedLevel = myTableModel->getData(i, myType.getTaggedColumn("LEVEL")).toDouble();
        bool isChildFloor = myTableModel->getData(i, myType.getTaggedColumn("CHILDFLOOR")).toBool();
        QUuid parentId = myTableModel->getData(i, myType.getTaggedColumn("PARENTFLOOR")).toString();
        if (!isChildFloor) parentId = QUuid();

        FLOORVALUES fv;
        fv.UUID = UnifiedInputData::getInInstance()->nameToID(floorName, DATATYPE::TPFLOOR);
        fv.adjustedLevel = ajustedLevel;
        fv.isChildFloor = isChildFloor;
        fv.parentID = parentId;

        floorList.append(fv);
    }

    return floorList;
}

void AnalysisFloorTableWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    QList<FLOORVALUES> floors = UnifiedInputData::getInInstance()->getFloorList();
    Q_FOREACH(FLOORVALUES fl, floors){
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(fl.floorName));
        data.append(QVariant(fl.analysisName));
        data.append(QVariant(QString::number(fl.adjustedLevel)));
        data.append(QVariant(QString::number(fl.isChildFloor)));
        data.append(QVariant(fl.parentID.toString()));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    undoStack->clear();
}
} // namespace post3dapp
