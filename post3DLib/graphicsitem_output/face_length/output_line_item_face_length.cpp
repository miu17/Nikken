#include "output_line_item_face_length.h"

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

OutputLineItemFaceLength::OutputLineItemFaceLength(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist, const VEC3D mypvec,bool shape_on)
    : OutputLineItem(fig, dtype,stype, uid,plist,shape_on)
{
    Q_UNUSED(shape_on);
    myPlaneVector = mypvec; //描画面の法線

    createFacePath();
}

void  OutputLineItemFaceLength::createFacePath()
{

    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();


    //ps-peに対して右が正か左が正かによって回転方向を場合分けすること
    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));

    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    qreal totalLength = myLenList.last();
    qreal memberLength = md->getTotalLength();
    QVector<qreal> len_f_r=getOutputFaceRigidLength();
    iface = len_f_r.at(0);
    jface = memberLength-len_f_r.at(1);
    qreal rr = 120;
    QPointF ppp = QPointF(qCos(p_ang) * (iface+rr) / memberLength * totalLength,
                          qSin(p_ang) * (iface+rr) / memberLength * totalLength);
    QPointF jjj = QPointF(qCos(p_ang) * (jface-rr) / memberLength * totalLength,
                          qSin(p_ang) * (jface-rr) / memberLength * totalLength);


    QColor color = normalTextColor;
    OTvaluetext.append( POINT_AND_TEXT(QPointF(ps.x(), -ps.y()), p_ang, QString::number(iface, 'f', 0),
                                       color, OFFSETPOINT::OFFSET_RIGHTBOTTOM, 999999.0,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(pe.x(), -pe.y()), p_ang,
                                       QString::number(memberLength - jface, 'f', 0), color, OFFSETPOINT::OFFSET_LEFTTOP, 999999.0,valueTextSize) );




    QPainterPath facePath_i = QPainterPath();
    QPainterPath facePath_j = QPainterPath();

    facePath_i.addEllipse(ppp,rr,rr);
    facePath_j.addEllipse(jjj,rr,rr);


    facePath_i.closeSubpath();
    facePath_j.closeSubpath();

    const QPen pen0(QBrush(Qt::black), 0); //pen for svg
//    const QPen pen1(QBrush(Qt::NoBrush), 0); //pen for svg

    //見てる方向で、ピンは白丸
    //フェイスは黒丸
    //L=0のところは、端に合わせる
    DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    int iyfix = cmdl.ifpmm.at(numm).at(0);
    int izfix = cmdl.ifpmm.at(numm).at(1);
    int jyfix = cmdl.ifpmm.at(numm).at(2);
    int jzfix = cmdl.ifpmm.at(numm).at(3);

    //qreal xn=qAbs(CalcVector3D::dot(myPlaneVector,md->getUVector()));
    qreal yn=qAbs(CalcVector3D::dot(myPlaneVector,md->getVVector()));
    qreal zn=qAbs(CalcVector3D::dot(myPlaneVector,md->getWVector()));
    //
    //法線方向と、部材のY軸が一致すると、yfixを参照（軸図など）
    //法線方向と、部材のZ軸が一致すると、zfixを参照（伏図など）
    if((zn >= yn && izfix == 1)//i is pin
            ||(yn > zn && iyfix == 1))//i is pin
    {
        OTpathes.append(PATH_AND_PEN(facePath_i,pen0,QBrush(Qt::NoBrush)));
    }else{
        OTpathes.append(PATH_AND_PEN(facePath_i,pen0,QBrush(Qt::black)));
    }
    if((zn >= yn && jzfix == 1)//j is pin
            ||(yn > zn && jyfix == 1))//j is pin
    {
        OTpathes.append(PATH_AND_PEN(facePath_j,pen0,QBrush(Qt::NoBrush)));
    }else{
        OTpathes.append(PATH_AND_PEN(facePath_j,pen0,QBrush(Qt::black)));
    }


}

} // namespace post3dapp
