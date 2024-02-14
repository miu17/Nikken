#include <QtGui>
#include <QDebug>

#include "analysisrange_tablewidget.h"
#include "analysisrange_tablemodel.h"
#include "sortfilter_proxymodel.h"
#include "customtable_view.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_inputdata.h"
#include "unified_project.h"
#include "utility.h"

namespace post3dapp{

AnalysisRangeTableWidget::AnalysisRangeTableWidget( const CustomTableDataType &type, QWidget* parent )
    : CustomTableWidget( type, parent )
{
}

void AnalysisRangeTableWidget::createWidgets()
{
    QVBoxLayout *vlay=new QVBoxLayout;
    vlay->setContentsMargins(0,0,0,0);
    vlay->setSpacing(0);

    vlay->addLayout(topLayout,0);
    vlay->addWidget(myTableView,1);
    this->setLayout(vlay);
}

void AnalysisRangeTableWidget::slotDeleteLine()
{
    CustomTableWidget::slotDeleteLine();
    emit signalTableItemChanged();
}

int AnalysisRangeTableWidget::currentRow() const
{
    return myTableView->currentIndex().row();
}

QList<GroupSetting> AnalysisRangeTableWidget::getData() const
{
    // 空白行あり
    auto data = static_cast<AnalysisRangeTableModel*>(myTableModel)->getData();

    QList<GroupSetting> ret;
    foreach(auto d, data){
        if (d.group.trimmed() == QString{} && d.list.count() == 0 && d.comment.trimmed() == QString{})
            continue;

        ret.append(GroupSetting(d.group, d.list, d.comment));
    }
    return ret;
}


void AnalysisRangeTableWidget::slotManageTableData( const QModelIndex &idx_a, const QModelIndex &idx_b, const QVector<int> &roles)
{
    Q_UNUSED(idx_a);
    Q_UNUSED(roles);
    if( idx_b.column() > 0 ) emit signalTableItemChanged();
}

void AnalysisRangeTableWidget::slotCurrentRowChanged(int row)
{
    QVariant var=myProxyModel->data(myProxyModel->index(row,0));
    int t_row=var.toInt()-1;
    emit signalCurrentRowChanged(t_row);
}

void AnalysisJointRangeTableWidget::createActions()
{
    allocateAction = new QAction(u8"割当", this);
    myActionList.append(allocateAction);
    connect (allocateAction, &QAction::triggered, this, &AnalysisJointRangeTableWidget::signalAllocateCurrentJoints);

    additionallyAllocateAction = new QAction(u8"追加割当");
    myActionList.append(additionallyAllocateAction);
    connect (additionallyAllocateAction, &QAction::triggered, this, &AnalysisJointRangeTableWidget::signalAdditionallyAllocateCurrentJoints);

    confirmAction = new QAction(u8"確認", this);
    myActionList.append(confirmAction);
    connect(confirmAction, &QAction::triggered, this, &AnalysisJointRangeTableWidget::signalConfirmTableJoints);

    deleteAction = new QAction(u8"行削除", this);
    deleteAction->setShortcut(tr("Ctrl+D"));
    deleteAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(deleteAction);
    connect ( deleteAction, &QAction::triggered, this, &AnalysisRangeTableWidget::slotDeleteLine );

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

    myMenu->addActions(myActionList);
    myMenu->insertSeparator(deleteAction);

}

AnalysisJointRangeTableWidget::AnalysisJointRangeTableWidget(const CustomTableDataType &type, const QList<GroupSetting> &jlist, QWidget *parent)
    :AnalysisRangeTableWidget(type, parent)
{
    myMenu=new QMenu(this);
    undoStack = new QUndoStack(this);

    createActions();
    createButtons();
    createModels();
    createViews();
    createWidgets();
    setFilterWidgetsVisible(false);
    setUndoOnOff(false);
    this->modelRewind(jlist);
}

void AnalysisJointRangeTableWidget::setSelectedItems( const QList<JointData*>& jdlist)
{
    int row=myTableView->currentIndex().row();
    QVariant var=myProxyModel->data(myProxyModel->index(row,0));
    row=var.toInt()-1;
    if( row<0 ) return;
    QModelIndex idx_a=myTableModel->index(row,myType.getTaggedColumn("JOINTS"));
    QList<QVariant> idList;
    Q_FOREACH( JointData* jd, jdlist ){
        idList.append(QVariant(jd->getUuid()));
    }
    myTableModel->setDataDirect( idx_a, QVariant(idList), true );
}

QList<JointData*> AnalysisJointRangeTableWidget::getSelectedJoints() const
{
    QVariant var=myTableModel->getData(currentRow(),myType.getTaggedColumn("JOINTS"));
    QList<QUuid> idlist;
    for (auto uuid : var.toList()){
        idlist.append(uuid.toUuid());
    }
    QList<JointData*> jdlist=UnifiedInputData::getInInstance()->jointIDsToPointers(idlist);
    for( int i=jdlist.count()-1; i>=0; i-- ){
        if( jdlist.at(i)==NULL )
            jdlist.removeAt(i);
    }
    return jdlist;
}

void AnalysisJointRangeTableWidget::modelRewind(const QList<GroupSetting>& list)
{
    auto tableModel = static_cast<AnalysisJointRangeTableModel*>(myTableModel);
    tableModel->modelRewind(list);
}

bool AnalysisJointRangeTableWidget::checkData() const
{
    auto jointGroup = getData();
    foreach(auto jGroup, jointGroup){
        if (jGroup.group.trimmed() == QString{}){
            Utility::showWarningMessage(nullptr, u8"グループ名がありません");
            return false;
        }
        if (jGroup.list.count() == 0){
            Utility::showWarningMessage(nullptr, u8"選択節点がありません");
            return false;
        }

        int count = 0;
        foreach(auto j, jointGroup){
            if (j.group == jGroup.group)
                count++;
        }
        if (count != 1){
            Utility::showWarningMessage(nullptr, u8"グループ名が重複しています");
            return false;
        }

        foreach(auto j, jGroup.list){
            if (!UnifiedInputData::getInInstance()->isExistedID(j, DATATYPE::TPJOINT)){
                Utility::showWarningMessage(nullptr, u8"UUIDがありません。");
                return false;
            }
        }
    }

    return true;
}

void AnalysisJointRangeTableWidget::createModels()
{
    myTableModel = new AnalysisJointRangeTableModel(myType, this);
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel=new QItemSelectionModel(myProxyModel);
    connect ( myTableModel, &AnalysisRangeTableModel::rowCountChanged,
              this, &AnalysisRangeTableWidget::slotChangeSpinBoxValue);
    connect ( myTableModel, &AnalysisRangeTableModel::dataCountChanged,
              this, &AnalysisRangeTableWidget::slotChangeSpinBoxMinimum);
    connect ( myTableModel, &AnalysisRangeTableModel::dataChanged,
              this, &AnalysisRangeTableWidget::slotManageTableData);
    connect ( myTableModel, &AnalysisRangeTableModel::itemEdited,
              this, &AnalysisRangeTableWidget::slotEditItem);

}

void AnalysisJointRangeTableWidget::createViews()
{
    myTableView=new CustomTableView(this);
    myTableView->setFirstColumn(myType.getTaggedColumn("NAME"));
    myTableView->setLastColumn(myType.getTaggedColumn("COMMENTS"));

//    myTableView->setSpan(1,1,1,10);

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth(myType.getTaggedColumn("NO"),30);
    myTableView->setColumnWidth(myType.getTaggedColumn("NAME"),80);
    myTableView->setColumnWidth(myType.getTaggedColumn("JOINTS"),150);
    myTableView->setColumnWidth(myType.getTaggedColumn("JOINTNUMBERS"),200);
    myTableView->setColumnWidth(myType.getTaggedColumn("COMMENTS"),150);

    connect ( myTableView, QOverload<void>::of(&CustomTableView::delKeyClicked),
              this, &AnalysisJointRangeTableWidget::del);
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &AnalysisRangeTableWidget::slotRowActionEnable);
    connect ( myTableView, &CustomTableView::currentRowChanged,
              this, &AnalysisRangeTableWidget::slotCurrentRowChanged);
}

AnalysisMemberRangeTableWidget::AnalysisMemberRangeTableWidget(const CustomTableDataType &type, const QList<GroupSetting> &mlist, QWidget *parent)
    :AnalysisRangeTableWidget(type, parent)
{
    myMenu=new QMenu(this);
    undoStack = new QUndoStack(this);

    createActions();
    createButtons();
    createModels();
    createViews();
    createWidgets();
    setFilterWidgetsVisible(false);
    setUndoOnOff(false);
    this->modelRewind(mlist);
}

void AnalysisMemberRangeTableWidget::setSelectedItems(const QList<MemberData*>& mdlist )
{
    int row=myTableView->currentIndex().row();
    QVariant var=myProxyModel->data(myProxyModel->index(row,0));
    row=var.toInt()-1;
    if( row<0 ) return;
    QModelIndex idx_b=myTableModel->index(row,myType.getTaggedColumn("MEMBERS"));
    QList<QVariant> idList;
    Q_FOREACH( MemberData* md, mdlist ){
        idList.append(QVariant(md->getUuid()));
    }
    myTableModel->setDataDirect( idx_b, QVariant(idList), true );
}

QList<MemberData*> AnalysisMemberRangeTableWidget::getSelectedMembers() const
{
    QVariant var=myTableModel->getData(currentRow(),myType.getTaggedColumn("MEMBERS"));
    QList<QUuid> idlist;
    for(auto v : var.toList()){
        idlist.append(v.toUuid());
    }
    QList<MemberData*> mdlist=UnifiedInputData::getInInstance()->memberIDsToPointers(idlist);
    for( int i=mdlist.count()-1; i>=0; i-- ){
        if( mdlist.at(i)==NULL ) mdlist.removeAt(i);
    }
    return mdlist;
}

void AnalysisMemberRangeTableWidget::modelRewind(const QList<GroupSetting>& list)
{
    auto tableModel = static_cast<AnalysisMemberRangeTableModel*>(myTableModel);
    tableModel->modelRewind(list);
}

bool AnalysisMemberRangeTableWidget::checkData() const
{
    auto memberGroup = getData();
    foreach(auto mGroup, memberGroup){
        if (mGroup.group.trimmed() == QString{}){
            Utility::showWarningMessage(nullptr, u8"グループ名がありません");
            return false;
        }

        if (mGroup.group.trimmed().length() >= 10){
            Utility::showWarningMessage(nullptr, u8"グループ名は10文字未満にしてください。");
            return false;
        }

        if (mGroup.list.count() == 0){
            Utility::showWarningMessage(nullptr, u8"選択要素がありません");
            return false;
        }

        int count = 0;
        foreach(auto m, memberGroup){
            if (m.group == mGroup.group)
                count++;
        }
        if (count != 1){
            Utility::showWarningMessage(nullptr, u8"グループ名が重複しています");
            return false;
        }

        foreach(auto j, mGroup.list){
            if (!UnifiedInputData::getInInstance()->isExistedID(j, DATATYPE::TPCOLUMN)){ // MEMBERであればどれも同じ
                Utility::showWarningMessage(nullptr, u8"UUIDがありません。");
                return false;
            }
        }
    }


    return true;

}

void AnalysisMemberRangeTableWidget::createModels()
{
    myTableModel = new AnalysisMemberRangeTableModel(myType, this);
    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel=new QItemSelectionModel(myProxyModel);
    connect ( myTableModel, &AnalysisRangeTableModel::rowCountChanged,
              this, &AnalysisRangeTableWidget::slotChangeSpinBoxValue);
    connect ( myTableModel, &AnalysisRangeTableModel::dataCountChanged,
              this, &AnalysisRangeTableWidget::slotChangeSpinBoxMinimum);
    connect ( myTableModel, &AnalysisRangeTableModel::dataChanged,
              this, &AnalysisRangeTableWidget::slotManageTableData);
    connect ( myTableModel, &AnalysisRangeTableModel::itemEdited,
              this, &AnalysisRangeTableWidget::slotEditItem);
}


void AnalysisMemberRangeTableWidget::createViews()
{
    myTableView=new CustomTableView(this);
    myTableView->setFirstColumn(myType.getTaggedColumn("NAME"));
    myTableView->setLastColumn(myType.getTaggedColumn("COMMENTS"));

//    myTableView->setSpan(1,1,1,10);

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth(myType.getTaggedColumn("NO"),30);
    myTableView->setColumnWidth(myType.getTaggedColumn("NAME"),80);
    myTableView->setColumnWidth(myType.getTaggedColumn("MEMBERS"),150);
    myTableView->setColumnWidth(myType.getTaggedColumn("DATATYPE"),200);
    myTableView->setColumnWidth(myType.getTaggedColumn("COMMENTS"),150);

//    connect ( myTableView, QOverload<void>::of(&CustomTableView::delKeyClicked),
//              this, &AnalysisMemberRangeTableWidget::del);
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &AnalysisRangeTableWidget::slotRowActionEnable);
    connect ( myTableView, &CustomTableView::currentRowChanged,
              this, &AnalysisRangeTableWidget::slotCurrentRowChanged);
}

void AnalysisMemberRangeTableWidget::createActions()
{
    allocateAction = new QAction(u8"割当", this);
    myActionList.append(allocateAction);
    connect (allocateAction, &QAction::triggered, this, &AnalysisMemberRangeTableWidget::signalAllocateCurrentMembers);

    additionallyAllocateAction = new QAction(u8"追加割当");
    myActionList.append(additionallyAllocateAction);
    connect (additionallyAllocateAction, &QAction::triggered, this, &AnalysisMemberRangeTableWidget::signalAdditionallyAllocateCurrentMembers);

    confirmAction = new QAction(u8"確認", this);
    myActionList.append(confirmAction);
    connect(confirmAction, &QAction::triggered, this, &AnalysisMemberRangeTableWidget::signalConfirmTableMembers);

    deleteAction = new QAction(u8"行削除", this);
    deleteAction->setShortcut(tr("Ctrl+D"));
    deleteAction->setShortcutContext(Qt::WidgetShortcut);
    myActionList.append(deleteAction);
    connect ( deleteAction, &QAction::triggered, this, &AnalysisRangeTableWidget::slotDeleteLine );

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

    myMenu->addActions(myActionList);
    myMenu->insertSeparator(deleteAction);

}

} // namespace post3dapp
