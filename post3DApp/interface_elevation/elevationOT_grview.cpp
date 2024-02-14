#include "elevationOT_grview.h"

#include <QDebug>
#include <QGraphicsLineItem>
#include <QMouseEvent>
#include <QMenu>

#include "elevationOT_grscene.h"
#include "fixed_data.h"
#include "nmint_dialog.h"
#include "output_graphicsitem.h"
#include "textoutput_dialog.h"
#include "unified_analysisdata.h"
#include "unified_editingdata.h"

namespace post3dapp{
ElevationOutputGrView::ElevationOutputGrView( ElevationOutputGrScene *scene, QWidget *parent )
    : CustomDrawGrView( UnifiedDataType::Pstn,parent ),elevationGrScene(scene)
{
    qDebug()<<"ElevationOutputGrView::constractor in";
    setScene(elevationGrScene);
    isViewMode = true;
    rubberBandOn = false;

    dummyItem = new QGraphicsLineItem;
    elevationGrScene->addItem(dummyItem);
    dummyItem->setZValue(-10000);
    dummyItem->setFlag(QGraphicsItem::ItemIsSelectable, false);

    QAction *waitingAction = new QAction(u8"選択解除", this);
    waitingAction->setShortcut(tr("Esc"));
    waitingAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( waitingAction, &QAction::triggered, this, &ElevationOutputGrView::slotWaitingSelect );

    QAction *showTextAction = new QAction(u8"部材情報表示", this);
    showTextAction->setShortcut(tr("Ctrl+d"));
    showTextAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( showTextAction, &QAction::triggered, this, &ElevationOutputGrView::slotShowTextDialog );

    QAction *showNmintAction = new QAction(u8"NM-インタラクションの表示", this);
    showNmintAction->setShortcut(tr("Ctrl+n"));
    showNmintAction->setShortcutContext(Qt::WidgetShortcut);
    showNmintAction->setObjectName(u8"showNmint");
    connect ( showNmintAction, &QAction::triggered, this, &ElevationOutputGrView::slotShowNmintDialog );

    QList<QAction *> actionList;
    actionList << waitingAction << showTextAction<<showNmintAction;

//    selection = new QMenu(u8"選択",this);//（〇つから1つを選択。0選択の時グレー）

    menu = new QMenu(this);
//    menu->addMenu(selection);
    menu->addActions(actionList);
    menu->insertSeparator(showTextAction);
    menu->insertSeparator(showNmintAction);
    this->addActions(actionList);
    qDebug()<<"ElevationOutputGrView::constractor out";

}

void ElevationOutputGrView::fitWindow()
{
    qDebug()<<"ElevationOutputGrView::fitWindow() : in";
    QRectF bound_rect = ( scene()->items().count() > 1 ) ? scene()->itemsBoundingRect() :
                        QRectF(-5000., -5000., 20000., 20000.) ;

    /* ルーラー表示の領域を差し引き */
    qreal dx = 20.0 / qreal(viewport()->width() - 20) * qreal(bound_rect.width());
    qreal dy = 20.0 / qreal(viewport()->height() - 20) * qreal(bound_rect.height());

    fitInView(bound_rect.adjusted(-dx, 0, 0, dy), Qt::KeepAspectRatio);
    qreal scale = transform().m11();
    emit scaleChanged(scale);
    qDebug()<<"ElevationOutputGrView::fitWindow() : out";
}
void ElevationOutputGrView::drawBackground(QPainter *painter, const QRectF &rect)
{
    qDebug()<<"ElevationOutputGrView::drawBackGround() : in";
    CustomDrawGrView::drawBackground(painter, rect);
    elevationGrScene->drawBackGround(painter);
    qDebug()<<"ElevationOutputGrView::drawBackGround() : out";
}


void ElevationOutputGrView::mousePressEvent (QMouseEvent *event)
{
    qDebug()<<"ElevationOutputGrView::mousePressEvent() : in";
    tmpItems.clear();
    tmpItemIndex = 0;

    if ( event->button() == Qt::MidButton ) {
        isViewMode = !isViewMode;
        if ( isViewMode ) {
            scene()->clearSelection();
            mySelectedItems.clear();
        }
    }

    if ( isViewMode ) {
        CustomDrawGrView::mousePressEvent(event);
        return;
    }

    qDebug()<<"a";
    setCursor(Qt::ArrowCursor) ;

    if ( event->button() == Qt::LeftButton ) {

        rubberStartPoint = event->pos();
        if ( scene()->selectedItems().isEmpty() ) mySelectedItems.clear();
        if ( !( event->modifiers() == Qt::ControlModifier ) &&
                !( event->modifiers() == Qt::AltModifier ) ) clearSelectedItems();

        qDebug()<<"b";
        QGraphicsItem *detectedItem = NULL;
        QList<QGraphicsItem *> detectedItems;
        QPointF pp = mapToScene(rubberStartPoint);
        Q_FOREACH (QGraphicsItem *item, scene()->items()) {
            QPointF qq = item->mapFromScene(pp);
            if ( ( item->flags() & QGraphicsItem::ItemIsSelectable ) && item->contains(qq) ) {
                detectedItem = item;//最後のやつだけヒット
                detectedItems<<item;
            }
        }
        qDebug()<<"b";
        if(detectedItems.count() >= 2){
            QMenu *selection = new QMenu(this);
            int count = 0;
            Q_FOREACH ( QGraphicsItem *item,detectedItems) {
                OutputGraphicsItem *i_item = qgraphicsitem_cast<OutputGraphicsItem *>(item);
                QAction* act = new QAction(QString::number(count+1)+" : "+i_item->getShortTip(),this);
                act->setObjectName(u8"Select_"+QString::number(count));
                selection->addAction(act);
                tmpItems<<item;
                count++;

            }
            connect ( selection, &QMenu::triggered, this, &ElevationOutputGrView::slotSelectOne );
            connect ( selection, &QMenu::hovered, this, &ElevationOutputGrView::slotSelectHighLight );
            selection->exec(QCursor::pos());
            detectedItem = detectedItems.at(tmpItemIndex);
        }
        qDebug()<<"c";

        rubberBandOn = ( detectedItem ==
                         NULL ); // マウスヒットした先にアイテムがあればラバーバンドOFF
        if ( !rubberBandOn ) {
            if ( ( event->modifiers() == Qt::ControlModifier ) ||
                    ( event->modifiers() == Qt::NoModifier ) ) {
                if ( !mySelectedItems.contains(detectedItem) ) {
                    mySelectedItems.append(detectedItem);
                }
            }
            else if ( event->modifiers() == Qt::AltModifier ) {
                detectedItem->setSelected(false);
                if ( mySelectedItems.contains(detectedItem) ) {
                    mySelectedItems.removeOne(detectedItem);
                }
            }
            else {
                mySelectedItems.append(detectedItem);
            }
            slotSelectionChanged();
        } else {
            QGraphicsView::mousePressEvent(event); // ラバーバンド表示
        }
        qDebug()<<"d";

        Q_FOREACH (QGraphicsItem *item, mySelectedItems) item->setSelected(true);
        moveSelectedItems = mySelectedItems;

    } else if ( event->button() == Qt::RightButton ) {

        Q_FOREACH ( QAction *ac, menu->actions() ){
            ac->setEnabled( !mySelectedItems.isEmpty() );
            if(ac->objectName()=="showNmint" ){
                int count=0;
                Q_FOREACH ( QGraphicsItem *item, mySelectedItems ) {
                    OutputGraphicsItem *i_item = qgraphicsitem_cast<OutputGraphicsItem *>(item);
                    if ( i_item->dataType() == DATATYPE::TPGIRDER
                         || i_item->dataType() == DATATYPE::TPCOLUMN
                         ||i_item->dataType() == DATATYPE::TPBRACE) {
                        count++;
                    }
                }
                if(count != 1)ac->setEnabled(false);
            }
        }
        menu->exec(QCursor::pos());

    }
    qDebug()<<"ElevationOutputGrView::mousePressEvent() : out";
}
void ElevationOutputGrView::slotSelectOne (QAction* act){
    qDebug()<<"ElevationOutputGrView::slotSelectOne() : in";
    //選択されたアクションに対応するIndexをtmpに格納
    //ObjectNameでやりとりしているので、あまりきれいな実装ではない
    tmpItemIndex = act->objectName().split('_').last().toInt();
    qDebug()<<"ElevationOutputGrView::slotSelectOne() : out";
}
void ElevationOutputGrView::slotSelectHighLight (QAction* act){
    qDebug()<<"ElevationOutputGrView::slotSelectHightLight() : in";
    //ホバーされているものをハイライト。ホバーが外れた場合は戻す。
    int ind = act->objectName().split('_').last().toInt();
    for(int i=0;i<tmpItems.count();i++){
        QGraphicsItem* item = tmpItems[i];
        if(i==ind){
            item->setSelected(true);
        }else if(i != ind && !mySelectedItems.contains(item)){//もともと選択されていたものはそのまま
            item->setSelected(false);
        }
    }
    qDebug()<<"ElevationOutputGrView::slotSelectHightLight() : out";
}


void ElevationOutputGrView::mouseMoveEvent (QMouseEvent *event)
{
    if ( isViewMode ) {
        CustomDrawGrView::mouseMoveEvent(event);
        return;
    }

    if ( rubberBandOn ) {

        scene()->blockSignals(true);

        moveSelectedItems = mySelectedItems;
        scene()->clearSelection();
        QGraphicsView::mouseMoveEvent(event);

        QList<QGraphicsItem *> currentSelectedItems = scene()->selectedItems();
        if ( event->modifiers() == Qt::ControlModifier ) {
            Q_FOREACH ( QGraphicsItem *item, currentSelectedItems )
                if ( !moveSelectedItems.contains(item) ) moveSelectedItems.append(item);
        } else if ( event->modifiers() == Qt::AltModifier ) {
            Q_FOREACH ( QGraphicsItem *item, currentSelectedItems ) {
                if ( moveSelectedItems.contains(item) ) moveSelectedItems.removeOne(item);
                item->setSelected(false);
            }
        } else if ( event->modifiers() == Qt::NoModifier ) {
            moveSelectedItems = currentSelectedItems;
        }

        Q_FOREACH ( QGraphicsItem *item, moveSelectedItems ) {
            item->setSelected(true);
        }

        scene()->blockSignals(false);
        return;
    }


    QGraphicsView::mouseMoveEvent(event);
}

void ElevationOutputGrView::mouseReleaseEvent (QMouseEvent *event)
{
    if ( isViewMode ) {
        CustomDrawGrView::mouseReleaseEvent(event);
        return;
    }

    mySelectedItems = moveSelectedItems;
    rubberBandOn = false;
    slotSelectionChanged();

}

void ElevationOutputGrView::slotWaitingSelect()
{
    scene()->clearSelection();
    mySelectedItems.clear();
    slotSelectionChanged();
}

void ElevationOutputGrView::slotSelectionChanged()
{
        QList<JointData*> jdlist;
        QList<MemberData*> mdlist;
        QList<SpringData*> sdlist;
        Q_FOREACH ( QGraphicsItem *item, mySelectedItems ) {
            OutputGraphicsItem *i_item = qgraphicsitem_cast<OutputGraphicsItem *>(item);
            if ( i_item->dataType() == DATATYPE::TPJOINT ) {
                jdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(i_item->getUuid()));
            }else if ( i_item->dataType() == DATATYPE::TPSPRING ) {
                sdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->springIDToPointer(i_item->getUuid()));
            } else {
                mdlist.append(UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(i_item->getUuid()));
            }
        }
        //UnifiedEditingDataにAnalysis系のデータ持たせる？変じゃない？
        UnifiedEditingData::getInstance()->changeActiveElements( jdlist, mdlist );
}

void ElevationOutputGrView::slotShowTextDialog()
{
    TextOutputDialog dialog(this);

    QList<QUuid> jdlist, mdlist, sdlist;
    Q_FOREACH ( QGraphicsItem *item, mySelectedItems ) {
        OutputGraphicsItem *i_item = qgraphicsitem_cast<OutputGraphicsItem *>(item);
        if ( i_item->dataType() == DATATYPE::TPJOINT ) {
            jdlist.append( i_item->getUuid() );
        } else if(i_item->dataType() == DATATYPE::TPSPRING) {
            sdlist.append( i_item->getUuid() );
        } else {
            mdlist.append( i_item->getUuid() );
        }
    }

    dialog.setEditorText( elevationGrScene->getFigureTypeProperty(), jdlist, mdlist,elevationGrScene->getFrameFloorId());
    dialog.resize(300,400);
    dialog.exec();

}
void ElevationOutputGrView::slotShowNmintDialog()
{
    //部材一つだけ選択している場合のみ。
    NmintDialog* dialog=new NmintDialog(this);

    Q_FOREACH ( QGraphicsItem *item, mySelectedItems ) {
        OutputGraphicsItem *i_item = qgraphicsitem_cast<OutputGraphicsItem *>(item);
        if ( i_item->dataType() == DATATYPE::TPGIRDER
             || i_item->dataType() == DATATYPE::TPCOLUMN
             ||i_item->dataType() == DATATYPE::TPBRACE) {
            dialog->setMemberId(i_item->getUuid());
            dialog->updateGraph();
            dialog->update3dGraph();
            break;
        }
    }
    dialog->setModal(false);
    dialog->show();

}

} // namespace post3dapp
