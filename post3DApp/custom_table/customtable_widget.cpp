#include "customtable_widget.h"

#include <QtGui>
#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QSpinBox>

#include "combobox_delegate.h"
#include "customtable_commands.h"
#include "customtable_model.h"
#include "customtable_view.h"
#include "sortfilter_proxymodel.h"
#include "unified_editingdata.h"
#include "utility.h"


namespace post3dapp{
CustomTableWidget::CustomTableWidget( const CustomTableDataType &type, QWidget *parent )
    : QWidget( parent ), myType(type)
{
    this->setFont(QFont(tr("Microsoft JhengHei"), 9));
    filterWidgetsVisible = true;
    undoOnOff = true;
}

void CustomTableWidget::createActions()
{
    copyAction = new QAction(u8"コピー", this);
    copyAction->setIcon(QIcon(":/icons/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(copyAction);
    connect ( copyAction, &QAction::triggered, this, &CustomTableWidget::copy );
    cutAction = new QAction(u8"切り取り", this);
    cutAction->setIcon(QIcon(":/icons/cut.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(cutAction);
    connect ( cutAction, &QAction::triggered, this, &CustomTableWidget::cut );
    pasteAction = new QAction(u8"貼り付け", this);
    pasteAction->setIcon(QIcon(":/icons/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(pasteAction);
    connect ( pasteAction, &QAction::triggered, this, &CustomTableWidget::paste );

    deleteAction = new QAction(u8"行削除", this);
    deleteAction->setShortcut(tr("Ctrl+D"));
    deleteAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(deleteAction);
    connect ( deleteAction, &QAction::triggered, this, &CustomTableWidget::slotDeleteLine );
    insertAction = new QAction(u8"行挿入", this);
    insertAction->setShortcut(tr("Ctrl+I"));
    insertAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(insertAction);
    connect ( insertAction, &QAction::triggered, this, &CustomTableWidget::slotInsertLine );

    upAction = new QAction(u8"行移動（上へ）", this);
    upAction->setIcon(QIcon(":/icons/up.png"));
    upAction->setShortcut(tr("Ctrl+H"));
    upAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(upAction);
    connect ( upAction, &QAction::triggered, this, &CustomTableWidget::slotMoveUp );
    downAction = new QAction(u8"行移動（下へ）", this);
    downAction->setIcon(QIcon(":/icons/down.png"));
    downAction->setShortcut(tr("Ctrl+L"));
    downAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(downAction);
    connect ( downAction, &QAction::triggered, this, &CustomTableWidget::slotMoveDown );

    undoAction = undoStack->createUndoAction(this, u8"元に戻す（Undo）");
    undoAction->setIcon(QIcon(":/icons/undo.png"));
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(undoAction);
    redoAction = undoStack->createRedoAction(this, u8"やり直し（Redo）");
    redoAction->setIcon(QIcon(":/icons/redo.png"));
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(redoAction);

    myMenu->addActions(myActionList);
    myMenu->insertSeparator(deleteAction);
    myMenu->insertSeparator(upAction);
    myMenu->insertSeparator(undoAction);
}

void CustomTableWidget::slotRowActionEnable(bool on)
{
    deleteAction->setEnabled(on);
    insertAction->setEnabled(on);
    upAction->setEnabled(on);
    downAction->setEnabled(on);
}

void CustomTableWidget::createButtons()
{
    filter_combo = new QComboBox(this);
    QStringList itemlist = myType.getHeaderStrings();
    for ( int i = 0 ; i < itemlist.count() ; i++ ) {
        QString str = QString("%1: ").arg(i) + itemlist.at(i);
        str.replace("\n", " ");
        itemlist.replace(i, str);
    }

    itemlist.insert(0, u8"（列の指定）");
    filter_combo->addItems(itemlist);
    filter_combo->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );

    filter_ledit = new QLineEdit("", this);
    filter_ledit->setFixedWidth(120);
    QPushButton *filter_on_button = new QPushButton("GO", this);
    QPushButton *filter_reset_button = new QPushButton("Reset", this);
    filter_on_button->setAutoDefault(false);
    filter_reset_button->setAutoDefault(false);
    filter_on_button->setFixedWidth(50);
    filter_reset_button->setFixedWidth(50);

    connect ( filter_ledit, &QLineEdit::returnPressed, this, &CustomTableWidget::slotFilterOn);
    connect ( filter_on_button, &QAbstractButton::clicked, this, &CustomTableWidget::slotFilterOn);
    connect ( filter_reset_button, &QAbstractButton::clicked, this, &CustomTableWidget::slotFilterReset);


    filterWidgets = new QWidget(this);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setSpacing(3);
    hlay->setContentsMargins(0, 0, 0, 0);
    hlay->addStretch();
    hlay->addWidget(filter_combo);
    hlay->addSpacing(5);
    hlay->addWidget(new QLabel(u8"フィルター語句", this));
    hlay->addWidget(filter_ledit);
    hlay->addWidget(filter_on_button);
    hlay->addWidget(filter_reset_button);
    filterWidgets->setLayout(hlay);

    mySpinBoxLabel = new QLabel(u8"定義数：", this);
    mySpinBox = new QSpinBox(this);
    mySpinBox->setMinimum(0);
    mySpinBox->setMaximum(9999);
    mySpinBox->setValue(1);

    connect ( mySpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
              this, &CustomTableWidget::slotChangeTableRowCount );

    topLayout = new QHBoxLayout;
    topLayout->setSpacing(3);
    topLayout->setContentsMargins(5, 5, 5, 5);
    topLayout->addWidget(mySpinBoxLabel);
    topLayout->addWidget(mySpinBox);
    topLayout->addStretch();
    topLayout->addWidget(filterWidgets);
}

void CustomTableWidget::setSpinBoxVisible(bool onoff)
{
    mySpinBoxLabel->setVisible(onoff);
    mySpinBox->setVisible(onoff);
}

void CustomTableWidget::cut()
{
    copy();
    del();
}

void CustomTableWidget::copy()
{
    QModelIndexList idxlist = mySelectionModel->selectedIndexes();
    QMultiMap<int, QModelIndex> index_map;
    Q_FOREACH (QModelIndex idx, idxlist) index_map.insert(idx.row(), idx);
    QList<int> rowlist = index_map.uniqueKeys();
    QString str;
    for (int i = 0; i < rowlist.count(); ++i) {
        if (i > 0) str += "\n";
        QModelIndexList a_idxlist = index_map.values(rowlist[i]);
        QModelIndexList b_idxlist;
        Q_FOREACH (QModelIndex a_idx, a_idxlist) {
            int ii = 0;
            Q_FOREACH (QModelIndex b_idx, b_idxlist) {
                if ( a_idx.column() > b_idx.column() ) ii++;
            }
            b_idxlist.insert(ii, a_idx);
        }
        for ( int j = 0; j < b_idxlist.count() ; j++ ) {
            if (j > 0) str += "\t";
            str += myProxyModel->mapToSource(b_idxlist[j]).data(Qt::EditRole).toString();
        }
    }
    str += "\n";
    QApplication::clipboard()->setText(str);
}

void CustomTableWidget::paste()
{
    QModelIndex current_idx = myTableView->currentIndex();
    QString str = QApplication::clipboard()->text();
    QUndoCommand *pasteCommand = new PasteCommand(myProxyModel, current_idx, str);
    undoStack->push(pasteCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }
}

void CustomTableWidget::del()
{
    if (!mySelectionModel->hasSelection()) return;
    QModelIndexList idxlist = mySelectionModel->selectedIndexes();

    QUndoCommand *deleteCommand = new DeleteCommand(myProxyModel, idxlist);
    undoStack->push(deleteCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }
}

void CustomTableWidget::slotChangeTableRowCount(int value)
{
    if ( value == myTableModel->rowCount() ) return;

    QUndoCommand *setrowcountCommand = new SetRowCountCommand(myTableModel, value);
    undoStack->push(setrowcountCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }
}

void CustomTableWidget::slotChangeSpinBoxValue(int value)
{
    if ( value == mySpinBox->value() ) return;
    mySpinBox->setValue(value);
}
void CustomTableWidget::slotChangeSpinBoxMinimum(int value)
{
    mySpinBox->setMinimum(value);
}

void CustomTableWidget::slotInsertLine()
{
    QModelIndexList rows_idxlist = mySelectionModel->selectedRows();
    if ( rows_idxlist.isEmpty() ) return;

    int irow = rows_idxlist.first().row();
    bool ok = true;
    Q_FOREACH (QModelIndex idx, rows_idxlist) {
        if (idx.row() != irow) ok = false;
        irow++;
    }

    if ( !ok ) {
        warningSelectContinuousLine();
        return;
    }

    int start_row = rows_idxlist.first().data().toInt() - 1;
    QUndoCommand *insertLineCommand =
        new InsertLineCommand(myProxyModel, start_row, rows_idxlist.count());
    undoStack->push(insertLineCommand);
    if ( !undoOnOff ) {
        undoStack->clear();
    }

    mySelectionModel->clear();
//    myTableView->selectRow(start_row+rows_idxlist.count());
    QModelIndex s_idx = myTableModel->index(start_row + rows_idxlist.count(), 0);
    QModelIndex p_idx = myProxyModel->mapFromSource(s_idx);
    myTableView->selectRow(p_idx.row());
}

void CustomTableWidget::slotDeleteLine()
{
    QModelIndexList rows_idxlist = mySelectionModel->selectedRows();
    if ( rows_idxlist.isEmpty() ) return;
    int irow = rows_idxlist.first().row();
    bool ok = true;
    Q_FOREACH (QModelIndex idx, rows_idxlist) {
        if (idx.row() != irow) ok = false;
        irow++;
    }

    if ( !ok ) {
        warningSelectContinuousLine();
        return;
    }

    int start_row = rows_idxlist.first().data().toInt() - 1;
    int delete_count = rows_idxlist.count();
    if ( start_row + delete_count - 1 > myTableModel->rowCount() ) {
        Utility::showWarningMessage(this, u8"指定範囲を削除できません。");
        return;
    }
    QUndoCommand *deleteLineCommand =
        new DeleteLineCommand(myProxyModel, start_row, delete_count);
    undoStack->push(deleteLineCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }

    mySelectionModel->clear();
//    myTableView->selectRow(start_row);
    QModelIndex s_idx = myTableModel->index(start_row, 0);
    QModelIndex p_idx = myProxyModel->mapFromSource(s_idx);
    myTableView->selectRow(p_idx.row());
}

void CustomTableWidget::slotMoveUp()
{
    QModelIndexList rows_idxlist = mySelectionModel->selectedRows();
    if ( rows_idxlist.count() != 1 ) {
        warningSelectOneLine();
        return;
    }

    int a_row = rows_idxlist.first().data().toInt() - 1;
    if ( a_row < 1 ) return;

    QUndoCommand *moveLineCommand =
        new MoveLineCommand(myProxyModel, a_row, a_row - 1);
    undoStack->push(moveLineCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }

    mySelectionModel->clear();
    QModelIndex s_idx = myTableModel->index(a_row - 1, 0);
    QModelIndex p_idx = myProxyModel->mapFromSource(s_idx);
    myTableView->selectRow(p_idx.row());
}

void CustomTableWidget::slotMoveDown()
{
    QModelIndexList rows_idxlist = mySelectionModel->selectedRows();
    if (rows_idxlist.count() != 1) {
        warningSelectOneLine();
        return;
    }

    int a_row = rows_idxlist.first().data().toInt() - 1;
    if ( a_row >= myTableModel->rowCount() - 1 ) return;

    QUndoCommand *moveLineCommand =
        new MoveLineCommand(myProxyModel, a_row, a_row + 1);
    undoStack->push(moveLineCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }

    mySelectionModel->clear();
    QModelIndex s_idx = myTableModel->index(a_row + 1, 0);
    QModelIndex p_idx = myProxyModel->mapFromSource(s_idx);
    myTableView->selectRow(p_idx.row());
}

void CustomTableWidget::slotEditItem(const QModelIndex &idx, const QVariant &a_var)
{
//    qDebug() << a_var << myTableModel->data(idx,Qt::DisplayRole) << myTableModel->getData(idx.row(),idx.column());
//   if( a_var.toString()==myTableModel->data(idx,Qt::DisplayRole).toString() ) return;
    if ( a_var.toString() == myTableModel->getData(idx.row(), idx.column()).toString() ) return;

    QUndoCommand *setdataCommand = new SetDataCommand(myTableModel, idx, a_var);
    undoStack->push(setdataCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }
}

void CustomTableWidget::slotFilterOn()
{
    if (filter_combo->currentIndex() == 0) return;
    QString str = filter_ledit->text();
    if (str.isEmpty()) return;
    QRegExp regExp(str, Qt::CaseSensitive, QRegExp::RegExp );
    myProxyModel->setFilterColumn( filter_combo->currentIndex() - 1 );
    myProxyModel->setFilterRegExp(regExp);
    myTableModel->setFilterStatus(true);
}

void CustomTableWidget::slotFilterReset()
{
    QString str;
    filter_combo->setCurrentIndex(0);
    myProxyModel->setFilterRegExp(str);
    myTableModel->setFilterStatus(false);
}


bool CustomTableWidget::checkTextFile ( QTextStream &infile ) const
{
    return ( myTableModel->checkTextData(infile) );
}

bool CustomTableWidget::readTextFile ( QTextStream &infile,bool flg)
{
    if ( !myTableModel->readListTextData(infile, flg) ) return false;
    undoStack->clear();
    return true;
}

void CustomTableWidget::writeTextFile ( QTextStream &outfile)
{
    //outfile << mySpinBox->value()<<"\n";
    QList<int> list;
    list.append( myType.getTaggedColumn("UUID") );
    myTableModel->writeListTextData(outfile,list);
}

bool CustomTableWidget::checkBinaryFile ( QDataStream &infile ) const
{
    if ( infile.atEnd() ) return false;
    int vv;
    infile >> vv;
    return ( myTableModel->checkBinaryData(infile) );
}

void CustomTableWidget::readBinaryFile ( QDataStream &infile )
{
    int snum;
    infile >> snum;
    qDebug() << "snum=" << snum;
    myTableModel->initializeModel();
    mySpinBox->setValue(snum);
    myTableModel->readBinaryData(infile);
}

void CustomTableWidget::writeBinaryFile ( QDataStream &outfile )
{
    outfile << mySpinBox->value();
    myTableModel->writeBinaryData(outfile);
}

void CustomTableWidget::clearData()
{
    myTableModel->initializeModel();
    mySpinBox->setValue(1);
    myTableModel->setRowCount(1);
}

void CustomTableWidget::replaceMatchingName( const QString &str_a, const QString &str_b )
{
    myTableModel->replaceMatchingData( myType.nameColumn(), str_a, str_b );
    undoStack->clear();
}

void CustomTableWidget::warningSelectContinuousLine()
{
    Utility::showWarningMessage(this, u8"連続した行範囲を指定して下さい。");
}

void CustomTableWidget::warningSelectOneLine()
{
    Utility::showWarningMessage(this, u8"１行しか選択できません。");
}

CustomTableModel* CustomTableWidget::getTableModel() const{
    return myTableModel;
}

} // namespace post3dapp
