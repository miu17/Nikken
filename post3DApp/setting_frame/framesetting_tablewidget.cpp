#include "framesetting_tablewidget.h"

#include <QtGui>
#include <QApplication>
#include <QBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QSlider>
#include <QSplitter>
#include <QUndoStack>

#include "customtable_view.h"
#include "define_framesettingtable.h"
#include "framesetting_subtablewidget.h"
#include "framesetting_tablemodel.h"
#include "refercombobox_delegate.h"
#include "sortfilter_proxymodel.h"
#include "utility.h"

namespace post3dapp{
FrameSettingTableWidget::FrameSettingTableWidget( const CustomTableDataType &type, QWidget *parent )
    : CustomTableWidget( type, parent )
{
    myMenu = new QMenu(this);
    undoStack = new QUndoStack(this);

    createActions();
    createMyActions();
    createButtons();
    createModels();
    createViews();
    createWidgets();
    setFilterWidgetsVisible(false);
    //    setUndoOnOff(false);
    setSingnalOn();
}

void FrameSettingTableWidget::createMyActions()
{
    groupCopyAction = new QAction(u8"（通り情報ごと）コピー", this);
    groupCopyAction->setIcon(QIcon(":/icons/copy.png"));
    groupCopyAction->setShortcut(tr("Ctrl+G"));
    groupCopyAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(groupCopyAction);
    connect ( groupCopyAction, &QAction::triggered, this, &FrameSettingTableWidget::groupCopy );

    groupPasteAction = new QAction(u8"（通り情報ごと）貼り付け", this);
    groupPasteAction->setIcon(QIcon(":/icons/paste.png"));
    groupPasteAction->setShortcut(tr("Ctrl+W"));
    groupPasteAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(groupPasteAction);
    connect ( groupPasteAction, &QAction::triggered, this, &FrameSettingTableWidget::groupPaste );

    myMenu->insertAction(deleteAction, groupCopyAction);
    myMenu->insertAction(deleteAction, groupPasteAction);
    myMenu->insertSeparator(deleteAction);
}

void FrameSettingTableWidget::createModels()
{
    frameGroupModel = new FrameSettingTableModel(myType, this);
    myTableModel = frameGroupModel;

    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel = new QItemSelectionModel(myProxyModel);

    connect ( myTableModel, &CustomTableModel::rowCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxValue );
    connect ( myTableModel, &CustomTableModel::dataCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxMinimum );
    connect ( myTableModel, &QAbstractItemModel::dataChanged,
              this, &FrameSettingTableWidget::slotManageTableData );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &FrameSettingTableWidget::slotEditItem );
}

void FrameSettingTableWidget::createViews()
{
    myTableView = new CustomTableView(this);
    myTableView->setFirstColumn(myType.getTaggedColumn("FRAMEGROUP"));
    myTableView->setLastColumn(myType.getTaggedColumn("GID") - 1);

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth(myType.getTaggedColumn("NO"), 60);
    myTableView->setColumnWidth(myType.getTaggedColumn("FRAMEGROUP"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("FRAMETYPE"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("FRAMESUM"), 50);
    myTableView->setColumnWidth(myType.getTaggedColumn("OX"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("OY"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("STARTANGLE"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("ENDANGLE"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("DIVISIONS"), 50);
    myTableView->setColumnWidth(myType.getTaggedColumn("LENGTHPLUS"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("LENGTHMINUS"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("FLOORFROM"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("FLOORTO"), 70);
    myTableView->setColumnWidth(myType.getTaggedColumn("GID"), 120);
//    myTableView->setColumnWidth(myType.getTaggedColumn("UUID"),120);

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
    connect ( myTableView, QOverload<>::of(&CustomTableView::delKeyClicked), this, &FrameSettingTableWidget::del );
    connect ( myTableView, &CustomTableView::currentRowChanged,
              this, &FrameSettingTableWidget::slotChangeSubView );
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &FrameSettingTableWidget::slotRowActionEnable );

    myTableView->horizontalHeader()->setStretchLastSection(true);
}

void FrameSettingTableWidget::createWidgets()
{
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addLayout(topLayout, 0);
    vlay->addWidget(myTableView, 1);

    QWidget *tv_wid = new QWidget(this);
    tv_wid->setLayout(vlay);


    FrameSubDataType my_subtype;
    subTablewidget = new FrameSettingSubTableWidget(my_subtype, frameGroupModel, undoStack, this);

    connect ( subTablewidget, &FrameSettingSubTableWidget::frameCountChanged,
              this, &FrameSettingTableWidget::slotChangeFrameCount);

    connect ( subTablewidget, &FrameSettingSubTableWidget::frameItemChanged,
              this, &FrameSettingTableWidget::signalFrameItemChanged);

    QSplitter *splv = new QSplitter(Qt::Vertical, this);
    splv->addWidget(tv_wid);
    splv->addWidget(subTablewidget);

    QVBoxLayout *vlay2 = new QVBoxLayout;
    vlay2->setContentsMargins(0, 0, 0, 0);
    vlay2->addWidget(splv);
    this->setLayout(vlay2);
}

void FrameSettingTableWidget::slotRowActionEnable(bool on)
{
    groupCopyAction->setEnabled(on);
    groupPasteAction->setEnabled(on);
    CustomTableWidget::slotRowActionEnable(on);
}

void FrameSettingTableWidget::groupCopy()
{
    QString str;
    QModelIndexList rows_idxlist = mySelectionModel->selectedRows();
    str += QString("%1\n").arg(rows_idxlist.count());
    Q_FOREACH ( QModelIndex idx, rows_idxlist ) {
        int row = idx.data().toInt() - 1;
        QList<QVariant> vlist = myTableModel->getRowData(row);
        for ( int j = 0 ; j < vlist.count() ; j++ ) {
            if (j > 0) str += "\t";
            str += vlist.at(j).toString();
        }
        str += "\n";
        subTablewidget->appendClipBoad(str, frameGroupModel->getGIDOfRow(row));
    }

    QApplication::clipboard()->setText(str);
}

void FrameSettingTableWidget::groupPaste()
{
    QModelIndex current_idx = myTableView->currentIndex();
    if ( current_idx.column() != 0 ) {
        warningGroupCopyPosition();
        return;
    }

    QString str = QApplication::clipboard()->text();
    QStringList line = str.split('\n');
    QStringList cell = line.at(0).split('\t');
    if ( cell.count() != 1 ) return;

    int rowcount = cell.at(0).toInt();
    if ( current_idx.row() + rowcount > myProxyModel->rowCount() ) {
        warningGroupCopyRange();
        return;
    }
    QList<int> rowlist;
    for ( int i = 0; i < rowcount; i++ ) {
        QVariant var = myProxyModel->data(myProxyModel->index(current_idx.row() + i, 0));
        rowlist.append(var.toInt() - 1);
    }

    subTablewidget->setGroupPasteCommand(rowlist, str);
}

void FrameSettingTableWidget::slotDeleteLine()
{
    bool isBlocked = false;
    bool isDisable = false;

    QModelIndexList rows_idxlist = mySelectionModel->selectedRows();
    Q_FOREACH (QModelIndex idx, rows_idxlist) {
        int row = idx.data().toInt() - 1;
        if ( frameGroupModel->isGIDRow(row) ) isBlocked = true;
        if ( frameGroupModel->isRegisterRow(row) ) isDisable = true;
    }

    if ( isDisable && !frameGroupModel->isEditOn() ) return;
    if ( isBlocked ) {
        if ( warningLineDelete() ) return;
    }

    CustomTableWidget::slotDeleteLine();
    if ( isBlocked ) {
        emit frameItemChanged();
        undoStack->clear();
    }
}

void FrameSettingTableWidget::slotEditItem(const QModelIndex &idx, const QVariant &var)
{
    if ( var.toString() == myTableModel->getData(idx.row(), idx.column()).toString() ) return;

    bool isSumCol = false;
    int iorg = myTableModel->getData(idx.row(), idx.column()).toInt();
    if ( idx.column() == myType.getTaggedColumn("FRAMESUM") && iorg > 0 && var.toInt() == 0 ) {
        isSumCol = true;
        if ( warningFrameCount() ) return;
    }
    CustomTableWidget::slotEditItem(idx, var);
    if ( isSumCol ) undoStack->clear();
}

void FrameSettingTableWidget::paste()
{
    bool isSumCol = false;

    QModelIndex current_idx = myTableView->currentIndex();
    QString str = QApplication::clipboard()->text();

    if ( !current_idx.isValid() ) return;
    int c_row = current_idx.row();
    int c_col = current_idx.column();

    QStringList rows = str.split('\n');
    int numRows = rows.count() - 1;

    for (int i = 0; i < numRows; i++) {
        QStringList columns = rows[i].split('\t');
        for (int j = 0; j < rows[i].count('\t') + 1 ; j++) {
            int row = c_row + i;
            int col = c_col + j;
            if ( row >= myProxyModel->rowCount() ) continue;
            if ( col == myType.getTaggedColumn("FRAMESUM") ) {
                int a_count = myProxyModel->data(myProxyModel->index(row, col), Qt::EditRole).toInt();
                bool flg;
                int b_count = columns.at(j).toInt(&flg);
                if ( a_count != 0 && flg == true && b_count == 0 ) isSumCol = true;
            }
        }
    }

    if ( isSumCol ) {
        if ( warningFrameCount() ) return;
    }

    CustomTableWidget::paste();
    if ( isSumCol ) undoStack->clear();
}

void FrameSettingTableWidget::del()
{
    bool isSumCol = false;

    if ( !mySelectionModel->hasSelection() ) return;
    QModelIndexList idxlist = mySelectionModel->selectedIndexes();
    Q_FOREACH (QModelIndex idx, idxlist) {
        if ( idx.column() == myType.getTaggedColumn("FRAMESUM") ) {
            int a_count = myProxyModel->data(myProxyModel->index(idx.row(), idx.column()),
                                             Qt::EditRole).toInt();
            if ( a_count != 0 ) isSumCol = true;
        }
    }

    if ( isSumCol ) {
        if ( warningFrameCount() ) return;
    }

    CustomTableWidget::del();
    if ( isSumCol ) undoStack->clear();
}

void FrameSettingTableWidget::slotManageTableData( const QModelIndex &idx_a,
                                                   const QModelIndex &idx_b)
{
    int a_row = idx_a.row();
    int b_row = idx_b.row();

    if (isSignalBlocked) return;

    if ( idx_a.column() <= myType.getTaggedColumn("FRAMESUM") && idx_b.column() >= 1 ) {
        for ( int i = a_row; i <= b_row; i++ ) {
            QModelIndex idx = myTableModel->index(i, myType.getTaggedColumn("GID"));
            QString gid = myTableModel->data(idx, Qt::EditRole).toString();
            subTablewidget->updateSubTable(gid);
        }
    }

    QVariant var = myTableModel->data(myTableModel->index(a_row, myType.getTaggedColumn("GID")),
                                      Qt::EditRole);
    QString gid = var.toString();
    subTablewidget->changeCurrentGroup(gid);
    emit currentGroupChanged(gid);

    if ( idx_a.column() > 0 || idx_b.column() > 0 ) emit frameItemChanged();
}

QList<FRAMEGROUPVALUE> FrameSettingTableWidget::getFrameGroupList() const
{
    QList<FRAMEGROUPVALUE> groupList;

    QStringList gidlist = frameGroupModel->getGIDList();
    Q_FOREACH (QString gid, gidlist) {
        FRAMEGROUPVALUE fgitem = frameGroupModel->getFrameGroupItemOfGID(gid);
        groupList.append(fgitem);
    }
    return groupList;
}

QList<FRAMEPOINTSVALUE> FrameSettingTableWidget::getFramePointList() const
{
    return subTablewidget->getFramePointList();
}

bool FrameSettingTableWidget::existResisterRow()
{
    return frameGroupModel->existRegisterRow();
}

void FrameSettingTableWidget::signalFrameItemChanged()
{
    emit frameItemChanged();
}

void FrameSettingTableWidget::slotChangeSubView(int row)
{
    if ( row < 0 || row >= myProxyModel->rowCount() ) return;

    QVariant var = myProxyModel->data(myProxyModel->index(row, 0), Qt::EditRole);
    QString gid = frameGroupModel->getGIDOfRow(var.toInt() - 1);

    subTablewidget->changeCurrentGroup(gid);
    emit currentGroupChanged(gid);
}

void FrameSettingTableWidget::slotChangeFrameCount(const QString &gid, int count)
{
    int row = frameGroupModel->getRowOfGID(gid);
    if ( row < 0 ) return;
    setSingnalOff();
    myTableModel->setDataDirect(myTableModel->index(row, myType.getTaggedColumn("FRAMESUM")),
                                QVariant(count), false);
    setSingnalOn();

}

void FrameSettingTableWidget::modelUpload()
{
    frameGroupModel->linkUUID();
    frameGroupModel->setEditOn(false);
    frameGroupModel->keepCurrentData();
    subTablewidget->modelUpload();
    undoStack->clear();
}

void FrameSettingTableWidget::modelEdit()
{
    frameGroupModel->setEditOn(true);
    subTablewidget->modelEdit();
}

void FrameSettingTableWidget::modelRewind()
{
    frameGroupModel->backPreviousData();
    frameGroupModel->setEditOn(false);

    subTablewidget->modelRewind();
    undoStack->clear();
}

bool FrameSettingTableWidget::warningFrameCount()
{
    return Utility::showWarningMessage2(this, u8"通り数を０にすると元に戻せません。\n続けますか？");
}

bool FrameSettingTableWidget::warningLineDelete()
{
    return Utility::showWarningMessage2(this, u8"定義済の通りグループを削除すると元に戻せません。\n削除しますか？");
}

void FrameSettingTableWidget::warningGroupCopyPosition()
{
    Utility::showWarningMessage(this, u8"行頭の位置で貼り付けを実行してください。");
}

void FrameSettingTableWidget::warningGroupCopyRange()
{
    Utility::showWarningMessage(this, u8"貼り付ける範囲がテーブルの行数をオーバーしています。\n行数を増やしてから貼り付けを実行してください");
}

bool FrameSettingTableWidget::checkBinaryFile ( QDataStream &infile) const
{
    bool ok = true;
    ok = CustomTableWidget::checkBinaryFile(infile);
    if ( !ok ) return false;
    return subTablewidget->checkBinaryFile(infile);
}

void FrameSettingTableWidget::readBinaryFile ( QDataStream &infile)
{
    CustomTableWidget::readBinaryFile(infile);
    subTablewidget->readBinaryFile(infile);
}

void FrameSettingTableWidget::writeBinaryFile ( QDataStream &outfile)
{
    CustomTableWidget::writeBinaryFile(outfile);
    subTablewidget->writeBinaryFile(outfile);
}

void FrameSettingTableWidget::clearData()
{
    CustomTableWidget::clearData();
    subTablewidget->clearData();
}

void FrameSettingTableWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(FRAMEGROUPVALUE fg, UnifiedInputData::getInInstance()->getFrameGroupList()){
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(fg.groupName));
        data.append(QVariant(FrameType::enumToString(fg.frameType)));
        data.append(QVariant(fg.count));
        data.append(QVariant(fg.basePoint.x()));
        data.append(QVariant(fg.basePoint.y()));
        data.append(QVariant(fg.startAngle));
        data.append(QVariant(fg.endAngle));
        data.append(QVariant(fg.division));
        data.append(QVariant(fg.plusLength));
        data.append(QVariant(fg.minusLength));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(fg.startFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(UnifiedInputData::getInInstance()->idToName(fg.endFloorID, DATATYPE::TPFLOOR)));
        data.append(QVariant(QUuid().toString()));
        i++;
        temp.appendRowData(data);
    }
    myTableModel->reflectUnifiedData(temp);
    myTableModel->linkUUID();
    subTablewidget->reflectUnifiedData();
}

bool FrameSettingTableWidget::checkTextFile(QTextStream &infile) const
{
    if ( !myTableModel->checkTextData(infile) ) return false;
    if ( !subTablewidget->checkTextFile(infile) ) return false;
    //        modelEdit();
    //        undoStack->clear();
    //        emit frameItemChanged();
    //        slotChangeSubView(0);
    return true;
}

bool FrameSettingTableWidget::readTextFile(QTextStream &infile, bool flg)
{
    if ( !myTableModel->readListTextData(infile, flg) ) return false;
    if ( !subTablewidget->readTextFile(infile, flg) ) return false;
    modelEdit();
    undoStack->clear();
    emit frameItemChanged();
    slotChangeSubView(0);
    return true;
}

void FrameSettingTableWidget::writeTextFile(QTextStream &outfile, bool writeUuid)
{
    QList<int> list;
    list.append( myType.getTaggedColumn("UUID") ); // columnUUID=15
    myTableModel->writeListTextData(outfile, list );
    subTablewidget->writeTextFile(outfile, writeUuid);
}

CustomTableModel* FrameSettingTableWidget::getSubTableModel() const{
    return subTablewidget->getTableModel();
}

} // namespace post3dapp
