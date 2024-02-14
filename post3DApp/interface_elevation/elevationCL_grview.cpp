#include "elevationCL_grview.h"

#include <QDebug>
#include <QMenu>
#include <QMouseEvent>

#include "calcloadgraphicsitem.h"
#include "elevationCL_grscene.h"
#include "fixed_data.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "textcalcload_dialog.h"

namespace post3dapp{
ElevationCalcLoadGrView::ElevationCalcLoadGrView( ElevationCalcLoadGrScene *scene, QWidget *parent )
    : CustomDrawGrView( UnifiedDataType::FixedInput,parent ), elevationGrScene(scene)
{
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
    connect ( waitingAction, &QAction::triggered, this, &ElevationCalcLoadGrView::slotWaitingSelect );

    QAction *showTextAction = new QAction(u8"テキスト表示", this);
    showTextAction->setShortcut(tr("Ctrl+d"));
    showTextAction->setShortcutContext(Qt::WidgetShortcut);
    connect ( showTextAction, &QAction::triggered, this, &ElevationCalcLoadGrView::slotShowTextDialog );

    QList<QAction *> actionList;
    actionList << waitingAction << showTextAction;

    menu = new QMenu(this);
    menu->addActions(actionList);
    menu->insertSeparator(showTextAction);
    this->addActions(actionList);
}

void ElevationCalcLoadGrView::fitWindow()
{
    QRectF bound_rect = ( scene()->items().count() > 1 ) ? scene()->itemsBoundingRect() :
                        QRectF(-5000., -5000., 20000., 20000.) ;

    /* ルーラー表示の領域を差し引き */
    qreal dx = 20.0 / qreal(viewport()->width() - 20) * qreal(bound_rect.width());
    qreal dy = 20.0 / qreal(viewport()->height() - 20) * qreal(bound_rect.height());

    fitInView(bound_rect.adjusted(-dx, 0, 0, dy), Qt::KeepAspectRatio);
    qreal scale = transform().m11();
    emit scaleChanged(scale);
}
void ElevationCalcLoadGrView::drawBackground(QPainter *painter, const QRectF &rect)
{
    CustomDrawGrView::drawBackground(painter, rect);
    elevationGrScene->drawBackGround(painter);
}
//void ElevationCalcLoadGrView::drawForeground(QPainter *painter, const QRectF &rect)
//{
//    CustomDrawGrView::drawForeground(painter,rect);
//    elevationGrScene->drawForeGround(painter);
//}


void ElevationCalcLoadGrView::mousePressEvent (QMouseEvent *event)
{
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

    setCursor(Qt::ArrowCursor) ;

    if ( event->button() == Qt::LeftButton ) {

        rubberStartPoint = event->pos();
        if ( scene()->selectedItems().isEmpty() ) mySelectedItems.clear();
        if ( !( event->modifiers() == Qt::ControlModifier ) &&
                !( event->modifiers() == Qt::AltModifier ) ) clearSelectedItems();

        QGraphicsItem *detectedItem = nullptr;
        QPointF pp = mapToScene(rubberStartPoint);
        Q_FOREACH (QGraphicsItem *item, scene()->items()) {
            QPointF qq = item->mapFromScene(pp);
            if ( ( item->flags() & QGraphicsItem::ItemIsSelectable ) && item->contains(qq) ) {
                detectedItem = item;
            }
        }

        rubberBandOn = ( detectedItem ==
                         nullptr ); // マウスヒットした先にアイテムがあればラバーバンドOFF
        if ( !rubberBandOn ) {
            if ( ( event->modifiers() == Qt::ControlModifier ) ||
                    ( event->modifiers() == Qt::NoModifier ) ) {
                if ( !mySelectedItems.contains(detectedItem) ) mySelectedItems.append(detectedItem);
            }
            if ( event->modifiers() == Qt::AltModifier ) {
                detectedItem->setSelected(false);
                if ( mySelectedItems.contains(detectedItem) ) mySelectedItems.removeOne(detectedItem);
            }
        } else {
            QGraphicsView::mousePressEvent(event); // ラバーバンド表示
        }

        Q_FOREACH (QGraphicsItem *item, mySelectedItems) item->setSelected(true);
        moveSelectedItems = mySelectedItems;

    } else if ( event->button() == Qt::RightButton ) {

        Q_FOREACH ( QAction *ac, menu->actions() ) ac->setEnabled( !mySelectedItems.isEmpty() );
        menu->exec(QCursor::pos());

    }

}

QPair<QList<JointData *>, QList<MemberData *> > ElevationCalcLoadGrView::getSelectedItems() const
{
    QList<JointData*> jdlist;
    QList<MemberData*> mdlist;
    Q_FOREACH(QGraphicsItem *item, mySelectedItems){
        CalcLoadGraphicsItem *cItem = qgraphicsitem_cast<CalcLoadGraphicsItem *>(item);
        if (cItem->dataType() == DATATYPE::TPJOINT) {
            jdlist.append(UnifiedFixedData::getInstance()->getInputData()->jointIDToPointer(cItem->getUuid()));
        }else{
            mdlist.append(UnifiedFixedData::getInstance()->getInputData()->memberIDToPointer(cItem->getUuid()));
        }
    }
    return QPair<QList<JointData *>, QList<MemberData *>>(jdlist, mdlist);
}

void ElevationCalcLoadGrView::sendSelectionChanged()
{
    auto items = getSelectedItems();
    UnifiedEditingData::getInstance()->changeActiveElements( items.first, items.second );
}

void ElevationCalcLoadGrView::mouseMoveEvent (QMouseEvent *event)
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

        Q_FOREACH ( QGraphicsItem *item, moveSelectedItems ) item->setSelected(true);

        scene()->blockSignals(false);
        return;
    }


    QGraphicsView::mouseMoveEvent(event);
}

void ElevationCalcLoadGrView::mouseReleaseEvent (QMouseEvent *event)
{
    if ( isViewMode ) {
        CustomDrawGrView::mouseReleaseEvent(event);
        return;
    }

    mySelectedItems = moveSelectedItems;
    rubberBandOn = false;
    sendSelectionChanged();
}

void ElevationCalcLoadGrView::slotWaitingSelect()
{
    scene()->clearSelection();
    mySelectedItems.clear();
    sendSelectionChanged();
}

void ElevationCalcLoadGrView::slotShowTextDialog()
{
    TextCalcLoadDialog dialog(this);
    auto items = getSelectedItems();
    dialog.setEditorText(items.first, items.second);
    dialog.exec();

}
} // namespace post3dapp
