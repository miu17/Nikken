#ifndef CALCLOADMEMBERLOADITEM_H
#define CALCLOADMEMBERLOADITEM_H

#include "calcloaditem_line.h"
namespace post3dapp{
class CalcLoadMemberLoadItem : public CalcLoadLineItem
{
public:

    CalcLoadMemberLoadItem( DATATYPE dtype, const QUuid &uid,
                            const CalculationLoad2DViewSettings& setting,
                            const FIGURE_TYPE_PROPERTY &figureTypeProperty,
                            const QList<QPointF> &plist, qreal tlen,
                            bool zin, const QList<MLOADSUM *> & );

    void setStatus(const FIGURE_TYPE_PROPERTY&);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QPainterPath sumUpLoadPath;
    QPainterPath concentratedLoadPath;
    QPainterPath wabLoadPath;

    const qreal totalLength;
    const QList<MLOADSUM *> myLoads;
    const bool isZin; // 部材Z軸が面内であるか否か

    LoadType::LOADTYPE loadType;
    LOADSUMTYPE loadSumType;
    OUTPUTDIR outputDir;

    int stringSize;

    bool pLoadVisible;
    bool dLoadVisible;
    bool wLoadVisible;
    QColor pLoadColor;
    QColor dLoadColor;
    QColor wLoadColor;
    qreal pLoadScale;
    qreal dLoadScale;
    qreal wLoadScale;


    void appendSumUpLoad( const QString &, const QList<qreal> &, bool isMoment = false );
    void appendConcentratedLoad( const QString &, const QList<QPointF> &, bool isMoment = false );
    void appendWABLoad( const QString &, const WABLOAD &, bool isMoment = false );

};
} // namespace post3dapp
#endif
