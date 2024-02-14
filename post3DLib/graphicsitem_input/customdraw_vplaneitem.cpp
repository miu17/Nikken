#include "customdraw_vplaneitem.h"

#include <QDebug>
#include <QPainter>

#include "calc_vector2d.h"
namespace post3dapp{
CustomDrawVPlaneItem::CustomDrawVPlaneItem(bool _isPlan, const Input2DViewSettings &setting,ELEMENTTYPE type, const QList<QPointF> &plist,
                                           const QString &str, bool onplane, qreal thickness, qreal shift_z)
    : CustomDrawItem(setting,type, str, ITEMTYPE::VPLANEITEM), isOnPlane(onplane), myThickness(thickness), shiftZ(shift_z), myElementDir(ELEMENTDIR::ELCROSS)
{
    if (!_isPlan){
        normalColor = setting.getCrossElementView().lineColor;
        normalBrush = setting.getCrossElementView().paintBrush;
        isNameVisible &= setting.getCrossElementView().isNameVisible;
    }

    if ( setting.getIsPriorIndividual() ) {
        if ( setting.existIndividualSetting(sectionName) ) {
            normalColor = setting.getIndividualSetting(sectionName).lineColor;
            normalBrush = setting.getIndividualSetting(sectionName).lineColor;
        }
    }

    QPointF p0 = plist.first();
    myPointList.clear();
    qreal xmin = p0.x();
    qreal xmax = p0.x();
    qreal ymin = p0.y();
    qreal ymax = p0.y();
    Q_FOREACH (QPointF p1, plist) {
        if ( p1.x() < xmin ) xmin = p1.x();
        if ( p1.x() > xmax ) xmax = p1.x();
        if ( p1.y() < ymin ) ymin = p1.y();
        if ( p1.y() > ymax ) ymax = p1.y();
        myPointList.append(p1 - p0);
    }
    if ( (xmax - xmin) < 5000.0 ) {
        xmin = (xmax + xmin) / 2.0 - 2500.0;
        xmax = xmin + 5000.0;
    }
    if ( (ymax - ymin) < 5000.0 ) {
        ymin = (ymax + ymin) / 2.0 - 2500.0;
        ymax = ymin + 5000.0;
    }
    appRect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
}

bool CustomDrawVPlaneItem::approximatelyContains(QPointF pp) const
{
    return appRect.contains(pp);
}

void CustomDrawVPlaneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *)
{
    setPainter(painter, option);

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->drawPath(drawShapePath);

    if ( !isNameVisible)
        return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->translate(textPoint.x(), -textPoint.y());

    painter->setFont(QFont("Consolas", FontSize(*painter)));
    QRect boundingRect = QFontMetrics(painter->font()).boundingRect(textName);
    painter->rotate(-textAngle);
    painter->drawText(QRect(-boundingRect.width() / 2.0,  -textShift, boundingRect.width(), boundingRect.height()),
                      Qt::AlignCenter, textName);
}

bool CustomDrawVPlaneItem::isOnLinePoint(QPointF pp, bool exceptPoint) const
{
    if ( exceptPoint
            && ( CalcVector2D::length( myPointList.first() + this->pos() - pp ) < 1.0e-3 ) ) return false;
    for (int i = 0; i < myPointList.count() - 1 ; i++) {
        QPointF p1 = myPointList.at(i) + this->pos();
        QPointF p2 = myPointList.at(i + 1) + this->pos();
        if ( exceptPoint && ( CalcVector2D::length( p2 - pp ) < 1.0e-3 ) ) return false;
        if ( CalcVector2D::between(p1, p2, pp, 1.0e-3) ) return true;
    }
    return false;
}

void CustomDrawVPlaneItem::setDrawShape(QPainterPath& path, qreal drawWidth, qreal center)
{
    path = QPainterPath();
    for ( int i = 0; i < myPointList.count() - 1 ; i++ ) {
        QPointF p1 = myPointList.at(i);
        QPointF p2 = myPointList.at(i + 1);
        qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());
        qreal b1 = center + drawWidth / 2.0 + shiftZ;
        qreal b2 = center - drawWidth / 2.0 + shiftZ;
        qreal cs = qCos(ag - M_PI / 2.0);
        qreal si = qSin(ag - M_PI / 2.0);

        QPointF q1 = p1 + QPointF(b1 * cs, b1 * si);
        QPointF q2 = p2 + QPointF(b1 * cs, b1 * si);
        QPointF q3 = p2 + QPointF(b2 * cs, b2 * si);
        QPointF q4 = p1 + QPointF(b2 * cs, b2 * si);

        path.addPolygon(QPolygonF{QVector<QPointF>{q1, q2, q3, q4, q1}});
    }

    path.closeSubpath();
}
void CustomDrawVPlaneItem::setTextProperty(qreal drawWidth, qreal center){
    // 符号描画の諸元（パスに含まず）
    const qreal len_mid = myLenList.last() / 2.0;
    const qreal eps = 1.0e-3;
    int idx_mid = 0;
    for (int i = 0; i < myLenList.count() - 1; i++) {
        if ( myLenList.at(i) - eps < len_mid && len_mid < myLenList.at(i + 1) ) {
            idx_mid = i;
            break;
        }
    }
    textAngle = qAtan2(myPointList.at(idx_mid + 1).y() - myPointList.at(idx_mid).y(),
                       myPointList.at(idx_mid + 1).x() - myPointList.at(idx_mid).x()) * 180.0 / M_PI;
    const qreal rr = (len_mid - myLenList.at(idx_mid)) / (myLenList.at(idx_mid + 1) - myLenList.at(idx_mid));
    textPoint = CalcVector2D::interiorPoint(myPointList.at(idx_mid), myPointList.at(idx_mid + 1), rr);

    textShift = - drawWidth / 2.0 - center;
    if ( textAngle < 92.0 && textAngle > -88.0 ) {
        textShift -= shiftZ;
    } else {
        textAngle += 180.0;
        textShift += shiftZ;
    }
}
} // namespace post3dapp
