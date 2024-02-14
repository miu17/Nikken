#include "framesetting_subtablewidget.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QSpinBox>

#include "calc_vector2d.h"
#include "combobox_delegate.h"
#include "commands_grouppaste.h"
#include "customtable_view.h"
#include "framesetting_subtablemodel.h"
#include "framesetting_tablemodel.h"
#include "framesetting_xydialog.h"
#include "sortfilter_proxymodel.h"
#include "unified_inputdata.h"
#include "utility.h"


namespace post3dapp{
FrameSettingSubTableWidget::FrameSettingSubTableWidget( const CustomTableDataType &type,
                                                        FrameSettingTableModel *model,
                                                        QUndoStack *undo_stack,
                                                        QWidget *parent )
    : CustomTableWidget( type, parent )
{
    myMenu = new QMenu(this);
    undoStack = undo_stack;

    createActions();
    createButtons();
    createModels();
    createViews();
    createWidgets();
    setFilterWidgetsVisible(false);
    setSingnalOn();
    frameGroupModel = model;

    connect ( frameGroupModel, &FrameSettingTableModel::gidRemoved,
              this, &FrameSettingSubTableWidget::removeGidRows);

}

void FrameSettingSubTableWidget::createModels()
{
    frameSubModel = new FrameSettingSubTableModel(myType, this);
    myTableModel = frameSubModel;

    myProxyModel = new MySortFilterProxyModel(this);
    myProxyModel->setSourceModel(myTableModel);
    myProxyModel->setDynamicSortFilter(true);
    mySelectionModel = new QItemSelectionModel(myProxyModel);
    myTableModel->setFilterStatus(true);

    connect ( myTableModel, &CustomTableModel::rowCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxValue );
    connect ( myTableModel, &CustomTableModel::dataCountChanged,
              this, &CustomTableWidget::slotChangeSpinBoxMinimum );
    connect ( frameSubModel, &FrameSettingSubTableModel::gidDataChanged,
              this, &FrameSettingSubTableWidget::slotManageRowCount );
    connect ( myTableModel, &CustomTableModel::itemEdited,
              this, &CustomTableWidget::slotEditItem );

    connect ( frameSubModel, &FrameSettingSubTableModel::rowDataChanged,
              this, &FrameSettingSubTableWidget::slotSignalRowCount );
    connect ( frameSubModel, &QAbstractItemModel::dataChanged,
              this, &FrameSettingSubTableWidget::signalDataChanged );

}

void FrameSettingSubTableWidget::createViews()
{
    myTableView = new CustomTableView;
    myTableView->setSortingEnabled(false);
    myTableView->setFirstColumn(myType.getTaggedColumn("NAME"));
    myTableView->setLastColumn(myType.getTaggedColumn("XYPOINTS"));

    myTableView->addActions(myActionList);
    myTableView->setMenu(myMenu);

    myTableView->setModel(myProxyModel);
    myTableView->setSelectionModel(mySelectionModel);
    myTableView->setColumnWidth(myType.getTaggedColumn("NO"), 60);
    myTableView->setColumnWidth(myType.getTaggedColumn("FRAMEGROUP"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("FRAMETYPE"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("NAME"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("SPAN"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("ANGLE"), 80);
    myTableView->setColumnWidth(myType.getTaggedColumn("BUTTON"), 60);
    myTableView->setColumnWidth(myType.getTaggedColumn("XYPOINTS"), 150);
    myTableView->setColumnWidth(myType.getTaggedColumn("GID"), 120);
    myTableView->setColumnWidth(myType.getTaggedColumn("UUID"), 120);

    QHash<int, QStringList> lmlist = myType.getLimitedStrings();
    Q_FOREACH (int col, lmlist.keys()) {
        ComboBoxDelegate *deleg = new ComboBoxDelegate(lmlist.value(col));
        myTableView->setItemDelegateForColumn(col, deleg);
    }

    connect ( myTableView, QOverload<>::of(&CustomTableView::delKeyClicked), this, &CustomTableWidget::del );
    connect ( myTableView, &QAbstractItemView::clicked,
              this, &FrameSettingSubTableWidget::slotShowXYDialog );
    connect ( myTableView, &CustomTableView::selectionBehaviorChanged,
              this, &CustomTableWidget::slotRowActionEnable );
    myTableView->horizontalHeader()->setStretchLastSection(true);
}

void FrameSettingSubTableWidget::createWidgets()
{
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addWidget(myTableView, 1);

    this->setLayout(vlay);
}

void FrameSettingSubTableWidget::clearData()
{
    myTableModel->initializeModel();
    mySpinBox->setValue(0);
    myTableModel->setRowCount(0);
}

bool FrameSettingSubTableWidget::readTextFile ( QTextStream &infile, bool flg)
{
    if ( myTableModel->readListTextData(infile, flg) ) {
        QStringList gidlist = frameGroupModel->getGIDList();
        setSingnalOn();
        Q_FOREACH (QString gid, gidlist)
            this->slotManageRowCount( gid ); // frameGroupModelのminValueListの更新
        return true;
    }
    return false;
}

void FrameSettingSubTableWidget::writeTextFile(QTextStream &outfile, bool writeUuid)
{
    QList<int> list;
    if (!writeUuid){
        list.append( myType.getTaggedColumn("UUID") ); // columnUUID=9
    }
    myTableModel->writeListTextData(outfile, list);
}

void FrameSettingSubTableWidget::changeCurrentGroup(const QString &gid)
{
    bool isBlocked = ( frameGroupModel->hasUUID(gid) && !frameGroupModel->isEditOn() );
    setEditOn(!isBlocked);

    QList<QVariant> vlist = frameGroupModel->getRowDataOfGID(gid);
    QString type = ( vlist.count() < myTableModel->columnCount() ) ?
                   "" : vlist.at(myType.getTaggedColumn("FRAMETYPE")).toString() ;
    FrameType::FRAMETYPE itype = static_cast<FrameType::FRAMETYPE>(myType.getLimitedStrings().value(myType.getTaggedColumn("FRAMETYPE")).indexOf(type) + 1);

    if ( itype == FrameType::FRAMETYPE::STRAIGHT || itype == FrameType::FRAMETYPE::CIRCLE || itype == FrameType::FRAMETYPE::CLCIRCLE ) {
        myTableView->setColumnHidden(myType.getTaggedColumn("SPAN"), false);
        myTableView->setColumnHidden(myType.getTaggedColumn("ANGLE"), true);
        myTableView->setColumnHidden(myType.getTaggedColumn("BUTTON"), true);
        myTableView->setColumnHidden(myType.getTaggedColumn("XYPOINTS"), true);
        myTableView->setLastColumn(myType.getTaggedColumn("SPAN"));
    } else if ( itype == FrameType::FRAMETYPE::RADIUS || itype == FrameType::FRAMETYPE::DIAMETER ) {
        myTableView->setColumnHidden(myType.getTaggedColumn("SPAN"), true);
        myTableView->setColumnHidden(myType.getTaggedColumn("ANGLE"), false);
        myTableView->setColumnHidden(myType.getTaggedColumn("BUTTON"), true);
        myTableView->setColumnHidden(myType.getTaggedColumn("XYPOINTS"), true);
        myTableView->setLastColumn(myType.getTaggedColumn("ANGLE"));
    } else if ( itype == FrameType::FRAMETYPE::VARIABLE || itype == FrameType::FRAMETYPE::CLVARIABLE ) {
        myTableView->setColumnHidden(myType.getTaggedColumn("SPAN"), true);
        myTableView->setColumnHidden(myType.getTaggedColumn("ANGLE"), true);
        myTableView->setColumnHidden(myType.getTaggedColumn("BUTTON"), false);
        myTableView->setColumnHidden(myType.getTaggedColumn("XYPOINTS"), false);
        myTableView->setLastColumn(myType.getTaggedColumn("XYPOINTS"));
    } else {
        myTableView->setColumnHidden(myType.getTaggedColumn("SPAN"), true);
        myTableView->setColumnHidden(myType.getTaggedColumn("ANGLE"), true);
        myTableView->setColumnHidden(myType.getTaggedColumn("BUTTON"), true);
        myTableView->setColumnHidden(myType.getTaggedColumn("XYPOINTS"), true);
        myTableView->setLastColumn(myType.getTaggedColumn("XYPOINTS"));
    }

    QString filter_word = ( gid.isEmpty() ) ? "____" : gid;
    myProxyModel->setFilterColumn(myType.getTaggedColumn("GID"));
    myProxyModel->setFilterFixedString(filter_word);

}

void FrameSettingSubTableWidget::updateSubTable(const QString &gid)
{
    if ( frameGroupModel->getRowOfGID(gid) < 0 ) return;

    QList<QVariant> vlist = frameGroupModel->getRowDataOfGID(gid);
    QString str1 = vlist.at(myType.getTaggedColumn("FRAMEGROUP")).toString();
    QString str2 = vlist.at(myType.getTaggedColumn("FRAMETYPE")).toString();
    int rows = vlist.at(myType.getTaggedColumn("NAME")).toInt();

    importRowList.insert(gid, rows);
    if ( !minRowList.contains(gid) ) minRowList.insert(gid, 0);

    setSingnalOff();

    QList<int> rowlist = frameSubModel->getRowsOfGID(gid);
    for ( int i = 0 ; i < rowlist.count() ; i++ ) {
        frameSubModel->setGroupName(rowlist.at(i), str1, str2);
    }

    if (rowlist.count() > rows) {
        myTableModel->removeRows(rowlist.at(rows), rowlist.count() - rows);
    } else if (rowlist.count() < rows) {
        if ( rowlist.isEmpty() ) {
            myTableModel->insertRows(myTableModel->rowCount(), rows);
            for (int i = 0; i < rows; i++) {
                int row = myTableModel->rowCount() - i - 1;
                frameSubModel->setGIDDirect(row, gid);
                frameSubModel->setGroupName(myTableModel->rowCount() - i - 1, str1, str2);
            }
        } else {
            myTableModel->insertRows(rowlist.last() + 1, rows - rowlist.count());
            for (int i = 0; i < rows - rowlist.count(); i++) {
                int row = rowlist.last() + 1 + i;
                frameSubModel->setGIDDirect(row, gid);
                frameSubModel->setGroupName(rowlist.last() + 1 + i, str1, str2);
            }
        }
    }

    setSingnalOn();

    changeCurrentGroup(gid);
}

void FrameSettingSubTableWidget::appendClipBoad(QString &str, const QString &gid)
{
    QList<int> rowlist = frameSubModel->getRowsOfGID(gid);
    for ( int i = 0 ; i < rowlist.count() ; i++ ) {
        QList<QVariant> vlist = myTableModel->getRowData(rowlist.at(i));
        for ( int j = 0 ; j < vlist.count() ; j++ ) {
            if (j > 0) str += "\t";
            str += vlist.at(j).toString();
        }
        str += "\n";
    }
}

void FrameSettingSubTableWidget::setGroupPasteCommand( const QList<int> &rowlist,
                                                       const QString &str )
{
    QUndoCommand *gpasteCommand = new GroupPasteCommand(frameGroupModel, frameSubModel,
                                                        rowlist, str);
    undoStack->push(gpasteCommand);
    if (!undoOnOff) {
        undoStack->clear();
    }
}

void FrameSettingSubTableWidget::removeGidRows(const QString &gid)
{
    setSingnalOff();

    QList<int> rowlist = frameSubModel->getRowsOfGID(gid);
    for (int i = rowlist.count() - 1; i >= 0; i--)
        frameSubModel->deleteRow(rowlist.at(i));

    setSingnalOn();

    importRowList.remove(gid);
    minRowList.remove(gid);
}

void FrameSettingSubTableWidget::slotDeleteLine()
{
    if ( myTableView->selectionBehavior() != QAbstractItemView::SelectRows ) {
        Utility::showWarningMessage(this, u8"行全体を範囲指定して下さい。");
        return;
    }

    QModelIndexList rows_idxlist = mySelectionModel->selectedRows();

    /* UUIDを持つ通りの削除 */
    bool isDisable = false;
    Q_FOREACH (QModelIndex idx, rows_idxlist) {
        int row = idx.data().toInt() - 1;
        if ( frameSubModel->isRegisterRow(row) ) isDisable = true;
    }
    if ( isDisable )
        if ( warningDeleteLine() ) return;

    /* 通りグループ（通り全て）の削除 */
    int row = rows_idxlist.first().data().toInt() - 1;
    QString gid = frameSubModel->getGIDOfRow(row);
    bool isBlocked = false;
    if ( rows_idxlist.count() >= frameSubModel->getRowsOfGID(gid).count() ) {
        isBlocked = true;
        if ( warningFrameCount() ) return;
    }

    CustomTableWidget::slotDeleteLine();
    if ( isBlocked ) undoStack->clear();

    emit frameItemChanged();
}

void FrameSettingSubTableWidget::slotSignalRowCount(const QString &gid)
{

    if (isSignalBlocked) return;


    QList<int> rowlist = frameSubModel->getRowsOfGID(gid);
    if ( rowlist.isEmpty() ) {
        frameGroupModel->setMinValueOfFrameCount(gid, 0);
        minRowList.remove(gid);
        importRowList.remove(gid);
        emit frameCountChanged(gid, 0);
    } else if ( rowlist.count() != importRowList.value(gid) ) {
        importRowList.insert(gid, rowlist.count());
        emit frameCountChanged(gid, rowlist.count());
    }

}

void FrameSettingSubTableWidget::slotManageRowCount( const QString &gid )
{
    if (isSignalBlocked) return;

    QList<int> rowlist = frameSubModel->getRowsOfGID(gid);
    int minRow = rowlist.count();
    for ( int i = rowlist.count() - 1; i >= 0 ; i-- ) {
        if ( frameSubModel->isValidRange(rowlist.at(i), myType.getTaggedColumn("NAME"),
                                         myType.getTaggedColumn("GID") - 1) ) break;
        minRow--;
    }

    if ( minRow != minRowList.value(gid) ) {
        minRowList.insert(gid, minRow);
        frameGroupModel->setMinValueOfFrameCount(gid, minRow);
    }
}

void FrameSettingSubTableWidget::slotShowXYDialog( const QModelIndex &idx )
{
    if ( idx.column() != myType.getTaggedColumn("BUTTON") ) return;

    int row = myProxyModel->data(myProxyModel->index(idx.row(), 0), Qt::EditRole).toInt() - 1;
    QModelIndex o_idx = myTableModel->index(row, myType.getTaggedColumn("XYPOINTS"));
    QString str = o_idx.data(Qt::EditRole).toString();

    QString str1 = myTableModel->data(myTableModel->index(row, myType.getTaggedColumn("FRAMEGROUP")),
                                      Qt::EditRole).toString();
    QString str2 = myTableModel->data(myTableModel->index(row, myType.getTaggedColumn("NAME")),
                                      Qt::EditRole).toString();
    QString title = QString("No.%1").arg(row + 1) + "  " + str1 + " - " + str2;

    FrameSettingXYDialog dialog(title, str, this);
    if (dialog.exec()) {
        QString str_out = dialog.xyString();
        myTableModel->setData(o_idx, QVariant(str_out));
    }
}

void FrameSettingSubTableWidget::signalDataChanged(const QModelIndex &idx_a,
                                                   const QModelIndex &idx_b)
{
    if (isSignalBlocked) return;
    if ( idx_a.column() > 0 || idx_b.column() > 0 ) emit frameItemChanged();
}

QList<FRAMEPOINTSVALUE> FrameSettingSubTableWidget::getFramePointList() const
{
    QList<FRAMEPOINTSVALUE> drawList;

    for (int i = 0; i < frameSubModel->rowCount() ; i++) {

        QString gid = frameSubModel->getGIDOfRow(i);
        FRAMEGROUPVALUE fgitem = frameGroupModel->getFrameGroupItemOfGID(gid);
        QList<QPointF> plist;

        FrameType::FRAMETYPE itype = fgitem.frameType;
        qreal span = 0.0;
        QPointF npoint;
        STRAIGHTFRAMEVALUE st_value;

        if (itype == FrameType::FRAMETYPE::STRAIGHT) {

            QPointF po = fgitem.basePoint;
            qreal theta = fgitem.startAngle / 180.0 * M_PI;
            span = frameSubModel->getSumSpanOfRow(i);
            int gcount = frameSubModel->getGIDCountOfRow(i);
            qreal p_len = fgitem.plusLength;
            qreal m_len = fgitem.minusLength;
            QPointF p1, p2, p3, p4;
            QPointF spoint;
            qreal m_span = frameSubModel->getSpanOfRow(i);
            qreal s_angle;

            if ( fgitem.startAngle < 45.0 ) {
                p1 = QPointF(-m_len * qCos(theta) - span * qSin(theta),
                             -m_len * qSin(theta) + span * qCos(theta)) + po;
                p2 = QPointF( p_len * qCos(theta) - span * qSin(theta),
                              p_len * qSin(theta) + span * qCos(theta)) + po;
                npoint = CalcVector2D::extendPoint(p1, p2, 10.0);
                s_angle = fgitem.startAngle + 90.;
                if (gcount == fgitem.count - 1) {
                    p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
                    p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                    spoint = CalcVector2D::extendPoint(p3, p4, 5.0);
                } else {
                    spoint = CalcVector2D::extendPoint(p1, p2, 3.0);
                }
            } else {
                p1 = QPointF(-m_len * qCos(theta) + span * qSin(theta),
                             -m_len * qSin(theta) - span * qCos(theta)) + po;
                p2 = QPointF( p_len * qCos(theta) + span * qSin(theta),
                              p_len * qSin(theta) - span * qCos(theta)) + po;
                npoint = CalcVector2D::extendPoint(p2, p1, 10.0);
                s_angle = fgitem.startAngle - 90.;
                if (gcount == fgitem.count - 1) {
                    p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
                    p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                    spoint = CalcVector2D::extendPoint(p4, p3, 5.0);
                } else {
                    spoint = CalcVector2D::extendPoint(p2, p1, 3.0);
                }
            }
            plist.append(p1);
            plist.append(p2);

            if (gcount == fgitem.count - 1) m_span = span;
            st_value = STRAIGHTFRAMEVALUE(m_span, s_angle, spoint,
                                          (gcount == 0), (gcount == fgitem.count - 1));

        } else if (itype == FrameType::FRAMETYPE::CIRCLE) {

            QPointF po = fgitem.basePoint;
            qreal s_angle = fgitem.startAngle;
            qreal e_angle = fgitem.endAngle;
            span = frameSubModel->getSumSpanOfRow(i);
            int div = fgitem.division;

            if ( (s_angle < e_angle) && (e_angle - s_angle < 360.0) && div > 0 ) {
                qreal s_theta = s_angle / 180.0 * M_PI;
                qreal e_theta = e_angle / 180.0 * M_PI;
                qreal d_theta = (e_theta - s_theta) / qreal(div);
                for ( int j = 0; j < div + 1; j++ ) {
                    qreal j_theta = s_theta + d_theta * qreal(j);
                    QPointF p1 = QPointF(po.x() + span * qCos(j_theta), po.y() + span * qSin(j_theta));
                    plist.append(p1);
                }
            }

            if (plist.count() > 1)
                npoint = CalcVector2D::extendPoint(plist.at(1), plist.at(0), 3.0);
            st_value = STRAIGHTFRAMEVALUE(span, 0, QPointF(), false, false);

        } else if (itype == FrameType::FRAMETYPE::RADIUS) {

            QPointF po = fgitem.basePoint;
            qreal angle = fgitem.startAngle + frameSubModel->getSumAngleOfRow(i);
            qreal theta = angle / 180.0 * M_PI;
            qreal p_len = fgitem.plusLength;

            plist.append(po);
            QPointF p1 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
            plist.append(p1);

            npoint = CalcVector2D::extendPoint(po, p1, 3.0);
            st_value = STRAIGHTFRAMEVALUE(0, angle, QPointF(), false, false);

        } else if (itype == FrameType::FRAMETYPE::DIAMETER) {

            QPointF po = fgitem.basePoint;
            qreal angle = fgitem.startAngle + frameSubModel->getSumAngleOfRow(i);
            qreal theta = angle / 180.0 * M_PI;
            qreal p_len = fgitem.plusLength;
            qreal m_len = fgitem.minusLength;

            QPointF p1 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
            QPointF p2 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
            plist.append(p1);
            plist.append(p2);

            npoint = CalcVector2D::extendPoint(p1, p2, 3.0);
            st_value = STRAIGHTFRAMEVALUE(0, angle, QPointF(), false, false);

        } else if (itype == FrameType::FRAMETYPE::VARIABLE) {

            plist.append(frameSubModel->getVariablePointsOfRow(i));

            if (plist.count() > 1)
                npoint = CalcVector2D::extendPoint(plist.at(1), plist.at(0), 3.0);

        } else if (itype == FrameType::FRAMETYPE::CLCIRCLE) {

            QPointF po = fgitem.basePoint;
            span = frameSubModel->getSumSpanOfRow(i);
            int div = fgitem.division;

            if ( div > 0 ) {
                qreal d_theta = (2.0 * M_PI) / qreal(div);
                for ( int j = 0; j < div; j++ ) {
                    qreal j_theta = d_theta * qreal(j);
                    QPointF p1 = QPointF(po.x() + span * qCos(j_theta), po.y() + span * qSin(j_theta));
                    plist.append(p1);
                }
            }

            if (plist.count() > 1)
                npoint = CalcVector2D::extendPoint(plist.at(1), plist.at(0), 3.0);

        } else if (itype == FrameType::FRAMETYPE::CLVARIABLE) {

            plist.append(frameSubModel->getVariablePointsOfRow(i));

            if (plist.count() > 1)
                npoint = CalcVector2D::extendPoint(plist.at(1), plist.at(0), 3.0);

        }

        QString name = frameSubModel->getFrameName(i);
        QString uid = frameSubModel->getUUIDOfRow(i);

        FRAMEPOINTSVALUE fmp = FRAMEPOINTSVALUE(gid, plist, name, npoint, itype, st_value, uid);
        drawList.append(fmp);

    }

    return drawList;
}

void FrameSettingSubTableWidget::modelUpload()
{
    setEditOn(false);
    frameSubModel->linkUUID();
    frameSubModel->keepCurrentData();
}

void FrameSettingSubTableWidget::modelEdit()
{
    setEditOn(true);
}

void FrameSettingSubTableWidget::modelRewind()
{
    frameSubModel->backPreviousData();
    setEditOn(false);
    emit frameItemChanged();
}

void FrameSettingSubTableWidget::setEditOn(bool onoff)
{
    cutAction->setEnabled(onoff);
    pasteAction->setEnabled(onoff);
    deleteAction->setEnabled(onoff);
    insertAction->setEnabled(onoff);
    upAction->setEnabled(onoff);
    downAction->setEnabled(onoff);

    frameSubModel->setEditOn(onoff);
}

bool FrameSettingSubTableWidget::warningFrameCount()
{
    return Utility::showWarningMessage2(this, u8"通り数を０にすると元に戻せません。\n続けますか？");
}

bool FrameSettingSubTableWidget::warningDeleteLine()
{
    return Utility::showWarningMessage2(this, u8"登録済の通りが選択されています。\n削除しますか？");
}

void FrameSettingSubTableWidget::reflectUnifiedData(){
    DoubleList temp(frameSubModel->columnCount());
    int i = 0;
    Q_FOREACH(FRAMEPOINTSVALUE fp, UnifiedInputData::getInInstance()->getFramePointList()){
        QList<QVariant> data;
        data.append(QVariant(i));
        Q_FOREACH(FRAMEGROUPVALUE fg, UnifiedInputData::getInInstance()->getFrameGroupList()){
            if (fg.GID == fp.GID){
                data.append(QVariant(fg.groupName));
                break;
            }
        }
        data.append(QVariant(FrameType::enumToString(fp.frameType)));
        data.append(fp.frameName);
        data.append(fp.straight.span);
        data.append(fp.straight.angle);
        data.append("1");
        data.append("1");
        data.append(fp.GID.toString());
        data.append(fp.UUID.toString());
        temp.appendRowData(data);
        i++;
    }
    frameSubModel->reflectUnifiedData(temp);
    QStringList gidlist = frameGroupModel->getGIDList();
    setSingnalOn();
    Q_FOREACH (QString gid, gidlist)
        this->slotManageRowCount( gid ); // frameGroupModelのminValueListの更新
}
} // namespace post3dapp
