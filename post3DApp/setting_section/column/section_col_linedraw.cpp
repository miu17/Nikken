#include <QtGui>
#include "section_col_linedraw.h"
namespace post3dapp{
SectionColumnLineDraw::SectionColumnLineDraw(const SectionDataType &type, QWidget *parent)
    :SectionLineDraw(type, parent)
{

}

void SectionColumnLineDraw::paintEvent( QPaintEvent * )
{
    qDebug()<<"section line draw paint";

    QPainter painter( this );

    painter.setPen(QPen(Qt::darkGray, 1, Qt::SolidLine));
    QFont serifFont("Meiryo UI", 10);
    painter.setFont(serifFont);

    painter.drawRect(0, 0, width() - 1, height() - 1);

    if ( myValues.count() < 1 )return;
    drawOuterFrame(&painter);

    if (mySectionType.getStructType() == STRUCTTYPE::STP_RC) {
        drawRcColumnSection(&painter);
    } else if (mySectionType.getStructType() == STRUCTTYPE::STP_S) {
        drawSteelColumnSection(&painter);
    } else if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) {
        drawSrcColumnSection(&painter);
    } else if (mySectionType.getStructType() == STRUCTTYPE::STP_CFT) {
        drawCftColumnSection(&painter);
    }
}


void SectionColumnLineDraw::drawOuterFrame(QPainter *pt)
{
    int hh = height();
    int ww = width();

    bool table3input = mySectionType.hasData(myValues, 3);
    QStringList label;

    if (!table3input) {
        drawflag = drawFlag::I;
        label << u8"全断面";
    } else if (table3input) {
        drawflag = drawFlag::IJ;
        label << u8"柱脚" << u8"柱頭";
    }
    pt->drawLine(QPoint(0, 25), QPoint(ww, 25));

    QString labely = "y";
    QString labelz = "z";

    if (drawflag == drawFlag::IJ) {
        pt->drawLine(QPoint(ww / 2, 0), QPoint(ww / 2, hh));
        pt->setPen(Qt::black);
        pt->drawText(QRect(0, 0, ww / 2, 25), Qt::AlignCenter, label.at(0));
        pt->drawText(QRect(ww / 2, 0, ww / 2, 25), Qt::AlignCenter, label.at(1));
        pt->setPen(Qt::lightGray);
        pt->drawLine(QPoint(0, 10 + hh / 2), QPoint(ww, 10 + hh / 2));
        pt->drawLine(QPoint(ww * 0.25, 25), QPoint(ww * 0.25, hh));
        pt->drawLine(QPoint(ww * 0.75, 25), QPoint(ww * 0.75, hh));
        pt->drawText(QRect(ww * 0.25 - 20,     25, 20, 20), Qt::AlignCenter, labelz);
        pt->drawText(QRect(ww * 0.5 - 20, 13 + hh / 2, 20, 20), Qt::AlignCenter, labely);
        pt->drawText(QRect(ww * 0.75 - 20,     25, 20, 20), Qt::AlignCenter, labelz);
        pt->drawText(QRect(ww - 20, 13 + hh / 2, 20, 20), Qt::AlignCenter, labely);
    } else if (drawflag == drawFlag::I) {
        pt->setPen(Qt::black);
        pt->drawText(QRect(0, 0, ww, 25), Qt::AlignCenter, label.at(0));
        pt->setPen(Qt::lightGray);
        pt->drawLine(QPoint(0, 10 + hh / 2), QPoint(ww, 10 + hh / 2));
        pt->drawLine(QPoint(ww * 0.5, 25), QPoint(ww * 0.5, hh));
        pt->drawText(QRect(ww * 0.5 - 20,     25, 20, 20), Qt::AlignCenter, labelz);
        pt->drawText(QRect(ww - 20, 10 + hh / 2, 20, 20), Qt::AlignCenter, labely);
    }
}

void SectionColumnLineDraw::drawRcOneColumn(QPainter *pt, const LinePartsParameterSRC& param, qreal ox, qreal oy) const{
    int ind = barToDouble(param.mainBar1);
    int iand = barToDouble(param.axialBar);
    //qreal iRKb = param.RK;
    QList<QPointF> iplist = multiplyScaleFactor(param.barPointList(mySectionType.getMemberType(), false), scaleFactor, ox, oy);
    Q_FOREACH (QPointF pp, iplist) {
        drawReinShape(pt, qreal(ind), pp, scaleFactor);
    }
    QList<QPointF> iaplist = multiplyScaleFactor(param.barPointList(mySectionType.getMemberType(), true), scaleFactor, ox, oy);
    Q_FOREACH (QPointF pp, iaplist) {
        drawReinShape(pt, qreal(iand), pp, scaleFactor);
    }

    if (param.concType == ConcreteType::Type::CIRCLE) {
        drawConcCircle(pt, ox, oy, scaleFactor, param);
        if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) {
            drawScale(-param.Dy / 2., param.Dy / 2., -param.Dz / 2., param.Dz / 2., ox, oy, SCALEDW::D3N3, pt, scaleFactor);
        } else {
            drawScale(-param.Dy / 2., param.Dy / 2., -param.Dz / 2., param.Dz / 2., ox, oy, SCALEDW::D1N1, pt, scaleFactor);
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
            pt->setRenderHint(QPainter::Antialiasing, false);
        }
    } else if (param.concType == ConcreteType::Type::RECTANGLE) {
        drawConcRect(pt, ox, oy, scaleFactor, param);
        if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) {
            drawScale(-param.Dy / 2., param.Dy / 2., -param.Dz / 2., param.Dz / 2., ox, oy, SCALEDW::D3N3, pt, scaleFactor);
        } else {
            drawScale(-param.Dy / 2., param.Dy / 2., -param.Dz / 2., param.Dz / 2., ox, oy, SCALEDW::D1N1, pt, scaleFactor);
        }
        //hoop
        pt->setPen(QPen(Qt::darkGray, 1, Qt::SolidLine));
        pt->setRenderHint(QPainter::Antialiasing, false);
        QPointF p1 = QPointF(( param.Dy / 2. -param.RK + qreal(ind) / 2.) * scaleFactor + ox,
                             -( param.Dz / 2.0 - param.RK + qreal(ind) / 2.) * scaleFactor - 1 + oy); //右上
        QPointF p2 = QPointF(( param.Dy / 2. - param.RK + qreal(ind) / 2.) * scaleFactor + ox,
                             -(-param.Dz / 2.0 + param.RK - qreal(ind) / 2.) * scaleFactor  + oy); //右下
        QPointF p3 = QPointF((-param.Dy / 2. + param.RK - qreal(ind) / 2.) * scaleFactor + ox - 1,
                             -(-param.Dz / 2.0 + param.RK - qreal(ind) / 2.) * scaleFactor  + oy); //左下
        QPointF p4 = QPointF((-param.Dy / 2. + param.RK - qreal(ind) / 2.) * scaleFactor + ox - 1,
                             -( param.Dz / 2.0 - param.RK + qreal(ind) / 2.) * scaleFactor - 1 + oy); //左上
        if (param.hoopY > 0) {
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
        pt->setRenderHint(QPainter::Antialiasing, false);
    }
}


void SectionColumnLineDraw::drawRcColumnSection(QPainter *pt)
{
    scaleFactor = calcScaleFactor(0, 0, 0, 0, 0);
    if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) {
        scaleFactor = calcScaleFactor(mySection.i_param.Dy, mySection.i_param.Dz, 0., 0., scaleFactor);
        scaleFactor = calcScaleFactor(mySection.j_param.Dy, mySection.j_param.Dz, 0., 0., scaleFactor);
    } else {
        scaleFactor = calcScaleFactor(mySection.i_param.Dy, mySection.i_param.Dz, 0., 0., scaleFactor);
        scaleFactor = calcScaleFactor(mySection.j_param.Dy, mySection.j_param.Dz, 0., 0., scaleFactor);
    }

    if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) { //SRCのとき、SのScaleFactor考慮
        scaleFactor = calcScaleFactor(mySection.i_param.sizeB, mySection.i_param.sizeH, mySection.i_param.cY, mySection.i_param.cZ, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.j_param.sizeB,  mySection.j_param.sizeH, mySection.j_param.cY, mySection.j_param.cZ, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.i_param.H_T, mySection.i_param.B_T, mySection.i_param.cY_T, mySection.i_param.cZ_T, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.j_param.H_T, mySection.j_param.B_T, mySection.j_param.cY_T, mySection.j_param.cZ_T, scaleFactor);
    }
    if ( scaleFactor > 999. || scaleFactor < EPS ) return;

    qreal oy = 10 + height() / 2;
    if (drawflag == drawFlag::IJ) {
        drawRcOneColumn(pt, mySection.i_param, width() * 0.25, oy);
        drawRcOneColumn(pt, mySection.j_param, width() * 0.75, oy);
    } else if (drawflag == drawFlag::I) {
        drawRcOneColumn(pt, mySection.i_param, width() * 0.5, oy);
    }
}

void SectionColumnLineDraw::drawSteelOneColumn(QPainter *pt, const LinePartsParameterSRC& param, qreal ox, qreal oy) {
    if (param.steelType == SteelType::Type::ST_CROSSH || param.steelType == SteelType::Type::ST_CROSS) {
        drawScale(-param.sizeH/2.0, param.sizeH/2.0, -param.getSizeB()/2.0, param.getSizeB()/2.0, ox, oy, SCALEDW::D3N1, pt, scaleFactor);
        drawScale(-param.getSizeB_T()/2.0, param.getSizeB_T()/2.0, -param.H_T/2.0, param.H_T/2.0, ox, oy, SCALEDW::D3N2, pt, scaleFactor);
    } else {
        switch ( param.steelType ) {
        case SteelType::Type::ST_T3 :
        case SteelType::Type::ST_WT3 :
        case SteelType::Type::ST_T4 :
        case SteelType::Type::ST_WT4 :
        case SteelType::Type::ST_C3 :
        case SteelType::Type::ST_WC3 :
        case SteelType::Type::ST_C4 :
        case SteelType::Type::ST_WC4 :
        case SteelType::Type::ST_I :
        case SteelType::Type::ST_WI :
            drawScale(-param.getSizeB()/2.0, param.getSizeB()/2.0, -param.sizeH/2.0, param.sizeH/2.0, ox, oy, SCALEDW::D3N1, pt, scaleFactor);
            break;
        default:
            drawScale(-param.sizeH/2.0, param.sizeH/2.0, -param.getSizeB()/2.0, param.getSizeB()/2.0, ox, oy, SCALEDW::D3N1, pt, scaleFactor);
        }
    }
    if ( SteelType::isStandardType(param.steelType) ) {
        if ( param.steelType == SteelType::Type::ST_H ) {
            drawSteelH(pt, ox, oy, scaleFactor, param, true);
        } else if ( param.steelType == SteelType::Type::ST_BX ) {
            drawSteelRollBox(pt, ox, oy, scaleFactor, param, true);
        } else if ( param.steelType == SteelType::Type::ST_P ) {
            drawSteelP(pt, ox, oy, scaleFactor, param);
        }
    }

    if (param.steelType == SteelType::Type::ST_WH) {
        drawSteelH(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WI) {
        drawSteelH(pt, ox, oy, scaleFactor, param, false);
    } else if (param.steelType == SteelType::Type::ST_WBX) {
        drawSteelBuildBox(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WP) {
        drawSteelP(pt, ox, oy, scaleFactor, param);
    } else if (param.steelType == SteelType::Type::ST_WC1) {
        drawSteelC1(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WC2) {
        drawSteelC2(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WC3) {
        drawSteelC3(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WC4) {
        drawSteelC4(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WT1) {
        drawSteelT1(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WT2) {
        drawSteelT2(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WT3) {
        drawSteelT3(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WT4) {
        drawSteelT4(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WPL) {
        drawSteelPlate(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_CROSS) {
        drawSteelCross(pt, ox, oy, scaleFactor, param);
    } else if (param.steelType == SteelType::Type::ST_WBR) {
        drawSteelRound(pt, ox, oy, scaleFactor, param);
    }else if (param.steelType == SteelType::Type::ST_CROSSH) {
        drawSteelH(pt, ox, oy, scaleFactor, param, true);
        drawSteelH_T(pt, ox, oy, scaleFactor, param);
    }
}

void SectionColumnLineDraw::drawSrcSteelOneColumn(QPainter *pt, const LinePartsParameterSRC& param, qreal ox, qreal oy) {
    drawScale(-param.sizeH / 2. + param.cY, param.sizeH / 2. + param.cY, -param.getSizeB() / 2. + param.cZ, param.getSizeB() / 2. + param.cZ, ox, oy, SCALEDW::D3N1, pt, scaleFactor);
    drawScale(-param.getSizeB_T() / 2. + param.cY_T, param.getSizeB_T() / 2. + param.cY_T, -param.H_T / 2. + param.cZ_T, param.H_T / 2. + param.cZ_T, ox, oy, SCALEDW::D3N2, pt, scaleFactor);

    if ( SteelType::isStandardType(param.steelType) ) {
        if ( param.steelType == SteelType::Type::ST_H ) {
            drawSteelH(pt, ox, oy, scaleFactor, param, true);
        } else if ( param.steelType == SteelType::Type::ST_BX ) {
            drawSteelRollBox(pt, ox, oy, scaleFactor, param, true);
        } else if ( param.steelType == SteelType::Type::ST_P ) {
            drawSteelP(pt, ox, oy, scaleFactor, param);
        }
    }

    if (param.steelType == SteelType::Type::ST_WH) {
        drawSteelH(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WBX) {
        drawSteelBuildBox(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WP) {
        drawSteelP(pt, ox, oy, scaleFactor, param);
    } else if (param.steelType == SteelType::Type::ST_WC1) {
        drawSteelC1(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WC2) {
        drawSteelC2(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WT1) {
        drawSteelT1(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WT2) {
        drawSteelT2(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WPL) {
        drawSteelPlate(pt, ox, oy, scaleFactor, param, true);
    } else if (param.steelType == SteelType::Type::ST_WBR) {
        drawSteelRound(pt, ox, oy, scaleFactor, param);
    }

    if ( param.steelType_T == SteelType::Type::ST_H && SteelType::isStandardType(param.steelType_T) ) {
        drawSteelH_T(pt, ox, oy, scaleFactor, param);
    }else if (param.steelType_T == SteelType::Type::ST_WH) {
        drawSteelH_T(pt, ox, oy, scaleFactor, param);
    } else if (param.steelType_T == SteelType::Type::ST_WC1) {
        drawSteelC1_T(pt, ox, oy, scaleFactor, param);
    } else if (param.steelType_T == SteelType::Type::ST_WC2) {
        drawSteelC2_T(pt, ox, oy, scaleFactor, param);
    } else if (param.steelType_T == SteelType::Type::ST_WT1) {
        drawSteelT1_T(pt, ox, oy, scaleFactor, param);
    }else if (param.steelType_T == SteelType::Type::ST_WT2) {
        drawSteelT2_T(pt, ox, oy, scaleFactor, param);
    }
 }


void SectionColumnLineDraw::drawSteelColumnSection(QPainter *pt)
{
    if (mySectionType.getStructType() != STRUCTTYPE::STP_SRC){
        scaleFactor = calcScaleFactor(0, 0, 0, 0, 0);
    }
    if (mySection.i_param.steelType == SteelType::Type::ST_CROSSH || mySection.i_param.steelType == SteelType::Type::ST_CROSS) {
        scaleFactor = calcScaleFactor(mySection.i_param.sizeH, mySection.i_param.getSizeB(), 0.0, 0.0, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.i_param.getSizeB_T(), mySection.i_param.H_T, 0.0, 0.0, scaleFactor);
    } else {
        switch ( mySection.i_param.steelType ) {
        case SteelType::Type::ST_T3 :
        case SteelType::Type::ST_WT3 :
        case SteelType::Type::ST_T4 :
        case SteelType::Type::ST_WT4 :
        case SteelType::Type::ST_C3 :
        case SteelType::Type::ST_WC3 :
        case SteelType::Type::ST_C4 :
        case SteelType::Type::ST_WC4 :
        case SteelType::Type::ST_I :
        case SteelType::Type::ST_WI :
            scaleFactor = calcScaleFactor(mySection.i_param.getSizeB(), mySection.i_param.sizeH, 0.0, 0.0, scaleFactor);
            break;
        default:
            scaleFactor = calcScaleFactor(mySection.i_param.sizeH, mySection.i_param.getSizeB(), 0.0, 0.0, scaleFactor);
            break;
        }
    }

    if (mySection.j_param.steelType == SteelType::Type::ST_CROSSH || mySection.j_param.steelType == SteelType::Type::ST_CROSS ){
        scaleFactor = calcScaleFactor(mySection.j_param.sizeH, mySection.j_param.getSizeB(), 0.0, 0.0, scaleFactor);
        scaleFactor = calcScaleFactor(mySection.j_param.getSizeB_T(), mySection.j_param.H_T, 0.0, 0.0, scaleFactor);
    }else{
        switch ( mySection.j_param.steelType ) {
        case SteelType::Type::ST_T3 :
        case SteelType::Type::ST_WT3 :
        case SteelType::Type::ST_T4 :
        case SteelType::Type::ST_WT4 :
        case SteelType::Type::ST_C3 :
        case SteelType::Type::ST_WC3 :
        case SteelType::Type::ST_C4 :
        case SteelType::Type::ST_WC4 :
        case SteelType::Type::ST_I :
        case SteelType::Type::ST_WI :
            scaleFactor = calcScaleFactor(mySection.j_param.getSizeB(), mySection.j_param.sizeH, 0.0, 0.0, scaleFactor);
            break;
        default:
            scaleFactor = calcScaleFactor(mySection.j_param.sizeH, mySection.j_param.getSizeB(), 0.0, 0.0, scaleFactor);
            break;
        }
    }

    if ( scaleFactor > 999. || scaleFactor < EPS ) return;

    qreal oy = 10 + height() / 2;
    if (drawflag == drawFlag::IJ) {
        drawSteelOneColumn(pt, mySection.i_param, width() * 0.25, oy);
        drawSteelOneColumn(pt, mySection.j_param, width() * 0.75, oy);
    } else if (drawflag == drawFlag::I) {
        drawSteelOneColumn(pt, mySection.i_param, width() * 0.5, oy);
    }
}

void SectionColumnLineDraw::drawSrcSteelColumnSection(QPainter *pt)
{
    scaleFactor = calcScaleFactor(0, 0, 0, 0, 0);

    scaleFactor = calcScaleFactor(mySection.i_param.sizeH, mySection.i_param.getSizeB(), mySection.i_param.cY, mySection.i_param.cZ, scaleFactor);
    scaleFactor = calcScaleFactor(mySection.i_param.getSizeB_T(), mySection.i_param.H_T, mySection.i_param.cY_T, mySection.i_param.cZ_T, scaleFactor);
    scaleFactor = calcScaleFactor(mySection.j_param.sizeH, mySection.j_param.getSizeB(), mySection.j_param.cY, mySection.j_param.cZ, scaleFactor);
    scaleFactor = calcScaleFactor(mySection.j_param.getSizeB_T(), mySection.j_param.H_T, mySection.j_param.cY_T, mySection.j_param.cZ_T, scaleFactor);

    scaleFactor = calcScaleFactor(mySection.i_param.Dy, mySection.i_param.Dz, 0., 0., scaleFactor);
    scaleFactor = calcScaleFactor(mySection.j_param.Dy, mySection.i_param.Dz, 0., 0., scaleFactor);

    if ( scaleFactor > 999. || scaleFactor < EPS ) return;

    qreal oy = 10 + height() / 2;
    if (drawflag == drawFlag::IJ) {
        drawSrcSteelOneColumn(pt, mySection.i_param, width() * 0.25, oy);
        drawSrcSteelOneColumn(pt, mySection.j_param, width() * 0.75, oy);
    } else if (drawflag == drawFlag::I) {
        drawSrcSteelOneColumn(pt, mySection.i_param, width() * 0.5, oy);
    }
}

void SectionColumnLineDraw::drawSrcColumnSection(QPainter *pt)
{
    drawRcColumnSection(pt);
    drawSrcSteelColumnSection(pt);
}

void SectionColumnLineDraw::drawCftColumnSection(QPainter *pt)
{
    drawSteelColumnSection(pt);
}

} // namespace post3dapp
