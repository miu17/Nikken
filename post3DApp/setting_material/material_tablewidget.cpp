#include <QtGui>
#include <QDebug>
#include <QUndoStack>

#include "material_tablewidget.h"
#include "sortfilter_proxymodel.h"
#include "combobox_delegate.h"
#include "standard_material_delegate.h"
#include "material_filterdialog.h"
#include "material_tablemodel.h"
#include "material_tableview.h"
#include "customtable_commands.h"
#include "unified_inputdata.h"
#include "define_materialtable.h"
#include "customtable_model.h"
#include "define_material_struct.h"

namespace post3dapp{
MaterialTableWidget::MaterialTableWidget( const MaterialDataType &type, QWidget *parent )
    : CustomTableWidget( type, parent ), myMaterialType(type)
{
    myMenu = new QMenu(this);
    undoStack = new QUndoStack(this);
    createActions();
    createButtons();
    createModels();
    createViews();
    createWidgets();
}

void MaterialTableWidget::createModels()
{
    myTableModel = new MaterialTableModel(myMaterialType, this);
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel = new QItemSelectionModel(myProxyModel);

    connect ( myTableModel, &CustomTableModel::rowCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxValue );
    connect ( myTableModel, &CustomTableModel::dataCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxMinimum );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &MaterialTableWidget::slotEditItem );
    connect ( myTableModel, &QAbstractItemModel::dataChanged,
              this, &MaterialTableWidget::slotManageTableData );
}

void MaterialTableWidget::createViews()
{
    myTableView = new MaterialTableView(this);
    tableView_1 = new MaterialTableView(this);

    QList<CustomTableView *> views;
    views << myTableView << tableView_1;
    Q_FOREACH (CustomTableView *view, views) {
        view->addActions(myActionList);
        view->setMenu(myMenu);
        view->setModel(myProxyModel);
        view->setSelectionModel(mySelectionModel);
        connect ( view, QOverload<>::of(&CustomTableView::delKeyClicked), this, &MaterialTableWidget::del );
        connect ( view, SIGNAL( homeKeyClicked() ), this, SLOT( changeCurrentView() ) );
        connect ( view, SIGNAL( requestChangeViewNext() ), this, SLOT( changeCurrentView() ) );
        connect ( view, SIGNAL( requestChangeViewBack() ), this, SLOT( changeCurrentViewBack() ) );
    }


    QHash<int, QStringList> lmlist = myMaterialType.getLimitedStrings();
    Q_FOREACH (int cc, lmlist.keys()) {
        QStringList stl = lmlist.value(cc);
        myTableView->setItemDelegateForColumn(cc, new ComboBoxDelegate(stl));
        tableView_1->setItemDelegateForColumn(cc, new ComboBoxDelegate(stl));
        myTableModel->setLimitedStrings(cc, stl);
    }

    int sp1 = myMaterialType.getSeparateColumn1();
    for (int i = 0; i < myProxyModel->columnCount() ; i++) {
        if ( sp1 <= i ) myTableView->setColumnHidden(i, true);
        if ( i < sp1 ) tableView_1->setColumnHidden(i, true);
    }

    myTableView->setItemDelegateForColumn(2, new StandardMaterialDelegate);

    filter_columns = MaterialFilterDialog::loadDefaultFilters();
    setFilterColumns(filter_columns);
    ((MaterialTableView *)myTableView)->setTableViewBorderColumns(myMaterialType, filter_columns);
    tableView_1->setTableViewBorderColumns(myMaterialType, filter_columns);

    QHash<QString, int> coltag = myMaterialType.getTaggedColumns();
    myTableView->setColumnWidth(coltag["NO"], 60);
    myTableView->setColumnWidth(coltag["CATEGORY"], 100);
    myTableView->setColumnWidth(coltag["LABEL"], 110);
    myTableView->setColumnWidth(coltag["SYMBOL"], 60);
    myTableView->setColumnWidth(coltag["RANGEFROM"], 60);
    myTableView->setColumnWidth(coltag["RANGETO"], 60);

    for ( int i = 1; i < tableView_1->model()->columnCount(); ++i )
        tableView_1->setColumnWidth(i, 75);

    myTableView->horizontalHeader()->setStretchLastSection(true);
    tableView_1->horizontalHeader()->setStretchLastSection(true);
}

void MaterialTableWidget::createWidgets()
{

    QWidget *tv1_wid = createTableLabel(myTableView, u8"材料");
    QWidget *tv2_wid = createTableLabel(tableView_1, u8"諸元");
    tv1_wid->setFixedWidth(453);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(tv1_wid);
    hlay1->addWidget(tv2_wid);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->setSpacing(0);
    vlay1->addLayout(topLayout, 0);
    vlay1->addLayout(hlay1, 1);
    QFrame *view_frame = new QFrame(this);
    view_frame->setLayout(vlay1);

    QHBoxLayout *hlay3 = new QHBoxLayout;
    hlay3->addWidget(view_frame);
    this->setLayout(hlay3);
}

QWidget *MaterialTableWidget::createTableLabel(CustomTableView *tv, const QString &str)
{
    QLabel *tv_label = new QLabel(str);
    QPalette pal = tv_label->palette();
    pal.setColor( QPalette::Window, QColor(200, 220, 200));
    tv_label->setPalette( pal );
    tv_label->setAutoFillBackground(true);
    tv_label->setFixedHeight(25);
    tv_label->setAlignment(Qt::AlignCenter);
    QVBoxLayout *tv_lay = new QVBoxLayout;
    tv_lay->setSpacing(0);
    tv_lay->addWidget(tv_label);
    tv_lay->addWidget(tv);
    tv_lay->setContentsMargins(0, 0, 0, 0);
    QWidget *tv_wid = new QWidget(this);
    tv_wid->setLayout(tv_lay);

    return tv_wid;
}

void MaterialTableWidget::modelUpload()
{
    myTableModel->keepCurrentData();
    undoStack->clear();
}

void MaterialTableWidget::modelRewind()
{
    myTableModel->backPreviousData();
    undoStack->clear();
    emit sectionItemChanged();
}

bool MaterialTableWidget::readTextFile(QTextStream &infile, bool flg)
{
    if ( ! myTableModel->readListTextData(infile, flg) ) return false;
    emit sectionItemChanged();
    undoStack->clear();
    return true;
}

void MaterialTableWidget::slotManageTableData(const QModelIndex &, const QModelIndex &idx)
{
    if ( idx.column() > 0 ) emit sectionItemChanged();

    QModelIndex cur_idx = myTableView->currentIndex();

    if (tableView_1 != NULL)tableView_1->reset();
    myTableView->reset();

    int sp1 = myMaterialType.getSeparateColumn1();
    if (sp1 < 0) {
        sp1 = 1000;
    }

    if ( cur_idx.column() < sp1 ) {
        myTableView->setCurrentIndex(cur_idx);
        myTableView->setFocus();
    } else if ( sp1 <= cur_idx.column() ) {
        tableView_1->setCurrentIndex(cur_idx);
        tableView_1->setFocus();
    }
}

void MaterialTableWidget::changeCurrentView()
{
    int viewcount = 0;
    if (myTableView != NULL)viewcount++;
    if (tableView_1 != NULL)viewcount++;

    QModelIndex idx = myTableView->currentIndex();
    int row = idx.row();
    int col = idx.column();

    int sp1 = myMaterialType.getSeparateColumn1();
    if (sp1 < 0) {
        sp1 = 1000;
    }

    QList<int> mfirstcolumns = ((MaterialTableView *)myTableView)->getMiddleViewFirstColumns();
    int firstcolumn = 0; // ((MaterialTableView*)myTableView)->getFirstColumn();
    QModelIndex idx1;
    if ( col < sp1 ) {
        int nextfirstcolumn = (viewcount == 1 ? firstcolumn : mfirstcolumns.at(0));
        if ( viewcount == 1 && row < myProxyModel->rowCount() - 1 ) row++;
        idx1 = myProxyModel->index(row, nextfirstcolumn);
    } else if ( sp1 <= col ) {
        int nextfirstcolumn = (viewcount == 2 ? firstcolumn : mfirstcolumns.at(1));
        if ( viewcount == 2 && row < myProxyModel->rowCount() - 1 ) row++;
        idx1 = myProxyModel->index(row, nextfirstcolumn);
    }

    QAbstractItemView::SelectionBehavior behavor
        = ( idx1.column() == 0 ) ? QAbstractItemView::SelectRows : QAbstractItemView::SelectItems ;

    if ( idx1.column() < sp1 ) {
        myTableView->setCurrentIndex(idx1);
        myTableView->setFocus();
        myTableView->setSelectionBehavior(behavor);
    } else if ( idx1.column() >= sp1 ) {
        tableView_1->setCurrentIndex(idx1);
        tableView_1->setFocus();
        tableView_1->setSelectionBehavior(behavor);
    }

}

void MaterialTableWidget::changeCurrentViewBack()
{
    QModelIndex idx = myTableView->currentIndex();
    int row = idx.row();
    int col = idx.column();

    int sp1 = myMaterialType.getSeparateColumn1();
    if (sp1 < 0) {
        sp1 = 1000;
    }
    QList<int> mlastcolumns = ((MaterialTableView *)myTableView)->getMiddleViewLastColumns();
    int lastcolumn = ((MaterialTableView *)myTableView)->getLastColumn();

    QModelIndex idx1;
    if ( col < sp1 ) {
        if ( row > 0 ) row--;
        idx1 = myProxyModel->index(row, lastcolumn);
    } else if ( sp1 <= col ) {
        idx1 = myProxyModel->index(row, mlastcolumns.at(0));
    }

    QAbstractItemView::SelectionBehavior behavor
        = ( idx1.column() == 0 ) ? QAbstractItemView::SelectRows : QAbstractItemView::SelectItems;

    if (idx1.column() < sp1) {
        myTableView->setCurrentIndex(idx1);
        myTableView->setFocus();
        myTableView->setSelectionBehavior(behavor);
    } else if (idx1.column() >= sp1) {
        tableView_1->setCurrentIndex(idx1);
        tableView_1->setFocus();
        tableView_1->setSelectionBehavior(behavor);
    }

}

void MaterialTableWidget::setDefaultValuePatch(int row, int col, const QVariant &dvalue)
{
    myMaterialType.setDefaultValuePatch(row, col, dvalue);
    ((MaterialTableModel *)myTableModel)->setDefaultValuePatch(row, col, dvalue);

    slotManageTableData(QModelIndex(), QModelIndex());
}

void MaterialTableWidget::setFilterColumns(const QList<bool> &_filter)
{
    QList<bool> filter = _filter;
    if (filter.count() != myTableModel->columnCount()) {
        filter.clear();
        for (int i = 0; i < myTableModel->columnCount(); ++i)filter << true;
    }//filterの要素数がモデルのコラム数と違う場合は全部trueにする
    int sp1 = myMaterialType.getSeparateColumn1();
    if (sp1 < 0)sp1 = 1000;
    for (int i = 0; i < filter.count(); ++i) {
        if (i < sp1) {
            if (filter[i])myTableView->showColumn(i);
            if (!filter[i])myTableView->hideColumn(i);
        } else if (i >= sp1) {
            if (filter[i])tableView_1->showColumn(i);
            if (!filter[i])tableView_1->hideColumn(i);
        }
    }
    if (myTableView != NULL)myTableView->reset();
    if (tableView_1 != NULL)tableView_1->reset();
    filter_columns = filter;
    emit filterColumnsChanged(filter);
}

void MaterialTableWidget::setTableViewBorderColumns(const MaterialDataType &mtype,
                                                    const QList<bool> &filters)
{
    ((MaterialTableView *)myTableView)->setTableViewBorderColumns(mtype, filters);
    if (tableView_1 != NULL)tableView_1->setTableViewBorderColumns(mtype, filters);
}

void MaterialTableWidget::slotDeleteLine()
{
    CustomTableWidget::slotDeleteLine();
    emit sectionItemChanged();
}

void MaterialTableWidget::paste()
{
    this->pasteSectionData();
}

void MaterialTableWidget::del()
{
    this->deleteSectionData();
}

void MaterialTableWidget::slotEditItem(const QModelIndex &idx, const QVariant &var)
{
    if ( var.toString() == myTableModel->getData(idx.row(), idx.column()).toString() ) return;
    this->editSectionData(idx, var);
}

void MaterialTableWidget::pasteSectionData()
{
    QModelIndex cur_idx = myTableView->currentIndex();
    QString clipstr = QApplication::clipboard()->text();

    undoStack->beginMacro("sectionTable paste");

    QModelIndexList idxlist;
    QList<QVariant> varlist;
    QStringList oneline = clipstr.split('\n');
    for (int i = 0; i < oneline.count() - 1; ++i) {
        int row = cur_idx.row() + i;
        QStringList cells = oneline[i].split('\t');
        for (int j = 0; j < oneline[i].count('\t') + 1 ; ++j) {
            int col = cur_idx.column() + j;
            if ( row >= myProxyModel->rowCount() || col >= myProxyModel->columnCount() ) continue;
            QModelIndex idx = myProxyModel->mapToSource( myProxyModel->index(row, col) );

            if ( myTableModel->isDisableItem(idx) ) continue; //////////////

            idxlist.append( idx );
            varlist.append( QVariant( cells[j] ) );
        }
    }
    QUndoCommand *setCommand = new SetMultiCommand(myTableModel, idxlist, varlist);
    undoStack->push(setCommand);

    QModelIndexList i_idxlist;
    QList<QVariant> i_varlist;
    Q_FOREACH ( QModelIndex idx, idxlist )
        inputStandardValues( idx, i_idxlist, i_varlist );
    if ( !i_idxlist.isEmpty() ) {
        QUndoCommand *i_setCommand = new SetMultiCommand(myTableModel, i_idxlist, i_varlist);
        undoStack->push(i_setCommand);
    }

    undoStack->endMacro();

    if (!undoOnOff) {
        undoStack->clear();
    }
}

void MaterialTableWidget::deleteSectionData()
{
    if ( !mySelectionModel->hasSelection() ) return;
    QModelIndexList p_idxlist = mySelectionModel->selectedIndexes();

    undoStack->beginMacro("sectionTable delete");

    QModelIndexList idxlist;
    QList<QVariant> varlist;
    Q_FOREACH ( QModelIndex p_idx, p_idxlist ) {
        QModelIndex s_idx = myProxyModel->mapToSource( p_idx );

        if ( myTableModel->isDisableItem(s_idx) ) continue; //////////////

        idxlist.append( s_idx );
        varlist.append( QVariant("") );
    }
    QUndoCommand *setCommand = new SetMultiCommand(myTableModel, idxlist, varlist);
    undoStack->push(setCommand);

    QModelIndexList i_idxlist;
    QList<QVariant> i_varlist;
    Q_FOREACH ( QModelIndex idx, idxlist )
        inputStandardValues( idx, i_idxlist, i_varlist );
    if ( !i_idxlist.isEmpty() ) {
        QUndoCommand *i_setCommand = new SetMultiCommand(myTableModel, i_idxlist, i_varlist);
        undoStack->push(i_setCommand);
    }

    undoStack->endMacro();

    if (!undoOnOff) {
        undoStack->clear();
    }
}

void MaterialTableWidget::editSectionData(const QModelIndex &idx, const QVariant &var)
{
    undoStack->beginMacro("sectionTable editItem");

    QModelIndexList idxlist;
    QList<QVariant> varlist;
    idxlist.append( idx );
    varlist.append( var );
    QUndoCommand *setCommand = new SetMultiCommand(myTableModel, idxlist, varlist);
    undoStack->push(setCommand);

    QModelIndexList i_idxlist;
    QList<QVariant> i_varlist;
    inputStandardValues( idx, i_idxlist, i_varlist );
    if ( !i_idxlist.isEmpty() ) {
        QUndoCommand *i_setCommand = new SetMultiCommand(myTableModel, i_idxlist, i_varlist);
        undoStack->push(i_setCommand);
    }

    undoStack->endMacro();

    if ( !undoOnOff ) {
        undoStack->clear();
    }
}

void MaterialTableWidget::inputStandardValues(const QModelIndex &idx,
                                              QModelIndexList &idxlist, QList<QVariant> &varlist)
{
    int row = idx.row();
    int col = idx.column();
    QString str = idx.data(Qt::EditRole).toString();
    if ( myMaterialType.getShapeTypeColumns().contains(col) ) {
        if ( str.isEmpty() ) return;
        QString str_a = myTableModel->index(row, myType.getTaggedColumn("LABEL")).data().toString();
        if ( myMaterialType.standardMaterialNameList(str).contains(str_a) ) {
            for ( int i = myType.getTaggedColumn("E"); i <= myType.getTaggedColumn("UA_S") ; i++ ) {
                StandardMaterialTable::format fm
                    = static_cast<StandardMaterialTable::format>(i - 4);
                QString stt = myMaterialType.standardMaterialValue(str_a, fm);
                idxlist.append( myTableModel->index(row, i) );
                varlist.append( QVariant(stt) );
            }
            return;
        }
    } else if ( myMaterialType.getShapeTypeColumns().contains(col - 1) ) {
        QString str_a = myTableModel->index(row, 1).data().toString();
        if ( str_a.isEmpty() ) return;

        if ( myMaterialType.standardMaterialNameList(str_a).contains(str) ) {
            for ( int i = myType.getTaggedColumn("E"); i <= myType.getTaggedColumn("UA_S") ; i++ ) {
                StandardMaterialTable::format fm
                    = static_cast<StandardMaterialTable::format>(i - 4);
                QString stt = myMaterialType.standardMaterialValue(str, fm);
                idxlist.append( myTableModel->index(row, i) );
                varlist.append( QVariant(stt) );
            }
            return;
        }
    }
}

void  MaterialTableWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(QString conc, UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPMACONC)){
        MATERIALVALUES mv = UnifiedInputData::getInInstance()->getMaterialValue(DATATYPE::TPMACONC, conc);
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(MaterialType::enumToString(MaterialType::Type::CONCRETE)));
        data.append(QVariant(mv.materialName));
        data.append(QVariant());
        data.append(QVariant());
        data.append(QVariant());
        data.append(QVariant(QString::number(mv.E)));
        data.append(QVariant(QString::number(mv.G)));
        data.append(QVariant(QString::number(mv.unitW1)));
        data.append(QVariant(QString::number(mv.unitW2)));
        data.append(QVariant(QString::number(mv.modularRatio)));
        data.append(QVariant(QString::number(mv.F)));
        data.append(QVariant(QString::number(mv.Ft_L)));
        data.append(QVariant(QString::number(mv.Ft_S)));
        data.append(QVariant(QString::number(mv.Ft_U)));
        data.append(QVariant(QString::number(mv.Fc_L)));
        data.append(QVariant(QString::number(mv.Fc_S)));
        data.append(QVariant(QString::number(mv.Fc_U)));
        data.append(QVariant(QString::number(mv.Fs_L)));
        data.append(QVariant(QString::number(mv.Fs_S)));
        data.append(QVariant(QString::number(mv.Fs_U)));
        data.append(QVariant(QString::number(mv.Fa_L)));
        data.append(QVariant(QString::number(mv.Fa_S)));
        data.append(QVariant(QString::number(mv.Fa_U)));
        data.append(QVariant(QString::number(mv.Fa_Lu)));
        data.append(QVariant(QString::number(mv.Fa_Su)));
        i++;
        temp.appendRowData(data);
    }

    Q_FOREACH(QString steel, UnifiedInputData::getInInstance()->getNameList(DATATYPE::TPMASTEEL)){
        MATERIALVALUES mv = UnifiedInputData::getInInstance()->getMaterialValue(DATATYPE::TPMASTEEL, steel);
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(MaterialType::enumToString(MaterialType::Type::STEEL)));
        data.append(QVariant(mv.materialName));
        data.append(QVariant());
        data.append(QVariant());
        data.append(QVariant());
        data.append(QVariant(QString::number(mv.E)));
        data.append(QVariant(QString::number(mv.G)));
        data.append(QVariant(QString::number(mv.unitW1)));
        data.append(QVariant(QString::number(mv.unitW2)));
        data.append(QVariant(QString::number(mv.modularRatio)));
        data.append(QVariant(QString::number(mv.F)));
        data.append(QVariant(QString::number(mv.Ft_L)));
        data.append(QVariant(QString::number(mv.Ft_S)));
        data.append(QVariant(QString::number(mv.Ft_U)));
        data.append(QVariant(QString::number(mv.Fc_L)));
        data.append(QVariant(QString::number(mv.Fc_S)));
        data.append(QVariant(QString::number(mv.Fc_U)));
        data.append(QVariant(QString::number(mv.Fs_L)));
        data.append(QVariant(QString::number(mv.Fs_S)));
        data.append(QVariant(QString::number(mv.Fs_U)));
        data.append(QVariant(QString::number(mv.Fa_L)));
        data.append(QVariant(QString::number(mv.Fa_S)));
        data.append(QVariant(QString::number(mv.Fa_U)));
        data.append(QVariant(QString::number(mv.Fa_Lu)));
        data.append(QVariant(QString::number(mv.Fa_Su)));
        i++;
        temp.appendRowData(data);
    }

    Q_FOREACH(QString rein, UnifiedInputData::getInInstance()->getReinforceNameList()){
        MATERIALVALUES mv = UnifiedInputData::getInInstance()->getMaterialValue(DATATYPE::TPMAREIN, rein);
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(MaterialType::enumToString(MaterialType::Type::REINFORCE)));
        data.append(QVariant(mv.materialName));
        data.append(QVariant(mv.symbol == QString{} ? u8"D" : mv.symbol));
        data.append(QVariant(mv.barMin));
        data.append(QVariant(mv.barMax));
        data.append(QVariant(QString::number(mv.E)));
        data.append(QVariant(QString::number(mv.G)));
        data.append(QVariant(QString::number(mv.unitW1)));
        data.append(QVariant(QString::number(mv.unitW2)));
        data.append(QVariant(QString::number(mv.modularRatio)));
        data.append(QVariant(QString::number(mv.F)));
        data.append(QVariant(QString::number(mv.Ft_L)));
        data.append(QVariant(QString::number(mv.Ft_S)));
        data.append(QVariant(QString::number(mv.Ft_U)));
        data.append(QVariant(QString::number(mv.Fc_L)));
        data.append(QVariant(QString::number(mv.Fc_S)));
        data.append(QVariant(QString::number(mv.Fc_U)));
        data.append(QVariant(QString::number(mv.Fs_L)));
        data.append(QVariant(QString::number(mv.Fs_S)));
        data.append(QVariant(QString::number(mv.Fs_U)));
        data.append(QVariant(QString::number(mv.Fa_L)));
        data.append(QVariant(QString::number(mv.Fa_S)));
        data.append(QVariant(QString::number(mv.Fa_U)));
        data.append(QVariant(QString::number(mv.Fa_Lu)));
        data.append(QVariant(QString::number(mv.Fa_Su)));
        i++;
        temp.appendRowData(data);
    }

    myTableModel->reflectUnifiedData(temp);
}

MaterialDataType MaterialTableWidget::getDataType() const
{
    return myMaterialType;
}

int MaterialTableWidget::rowCount() const
{
    return myTableModel->rowCount();
}

QList<MATERIALVALUES> MaterialTableWidget::getMaterialList() const
{
    return ((MaterialTableModel *)myTableModel)->getMaterialList();
}
} // namespace post3dapp
