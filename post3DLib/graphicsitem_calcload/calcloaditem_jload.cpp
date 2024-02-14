#include "calcloaditem_jload.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>
#include <qmath.h>

#include "calc_vector3d.h"
#include "unified_settings.h"
namespace post3dapp{
CalcLoadJointLoadItem::CalcLoadJointLoadItem( DATATYPE dtype,
                                              const QUuid &uid,
                                              const CalculationLoad2DViewSettings &setting,
                                              const FIGURE_TYPE_PROPERTY &figureTypeProperty,
                                              int inum,
                                              VEC3D vec,
                                              const QList<JLOADSUM *> &jloads )
    : CalcLoadJointItem( dtype, uid, setting,inum ), myLoads(jloads), normVec(vec)
{

    stringSize = setting.getStringSize();
    valueSize = setting.getJLoadSize();
    jLoadVisible = setting.getJLoadVisible();
    jLoadColor = setting.getJLoadColor();

    setStatus(figureTypeProperty);
}

void CalcLoadJointLoadItem::setStatus(const FIGURE_TYPE_PROPERTY &figureTypeProperty)
{
    LoadType::LOADTYPE ltype = figureTypeProperty.loadType;
    LOADSUMTYPE lstype = figureTypeProperty.loadSumType;
    OUTPUTDIR outdir = figureTypeProperty.viewdirection;

    drawAutoLoadPath = QPainterPath();
    drawManualLoadPath = QPainterPath();
    valueTextPos.clear();

    if ( outdir == OUTPUTDIR::ODIR_AXIAL ) return;

    bool isXin = ( qAbs(normVec.x) < qAbs(normVec.y) );

    Q_FOREACH ( JLOADSUM *jload, myLoads ) {

        if ( jload->loadType != ltype ) continue;
        if ( lstype == LOADSUMTYPE::LOADSUM_SELF ) continue;
        if ( lstype == LOADSUMTYPE::LOADSUM_AUTO && !jload->loadName.isEmpty() ) continue;
        if ( lstype == LOADSUMTYPE::LOADSUM_MANUAL && jload->loadName.isEmpty()) continue;

        QString str;

        if ( normVec.z > 0.99999 ) {
            if ( outdir == OUTPUTDIR::ODIR_PLIN && ( jload->loadDir == LOADDIR::LDIR_Z || jload->loadDir == LOADDIR::LDIR_MZ ) ) continue;
            if ( outdir == OUTPUTDIR::ODIR_PLOUT && ( jload->loadDir == LOADDIR::LDIR_X || jload->loadDir == LOADDIR::LDIR_MX ) ) continue;
            if ( outdir == OUTPUTDIR::ODIR_PLOUT && ( jload->loadDir == LOADDIR::LDIR_Y || jload->loadDir == LOADDIR::LDIR_MY ) ) continue;

            switch ( jload->loadDir ) {
            case LOADDIR::LDIR_X:
                str += QString("%1kN(X)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendArrow( str, false, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_Y:
                str += QString("%1kN(Y)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendArrow( str, false, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_Z:
                str += QString("%1kN(Z)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendArrow( str, true, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_MX:
                str += QString("%1kN*m(X)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendDoubleArrow( str, false, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_MY:
                str += QString("%1kN*m(Y)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendDoubleArrow( str, false, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_MZ:
                str += QString("%1kN*m(Z)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendDoubleArrow( str, true, !jload->loadName.isEmpty() );
                break;
            default:
                break;
            }
        } else {
            if ( outdir == OUTPUTDIR::ODIR_PLOUT && ( jload->loadDir == LOADDIR::LDIR_Z || jload->loadDir == LOADDIR::LDIR_MZ ) ) continue;
            bool xok = ( ( outdir == OUTPUTDIR::ODIR_PLIN && isXin ) || ( outdir == OUTPUTDIR::ODIR_PLOUT && !isXin ) );
            bool yok = ( ( outdir == OUTPUTDIR::ODIR_PLIN && !isXin ) || ( outdir == OUTPUTDIR::ODIR_PLOUT && isXin ) );
            if ( !xok && ( jload->loadDir == LOADDIR::LDIR_X || jload->loadDir == LOADDIR::LDIR_MX ) ) continue;
            if ( !yok && ( jload->loadDir == LOADDIR::LDIR_Y || jload->loadDir == LOADDIR::LDIR_MY ) ) continue;

            switch ( jload->loadDir ) {
            case LOADDIR::LDIR_X:
                str += QString("%1kN(X)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendArrow( str, false, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_Y:
                str += QString("%1kN(Y)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendArrow( str, false, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_Z:
                str += QString("%1kN(Z)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendArrow( str, true, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_MX:
                str += QString("%1kN*m(X)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendDoubleArrow( str, false, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_MY:
                str += QString("%1kN*m(Y)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendDoubleArrow( str, false, !jload->loadName.isEmpty() );
                break;
            case LOADDIR::LDIR_MZ:
                str += QString("%1kN*m(Z)").arg(jload->concentratedLoad, 0, 'f', 1);
                appendDoubleArrow( str, true, !jload->loadName.isEmpty() );
                break;
            default:
                break;
            }
        }
    }
    drawManualLoadPath.closeSubpath();
    drawAutoLoadPath.closeSubpath();
}

void CalcLoadJointLoadItem::appendArrow( const QString &str, bool isUpPos, bool isManual )
{
    qDebug()<<"appendArrow init";

    QPainterPath path;

    qreal value = 200.0 + 50.0 * qreal(valueSize);
    QPointF p0 = this->pos();
    ( isManual ) ? p0 += QPointF(-0.1 * value, -0.1 * value) : p0 += QPointF(0.1 * value,
                                                                             -0.1 * value) ;
//    QColor color=( isManual ) ? Qt::red : Qt::blue ;

    if ( isUpPos ) {
        QPointF p2 = p0 + QPointF(  0.5 * value, 0.866 * value );
        QPointF p3 = p0 + QPointF( -0.5 * value, 0.866 * value );
        path.moveTo(p0);
        path.lineTo(p2);
        path.lineTo(p3);
        path.lineTo(p0);
        valueTextPos.append( POINT_AND_TEXT(p0 + QPointF(0.0, -value), 0.0, str, jLoadColor) );
    } else {
        QPointF p2 = p0 + QPointF( -0.866 * value, 0.5 * value );
        QPointF p3 = p0 + QPointF( -0.866 * value, -0.5 * value );
        path.moveTo(p0);
        path.lineTo(p2);
        path.lineTo(p3);
        path.lineTo(p0);
        valueTextPos.append( POINT_AND_TEXT(p0 + QPointF(-value, 0), M_PI / 2.0, str, jLoadColor) );
    }
    path.closeSubpath();

    ( isManual ) ? drawManualLoadPath.addPath(path) : drawAutoLoadPath.addPath(path) ;
}

void CalcLoadJointLoadItem::appendDoubleArrow( const QString &str, bool isUpPos, bool isManual )
{
    qDebug()<<"appendDoubleArrow init";

    QPainterPath path;

    qreal value = 200.0 + 50.0 * qreal(valueSize);
    QPointF p0 = this->pos();
    ( isManual ) ? p0 += QPointF(-0.1 * value, -0.1 * value) : p0 += QPointF(0.1 * value,
                                                                             -0.1 * value) ;

    if ( isUpPos ) {
        QPointF p2 = p0 + QPointF(  0.5 * value, 0.866 * value );
        QPointF p3 = p0 + QPointF( -0.5 * value, 0.866 * value );
        QPointF p4 = p0 + QPointF(        0.0, 0.333 * value );
        QPointF p5 = p4 + QPointF(  0.5 * value, 0.866 * value );
        QPointF p6 = p4 + QPointF( -0.5 * value, 0.866 * value );
        path.moveTo(p0);
        path.lineTo(p2);
        path.moveTo(p0);
        path.lineTo(p3);
        path.moveTo(p4);
        path.lineTo(p5);
        path.moveTo(p4);
        path.lineTo(p6);

        valueTextPos.append( POINT_AND_TEXT( p0 + QPointF(0.0, -value), 0.0, str, jLoadColor) );
    } else {
        QPointF p2 = p0 + QPointF( -0.866 * value, 0.5 * value );
        QPointF p3 = p0 + QPointF( -0.866 * value, -0.5 * value );
        QPointF p4 = p0 + QPointF( -0.333 * value,       0.0 );
        QPointF p5 = p4 + QPointF( -0.866 * value, 0.5 * value );
        QPointF p6 = p4 + QPointF( -0.866 * value, -0.5 * value );
        path.moveTo(p0);
        path.lineTo(p2);
        path.moveTo(p0);
        path.lineTo(p3);
        path.moveTo(p4);
        path.lineTo(p5);
        path.moveTo(p4);
        path.lineTo(p6);

        valueTextPos.append( POINT_AND_TEXT( p0 + QPointF(-value, 0.0), M_PI / 2.0, str, jLoadColor) );
    }
    path.closeSubpath();

    ( isManual ) ? drawManualLoadPath.addPath(path) : drawAutoLoadPath.addPath(path) ;
}

void CalcLoadJointLoadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                QWidget *widget)
{
    qDebug()<<"paint output init";

    CalcLoadJointItem::paint(painter, option, widget);

    if ( !jLoadVisible ) return;

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(Qt::NoBrush);
    QPen selectpen = QPen(QBrush(selectColor),0,Qt::SolidLine);
    QPen jloadpen = QPen(QBrush(jLoadColor),0,Qt::SolidLine);

    if ( !drawManualLoadPath.isEmpty() ) {
        ( option->state & QStyle::State_Selected ) ?
        painter->setPen(selectpen) : painter->setPen(jloadpen);
        painter->drawPath(drawManualLoadPath);
    }

    if ( !drawAutoLoadPath.isEmpty() ) {
        ( option->state & QStyle::State_Selected ) ?
        painter->setPen(selectpen) : painter->setPen(jloadpen) ;
        painter->drawPath(drawAutoLoadPath) ;
    }

    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);

    painter->setFont(QFont("Consolas", FontSize(*painter, stringSize)));

    Q_FOREACH ( POINT_AND_TEXT pst, valueTextPos ) {
        ( option->state & QStyle::State_Selected ) ?
        painter->setPen(selectpen) : painter->setPen(jloadpen);
        painter->translate(pst.pos.x(), pst.pos.y());
        painter->rotate(-pst.angle * 180.0 / M_PI);
        QRect boundingRect = QFontMetrics(painter->font()).boundingRect(pst.vtext);
        painter->drawText(QRect(0, -boundingRect.height(), boundingRect.width(), boundingRect.height()),
                          Qt::AlignLeft | Qt::AlignVCenter, pst.vtext);
        painter->rotate(pst.angle * 180.0 / M_PI);
        painter->translate(-pst.pos.x(), -pst.pos.y());
    }
    painter->setTransform(QTransform(1, 0, 0, -1, 0, 0), true);
}
} // namespace post3dapp
