#include "section_gir_linedraw.h"
#include <QtGui>
namespace post3dapp{
SectionGirderLineDraw::SectionGirderLineDraw(const SectionDataType &type, QWidget *parent)
    : SectionLineDraw(type, parent)
{

}

void SectionGirderLineDraw::paintEvent( QPaintEvent * )
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
        drawRCFrameSection(&painter);
    } else if (mySectionType.getStructType() == STRUCTTYPE::STP_S) {
        drawSFrameSection(&painter);
    } else if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) {
        drawSRCFrameSection(&painter);
    }
}

void SectionGirderLineDraw::drawOuterFrame(QPainter *pt)
{
    bool isCantilever = mySection.line_param.isCantilever;
    bool table3input = mySectionType.hasData(myValues, 3);
    bool table4input = mySectionType.hasData(myValues, 4);
    QStringList label;

    if (!table3input && isCantilever) {
        drawflag = drawFlag::I;
        label << u8"全断面";
    } else if (table3input && isCantilever) {
        drawflag = drawFlag::IM;
        label << u8"元端" << u8"先端";
    } else if (!table3input && !table4input && !isCantilever) {
        drawflag = drawFlag::I;
        label << u8"全断面";
    } else if (!table3input && table4input && !isCantilever) {
        drawflag = drawFlag::IJ;
        label << u8"左端中央" << u8"右端";
    } else if (table3input && !table4input && !isCantilever) {
        drawflag = drawFlag::IM;
        label << u8"両端" << u8"中央";
    } else if (table3input && table4input && !isCantilever) {
        drawflag = drawFlag::IMJ;
        label << u8"左端" << u8"中央" << u8"右端";
    }
    int ww = width();
    pt->drawLine(QPoint(0, 25), QPoint(ww, 25));
    QString labely = "y";
    QString labelz = "z";

    int hh = height();
    if (drawflag == drawFlag::IJ || drawflag == drawFlag::IM) {
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
    } else if (drawflag == drawFlag::IMJ) {
        pt->drawLine(QPoint(  ww / 3, 0), QPoint(  ww / 3, hh));
        pt->drawLine(QPoint(2 * ww / 3, 0), QPoint(2 * ww / 3, hh));
        pt->setPen(Qt::black);
        pt->drawText(QRect(     0, 0, ww / 3, 25), Qt::AlignCenter, label.at(0));
        pt->drawText(QRect(  ww / 3, 0, ww / 3, 25), Qt::AlignCenter, label.at(1));
        pt->drawText(QRect(2 * ww / 3, 0, ww / 3, 25), Qt::AlignCenter, label.at(2));
        pt->setPen(Qt::lightGray);
        pt->drawLine(QPoint(0, 10 + hh / 2), QPoint(ww, 10 + hh / 2));
        pt->drawLine(QPoint(ww * 0.1667, 25), QPoint(ww * 0.1667, hh));
        pt->drawLine(QPoint(ww * 0.5, 25), QPoint(ww * 0.5, hh));
        pt->drawLine(QPoint(ww * 0.8333, 25), QPoint(ww * 0.8333, hh));
        pt->drawText(QRect(ww * 0.1667 - 20,     25, 20, 20), Qt::AlignCenter, labelz);
        pt->drawText(QRect(ww * 0.3333 - 20, 13 + hh / 2, 20, 20), Qt::AlignCenter, labely);
        pt->drawText(QRect(ww * 0.5 - 20,     25, 20, 20), Qt::AlignCenter, labelz);
        pt->drawText(QRect(ww * 0.6667 - 20, 13 + hh / 2, 20, 20), Qt::AlignCenter, labely);
        pt->drawText(QRect(ww * 0.8333 - 20,     25, 20, 20), Qt::AlignCenter, labelz);
        pt->drawText(QRect(ww - 20, 13 + hh / 2, 20, 20), Qt::AlignCenter, labely);
    }
}
} // namespace post3dapp
