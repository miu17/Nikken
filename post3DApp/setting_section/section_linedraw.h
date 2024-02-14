#ifndef SECTIONLINEDRAW_H
#define SECTIONLINEDRAW_H

#include <QFrame>
#include "define_sectiontable.h"
#include "define_section_struct.h"

namespace post3dapp{
enum class drawFlag {I, IM, IJ, IMJ};
enum class SCALEDW {D1N1 = 0, D2N1, D2N2, D3N1, D3N2, D3N3};

class SectionLineDraw : public QFrame
{
    Q_OBJECT

public:

    SectionLineDraw( const SectionDataType &type, QWidget *parent = 0 );

    void setLineValues(const QList<QVariant> &values, const SectionValues &section)
    {
        myValues = values;
        mySection = section;
        update();
    }

protected:
    static const qreal EPS;
    virtual void drawOuterFrame(QPainter *painter) =  0;

    QList<QVariant> myValues;
    SectionValues mySection;
    SectionDataType mySectionType;
    drawFlag drawflag;
    qreal calcScaleFactor(qreal, qreal, qreal, qreal, qreal);
    qreal scaleFactor;
    void drawRCFrameOneSection(QPainter *painter, const LinePartsParameterSRC&, qreal, qreal, bool);
    void drawRCFrameSection(QPainter *painter);
    void drawSFrameOneSection(QPainter *pt,  const LinePartsParameterSRC& param, qreal ox, qreal oy, bool isSRC);
    void drawSFrameSection(QPainter *painter);
    void drawSRCFrameSection(QPainter *painter);
    static void drawArrow(const QPointF &p1, const QPointF &p2, int size, QPainter *pt);
    static void drawCrossPoint(const QPointF&, const QPointF&, qreal, qreal, qreal, QPainter *pt);
    static void drawReinShape(QPainter *, qreal, const QPointF&, qreal);
    void drawScale(qreal, qreal, qreal, qreal, qreal, qreal, SCALEDW, QPainter *, qreal) const;

    static void drawConcRect(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&);
    static void drawConcCircle(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&);

    static void drawSteelH(QPainter * ,qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelH_T(QPainter * ,qreal, qreal, qreal, const LinePartsParameterSRC&);
    static void drawSteelT1(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelT2(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelT3(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelT4(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelT1_T(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&);
    static void drawSteelT2_T(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&);
    static void drawSteelC1(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelC2(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelC3(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelC4(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelC1_T(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&);
    static void drawSteelC2_T(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&);

    static void drawSteelPlate(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool);
    static void drawSteelRound(QPainter * ,qreal, qreal, qreal, const LinePartsParameterSRC&);
    static void drawSteelCross(QPainter * ,qreal, qreal, qreal, const LinePartsParameterSRC&);
    void drawSteelBuildBox(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool) const;
    void drawSteelRollBox(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&, bool) const;
    void drawSteelP(QPainter *, qreal, qreal, qreal, const LinePartsParameterSRC&) const;


    qreal barToDouble(const QString &phi) const
    {
        return ( phi.isEmpty() ) ? 0.0 : phi.mid(1).toDouble();
    }
    QList<QPointF> multiplyScaleFactor(const QList<QPointF>&, qreal, qreal, qreal) const;

public slots:

private slots:

signals:

private:
    QPointF mPoint;
    static void drawSteelI(qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, QPainter *, qreal);
    static void drawSteelH(qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, QPainter *, qreal);

    static void drawSteelCRight(QPainter *, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal); //[
    static void drawSteelCTop(QPainter *, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal);//|_|
    static void drawSteelCBottom(QPainter *, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal);
    static void drawSteelCLeft(QPainter *, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal); // ]

    static void drawSteelTRight(QPainter *, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal); //|-
    static void drawSteelTTop(QPainter *, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal);
    static void drawSteelTBottom(QPainter *, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal); // T
    static void drawSteelTLeft(QPainter *, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal); //  -|



};
} // namespace post3dapp
#endif
