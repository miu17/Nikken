#include "framesetting_drawgrview.h"

#include <QtGui>

#include "background_floor.h"
#include "calc_simple.h"
#include "factory_unifieddata.h"
#include "unified_inputdata.h"

namespace post3dapp{
FrameSettingDrawGrView::FrameSettingDrawGrView(QWidget *parent)
    : CustomDrawGrView(UnifiedDataType::Input,parent)
{
    initializeFittingRect();

    backFloor = new BackGroundFloor(true, UnifiedInputData::getInInstance());
    backFloor->setColor(QColor(Qt::black), QColor(Qt::blue));
    backFloor->setSpanColorMode(true);

    drawingFloor = QString("");
}

FrameSettingDrawGrView::~FrameSettingDrawGrView()
{
    delete backFloor;
}

void FrameSettingDrawGrView::fitWindow()
{
    QPointF p1 = minPoint + QPointF(-5500., -1500.);
    QPointF p2 = maxPoint + QPointF( 1500., 5500.);
    QRectF bound_rect = QRectF(p1, p2);
    if (!bound_rect.isValid()) bound_rect = QRectF(-10000., -10000., 20000., 20000.);

    fitInView(bound_rect, Qt::KeepAspectRatio);
    qreal scale = transform().m11();
    emit scaleChanged(scale);

}

void FrameSettingDrawGrView::setDrawingData( const QList<FRAMEGROUPVALUE>  &glist,
                                             const QList<FRAMEPOINTSVALUE> &flist)
{
    groupList = glist;
    frameList = flist;
    filterGID();
    calcMaxMinPoint();
    scene()->update();
}

void FrameSettingDrawGrView::setCurrentGID(const QUuid &gid)
{
    backFloor->setCurrentGID(gid);
    scene()->update();
}

void FrameSettingDrawGrView::changeDrawingGID(const QString &fl)
{
    drawingFloor = fl;
    if ( !FactoryUnifiedData::getInstance(isOutput)->isExistedName(fl, DATATYPE::TPFLOOR) )
        drawingFloor = QString() ;
    filterGID();
    scene()->update();
}

void FrameSettingDrawGrView::filterGID()
{
    QList<QUuid> gidlist;

    QUuid fl_id = FactoryUnifiedData::getInstance(isOutput)->nameToID(drawingFloor, DATATYPE::TPFLOOR);
    qreal fl_h = FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(fl_id);

    Q_FOREACH (FRAMEGROUPVALUE fg, groupList) {
        QUuid sf = fg.startFloorID;
        qreal sf_h = ( sf.isNull() ) ? FactoryUnifiedData::getInstance(isOutput)->getMinimumFloorHeight() :
                     FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(sf);
        QUuid ef = fg.endFloorID;
        qreal ef_h = ( ef.isNull() ) ? FactoryUnifiedData::getInstance(isOutput)->getMaximumFloorHeight() :
                     FactoryUnifiedData::getInstance(isOutput)->getFloorHeight(ef);
        if ( fl_id.isNull() || ( fl_h >= sf_h && fl_h <= ef_h ) ) {
            gidlist.append(fg.GID);
        }
    }

    QList<FRAMEPOINTSVALUE> plist;
    Q_FOREACH (FRAMEPOINTSVALUE fp, frameList) {
        if (gidlist.contains(fp.GID)) plist.append(fp);
    }
    backFloor->resetFramePoints(plist);
}

void FrameSettingDrawGrView::drawBackground(QPainter *painter, const QRectF &rect)
{
    CustomDrawGrView::drawBackground(painter, rect);

    backFloor->drawWhole(painter);
}

void FrameSettingDrawGrView::calcMaxMinPoint()
{
    initializeFittingRect();

    Q_FOREACH (FRAMEPOINTSVALUE fp, frameList) {
        Q_FOREACH (QPointF p1, fp.pointList) {
            p1 *= 1000.;
            updateFittingRect(p1);
        }
        QPointF p2 = fp.namePoint;
        p2 *= 1000.;
        updateFittingRect(p2);
    }
}

void FrameSettingDrawGrView::initializeFittingRect()
{
    minPoint = QPointF(1500000., 1500000.);
    maxPoint = QPointF(-1000000., -1000000.);
}

void FrameSettingDrawGrView::updateFittingRect(const QPointF &p1)
{
    qreal min_x = minPoint.x();
    qreal min_y = minPoint.y();
    qreal max_x = maxPoint.x();
    qreal max_y = maxPoint.y();

    if (p1.x() < min_x) {
        min_x = p1.x();
    } else if ( p1.x() > max_x ) {
        max_x = p1.x();
    }

    if (p1.y() < min_y) {
        min_y = p1.y();
    } else if ( p1.y() > max_y ) {
        max_y = p1.y();
    }
    minPoint = QPointF(min_x, min_y);
    maxPoint = QPointF(max_x, max_y);
}
} // namespace post3dapp
