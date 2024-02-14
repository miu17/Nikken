#include "memberloadsetting_tablewidget.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QMenu>

#include "combobox_delegate.h"
#include "customtable_model.h"
#include "customtable_view.h"
#include "define_defaultvalue.h"
#include "sortfilter_proxymodel.h"
#include "unified_inputdata.h"

namespace post3dapp{
MemberLoadSettingTableWidget::MemberLoadSettingTableWidget( const CustomTableDataType &type,
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

void MemberLoadSettingTableWidget::createModels()
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
              this, &MemberLoadSettingTableWidget::slotManageTableData );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &CustomTableWidget::slotEditItem );
}

void MemberLoadSettingTableWidget::createViews()
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
    myTableView->setColumnWidth(myType.getTaggedColumn("TYPE"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("COORDINATE"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("DIRECTION"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("WA"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("WB"), 80);
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

void MemberLoadSettingTableWidget::createWidgets()
{
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout, 0);
    vlay->addWidget(myTableView, 1);
    this->setLayout(vlay);
}

void MemberLoadSettingTableWidget::slotDeleteLine()
{
    CustomTableWidget::slotDeleteLine();
    emit sectionItemChanged();
}

void MemberLoadSettingTableWidget::modelUpload()
{
    localModel->keepCurrentData();
    undoStack->clear();
}

void MemberLoadSettingTableWidget::modelRewind()
{
    localModel->backPreviousData();
    undoStack->clear();
    emit sectionItemChanged();
}

bool MemberLoadSettingTableWidget::readTextFile(QTextStream &infile, bool flg)
{
    if ( ! myTableModel->readListTextData(infile, flg) ) return false;
    emit sectionItemChanged();
    undoStack->clear();
    return true;
}

void MemberLoadSettingTableWidget::slotManageTableData( const QModelIndex &idx_a,
                                                        const QModelIndex &idx_b)
{
    Q_UNUSED(idx_a);
    if ( idx_b.column() > 0 ) emit sectionItemChanged();
}

QStringList MemberLoadSettingTableWidget::checkLoadTypes() const
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

QList<MEMBERLOADVALUES> MemberLoadSettingTableWidget::getValueList() const
{
    QList<MEMBERLOADVALUES> mloadList;

    const QStringList slist1{REGULATION::defaultLoadType};
    const QStringList slist2{REGULATION::defaultMemberLoadCoordinate};
    const QStringList slist3{REGULATION::defaultMemberLoadDirection};

    QStringList nameList;
    for ( int i = 0; i < myTableModel->rowCount() ; i++ ) {
        QVariant var1 = myTableModel->getData(i, myType.getTaggedColumn("NAME"));
        if ( !var1.isValid() ) continue;
        QString loadName = var1.toString();

        int itype = slist1.indexOf(myTableModel->getData(i, myType.getTaggedColumn("TYPE")).toString());
        LoadType::LOADTYPE ltype = ( itype < 0 ) ? LoadType::LTYPE_DL : static_cast<LoadType::LOADTYPE>(itype) ;

        int icor = slist2.indexOf(myTableModel->getData(i, myType.getTaggedColumn("COORDINATE")).toString());
        LCORSYSTEM lcor = ( icor < 0 ) ? LCORSYSTEM::LCOR_GLOBAL : static_cast<LCORSYSTEM>(icor) ;

        int idir = slist3.indexOf(myTableModel->getData(i, myType.getTaggedColumn("DIRECTION")).toString());
        LOADDIR ldir = ( idir < 0 ) ? LOADDIR::LDIR_Z : static_cast<LOADDIR>(idir) ;

        qreal wa = myTableModel->getData(i, myType.getTaggedColumn("WA")).toDouble();
        qreal wb = myTableModel->getData(i, myType.getTaggedColumn("WB")).toDouble();
        QString comment = myTableModel->getData(i, myType.getTaggedColumn("COMMENT")).toString();

        nameList.append(loadName);
        mloadList.append(MEMBERLOADVALUES(loadName, QHash<LoadType::LOADTYPE, MEMBERLOADDIM>{{ltype, MEMBERLOADDIM(lcor, ldir, wa, wb, comment)}}));
    }

    return mloadList;
}

QStringList MemberLoadSettingTableWidget::getSectionNameList() const
{
    QList<QVariant> vlist = myTableModel->getColumnData(myType.getTaggedColumn("NAME"));
    QStringList namelist;
    Q_FOREACH (QVariant var, vlist) {
        QString str = var.toString();
        if ( !namelist.contains(str) && !str.isEmpty() ) namelist.append(str);
    }
    return namelist;
}

void MemberLoadSettingTableWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    QList<MEMBERLOADVALUES> list = UnifiedInputData::getInInstance()->getMemberLoadList();
    Q_FOREACH(MEMBERLOADVALUES ml, list){
        QHashIterator<LoadType::LOADTYPE, MEMBERLOADDIM> iter(ml.loadTable);
        while(iter.hasNext()){
            iter.next();
            QList<QVariant> data;
            data.append(QVariant(i));
            data.append(QVariant(ml.loadName));
            data.append(QVariant(LoadType::enumToString(iter.key())));
            MEMBERLOADDIM dim = iter.value();
            data.append(QVariant(REGULATION::defaultMemberLoadCoordinate.at(static_cast<int>(dim.corSytem))));
            data.append(QVariant(REGULATION::defaultMemberLoadDirection.at(static_cast<int>(dim.loadDir))));
            data.append(QVariant(QString::number(dim.wa)));
            data.append(QVariant(QString::number(dim.wb)));
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
