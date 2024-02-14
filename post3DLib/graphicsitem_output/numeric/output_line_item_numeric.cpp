#include "output_line_item_numeric.h"

#include "calc_vector2d.h"
#include "check_elevation_template.h"
#include "check_plan_template.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"
namespace post3dapp{

OutputLineItemNumeric::OutputLineItemNumeric(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid,
                                             const QList<QPointF> &plist, bool zin, bool isaroundplane, bool shape_on)
    : OutputLineItem(fig, dtype,stype, uid,plist,shape_on),isZin(zin)
{
//    checkVal = 0.0;
    isAroundPlane = isaroundplane;
    MemberData *member = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(uid);
    QList<JointData *> jlist = member->getJointList();

    iRelatedMember = jlist.first()->getRelatedMembers();
    jRelatedMember = jlist.last()->getRelatedMembers();


    switch(myFigureTemplate->getOutFigType()){
    case OUTFIGTYPE::LENGTH:
    case OUTFIGTYPE::RIGIDITYPHI:
    case OUTFIGTYPE::EULERBUCKLEN:
    case OUTFIGTYPE::LATERALBUCKLEN:
    case OUTFIGTYPE::TORSION:
    case OUTFIGTYPE::OUTERFORCE:
    case OUTFIGTYPE::OUTERMOMENT:
        layout1();
        break;
    case OUTFIGTYPE::ALLOW://59
    case OUTFIGTYPE::CHECKRATIO://58
    case OUTFIGTYPE::ALLOWRATIOE:
    case OUTFIGTYPE::ALLOWRATIOU:
    case OUTFIGTYPE::STRENGTH:
    case OUTFIGTYPE::YIELDSTEP:
    case OUTFIGTYPE::YIELDLOADMAG:
    case OUTFIGTYPE::PLASTICRATIO:
        layout2();
        break;
    default:
        break;
    }

}

//QColor OutputLineItemNumeric::textColor(){
//    switch(myFigureTemplate->getOutFigType()){
//    case OUTFIGTYPE::CHECKRATIO://58
//    case OUTFIGTYPE::ALLOWRATIOE:
//    case OUTFIGTYPE::ALLOWRATIOU:
//        return checkColor();
//        break;
//    default:
//        return myFigureTemplate->valueTextColor(dataType(),ATTENTION_STATUS::STATUS_NORMAL);
//        break;
//    }

//}
QColor OutputLineItemNumeric::checkColor(qreal checkVal){
bool grad = static_cast<CheckElevationTemplate*>(myFigureTemplate)->useGradation();
QColor ok = static_cast<CheckElevationTemplate*>(myFigureTemplate)->okColor();
QColor ng = static_cast<CheckElevationTemplate*>(myFigureTemplate)->ngColor();
if(!grad){
     if(checkVal < 1.0) return ok;
     return ng;
}else{
    if(checkVal < 0.0)return ok;
    if(checkVal >= 1.0) return ng;
    return QColor((1.0-checkVal)*ok.red()+checkVal*ng.red(),
           (1.0-checkVal)*ok.green()+checkVal*ng.green(),
           (1.0-checkVal)*ok.blue()+checkVal*ng.blue());
}

}

void OutputLineItemNumeric::layout1()
{
    QString textCenterTop= QString();
    QString textCenterBottom= QString();
    QString textLeftTop= QString();
    QString textRightBottom= QString();
    QString textLeftBottom= QString();
    QString textRightTop= QString();
    QString textCenterTop2= QString();
    QString textCenterBottom2= QString();
    QString textLeftTop2= QString();
    QString textRightTop2= QString();
    //横座屈用
    QVector<qreal> ll(12,0.0);//lyi+ lyi- lyc+ lyc- lyj+ lyj- lzi+ lzi- lzc+ lzc- lzj+ lzj-
    //ねじれM用
    MemberForce mf;


    //    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    //    DataNBLD3D_CHCK chck = UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();


    //    isZin = zin;//面外-planと面内-elevのときtrueで、部材y軸回りの値を用いる。ただしβ＞45度のときは逆。

    MemberData *member = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    memberText = QString::number(member->getTotalLength(), 'f', 0);
    DataNBLD3D_CMDL cmdl= UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);

    switch(myFigureTemplate->getOutFigType()){
    case OUTFIGTYPE::LENGTH:
        textCenterTop = "";
        textCenterBottom = QString::number(member->getTotalLength(), 'f', 0);
        break;
    case OUTFIGTYPE::RIGIDITYPHI:
        qreal phai;qreal EI;
        if(isZin){
            QString modelname = myFigureTemplate->getFigureTypeProperty().model;
            DataNBLD3D_PREA prea = UnifiedFixedData::getInstance()->getPstnData()->getPreaData().value(modelname);
            phai = prea.dmm2.at(numm).at(1);
            EI = prea.dmm2.at(numm).at(14)/member->getTotalLength()*0.1;
        }else{
            QString modelname = myFigureTemplate->getFigureTypeProperty().model;
            DataNBLD3D_PREA prea = UnifiedFixedData::getInstance()->getPstnData()->getPreaData().value(modelname);
            phai = prea.dmm2.at(numm).at(2);
            EI = prea.dmm2.at(numm).at(15)/member->getTotalLength()*0.0001;
        }
        textCenterTop = QString::number(EI , 'f', 2);
        textCenterBottom = QString::number(phai, 'f', 2);
        break;
    case OUTFIGTYPE::EULERBUCKLEN:
        qreal eulen;
        if(isZin){
            eulen = UnifiedFixedData::getInstance()->getPstnData()->getPrem(member->getUuid())->Lky;
        }else{
            eulen = UnifiedFixedData::getInstance()->getPstnData()->getPrem(member->getUuid())->Lkz;
        }
        textCenterTop = QString::number(eulen*1000.0 , 'f', 0);
        break;
    case OUTFIGTYPE::LATERALBUCKLEN:
        for(int ind=0;ind<12;ind++){ll[ind]=cmdl.dmm1.at(numm).at(ind+8)*1000.0;}
        //lyi+ lyi- lyc+ lyc- lyj+ lyj- lzi+ lzi- lzc+ lzc- lzj+ lzj-
        if(!isZin){
            textLeftTop= QString::number(ll.at(1), 'f', 0);
            textLeftBottom= QString::number(ll.at(0), 'f', 0);
            if(member->dataType()!=DATATYPE::TPCOLUMN){
                textCenterTop2= QString::number(ll.at(3), 'f', 0);
                textCenterBottom2= QString::number(ll.at(2), 'f', 0);
            }
            textRightTop= QString::number(ll.at(5), 'f', 0);
            textRightBottom= QString::number(ll.at(4), 'f', 0);
        }else{
            textLeftTop= QString::number(ll.at(1+6), 'f', 0);
            textLeftBottom= QString::number(ll.at(0+6), 'f', 0);
            if(member->dataType()!=DATATYPE::TPCOLUMN){
                textCenterTop2= QString::number(ll.at(3+6), 'f', 0);
                textCenterBottom2= QString::number(ll.at(2+6), 'f', 0);
            }
            textRightTop= QString::number(ll.at(5+6), 'f', 0);
            textRightBottom= QString::number(ll.at(4+6), 'f', 0);
        }
        break;
    case OUTFIGTYPE::TORSION:
        mf = UnifiedFixedData::getInstance()->getPstnData()->getCombinationNodeMemberForce(myUuid, myFigureTemplate->getFigureTypeProperty().stress.at(0));
        textCenterTop = QString::number(mf.getMx(0.5) , 'f', 2);
        break;
    case OUTFIGTYPE::OUTERFORCE://32
        break;
    case OUTFIGTYPE::OUTERMOMENT://33
        break;

    default:
        break;
    }


    // 符号描画の諸元（パスに含まず）
    qreal eps = 1.0e-6;
    qreal len_mid = ( myLenList.first() + myLenList.last() ) / 2.0;

    int idx_mid = 0;
    for (int i = 0; i < myLenList.count() - 1; i++) {
        if ( myLenList.at(i) - eps < len_mid && len_mid < myLenList.at(i + 1) ) {
            idx_mid = i;
            break;
        }
    }
    textAngle = qAtan2(myPointList.at(idx_mid + 1).y() - myPointList.at(idx_mid).y(),
                       myPointList.at(idx_mid + 1).x() - myPointList.at(idx_mid).x()) * 180.0 / M_PI;
    qreal rr = (len_mid - myLenList.at(idx_mid)) / (myLenList.at(idx_mid + 1) - myLenList.at(idx_mid));
    memberTextPos = CalcVector2D::interiorPoint(myPointList.at(idx_mid), myPointList.at(idx_mid + 1),
                                                rr);

    if ( textAngle >= 92.0 || textAngle <= -88.0 ) textAngle += 180.0;
    textAngle = textAngle * M_PI / 180.0;
    QColor color = stringColor(ATTENTION_STATUS::STATUS_NORMAL);

    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textCenterTop, color, OFFSETPOINT::OFFSET_CENTERTOP, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textCenterBottom, color, OFFSETPOINT::OFFSET_CENTERBOTTOM, 999999,valueTextSize) );

    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textCenterTop2, color, OFFSETPOINT::OFFSET_CENTERTOP2, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textCenterBottom2, color, OFFSETPOINT::OFFSET_CENTERBOTTOM2, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textLeftTop, color, OFFSETPOINT::OFFSET_LEFTTOP, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textLeftBottom, color, OFFSETPOINT::OFFSET_LEFTBOTTOM, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textRightTop, color, OFFSETPOINT::OFFSET_RIGHTTOP, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textRightBottom, color, OFFSETPOINT::OFFSET_RIGHTBOTTOM, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textLeftTop2, color, OFFSETPOINT::OFFSET_LEFTTOP2, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(memberTextPos.x(), -memberTextPos.y()), textAngle,
                                       textRightTop2, color, OFFSETPOINT::OFFSET_RIGHTTOP2, 999999,valueTextSize) );

}
void OutputLineItemNumeric::layout2()
{
    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    DataNBLD3D_CHCK chck;
    DataNBLD3D_RSTQD qd;
    int ichk = 0;
    QString name = myFigureTemplate->getFigureTypeProperty().caseName;
    //    isZin = zin;//面外-planと面内-elevのときtrueで、部材y軸回りの値を用いる。ただしβ＞45度のときは逆。

    MemberData *member = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
    memberText = QString::number(member->getTotalLength(), 'f', 0);
    DataNBLD3D_CMDL cmdl= UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    bool iPin = false;
    if(model.ifpmm[numm][0]==1 && model.ifpmm[numm][1]==1 && model.ifpmm[numm][2]==1
            && model.ifpmm[numm][3]==1 && model.ifpmm[numm][4]==1)iPin = true;

    //    bool iCanti=false;
    //    bool jCanti=false;
    qreal eps = 0.00001;
    bool plotYieldMark = false;
    QVector<int> yieldstate(5,0);

//    QColor textColorNormal=myFigureTemplate->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_NORMAL);
//    QColor textColorFatal=myFigureTemplate->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_FATAL);
    int stringSize=valueTextSize;
    int numstr=7;
    QVector<QColor> textColor(numstr,myFigureTemplate->valueTextColor(myDataType,ATTENTION_STATUS::STATUS_NORMAL));
    QVector<qreal> value(numstr,0.0);
    QVector<QString> valuestr(numstr,QString());
    int is4,is5,is6;


    switch(myFigureTemplate->getOutFigType()){
    case OUTFIGTYPE::ALLOW://59
        chck= UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();
        for(int i=0;i<chck.namech.count();i++){
            qDebug()<<"chck_caseName="<<name<<chck.namech.at(i).trimmed();
            if(name == chck.namech.at(i).trimmed()){
                ichk = i;break;
            }
        }

        //数値の配置は応力図に近い。壁の周囲の梁で軸組みの場合は書かない。片持ちの先端も書かない。ピンの時、RSTMMCの値で何かしている？
        if(iPin){
            if(chck.rstmmc[ichk][numm][2][0] > 0.0){
                value[2] = -chck.rstmma[ichk][numm][2][0];//NA+
            }else{
                value[2] = -chck.rstmma[ichk][numm][2][1];//NA-
            }
            break;
        }

        value[6]= chck.rstmma[ichk][numm][0][31];//Na
        if(isZin){
            value[0]=-chck.rstmma[ichk][numm][0][4];//MAy
            value[1]=chck.rstmma[ichk][numm][2][4];//MAy
            value[3]=-chck.rstmma[ichk][numm][0][16];//QAz
            value[4]=-chck.rstmma[ichk][numm][2][16];//QAz
            value[5]=-chck.rstmma[ichk][numm][1][16];//QAz
            if(myDataType==DATATYPE::TPGIRDER)value[2] = -chck.rstmma[ichk][numm][1][4];
        }else{
            value[0]=chck.rstmma[ichk][numm][0][5];//MAz
            value[1]=-chck.rstmma[ichk][numm][2][5];//MAz
            value[3]=-chck.rstmma[ichk][numm][0][15];//QAy
            value[4]=-chck.rstmma[ichk][numm][2][15];//QAy
            value[5]=-chck.rstmma[ichk][numm][1][15];//QAy
            if(myDataType==DATATYPE::TPGIRDER)value[2]= -chck.rstmma[ichk][numm][1][5];
        }
        for(int i=0;i<numstr;i++){
            if(qAbs(value[i])<eps){
                valuestr[i]=QString();
            }else{
                if(myDataType==DATATYPE::TPCOLUMN){
                    valuestr[i]=QString::number(-value[i],'f',0);
                }else{
                    valuestr[i]=QString::number(value[i],'f',0);
                }
            }
        }
        break;
    case OUTFIGTYPE::CHECKRATIO://58
        //n3dの画面における変動分検定比図の値。検定比図別途必要？図面の名前は余裕率図になってる
        chck= UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();
        for(int i=0;i<chck.namech.count();i++){
            qDebug()<<"chck_caseName="<<name<<chck.namech.at(i).trimmed();
            if(name == chck.namech.at(i).trimmed()){
                ichk = i;break;
            }
        }

        if(iPin){
            value[0] = chck.rstmma[ichk][numm][0][32];//αNa
            break;
        }
        value[0] = chck.rstmma[ichk][numm][0][11];//MD/MA（N変動）：余裕率
        value[1] = chck.rstmma[ichk][numm][2][11];//MD/MA（N変動）：余裕率
        value[2] = 0.0;
        if(chck.rstmma[ichk][numm][1][11] > eps){
            value[2] = chck.rstmma[ichk][numm][1][11];//中央
        }
        if(myDataType==DATATYPE::TPCOLUMN)value[2]=0.0;
        value[3] = chck.rstmma[ichk][numm][0][21];//QD/QA
        value[4] = chck.rstmma[ichk][numm][2][21];//QD/QA
        value[5] = chck.rstmma[ichk][numm][1][21];
        value[6] = chck.rstmma[ichk][numm][2][32];//αNa
        if(model.njmem[model.indm[numm][0]]==1 && qAbs(value[0])<eps){
            value[0]=0.0;
            value[3]=0.0;
        }
        if(model.njmem[model.indm[numm][1]]==1 && qAbs(value[1])<eps){
            value[1]=0.0;
            value[4]=0.0;
        }
        for(int i=0;i<numstr;i++){
            if(qAbs(value[i])<eps){
                valuestr[i]=QString();
            }else{
                valuestr[i]=QString::number(value[i],'f',2);
            }
//            if(value[i]>=1.0){
                textColor[i]=checkColor(value[i]);
//            }
        }

        break;
    case OUTFIGTYPE::ALLOWRATIOE://63
        plotYieldMark=true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        if(iPin){
            value[0] = qd.ultmm[numm][80];
            if(plotYieldMark){
                if(qd.ultmm[numm][30]>0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=1;//軸引張降伏
                if(qd.ultmm[numm][30]<=0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=2;//軸圧縮降伏
            }
            break;
        }
        value[0]=qd.ultmm[numm][74];
        value[1]=qd.ultmm[numm][75];
        value[2]=0.0;
        value[3]=qd.ultmm[numm][76];
        value[4]=qd.ultmm[numm][78];
        value[5]=qd.ultmm[numm][80];

        if(plotYieldMark){
            if(qd.istpmm[numm][3]>0.0)yieldstate[0]=2;
            if(qd.istpmm[numm][4]>0.0)yieldstate[1]=2;
            if(qd.istpmm[numm][5]>0.0)yieldstate[2]=2;
        }
        for(int i=0;i<numstr;i++){
            if(qAbs(value[i])<eps){
                valuestr[i]=QString();
            }else{
                valuestr[i]=QString::number(value[i],'f',2);
            }
            textColor[i]=checkColor(value[i]);
//            if(value[i]>=1.0)textColor[i]=textColorFatal;
        }
        break;
    case OUTFIGTYPE::ALLOWRATIOU://62
        plotYieldMark=true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        if(iPin){
            value[0] = qd.ultmm[numm][87];
            if(plotYieldMark){
                if(qd.ultmm[numm][30]>0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=1;//軸引張降伏
                if(qd.ultmm[numm][30]<=0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=2;//軸圧縮降伏
            }
            break;
        }

        value[0]=qd.ultmm[numm][81];
        value[1]=qd.ultmm[numm][82];
        value[2]=0.0;
        value[3]=qd.ultmm[numm][83];
        value[4]=qd.ultmm[numm][85];
        value[5]=qd.ultmm[numm][87];

        if(plotYieldMark){
            if(qd.istpmm[numm][3]>0.0)yieldstate[0]=2;
            if(qd.istpmm[numm][4]>0.0)yieldstate[1]=2;
            if(qd.istpmm[numm][5]>0.0)yieldstate[2]=2;
        }
        for(int i=0;i<numstr;i++){
            if(qAbs(value[i])<eps){
                valuestr[i]=QString();
            }else{
                valuestr[i]=QString::number(value[i],'f',2);
            }
            textColor[i]=checkColor(value[i]);
//            if(value[i]>=1.0)textColor[i]=textColorFatal;
        }
        break;
    case OUTFIGTYPE::STRENGTH://61
        plotYieldMark=true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        if(iPin){
            if(qd.ultmm[numm][30]>0.0){//軸力正
                value[2] = qMax(qd.ultmm[numm][42],qd.ultmm[numm][54]);
            }else{
                value[2] = qMax(qd.ultmm[numm][43],qd.ultmm[numm][55]);
            }
            if(plotYieldMark){
                if(qd.ultmm[numm][30]>0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=1;//軸引張降伏
                if(qd.ultmm[numm][30]<=0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=2;//軸圧縮降伏
            }
            break;
        }

        if(qAbs(qd.ultmm[numm][30])< eps){
        }else if(qd.ultmm[numm][30]>0.0){
            value[6] = -qd.ultmm[numm][42];
        }else{
            value[6] = -qd.ultmm[numm][43];
        }
        if(isZin){
            value[0]=-qd.ultmm[numm][36];//Muyi
            value[1]=qd.ultmm[numm][48];//Muyj
            value[3]=-qd.ultmm[numm][63];//Quzi
            value[4]=-qd.ultmm[numm][67];//Quzj
            value[5]=-qd.ultmm[numm][65];//Quzc
        }else{
            value[0]=qd.ultmm[numm][37];//Muzi
            value[1]=-qd.ultmm[numm][49];//Muzj
            value[3]=-qd.ultmm[numm][62];//Quyi
            value[4]=-qd.ultmm[numm][66];//Quyj
            value[5]=-qd.ultmm[numm][64];//Quyc
        }
        if(plotYieldMark){
            if(qd.istpmm[numm][3]>0.0)yieldstate[0]=2;
            if(qd.istpmm[numm][4]>0.0)yieldstate[1]=2;
            if(qd.istpmm[numm][5]>0.0)yieldstate[2]=2;
        }

        for(int i=0;i<numstr;i++){
            if(qAbs(value[i])<eps){
                valuestr[i]=QString();
            }else{
                if(myDataType==DATATYPE::TPCOLUMN){
                    valuestr[i]=QString::number(-value[i],'f',0);
                }else{
                    valuestr[i]=QString::number(value[i],'f',0);
                }
            }
        }

        qDebug()<<"STRENGTH:5";

        break;
    case OUTFIGTYPE::PLASTICRATIO://64、DUFMMの１～４と対応？テストモデル必要
        plotYieldMark=true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        if(iPin){
            if(qAbs(qd.dufmm[numm][2])>eps){
                valuestr[2]=QString::number(qd.dufmm[numm][2],'f',2);
            }
            if(plotYieldMark){
                if(qd.ultmm[numm][30]>0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=1;//軸引張降伏
                if(qd.ultmm[numm][30]<=0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=2;//軸圧縮降伏
            }
        }
        if(qAbs(qd.dufmm[numm][0])>eps)valuestr[0]=QString::number(qd.dufmm[numm][0],'f',2);
        if(qAbs(qd.dufmm[numm][1])>eps)valuestr[1]=QString::number(qd.dufmm[numm][1],'f',2);
        if(qAbs(qd.dufmm[numm][3])>eps)valuestr[2]=QString::number(qd.dufmm[numm][3],'f',2);

        if(plotYieldMark){
            if(qd.istpmm[numm][3]>0.0)yieldstate[0]=2;
            if(qd.istpmm[numm][4]>0.0)yieldstate[1]=2;
            if(qd.istpmm[numm][5]>0.0)yieldstate[2]=2;
        }
        break;

    case OUTFIGTYPE::YIELDSTEP://65
        plotYieldMark=true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        if(iPin){
            is6 = qd.istpmm[numm][5];//軸降伏
            if(is6 != 0)valuestr[2] = QString::number(qd.iastep[is6-1],'f',0);
            if(plotYieldMark){
                if(qd.ultmm[numm][30]>0.0 && qd.istpmm[numm][5]>0.0 && is6>0)yieldstate[3]=1;//軸引張降伏
                if(qd.ultmm[numm][30]<=0.0 && qd.istpmm[numm][5]>0.0 && is6>0)yieldstate[3]=2;//軸圧縮降伏
            }
        }

        is4 = qd.istpmm[numm][3];//i端曲げ降伏
        is5 = qd.istpmm[numm][4];//j端曲げ降伏
        is6 = qd.istpmm[numm][5];//軸降伏・せん断降伏
        if(is4 != 0)valuestr[0] = QString::number(qd.iastep[is4-1],'f',0);
        if(is5 != 0)valuestr[1] = QString::number(qd.iastep[is5-1],'f',0);
        if(is6 != 0)valuestr[2] = QString::number(qd.iastep[is6-1],'f',0);

        if(plotYieldMark){
            if(is4>0.0)yieldstate[0]=2;
            if(is5>0.0)yieldstate[1]=2;
            if(is5>0.0)yieldstate[2]=2;
        }
        break;
    case OUTFIGTYPE::YIELDLOADMAG://66
        plotYieldMark=true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        if(iPin){
            if(qd.istpmm[numm][5] != 0)value[2]= qd.cstpmm[numm][5];

            if(plotYieldMark){
                if(qd.ultmm[numm][30]>0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=1;//軸引張降伏
                if(qd.ultmm[numm][30]<=0.0 && qd.istpmm[numm][5]>0.0)yieldstate[3]=2;//軸圧縮降伏
            }
        }
        if(qd.istpmm[numm][3] != 0)value[0]= qd.cstpmm[numm][3];
        if(qd.istpmm[numm][4] != 0)value[1]= qd.cstpmm[numm][4];
        if(qd.istpmm[numm][5] != 0 && myDataType==DATATYPE::TPCOLUMN)value[2]= qd.cstpmm[numm][5];
        for(int i=0;i<numstr;i++){
            if(qAbs(value[i])<eps){
                valuestr[i]=QString();
            }else{
                valuestr[i]=QString::number(value[i],'f',2);
            }
        }
        if(plotYieldMark){
            if(qd.istpmm[numm][3]>0.0)yieldstate[0]=2;
            if(qd.istpmm[numm][4]>0.0)yieldstate[1]=2;
            if(qd.istpmm[numm][5]>0.0)yieldstate[2]=2;
        }

        break;
    default:
        break;
    }

    QPointF ps = myPointList.first();
    QPointF pe = myPointList.last();
    qreal totalLength=member->getTotalLength();
    qreal p_ang = qAtan2((pe.y() - ps.y()), (pe.x() - ps.x()));
    //qreal t_ang;
    //    if (isClockWize)t_ang = p_ang - M_PI / 2.0;
    //    if (!isClockWize)t_ang = p_ang + M_PI / 2.0;
    bool wallgirderVisible = false;

    qreal maxLength = 999999.0;
    qreal iLeftMaxLength = 999999.0;
    qreal jLeftMaxLength = 999999.0;
    qreal cLeftMaxLength = 999999.0;
    qreal iRightMaxLength = 999999.0;
    qreal jRightMaxLength = 999999.0;
    qreal cRightMaxLength = 999999.0;
    if (myDataType==DATATYPE::TPCOLUMN) {
        qreal maxdot = -999999;
        qreal mindot = 999999;
        Q_FOREACH (MemberData *md, iRelatedMember) {
            if (md->dataType() == DATATYPE::TPGIRDER) {
                //i-jとi-多端の外積と、通り面の内積が正か負かで左か右か判断
                MemberData *member = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
                VEC3D ijvec = member->getUVector();
                VEC3D mdvec;
                if (md->getJointList().first() == member->getJointList().first()) {
                    mdvec = VEC3D(md->jXYZ().x - md->iXYZ().x,
                                  md->jXYZ().y - md->iXYZ().y,
                                  md->jXYZ().z - md->iXYZ().z);
                } else {
                    mdvec = VEC3D(md->iXYZ().x - md->jXYZ().x,
                                  md->iXYZ().y - md->jXYZ().y,
                                  md->iXYZ().z - md->jXYZ().z);
                }
                VEC3D cross = CalcVector3D::crossNormalized(ijvec, mdvec);
                qreal dot = CalcVector3D::dot(cross, myPlaneVector);
                if (dot > 0 && dot > maxdot)iLeftMaxLength = md->getTotalLength() / 2.0;
                if (dot < 0 && dot < mindot)iRightMaxLength = md->getTotalLength() / 2.0;
            }
        }
        Q_FOREACH (MemberData *md, jRelatedMember) {
            if (md->dataType() == DATATYPE::TPGIRDER) {
                //j-iとj-他端の外積と、通り面の内積が正か負かで左か右か判断
                MemberData *member = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(myUuid);
                VEC3D jivec = member->getUVector();
                jivec = VEC3D(-jivec.x, -jivec.y, -jivec.z);
                VEC3D mdvec;
                if (md->getJointList().first() == member->getJointList().last()) {
                    mdvec = VEC3D(md->jXYZ().x - md->iXYZ().x,
                                  md->jXYZ().y - md->iXYZ().y,
                                  md->jXYZ().z - md->iXYZ().z);
                } else {
                    mdvec = VEC3D(md->iXYZ().x - md->jXYZ().x,
                                  md->iXYZ().y - md->jXYZ().y,
                                  md->iXYZ().z - md->jXYZ().z);
                }
                VEC3D cross = CalcVector3D::crossNormalized(jivec, mdvec);
                qreal dot = CalcVector3D::dot(cross, myPlaneVector);
                if (dot > 0 && dot > maxdot)jRightMaxLength = md->getTotalLength() / 2.0;
                if (dot < 0 && dot < mindot)jLeftMaxLength = md->getTotalLength() / 2.0;
            }
        }

    }
    if (myDataType==DATATYPE::TPCOLUMN) {
        cRightMaxLength = (iRightMaxLength + jRightMaxLength) / 2.0;
        cLeftMaxLength = (iLeftMaxLength + jLeftMaxLength) / 2.0;
    }
    if (myDataType==DATATYPE::TPGIRDER)maxLength = totalLength / 2.0;

    if (myDataType==DATATYPE::TPCOLUMN && myFigureTemplate->figureVisible(DATATYPE::TPCOLUMN)) {
        //        if(!value3_leftMiddle.isEmpty()){
        //            OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
        //                                                       -0.5 * ps.y() - 0.5 * pe.y()), p_ang - M_PI / 2.0, value3_leftMiddle, textColor,
        //                                               OFFSETPOINT::OFFSET_LEFTMIDDLE, cLeftMaxLength,stringSize) );
        //        }
        OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                                                   -0.5 * ps.y() - 0.5 * pe.y()), p_ang - M_PI / 2.0, valuestr[6], textColor[6],
                OFFSETPOINT::OFFSET_LEFTMIDDLE, cRightMaxLength,stringSize) );
        OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                                                   -0.5 * ps.y() - 0.5 * pe.y()), p_ang - M_PI / 2.0, valuestr[3], textColor[3],
                OFFSETPOINT::OFFSET_RIGHTMIDDLE, cRightMaxLength,stringSize) );

        OTvaluetext.append( POINT_AND_TEXT(QPointF(ps.x(), -ps.y()), p_ang - M_PI / 2.0,
                                           valuestr[0], textColor[0], OFFSETPOINT::OFFSET_RIGHTTOP, jRightMaxLength,stringSize) );

        OTvaluetext.append( POINT_AND_TEXT(QPointF(pe.x(), -pe.y()), p_ang - M_PI / 2.0,
                                           valuestr[1], textColor[1], OFFSETPOINT::OFFSET_LEFTBOTTOM, iLeftMaxLength,stringSize) );
    } else if (myDataType==DATATYPE::TPGIRDER && myFigureTemplate->figureVisible(DATATYPE::TPGIRDER)) {
        if (!isAroundPlane
                || wallgirderVisible)OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                                                                                -0.5 * ps.y() - 0.5 * pe.y()), p_ang, valuestr[2], textColor[2],
                OFFSETPOINT::OFFSET_CENTERBOTTOM, maxLength,stringSize ));
        //        if(!value3_leftMiddle.isEmpty()){
        //            if (!isAroundPlane
        //                    || wallgirderVisible)OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
        //                                                                                    -0.5 * ps.y() - 0.5 * pe.y()), p_ang, value3_leftMiddle, textColor,
        //                                                                            OFFSETPOINT::OFFSET_CENTERTOP, maxLength,stringSize) );
        //        }
        if (!isAroundPlane
                || wallgirderVisible)OTvaluetext.append( POINT_AND_TEXT(QPointF(ps.x(), -ps.y()), p_ang,
                                                                        valuestr[3], textColor[3], OFFSETPOINT::OFFSET_RIGHTTOP2, maxLength,stringSize) );
        if (!isAroundPlane
                || wallgirderVisible)OTvaluetext.append( POINT_AND_TEXT(QPointF(pe.x(), -pe.y()), p_ang,
                                                                        valuestr[4], textColor[4], OFFSETPOINT::OFFSET_LEFTTOP2, maxLength,stringSize) );
        if (!isAroundPlane
                || wallgirderVisible)OTvaluetext.append( POINT_AND_TEXT(QPointF(ps.x(), -ps.y()), p_ang,
                                                                        valuestr[0], textColor[0], OFFSETPOINT::OFFSET_RIGHTBOTTOM, maxLength,stringSize) );
        if (!isAroundPlane
                || wallgirderVisible)OTvaluetext.append( POINT_AND_TEXT(QPointF(pe.x(), -pe.y()), p_ang,
                                                                        valuestr[1], textColor[1], OFFSETPOINT::OFFSET_LEFTTOP, maxLength,stringSize) );
    } else if (myDataType==DATATYPE::TPBRACE && myFigureTemplate->figureVisible(DATATYPE::TPBRACE)) {
        OTvaluetext.append( POINT_AND_TEXT(QPointF(0.5 * ps.x() + 0.5 * pe.x(),
                                                   -0.5 * ps.y() - 0.5 * pe.y()), p_ang, valuestr[0], textColor[0],
                OFFSETPOINT::OFFSET_CENTERTOP, maxLength,stringSize) );
    }

//    plotYieldMark = true;
//    yieldstate[0]=2;
//    yieldstate[1]=2;
//    yieldstate[2]=2;
//    yieldstate[3]=2;
    if(plotYieldMark){
        QColor color = myFigureTemplate->plasticSymbolColor();
        int size = myFigureTemplate->plasticSymbolSize();
        size = 50 + (size-3)*10;

        QVector<qreal> len_f_r=getOutputFaceRigidLength();
        qreal FL = len_f_r.at(0);
        qreal FR = len_f_r.at(1);

        qreal iFaceRatio = FL/member->getTotalLength();
        qreal jFaceRatio = FR/member->getTotalLength();
        QPointF iFacePoint = (1.0-iFaceRatio)*ps + iFaceRatio * pe;
        QPointF jFacePoint = (1.0-jFaceRatio)*ps + jFaceRatio * pe;
        QPointF middlePoint = 0.5*ps + 0.5 * pe;
        QPainterPath i_path;
        i_path.addEllipse(iFacePoint,2.0*size,2.0*size);
        QPainterPath j_path;
        j_path.addEllipse(jFacePoint,2.0*size,2.0*size);
        QPainterPath shear_path;
        shear_path.moveTo(middlePoint+QPointF(0,150));
        shear_path.lineTo(middlePoint+QPointF(0,-150));
        QPainterPath shear_path2;
        QPainterPath hippari_path;
        QPainterPath assyuku_path;
        if(myDataType==DATATYPE::TPCOLUMN){
            shear_path2.moveTo(middlePoint+QPointF(2.0*size,-size));
            shear_path2.lineTo(middlePoint+QPointF(3.0*size,0));
            shear_path2.lineTo(middlePoint+QPointF(-3.0*size,0));
            shear_path2.lineTo(middlePoint+QPointF(-2.0*size,size));
            hippari_path.moveTo(middlePoint+QPointF(2.0*size,-3.0*size));
            hippari_path.lineTo(middlePoint+QPointF(0,-size));
            hippari_path.lineTo(middlePoint+QPointF(-2.0*size,-3.0*size));
            hippari_path.moveTo(middlePoint+QPointF(2.0*size,3.0*size));
            hippari_path.lineTo(middlePoint+QPointF(0,size));
            hippari_path.lineTo(middlePoint+QPointF(-2.0*size,3.0*size));
            assyuku_path.moveTo(middlePoint+QPointF(2.0*size,-size));
            assyuku_path.lineTo(middlePoint+QPointF(0,-3.0*size));
            assyuku_path.lineTo(middlePoint+QPointF(-2.0*size,-size));
            assyuku_path.moveTo(middlePoint+QPointF(2.0*size,size));
            assyuku_path.lineTo(middlePoint+QPointF(0,3.0*size));
            assyuku_path.lineTo(middlePoint+QPointF(-2.0*size,size));

        }else{
            shear_path2.moveTo(middlePoint+QPointF(-size,2.0*size));
            shear_path2.lineTo(middlePoint+QPointF(0,3.0*size));
            shear_path2.lineTo(middlePoint+QPointF(0,-3.0*size));
            shear_path2.lineTo(middlePoint+QPointF(size,-2.0*size));
            hippari_path.moveTo(middlePoint+QPointF(-3.0*size,2.0*size));
            hippari_path.lineTo(middlePoint+QPointF(-size,0));
            hippari_path.lineTo(middlePoint+QPointF(-3.0*size,-2.0*size));
            hippari_path.moveTo(middlePoint+QPointF(3.0*size,2.0*size));
            hippari_path.lineTo(middlePoint+QPointF(size,0));
            hippari_path.lineTo(middlePoint+QPointF(3.0*size,-2.0*size));
            assyuku_path.moveTo(middlePoint+QPointF(-size,2.0*size));
            assyuku_path.lineTo(middlePoint+QPointF(-3.0*size,0));
            assyuku_path.lineTo(middlePoint+QPointF(-size,-2.0*size));
            assyuku_path.moveTo(middlePoint+QPointF(size,2.0*size));
            assyuku_path.lineTo(middlePoint+QPointF(3.0*size,0));
            assyuku_path.lineTo(middlePoint+QPointF(size,-2.0*size));
        }


        //1...白丸、2...黒丸
        if(yieldstate[0]==1)OTpathes.append(PATH_AND_PEN(i_path,QPen(color,0),QBrush(Qt::NoBrush)));//i
        if(yieldstate[0]==2)OTpathes.append(PATH_AND_PEN(i_path,QPen(color,0),QBrush(color)));//i
        if(yieldstate[1]==1)OTpathes.append(PATH_AND_PEN(j_path,QPen(color,0),QBrush(Qt::NoBrush)));//i
        if(yieldstate[1]==2)OTpathes.append(PATH_AND_PEN(j_path,QPen(color,0),QBrush(color)));//j
        if(yieldstate[2]==2)OTpathes.append(PATH_AND_PEN(shear_path2,QPen(color,0)));//せん断
        if(yieldstate[3]==1)OTpathes.append(PATH_AND_PEN(hippari_path,QPen(color,0)));//軸引張
        if(yieldstate[3]==2)OTpathes.append(PATH_AND_PEN(assyuku_path,QPen(color,0)));//軸圧縮
    }

}

} // namespace post3dapp
