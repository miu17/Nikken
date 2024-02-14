#include "elevationUI_grview.h"

#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <QMenu>

#include "elevationUI_grscene.h"
#include "eventoperation_point.h"
#include "eventoperation_line.h"
#include "eventoperation_plane.h"
#include "eventoperation_editall.h"
#include "eventoperation_wall.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "unified_settings.h"


namespace post3dapp{
ElevationInterfaceGrView::ElevationInterfaceGrView( ElevationInterfaceGrScene *scene,
                                                    QWidget *parent )
    : elevationGrScene(scene), CustomDrawGrView( UnifiedDataType::Input,parent )
{
    setScene(elevationGrScene);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentStatusChanged,
            this, &ElevationInterfaceGrView::importCurrentStatus);

    dummyItem = new QGraphicsLineItem;
    elevationGrScene->addItem(dummyItem);
    dummyItem->setZValue(-10000);
    dummyItem->setFlag(QGraphicsItem::ItemIsSelectable, false);

    pointEventOperation = new PointEventOperation(this);
    lineEventOperation = new LineEventOperation(this);
    planeEventOperation = new PlaneEventOperation(this);
    wallEventOperation = new WallEventOperation(this);
    editAllEventOperation = new EditAllEventOperation(this);
    noneEventOperation = new AbstractEventOperation(this);

    currentEventOperation = pointEventOperation;
    importCurrentStatus();
    connect(elevationGrScene, &InputGrScene::sendSelectedItems, this, &ElevationInterfaceGrView::sendSelectedItems);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::selectTermChanged, elevationGrScene, &InputGrScene::slotSelectTermChanged);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentStatusChanged, elevationGrScene, &InputGrScene::slotOperationSettiingChanged);
}

void ElevationInterfaceGrView::importCurrentStatus()
{
    ELEMENTTYPE el_type = UnifiedEditingData::getInstance()->currentElement();
    ELEMENTGROUP el_group = UnifiedEditingData::getInstance()->elementGroup(el_type);

    AbstractEventOperation *old_operation = currentEventOperation;

    switch ( el_group ) {
    case ELGPOINT:
        currentEventOperation = pointEventOperation;
        break;
    case ELGLINE:
        currentEventOperation = lineEventOperation;
        break;
    case ELGPLANE:
        currentEventOperation = planeEventOperation;
        break;
    case ELGWALL:
        currentEventOperation = wallEventOperation;
        break;
    case ELGALL:
        currentEventOperation = editAllEventOperation;
        break;
    case ELGNONE:
        currentEventOperation = noneEventOperation;
        break;
    default:
        break;
    }

    currentEventOperation->setViewModeOn( old_operation->isViewModeOn() );
}

void ElevationInterfaceGrView::sendSelectedItems()
{
    QList<JointData*> selected_joints=elevationGrScene->selectedJoints(UnifiedEditingData::getInstance()->currentElement());
    QList<MemberData*> selected_members = elevationGrScene->selectedMembers();

    if(selected_joints.count()==0 && selected_members.count()==0
            &&UnifiedEditingData::getInstance()->isNoActiveElements())
        return;

    emit elevationGrScene->itemSelectChanged();
    UnifiedEditingData::getInstance()->changeActiveElements( selected_joints,
                                                             selected_members);


}

void ElevationInterfaceGrView::initializeEventOperation()
{
    mySelectedItems.clear();
    moveSelectedItems.clear();
    currentEventOperation->initializeSettings();
}

void ElevationInterfaceGrView::fitWindow()
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

void ElevationInterfaceGrView::drawForeground(QPainter *painter, const QRectF &rect)
{
    CustomDrawGrView::drawForeground(painter, rect);
    currentEventOperation->execDrawForeground(painter, viewport()->rect());
}

void ElevationInterfaceGrView::drawBackground(QPainter *painter, const QRectF &rect)
{
    CustomDrawGrView::drawBackground(painter, rect);
    currentEventOperation->execDrawBackground(painter, viewport()->rect());
}

void ElevationInterfaceGrView::mousePressEvent (QMouseEvent *event)
{
    if ( event->button() == Qt::MidButton )
        currentEventOperation->toggleViewMode();

    if ( currentEventOperation->isViewModeOn() ) {
        CustomDrawGrView::mousePressEvent(event);
        return;
    }

    setCursor(Qt::ArrowCursor) ;
    currentEventOperation->execMousePress(event);
    /* BatchModeから入力用のMousePress,EditModeからコンテキストメニュー実行後のMousePressがあった場合、以下の処理は飛ばされる */

    if ( currentEventOperation->isBatchInputMode() && event->button() == Qt::LeftButton ) {
        rubberStartPoint = event->pos();
        bool detected = currentEventOperation->existNearlyPoint(rubberStartPoint);
        currentEventOperation->hideRubberBand(detected);
        if ( !currentEventOperation->isHiddenRubberBand() ) {
            QGraphicsView::mousePressEvent(event); // ラバーバンド表示
        }
    }

    if ( currentEventOperation->isSelectItemMode() && event->button() == Qt::LeftButton ) {

        rubberStartPoint = event->pos();
        if ( scene()->selectedItems().isEmpty() ) mySelectedItems.clear();
        if ( !( event->modifiers() == Qt::ControlModifier ) &&
                !( event->modifiers() == Qt::AltModifier ) ) clearSelectedItems();

        QGraphicsItem *detectedItem = NULL;
        QPointF pp = mapToScene(rubberStartPoint);
        Q_FOREACH (QGraphicsItem *item, scene()->items()) {
            QPointF qq = item->mapFromScene(pp);
            if ( ( item->flags() & QGraphicsItem::ItemIsSelectable ) && item->contains(qq) ) {
                detectedItem = item;
            }
        }

        currentEventOperation->hideRubberBand(detectedItem != NULL);
        if ( currentEventOperation->isHiddenRubberBand() ) {
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
    }

}

void ElevationInterfaceGrView::mouseMoveEvent (QMouseEvent *event)
{
    if ( currentEventOperation->isViewModeOn() ) {
        CustomDrawGrView::mouseMoveEvent(event);
        return;
    }

    if ( currentEventOperation->isPositionSnapOn() ) {
        currentEventOperation->changePosition(event->pos());
        dummyItem->setPos(currentEventOperation->currentPosition());
    }

    if ( currentEventOperation->isBatchInputMode() ) {
        currentEventOperation->changeBatchRect(rubberStartPoint, event->pos());
    }

    if ( currentEventOperation->isSelectItemMode() && !currentEventOperation->isHiddenRubberBand() ) {

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
        elevationGrScene->sendSelectedItems();

        return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void ElevationInterfaceGrView::mouseReleaseEvent (QMouseEvent *event)
{
    if ( currentEventOperation->isViewModeOn() ) {
        CustomDrawGrView::mouseReleaseEvent(event);
        return;
    }

    if ( currentEventOperation->isSelectItemMode() ) {
        mySelectedItems = moveSelectedItems;
        if ( currentEventOperation->isHiddenRubberBand() ) return;
    }

    currentEventOperation->execMouseRelease(event);

    QGraphicsView::mouseReleaseEvent (event);
}

void ElevationInterfaceGrView::keyPressEvent ( QKeyEvent *event )
{
    currentEventOperation->execKeyPress(event);
    QGraphicsView::keyPressEvent(event);
}

void ElevationInterfaceGrView::mouseDoubleClickEvent (QMouseEvent *event)
{
    currentEventOperation->execDoubleClick(event);
}
} // namespace post3dapp
