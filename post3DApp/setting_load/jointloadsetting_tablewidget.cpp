#include "jointloadsetting_tablewidget.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QUndoStack>

#include "combobox_delegate.h"
#include "customtable_model.h"
#include "customtable_view.h"
#include "sortfilter_proxymodel.h"
#include "unified_inputdata.h"

namespace post3dapp{
JointLoadSettingTableWidget::JointLoadSettingTableWidget( const CustomTableDataType &type,
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

void JointLoadSettingTableWidget::createModels()
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
              this, &JointLoadSettingTableWidget::slotManageTableData );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &CustomTableWidget::slotEditItem );
}

void JointLoadSettingTableWidget::createViews()
{
    myTableView = new CustomTableView(this);
    myTableView->setFirstColumn(myType.getTaggedColumn("NO"));
    myTableView->setLastColumn(myType.getTaggedColumn("COMMENT"));

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth(myType.getTaggedColumn("NO"), 60);
    myTableView->setColumnWidth(myType.getTaggedColumn("NAME"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("TYPE"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("FX"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("FY"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("FZ"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("MX"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("MY"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("MZ"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("COMMENT"), 150);

    QHash<int, QStringList> lmlist = myType.getLimitedStrings();
    Q_FOREACH (int col, lmlist.keys()) {
        ComboBoxDelegate *deleg = new ComboBoxDelegate(lmlist.value(col));
        myTableView->setItemDelegateForColumn(col, deleg);
    }
    connect ( myTableView, QOverload<>::of(&CustomTableView::delKeyClicked), this, &CustomTableWidget::del );
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &CustomTableWidget::slotRowActionEnable );
}

void JointLoadSettingTableWidget::createWidgets()
{
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout, 0);
    vlay->addWidget(myTableView, 1);
    this->setLayout(vlay);
}

void JointLoadSettingTableWidget::slotDeleteLine()
{
    CustomTableWidget::slotDeleteLine();
    emit sectionItemChanged();
}

void JointLoadSettingTableWidget::modelUpload()
{
    localModel->keepCurrentData();
    undoStack->clear();
}

void JointLoadSettingTableWidget::modelRewind()
{
    localModel->backPreviousData();
    undoStack->clear();
    emit sectionItemChanged();
}

bool JointLoadSettingTableWidget::readTextFile(QTextStream &infile, bool flg)
{
    if ( ! myTableModel->readListTextData(infile, flg) ) return false;
    emit sectionItemChanged();
    undoStack->clear();
    return true;
}

void JointLoadSettingTableWidget::slotManageTableData( const QModelIndex &idx_a,
                                                       const QModelIndex &idx_b)
{
    Q_UNUSED(idx_a);
    if ( idx_b.column() > 0 ) emit sectionItemChanged();
}

QStringList JointLoadSettingTableWidget::checkLoadTypes() const
{
    QStringList nameList, errList;
    QList<QStringList> typeList;
    for ( int i = 0; i < myTableModel->rowCount() ; i++ ) {
        QVariant var1 = myTableModel->getData(i, myType.getTaggedColumn("NAME"));
        QVariant var2 = myTableModel->getData(i, myType.getTaggedColumn("TYPE"));
        if ( !var1.isValid() ) continue;
        QString str1 = var1.toString();
        QString str2 = var2.toString();
        if ( str2.isEmpty() ) {
            if ( !errList.contains(str1) ) errList.append(str1);
            continue;
        }
        int idx = nameList.indexOf(str1);
        if ( idx < 0 ) {
            nameList.append( str1 );
            typeList.append( QStringList() << str2 );
        } else {
            QStringList types = typeList.at(idx);
            if ( types.contains(str2) ) {
                if ( !errList.contains(str1) ) errList.append(str1);
                continue;
            }
            typeList.replace( idx, types << str2 );
        }
    }
    return errList;
}

QList<JOINTLOADVALUES> JointLoadSettingTableWidget::getValueList() const
{
    QList<JOINTLOADVALUES> jloadList;

    QStringList slist;
    slist << "DL" << "LL1"
          << "LL2" << "LL3"
          << "LL4"   << "LLE";

    QStringList nameList;
    for ( int i = 0; i < myTableModel->rowCount() ; i++ ) {
        QVariant var1 = myTableModel->getData(i, myType.getTaggedColumn("NAME"));
        QVariant var2 = myTableModel->getData(i, myType.getTaggedColumn("TYPE"));
        QVariant var3 = myTableModel->getData(i, myType.getTaggedColumn("FX"));
        QVariant var4 = myTableModel->getData(i, myType.getTaggedColumn("FY"));
        QVariant var5 = myTableModel->getData(i, myType.getTaggedColumn("FZ"));
        QVariant var6 = myTableModel->getData(i, myType.getTaggedColumn("MX"));
        QVariant var7 = myTableModel->getData(i, myType.getTaggedColumn("MY"));
        QVariant var8 = myTableModel->getData(i, myType.getTaggedColumn("MZ"));
        QVariant var9 = myTableModel->getData(i, myType.getTaggedColumn("COMMENT"));

        if ( !var1.isValid() ) continue;

        int itype = slist.indexOf(var2.toString());
        LoadType::LOADTYPE ltype = ( itype < 0 ) ? LoadType::LTYPE_DL : static_cast<LoadType::LOADTYPE>(itype) ;

        QString loadName = var1.toString();
        int idx = nameList.indexOf(loadName);

        if ( idx < 0 ) {
            nameList.append(loadName);
            QHash<LoadType::LOADTYPE, JOINTLOADDIM> hash;
            hash.insert(ltype, JOINTLOADDIM(var3.toDouble(), var4.toDouble(), var5.toDouble(),
                                            var6.toDouble(), var7.toDouble(), var8.toDouble(),var9.toString()));
            jloadList.append(JOINTLOADVALUES(loadName, hash));
        } else {
            JOINTLOADVALUES jv = jloadList.at(idx);
            QHash<LoadType::LOADTYPE, JOINTLOADDIM> hash = jv.loadTable;
            hash.insert(ltype, JOINTLOADDIM(var3.toDouble(), var4.toDouble(), var5.toDouble(),
                                            var6.toDouble(), var7.toDouble(), var8.toDouble(), var9.toString()));
            jloadList.replace(idx, JOINTLOADVALUES(loadName, hash));
        }
    }

    return jloadList;
}

QStringList JointLoadSettingTableWidget::getSectionNameList() const
{
    QList<QVariant> vlist = myTableModel->getColumnData(1);
    QStringList namelist;
    Q_FOREACH (QVariant var, vlist) {
        QString str = var.toString();
        if ( !namelist.contains(str) && !str.isEmpty() ) namelist.append(str);
    }
    return namelist;
}

void JointLoadSettingTableWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    QList<JOINTLOADVALUES> list = UnifiedInputData::getInInstance()->getJointLoadList();
    Q_FOREACH(JOINTLOADVALUES jl, list){
        QHashIterator<LoadType::LOADTYPE, JOINTLOADDIM> iter(jl.loadTable);
        while(iter.hasNext()){
            iter.next();
            QList<QVariant> data;
            data.append(QVariant(i));
            data.append(QVariant(jl.loadName));
            data.append(QVariant(LoadType::enumToString(iter.key())));
            JOINTLOADDIM dim = iter.value();
            data.append(QVariant(QString::number(dim.fx)));
            data.append(QVariant(QString::number(dim.fy)));
            data.append(QVariant(QString::number(dim.fz)));
            data.append(QVariant(QString::number(dim.mx)));
            data.append(QVariant(QString::number(dim.my)));
            data.append(QVariant(QString::number(dim.mz)));
            data.append(QVariant(dim.notes));
            temp.appendRowData(data);
            i++;
        }
    }

    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    undoStack->clear();
}
} // namespace post3dapp
