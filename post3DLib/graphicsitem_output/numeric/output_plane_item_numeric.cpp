#include "output_plane_item_numeric.h"

#include <QPainter>

#include "calc_vector2d.h"
#include "check_elevation_template.h"
#include "define_npstn.h"
#include "define_output.h"
#include "figure_template.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"


namespace post3dapp{

OutputPlaneItemNumeric::OutputPlaneItemNumeric(FigureCommonTemplate* fig, DATATYPE dtype, STRUCTTYPE stype, const QUuid &uid, const QList<QPointF> &plist)
    : OutputPlaneItem(fig, dtype,stype, uid,plist)
{
    qDebug() << "outplaneNumeric";
}

QColor OutputPlaneItemNumeric::checkColor(qreal checkVal){
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
void OutputPlaneItemNumeric::setDrawingStatusDirect()
{
    qDebug()<< "setDrawingStatusDirect init";
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
    int stringSize=valueTextSize;

    qreal minx, maxx, miny, maxy;
    for (int i = 0; i < myPointList.count(); i++) {
        if (i == 0) {
            minx = myPointList.at(i).x();
            maxx = myPointList.at(i).x();
            miny = myPointList.at(i).y();
            maxy = myPointList.at(i).y();
        } else {
            minx = qMin(minx, myPointList.at(i).x());
            maxx = qMax(maxx, myPointList.at(i).x());
            miny = qMin(miny, myPointList.at(i).y());
            maxy = qMax(maxy, myPointList.at(i).y());
        }
    }
    QPointF ps;
    QPointF pe;
    ps = QPointF((minx + maxx) / 2.0, miny); //i端
    pe = QPointF((minx + maxx) / 2.0, maxy); //j端

    ps.setY(-ps.y());
    pe.setY(-pe.y());

    QColor wallColor = myFigureTemplate->valueTextColor(DATATYPE::TPWALL,ATTENTION_STATUS::STATUS_NORMAL);
//    QColor wallColorFatal = myFigureTemplate->valueTextColor(DATATYPE::TPWALL,ATTENTION_STATUS::STATUS_FATAL);


    DataNBLD3D_CHCK chck;
    DataNBLD3D_RSTQD qd;
    bool plotYieldMark = false;
    QVector<int> yieldstate(5,0);

    int ichk = 0;
    QString name = myFigureTemplate->getFigureTypeProperty().caseName;
    int numww = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
    int numval = 3;
    QVector<qreal> value(numval,0);
     QVector<QString> valuestr(numval,QString());
    int is1,is2,is4;

    switch(type.type){
    case OUTFIGTYPE::CHECKRATIO:
        chck= UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();
        for(int i=0;i<chck.namech.count();i++){
            if(name == chck.namech.at(i).trimmed()){
                ichk = i;break;
            }
        }
        value[0] = chck.rstwwa[ichk][numww][14];
        valuestr[0] = QString::number(value[0],'f',2);
//        if(value[0]>=1.0)wallColor=wallColorFatal;
        wallColor = checkColor(value[0]);
        break;
    case OUTFIGTYPE::ALLOW:
        chck= UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();
        for(int i=0;i<chck.namech.count();i++){
            if(name == chck.namech.at(i).trimmed()){
                ichk = i;break;
            }
        }
        value[0] = chck.rstwwa[ichk][numww][12];
        valuestr[0] = QString::number(value[0],'f',0);
        break;
    case OUTFIGTYPE::STRENGTH://61
        plotYieldMark = true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }

        value[0] = qd.ultww[numww][3];
        valuestr[0] = QString::number(value[0],'f',0);
        if(plotYieldMark){
            if(qd.istpww[numww][0]>0.0)yieldstate[0]=2;
            if(qd.istpww[numww][1]>0.0)yieldstate[1]=2;
            if(qd.istpww[numww][3]>0.0)yieldstate[2]=2;
        }
        break;
    case OUTFIGTYPE::ALLOWRATIOU://62
        plotYieldMark = true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        value[0] = qd.ultww[numww][5];
        valuestr[0] = QString::number(value[0],'f',2);
        if(plotYieldMark){
            if(qd.istpww[numww][0]>0.0)yieldstate[0]=2;
            if(qd.istpww[numww][1]>0.0)yieldstate[1]=2;
            if(qd.istpww[numww][3]>0.0)yieldstate[2]=2;
        }
        wallColor = checkColor(value[0]);
//        if(value[0]>=1.0)wallColor=wallColorFatal;
        break;
    case OUTFIGTYPE::ALLOWRATIOE://63
        plotYieldMark = true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        value[0] = qd.ultww[numww][4];
        valuestr[0] = QString::number(value[0],'f',2);
        if(plotYieldMark){
            if(qd.istpww[numww][0]>0.0)yieldstate[0]=2;
            if(qd.istpww[numww][1]>0.0)yieldstate[1]=2;
            if(qd.istpww[numww][3]>0.0)yieldstate[2]=2;
        }
        wallColor = checkColor(value[0]);
//        if(value[0]>=1.0)wallColor=wallColorFatal;
        break;
    case OUTFIGTYPE::PLASTICRATIO://64
        break;
    case OUTFIGTYPE::YIELDSTEP://65
        plotYieldMark = true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        is1 = qd.istpww[numww][0];//i曲げ降伏
        is2 = qd.istpww[numww][1];//j曲げ降伏
        is4 = qd.istpww[numww][3];//せん断降伏
        if(is1 != 0)value[1]=(double)(0.01+qd.iastep[is1-1]);
        if(is2 != 0)value[2]=(double)(0.01+qd.iastep[is2-1]);
        if(is4 != 0)value[0]=(double)(0.01+qd.iastep[is4-1]);
        valuestr[1] = QString::number(value[1],'f',0);
        valuestr[2] = QString::number(value[2],'f',0);
        valuestr[0] = QString::number(value[0],'f',0);
        if(plotYieldMark){
            if(qd.istpww[numww][0]>0.0)yieldstate[0]=2;
            if(qd.istpww[numww][1]>0.0)yieldstate[1]=2;
            if(qd.istpww[numww][3]>0.0)yieldstate[2]=2;
        }
        break;
    case OUTFIGTYPE::YIELDLOADMAG://66
        plotYieldMark = true;
        if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::DS){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).dsFileName);
        }else if(myFigureTemplate->getFigureTypeProperty().ds_or_qu==FIGURE_TYPE_PROPERTY::DSQU::QU){
            qd = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData().value(UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(name).quFileName);
        }else{
            qDebug()<<"unknown qu ds type";
        }
        is1 = qd.istpww[numww][0];//i曲げ降伏
        is2 = qd.istpww[numww][1];//j曲げ降伏
        is4 = qd.istpww[numww][3];//せん断降伏
        if(is1 != 0)value[1]=qd.cstpww[numww][0];
        if(is2 != 0)value[2]=qd.cstpww[numww][1];
        if(is4 != 0)value[0]=qd.cstpww[numww][3];
        valuestr[1] = QString::number(value[1],'f',2);
        valuestr[2] = QString::number(value[2],'f',2);
        valuestr[0] = QString::number(value[0],'f',2);
        if(plotYieldMark){
            if(qd.istpww[numww][0]>0.0)yieldstate[0]=2;
            if(qd.istpww[numww][1]>0.0)yieldstate[1]=2;
            if(qd.istpww[numww][3]>0.0)yieldstate[2]=2;
        }

        break;

    default:
        break;
    }

    if (myFigureTemplate->figureVisible(DATATYPE::TPWALL)) {
        if(!isZero(value[0])){//center
            OTvaluetext.append( POINT_AND_TEXT(0.5 * ps + 0.5 * pe, 0.0, valuestr[0],
                                               wallColor, OFFSETPOINT::OFFSET_NONE, 999999,stringSize) );
        }
        if(!isZero(value[1])){//i
            OTvaluetext.append( POINT_AND_TEXT(0.8 * ps + 0.2 * pe, 0.0, valuestr[1],
                                               wallColor, OFFSETPOINT::OFFSET_NONE, 999999,stringSize) );
        }
        if(!isZero(value[2])){//j
            OTvaluetext.append( POINT_AND_TEXT(0.2 * ps + 0.8 * pe, 0.0, valuestr[2],
                                               wallColor, OFFSETPOINT::OFFSET_NONE, 999999,stringSize) );
        }
    }

//    plotYieldMark = true;
//    yieldstate[0]=2;
//    yieldstate[1]=2;
//    yieldstate[2]=2;
    if(plotYieldMark){
        QColor color = myFigureTemplate->plasticSymbolColor();
        int size = myFigureTemplate->plasticSymbolSize();
        size = 50 + (size - 3) * 10;
        ps.setY(-ps.y());
        pe.setY(-pe.y());

        qreal iFaceRatio = 0.1;
        qreal jFaceRatio = 0.9;
        QPointF iFacePoint = (1.0-iFaceRatio)*ps + iFaceRatio * pe;
        QPointF jFacePoint = (1.0-jFaceRatio)*ps + jFaceRatio * pe;
        QPointF middlePoint = 0.5*ps + 0.5 * pe;
        QPainterPath i_path;
        i_path.addEllipse(iFacePoint,2.0*size,2.0*size);
        QPainterPath j_path;
        j_path.addEllipse(jFacePoint,2.0*size,2.0*size);
        QPainterPath shear_path;
        shear_path.moveTo(middlePoint+QPointF(3.0*size,0));
        shear_path.lineTo(middlePoint+QPointF(-3.0*size,0));
        QPainterPath shear_path2;
        shear_path2.moveTo(middlePoint+QPointF(2.0*size,-size));
        shear_path2.lineTo(middlePoint+QPointF(3.0*size,0));
        shear_path2.lineTo(middlePoint+QPointF(-3.0*size,0));
        shear_path2.lineTo(middlePoint+QPointF(-2.0*size,size));



        if(yieldstate[0]==2)OTpathes.append(PATH_AND_PEN(i_path,QPen(color,0),QBrush(color)));//i
        if(yieldstate[1]==2)OTpathes.append(PATH_AND_PEN(j_path,QPen(color,0),QBrush(color)));//j
        if(yieldstate[2]==2)OTpathes.append(PATH_AND_PEN(shear_path2,QPen(color,0)));//せん断
    }
    qDebug()<< "setDrawingStatusDirect done";

}



} // namespace post3dapp
