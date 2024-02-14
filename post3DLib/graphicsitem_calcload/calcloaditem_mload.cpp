#include "calcloaditem_mload.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "calc_vector2d.h"
#include "calc_vector3d.h"
#include "unified_settings.h"
namespace post3dapp{
CalcLoadMemberLoadItem::CalcLoadMemberLoadItem( DATATYPE dtype, const QUuid &uid,
                                                const CalculationLoad2DViewSettings &setting,
                                                const FIGURE_TYPE_PROPERTY &figureTypeProperty,
                                                const QList<QPointF> &plist,
                                                qreal tlen,
                                                bool zin,
                                                const QList<MLOADSUM *> &mloads )
    : CalcLoadLineItem(dtype, uid, setting, plist),totalLength(tlen), myLoads(mloads), isZin(zin)
{
    pLoadVisible = setting.getIsPLoadVisible();
    dLoadVisible = setting.getIsDloadVisible();
    wLoadVisible = setting.getIsWloadVisible();

    pLoadColor = setting.getPLoadColor();
    dLoadColor = setting.getDloadColor();
    wLoadColor = setting.getWLoadColor();
    qDebug()<<pLoadColor << dLoadColor<<wLoadColor;

    int psize = setting.getPLoadSize();
    pLoadScale = 200.0 + 50.0 * qreal(psize);
    dLoadScale = 1000.0 / setting.getDloadSize();
    wLoadScale = 1000.0 / setting.getWLoadSize();

    stringSize = setting.getStringSize();

    setStatus(figureTypeProperty);
}


void CalcLoadMemberLoadItem::setStatus(const FIGURE_TYPE_PROPERTY &figureTypeProperty)
{
    LoadType::LOADTYPE ltype = figureTypeProperty.loadType;
    LOADSUMTYPE lstype = figureTypeProperty.loadSumType;
    OUTPUTDIR outdir = figureTypeProperty.viewdirection;

    sumUpLoadPath = QPainterPath();
    concentratedLoadPath = QPainterPath();
    wabLoadPath = QPainterPath();

    Q_FOREACH ( MLOADSUM *mload, myLoads ) {

        if ( mload->loadType != ltype ) continue;
        if ( lstype == LOADSUMTYPE::LOADSUM_AUTO || lstype == LOADSUMTYPE::LOADSUM_SELF ) {
            if ( !mload->loadName.isEmpty() ) continue;
        } else if ( lstype == LOADSUMTYPE::LOADSUM_MANUAL ) {
            if ( mload->loadName.isEmpty() ) continue;
        }

        if ( isZin ) {
            if ( outdir == OUTPUTDIR::ODIR_PLIN && ( mload->loadDir == LOADDIR::LDIR_Y || mload->loadDir == LOADDIR::LDIR_MY ) ) continue;
            if ( outdir == OUTPUTDIR::ODIR_PLOUT && ( mload->loadDir == LOADDIR::LDIR_Z || mload->loadDir == LOADDIR::LDIR_MZ ) ) continue;
        } else {
            if ( outdir == OUTPUTDIR::ODIR_PLOUT && ( mload->loadDir == LOADDIR::LDIR_Y || mload->loadDir == LOADDIR::LDIR_MY ) ) continue;
            if ( outdir == OUTPUTDIR::ODIR_PLIN && ( mload->loadDir == LOADDIR::LDIR_Z || mload->loadDir == LOADDIR::LDIR_MZ ) ) continue;
        }

        if ( outdir == OUTPUTDIR::ODIR_AXIAL ) {
            if ( mload->loadDir != LOADDIR::LDIR_X && mload->loadDir != LOADDIR::LDIR_MX ) continue;
            bool isMoment = ( mload->loadDir == LOADDIR::LDIR_MX );
            if ( !mload->sumUpLoad.isEmpty() && lstype != LOADSUMTYPE::LOADSUM_SELF )
                appendSumUpLoad( mload->loadName, mload->sumUpLoad, isMoment );
            if ( !mload->concentratedLoad.isEmpty() && lstype != LOADSUMTYPE::LOADSUM_SELF )
                appendConcentratedLoad( mload->loadName, mload->concentratedLoad, isMoment );
            if ( ( qAbs(mload->loadWAB.loadWA.y()) > 1.0e-3
                    || qAbs(mload->loadWAB.loadWB.y()) > 1.0e-3 ) && lstype != LOADSUMTYPE::LOADSUM_AUTO )
                appendWABLoad( mload->loadName, mload->loadWAB, isMoment );
        } else {
            if ( mload->loadDir == LOADDIR::LDIR_X || mload->loadDir == LOADDIR::LDIR_MX ) continue;
            bool isMoment = ( mload->loadDir == LOADDIR::LDIR_MY || mload->loadDir == LOADDIR::LDIR_MZ );
            if ( !mload->sumUpLoad.isEmpty() && lstype != LOADSUMTYPE::LOADSUM_SELF )
                appendSumUpLoad( mload->loadName, mload->sumUpLoad, isMoment );
            if ( !mload->concentratedLoad.isEmpty() && lstype != LOADSUMTYPE::LOADSUM_SELF )
                appendConcentratedLoad( mload->loadName, mload->concentratedLoad, isMoment );
            if ( ( qAbs(mload->loadWAB.loadWA.y()) > 1.0e-3
                    || qAbs(mload->loadWAB.loadWB.y()) > 1.0e-3 ) && lstype != LOADSUMTYPE::LOADSUM_AUTO )
                appendWABLoad( mload->loadName, mload->loadWAB, isMoment );
        }

    }

    sumUpLoadPath.closeSubpath();
    concentratedLoadPath.closeSubpath();
    wabLoadPath.closeSubpath();
}

void CalcLoadMemberLoadItem::appendSumUpLoad( const QString &loadName,
                                            const QList<qreal> &loads, bool isMoment )
{
    Q_UNUSED(loadName);

    if ( !dLoadVisible ) return;

    QPainterPath path;

    qreal mband = 500.0; // 部材の分割幅　50cm
    qreal medg = ( totalLength - qreal(loads.count()) * mband ) / 2.0;

    qreal lratio = myLenList.last() / totalLength;
    mband = mband * lratio;
    medg = medg * lratio;

    qreal max_load = 0.0;
    QPointF max_tpoint;
    qreal max_angle = 0.0;
    qreal cur_len = 0.0;
    for ( int i = 0; i < loads.count(); i++ ) {
        qreal cur_load = loads.at(i);
        qreal e_len = cur_len + mband;
        if ( i == 0 || i == loads.count() - 1 ) e_len += medg;
        qreal cratio = dLoadScale / (e_len - cur_len) * 1000.0;

        int idx = 0;
        qreal c_len = cur_len + (e_len - cur_len) / 2.0;
        for ( int j = 0; j < myLenList.count() - 1; j++ ) {
            if ( myLenList.at(j) > c_len - 1.0e-3 ) break;
            idx = j;
        }
        QPointF q1 = myPointList.at(idx);
        QPointF q2 = myPointList.at(idx + 1);

        qreal r0 = (cur_len - myLenList.at(idx)) / (myLenList.at(idx + 1) - myLenList.at(idx));
        QPointF p0 = CalcVector2D::interiorPoint(q1, q2, r0);
        qreal r1 = (e_len - myLenList.at(idx)) / (myLenList.at(idx + 1) - myLenList.at(idx));
        QPointF p1 = CalcVector2D::interiorPoint(q1, q2, r1);

        qreal q_ang = qAtan2((q2.y() - q1.y()), (q2.x() - q1.x()));
        if ( q_ang * 180.0 / M_PI >= 92.0 || q_ang * 180.0 / M_PI <= -88.0 ) q_ang += M_PI;
        qreal p_ang = q_ang + M_PI / 2.0;
        QPointF p2 = p0 + QPointF(-cur_load * cratio * qCos(p_ang), -cur_load * cratio * qSin(p_ang));
        QPointF p3 = p1 + QPointF(-cur_load * cratio * qCos(p_ang), -cur_load * cratio * qSin(p_ang));

        path.moveTo(p0);
        path.lineTo(p1);
        path.lineTo(p3);
        path.lineTo(p2);
        path.lineTo(p0);
        /*
                if( i==0 ) path.moveTo(p0);
                path.lineTo(p2);
                path.lineTo(p3);
                if( i==loads.count()-1 ) path.lineTo(p1);
        */
        cur_len = e_len;

        if ( qAbs(cur_load) > qAbs(max_load) ) {
            max_tpoint = CalcVector2D::interiorPoint(p0, p1, 0.5);
            max_angle = q_ang;
            max_load = cur_load;
        }
    }

    QPointF tp = max_tpoint + QPointF(-max_load * dLoadScale * 2.0 * qCos(max_angle + M_PI / 2.0),
                                      -max_load * dLoadScale * 2.0 * qSin(max_angle + M_PI / 2.0));
    tp.setY(-tp.y()); // テキストはy座標逆転
    QString str = ( isMoment ) ? QString("%1kN*m/m").arg(max_load * 2.0, 0, 'f', 1)
                  : QString("%1kN/m").arg(max_load * 2.0, 0, 'f', 1) ;
    valueTextPos.append( POINT_AND_TEXT(tp, max_angle, str, dLoadColor) );

    path.closeSubpath();
    sumUpLoadPath.addPath(path);
}

void CalcLoadMemberLoadItem::appendConcentratedLoad( const QString &loadName,
                                                   const QList<QPointF> &loads, bool isMoment )
{
    Q_UNUSED(loadName);
    if ( !pLoadVisible ) return;

    QPainterPath path;

    qreal lratio = myLenList.last() / totalLength;

    for ( int i = 0; i < loads.count(); i++ ) {

        qreal cur_len = loads.at(i).x() * 1000.0 * lratio;
        qreal cur_load = loads.at(i).y();

        int idx = 0;
        for ( int j = 0; j < myLenList.count() - 1; j++ ) {
            if ( myLenList.at(j) > cur_len - 1.0e-3 ) break;
            idx = j;
        }
        QPointF q1 = myPointList.at(idx);
        QPointF q2 = myPointList.at(idx + 1);

        qreal r0 = (cur_len - myLenList.at(idx)) / (myLenList.at(idx + 1) - myLenList.at(idx));
        QPointF p0 = CalcVector2D::interiorPoint(q1, q2, r0);

        qreal q_ang = qAtan2((q2.y() - q1.y()), (q2.x() - q1.x()));
        if ( q_ang * 180.0 / M_PI >= 92.0 || q_ang * 180.0 / M_PI <= -88.0 ) q_ang += M_PI;
        qreal p_ang = q_ang + M_PI / 3.0;
        QPointF p1 = p0 + QPointF(pLoadScale * qCos(p_ang), pLoadScale * qSin(p_ang));
        p_ang = q_ang + M_PI * 2.0 / 3.0;
        QPointF p2 = p0 + QPointF(pLoadScale * qCos(p_ang), pLoadScale * qSin(p_ang));

        path.moveTo(p0);
        path.lineTo(p1);
        path.lineTo(p2);
        path.lineTo(p0);

        QString str = ( isMoment ) ? QString("%1kN*m").arg(cur_load, 0, 'f', 1)
                      : QString("%1kN").arg(cur_load, 0, 'f', 1) ;
        QPointF p3 = p0 + QPointF(0.866 * pLoadScale * qCos(q_ang + M_PI / 2.0),
                                  0.866 * pLoadScale * qSin(q_ang + M_PI / 2.0));
        p3.setY(-p3.y()); // テキストはy座標逆転
        valueTextPos.append( POINT_AND_TEXT( p3, q_ang, str, pLoadColor) );
    }

    path.closeSubpath();
    concentratedLoadPath.addPath(path);
}

void CalcLoadMemberLoadItem::appendWABLoad( const QString &loadName,
                                          const WABLOAD &loads, bool isMoment )
{
    Q_UNUSED(loadName);
    if ( !wLoadVisible ) return;

    QPainterPath path;

    qreal lratio = myLenList.last() / totalLength;

    qreal len_A = loads.loadWA.x() * 1000.0 * lratio;
    qreal len_B = loads.loadWB.x() * 1000.0 * lratio;
    qreal load_A = loads.loadWA.y();
    qreal load_B = loads.loadWB.y();

    bool isDoneA = false;
    bool isDoneB = false;

    for ( int i = 0; i < myLenList.count() - 1; i++ ) {

        if ( isDoneB ) break;

        qreal c1_len = myLenList.at(i);
        qreal c2_len = myLenList.at(i + 1);

        bool isOnA = ( len_A > c1_len - 1.0e-3 && len_A < c2_len + 1.0e-3 );
        bool isOnB = ( len_B > c1_len - 1.0e-3 && len_B < c2_len + 1.0e-3 );

        QPointF q1 = myPointList.at(i);
        QPointF q2 = myPointList.at(i + 1);
        qreal q_ang = qAtan2((q2.y() - q1.y()), (q2.x() - q1.x()));
        if ( q_ang * 180.0 / M_PI >= 92.0 || q_ang * 180.0 / M_PI <= -88.0 ) q_ang += M_PI;
        qreal p_ang = q_ang + M_PI / 2.0;

        QPointF p0, p1, p2, p3;

        if ( isOnA ) {
            qreal r0 = (len_A - c1_len) / (c2_len - c1_len);
            p0 = CalcVector2D::interiorPoint(q1, q2, r0);
            p1 = p0 + QPointF(-load_A * wLoadScale * qCos(p_ang), -load_A * wLoadScale * qSin(p_ang));
            path.moveTo(p0);
            path.lineTo(p1);
            isDoneA = true;

            QString str = ( isMoment ) ? QString("%1kN*m/m").arg(load_A, 0, 'f', 1)
                          : QString("%1kN/m").arg(load_A, 0, 'f', 1) ;
            QPointF pp = QPointF( p1.x(), -p1.y() ); // テキストはy座標逆転
            valueTextPos.append( POINT_AND_TEXT( pp, q_ang, str, wLoadColor) );
        }

        if ( !isOnA && isDoneA ) {
            p0 = q1;
            qreal ll = load_A + (load_B - load_A) * (c1_len - len_A) / (len_B - len_A);
            p1 = p0 + QPointF(-ll * wLoadScale * qCos(p_ang), -ll * wLoadScale * qSin(p_ang));
            path.moveTo(p0);
            path.lineTo(p1);
        }

        if ( isOnB ) {
            qreal r0 = (len_B - c1_len) / (c2_len - c1_len);
            p2 = CalcVector2D::interiorPoint(q1, q2, r0);
            p3 = p2 + QPointF(-load_B * wLoadScale * qCos(p_ang), -load_B * wLoadScale * qSin(p_ang));
            path.lineTo(p3);
            path.lineTo(p2);
            isDoneB = true;

            QString str = ( isMoment ) ? QString("%1kN*m/m").arg(load_B, 0, 'f', 1)
                          : QString("%1kN/m").arg(load_B, 0, 'f', 1) ;
            QPointF pp = QPointF( p3.x(), -p3.y() ); // テキストはy座標逆転
            valueTextPos.append( POINT_AND_TEXT( pp, q_ang, str, wLoadColor) );
        }

        if ( !isOnB && isDoneA ) {
            p2 = q2;
            qreal ll = load_A + (load_B - load_A) * (c2_len - len_A) / (len_B - len_A);
            p3 = p2 + QPointF(-ll * wLoadScale * qCos(p_ang), -ll * wLoadScale * qSin(p_ang));
            path.lineTo(p3);
            path.lineTo(p2);
        }

        if ( isDoneA ) path.lineTo(p0);

    }

    path.closeSubpath();
    wabLoadPath.addPath(path);
}

void CalcLoadMemberLoadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
    CalcLoadLineItem::paint(painter, option, widget);
    QPen selectpen = QPen(QBrush(selectColor),0,Qt::SolidLine);
    QPen dpen = QPen(QBrush(dLoadColor),0,Qt::SolidLine);
    QPen ppen = QPen(QBrush(pLoadColor),0,Qt::SolidLine);
    QPen wpen = QPen(QBrush(wLoadColor),0,Qt::SolidLine);

    painter->setBrush(Qt::NoBrush);
    if ( !sumUpLoadPath.isEmpty() ) {
        ( option->state & QStyle::State_Selected ) ?
        painter->setPen(selectpen) : painter->setPen(dpen);
        painter->drawPath(sumUpLoadPath);
    }

    if ( !concentratedLoadPath.isEmpty() ) {
        ( option->state & QStyle::State_Selected ) ?
        painter->setPen(selectpen) : painter->setPen(ppen);
        painter->drawPath(concentratedLoadPath);
    }

    if ( !wabLoadPath.isEmpty() ) {
        ( option->state & QStyle::State_Selected ) ?
        painter->setPen(selectpen) : painter->setPen(wpen);
        painter->drawPath(wabLoadPath);
    }


    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
    painter->setFont(QFont("Consolas", FontSize(*painter, stringSize)));

    Q_FOREACH ( POINT_AND_TEXT pst, valueTextPos ) {
        QPen selectpen = QPen(selectColor,0,painter->pen().style());
        QPen pstpen = QPen(pst.color,0,painter->pen().style());

        ( option->state & QStyle::State_Selected ) ?
        painter->setPen(selectpen) : painter->setPen(pstpen);
        painter->translate(pst.pos.x(), pst.pos.y());
        painter->rotate(-pst.angle * 180.0 / M_PI);
        QRect boundingRect = QFontMetrics(painter->font()).boundingRect(pst.vtext);
        painter->drawText(QRect(-boundingRect.width() / 2.0, -boundingRect.height(), boundingRect.width(), boundingRect.height()),
                          Qt::AlignHCenter | Qt::AlignVCenter, pst.vtext);
        painter->rotate(pst.angle * 180.0 / M_PI);
        painter->translate(-pst.pos.x(), -pst.pos.y());
    }
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);

}
} // namespace post3dapp
