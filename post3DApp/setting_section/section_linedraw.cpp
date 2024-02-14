#include "section_linedraw.h"

#include <QtGui>
#include <QDebug>

namespace post3dapp{
const qreal SectionLineDraw::EPS = 1.0e-5;

SectionLineDraw::SectionLineDraw( const SectionDataType &type, QWidget *parent )
    : QFrame( parent ), mySectionType( type )
{
    this->setFrameStyle(StyledPanel | Plain);
    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(pal);
    this->setAutoFillBackground(true);
}

qreal SectionLineDraw::calcScaleFactor(qreal Dy, qreal Dz, qreal ry, qreal rz, qreal scaleFactor)
{
    if (scaleFactor < EPS) {
        scaleFactor = 1000.0;
        mPoint = QPointF(0., 0.);
    }

    if ( qAbs(Dy) < EPS  )
        return scaleFactor;
    if ( qAbs(Dz) < EPS  )
        Dz = Dy;

    if (  Dy / 2. + ry > mPoint.x() ) // 右に表示
        mPoint.setX( Dy / 2. + ry);
    if (  -Dz / 2. + rz < mPoint.y() ) // 下に表示
        mPoint.setY( -Dz / 2. + rz);
    if (drawflag == drawFlag::IM || drawflag == drawFlag::IJ) {
        scaleFactor = qMin(qreal(width() * 0.25 - 50) / (Dy / 2. + qAbs(ry)), scaleFactor);
    } else if (drawflag == drawFlag::I) {
        scaleFactor = qMin(qreal(width() * 0.5 - 50) / (Dy / 2. + qAbs(ry)), scaleFactor);
    } else if (drawflag == drawFlag::IMJ) {
        scaleFactor = qMin(qreal(width() * 0.1667 - 50) / (Dy / 2. + qAbs(ry)), scaleFactor);
    }
    return qMin(qreal(height() * 0.5 - 60) / (Dz / 2. + qAbs(rz)), scaleFactor);
}

void SectionLineDraw::drawScale(qreal minX, qreal maxX, qreal minY, qreal maxY,
                                qreal ox, qreal oy, SCALEDW nd, QPainter *pt, qreal scaleFactor) const
{
    qreal margin;
    switch (nd) {
    case SCALEDW::D1N1:
        margin = 35;
        break;
    case SCALEDW::D2N1:
        margin = 20;
        break;
    case SCALEDW::D2N2:
        margin = 40;
        break;
    case SCALEDW::D3N1:
        margin = 15;
        break;
    case SCALEDW::D3N2:
        margin = 30;
        break;
    case SCALEDW::D3N3:
        margin = 45;
        break;
    default:
        return;
    }


    if ( (maxX - minX) > EPS) {
        qreal y = -mPoint.y()*scaleFactor + oy + margin;
        QPointF px1(minX * scaleFactor + ox, y);
        QPointF px2(maxX * scaleFactor + ox, y);
        pt->setPen(Qt::darkGray);
        pt->drawLine(px1 + QPointF(0., -5.), px1 + QPointF(0., +5.));
        pt->drawLine(px2 + QPointF(0., -5.), px2 + QPointF(0., +5.));
        pt->drawLine(px1, px2);
        pt->setPen(Qt::black);
        pt->translate(px1 + QPointF(0., -16.));
        pt->drawText(QRect(0, 0, (maxX - minX)*scaleFactor, 16), Qt::AlignCenter, QString("%1").arg(int(maxX - minX)));
        pt->translate(-px1 + QPointF(0., +16.));
    }

    if ( (maxY - minY) > EPS) {
        qreal x = mPoint.x()*scaleFactor + ox + margin;
        QPointF py1(x, -minY * scaleFactor + oy);
        QPointF py2(x, -maxY * scaleFactor + oy);
        pt->setPen(Qt::darkGray);
        pt->drawLine(py1 + QPointF(-5., 0.), py1 + QPointF(+5., 0.));
        pt->drawLine(py2 + QPointF(-5., 0.), py2 + QPointF(+5., 0.));
        pt->drawLine(py1, py2);
        pt->setPen(Qt::black);
        pt->translate(py1 + QPointF(-16., 0.));
        pt->rotate(-90.0);
        pt->drawText(QRect(0, 0, (maxY - minY)*scaleFactor, 16), Qt::AlignCenter, QString("%1").arg(int(maxY - minY)));
        pt->rotate(+90.0);
        pt->translate(-py1 + QPointF(16., 0.));
    }

}

void SectionLineDraw::drawArrow(const QPointF &p1, const QPointF &p2, int size, QPainter *pt)
{
    if ((QVector2D(p1) - QVector2D(p2)).length() < size) return;

    QVector2D unit = (QVector2D(p1) - QVector2D(p2)).normalized();
    qreal angle = 0.5;
    QPointF pc = (p1 + p2) / 2;
    QPointF pe1 = pc + size * QPointF(qCos(angle) * unit.x() - qSin(angle) * unit.y(),
                                      qSin(angle) * unit.x() + qCos(angle) * unit.y());
    QPointF pe2 = pc + size * QPointF(qCos(-angle) * unit.x() - qSin(-angle) * unit.y(),
                                      qSin(-angle) * unit.x() + qCos(-angle) * unit.y());

    pt->drawLine(pc, pe1);
    pt->drawLine(pc, pe2);
}

void SectionLineDraw::drawCrossPoint(const QPointF& origin, const QPointF& point, qreal maxx, qreal maxy, qreal scaleFactor, QPainter *pt){
    QString unitx, unity;
    double ccx, ccy;
    if     (maxx < 0.1) {
        unitx = "mm";
        ccx = 1000;
    } else if (maxx < 1.) {
        unitx = "cm";
        ccx = 1000;
    } else             {
        unitx = "m";
        ccx = 1;
    }
    if     (maxy <= 1) {
        unity = "N";
        ccy = 1000;
    } else             {
        unity = "kN";
        ccy = 1;
    }


    QPointF p1 = QPointF(point.x(), origin.y());
    QPointF p2 = QPointF(origin.x(), point.y());
    QPointF p3 = QPointF(point.x(), point.y());

    if (abs(p1.y() - p3.y()) < EPS || abs(p2.x() - p3.x()) < EPS)return;

    pt->setPen(Qt::DotLine);
    pt->drawLine(p1, p3);
    pt->drawLine(p2, p3);
    QString labelx = QString::number((point.x() - origin.x())*maxx * ccx / scaleFactor, 'f', 2) + unitx;
    pt->drawText(QRect(point.x() - 30, origin.y(), 60, 25), Qt::AlignCenter, labelx);
    QString labely = QString::number(-(point.y() - origin.y())*maxy * ccy / scaleFactor, 'f', 0) + unity;
    pt->drawText(QRect(origin.x() - 60, point.y() - 12.5, 60, 25), Qt::AlignCenter, labely);
}


void SectionLineDraw::drawRCFrameOneSection(QPainter *pt,  const LinePartsParameterSRC& param, qreal ox, qreal oy, bool isSRC){
    int ind = barToDouble(param.mainBar1);
    int iand = barToDouble(param.axialBar);
    if (param.concType == ConcreteType::Type::CIRCLE) {
        drawConcCircle(pt, ox, oy, scaleFactor, param);
        if (isSRC) {
            drawScale(-param.Dy / 2., param.Dy / 2., -param.Dz / 2., param.Dz / 2., ox, oy, SCALEDW::D3N3, pt, scaleFactor);
        } else {
            drawScale(-param.Dy / 2., param.Dy / 2., -param.Dz / 2., param.Dz / 2., ox, oy, SCALEDW::D1N1, pt, scaleFactor);
        }
        QList<QPointF> iplist = multiplyScaleFactor(param.barPointList(mySectionType.getMemberType(), false), scaleFactor, ox, oy);
        Q_FOREACH (QPointF pp, iplist){
            drawReinShape(pt, qreal(ind), pp, scaleFactor);
        }
        QList<QPointF> iaplist = multiplyScaleFactor(param.barPointList(mySectionType.getMemberType(), true), scaleFactor, ox, oy);
        Q_FOREACH (QPointF pp, iaplist) {
            drawReinShape(pt, qreal(iand), pp, scaleFactor);
        }

        if (param.hoopY > 1) {
            pt->setPen(QPen(Qt::darkGray, 1, Qt::SolidLine));
            QPointF p2 = QPointF(( param.Dy / 2. - param.RK + qreal(ind) / 2.) * scaleFactor  + ox,
                                 -(-param.Dy / 2.0 + param.RK - qreal(ind) / 2.) * scaleFactor  + oy);
            QPointF p4 = QPointF((-param.Dy / 2. + param.RK - qreal(ind) / 2.) * scaleFactor - 1 + ox,
                                 -( param.Dy / 2.0 - param.RK + qreal(ind) / 2.) * scaleFactor - 1 + oy);
            pt->setRenderHint(QPainter::Antialiasing, true);
            pt->setBrush(Qt::NoBrush);
            pt->drawEllipse(QRect(p4.toPoint(), p2.toPoint()));
        }
    } else if (param.concType == ConcreteType::Type::RECTANGLE) {
        drawConcRect(pt, ox, oy, scaleFactor, param);
        if (isSRC) {
            drawScale(-param.Dy / 2., param.Dy / 2., -param.Dz / 2., param.Dz / 2., ox, oy, SCALEDW::D3N3, pt, scaleFactor);
        } else {
            drawScale(-param.Dy / 2., param.Dy / 2., -param.Dz / 2., param.Dz / 2., ox, oy, SCALEDW::D1N1, pt, scaleFactor);
        }
        QList<QPointF> iplist = multiplyScaleFactor(param.barPointList(mySectionType.getMemberType(), false), scaleFactor, ox, oy);
        Q_FOREACH (QPointF pp, iplist){
            drawReinShape(pt, qreal(ind), pp, scaleFactor);
        }
        QList<QPointF> iaplist = multiplyScaleFactor(param.barPointList(mySectionType.getMemberType(), true), scaleFactor, ox, oy);
        Q_FOREACH (QPointF pp, iaplist){
            drawReinShape(pt, qreal(iand), pp, scaleFactor);
        }
        //hoop
        pt->setPen(QPen(Qt::darkGray, 1, Qt::SolidLine));
        pt->setRenderHint(QPainter::Antialiasing, false);
        QPointF p1 = QPointF(( param.Dy / 2. - param.RK + qreal(ind) / 2.) * scaleFactor + ox,
                             -( param.Dz / 2.0 - param.RKu + qreal(ind) / 2.) * scaleFactor - 1 + oy); //右上
        QPointF p2 = QPointF(( param.Dy / 2. - param.RK + qreal(ind) / 2.) * scaleFactor + ox,
                             -(-param.Dz / 2.0 + param.RKb - qreal(ind) / 2.) * scaleFactor  + oy); //右下
        QPointF p3 = QPointF((-param.Dy / 2. + param.RK - qreal(ind) / 2.) * scaleFactor + ox - 1,
                             -(-param.Dz / 2.0 + param.RKb - qreal(ind) / 2.) * scaleFactor  + oy); //左下
        QPointF p4 = QPointF((-param.Dy / 2. + param.RK - qreal(ind) / 2.) * scaleFactor + ox - 1,
                             -( param.Dz / 2.0 - param.RKu + qreal(ind) / 2.) * scaleFactor - 1 + oy); //左上
        if (param.hoopZ > 0) {
            QPointF dp1 = p4 - p3;
            QPointF dp2 = p1 - p2;
            if (param.hoopY > 1) {
                dp1 = dp1 / qreal(param.hoopY - 1);
                dp2 = dp2 / qreal(param.hoopY - 1);
            }
            for (int i = 0 ; i < param.hoopY; i++) {
                if (i == 0) {
                    pt->drawLine(p3, p2);
                    continue;
                }
                if (i == 1) {
                    pt->drawLine(p4, p1);
                    continue;
                }
                pt->drawLine(p3 + dp1 * qreal(i - 1), p2 + dp2 * qreal(i - 1));
            }
        }
        if (param.hoopZ > 0) {
            QPointF dp1 = p2 - p3;
            QPointF dp2 = p1 - p4;
            if (param.hoopZ > 1) {
                dp1 = dp1 / qreal(param.hoopZ - 1);
                dp2 = dp2 / qreal(param.hoopZ - 1);
            }
            for (int i = 0 ; i < param.hoopZ; i++) {
                if (i == 0) {
                    pt->drawLine(p3, p4);
                    continue;
                }
                if (i == 1) {
                    pt->drawLine(p2, p1);
                    continue;
                }
                pt->drawLine(p3 + dp1 * qreal(i - 1), p4 + dp2 * qreal(i - 1));
            }
        }
    }
}


void SectionLineDraw::drawRCFrameSection(QPainter *pt)
{
    scaleFactor = calcScaleFactor(0, 0, 0, 0, 0);
    bool isSRC = ( mySectionType.getStructType() == STRUCTTYPE::STP_SRC );
    if (isSRC) {
        scaleFactor = calcScaleFactor(mySection.i_param.Dy, mySection.i_param.Dz, 0., 0., scaleFactor);
    } else {
        scaleFactor = calcScaleFactor(mySection.i_param.Dy, mySection.i_param.Dz, 0., 0., scaleFactor);
    }

    scaleFactor = calcScaleFactor(mySection.c_param.Dy, mySection.c_param.Dz, 0., 0., scaleFactor);
    scaleFactor = calcScaleFactor(mySection.j_param.Dy, mySection.j_param.Dz, 0., 0., scaleFactor);

    if (isSRC) { //SRCのとき、SのScaleFactor考慮
        scaleFactor = calcScaleFactor(mySection.i_param.sizeB, mySection.i_param.sizeH, mySection.i_param.cY, mySection.i_param.cZ, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.c_param.sizeB, mySection.c_param.sizeH, mySection.c_param.cY, mySection.c_param.cZ, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.j_param.sizeB, mySection.j_param.sizeH, mySection.j_param.cY, mySection.j_param.cZ, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.i_param.H_T, mySection.i_param.B_T, mySection.i_param.cY_T, mySection.i_param.cZ_T, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.c_param.H_T, mySection.c_param.B_T, mySection.c_param.cY_T, mySection.c_param.cZ_T, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.j_param.H_T, mySection.j_param.B_T, mySection.j_param.cY_T, mySection.j_param.cZ_T, scaleFactor);
    }
    if ( scaleFactor > 999. || scaleFactor < EPS ) return;
    qreal oy = 10 + height() / 2;
    if (drawflag == drawFlag::IJ) {
        drawRCFrameOneSection(pt, mySection.i_param, width() * 0.25, oy, isSRC);
        drawRCFrameOneSection(pt, mySection.j_param, width() * 0.75, oy, isSRC);
    } else if (drawflag == drawFlag::IM) {
        drawRCFrameOneSection(pt, mySection.i_param, width() * 0.25, oy, isSRC);
        drawRCFrameOneSection(pt, mySection.c_param, width() * 0.75, oy, isSRC);
    } else if (drawflag == drawFlag::I) {
        drawRCFrameOneSection(pt, mySection.i_param, width() * 0.5, oy, isSRC);
    } else if (drawflag == drawFlag::IMJ) {
        drawRCFrameOneSection(pt, mySection.i_param, width() * 0.1667, oy, isSRC);
        drawRCFrameOneSection(pt, mySection.c_param, width() * 0.5, oy, isSRC);
        drawRCFrameOneSection(pt, mySection.j_param, width() * 0.8333, oy, isSRC);
    }
}

void SectionLineDraw::drawSFrameOneSection(QPainter *pt,  const LinePartsParameterSRC& param, qreal ox, qreal oy, bool isSRC){

    if (isSRC) {
        if (SteelType::isSideways(param.steelType)) {
            drawScale(-param.sizeH / 2. + param.cY, param.sizeH / 2. + param.cY, -param.getSizeB() / 2. + param.cZ, param.getSizeB() / 2. + param.cZ, ox, oy, SCALEDW::D3N1, pt, scaleFactor);
        } else             {
            drawScale(-param.getSizeB() / 2. + param.cY, param.getSizeB() / 2. + param.cY, -param.sizeH / 2. + param.cZ, param.sizeH / 2. + param.cZ, ox, oy, SCALEDW::D3N1, pt, scaleFactor);
        }
    } else {
        if (SteelType::isSideways(param.steelType)) {
            drawScale(-param.sizeH / 2., param.sizeH / 2., -param.getSizeB() / 2., param.getSizeB() / 2., ox, oy, SCALEDW::D3N1, pt, scaleFactor);
        } else             {
            drawScale(-param.getSizeB() / 2., param.getSizeB() / 2., -param.sizeH / 2., param.sizeH / 2., ox, oy, SCALEDW::D3N1, pt, scaleFactor);
        }
    }

    if ( SteelType::isStandardType(param.steelType) ) {
        if ( param.steelType == SteelType::Type::ST_H ) {
            drawSteelH(pt, ox, oy, scaleFactor, param, false);
        } else if ( param.steelType == SteelType::Type::ST_BX ) {
            drawSteelRollBox(pt, ox, oy, scaleFactor, param, false);
        } else if ( param.steelType == SteelType::Type::ST_P ) {
            drawSteelP(pt, ox, oy, scaleFactor, param);
        }
    }

    if (param.steelType == SteelType::Type::ST_WH) {
        drawSteelH(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WI) {
        drawSteelH(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WBX) {
        drawSteelBuildBox(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WP) {
        drawSteelP(pt, ox, oy, scaleFactor, param);
    } else if (param.steelType == SteelType::Type::ST_WC1) {
        drawSteelC1(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WC2) {
        drawSteelC2(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WC3) {
        drawSteelC3(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WC4) {
        drawSteelC4(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WT1) {
        drawSteelT1(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WT2) {
        drawSteelT2(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WT3) {
        drawSteelT3(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WT4) {
        drawSteelT4(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WPL) {
        drawSteelPlate(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WBR) {
        drawSteelRound(pt, ox, oy, scaleFactor, param);
    }
}

void SectionLineDraw::drawSFrameSection(QPainter *pt)
{
    bool isSRC = ( mySectionType.getStructType() == STRUCTTYPE::STP_SRC );
    if (!isSRC){
        scaleFactor = calcScaleFactor(0, 0, 0, 0, 0);
    }
    qreal iyA = mySection.i_param.sizeH;
    qreal iyB = mySection.i_param.getSizeB();
    qreal iyRy = isSRC ? mySection.i_param.cY : 0.0;
    qreal iyRz = isSRC ? mySection.i_param.cZ : 0.0;
    if (SteelType::isSideways(mySection.i_param.steelType)){
        scaleFactor = calcScaleFactor(iyA, iyB, iyRy, iyRz, scaleFactor);
    }else{
        scaleFactor = calcScaleFactor(iyB, iyA, iyRy, iyRz, scaleFactor);
    }

    qreal myA = mySection.c_param.sizeH;
    qreal myB = mySection.c_param.getSizeB();
    qreal myRy = isSRC ? mySection.c_param.cY : 0.0;
    qreal myRz = isSRC ? mySection.c_param.cZ : 0.0;
    if (SteelType::isSideways(mySection.i_param.steelType)){
        scaleFactor = calcScaleFactor(myA, myB, myRy, myRz, scaleFactor);
    }else{
        scaleFactor = calcScaleFactor(myB, myA, myRy, myRz, scaleFactor);
    }

    qreal jyA = mySection.j_param.sizeH;
    qreal jyB = mySection.j_param.getSizeB();
    qreal jyRy = isSRC ? mySection.j_param.cY : 0.0;
    qreal jyRz = isSRC ? mySection.j_param.cZ : 0.0;
    if (SteelType::isSideways(mySection.i_param.steelType)){
        scaleFactor = calcScaleFactor(jyA, jyB, jyRy, jyRz, scaleFactor);
    }else{
        scaleFactor = calcScaleFactor(jyB, jyA, jyRy, jyRz, scaleFactor);
    }

    if (isSRC ) { //SRCのとき、RCのScaleFactorを設定
        scaleFactor = calcScaleFactor(mySection.i_param.Dy, mySection.i_param.Dz, 0., 0., scaleFactor);
        scaleFactor = calcScaleFactor(mySection.c_param.Dy, mySection.c_param.Dz, 0., 0., scaleFactor);
        scaleFactor = calcScaleFactor(mySection.j_param.Dy, mySection.j_param.Dz, 0., 0., scaleFactor);
    }

    if ( scaleFactor > 999. || scaleFactor < EPS ) return;

    qreal oy= 10 + height() / 2;
    if (drawflag == drawFlag::IJ) {
        drawSFrameOneSection(pt, mySection.i_param, width() * 0.25, oy, isSRC);
        drawSFrameOneSection(pt, mySection.j_param, width() * 0.75, oy, isSRC);
    } else if (drawflag == drawFlag::IM) {
        drawSFrameOneSection(pt, mySection.i_param, width() * 0.25, oy, isSRC);
        drawSFrameOneSection(pt, mySection.c_param, width() * 0.75, oy, isSRC);
    } else if (drawflag == drawFlag::I) {
        drawSFrameOneSection(pt, mySection.i_param, width() * 0.5, oy, isSRC);
    } else if (drawflag == drawFlag::IMJ) {
        drawSFrameOneSection(pt, mySection.i_param, width() * 0.1667, oy, isSRC);
        drawSFrameOneSection(pt, mySection.c_param, width() * 0.5, oy, isSRC);
        drawSFrameOneSection(pt, mySection.j_param, width() * 0.8333, oy, isSRC);
    }
}

void SectionLineDraw::drawSRCFrameSection(QPainter *pt)
{
    drawRCFrameSection(pt);
    drawSFrameSection(pt);
}

void SectionLineDraw::drawConcRect(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param)
{
    if ( qAbs(param.Dy) < EPS || qAbs(param.Dz) < EPS  ) return;

    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(200, 200, 200, 100)));
    qreal xx = ox - param.Dy / 2.*scaleFactor;
    qreal yy = oy - param.Dz / 2.*scaleFactor;
    qreal ww = param.Dy * scaleFactor;
    qreal hh = param.Dz * scaleFactor;
    pt->drawRect(QRect(xx, yy, ww, hh));
}

void SectionLineDraw::drawConcCircle(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param)
{
    if ( qAbs(param.Dy) < EPS ) return;

    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(200, 200, 200, 100)));
    qreal xx = ox - param.Dy / 2.*scaleFactor;
    qreal yy = oy - param.Dy / 2.*scaleFactor;
    qreal ww = param.Dy * scaleFactor;
    qreal hh = param.Dy * scaleFactor;
    pt->drawEllipse(QRect(xx, yy, ww, hh));
}

void SectionLineDraw::drawReinShape(QPainter *pt, qreal d, const QPointF& point, qreal scaleFactor)
{
    if ( qAbs(d) < EPS ) return;
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(150, 150, 150, 150)));
    pt->drawEllipse(point, qreal(0.5 * d * scaleFactor - 0.5),
                    qreal(0.5 * d * scaleFactor - 0.5)); //ペン幅分マイナス
}

void SectionLineDraw::drawSteelH_T(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param)
{
    if ( qAbs(param.H_T) < EPS || qAbs(param.B_T) < EPS || qAbs(param.t1_T) < EPS || qAbs(param.t2_T) < EPS  )
        return;
    QString var = SectionDataType::standardSteelSectionValue(param.standardName, StandardSectionTable::SteelFormat::ST_R);
    qreal rr = ( var.isEmpty() ) ? 0.0 : var.toDouble() ;
    drawSteelI(param.H_T, param.B_T, param.t1_T, param.t2_T, param.cY_T, param.cZ_T, rr, ox, oy, pt, scaleFactor);
}

void SectionLineDraw::drawSteelH(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if ( qAbs(param.sizeH) < EPS || qAbs(param.sizeB) < EPS || qAbs(param.t1) < EPS || qAbs(param.t2) < EPS  )
        return;
    QString var = SectionDataType::standardSteelSectionValue(param.standardName, StandardSectionTable::SteelFormat::ST_R);
    qreal rr = ( var.isEmpty() ) ? 0.0 : var.toDouble() ;
    if (isYDirection)
        drawSteelH(param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ, rr, ox, oy, pt, scaleFactor);
    else
        drawSteelI(param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ, rr, ox, oy, pt, scaleFactor);
}

void SectionLineDraw::drawSteelI(qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz,
                                 qreal rr, qreal ox, qreal oy, QPainter *pt, qreal scaleFactor)
{
    QPainterPath myPath;
    myPath.moveTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    if (rr > EPS) {
        myPath.lineTo(ox + (ry - t1 / 2. - rr)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
        myPath.arcTo(ox + (ry - t1 / 2. - 2.*rr)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor,
                     2.*rr * scaleFactor, 2.*rr * scaleFactor, 90., -90.);
        myPath.lineTo(ox + (ry - t1 / 2.)*scaleFactor, oy + (-rz + A / 2. - t2 - rr)*scaleFactor);
        myPath.arcTo(ox + (ry - t1 / 2. - 2.*rr)*scaleFactor, oy + (-rz + A / 2. - t2 - 2.*rr)*scaleFactor,
                     2.*rr * scaleFactor, 2.*rr * scaleFactor, 0., -90.);
    } else {
        myPath.lineTo(ox + (ry - t1 / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
        myPath.lineTo(ox + (ry - t1 / 2.)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    }
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz + A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz + A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    if (rr > EPS) {
        myPath.lineTo(ox + (ry + t1 / 2. + rr)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
        myPath.arcTo(ox + (ry + t1 / 2.)*scaleFactor, oy + (-rz + A / 2. - t2 - 2.*rr)*scaleFactor,
                     2.*rr * scaleFactor, 2.*rr * scaleFactor, 270., -90.);
        myPath.lineTo(ox + (ry + t1 / 2.)*scaleFactor, oy + (-rz - A / 2. + t2 + rr)*scaleFactor);
        myPath.arcTo(ox + (ry + t1 / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor,
                     2.*rr * scaleFactor, 2.*rr * scaleFactor, 180., -90.);
    } else {
        myPath.lineTo(ox + (ry + t1 / 2.)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
        myPath.lineTo(ox + (ry + t1 / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    }
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.closeSubpath();

    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
}

void SectionLineDraw::drawSteelH(qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz,
                                 qreal rr, qreal ox, qreal oy, QPainter *pt, qreal scaleFactor)
{
    QPainterPath myPath;
    myPath.moveTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    if (rr > EPS) {
        myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz + t1 / 2. + rr)*scaleFactor);
        myPath.arcTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz + t1 / 2.)*scaleFactor,
                     2.*rr * scaleFactor, 2.*rr * scaleFactor, 180., -90.);
        myPath.lineTo(ox + (ry + A / 2. - t2 - rr)*scaleFactor, oy + (-rz + t1 / 2.)*scaleFactor);
        myPath.arcTo(ox + (ry + A / 2. - t2 - 2.*rr)*scaleFactor, oy + (-rz + t1 / 2.)*scaleFactor,
                     2.*rr * scaleFactor, 2.*rr * scaleFactor, 90., -90.);
    } else {
        myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz + t1 / 2.)*scaleFactor);
        myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz + t1 / 2.)*scaleFactor);
    }
    myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2.)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    if (rr > EPS) {
        myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz - t1 / 2. - rr)*scaleFactor);
        myPath.arcTo(ox + (ry + A / 2. - t2 - 2.*rr)*scaleFactor, oy + (-rz - t1 / 2. - 2.*rr)*scaleFactor,
                     2.*rr * scaleFactor, 2.*rr * scaleFactor, 0., -90.);
        myPath.lineTo(ox + (ry - A / 2. + t2 + rr)*scaleFactor, oy + (-rz - t1 / 2.)*scaleFactor);
        myPath.arcTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz - t1 / 2. - 2.*rr)*scaleFactor,
                     2.*rr * scaleFactor, 2.*rr * scaleFactor, 270., -90.);
    } else {
        myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz - t1 / 2.)*scaleFactor);
        myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz - t1 / 2.)*scaleFactor);
    }
    myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.closeSubpath();

    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
    pt->setBrush(Qt::NoBrush);
    pt->setRenderHint(QPainter::Antialiasing, false);
}

void SectionLineDraw::drawSteelBuildBox(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection) const{
        if ( qAbs(param.sizeH) < EPS || qAbs(param.sizeB) < EPS || qAbs(param.t1) < EPS || qAbs(param.t2) < EPS )
            return;

        qreal Dy, Dz, t1, t2;
        if (isYDirection) {
            Dy = param.sizeH;
            Dz = param.sizeB;
            t1 = param.t2;
            t2 = param.t1;
        } else {
            Dy = param.sizeB;
            Dz = param.sizeH;
            t1 = param.t1;
            t2 = param.t2;
        }

        QPainterPath myPath;

        myPath.moveTo(ox + (param.cY - Dy / 2.)*scaleFactor, oy + (-param.cZ - Dz / 2.)*scaleFactor);
        myPath.lineTo(ox + (param.cY - Dy / 2.)*scaleFactor, oy + (-param.cZ + Dz / 2.)*scaleFactor);
        myPath.lineTo(ox + (param.cY + Dy / 2.)*scaleFactor, oy + (-param.cZ + Dz / 2.)*scaleFactor);
        myPath.lineTo(ox + (param.cY + Dy / 2.)*scaleFactor, oy + (-param.cZ - Dz / 2.)*scaleFactor);
        myPath.lineTo(ox + (param.cY - Dy / 2.)*scaleFactor, oy + (-param.cZ - Dz / 2.)*scaleFactor);
        myPath.closeSubpath();

        qreal Dy2 = Dy - 2.0 * t1;
        qreal Dz2 = Dz - 2.0 * t2;

        myPath.moveTo(ox + (param.cY - Dy2 / 2.)*scaleFactor, oy + (-param.cZ - Dz2 / 2.)*scaleFactor);
        myPath.lineTo(ox + (param.cY - Dy2 / 2.)*scaleFactor, oy + (-param.cZ + Dz2 / 2.)*scaleFactor);
        myPath.lineTo(ox + (param.cY + Dy2 / 2.)*scaleFactor, oy + (-param.cZ + Dz2 / 2.)*scaleFactor);
        myPath.lineTo(ox + (param.cY + Dy2 / 2.)*scaleFactor, oy + (-param.cZ - Dz2 / 2.)*scaleFactor);
        myPath.lineTo(ox + (param.cY - Dy2 / 2.)*scaleFactor, oy + (-param.cZ - Dz2 / 2.)*scaleFactor);
        myPath.closeSubpath();
        myPath.setFillRule(Qt::OddEvenFill);

        pt->setRenderHint(QPainter::Antialiasing, true);
        pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
        pt->setBrush(QBrush(QColor(50, 50, 50, 150), Qt::SolidPattern));
        pt->drawPath(myPath);

        QRectF rect = QRect(QPoint(ox + (param.cY - Dy2 / 2.)*scaleFactor, oy + (-param.cZ - Dz2 / 2.)*scaleFactor)
                            , QPoint(ox + (param.cY + Dy2 / 2.)*scaleFactor, oy + (-param.cZ + Dz2 / 2.)*scaleFactor));

        if (mySectionType.getStructType() == STRUCTTYPE::STP_CFT) {
            pt->setPen(Qt::NoPen);
            pt->setBrush(QBrush(Qt::lightGray, Qt::BDiagPattern));
            pt->drawRect(rect);
        }
        if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) {
            pt->setPen(Qt::NoPen);
            pt->setBrush(QBrush(Qt::white));
            pt->drawRect(rect);
            if (!mySection.line_param.filledConcMaterial.isEmpty()){
                pt->setPen(Qt::NoPen);
                pt->setBrush(QBrush(Qt::lightGray, Qt::BDiagPattern));
                pt->drawRect(rect);
            }
        }
}

void SectionLineDraw::drawSteelRollBox(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection) const
{
    if ( qAbs(param.sizeH) < EPS || qAbs(param.sizeB) < EPS || qAbs(param.t1) < EPS )
        return;

    qreal Dy, Dz;
    if (isYDirection) {
        Dy = param.sizeH;
        Dz = param.sizeB;
    } else {
        Dy = param.sizeB;
        Dz = param.sizeH;
    }

    QString var = SectionDataType::standardSteelSectionValue(param.standardName, StandardSectionTable::SteelFormat::ST_R);
    qreal rr = ( var.isEmpty() ) ? 0.0 : var.toDouble() ;
    if (rr < EPS)
        rr = 0.0;
    qreal cos125 = qCos(12.5 / 180.); //iba:意味のある値？
    qreal sin125 = qSin(12.5 / 180.); //iba:意味のある値？

    QPainterPath myPath1;
    myPath1.moveTo(ox + (param.cY - Dy / 2.)*scaleFactor,
                   oy + (-param.cZ - Dz / 2. + rr * cos125 - rr * sin125)*scaleFactor);
    myPath1.lineTo(ox + (param.cY - Dy / 2.)*scaleFactor,
                   oy + (-param.cZ + Dz / 2. - rr * cos125 + rr * sin125)*scaleFactor);

    if (rr > EPS) {
        myPath1.arcTo(ox + (param.cY - Dy / 2. - rr + rr * cos125)*scaleFactor,
                      oy + (-param.cZ + Dz / 2. - rr - rr * cos125)*scaleFactor,
                      2.*rr * scaleFactor, 2.*rr * scaleFactor, 192.5, 65.);
    }

    myPath1.lineTo(ox + (param.cY + Dy / 2. - rr * cos125 + rr * sin125)*scaleFactor,
                   oy + (-param.cZ + Dz / 2.)*scaleFactor);

    if (rr > EPS) {
        myPath1.arcTo(ox + (param.cY + Dy / 2. - rr - rr * cos125)*scaleFactor,
                      oy + (-param.cZ + Dz / 2. - rr - rr * cos125)*scaleFactor,
                      2.*rr * scaleFactor, 2.*rr * scaleFactor, 282.5, 65.);
    }

    myPath1.lineTo(ox + (param.cY + Dy / 2.)*scaleFactor,
                   oy + (-param.cZ - Dz / 2. + rr * cos125 - rr * sin125)*scaleFactor);

    if (rr > EPS) {
        myPath1.arcTo(ox + (param.cY + Dy / 2. - rr - rr * cos125)*scaleFactor,
                      oy + (-param.cZ - Dz / 2. - rr + rr * cos125)*scaleFactor,
                      2.*rr * scaleFactor, 2.*rr * scaleFactor, 12.5, 65.);
    }

    myPath1.lineTo(ox + (param.cY - Dy / 2. + rr * cos125 - rr * sin125)*scaleFactor,
                   oy + (-param.cZ - Dz / 2.)*scaleFactor);

    if (rr > EPS) {
        myPath1.arcTo(ox + (param.cY - Dy / 2. - rr + rr * cos125)*scaleFactor,
                      oy + (-param.cZ - Dz / 2. - rr + rr * cos125)*scaleFactor,
                      2.*rr * scaleFactor, 2.*rr * scaleFactor, 102.5, 65.);
    }

    myPath1.closeSubpath();

    Dy = Dy - 2.0 * param.t1;
    Dz = Dz - 2.0 *  param.t1;
    qreal rr2 = qMax(rr -  param.t1, 0.0);

    QPainterPath myPath2;
    myPath2.moveTo(ox + (param.cY - Dy / 2.)*scaleFactor,
                   oy + (-param.cZ - Dz / 2. + rr2 * cos125 - rr2 * sin125)*scaleFactor);
    myPath2.lineTo(ox + (param.cY - Dy / 2.)*scaleFactor,
                   oy + (-param.cZ + Dz / 2. - rr2 * cos125 + rr2 * sin125)*scaleFactor);

    if (rr > EPS) {
        myPath2.arcTo(ox + (param.cY - Dy / 2. - rr2 + rr2 * cos125)*scaleFactor,
                      oy + (-param.cZ + Dz / 2. - rr2 - rr2 * cos125)*scaleFactor,
                      2.*rr2 * scaleFactor, 2.*rr2 * scaleFactor, 192.5, 65.);
    }

    myPath2.lineTo(ox + (param.cY + Dy / 2. - rr2 * cos125 + rr2 * sin125)*scaleFactor,
                   oy + (-param.cZ + Dz / 2.)*scaleFactor);

    if (rr > EPS) {
        myPath2.arcTo(ox + (param.cY + Dy / 2. - rr2 - rr2 * cos125)*scaleFactor,
                      oy + (-param.cZ + Dz / 2. - rr2 - rr2 * cos125)*scaleFactor,
                      2.*rr2 * scaleFactor, 2.*rr2 * scaleFactor, 282.5, 65.);
    }

    myPath2.lineTo(ox + (param.cY + Dy / 2.)*scaleFactor,
                   oy + (-param.cZ - Dz / 2. + rr2 * cos125 - rr2 * sin125)*scaleFactor);

    if (rr > EPS) {
        myPath2.arcTo(ox + (param.cY + Dy / 2. - rr2 - rr2 * cos125)*scaleFactor,
                      oy + (-param.cZ - Dz / 2. - rr2 + rr2 * cos125)*scaleFactor,
                      2.*rr2 * scaleFactor, 2.*rr2 * scaleFactor, 12.5, 65.);
    }

    myPath2.lineTo(ox + (param.cY - Dy / 2. + rr2 * cos125 - rr2 * sin125)*scaleFactor,
                   oy + (-param.cZ - Dz / 2.)*scaleFactor);

    if (rr > EPS) {
        myPath2.arcTo(ox + (param.cY - Dy / 2. - rr2 + rr2 * cos125)*scaleFactor,
                      oy + (-param.cZ - Dz / 2. - rr2 + rr2 * cos125)*scaleFactor,
                      2.*rr2 * scaleFactor, 2.*rr2 * scaleFactor, 102.5, 65.);
    }

    myPath2.closeSubpath();
    myPath1.connectPath(myPath2);
    myPath1.setFillRule(Qt::OddEvenFill);
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath1);

    if (mySectionType.getStructType() == STRUCTTYPE::STP_CFT) {
        pt->setPen(Qt::NoPen);
        pt->setBrush(QBrush(Qt::lightGray, Qt::BDiagPattern));
        pt->drawPath(myPath2);
    }
    if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC ) {
        pt->setPen(Qt::NoPen);
        pt->setBrush(QBrush(QColor(255, 255, 255, 150)));
        pt->drawPath(myPath2);
        if (!mySection.line_param.filledConcMaterial.isEmpty()){
            pt->setPen(Qt::NoPen);
            pt->setBrush(QBrush(Qt::lightGray, Qt::BDiagPattern));
            pt->drawPath(myPath2);
        }
    }
}

void SectionLineDraw::drawSteelP(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param) const
{
    if ( qAbs(param.sizeH) < EPS || qAbs(param.t1) < EPS  )
        return;

    QRectF rect1(ox + (param.cY - param.sizeH / 2.)*scaleFactor, oy + (-param.cZ - param.sizeH / 2.)*scaleFactor,
                 param.sizeH * scaleFactor, param.sizeH * scaleFactor);
    QRectF rect2(ox + (param.cY - param.sizeH / 2. + param.t1)*scaleFactor, oy + (-param.cZ - param.sizeH / 2. + param.t1)*scaleFactor,
                 (param.sizeH - 2.0 * param.t1)*scaleFactor, (param.sizeH - 2.0 * param.t1)*scaleFactor);

    QPainterPath myPath;
    myPath.addEllipse(rect1);
    myPath.addEllipse(rect2);
    myPath.setFillRule(Qt::OddEvenFill);

    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);

    if (mySectionType.getStructType() == STRUCTTYPE::STP_CFT) {
        pt->setPen(Qt::NoPen);
        pt->setBrush(QBrush(Qt::lightGray, Qt::BDiagPattern));
        pt->drawEllipse(rect2);
    }
    if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) {
        pt->setPen(Qt::NoPen);
        pt->setBrush(QBrush(Qt::white));
        pt->drawEllipse(rect2);

        if (!mySection.line_param.filledConcMaterial.isEmpty()){
            pt->setPen(Qt::NoPen);
            pt->setBrush(QBrush(Qt::lightGray, Qt::BDiagPattern));
            pt->drawEllipse(rect2);
        }
    }
}

void SectionLineDraw::drawSteelC1(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if (isYDirection){
        drawSteelCBottom(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    }else{
        drawSteelCRight(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    }
}
void SectionLineDraw::drawSteelC2(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if (isYDirection){
        drawSteelCTop(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    } else{
        drawSteelCLeft(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    }
}
void SectionLineDraw::drawSteelC3(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if (isYDirection){
        drawSteelCLeft(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    } else{
        drawSteelCBottom(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    }
}
void SectionLineDraw::drawSteelC4(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if (isYDirection){
        drawSteelCRight(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    } else{
        drawSteelCTop(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    }
}
void SectionLineDraw::drawSteelC1_T(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param)
{
    drawSteelCRight(pt, ox, oy, scaleFactor, param.H_T, param.B_T, param.t1_T, param.t2_T, param.cY_T, param.cZ_T);
}
void SectionLineDraw::drawSteelC2_T(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param)
{
    drawSteelCLeft(pt, ox, oy, scaleFactor, param.H_T, param.B_T, param.t1_T, param.t2_T, param.cY_T, param.cZ_T);
}

void SectionLineDraw::drawSteelT1(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if (isYDirection){
        drawSteelTLeft(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    } else{
        drawSteelTBottom(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    }
}
void SectionLineDraw::drawSteelT2(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if (isYDirection){
        drawSteelTRight(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    } else{
        drawSteelTTop(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    }
}
void SectionLineDraw::drawSteelT3(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if (isYDirection){
        drawSteelTTop(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    } else{
        drawSteelTLeft(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    }
}
void SectionLineDraw::drawSteelT4(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if (isYDirection){
        drawSteelTBottom(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    } else{
        drawSteelTRight(pt, ox, oy, scaleFactor, param.sizeH, param.sizeB, param.t1, param.t2, param.cY, param.cZ);
    }
}
void SectionLineDraw::drawSteelT1_T(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param)
{
    drawSteelTTop(pt, ox, oy, scaleFactor, param.H_T, param.B_T, param.t1_T, param.t2_T, param.cY_T, param.cZ_T);
}
void SectionLineDraw::drawSteelT2_T(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param)
{
    drawSteelTBottom(pt, ox, oy, scaleFactor, param.H_T, param.B_T, param.t1_T, param.t2_T, param.cY_T, param.cZ_T);
}

void SectionLineDraw::drawSteelCRight(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz){
    QPainterPath myPath;
    myPath.moveTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz + A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz + A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2. + t1)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2. + t1)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.closeSubpath();
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
}
void SectionLineDraw::drawSteelCTop(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz){
    QPainterPath myPath;
    myPath.moveTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz + B / 2. - t1)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz + B / 2. - t1)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2.)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.closeSubpath();
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
}
void SectionLineDraw::drawSteelCBottom(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz)
{
     QPainterPath myPath;
     myPath.moveTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
     myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
     myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
     myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz - B / 2. + t1)*scaleFactor);
     myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz - B / 2. + t1)*scaleFactor);
     myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
     myPath.lineTo(ox + (ry + A / 2.)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
     myPath.lineTo(ox + (ry + A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
     myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
     myPath.closeSubpath();
     pt->setRenderHint(QPainter::Antialiasing, true);
     pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
     pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
     pt->drawPath(myPath);
}
void SectionLineDraw::drawSteelCLeft(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz)
{
    QPainterPath myPath;
    myPath.moveTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz + A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz + A / 2)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2. - t1)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2. - t1)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.closeSubpath();
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
}

void SectionLineDraw::drawSteelTRight(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz)
{
    QPainterPath myPath;
    myPath.moveTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz - t1 / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2)*scaleFactor, oy + (-rz - t1 / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2.)*scaleFactor, oy + (-rz + t1 / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz + t1 / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2. + t2)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.closeSubpath();
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
}
void SectionLineDraw::drawSteelTTop(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz)
{
    QPainterPath myPath;
    myPath.moveTo(ox + (ry - t1 / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + t1 / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + t1 / 2.)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz + A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz + A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    myPath.lineTo(ox + (ry - t1 / 2.)*scaleFactor, oy + (-rz + A / 2. - t2)*scaleFactor);
    myPath.lineTo(ox + (ry - t1 / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.closeSubpath();
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
}
void SectionLineDraw::drawSteelTBottom(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz)
{
    QPainterPath myPath;
    myPath.moveTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    myPath.lineTo(ox + (ry - t1 / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    myPath.lineTo(ox + (ry - t1 / 2.)*scaleFactor, oy + (-rz + A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + t1 / 2.)*scaleFactor, oy + (-rz + A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + t1 / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz - A / 2. + t2)*scaleFactor);
    myPath.lineTo(ox + (ry + B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - B / 2.)*scaleFactor, oy + (-rz - A / 2.)*scaleFactor);
    myPath.closeSubpath();
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
}
void SectionLineDraw::drawSteelTLeft(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, qreal A, qreal B, qreal t1, qreal t2, qreal ry, qreal rz)
{
    QPainterPath myPath;
    myPath.moveTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - t1 / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz + t1 / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz + t1 / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2.)*scaleFactor, oy + (-rz + B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2.)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz - B / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry + A / 2. - t2)*scaleFactor, oy + (-rz - t1 / 2.)*scaleFactor);
    myPath.lineTo(ox + (ry - A / 2.)*scaleFactor, oy + (-rz - t1 / 2.)*scaleFactor);
    myPath.closeSubpath();
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawPath(myPath);
}

void SectionLineDraw::drawSteelPlate(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param, bool isYDirection)
{
    if ( qAbs(param.sizeH) < EPS || qAbs(param.sizeB) < EPS  ) return;

    qreal Dy, Dz;
    if (isYDirection) {
        Dy = param.sizeH;
        Dz = param.sizeB;
    } else {
        Dy = param.sizeB;
        Dz = param.sizeH;
    }
    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    qreal xx = ox + (param.cY - Dy / 2.) * scaleFactor;
    qreal yy = oy - (param.cZ + Dz / 2.) * scaleFactor;
    qreal ww = Dy * scaleFactor;
    qreal hh = Dz * scaleFactor;
    pt->drawRect(QRect(xx, yy, ww, hh));
}

void SectionLineDraw::drawSteelCross(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param)
{
    if ( qAbs(param.sizeH) < EPS || qAbs(param.sizeB) < EPS  )
        return;

    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    qreal xx = ox + (param.cY - param.sizeH / 2.) * scaleFactor;
    qreal yy = oy - (param.cZ + param.sizeB / 2.) * scaleFactor;
    qreal ww = param.sizeH * scaleFactor;
    qreal hh = param.sizeB * scaleFactor;
    pt->drawRect(QRect(xx, yy, ww, hh));
    xx = ox + (param.cY_T - param.B_T / 2.) * scaleFactor;
    yy = oy - (param.cZ_T + param.H_T / 2.) * scaleFactor;
    ww = param.B_T * scaleFactor;
    hh = param.H_T * scaleFactor;
    pt->drawRect(QRect(xx, yy, ww, hh));
}

void SectionLineDraw::drawSteelRound(QPainter *pt, qreal ox, qreal oy, qreal scaleFactor, const LinePartsParameterSRC& param)
{
    if ( qAbs(param.sizeH) < EPS  ) return;

    pt->setRenderHint(QPainter::Antialiasing, true);
    pt->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    pt->setBrush(QBrush(QColor(50, 50, 50, 150)));
    pt->drawEllipse(QPointF(ox + param.cY * scaleFactor, oy - param.cZ * scaleFactor), param.sizeH / 2.*scaleFactor,
                    param.sizeH / 2.*scaleFactor);
}

QList<QPointF> SectionLineDraw::multiplyScaleFactor(const QList<QPointF>& point, qreal scaleFactor, qreal ox, qreal oy) const{
    QList<QPointF> plist;
    Q_FOREACH(QPointF p, point){
        plist.append(QPointF(p.x() * scaleFactor + ox, -p.y() * scaleFactor + oy));
    }
    return plist;
}
} // namespace post3dapp
