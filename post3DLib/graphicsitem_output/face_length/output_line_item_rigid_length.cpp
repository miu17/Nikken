#include "output_line_item_rigid_length.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOption>

#include "calc_vector2d.h"
#include "fixed_data.h"
#include "manage_memberdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"
#include "unified_settings.h"

namespace post3dapp{

OutputLineItemRigidLength::OutputLineItemRigidLength(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, bool shape_on)
    : OutputLineItem(fig, dtype,stype, uid,plist,shape_on)
{
    Q_UNUSED(shape_on);

    createRigidPath();
}

void  OutputLineItemRigidLength::createRigidPath()
{

    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();
    QPointF p25 = 0.53*ps+0.47*pe;
    QPointF p75 = 0.47*ps+0.53*pe;


    //ps-peに対して右が正か左が正かによって回転方向を場合分けすること
    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));

    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    qreal totalLength = myLenList.last();
    qreal memberLength = md->getTotalLength();
    qreal irigidy = cmdl.dmm1.at(numm).at(0)*1000;
    qreal jrigidy = memberLength -cmdl.dmm1.at(numm).at(3)*1000;
    qreal irigidz = cmdl.dmm1.at(numm).at(1)*1000;
    qreal jrigidz = memberLength -cmdl.dmm1.at(numm).at(4)*1000;

    QVector<qreal> len_f_r=getOutputFaceRigidLength();
    qreal irigid = len_f_r.at(2);
    qreal jrigid = memberLength-len_f_r.at(3);

    QPointF ppp = QPointF(qCos(p_ang) * irigid / memberLength * totalLength,
                          qSin(p_ang) * irigid / memberLength * totalLength);
    QPointF jjj = QPointF(qCos(p_ang) * jrigid / memberLength * totalLength,
                          qSin(p_ang) * jrigid / memberLength * totalLength);


    QColor color = normalTextColor;
    //中央左下：i端剛域長y
    //中央左上：i端剛域長z
    //中央右下：j端剛域長y
    //中央右上：j端剛域長z
    OTvaluetext.append( POINT_AND_TEXT(QPointF(p25.x(), -p25.y()), p_ang, QString::number(irigidz, 'f', 0),
                                      color, OFFSETPOINT::OFFSET_LEFTTOP, 999999.0,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(p75.x(), -p75.y()), p_ang,
                                      QString::number(memberLength - jrigidz, 'f', 0), color, OFFSETPOINT::OFFSET_RIGHTTOP, 999999.0,valueTextSize) );

    OTvaluetext.append( POINT_AND_TEXT(QPointF(p25.x(), -p25.y()), p_ang, QString::number(irigidy, 'f', 0),
                                      color, OFFSETPOINT::OFFSET_LEFTBOTTOM, 999999.0,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(p75.x(), -p75.y()), p_ang,
                                      QString::number(memberLength - jrigidy, 'f', 0), color, OFFSETPOINT::OFFSET_RIGHTBOTTOM, 999999.0,valueTextSize) );


    qreal rigidWidth = 120.0;
    QPainterPath rigidPath = QPainterPath();

    QPointF q1, q2, q3, q4;

    QPointF p1;
    QPointF p2;
    for (int i = 0; i < 2; i++) {
        if (i == 0) { //i端
            p1 = ps;
            p2 = ppp;
        } else {  //j端
            p1 = jjj;
            p2 = pe;
        }
        qreal ag = qAtan2(p2.y() - p1.y(), p2.x() - p1.x());

        q1 = p1 + QPointF(rigidWidth / 2.0 * qCos(ag + M_PI / 2.0),
                          rigidWidth / 2.0 * qSin(ag + M_PI / 2.0));
        q2 = p2 + QPointF(rigidWidth / 2.0 * qCos(ag + M_PI / 2.0),
                          rigidWidth / 2.0 * qSin(ag + M_PI / 2.0));
        q3 = p2 + QPointF(rigidWidth / 2.0 * qCos(ag - M_PI / 2.0),
                          rigidWidth / 2.0 * qSin(ag - M_PI / 2.0));
        q4 = p1 + QPointF(rigidWidth / 2.0 * qCos(ag - M_PI / 2.0),
                          rigidWidth / 2.0 * qSin(ag - M_PI / 2.0));

        QPolygonF polygon;
        polygon << q1 << q2 << q3 << q4;
        rigidPath.addPolygon(polygon);
    }

    rigidPath.closeSubpath();

    const QPen pen(Qt::black, 0); //pen for svg

    OTpathes.append(PATH_AND_PEN(rigidPath,pen,QBrush(Qt::black)));


}

} // namespace post3dapp
