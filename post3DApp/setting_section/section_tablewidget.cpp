#include "section_tablewidget.h"

#include <QtGui>
#include <QDebug>

#include "combobox_delegate.h"
#include "customtable_commands.h"
#include "fileread_dialog.h"
#include "refercombobox_delegate.h"
#include "section_calculations.h"
#include "section_linebuttons.h"
#include "section_linecalculate.h"
#include "section_linedraw.h"
#include "section_tablemodel.h"
#include "section_tableview.h"
#include "sortfilter_proxymodel.h"
#include "standard_section_delegate.h"

namespace post3dapp{
SectionTableWidget::SectionTableWidget( const SectionDataType &type, QWidget *parent )
    : CustomTableWidget( type, parent ), mySectionType(type)
{
    myMenu = new QMenu(this);
    undoStack = new QUndoStack(this);
    createActions();
    createButtons();
}

void SectionTableWidget::createModels()
{
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel = new QItemSelectionModel(myProxyModel);

    connect ( myTableModel, &CustomTableModel::rowCountChanged,
              this, &SectionTableWidget::slotChangeSpinBoxValue );
    connect ( myTableModel, &CustomTableModel::dataCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxMinimum );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &SectionTableWidget::slotEditItem );
    connect ( myTableModel, &QAbstractItemModel::dataChanged,
              this, &SectionTableWidget::slotManageTableData );


    //edit flag
//    connect ( myTableModel, SIGNAL( dataChanged(const QModelIndex &, const QModelIndex &) ),
//              this, SIGNAL( signalDataEditing() ) );
}

void SectionTableWidget::createViews()
{

    /* viewの定義は派生クラスのcreateViewsにて、widgetに用いるviewの数に応じて行う。
     * 使わないViewはNULLとしておくこと。フィルタの初期化も派生先にて。 */

    QVector<CustomTableView *> views;
    views << myTableView << tableView_1 << tableView_2 << tableView_3;
    Q_FOREACH (CustomTableView *view, views) {
        if (view == NULL) continue;
        view->addActions(myActionList);
        view->setMenu(myMenu);
        view->setModel(myProxyModel);
        view->setSelectionModel(mySelectionModel);
        for (int i = 1; i < view->model()->columnCount(); ++i)
            view->setColumnWidth(i, 75);
        connect ( view, QOverload<>::of(&CustomTableView::delKeyClicked), this, &SectionTableWidget::del );
        connect ( view, SIGNAL( homeKeyClicked() ), this, SLOT( changeCurrentView() ) );
        connect ( view, SIGNAL( requestChangeViewNext() ), this, SLOT( changeCurrentView() ) );
        connect ( view, SIGNAL( requestChangeViewBack() ), this, SLOT( changeCurrentViewBack() ) );
        connect ( view, &CustomTableView::currentRowChanged, this, &SectionTableWidget::slotLineViewChange );

        connect ( view, &CustomTableView::selectionBehaviorChanged,
                  this, &CustomTableWidget::slotRowActionEnable );
    }

    myTableView->setColumnWidth(0, 60);

    int sp1 = mySectionType.getSeparateColumn1();
    int sp2 = mySectionType.getSeparateColumn2();
    int sp3 = mySectionType.getSeparateColumn3();
    if (sp1 < 0) {
        sp1 = 1000;
    }
    if (sp2 < 0) {
        sp2 = 1000;
    }
    if (sp3 < 0) {
        sp3 = 1000;
    }

    for (int i = 0; i < myProxyModel->columnCount() ; i++) {
        if ( i >= sp1 ) myTableView->setColumnHidden(i, true);
    }
    for (int i = 0; i < myProxyModel->columnCount() ; i++) {
        if ( i < sp1 || i >= sp2 )
            if (tableView_1 != NULL)tableView_1->setColumnHidden(i, true);
    }
    for (int i = 0; i < myProxyModel->columnCount() ; i++) {
        if ( i < sp2 || i >= sp3 )
            if (tableView_2 != NULL)tableView_2->setColumnHidden(i, true);
    }
    for (int i = 0; i < myProxyModel->columnCount() ; i++) {
        if ( i < sp3 )
            if (tableView_3 != NULL)tableView_3->setColumnHidden(i, true);
    }

    QHash<int, QStringList> lmlist = mySectionType.getLimitedStrings();
    Q_FOREACH (int cc, lmlist.keys()) {
        QStringList stl = lmlist.value(cc);
        QStyledItemDelegate *deleg;
        if ( mySectionType.getShapeTypeColumns().contains(cc - 1) ) {
            deleg = new StandardSectionDelegate;
        } else {
            deleg = new ComboBoxDelegate(stl);
        }
        if ( cc < sp1 ) {
            myTableView->setItemDelegateForColumn(cc, deleg);
        } else if ( sp1 <= cc && cc < sp2 ) {
            tableView_1->setItemDelegateForColumn(cc, deleg);
        } else if ( sp2 <= cc && cc < sp3 ) {
            tableView_2->setItemDelegateForColumn(cc, deleg);
        } else if ( sp3 <= cc ) {
            tableView_3->setItemDelegateForColumn(cc, deleg);
        }
        if ( !mySectionType.getShapeTypeColumns().contains(cc - 1) ) {
            myTableModel->setLimitedStrings(cc, stl);
        }
    }

    QHash<int, DATATYPE> refcol = mySectionType.getReferenceColumns();
    Q_FOREACH ( int cc, refcol.keys() ) {
        ReferenceComboBoxDelegate *deleg = new ReferenceComboBoxDelegate(refcol.value(cc));
        if ( cc < sp1 ) {
            myTableView->setItemDelegateForColumn(cc, deleg);
        } else if ( sp1 <= cc && cc < sp2 ) {
            tableView_1->setItemDelegateForColumn(cc, deleg);
        } else if ( sp2 <= cc && cc < sp3 ) {
            tableView_2->setItemDelegateForColumn(cc, deleg);
        } else if ( sp3 <= cc ) {
            tableView_3->setItemDelegateForColumn(cc, deleg);
        }
    }
}

QWidget *SectionTableWidget::createTableLabel(CustomTableView *tv, const QString &str)
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

void SectionTableWidget::setLineButtonsSpinBox()
{
    int min = (mySpinBox->value() < 1) ? 0 : 1 ;
    if (lineButtons != nullptr)lineButtons->setSpinBoxMinMax(min, mySpinBox->value());
}

void SectionTableWidget::modelUpload()
{
    myTableModel->keepCurrentData();
    undoStack->clear();
}

void SectionTableWidget::modelRewind()
{
    myTableModel->backPreviousData();
    slotManageTableData(QModelIndex(), QModelIndex());
    undoStack->clear();
    emit sectionItemChanged();
}


void SectionTableWidget::slotChangeCurrentRow(int row)
{
    //row　：　移動先のmodelのrow
    //idx : 移動前のviewのindex
    //idx_a:移動先のviewのindex
    if ( row < 0 || row >= myTableModel->rowCount() ) return;

    QModelIndex idx = myTableView->currentIndex();
    QModelIndex idx_a = myProxyModel->mapFromSource(myTableModel->index(row, idx.column()));

    int sp1 = mySectionType.getSeparateColumn1();
    int sp2 = mySectionType.getSeparateColumn2();
    int sp3 = mySectionType.getSeparateColumn3();
    if (sp1 < 0) {
        sp1 = 1000;   //1000 is large number
    }
    if (sp2 < 0) {
        sp2 = 1000;
    }
    if (sp3 < 0) {
        sp3 = 1000;
    }

    if ( idx.column() < sp1 ) {
        myTableView->setCurrentIndex(idx_a);
        myTableView->setFocus();
    } else if ( sp1 <= idx.column() && idx.column() < sp2 ) {
        tableView_1->setCurrentIndex(idx_a);
        tableView_1->setFocus();
    } else if ( sp2 <= idx.column() && idx.column() < sp3 ) {
        tableView_2->setCurrentIndex(idx_a);
        tableView_2->setFocus();
    } else if ( sp3 <= idx.column() ) {
        tableView_3->setCurrentIndex(idx_a);
        tableView_3->setFocus();
    }
    QList<QVariant> vlist = myTableModel->getRowData(row);
    SectionValues section = this->getMemberSection(row);

    if (lineButtons != nullptr)lineButtons->setLineValues(row, vlist);
    if (lineDraw != nullptr)lineDraw->setLineValues(vlist, section);
    if (lineCalc != nullptr)lineCalc->setLineValues(vlist, section);
    qDebug()<<"slotchangecurrent row done";
}

void SectionTableWidget::slotLineViewChange(int row)
{
    if ( row < 0 || row >= myTableModel->rowCount() ) return;

    QModelIndex tmp_idx = myProxyModel->mapToSource(myProxyModel->index(row, 0));
    int t_row = myTableModel->data(tmp_idx, Qt::DisplayRole).toInt() - 1;

    QList<QVariant> vlist = myTableModel->getRowData(t_row);
    SectionValues section = this->getMemberSection(t_row);

    if (lineButtons != nullptr)lineButtons->setLineValues(t_row, vlist);
    if (lineDraw != nullptr)lineDraw->setLineValues(vlist, section);
    if (lineCalc != nullptr)lineCalc->setLineValues(vlist, section);
}

void SectionTableWidget::slotManageTableData(const QModelIndex &idx_a, const QModelIndex &idx_b)
{
    if ( idx_a.column() > 0 || idx_b.column() > 0 ) emit sectionItemChanged();

    QModelIndex cur_idx = myTableView->currentIndex();
    QModelIndex p_idx = myProxyModel->mapToSource(cur_idx);

    if (tableView_1 != nullptr)tableView_1->reset();
    if (tableView_2 != nullptr)tableView_2->reset();
    if (tableView_3 != nullptr)tableView_3->reset();
    myTableView->reset();

    int sp1 = mySectionType.getSeparateColumn1();
    int sp2 = mySectionType.getSeparateColumn2();
    int sp3 = mySectionType.getSeparateColumn3();
    if (sp1 < 0) {
        sp1 = 1000;   //1000 is large number
    }
    if (sp2 < 0) {
        sp2 = 1000;
    }
    if (sp3 < 0) {
        sp3 = 1000;
    }

    if ( cur_idx.column() < sp1 ) {
        myTableView->setCurrentIndex(cur_idx);
        myTableView->setFocus();
    } else if ( sp1 <= cur_idx.column() && cur_idx.column() < sp2 ) {
        tableView_1->setCurrentIndex(cur_idx);
        tableView_1->setFocus();
    } else if ( sp2 <= cur_idx.column() && cur_idx.column() < sp3 ) {
        tableView_2->setCurrentIndex(cur_idx);
        tableView_2->setFocus();
    } else if ( sp3 <= cur_idx.column() ) {
        tableView_3->setCurrentIndex(cur_idx);
        tableView_3->setFocus();
    }

    int row = p_idx.row();
    if (!p_idx.isValid()
            && lineButtons != nullptr)row =
                    lineButtons->currentRow(); //セルが選択されてないとき、linebuttonsの表示に対応する行
    else if (!p_idx.isValid()
             && lineButtons == nullptr)row =
                     0; //セルが選択されておらずlinebuttonsがNULLのとき、0行目
    QList<QVariant> vlist = myTableModel->getRowData(row);
    SectionValues section = this->getMemberSection(row);

    if (lineButtons != nullptr)lineButtons->setLineValues(row, vlist);
    if (lineDraw != nullptr)lineDraw->setLineValues(vlist, section);
    if (lineCalc != nullptr)lineCalc->setLineValues(vlist, section);
}

void SectionTableWidget::changeCurrentView()
{
    int viewcount = 0;
    if (myTableView != nullptr)viewcount++;
    if (tableView_1 != nullptr)viewcount++;
    if (tableView_2 != nullptr)viewcount++;
    if (tableView_3 != nullptr)viewcount++;

    QModelIndex idx = myTableView->currentIndex();
    int row = idx.row();
    int col = idx.column();

    int sp1 = mySectionType.getSeparateColumn1();
    int sp2 = mySectionType.getSeparateColumn2();
    int sp3 = mySectionType.getSeparateColumn3();
    if (sp1 < 0) {
        sp1 = 1000;   //1000 is large number
    }
    if (sp2 < 0) {
        sp2 = 1000;
    }
    if (sp3 < 0) {
        sp3 = 1000;
    }

    QList<int> mfirstcolumns = ((SectionTableView *)myTableView)->getMiddleViewFirstColumns();
    int firstcolumn = 0; // ((SectionTableView*)myTableView)->getFirstColumn();
    QModelIndex idx1;
    if ( col < sp1 ) {
        int nextfirstcolumn = ( viewcount == 1 ? firstcolumn : mfirstcolumns.at(0) );
        if ( viewcount == 1 && row < myProxyModel->rowCount() - 1 ) row++;
        idx1 = myProxyModel->index(row, nextfirstcolumn);
    } else if ( sp1 <= col && col < sp2 ) {
        int nextfirstcolumn = ( viewcount == 2 ? firstcolumn : mfirstcolumns.at(1) );
        if ( viewcount == 2 && row < myProxyModel->rowCount() - 1 ) row++;
        idx1 = myProxyModel->index(row, nextfirstcolumn);
    } else if ( sp2 <= col && col < sp3 ) {
        int nextfirstcolumn = ( viewcount == 3 ? firstcolumn : mfirstcolumns.at(2) );
        if ( viewcount == 3 && row < myProxyModel->rowCount() - 1 ) row++;
        idx1 = myProxyModel->index(row, nextfirstcolumn);
    } else if ( sp3 <= col ) {
        int nextfirstcolumn = ( viewcount == 4 ? firstcolumn : mfirstcolumns.at(3) );
        if ( viewcount == 4 && row < myProxyModel->rowCount() - 1 ) row++;
        idx1 = myProxyModel->index(row, nextfirstcolumn);
    }

    QAbstractItemView::SelectionBehavior behavor
        = ( idx1.column() == 0 ) ? QAbstractItemView::SelectRows : QAbstractItemView::SelectItems;

    if (idx1.column() < sp1) {
        myTableView->setCurrentIndex(idx1);
        myTableView->setFocus();
        myTableView->setSelectionBehavior(behavor);
    } else if (idx1.column() >= sp1 && idx1.column() < sp2) {
        tableView_1->setCurrentIndex(idx1);
        tableView_1->setFocus();
        tableView_1->setSelectionBehavior(behavor);
    } else if (idx1.column() >= sp2 && idx1.column() < sp3) {
        tableView_2->setCurrentIndex(idx1);
        tableView_2->setFocus();
        tableView_2->setSelectionBehavior(behavor);
    } else if (idx1.column() >= sp3) {
        tableView_3->setCurrentIndex(idx1);
        tableView_3->setFocus();
        tableView_3->setSelectionBehavior(behavor);
    }
}

void SectionTableWidget::changeCurrentViewBack()
{
    QModelIndex idx = myTableView->currentIndex();
    int row = idx.row();
    int col = idx.column();

    int sp1 = mySectionType.getSeparateColumn1();
    int sp2 = mySectionType.getSeparateColumn2();
    int sp3 = mySectionType.getSeparateColumn3();
    if (sp1 < 0) {
        sp1 = 1000;   //1000 is large number
    }
    if (sp2 < 0) {
        sp2 = 1000;
    }
    if (sp3 < 0) {
        sp3 = 1000;
    }
    QList<int> mlastcolumns = ((SectionTableView *)myTableView)->getMiddleViewLastColumns();
    int lastcolumn = ((SectionTableView *)myTableView)->getLastColumn();

    QModelIndex idx1;
    if ( col < sp1 ) {
        if ( row > 0 ) row--;
        idx1 = myProxyModel->index(row, lastcolumn);
    } else if ( sp1 <= col && col < sp2 ) {
        idx1 = myProxyModel->index(row, mlastcolumns.at(0));
    } else if ( sp2 <= col && col < sp3 ) {
        idx1 = myProxyModel->index(row, mlastcolumns.at(1));
    } else if ( sp3 <= col ) {
        idx1 = myProxyModel->index(row, mlastcolumns.at(2));
    }

    QAbstractItemView::SelectionBehavior behavor
        = ( idx1.column() == 0 ) ? QAbstractItemView::SelectRows : QAbstractItemView::SelectItems;

    if (idx1.column() < sp1) {
        myTableView->setCurrentIndex(idx1);
        myTableView->setFocus();
        myTableView->setSelectionBehavior(behavor);
    } else if (idx1.column() >= sp1 && idx1.column() < sp2) {
        tableView_1->setCurrentIndex(idx1);
        tableView_1->setFocus();
        tableView_1->setSelectionBehavior(behavor);
    } else if (idx1.column() >= sp2 && idx1.column() < sp3) {
        tableView_2->setCurrentIndex(idx1);
        tableView_2->setFocus();
        tableView_2->setSelectionBehavior(behavor);
    } else if (idx1.column() >= sp3) {
        tableView_3->setCurrentIndex(idx1);
        tableView_3->setFocus();
        tableView_3->setSelectionBehavior(behavor);
    }
}

void SectionTableWidget::setDataMyModel(int row, int col, const QString &str)
{
    if ( row < 0 || col < 0 || row >= myTableModel->rowCount()
            || col >= myTableModel->columnCount() ) return;

    QModelIndex idx = myTableModel->index(row, col);
    QVariant b_var = QVariant(str);
    myTableModel->setData(idx, b_var, Qt::EditRole);
    if (lineButtons != nullptr)lineButtons->setButtonFocus(col);
}

void SectionTableWidget::slotChangeTableRowCount(int value)
{
    CustomTableWidget::slotChangeTableRowCount(value);
    setLineButtonsSpinBox();
}

void SectionTableWidget::slotChangeSpinBoxValue(int value)
{
    CustomTableWidget::slotChangeSpinBoxValue(value);
    setLineButtonsSpinBox();
}
void SectionTableWidget::setDefaultValuePatch(int row, int col, const QVariant &dvalue)
{
    mySectionType.setDefaultValuePatch(row, col, dvalue);
    ((SectionTableModel *)myTableModel)->setDefaultValuePatch(row, col, dvalue);

    slotManageTableData(QModelIndex(), QModelIndex());
}

void SectionTableWidget::setFilterColumns(const QList<bool> &_filter)
{
    QList<bool> filter = _filter;
    if (filter.count() != myTableModel->columnCount()) {
        filter.clear();
        for (int i = 0; i < myTableModel->columnCount(); ++i)filter << true;
    }//filterの要素数がモデルのコラム数と違う場合は全部trueにする
    int sp1 = mySectionType.getSeparateColumn1();
    int sp2 = mySectionType.getSeparateColumn2();
    int sp3 = mySectionType.getSeparateColumn3();
    if (sp1 < 0)sp1 = 1000;
    if (sp2 < 0)sp2 = 1000;
    if (sp3 < 0)sp3 = 1000; //←適当な大きい数（＞コラム数）
    for (int i = 0; i < filter.count(); ++i) {
        if (i < sp1) {
            if (filter[i])myTableView->showColumn(i);
            if (!filter[i])myTableView->hideColumn(i);
        } else if (i >= sp1 && i < sp2) {
            if (filter[i])tableView_1->showColumn(i);
            if (!filter[i])tableView_1->hideColumn(i);
        } else if (i >= sp2 && i < sp3) {
            if (filter[i])tableView_2->showColumn(i);
            if (!filter[i])tableView_2->hideColumn(i);
        } else if (i >= sp3) {
            if (filter[i])tableView_3->showColumn(i);
            if (!filter[i])tableView_3->hideColumn(i);
        }
    }
    if (myTableView != nullptr)myTableView->reset();
    if (tableView_1 != nullptr)tableView_1->reset();
    if (tableView_2 != nullptr)tableView_2->reset();
    if (tableView_3 != nullptr)tableView_3->reset();
    filter_columns = filter;
    emit filterColumnsChanged(filter);
}

void SectionTableWidget::setTableViewBorderColumns(const SectionDataType &sectiontype,
                                                   const QList<bool> &filters)
{
    ((SectionTableView *)myTableView)->setTableViewBorderColumns(sectiontype, filters);
    if (tableView_1 != nullptr)tableView_1->setTableViewBorderColumns(sectiontype, filters);
    if (tableView_2 != nullptr)tableView_2->setTableViewBorderColumns(sectiontype, filters);
    if (tableView_3 != nullptr)tableView_3->setTableViewBorderColumns(sectiontype, filters);
}

void SectionTableWidget::slotDeleteLine()
{
    CustomTableWidget::slotDeleteLine();
    emit sectionItemChanged();
}

void SectionTableWidget::paste()
{
    DATATYPE mtype = mySectionType.getMemberType();
    STRUCTTYPE stype = mySectionType.getStructType();
    bool isS = ( stype == STRUCTTYPE::STP_S || stype == STRUCTTYPE::STP_SRC || stype == STRUCTTYPE::STP_CFT );
    bool isRC = ( stype == STRUCTTYPE::STP_RC || stype == STRUCTTYPE::STP_SRC );
    if ( mtype == DATATYPE::TPBASE ) {
        isS = false;
        isRC = false;
    }
    bool isBASE = ( mtype == DATATYPE::TPBASE && stype == STRUCTTYPE::STP_S ) ;
    bool isISO = ( mtype == DATATYPE::TPISO );
    this->pasteSectionData(isS, isRC, isBASE, isISO);
}

void SectionTableWidget::del()
{
    DATATYPE mtype = mySectionType.getMemberType();
    STRUCTTYPE stype = mySectionType.getStructType();
    bool isS = ( stype == STRUCTTYPE::STP_S || stype == STRUCTTYPE::STP_SRC || stype == STRUCTTYPE::STP_CFT );
    bool isRC = ( stype == STRUCTTYPE::STP_RC || stype == STRUCTTYPE::STP_SRC );
    if ( mtype == DATATYPE::TPBASE ) {
        isS = false;
        isRC = false;
    }
    bool isBASE = ( mtype == DATATYPE::TPBASE && stype == STRUCTTYPE::STP_S ) ;
    bool isISO = ( mtype == DATATYPE::TPISO );
    this->deleteSectionData(isS, isRC, isBASE, isISO);
}

void SectionTableWidget::slotEditItem(const QModelIndex &idx, const QVariant &var)
{
//    if( var.toString()==myTableModel->data(idx,Qt::DisplayRole).toString() ) return;
//    qDebug() << var << myTableModel->data(idx,Qt::DisplayRole);
    if ( var.toString() == myTableModel->getData(idx.row(), idx.column()).toString() ) return;

    DATATYPE mtype = mySectionType.getMemberType();
    STRUCTTYPE stype = mySectionType.getStructType();
    bool isS = ( stype == STRUCTTYPE::STP_S || stype == STRUCTTYPE::STP_SRC || stype == STRUCTTYPE::STP_CFT );
    bool isRC = ( stype == STRUCTTYPE::STP_RC || stype == STRUCTTYPE::STP_SRC );
    if ( mtype == DATATYPE::TPBASE ) {
        isS = false;
        isRC = false;
    }
    bool isBASE = ( mtype == DATATYPE::TPBASE && stype == STRUCTTYPE::STP_S ) ;
    bool isISO = ( mtype == DATATYPE::TPISO );
    this->editSectionData(idx, var, isS, isRC, isBASE, isISO);
}

void SectionTableWidget::pasteSectionData(bool isS, bool isRC, bool isBASE, bool isISO)
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
    Q_FOREACH ( QModelIndex idx, idxlist ) {
        if (isS) inputStandardValuesS( idx, i_idxlist, i_varlist );
        if (isRC) inputStandardValuesRC( idx );
        if (isBASE) inputStandardValuesBASE( idx, i_idxlist, i_varlist );
        if (isISO) inputStandardValuesISO( idx, i_idxlist, i_varlist );
    }
    if ( !i_idxlist.isEmpty() ) {
        QUndoCommand *i_setCommand = new SetMultiCommand(myTableModel, i_idxlist, i_varlist);
        undoStack->push(i_setCommand);
    }

    undoStack->endMacro();

    if ( !undoOnOff ) {
        undoStack->clear();
    }
}

void SectionTableWidget::deleteSectionData(bool isS, bool isRC, bool isBASE, bool isISO)
{
    if (!mySelectionModel->hasSelection()) return;
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
    Q_FOREACH ( QModelIndex idx, idxlist ) {
        if (isS) inputStandardValuesS( idx, i_idxlist, i_varlist );
        if (isRC) inputStandardValuesRC( idx );
        if (isBASE) inputStandardValuesBASE( idx, i_idxlist, i_varlist );
        if (isISO) inputStandardValuesISO( idx, i_idxlist, i_varlist );
    }
    if ( !i_idxlist.isEmpty() ) {
        QUndoCommand *i_setCommand = new SetMultiCommand(myTableModel, i_idxlist, i_varlist);
        undoStack->push(i_setCommand);
    }

    undoStack->endMacro();

    if ( !undoOnOff ) {
        undoStack->clear();
    }
}

void SectionTableWidget::editSectionData(const QModelIndex &idx, const QVariant &var,
                                         bool isS, bool isRC, bool isBASE, bool isISO)
{
    undoStack->beginMacro("sectionTable editItem");

    QModelIndexList idxlist;
    QList<QVariant> varlist;
    idxlist.append( idx );
    DATATYPE type = myTableModel->dataType(idx);
    if ( type == DATATYPE::TPFLOOR || type == DATATYPE::TPFRAMEG || type == DATATYPE::TPFRAMEP ) {
        QString str = UnifiedInputData::getInInstance()->idToName(var.toString(), type);
        ( str.isEmpty() ) ? varlist.append( QVariant() ) : varlist.append( QVariant(str) );
    } else {
        varlist.append( var );
    }
    QUndoCommand *setCommand = new SetMultiCommand(myTableModel, idxlist, varlist);
    undoStack->push(setCommand);

    QModelIndexList i_idxlist;
    QList<QVariant> i_varlist;
    if (isS) inputStandardValuesS( idx, i_idxlist, i_varlist );
    if (isRC) inputStandardValuesRC( idx );
    if (isBASE) inputStandardValuesBASE( idx, i_idxlist, i_varlist );
    if (isISO) inputStandardValuesISO( idx, i_idxlist, i_varlist );
    if ( !i_idxlist.isEmpty() ) {
        QUndoCommand *i_setCommand = new SetMultiCommand(myTableModel, i_idxlist, i_varlist);
        undoStack->push(i_setCommand);
    }

    undoStack->endMacro();

    if ( !undoOnOff ) {
        undoStack->clear();
    }
}


void SectionTableWidget::inputStandardValuesS(const QModelIndex &idx,
                                              QModelIndexList &idxlist, QList<QVariant> &varlist)
{
    int row = idx.row();
    int col = idx.column();
    QString str = idx.data(Qt::EditRole).toString();
    if ( mySectionType.getShapeTypeColumns().contains(col) ) {
        QString sname = myTableModel->index(row, col + 1).data().toString();
        if ( !mySectionType.standardSectionNameList(str).contains(sname) ) {
            idxlist.append( myTableModel->index(row, col + 1) );
            varlist.append( QVariant("") );
            if ( mySectionType.standardSectionFileList(StandardSectionTable::listType::STEEL).contains(str) ) {
                for ( int i = 2; i < 6; i++) {
                    idxlist.append( myTableModel->index(row, col + i) );
                    varlist.append( QVariant("") );
                }
            }
        }
    } else if ( mySectionType.getShapeTypeColumns().contains(col - 1) ) {
        QString fname = myTableModel->index(row, col - 1).data().toString();
        if ( !mySectionType.standardSectionFileList(StandardSectionTable::listType::STEEL).contains(fname) ) return;
        if ( str.isEmpty() ) {
            for ( int i = 1; i < 5; i++) {
                idxlist.append( myTableModel->index(row, col + i) );
                varlist.append( QVariant("") );
            }
            return;
        }
        if ( str.left(1) == "H" ) {
            str = str.split('-').at(1); // str.remove(0,2);
            QStringList list = str.split('x');
            if (list.count() < 4)return;
            for ( int i = 0; i < 4; i++) {
                idxlist.append( myTableModel->index(row, col + i + 1) );
                varlist.append( QVariant(list.at(i)) );
            }
            return;
        }
        if ( str.left(2) == "BX" ) {
            str = str.split('-').at(1); // str.remove(0,3);
            QStringList list = str.split('x');
            if (list.count() < 3)return;
            for ( int i = 0; i < 3; i++) {
                idxlist.append( myTableModel->index(row, col + i + 1) );
                varlist.append( QVariant(list.at(i)) );
            }
            idxlist.append( myTableModel->index(row, col + 4) );
            varlist.append( QVariant("") );
            return;
        }
        if ( str.left(1) == "P" ) {
            str = str.split('-').at(1); // str.remove(0,2);
            QStringList list = str.split('x');
            if (list.count() < 2)return;
            idxlist.append( myTableModel->index(row, col + 1) );
            varlist.append( QVariant(list.at(0)) );
            idxlist.append( myTableModel->index(row, col + 2) );
            varlist.append( QVariant("") );
            idxlist.append( myTableModel->index(row, col + 3) );
            varlist.append( QVariant(list.at(1)) );
            idxlist.append( myTableModel->index(row, col + 4) );
            varlist.append( QVariant("") );
            return;
        }
        for ( int i = 1; i < 5; i++) {
            idxlist.append( myTableModel->index(row, col + i) );
            varlist.append( QVariant("") );
        }
    }
}

void SectionTableWidget::inputStandardValuesRC(const QModelIndex &idx)
{
    int row = idx.row();
    int col = idx.column();
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    int tmpBD;
    if (col == coltag["iMAINBARPHI"]) {
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "a3");
        setDefaultValuePatch(row, coltag["iRKU"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "a1");
        setDefaultValuePatch(row, coltag["iRK"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "a3");
        setDefaultValuePatch(row, coltag["iRKB"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "b1");
        setDefaultValuePatch(row, coltag["iRY"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
    }
    if (col == coltag["mMAINBARPHI"]) {
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "a3");
        setDefaultValuePatch(row, coltag["mRKU"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "a1");
        setDefaultValuePatch(row, coltag["mRK"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "a3");
        setDefaultValuePatch(row, coltag["mRKB"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "b1");
        setDefaultValuePatch(row, coltag["mRY"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
    }
    if (col == coltag["jMAINBARPHI"]) {
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "a3");
        setDefaultValuePatch(row, coltag["jRKU"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "a1");
        setDefaultValuePatch(row, coltag["jRK"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "a3");
        setDefaultValuePatch(row, coltag["jRKB"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getBarDistance(idx.data().toString(), "b1");
        setDefaultValuePatch(row, coltag["jRY"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
    }
//    slotTableViewReset();
}

void SectionTableWidget::inputStandardValuesBASE(const QModelIndex &idx,
                                                 QModelIndexList &idxlist, QList<QVariant> &varlist)
{
    int row = idx.row();
    int col = idx.column();
    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    int tmpBD;
    if (col == coltag["BOLTPHI"]) {
        tmpBD = SectionCalculations::getDefaultBoltLength(idx.data().toInt());
        setDefaultValuePatch(row, coltag["BOLTLENGTH"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getDefaultBoltHolePhi(idx.data().toInt());
        setDefaultValuePatch(row, coltag["BOLTHOLEPHI"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
        tmpBD = SectionCalculations::getDefaultScrewPhi(idx.data().toInt());
        setDefaultValuePatch(row, coltag["SCREWPHI"], ((tmpBD == 0) ? QVariant() : QVariant(tmpBD)));
    } else if (col == coltag["BC"] || col == coltag["DC"]) {
        double bc = myTableModel->index(row, coltag["BC"]).data().toDouble();
        double dc = myTableModel->index(row, coltag["DC"]).data().toDouble();
        setDefaultValuePatch(row, coltag["PROJECTEDAREA"],
                             ((int(bc * dc) == 0) ? QVariant() : QVariant(bc * dc)));
    }

    QString str = idx.data(Qt::EditRole).toString();
    if ( mySectionType.getShapeTypeColumns().contains(col)) {
        if (!mySectionType.standardSectionNameList(str).contains(myTableModel->index(row,
                                                                                     col + 1).data().toString())
                || myTableModel->index(row, col + 1).data().toString() == "") {
            idxlist.append( myTableModel->index(row, col + 1) );
            varlist.append( QVariant("") );
            if ( mySectionType.standardSectionFileList(StandardSectionTable::listType::BASE).contains(str) ) {
                for (int i = col + 2; i < myTableModel->columnCount(); ++i) {
                    idxlist.append( myTableModel->index(row, i) );
                    varlist.append( QVariant("") );
                }
            }
        }
    }
//    slotTableViewReset();
}

void SectionTableWidget::inputStandardValuesISO(const QModelIndex &idx,
                                                QModelIndexList &idxlist, QList<QVariant> &varlist)
{
    int row = idx.row();
    int col = idx.column();
    QString str = idx.data(Qt::EditRole).toString();
    if ( mySectionType.getShapeTypeColumns().contains(col)) {
        if (!mySectionType.standardSectionNameList(str).contains(myTableModel->index(row,
                                                                                     col + 1).data().toString())
                || myTableModel->index(row, col + 1).data().toString() == "") {
            idxlist.append( myTableModel->index(row, col + 1) );
            varlist.append( QVariant("") );
            if ( mySectionType.standardSectionFileList(StandardSectionTable::listType::ISOLATOR).contains(str) ) {
                for (int i = 2; i < 14; i++) {
                    idxlist.append( myTableModel->index(row, col + i) );
                    varlist.append( QVariant("") );
                }
            }
        }
    } else if ( mySectionType.getShapeTypeColumns().contains(col - 1) ) {
        QString fname = myTableModel->index(row, col - 1).data().toString();
        if ( !mySectionType.standardSectionFileList(StandardSectionTable::listType::ISOLATOR).contains(
                    fname) ) return;
        if ( str.isEmpty() ) {
            for (int i = 1; i < 13; i++) {
                idxlist.append( myTableModel->index(row, col + i) );
                varlist.append( QVariant("") );
            }
            return;
        }

        if ( mySectionType.standardSectionNameExists(str) ) {
            for ( int i = 1; i <= static_cast<int>(StandardSectionTable::IsolatorFormat::IS_Ft) ; i++ ) {
                StandardSectionTable::IsolatorFormat fm
                    = static_cast<StandardSectionTable::IsolatorFormat>(i);
                QString stt = mySectionType.standardIsolatorSectionValue(str, fm);
                idxlist.append( myTableModel->index(row, col + i) );
                varlist.append( QVariant(stt) );
            }
            return;
        }
        for ( int i = 1; i < 13; i++) {
            idxlist.append( myTableModel->index(row, col + i) );
            varlist.append( QVariant("") );
        }
    }
}

bool SectionTableWidget::checkBinaryFile(QDataStream &infile) const{
    if (infile.atEnd()) return false;
    int filterNumber;
    infile >> filterNumber;
    QList<bool> filters;
    infile >> filters;
    return CustomTableWidget::checkBinaryFile(infile);
}


void SectionTableWidget::readBinaryFile(QDataStream &infile)
{
    int filterCount;
    infile >> filterCount;
    QList<bool> filters;
    infile >> filters;
    setFilterColumns(filters);
    CustomTableWidget::readBinaryFile(infile);
    slotChangeCurrentRow(0); // 開いた瞬間に0行目を選択していてほしい
}
void SectionTableWidget::writeBinaryFile(QDataStream &outfile)
{
    outfile << filter_columns.count();
    outfile << filter_columns;
    CustomTableWidget::writeBinaryFile(outfile);
}

SectionValues SectionTableWidget::getMemberSection(int row) const
{
    return ((SectionTableModel *)myTableModel)->getMemberSection(row);
}

QList<SectionValues> SectionTableWidget::getSectionList() const
{
    return ((SectionTableModel *)myTableModel)->getSectionList();
}
} // namespace post3dapp
