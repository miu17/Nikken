#include "h_braceitem.h"
#include <QPainter>
#include "calc_vector2d.h"
namespace post3dapp{
BraceHLineItem::BraceHLineItem( const Input2DViewSettings& setting,const QList<QPointF> &plist, const QList<qreal> &lenlist,
                                const HLINESHAPE &shape, ITEMVIEWTYPE view_type,
                                const MEMBERPARTITION &partition )
    : GirderHLineItem(setting,ELBRACE, plist, lenlist, shape, view_type, partition)
{
    qreal len_lef = ( myLenList.first() + myLenList.last() ) / 4.0;
    qreal len_rgt = 3.0 * ( myLenList.first() + myLenList.last() ) / 4.0;
    int idx_lef = 0;
    int idx_rgt = 0;
    for (int i = 0; i < myLenList.count() - 1; i++) {
        if ( myLenList.at(i) - 1.0e-3 < len_lef && len_lef < myLenList.at(i + 1) ) {
            idx_lef = i;
            break;
        }
        if ( myLenList.at(i) - 1.0e-3 < len_rgt && len_rgt < myLenList.at(i + 1) ) {
            idx_rgt = i;
            break;
        }
    }
    textAngle = qAtan2(myPointList.at(idx_rgt + 1).y() - myPointList.at(idx_rgt).y(),
                       myPointList.at(idx_rgt + 1).x() - myPointList.at(idx_rgt).x()) * 180.0 / M_PI;

    if ( textAngle >= 0.0 ) {
        qreal rr = (len_lef - myLenList.at(idx_lef)) / (myLenList.at(idx_lef + 1) - myLenList.at(idx_lef));
        textPoint = CalcVector2D::interiorPoint(myPointList.at(idx_lef), myPointList.at(idx_lef + 1), rr);
    } else {
        qreal rr = (len_rgt - myLenList.at(idx_rgt)) / (myLenList.at(idx_rgt + 1) - myLenList.at(idx_rgt));
        textPoint = CalcVector2D::interiorPoint(myPointList.at(idx_rgt), myPointList.at(idx_rgt + 1), rr);
    }

    if ( textAngle >= 92.0 || textAngle <= -88.0 ) textAngle += 180.0;
}
} // namespace post3dapp
