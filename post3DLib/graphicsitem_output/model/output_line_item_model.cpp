#include "output_line_item_model.h"

#include "calc_vector2d.h"
#include "define_npstn.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "manage_memberdata.h"
#include "model_elevation_template.h"
#include "unified_analysisdata.h"
#include "unified_output.h"


namespace post3dapp{

OutputLineItemModel::OutputLineItemModel(FigureTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid,
                                          const QList<QPointF> &plist, qreal tlen,
                                          VEC3D mypvec,VEC3D mypxvec)
    : OutputLineItem(fig, dtype, stype,uid,plist)
{
    myPlaneVector = mypvec; //描画面の法線
    myPlaneXVector = mypxvec;
    totalLength = tlen;
}

void OutputLineItemModel::setDrawingStatusDirect()
{


    //変断面
    //雑壁（モデル別）
    createWallPath();
    //耐力壁（モデル別）
    //平面応力・平面ひずみ
    //断面名
    if(myFigureTemplate->nameVisible(myDataType))createSectionName();
    //部材座標
    createMemberDirection();
    //部材番号
    createMemberNumber();
    //剛域
    createRigidPath();
    //フェイス
    createFacePath();
    //弾性支承ばね
    createMemberBanePath();

}
void  OutputLineItemModel::createWallPath(){
    ModelElevationTemplate* tp = static_cast<ModelElevationTemplate*>(myFigureTemplate);
    QColor modelColor = tp->zwallColor();

    qDebug()<<"Model図 部材にとりつく雑壁";
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();

    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    int nummz = model.imm1.at(numm).at(11)-1;
    //    ○線材に取り付く雑壁の分割データ
    //    NMZ   (       NUMMZD)	線材に取り付く雑壁数
    //    IMZ   (MXMZ  ,NUMMZD)	NUMZZ配列の番号
    //    DMZ   (MXMZ  ,NUMMZD)	y軸からの取り付き角度
    //    IMZTYP(MXMZ  ,NUMMZ)	雑壁のタイプ	０：従来の変断面
    //            １：架構内壁,一様断面(雑壁face)
    //            ２：架構内壁,一様断面(柱梁face)
    //            ３：架構内壁,スリット壁
    //            ４：架構外,片持壁
    //    IZZ   (NIZZ  ,NUMZZD)
    //    1	変断面数
    //    2	雑壁番号（NUMWZの番号）
    //    DZZ   (NDZZ  ,NZDMAX,NUMZZD)
    //    1	ℓi
    //    2	hi
    //    DZZE  (NDZZE=3 ,       NUMZZD)	包絡開口せい・位置
    //    1	せい
    //    2	開始長さ（≧I端フェイス）
    //    3	終了長さ（材長－J端フェイス）

    if(nummz!=-1){
        int nmz = model.nmz.at(nummz);

        QPointF ps = myPointList.first();
        QPointF pe = myPointList.last();
        if (CalcVector2D::length(pe - ps) < 0.1)return;
//        QColor modelColor=itemColor(ATTENTION_STATUS::STATUS_NORMAL);

        //        //ps-peに対して右が正か左が正かによって回転方向を場合分けすること
        qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));
        qreal t_ang;
        t_ang = p_ang + M_PI / 2.0;
        //BLD3Dでの実装　→　cpdriwz.f

        //imztype
        //0:従来型
        //1:架構内一様断面（雑壁フェイス
        //2:架構内一様断面（フレーム断面
        //3:スリット指針
        //4:架構外片持ち壁


        for(int i=0;i<nmz;i++){
            int imz = model.imz.at(nummz).at(i)-1;
            qreal dmz = model.dmz.at(nummz).at(i);//取りつき角度
            int imztyp = model.imztyp.at(nummz).at(i);//雑壁タイプ

                //部材Y座標・取りつき角度・紙面座標から、描画角度を計算
                //取りつき角度を見て紙面に投影
                MemberData* md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
                QList<VEC3D> tmp;
                tmp<<md->getWVector();
                CalcVector3D::rotateToVR(tmp,dmz,md->getUVector());
                qreal touei = CalcVector3D::dot(tmp.at(0),myPlaneVector);

                QPainterPath path;
                if(imztyp== 0){//従来型
                    int izz1 = model.izz.at(imz).at(0);//変断面数
        //            int izz2 = model.izz.at(imz).at(1);//雑壁番号（NUMWZのばんごう
                    for(int j=0;j<izz1;j++){
                    qreal hh=model.dzz.at(imz).at(j).at(1)*1000.0;
                    qreal st= (j==0 ? 0.0 : model.dzz.at(imz).at(j-1).at(0)) *1000.0;
                    qreal en=model.dzz.at(imz).at(j).at(0)*1000.0;

                    QPointF p1,p2;
                    p1 = QPointF(qCos(p_ang) * st, qSin(p_ang) * st);
                    p2 = QPointF(qCos(p_ang) * en, qSin(p_ang) * en);

                    path.moveTo(p1);
                    path.lineTo(p1 + QPointF(touei*hh*qCos(t_ang),
                                             touei*hh*qSin(t_ang)));
                    path.lineTo(p2 + QPointF(touei*hh*qCos(t_ang),
                                             touei*hh*qSin(t_ang)));
                    path.lineTo(p2);

                    int maxdiv=(en-st)/200.0;//1mあたり5本ぐらいのハッチ
                    hatch(path,p1,p2,hh*touei,t_ang,maxdiv);

                    const QPen pen(QBrush(modelColor), 0); //pen for svg
                    OTpathes.append(PATH_AND_PEN(path,pen));
                    }

                }else if(imztyp==1){
                    //1:架構内一様断面（雑壁フェイス
                    qreal memberLength = md->getTotalLength();
                    qreal irz = model.dmm1.at(numm).at(1);
                    qreal iry = model.dmm1.at(numm).at(0);
                    qreal jrz = model.dmm1.at(numm).at(4);
                    qreal jry = model.dmm1.at(numm).at(3);
                    qreal irigid = qMax(irz,iry)*1000;
                    qreal jrigid = memberLength -qMax(jrz,jry)*1000;

                    qreal hh=model.dzz.at(imz).at(2).at(1)*1000.0;
                    qreal st=irigid;
                    qreal en=jrigid;

                    QPointF p1,p2;
                    p1 = QPointF(qCos(p_ang) * st, qSin(p_ang) * st);
                    p2 = QPointF(qCos(p_ang) * en, qSin(p_ang) * en);

                    path.moveTo(p1);
                    path.lineTo(p1 + QPointF(touei*hh*qCos(t_ang),
                                             touei*hh*qSin(t_ang)));
                    path.lineTo(p2 + QPointF(touei*hh*qCos(t_ang),
                                             touei*hh*qSin(t_ang)));
                    path.lineTo(p2);

                    int maxdiv=(jrigid-irigid)/200.0;//1mあたり5本ぐらいのハッチ
                    hatch(path,p1,p2,hh*touei,t_ang,maxdiv);
                    const QPen pen(QBrush(modelColor), 0,Qt::SolidLine); //pen for svg
                    OTpathes.append(PATH_AND_PEN(path,pen));

                }else if(imztyp==2){
                    //2:架構内一様断面（フレーム断面
                    qreal memberLength = md->getTotalLength();
                    QVector<qreal> len_f_r=getOutputFaceRigidLength();
                    qreal irigid = len_f_r.at(2);
                    qreal jrigid = memberLength -len_f_r.at(3);


                    QVector<qreal> hh;
                    QVector<qreal> pt;
                    hh<<model.dzz.at(imz).at(1).at(1)*1000.0
                        <<model.dzz.at(imz).at(2).at(1)*1000.0
                          <<model.dzz.at(imz).at(3).at(1)*1000.0;
                    pt<<irigid
                      <<model.dzz.at(imz).at(1).at(0)*1000.0-irigid
                      <<model.dzz.at(imz).at(2).at(0)*1000.0-model.dzz.at(imz).at(1).at(0)*1000.0
                      <<memberLength-model.dzz.at(imz).at(2).at(0)*1000.0-jrigid;

                    QPointF p1,p2,p3,p4;
                    p1 = QPointF(qCos(p_ang) * pt[0], qSin(p_ang) * pt[0]);
                    p2 = QPointF(qCos(p_ang) * pt[1], qSin(p_ang) * pt[1]);
                    p3 = QPointF(qCos(p_ang) * pt[2], qSin(p_ang) * pt[2]);
                    p4 = QPointF(qCos(p_ang) * pt[3], qSin(p_ang) * pt[3]);

                    path.moveTo(p1);
                    path.lineTo(p1 + QPointF(touei*hh[0]*qCos(t_ang),
                                             touei*hh[0]*qSin(t_ang)));
                    path.lineTo(p2 + QPointF(touei*hh[0]*qCos(t_ang),
                                             touei*hh[0]*qSin(t_ang)));
                    path.lineTo(p2 + QPointF(touei*hh[1]*qCos(t_ang),
                                             touei*hh[1]*qSin(t_ang)));
                    path.lineTo(p3 + QPointF(touei*hh[1]*qCos(t_ang),
                                             touei*hh[1]*qSin(t_ang)));
                    path.lineTo(p3 + QPointF(touei*hh[2]*qCos(t_ang),
                                             touei*hh[2]*qSin(t_ang)));
                    path.lineTo(p4 + QPointF(touei*hh[2]*qCos(t_ang),
                                             touei*hh[2]*qSin(t_ang)));
                    path.lineTo(p4);

                    int maxdiv1=(pt[1]-pt[0])/200.0;//1mあたり5本ぐらいのハッチ
                    hatch(path,p1,p2,hh[0]*touei,t_ang,maxdiv1);
                    int maxdiv2=(pt[2]-pt[1])/200.0;//1mあたり5本ぐらいのハッチ
                    hatch(path,p2,p3,hh[1]*touei,t_ang,maxdiv2);
                    int maxdiv3=(pt[3]-pt[2])/200.0;//1mあたり5本ぐらいのハッチ
                    hatch(path,p3,p4,hh[2]*touei,t_ang,maxdiv3);

                    const QPen pen(QBrush(modelColor), 0,Qt::SolidLine); //pen for svg
                    OTpathes.append(PATH_AND_PEN(path,pen));

                }else if(imztyp==3){
                    //3:スリット指針
                    qreal hh=model.dzz.at(imz).at(2).at(1)*1000.0;
                    qreal st=model.dzz.at(imz).at(0).at(0)*1000.0;
                    qreal en=model.dzz.at(imz).at(2).at(0)*1000.0;

                    QPointF p1,p2;
                    p1 = QPointF(qCos(p_ang) * st, qSin(p_ang) * st);
                    p2 = QPointF(qCos(p_ang) * en, qSin(p_ang) * en);

                    path.moveTo(p1);
                    path.lineTo(p1 + QPointF(touei*hh*qCos(t_ang),
                                             touei*hh*qSin(t_ang)));
                    path.lineTo(p2 + QPointF(touei*hh*qCos(t_ang),
                                             touei*hh*qSin(t_ang)));
                    path.lineTo(p2);

//                    int maxdiv=(en-st)/200.0;//1mあたり5本ぐらいのハッチ
//                    hatch(path,p1,p2,hh*touei,t_ang,maxdiv);

                    //        path.lineTo(pe);
                    const QPen pen(QBrush(modelColor), 0,Qt::DashLine); //pen for svg
                    OTpathes.append(PATH_AND_PEN(path,pen));

                }else if(imztyp==4){
                    //4:架構外片持ち壁
                    qreal memberLength = md->getTotalLength();
                     QVector<qreal> len_f_r=getOutputFaceRigidLength();
                    qreal irigid = len_f_r.at(2);
                    qreal jrigid = memberLength -len_f_r.at(3);

                    qreal hh=model.dzz.at(imz).at(1).at(1)*1000.0;
                    qreal st=irigid;
                    qreal en=jrigid;

                    QPointF p1,p2;
                    p1 = QPointF(qCos(p_ang) * st, qSin(p_ang) * st);
                    p2 = QPointF(qCos(p_ang) * en, qSin(p_ang) * en);

                    path.moveTo(p1);
                    path.lineTo(p1 + QPointF(touei*hh*qCos(t_ang),
                                             touei*hh*qSin(t_ang)));
                    path.lineTo(p2 + QPointF(touei*hh*qCos(t_ang),
                                             touei*hh*qSin(t_ang)));
                    path.lineTo(p2);

                    int maxdiv=(en-st)/200.0;//1mあたり5本ぐらいのハッチ
                    hatch(path,p1,p2,hh*touei,t_ang,maxdiv);

                    const QPen pen(QBrush(modelColor), 0,Qt::SolidLine); //pen for svg
                    OTpathes.append(PATH_AND_PEN(path,pen));
                }

        }
    }

}
void  OutputLineItemModel::hatch(QPainterPath& path,QPointF p1,QPointF p2,qreal hh,qreal ang,int maxdiv){
for(int pos=1;pos<maxdiv;pos++){
    QPointF pt = ((qreal)pos)/(qreal)maxdiv*p1+((qreal)maxdiv-(qreal)pos)/(qreal)maxdiv*p2;
    path.moveTo(pt);
    path.lineTo(pt + QPointF(hh*qCos(ang),
                             hh*qSin(ang)));
}
}
void  OutputLineItemModel::baneHatch(QPainterPath& path,QPointF p1,QPointF p2,qreal hh,qreal ang,int maxdiv)
{
    for(int pos=1;pos<maxdiv;pos++){
//        QPointF ps = ((qreal)pos)/(qreal)maxdiv*p1+((qreal)maxdiv-(qreal)pos)/(qreal)maxdiv*p2;
        QPointF ps = (2.0*(qreal)pos-1.0)/((qreal)maxdiv*2.0-2.0)*p1
                +(((qreal)maxdiv*2.0-2.0)-(2.0*(qreal)pos-1.0))/((qreal)maxdiv*2.0-2.0)*p2;
        QPointF pe = ps+QPointF(hh*qCos(ang),hh*qSin(ang));
        QPointF dpy = 1.0/14.0*(pe-ps);
        qreal len = 0.5*qSqrt((p1.x()-p2.x())*(p1.x()-p2.x())+(p1.y()-p2.y())*(p1.y()-p2.y())) / maxdiv / 2.0;
        QPointF dpx = QPointF(len*qCos(ang+M_PI_2),len*qSin(ang+M_PI_2));
        QVector<QPointF> pp;
        pp << ps;
        pp << ps + dpy;
        pp << ps + 2.0*dpy + dpx;
        pp << ps + 4.0*dpy - dpx;
        pp << ps + 6.0*dpy + dpx;
        pp << ps + 8.0*dpy - dpx;
        pp << ps + 10.0*dpy + dpx;
        pp << ps + 12.0*dpy - dpx;
        pp << ps + 13.0*dpy;
        pp << pe;

        path.moveTo(ps);
        for(int i=0;i<10;i++){
            path.lineTo(pp[i]);
        }
    }
}

void  OutputLineItemModel::createFacePath()
{

    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();


    //ps-peに対して右が正か左が正かによって回転方向を場合分けすること
    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));

    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
//    qreal iface = md->getIFaceLength();
//    qreal jface = md->getJFaceLength();

    qreal totalLength = myLenList.last();
    qreal memberLength = md->getTotalLength();
    QVector<qreal> len_f_r=getOutputFaceRigidLength();
    qreal iface = len_f_r.at(0);
    qreal jface = memberLength -len_f_r.at(1);

    qreal rr = 120;
    QPointF iii = QPointF(qCos(p_ang) * (iface+rr) / memberLength * totalLength,
                          qSin(p_ang) * (iface+rr) / memberLength * totalLength);
    QPointF jjj = QPointF(qCos(p_ang) * (jface-rr) / memberLength * totalLength,
                          qSin(p_ang) * (jface-rr) / memberLength * totalLength);

    //見てる方向で、ピンは白丸 →　塗りつぶしてもっと上
    //フェイスは黒丸
    //L=0のところは、端に合わせる
    DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    int iyfix = cmdl.ifpmm.at(numm).at(0);
    int izfix = cmdl.ifpmm.at(numm).at(1);
    int jyfix = cmdl.ifpmm.at(numm).at(2);
    int jzfix = cmdl.ifpmm.at(numm).at(3);

    qreal yn=qAbs(CalcVector3D::dot(myPlaneVector,md->getVVector()));
    qreal zn=qAbs(CalcVector3D::dot(myPlaneVector,md->getWVector()));
    //
    //法線方向と、部材のY軸が一致すると、yfixを参照（軸図など）
    //法線方向と、部材のZ軸が一致すると、zfixを参照（伏図など）

    QPainterPath facePath = QPainterPath();

    const QPen pen(QBrush(Qt::black), 0); //pen for svg
    const QPen whitepen(QBrush(Qt::white), 0); //pen for svg

    //i
    if(!md->getMemberValues().iyBaneName.isEmpty() || !md->getMemberValues().izBaneName.isEmpty()){
        QPainterPath circle;
        circle.addEllipse(iii,2.0*rr,2.0*rr);
        OTpathes.append(PATH_AND_PEN(circle,whitepen,QBrush(Qt::white)));
        QPointF p = QPointF(qCos(p_ang) * (iface-rr) / memberLength * totalLength,
                              qSin(p_ang) * (iface-rr) / memberLength * totalLength);

        facePath.moveTo(p);
        facePath.lineTo(iii);
        for(int ii=0;ii<36*2+1;ii++){
            qreal r = rr*(qreal)ii/36.0;
            qreal th = (qreal)ii/36.0*M_TAU + p_ang;
            facePath.lineTo(iii+QPointF(r*qCos(th),r*qSin(th)));
        }
        OTpathes.append(PATH_AND_PEN(facePath,pen,QBrush(Qt::NoBrush)));
    }else if((zn >= yn && izfix == 1)//i is pin
            ||(yn > zn && iyfix == 1))//i is pin
    {
        facePath.addEllipse(iii,rr,rr);
        OTpathes.append(PATH_AND_PEN(facePath,pen,QBrush(Qt::NoBrush)));
    }else{
        facePath.addEllipse(iii,rr,rr);
        OTpathes.append(PATH_AND_PEN(facePath,pen,QBrush(Qt::black)));
    }
    facePath.clear();


    //j
    if(!md->getMemberValues().jyBaneName.isEmpty() || !md->getMemberValues().jzBaneName.isEmpty()){
        QPainterPath circle;
        circle.addEllipse(jjj,2.0*rr,2.0*rr);
        OTpathes.append(PATH_AND_PEN(circle,whitepen,QBrush(Qt::white)));

        QPointF p = QPointF(qCos(p_ang) * (jface+rr) / memberLength * totalLength,
                              qSin(p_ang) * (jface+rr) / memberLength * totalLength);
        facePath.moveTo(p);
        facePath.lineTo(jjj);
        for(int ii=0;ii<36*2+1;ii++){
            qreal r = rr*(qreal)ii/36.0;
            qreal th = (qreal)ii/36.0*M_TAU + p_ang;
            facePath.lineTo(jjj+QPointF(r*qCos(th),r*qSin(th)));
        }
        OTpathes.append(PATH_AND_PEN(facePath,pen,QBrush(Qt::NoBrush)));
    }else if((zn >= yn && jzfix == 1)//j is pin
            ||(yn > zn && jyfix == 1))//j is pin
    {
        facePath.addEllipse(jjj,rr,rr);
        OTpathes.append(PATH_AND_PEN(facePath,pen,QBrush(Qt::NoBrush)));
    }else{
        facePath.addEllipse(jjj,rr,rr);
        OTpathes.append(PATH_AND_PEN(facePath,pen,QBrush(Qt::black)));
    }


}
void  OutputLineItemModel::createRigidPath()
{

    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();


    //ps-peに対して右が正か左が正かによって回転方向を場合分けすること
    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));

    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    qreal totalLength = myLenList.last();
    qreal memberLength = md->getTotalLength();
    QVector<qreal> len_f_r=getOutputFaceRigidLength();
    qreal irigid = len_f_r.at(2);
    qreal jrigid = memberLength-len_f_r.at(3);


    QPointF ppp = QPointF(qCos(p_ang) * irigid / memberLength * totalLength,
                          qSin(p_ang) * irigid / memberLength * totalLength);
    QPointF jjj = QPointF(qCos(p_ang) * jrigid / memberLength * totalLength,
                          qSin(p_ang) * jrigid / memberLength * totalLength);



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

    ModelElevationTemplate* tp = static_cast<ModelElevationTemplate*>(myFigureTemplate);
    QColor color = tp->rigidColor();
    const QPen pen(QBrush(color), 0); //pen for svg

    OTpathes.append(PATH_AND_PEN(rigidPath,pen,QBrush(color)));


}

void  OutputLineItemModel:: createSectionName(){
    ModelElevationTemplate* tp = static_cast<ModelElevationTemplate*>(myFigureTemplate);
    if( !tp->nameVisible(myDataType))return;
    QColor color = tp->nameColor(myDataType,STRUCTTYPE::NOSTRUCTTYPE,ATTENTION_STATUS::STATUS_NORMAL);
    int size = tp->nameTextSize(myDataType);


    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);

    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();
    if (CalcVector2D::length(pe - ps) < 0.1)return;

    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));
    QPointF pos = QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                          -0.5 * ps.y() - 0.5 * pe.y());

    QString str;
    if(myFigureTemplate->isLineDirVisible()){
        str = md->sectionName().split('-').first()+u8" >";
    }else{
        str = md->sectionName().split('-').first();
    }
    POINT_AND_TEXT pat(pos,p_ang,str,color,OFFSETPOINT::OFFSET_CENTERTOP,999999.9,size);
    OTvaluetext.append(pat);
}
void  OutputLineItemModel:: createMemberNumber(){
    ModelElevationTemplate* tp = static_cast<ModelElevationTemplate*>(myFigureTemplate);
    if( !tp->nameVisible(myDataType))return;
    QColor color = tp->nameColor(myDataType,STRUCTTYPE::NOSTRUCTTYPE,ATTENTION_STATUS::STATUS_NORMAL);
    int size = tp->nameTextSize(myDataType);

    //    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);

    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();
    if (CalcVector2D::length(pe - ps) < 0.1)return;

    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));
    QPointF pos =QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                         -0.5 * ps.y() - 0.5 * pe.y());


    QString str= QString::number(numm+1,10);
    POINT_AND_TEXT pat(pos,p_ang,str,color,OFFSETPOINT::OFFSET_CENTERBOTTOM,999999.9,size);
    OTvaluetext.append(pat);


}
void  OutputLineItemModel:: createMemberDirection(){
    qDebug()<<"createMemberDirection";

    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    md->getVVector();
    md->getWVector();

    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();
    if (CalcVector2D::length(pe - ps) < 0.1)return;

    QPointF pos =QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                         0.5 * ps.y() + 0.5 * pe.y());

    QPainterPath path;
    qreal factor = 40.0; //深い意味は無し。アイコンサイズ
    int fac250 = static_cast<int>(25*factor);
    int fac150 = static_cast<int>(15*factor);
//    int fac500 = static_cast<int>(50*factor);

    //右回り左回りはチェック必要？normalizedって正負消す？
    if(qAbs(CalcVector3D::dot(myPlaneVector,md->getVVector())) < 0.1){//内積0.1以下を直交とみなす
        VEC3D myYvec = CalcVector3D::crossNormalized(myPlaneVector,myPlaneXVector);
        QPointF vec = QPointF(CalcVector3D::dot(md->getVVector(),myPlaneXVector),
                             CalcVector3D::dot(md->getVVector(),myYvec));
        qreal vec_ang = qAtan2(vec.y(), vec.x());

        path.moveTo(pos);
        path.lineTo(pos+QPointF(vec.x()*fac250,vec.y()*fac250));
        path.lineTo(pos+QPointF(vec.x()*fac250,vec.y()*fac250)+QPointF(qCos(vec_ang+M_PI_4*3.0)*fac250*0.3,qSin(vec_ang+M_PI_4*3.0)*fac250*0.3));
        path.moveTo(pos+QPointF(vec.x()*fac250,vec.y()*fac250));
        path.lineTo(pos+QPointF(vec.x()*fac250,vec.y()*fac250)+QPointF(qCos(vec_ang-M_PI_4*3.0)*fac250*0.3,qSin(vec_ang-M_PI_4*3.0)*fac250*0.3));

        qDebug()<<"createMemberDirection Vvec"<<vec;
    }
    if(qAbs(CalcVector3D::dot(myPlaneVector,md->getWVector())) < 0.1){//内積0.1以下を直交とみなす
        VEC3D myYvec = CalcVector3D::crossNormalized(myPlaneVector,myPlaneXVector);
        QPointF vec = QPointF(CalcVector3D::dot(md->getWVector(),myPlaneXVector),
                             CalcVector3D::dot(md->getWVector(),myYvec));
        qreal vec_ang = qAtan2(vec.y(), vec.x());

        path.moveTo(pos);
        path.lineTo(pos+QPointF(vec.x()*fac250,vec.y()*fac250));
        path.lineTo(pos+QPointF(vec.x()*fac250,vec.y()*fac250)+QPointF(qCos(vec_ang+M_PI_4*3.0)*fac250*0.3,qSin(vec_ang+M_PI_4*3.0)*fac250*0.3));
        path.moveTo(pos+QPointF(vec.x()*fac250,vec.y()*fac250));
        path.lineTo(pos+QPointF(vec.x()*fac250,vec.y()*fac250)+QPointF(qCos(vec_ang-M_PI_4*3.0)*fac250*0.3,qSin(vec_ang-M_PI_4*3.0)*fac250*0.3));

        path.moveTo(pos+QPointF(vec.x()*fac150,vec.y()*fac150));
        path.lineTo(pos+QPointF(vec.x()*fac150,vec.y()*fac150)+QPointF(qCos(vec_ang+M_PI_4*3.0)*fac250*0.3,qSin(vec_ang+M_PI_4*3.0)*fac250*0.3));
        path.moveTo(pos+QPointF(vec.x()*fac150,vec.y()*fac150));
        path.lineTo(pos+QPointF(vec.x()*fac150,vec.y()*fac150)+QPointF(qCos(vec_ang-M_PI_4*3.0)*fac250*0.3,qSin(vec_ang-M_PI_4*3.0)*fac250*0.3));
        qDebug()<<"createMemberDirection Wvec"<<vec;
    }
    qDebug()<<"createMemberDirection end";

    PATH_AND_PEN pap(path,QPen(Qt::black,0));


    OTpathes.append(pap);

}
void  OutputLineItemModel:: createMemberBanePath(){
    qDebug()<<"createMemberBanePath";

    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    DataNBLD3D_CMDL cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    VEC3D vv = md->getVVector();
    VEC3D vw = md->getWVector();
    qreal ky=cmdl.dmm1[numm][22];//弾性支承ばね剛性y
    qreal kz=cmdl.dmm1[numm][23];//弾性支承ばね剛性z
    qreal eps = 0.0000001;
    qreal chokkou = 0.1;//内積0.1以下を直交とみなす



    //   abs(y)>eps && 部材y方向が画面の法線に直交
    //or abs(z)>eps && 部材z方向が画面の法線に直交
    if((qAbs(ky)>eps && qAbs(CalcVector3D::dot(vv,myPlaneVector))<chokkou)
            ||(qAbs(kz)>eps && qAbs(CalcVector3D::dot(vw,myPlaneVector))<chokkou)){
        QPointF ps = myPointList.first();
        QPointF pe = myPointList.last();
        if (CalcVector2D::length(pe - ps) < 0.1)return;

        QPainterPath path;
        qreal hh = 1000.0; //深い意味は無し。アイコンサイズ

        VEC3D myYvec = CalcVector3D::crossNormalized(myPlaneVector,myPlaneXVector);
        QPointF vec = QPointF(CalcVector3D::dot(md->getUVector(),myPlaneXVector),
                             CalcVector3D::dot(md->getUVector(),myYvec));
        qreal vec_ang = qAtan2(vec.y(), vec.x());

        qreal totalLength = myLenList.last();
        qreal memberLength = md->getTotalLength();
        QVector<qreal> len_f_r=getOutputFaceRigidLength();
        qreal iface = len_f_r.at(0);
        qreal jface = memberLength-len_f_r.at(1);

        QPointF pfi = QPointF(qCos(vec_ang) * iface / memberLength * totalLength,
                              qSin(vec_ang) * iface / memberLength * totalLength);
        QPointF pfj = QPointF(qCos(vec_ang) * jface / memberLength * totalLength,
                              qSin(vec_ang) * jface / memberLength * totalLength);

//        path.moveTo(pfi);
//        path.lineTo(pfi+QPointF(qCos(vec_ang-M_PI_2)*hh,qSin(vec_ang-M_PI_2)*hh));
//        path.lineTo(pfj+QPointF(qCos(vec_ang-M_PI_2)*hh,qSin(vec_ang-M_PI_2)*hh));
//        path.lineTo(pfj);
        path.moveTo(pfi+QPointF(qCos(vec_ang-M_PI_2)*hh,qSin(vec_ang-M_PI_2)*hh));
        path.lineTo(pfj+QPointF(qCos(vec_ang-M_PI_2)*hh,qSin(vec_ang-M_PI_2)*hh));
        baneHatch(path,pfi,pfj,hh,vec_ang-M_PI_2,(int)(memberLength/1000));
        PATH_AND_PEN pap(path,QPen(Qt::black,0));
        OTpathes.append(pap);
    }


}

} // namespace post3dapp
