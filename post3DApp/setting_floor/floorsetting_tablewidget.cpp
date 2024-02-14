#include "floorsetting_tablewidget.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QUndoStack>

#include "combobox_delegate.h"
#include "customtable_model.h"
#include "customtable_view.h"
#include "define_floorsettingtable.h"
#include "refercombobox_delegate.h"
#include "sortfilter_proxymodel.h"
#include "utility.h"

namespace post3dapp{
FloorSettingTableWidget::FloorSettingTableWidget( const CustomTableDataType &type, QWidget *parent )
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

void FloorSettingTableWidget::createModels()
{
    myTableModel = new CustomTableModel(myType, this);
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel = new QItemSelectionModel(myProxyModel);

    connect ( myTableModel, &CustomTableModel::rowCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxValue );
    connect ( myTableModel, &CustomTableModel::dataCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxMinimum );
    connect ( myTableModel, &QAbstractItemModel::dataChanged,
              this, &FloorSettingTableWidget::slotManageTableData );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &CustomTableWidget::slotEditItem );
}

void FloorSettingTableWidget::createViews()
{
    myTableView = new CustomTableView(this);
    myTableView->setFirstColumn( myType.getTaggedColumn("NAME") );
    myTableView->setLastColumn( myType.getTaggedColumn("SLAB") );

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth( myType.getTaggedColumn("NO"), 60 );
    myTableView->setColumnWidth( myType.getTaggedColumn("NAME"), 80 );
    myTableView->setColumnWidth( myType.getTaggedColumn("HEIGHT"), 80 );
    myTableView->setColumnWidth( myType.getTaggedColumn("FLOORSCALE"), 80 );
    myTableView->setColumnWidth( myType.getTaggedColumn("BEAM"), 80 );
    myTableView->setColumnWidth( myType.getTaggedColumn("SLAB"), 80 );
    myTableView->setColumnWidth( myType.getTaggedColumn("UUID"), 120 );

    QHash<int, QStringList> lmlist = myType.getLimitedStrings();
    Q_FOREACH (int col, lmlist.keys()) {
        ComboBoxDelegate *deleg = new ComboBoxDelegate(lmlist.value(col));
        myTableView->setItemDelegateForColumn(col, deleg);
    }
    connect ( myTableView, QOverload<>::of(&CustomTableView::delKeyClicked), this, &CustomTableWidget::del );
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &CustomTableWidget::slotRowActionEnable );

    QHash<int, DATATYPE> refcol = myType.getReferenceColumns();
    Q_FOREACH(int cc, refcol.keys()){
        ReferenceComboBoxDelegate *deleg = new ReferenceComboBoxDelegate(refcol.value(cc));
        myTableView->setItemDelegateForColumn(cc, deleg);
    }
    myTableView->horizontalHeader()->setStretchLastSection(true);
}

void FloorSettingTableWidget::createWidgets()
{
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout, 0);
    vlay->addWidget(myTableView, 1);
    this->setLayout(vlay);
}

void FloorSettingTableWidget::slotDeleteLine()
{

    QModelIndexList rows_idxlist = mySelectionModel->selectedRows();
    bool isDisable = false;
    Q_FOREACH (QModelIndex idx, rows_idxlist) {
        if (myTableModel->isRegisterRow(idx.row())) isDisable = true;
    }
    if ( isDisable ) {
        if ( !myTableModel->isEditOn() ) return;
        if ( warningLineDelete() ) return;
    }

    CustomTableWidget::slotDeleteLine();
    emit floorItemChanged();
}

void FloorSettingTableWidget::modelUpload()
{
    myTableModel->linkUUID();
    myTableModel->setEditOn(false);
    myTableModel->keepCurrentData();
    undoStack->clear();
}

void FloorSettingTableWidget::modelEdit()
{
    myTableModel->setEditOn(true);
}

void FloorSettingTableWidget::modelRewind()
{
    myTableModel->backPreviousData();
    myTableModel->setEditOn(false);
    undoStack->clear();
    emit floorItemChanged();
}

void FloorSettingTableWidget::slotManageTableData( const QModelIndex &idx_a,
                                                   const QModelIndex &idx_b)
{
    if ( idx_a.column() > 0 || idx_b.column() > 0 ) emit floorItemChanged();
}

QList<FLOORVALUES> FloorSettingTableWidget::getFloorList(bool isAll) const
{
    QList<FLOORVALUES> floorList;
    QList<qreal> fhlist;

    for (int i = 0; i < myTableModel->rowCount() ; i++) {

        QVariant var1 = myTableModel->getData(i, myType.getTaggedColumn("NAME"));
        QVariant var2 = myTableModel->getData(i, myType.getTaggedColumn("HEIGHT"));
        if ( !var1.isValid() || !var2.isValid() ) continue;
        QString name = var1.toString();
        qreal height = var2.toDouble();

        bool displaysFloorScale = myTableModel->getData(i, myType.getTaggedColumn("FLOORSCALE")).toString() != u8"表示しない";
        if ( !isAll && !displaysFloorScale){
            continue;
        }
        QString beam = myTableModel->getData(i, myType.getTaggedColumn("BEAM")).toString();
        QString slab = myTableModel->getData(i, myType.getTaggedColumn("SLAB")).toString();
        QUuid uid = myTableModel->getData(i, myType.getTaggedColumn("UUID")).toString();

        int idx = 0;
        for (int j = 0; j < fhlist.count(); j++) {
            if ( height < fhlist.at(j) ) break;
            idx = j + 1;
        }
        floorList.insert(idx, FLOORVALUES(name, height, displaysFloorScale, beam, slab, uid));
        fhlist.insert(idx, height);
    }

    return floorList;
}

bool FloorSettingTableWidget::existResisterRow()
{
    return myTableModel->existRegisterRow();
}

bool FloorSettingTableWidget::warningLineDelete()
{
    return Utility::showWarningMessage2(this, u8"登録済の階が選択されています。\n削除しますか？");
}

void FloorSettingTableWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    QList<FLOORVALUES> floors = UnifiedInputData::getInInstance()->getFloorList();
    for (int i = 0; i < floors.count(); i++){
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(floors.at(i).floorName));
        data.append(QVariant(floors.at(i).floorHeight));
        data.append(QVariant(floors.at(i).displaysFloorScale));
        data.append(QVariant(floors.at(i).beamConcrete));
        data.append(QVariant(floors.at(i).slabConcrete));
        data.append(QVariant(floors.at(i).UUID.toString()));
        temp.appendRowData(data);
    }
    myTableModel->reflectUnifiedData(temp);
}

bool FloorSettingTableWidget::readTextFile(QTextStream &infile, bool flg)
{
    if ( ! myTableModel->readListTextData(infile, flg) ) return false;
    modelEdit();
    emit floorItemChanged();
    undoStack->clear();
    return true;
}

void FloorSettingTableWidget::writeTextFile(QTextStream &outfile, bool writeUuid)
{
    QList<int> list;
    if (!writeUuid){
        list.append( myType.getTaggedColumn("UUID") );
    }
    myTableModel->writeListTextData(outfile, list );
}
} // namespace post3dapp
