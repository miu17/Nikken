#include "output_line_item_deformation.h"

#include "calc_vector2d.h"
#include "define_section_struct.h"
#include "deformation_elevation_template.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "fortranFunc.h"
#include "manage_memberdata.h"
#include "manage_jointdata.h"
#include "output_graphicsitem.h"
#include "unified_output.h"
#include "unified_analysisdata.h"

namespace post3dapp{

OutputLineItemDeformation::OutputLineItemDeformation(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid,
                                                     const QList<QPointF> &plist,bool zin,bool isCW,VEC3D mypvec,VEC3D mypxvec, bool shape_on)
    : OutputLineItem(fig, dtype,stype, uid,plist,shape_on)
{
    myPlaneVector = mypvec; //描画面の法線
    myPlaneXVector = mypxvec;
    isZin=zin;
    isClockWize=isCW;
    createDeformationShapePathWithShear();
}


void OutputLineItemDeformation::figplt(qreal XA0,qreal YA0,qreal XB0,qreal YB0,
                                       qreal UAI,qreal UBI,qreal VAI,qreal VBI,
                                       qreal TAI,qreal TBI,qreal RAL1,qreal RAL2,qreal FAL1,qreal FAL2,int PINA,int PINB,
                                       qreal EI,qreal GA,qreal D,QVector<qreal> xpos,QVector<qreal> qq,QVector<qreal> mm,int idv,qreal UBAI)
{
//UBAI=0.00001*UBAI;
    qreal EPS = 0.00000000001;
    QColor color= itemColor(ATTENTION_STATUS::STATUS_WARNING);

    //参考：figplt.f
    //上記＋せん断・ピン端について考慮した変形図を描画

    //単位系に依存してないが、基本的にkN-mm-radで揃えて渡しているつもり
    //(XA0,YA0)・・・I端の座標（紙面系）
    //(XB0,YB0)・・・J端の座標（紙面系）
    //(UAI,VAI)・・・I端の変位
    //(UBI,VBI)・・・J端の変位
    //(TAI,TBI)・・・IJ端の節点回転角
    //(RAL1,RAL2)・・・剛域長（節点間距離に対する割合）
    //(FAL1,FAL2)・・・フェイス長（同上）
    //Qi,Qj・・・節点位置のせん断力
    //GA・・・せん断弾性係数　ｘ　断面積
    //D・・・部材せい

    //部材変位と節点回転角、パネルの変形を考慮して剛域端の部材座標を求める
    //せん断変形を考慮し、剛域端の部材座標からせん断変形分を差し引く
    //上記の端部座標・端部角度からたわみ曲線求める　・・・A
    //プロットする中心座標・・・Aに、その座標までのせん断変形の総和を足す
    //プロットの上下座標・・・中心座標からＡの直交方向

    //disp
    qreal XAI=XA0+UAI*UBAI;
    qreal YAI=YA0+VAI*UBAI;
    qreal XBI=XB0+UBI*UBAI;
    qreal YBI=YB0+VBI*UBAI;

    //joint-joint kyori
    qreal XBA=XB0-XA0;
    qreal YBA=YB0-YA0;
    qreal XL2=XBA*XBA+YBA*YBA;
    qreal XL1=qSqrt(XL2);
    qreal RL=XL1*RAL1;
    qreal RR=XL1*RAL2;
    qreal FL=XL1*FAL1;
    qreal FR=XL1*FAL2;
    qreal XCC=XBA/XL1;
    qreal XSS=YBA/XL1;
    qreal SIA=qSin(-TAI*UBAI);
    qreal COA=qCos(-TAI*UBAI);
    qreal SIB=qSin(-TBI*UBAI);
    qreal COB=qCos(-TBI*UBAI);
    qreal TT0=atan2(YBA,XBA);//部材角

    //剛域端フェイス端
    qreal XAR=XAI+RL*(XCC*COA-XSS*SIA);
    qreal YAR=YAI+RL*(XSS*COA+XCC*SIA);
    qreal XBR=XBI-RR*(XCC*COB-XSS*SIB);
    qreal YBR=YBI-RR*(XSS*COB+XCC*SIB);


    QVector<qreal> XA_bend(idv,0.0);
    QVector<qreal> YA_bend(idv,0.0);
    QVector<qreal> XA_center(idv,0.0);
    QVector<qreal> YA_center(idv,0.0);
    QVector<qreal> XA_u(idv,0.0);
    QVector<qreal> YA_u(idv,0.0);
    QVector<qreal> XA_b(idv,0.0);
    QVector<qreal> YA_b(idv,0.0);
    QVector<qreal> sendan_disp(idv,0.0);
    QVector<qreal> mage_disp(idv,0.0);
    QVector<qreal> kaku_disp(idv,0.0);


    sendan_disp[0]=0.0;//剛域端
    for(int i=0;i<idv-1;i++){
        qreal dl = xpos[i+1]-xpos[i];
        sendan_disp[i+1]= sendan_disp[i]+UBAI*(0.5*qq[i+1]+0.5*qq[i])*dl/GA;
    }
    //曲げによるたわみ角
    kaku_disp[0]=0.0;
    EI = EI * 1000000.0;//kNm2 -> kNmm2
    for(int i=0;i<idv-1;i++){
        qreal dl = xpos[i+1]-xpos[i];
        kaku_disp[i+1]= kaku_disp[i]-(0.5*mm[i+1]+0.5*mm[i])*dl/EI;
    }
    //曲げによるたわみ
    mage_disp[0]=0.0;//剛域端
    for(int i=0;i<idv-1;i++){
        qreal dl = xpos[i+1]-xpos[i];
        mage_disp[i+1]= mage_disp[i]+UBAI*(0.5*kaku_disp[i+1]+0.5*kaku_disp[i])*dl;
    }
    //曲げたわみの基線補正
    qreal error = mage_disp[idv-1]-mage_disp[0];
    for(int i=0;i<idv;i++){
        mage_disp[i] -= error*xpos[i]/xpos[idv-1];
    }



    //pin-pin
    if(PINA==1 && PINB==1){
        calcBendingCurvePP(XA_bend, YA_bend, XAR, YAR,XBR+ qSin(TT0)*sendan_disp.last(), YBR- qCos(TT0)*sendan_disp.last(), RL,RR, FL, FR, TAI, TBI, TT0,XCC, XSS, idv,UBAI);
    }
    //pin-fix
    if(PINA==1 && PINB==0){
        calcBendingCurvePF(XA_bend, YA_bend, XAR, YAR,XBR+ qSin(TT0)*sendan_disp.last(), YBR- qCos(TT0)*sendan_disp.last(), RL,RR, FL, FR, TAI, TBI, TT0,XCC, XSS, idv,UBAI);
    }
    //fix-pin
    if(PINA==0 && PINB==1){
        calcBendingCurveFP(XA_bend, YA_bend, XAR, YAR,XBR+ qSin(TT0)*sendan_disp.last(), YBR- qCos(TT0)*sendan_disp.last(), RL,RR, FL, FR, TAI, TBI, TT0,XCC, XSS, idv,UBAI);
    }
    //fix-fix
    if(PINA==0 && PINB==0){
        calcBendingCurveFF(XA_bend, YA_bend, XAR, YAR,XBR+ qSin(TT0)*sendan_disp.last(), YBR- qCos(TT0)*sendan_disp.last(), RL,RR, FL, FR, TAI, TBI, TT0, idv,UBAI);
    }
    //hinge-hinge
    if(PINA==2 && PINB==2){
        qreal XBRmod=XBR+ qSin(TT0)*sendan_disp.last();
        qreal YBRmod=YBR- qCos(TT0)*sendan_disp.last();
        qreal XBA=XBRmod-XAR;
        qreal YBA=YBRmod-YAR;
        qreal XL2=XBA*XBA+YBA*YBA;
        qreal XL1=qSqrt(XL2);//弾性域長さ
        qreal TTR=atan2(YBA,XBA);

        qreal TA=-TAI*UBAI+(TT0-TTR);
        qreal TB=-TBI*UBAI+(TT0-TTR);
        qreal x1 = FL-RL;
        qreal xidv2 = XL1-FR+RR;
        qreal xidv1= XL1;
        qreal moment_iface = -EI*((4.0*TA+2.0*TB)/XL1+6.0*(TA+TB)*x1/XL1/XL1);
        qreal moment_jface = -EI*((4.0*TA+2.0*TB)/XL1+6.0*(TA+TB)*xidv2/XL1/XL1);
        qreal Mpi = qAbs(mm[1]);
        qreal Mpj = qAbs(mm[idv-2]);
        qreal ratio=qAbs((moment_jface-moment_iface)/(Mpi+Mpj));
        qreal delta =xidv1*qSin(TA);
        QVector<qreal > XA_kari(idv,0.0);
        QVector<qreal > YA_kari(idv,0.0);
        if(moment_iface * moment_jface < 0.0){
            calcBendingCurveFF(XA_kari, YA_kari, XAR, YAR,XBRmod- qSin(TT0)*(ratio-1.0)/ratio*delta, YBRmod+ qCos(TT0)*(ratio-1.0)/ratio*delta, RL,RR, FL, FR, TAI, TBI, TT0, idv,UBAI);
            for(int i=0;i<=1;i++){//i端側はそのまま
                XA_bend[i]=XA_kari[i];
                YA_bend[i]=YA_kari[i];
            }
            for(int i=idv-2;i<=idv-1;i++){//j端側は平行移動
                XA_bend[i]=XA_kari[i]+ qSin(TT0)*(ratio-1.0)/ratio*delta;
                YA_bend[i]=YA_kari[i]- qCos(TT0)*(ratio-1.0)/ratio*delta;
            }
            for(int i=2;i<=idv-3;i++){//iのヒンジを中心にθ回転。回転移動と平行移動の差分があるので、若干長さストレッチ
                qreal len1=qSqrt((XA_bend[idv-2]-XA_bend[1])*(XA_bend[idv-2]-XA_bend[1])+(YA_bend[idv-2]-YA_bend[1])*(YA_bend[idv-2]-YA_bend[1]));
                qreal len2=qSqrt((XA_kari[idv-2]-XA_kari[1])*(XA_kari[idv-2]-XA_kari[1])+(YA_kari[idv-2]-YA_kari[1])*(YA_kari[idv-2]-YA_kari[1]));
                XA_bend[i]=XA_kari[i]-XA_kari[1];
                YA_bend[i]=YA_kari[i]-YA_kari[1];
                qreal theta1 = atan2(YA_bend[idv-2]-YA_bend[1],XA_bend[idv-2]-XA_bend[1]);
                qreal theta2 = atan2(YA_kari[idv-2]-YA_kari[1],XA_kari[idv-2]-XA_kari[1]);
                qreal strech= len1/len2;
                qreal cc = qCos(theta1-theta2);
                qreal ss = qSin(theta1-theta2);
                qreal x=cc*XA_bend[i]-ss*YA_bend[i];
                qreal y=ss*XA_bend[i]+cc*YA_bend[i];
                XA_bend[i]=strech*x+XA_kari[1];
                YA_bend[i]=strech*y+YA_kari[1];
            }
        }else{//両端ヒンジだが、モーメントが同じ符号の時はピン―ピンと同じ表示
            calcBendingCurvePP(XA_bend, YA_bend, XAR, YAR,XBR+ qSin(TT0)*sendan_disp.last(), YBR- qCos(TT0)*sendan_disp.last(), RL,RR, FL, FR, TAI, TBI, TT0,XCC, XSS, idv,UBAI);
        }
    }

    //hinge-fix
    if(PINA==2 && PINB==0){

        qreal XBRmod=XBR+ qSin(TT0)*sendan_disp.last();
        qreal YBRmod=YBR- qCos(TT0)*sendan_disp.last();
        qreal XBA=XBRmod-XAR;
        qreal YBA=YBRmod-YAR;
        qreal XL2=XBA*XBA+YBA*YBA;
        qreal XL1=qSqrt(XL2);//弾性域長さ
        qreal TTR=atan2(YBA,XBA);

        QVector<qreal> xpos2(idv,0.0);
        xpos2[0]=0.0;
        xpos2[1]=FL-RL;
        xpos2[idv-2]=XL1-FR+RR;
        xpos2[idv-1]=XL1;
        for(int K=2;K<=idv-3;K++){
            xpos2[K]=xpos2[1]+(qreal)(K-1)*(xpos2[idv-2]-xpos2[1])/qreal(idv-3);
        }   //剛域端

        qreal TA=-TAI*UBAI+(TT0-TTR);
        qreal TB=-TBI*UBAI+(TT0-TTR);
        qreal moment_iface = -EI*((4.0*TA+2.0*TB)/XL1+6.0*(TA+TB)*xpos[1]/XL1/XL1);
        qreal moment_jface = -EI*((4.0*TA+2.0*TB)/XL1+6.0*(TA+TB)*xpos[idv-2]/XL1/XL1);
        if(qAbs(moment_jface-moment_iface)<EPS)return;
        qreal Mpi = qAbs(mm[1]);
        qreal ratio=(Mpi+qAbs(moment_iface))/qAbs(moment_jface-moment_iface);

        QVector<qreal > XA_kari(idv,0.0);
        QVector<qreal > YA_kari(idv,0.0);
        QVector<qreal > XA_kari2(idv,0.0);
        QVector<qreal > YA_kari2(idv,0.0);
        QVector<qreal > XA_kari3(idv,0.0);
        QVector<qreal > YA_kari3(idv,0.0);
        for(int i=0;i<=idv-1;i++){
        XA_kari3[i]=XAR+xpos2[i]*qCos(TB+TTR)/qCos(TB);
        YA_kari3[i]=YAR+xpos2[i]*qSin(TB+TTR)/qCos(TB);
        }

        calcBendingCurveFF(XA_kari, YA_kari, XAR, YAR,XA_kari3[idv-1]+ratio*(XBRmod-XA_kari3[idv-1]), YA_kari3[idv-1]+ratio*(YBRmod-YA_kari3[idv-1]), RL,RR, FL, FR, TAI, TBI, TT0, idv,UBAI);
        calcBendingCurvePF(XA_kari2, YA_kari2, XAR, YAR,XA_kari3[idv-1]+(1.0-ratio)*(XBRmod-XA_kari3[idv-1]), YA_kari3[idv-1]+(1.0-ratio)*(YBRmod-YA_kari3[idv-1]), RL,RR, FL, FR, TAI, TBI, TT0,XCC, XSS, idv,UBAI);

        for(int i=0;i<=idv-1;i++){
            XA_bend[i]=XA_kari[i]+XA_kari2[i]-XA_kari3[i];
            YA_bend[i]=YA_kari[i]+YA_kari2[i]-YA_kari3[i];
        }
    }
    //fix-hinge　　fix-fixとfix-pinの足し合わせ
    if(PINA==0 && PINB==2){
        qreal XBRmod=XBR+ qSin(TT0)*sendan_disp.last();
        qreal YBRmod=YBR- qCos(TT0)*sendan_disp.last();
        qreal XBA=XBRmod-XAR;
        qreal YBA=YBRmod-YAR;
        qreal XL2=XBA*XBA+YBA*YBA;
        qreal XL1=qSqrt(XL2);//弾性域長さ
        qreal TTR=atan2(YBA,XBA);

        QVector<qreal> xpos2(idv,0.0);
        xpos2[0]=0.0;
        xpos2[1]=FL-RL;
        xpos2[idv-2]=XL1-FR+RR;
        xpos2[idv-1]=XL1;
        for(int K=2;K<=idv-3;K++){
            xpos2[K]=xpos2[1]+(qreal)(K-1)*(xpos2[idv-2]-xpos2[1])/qreal(idv-3);
        }   //剛域端

        qreal TA=-TAI*UBAI+(TT0-TTR);
        qreal TB=-TBI*UBAI+(TT0-TTR);
        qreal moment_iface = -EI*((4.0*TA+2.0*TB)/XL1+6.0*(TA+TB)*xpos[1]/XL1/XL1);
        qreal moment_jface = -EI*((4.0*TA+2.0*TB)/XL1+6.0*(TA+TB)*xpos[idv-2]/XL1/XL1);
        if(qAbs(moment_jface-moment_iface)<EPS)return;
        qreal Mpj = qAbs(mm[idv-2]);
        qreal ratio=(Mpj+qAbs(moment_iface))/qAbs(moment_jface-moment_iface);

        QVector<qreal > XA_kari(idv,0.0);
        QVector<qreal > YA_kari(idv,0.0);
        QVector<qreal > XA_kari2(idv,0.0);
        QVector<qreal > YA_kari2(idv,0.0);
        QVector<qreal > XA_kari3(idv,0.0);
        QVector<qreal > YA_kari3(idv,0.0);
        for(int i=0;i<=idv-1;i++){
            XA_kari3[i]=XAR+xpos2[i]*qCos(TA+TTR)/qCos(TA);
            YA_kari3[i]=YAR+xpos2[i]*qSin(TA+TTR)/qCos(TA);
        }
        calcBendingCurveFF(XA_kari, YA_kari, XAR, YAR,XA_kari3[idv-1]+ratio*(XBRmod-XA_kari3[idv-1]), YA_kari3[idv-1]+ratio*(YBRmod-YA_kari3[idv-1]), RL,RR, FL, FR, TAI, TBI, TT0, idv,UBAI);
        calcBendingCurveFP(XA_kari2, YA_kari2, XAR, YAR,XA_kari3[idv-1]+(1.0-ratio)*(XBRmod-XA_kari3[idv-1]), YA_kari3[idv-1]+(1.0-ratio)*(YBRmod-YA_kari3[idv-1]), RL,RR, FL, FR, TAI, TBI, TT0,XCC, XSS, idv,UBAI);
        for(int i=0;i<=idv-1;i++){
            XA_bend[i]=XA_kari[i]+XA_kari2[i]-XA_kari3[i];
            YA_bend[i]=YA_kari[i]+YA_kari2[i]-YA_kari3[i];
        }
    }
    //hinge-pin  ->ピン―ピンと同じ表示
    if(PINA==2 && PINB==1){
    calcBendingCurvePP(XA_bend, YA_bend, XAR, YAR,XBR+ qSin(TT0)*sendan_disp.last(), YBR- qCos(TT0)*sendan_disp.last(), RL,RR, FL, FR, TAI, TBI, TT0,XCC, XSS, idv,UBAI);
    }
    //pin-hinge  ->ピン―ピンと同じ表示
    if(PINA==1 && PINB==2){
    calcBendingCurvePP(XA_bend, YA_bend, XAR, YAR,XBR+ qSin(TT0)*sendan_disp.last(), YBR- qCos(TT0)*sendan_disp.last(), RL,RR, FL, FR, TAI, TBI, TT0,XCC, XSS, idv,UBAI);
    }


    //せん断変形と中間荷重による曲げ変形を足して全体の変形とする
    for(int K=0;K<idv;K++){
        qreal UUU1=-qSin(TT0)*(sendan_disp[K]+mage_disp[K]);
        qreal UUU2=+qCos(TT0)*(sendan_disp[K]+mage_disp[K]);
        //bend+shear
        XA_center[K]= XA_bend[K]+UUU1;
        YA_center[K]= YA_bend[K]+UUU2;
    }

    qreal size = 0.5*D;///(xpos[2]-xpos[1]);

    for(int K=1;K<idv-1;K++){
        if(K==1){
            qreal angle1= atan2(YA_bend[K+1]-YA_bend[K],XA_bend[K+1]-XA_bend[K]);
            qreal angle2= atan2(YA_center[K+1]-YA_center[K],XA_center[K+1]-XA_center[K]);
            qreal cos=qCos(angle1);
            qreal sin=qSin(angle1);
            XA_u[K]=XA_center[K]-sin*size/qMax(0.3,qCos(angle1-angle2));
            YA_u[K]=YA_center[K]+cos*size/qMax(0.3,qCos(angle1-angle2));
            XA_b[K]=XA_center[K]+sin*size/qMax(0.3,qCos(angle1-angle2));
            YA_b[K]=YA_center[K]-cos*size/qMax(0.3,qCos(angle1-angle2));
        }else if(K==idv-2){
            qreal angle1= atan2(YA_bend[K]-YA_bend[K-1],XA_bend[K]-XA_bend[K-1]);
            qreal angle2= atan2(YA_center[K]-YA_center[K-1],XA_center[K]-XA_center[K-1]);
            qreal cos=qCos(angle1);
            qreal sin=qSin(angle1);
            XA_u[K]=XA_center[K]-sin*size/qMax(0.3,qCos(angle1-angle2));
            YA_u[K]=YA_center[K]+cos*size/qMax(0.3,qCos(angle1-angle2));
            XA_b[K]=XA_center[K]+sin*size/qMax(0.3,qCos(angle1-angle2));
            YA_b[K]=YA_center[K]-cos*size/qMax(0.3,qCos(angle1-angle2));
        }else{
            //面積同じにしたいので、角度の分だけ線を伸ばすが、上限はつける
            qreal angle1= atan2(YA_bend[K+1]-YA_bend[K-1],XA_bend[K+1]-XA_bend[K-1]);
            qreal angle2= atan2(YA_center[K+1]-YA_center[K-1],XA_center[K+1]-XA_center[K-1]);
            qreal cos=qCos(angle1);
            qreal sin=qSin(angle1);
            XA_u[K]=XA_center[K]-sin*size/qMax(0.3,qCos(angle1-angle2));
            YA_u[K]=YA_center[K]+cos*size/qMax(0.3,qCos(angle1-angle2));
            XA_b[K]=XA_center[K]+sin*size/qMax(0.3,qCos(angle1-angle2));
            YA_b[K]=YA_center[K]-cos*size/qMax(0.3,qCos(angle1-angle2));
        }
    }
    QPainterPath deformPath;
    QPointF ps = QPointF(XAI,YAI);
    QPointF pe = QPointF(XBI,YBI);
    deformPath.moveTo(ps);


    deformPath.moveTo(QPointF(XA_u[1],YA_u[1]));
    deformPath.lineTo(QPointF(XA_b[1],YA_b[1]));
    for (int i = 1; i < idv-2; i++) {
        deformPath.moveTo(QPointF(XA_u[i],YA_u[i]));
        deformPath.lineTo(QPointF(XA_u[i+1],YA_u[i+1]));
        deformPath.lineTo(QPointF(XA_b[i+1],YA_b[i+1]));
        deformPath.lineTo(QPointF(XA_b[i],YA_b[i]));
    }

    deformPath.moveTo(ps);
    for (int i = 0; i < idv; i++) {
        deformPath.lineTo(QPointF(XA_center[i],YA_center[i]));
    }
    deformPath.lineTo(pe);

    const QPen pen(QBrush(color), 0); //pen for svg
    OTpathes.append(PATH_AND_PEN(deformPath,pen));

    QPainterPath rigidPath;
    qreal t1=atan2(YA_center[0]-YAI,XA_center[0]-XAI);
    qreal t2=atan2(YA_center[idv-1]-YBI,XA_center[idv-1]-XBI);
    qreal wd=20.0;
    QPointF ps0=QPointF(XA_center[0],YA_center[0]);
    QPointF ps1=ps+QPointF(-wd*qSin(t1),wd*qCos(t1));
    QPointF ps2=ps-QPointF(-wd*qSin(t1),wd*qCos(t1));
    QPointF ps3=ps0+QPointF(-wd*qSin(t1),wd*qCos(t1));
    QPointF ps4=ps0-QPointF(-wd*qSin(t1),wd*qCos(t1));

    QPointF pe0=QPointF(XA_center[idv-1],YA_center[idv-1]);
    QPointF pe1=pe+QPointF(-wd*qSin(t2),wd*qCos(t2));
    QPointF pe2=pe-QPointF(-wd*qSin(t2),wd*qCos(t2));
    QPointF pe3=pe0+QPointF(-wd*qSin(t2),wd*qCos(t2));
    QPointF pe4=pe0-QPointF(-wd*qSin(t2),wd*qCos(t2));

    rigidPath.moveTo(ps1);
    rigidPath.lineTo(ps2);
    rigidPath.lineTo(ps4);
    rigidPath.lineTo(ps3);
    rigidPath.lineTo(ps1);
    rigidPath.moveTo(pe1);
    rigidPath.lineTo(pe2);
    rigidPath.lineTo(pe4);
    rigidPath.lineTo(pe3);
    rigidPath.lineTo(pe1);
    const QPen pen2(QBrush(Qt::black), 0); //pen for svg
    OTpathes.append(PATH_AND_PEN(rigidPath,pen2));

    if(PINA==1){
        qreal size = 50;
        QPainterPath pinPath;
        pinPath.addEllipse(QPointF(XA_center[1],YA_center[1]),size,size);
        const QPen penpin(QBrush(Qt::black), 0); //pen for svg
        OTpathes.append(PATH_AND_PEN(pinPath,penpin));
    }
    if(PINB==1){
        qreal size = 50;
        QPainterPath pinPath;
        pinPath.addEllipse(QPointF(XA_center[idv-2],YA_center[idv-2]),size,size);
        const QPen penpin(QBrush(Qt::black), 0); //pen for svg
        OTpathes.append(PATH_AND_PEN(pinPath,penpin));
    }
    if(PINA==2){
        qreal size = 50;
        QPainterPath pinPath;
        pinPath.addEllipse(QPointF(XA_center[1],YA_center[1]),size,size);
        const QPen penpin(QBrush(Qt::black), 0); //pen for svg
        OTpathes.append(PATH_AND_PEN(pinPath,penpin,QBrush(Qt::green)));
    }
    if(PINB==2){
        qreal size = 50;
        QPainterPath pinPath;
        pinPath.addEllipse(QPointF(XA_center[idv-2],YA_center[idv-2]),size,size);
        const QPen penpin(QBrush(Qt::black), 0); //pen for svg
        OTpathes.append(PATH_AND_PEN(pinPath,penpin,QBrush(Qt::green)));
    }

    boundRect = shapePath.boundingRect()|deformPath.boundingRect();
}
void OutputLineItemDeformation::calcBendingCurvePP(QVector<qreal> &XA_bend,QVector<qreal> &YA_bend,
                                                   qreal XAR,qreal YAR,qreal XBR,qreal YBR,
                                                   qreal RL,qreal RR,qreal FL,qreal FR,
                                                   qreal TAI,qreal TBI,qreal TT0, qreal XCC_origin, qreal XSS_origin,int idv,qreal UBAI)
{
    Q_UNUSED(TT0);
    QVector<qreal> xpos(idv,0.0);
    qreal SIA=qSin(-TAI*UBAI);
    qreal COA=qCos(-TAI*UBAI);
    qreal SIB=qSin(-TBI*UBAI);
    qreal COB=qCos(-TBI*UBAI);

    qreal XBA=XBR-XAR;
    qreal YBA=YBR-YAR;
    qreal XL2=XBA*XBA+YBA*YBA;
    qreal XL1=qSqrt(XL2);//弾性域長さ

    xpos[0]=0.0;
    xpos[1]=FL-RL;
    xpos[idv-2]=XL1-FR+RR;
    xpos[idv-1]=XL1;
    for(int K=2;K<=idv-3;K++){
        xpos[K]=xpos[1]+(qreal)(K-1)*(xpos[idv-2]-xpos[1])/qreal(idv-3);
    }   //剛域端
    XA_bend[0]=XAR;
    YA_bend[0]=YAR;
    //フェイス端。剛域を角度なりに延長
    XA_bend[1]=XAR+(FL-RL)*(XCC_origin*COA-XSS_origin*SIA);
    YA_bend[1]=YAR+(FL-RL)*(XSS_origin*COA+XCC_origin*SIA);
    //剛域端
    XA_bend[idv-1]=XBR;
    YA_bend[idv-1]=YBR;
    //フェイス端。剛域を角度なりに延長
    XA_bend[idv-2]=XBR-(FR-RR)*(XCC_origin*COB-XSS_origin*SIB);
    YA_bend[idv-2]=YBR-(FR-RR)*(XSS_origin*COB+XCC_origin*SIB);
    for(int K=2;K<idv-2;K++){
        qreal t = (xpos[K]-xpos[1])/(xpos[idv-2]-xpos[1]);
        XA_bend[K]=t*XA_bend[idv-2]+(1.0-t)*XA_bend[1];
        YA_bend[K]=t*YA_bend[idv-2]+(1.0-t)*YA_bend[1];
    }
}
void OutputLineItemDeformation::calcBendingCurveFP(QVector<qreal> &XA_bend, QVector<qreal> &YA_bend,
                                                   qreal XAR, qreal YAR, qreal XBR, qreal YBR,
                                                   qreal RL, qreal RR, qreal FL, qreal FR,
                                                   qreal TAI, qreal TBI, qreal TT0, qreal XCC_origin, qreal XSS_origin, int idv, qreal UBAI)
{
    QVector<qreal> xpos(idv,0.0);
    qreal SIB=qSin(-TBI*UBAI);
    qreal COB=qCos(-TBI*UBAI);

    qreal XBA=XBR-XAR;
    qreal YBA=YBR-YAR;
    qreal XL2=XBA*XBA+YBA*YBA;
    qreal XL1=qSqrt(XL2);//弾性域長さ
    qreal TTR=atan2(YBA,XBA);

    qreal TA=-TAI*UBAI+(TT0-TTR);
    xpos[0]=0.0;
    xpos[1]=FL-RL;
    xpos[idv-2]=XL1-FR+RR;
    xpos[idv-1]=XL1;
    for(int K=2;K<=idv-3;K++){
        xpos[K]=xpos[1]+(qreal)(K-1)*(xpos[idv-2]-xpos[1])/qreal(idv-3);
    }
    qreal delta =xpos[idv-2]*qSin(TA);
    qreal LL=xpos[idv-2]*qCos(TA);
    for(int K=0;K<idv;K++){
        qreal xx = xpos[K]*qCos(TA);
        qreal yy = -3.0*delta/LL/LL/LL*(xx*xx*xx/6.0-LL*xx*xx/2.0);
        qreal XX1 = xx*qCos(-TA)-yy*qSin(-TA);
        qreal YY1 = -1.0*(xx*qSin(-TA)+yy*qCos(-TA));
        qreal XX2 = XX1*qCos(TTR)-YY1*qSin(TTR);
        qreal YY2 = XX1*qSin(TTR)+YY1*qCos(TTR);
        XA_bend[K]=XAR+XX2;
        YA_bend[K]=YAR+YY2;
    }
    //剛域端
    XA_bend[idv-1]=XBR;
    YA_bend[idv-1]=YBR;
    //フェイス端。剛域を角度なりに延長
    XA_bend[idv-2]=XBR-(FR-RR)*(XCC_origin*COB-XSS_origin*SIB);
    YA_bend[idv-2]=YBR-(FR-RR)*(XSS_origin*COB+XCC_origin*SIB);
}
void OutputLineItemDeformation::calcBendingCurvePF(QVector<qreal> &XA_bend,QVector<qreal> &YA_bend,
                                                   qreal XAR,qreal YAR,qreal XBR,qreal YBR,
                                                   qreal RL,qreal RR,qreal FL,qreal FR,
                                                   qreal TAI,qreal TBI,qreal TT0,qreal XCC_origin,qreal XSS_origin,int idv,qreal UBAI)
{
    QVector<qreal> xpos(idv,0.0);
    qreal SIA=qSin(-TAI*UBAI);
    qreal COA=qCos(-TAI*UBAI);

    qreal XBA=XBR-XAR;
    qreal YBA=YBR-YAR;
    qreal XL2=XBA*XBA+YBA*YBA;
    qreal XL1=qSqrt(XL2);//弾性域長さ
    qreal TTR=atan2(YBA,XBA);

    qreal TB=-TBI*UBAI+(TT0-TTR);
    xpos[0]=0.0;
    xpos[1]=FL-RL;
    xpos[idv-2]=XL1-FR+RR;
    xpos[idv-1]=XL1;
    for(int K=2;K<=idv-3;K++){
        xpos[K]=xpos[1]+(qreal)(K-1)*(xpos[idv-2]-xpos[1])/qreal(idv-3);
    }
    qreal delta =(xpos[idv-1]-xpos[1])*qSin(TB);
    qreal LL=(xpos[idv-1]-xpos[1])*qCos(TB);
    for(int K=0;K<idv;K++){
        qreal xx = (xpos[idv-1]-xpos[idv-1-K])*qCos(TB);
        qreal yy = -3.0*delta/LL/LL/LL*(xx*xx*xx/6.0-LL*xx*xx/2.0);
        qreal XX1 = xx*qCos(-TB)-yy*qSin(-TB);
        qreal YY1 = -1.0*(xx*qSin(-TB)+yy*qCos(-TB));
        qreal XX2 = XX1*qCos(TTR)-YY1*qSin(TTR);
        qreal YY2 = XX1*qSin(TTR)+YY1*qCos(TTR);
        XA_bend[idv-1-K]=XBR-XX2;
        YA_bend[idv-1-K]=YBR-YY2;
    }
    //剛域端
    XA_bend[0]=XAR;
    YA_bend[0]=YAR;
    //フェイス端。剛域を角度なりに延長
    XA_bend[1]=XAR+(FL-RL)*(XCC_origin*COA-XSS_origin*SIA);
    YA_bend[1]=YAR+(FL-RL)*(XSS_origin*COA+XCC_origin*SIA);
}
void OutputLineItemDeformation::calcBendingCurveFF(QVector<qreal> &XA_bend,QVector<qreal> &YA_bend,
                                                   qreal XAR,qreal YAR,qreal XBR,qreal YBR,
                                                   qreal RL,qreal RR,qreal FL,qreal FR,
                                                   qreal TAI,qreal TBI,qreal TT0,int idv,qreal UBAI)
{
    QVector<qreal> xpos(idv,0.0);

    qreal XBA=XBR-XAR;
    qreal YBA=YBR-YAR;
    qreal XL2=XBA*XBA+YBA*YBA;
    qreal XL1=qSqrt(XL2);//弾性域長さ
    qreal XCC=XBA/XL1;
    qreal XSS=YBA/XL1;
    qreal TTR=atan2(YBA,XBA);

    qreal TA=-TAI*UBAI+(TT0-TTR);
    qreal TB=-TBI*UBAI+(TT0-TTR);
    xpos[0]=0.0;
    xpos[1]=FL-RL;
    xpos[idv-2]=XL1-FR+RR;
    xpos[idv-1]=XL1;
    for(int K=2;K<=idv-3;K++){
        xpos[K]=xpos[1]+(qreal)(K-1)*(xpos[idv-2]-xpos[1])/qreal(idv-3);
    }
    for(int K=0;K<idv;K++){
        qreal XXL =xpos[K];
        qreal UL1    = XXL;
        qreal UL2    = TA*XXL -((2.0*TA+TB)*XXL*XXL)/XL1+((TA+TB)*XXL*XXL*XXL)/(XL1*XL1);
        qreal UU1    = XCC*UL1-XSS*UL2;
        qreal UU2    = XSS*UL1+XCC*UL2;
        //bend
        XA_bend[K]=XAR+UU1 ;
        YA_bend[K]=YAR+UU2 ;
    }
}
void OutputLineItemDeformation::createDeformationShapePathWithShear()
{
    //参考：pdrimm.f
    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    QList<STRESS> st = myFigureTemplate->getFigureTypeProperty().stress.first();

    XYZ idisp = UnifiedFixedData::getInstance()->getPstnData()->getCombinationJointDisp(md->iJoint()->getUuid(),st);
    XYZ jdisp = UnifiedFixedData::getInstance()->getPstnData()->getCombinationJointDisp(md->jJoint()->getUuid(),st);
    VEC3D idispVec = VEC3D(idisp.x,idisp.y,idisp.z);
    VEC3D jdispVec = VEC3D(jdisp.x,jdisp.y,jdisp.z);
    VEC3D xVec = myPlaneXVector;
    VEC3D yVec = CalcVector3D::crossNormalized(myPlaneVector,myPlaneXVector);
    qreal UBAI = static_cast<DeformationElevationTemplate*>(myFigureTemplate)->deformScale();
    QPointF disp_i =1000.0*QPointF(CalcVector3D::dot(xVec,idispVec),CalcVector3D::dot(yVec,idispVec));
    QPointF disp_j =1000.0*QPointF(CalcVector3D::dot(xVec,jdispVec),CalcVector3D::dot(yVec,jdispVec));

    qreal XLL=0.0;//XYJ(1,1);//i-x　紙面上の座標（i端を0,0に座標変換済）
    qreal YLL=0.0;//XYJ(2,1);//i-y
    qreal XRR=myPointList.last().x();//XYJ(1,2);//j-x
    qreal YRR=myPointList.last().y();//XYJ(2,2);//j-y
    QVector<qreal> TF(6,0.0);
    TF[0]=disp_i.x();//XYJU(1,1)-XYJ(1,1)//紙面上の変位　i-x
    TF[1]=disp_j.x();//XYJU(1,2)-XYJ(1,2)
    TF[2]=disp_i.y();//XYJU(2,1)-XYJ(2,1)
    TF[3]=disp_j.y();//XYJU(2,2)-XYJ(2,2)
    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    int numj_i=UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(md->iJoint()->getUuid());
    int numj_j=UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(md->jJoint()->getUuid());
    VEC3D theta_i=VEC3D();
    VEC3D theta_j=VEC3D();
    VEC3D panel_i=VEC3D();
    VEC3D panel_j=VEC3D();
    int IPL=0,IPR=0;
    int IHL=0,IHR=0;


    for(int i=0;i<st.count();i++){
        DataNBLD3D_RSTStep step=UnifiedFixedData::getInstance()->getPstnData()->getRSTStep(st.at(i).rst,st.at(i).getStep());
        if(step.ujnt.count() <= numj_i)continue;
        if(step.ujnt.count() <= numj_j)continue;
        theta_i.x += step.ujnt.at(numj_i).at(3);
        theta_i.y += step.ujnt.at(numj_i).at(4);
        theta_i.z += step.ujnt.at(numj_i).at(5);
        panel_i.x += step.ujnt.at(numj_i).at(6);
        panel_i.y += step.ujnt.at(numj_i).at(7);
        panel_i.z += step.ujnt.at(numj_i).at(8);

        theta_j.x += step.ujnt.at(numj_j).at(3);
        theta_j.y += step.ujnt.at(numj_j).at(4);
        theta_j.z += step.ujnt.at(numj_j).at(5);
        panel_j.x += step.ujnt.at(numj_j).at(6);
        panel_j.y += step.ujnt.at(numj_j).at(7);
        panel_j.z += step.ujnt.at(numj_j).at(8);
        if(step.ipspmm[numm][0]<0)IHL=2;//塑性ヒンジ
        if(step.ipspmm[numm][1]<0)IHR=2;
    }
//    panel_i.x = 0;
//    panel_i.y = 0;
//    panel_i.z = 0;
//    panel_j.x = 0;
//    panel_j.y = 0;
//    panel_j.z = 0;

    int IPY1=model.ifpmm.at(numm).at(0);
    int IPZ1=model.ifpmm.at(numm).at(1);
    int IPY2=model.ifpmm.at(numm).at(2);
    int IPZ2=model.ifpmm.at(numm).at(3);
    qreal VIY= CalcVector3D::dot(myPlaneVector,md->getVVector());
    qreal VIZ= CalcVector3D::dot(myPlaneVector,md->getWVector());
    qreal RRY1= CalcVector3D::dot(md->getVVector(),theta_i);
    qreal RRZ1= CalcVector3D::dot(md->getWVector(),theta_i);
    qreal PNY1= CalcVector3D::dot(md->getVVector(),panel_i);
    qreal PNZ1= CalcVector3D::dot(md->getWVector(),panel_i);
    qreal RRY2= CalcVector3D::dot(md->getVVector(),theta_j);
    qreal RRZ2= CalcVector3D::dot(md->getWVector(),theta_j);
    qreal PNY2= CalcVector3D::dot(md->getVVector(),panel_j);
    qreal PNZ2= CalcVector3D::dot(md->getWVector(),panel_j);
    QString modelName = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstData().value(st.first().rst).nameam;
    QHash<QString, DataNBLD3D_PREA> preaRaw=UnifiedFixedData::getInstance()->getPstnData()->getPreaData();
    DataNBLD3D_PREA prea = preaRaw.value(modelName);
    qreal GA,EI,D;
    SectionValues sec = UnifiedFixedData::getInstance()->getAnalysisData()->getSectionValues(md->dataType(),md->sectionName(),md->iXYZ().z+0.1);

    if(qAbs(VIY) > qAbs(VIZ)) {
        if(VIY>0.0) {
            TF[4]=-(RRY1+0.5*PNY1);
            TF[5]=-(RRY2+0.5*PNY2);
            GA = prea.debmm.at(numm).at(14);
            EI = prea.debmm.at(numm).at(17);
            if(sec.sectionType == STRUCTTYPE::STP_RC ||sec.sectionType == STRUCTTYPE::STP_SRC ){
                D = sec.i_param.Dz;
            }else{
                D = sec.i_param.sizeH;
            }
        }else{
            TF[4]=(RRY1+0.5*PNY1);
            TF[5]=(RRY2+0.5*PNY2);
            GA = prea.debmm.at(numm).at(14);
            EI = prea.debmm.at(numm).at(17);
            if(sec.sectionType == STRUCTTYPE::STP_RC ||sec.sectionType == STRUCTTYPE::STP_SRC ){
                D = sec.i_param.Dz;
            }else{
                D = sec.i_param.sizeH;
            }
        }
        IPL=IPY1;
        IPR=IPY2;
    }else{
        if(VIZ > 0.0){
            TF[4]=-(RRZ1+0.5*PNZ1);
            TF[5]=-(RRZ2+0.5*PNZ2);
            GA = prea.debmm.at(numm).at(11);
            EI = prea.debmm.at(numm).at(20);
            if(sec.sectionType == STRUCTTYPE::STP_RC ||sec.sectionType == STRUCTTYPE::STP_SRC ){
                D = sec.i_param.Dy;
            }else{
                D = sec.i_param.sizeB;
            }
        }else{
            TF[4]=(RRZ1+0.5*PNZ1);
            TF[5]=(RRZ2+0.5*PNZ2);
            GA = prea.debmm.at(numm).at(11);
            EI = prea.debmm.at(numm).at(20);
            if(sec.sectionType == STRUCTTYPE::STP_RC ||sec.sectionType == STRUCTTYPE::STP_SRC ){
                D = sec.i_param.Dy;
            }else{
                D = sec.i_param.sizeB;
            }
        }
        IPL=IPZ1;
        IPR=IPZ2;
    }
    IPL=qMax(IPL,IHL);
    IPR=qMax(IPR,IHR);

    QVector<qreal> len_f_r=getOutputFaceRigidLength();
    qreal FL = len_f_r.at(0);
    qreal FR = len_f_r.at(1);
    qreal RL = len_f_r.at(2);
    qreal RR = len_f_r.at(3);
//    FL=0.0;
//    FR=0.0;
//    RL=0.0;
//    RR=0.0;
    qreal DL= md->getTotalLength();//部材長さ
    qreal RAL1=RL/DL;//剛域長の比（図面の角度に依存しない）
    qreal RAL2=RR/DL;
    qreal FAL1=FL/DL;//フェイス長の比（図面の角度に依存しない）
    qreal FAL2=FR/DL;
//    RAL1=0.01;
//    RAL2=0.01;
//    FAL1=0.1;
//    FAL2=0.1;

    OTpathes.clear();


    qreal dansei_len=DL-RL-RR;
    int idv = (int)(dansei_len+0.25*D)/(0.5*D) + 1;
    //    idv = qMax(idv,4);
    idv = qMin(20,qMax(idv,6));
    QVector<qreal> xpos(idv,0.0);
    QVector<qreal> qq(idv,0.0);
    QVector<qreal> mm(idv,0.0);
    if(myDataType==DATATYPE::TPGIRDER && (XRR-XLL)<=0.0){
        xpos[0]=0.0;
        xpos[1]=(FR-RR);
        xpos[idv-2]=DL-FL+RL;
        xpos[idv-1]=DL;
    }else{
        xpos[0]=0.0;
        xpos[1]=(FL-RL);
        xpos[idv-2]=DL-FR+RR;
        xpos[idv-1]=DL;
    }
    for(int K=2;K<=idv-3;K++){
        xpos[K]=xpos[1]+(qreal)(K-1)*(xpos[idv-2]-xpos[1])/qreal(idv-3);
    }
    for(int K=0;K<idv;K++){
        qreal x = xpos[K];
        if(myDataType==DATATYPE::TPGIRDER && (XRR-XLL)<=0.0)x=xpos[idv-1-K];
        MemberForce mf = UnifiedFixedData::getInstance()->getPstnData()->getCombinationNodeMemberForce(myUuid, st);
        MemberForceByIL mfil = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(myUuid,st,0,x);
        if(qAbs(VIY) > qAbs(VIZ)) {
            if(VIY>0.0) {
                qq[K] = -(mf.getQz(x/DL) + mfil.getQz());
                mm[K] = -(mf.getMy(x/DL) + mfil.getMy());
            }else{
                qq[K] = mf.getQz(x/DL) + mfil.getQz();
                mm[K] = mf.getMy(x/DL) + mfil.getMy();
            }
        }else{
            if(VIY>0.0) {
                qq[K] = -(mf.getQy(x/DL) + mfil.getQy());
                mm[K] = -(mf.getMz(x/DL) + mfil.getMz());
            }else{
                qq[K] = mf.getQy(x/DL) + mfil.getQy();
                mm[K] = mf.getMz(x/DL) + mfil.getMz();
            }

        }
    }
qDebug()<<"deform RRLL"<<XRR<<YRR<<XLL<<YLL;
qDebug()<<"deform TF"<<TF;
qDebug()<<"deform RAL"<<RAL1<<RAL2<<FAL1<<FAL2;
qDebug()<<"deform hoka"<<IPR<<IPL<<EI<<GA<<D<<xpos<<qq<<mm<<idv<<UBAI;
    if(myDataType==DATATYPE::TPGIRDER && (XRR-XLL)<=0.0){
        figplt(XRR,YRR,XLL,YLL,TF.at(1),TF.at(0),TF.at(3),TF.at(2),TF.at(5),TF.at(4),RAL2,RAL1,FAL2,FAL1,IPR,IPL,EI,GA,D,xpos,qq,mm,idv,UBAI);
    }else{
        figplt(XLL,YLL,XRR,YRR,TF.at(0),TF.at(1),TF.at(2),TF.at(3),TF.at(4),TF.at(5),RAL1,RAL2,FAL1,FAL2,IPL,IPR,EI,GA,D,xpos,qq,mm,idv,UBAI);
    }
}

void OutputLineItemDeformation::setDrawingStatusDirect()
{
    createDeformationShapePathWithShear();

}

} // namespace post3dapp
