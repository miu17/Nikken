#include "output_joint_item_numeric.h"

#include "figure_template.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"
namespace post3dapp{

OutputJointItemNumeric::OutputJointItemNumeric(FigureTemplate* fig, DATATYPE dtype,STRUCTTYPE stype, const QUuid &uid,VEC3D mypvec,VEC3D myxvec)
    : OutputJointItem( fig, dtype,stype, uid ,mypvec,myxvec){}



void OutputJointItemNumeric::setDrawingStatusDirect()
{
    qDebug()<<"numeric!!setDrawStatus!!";
    FIGURE_TYPE_PROPERTY type = myFigureTemplate->getFigureTypeProperty();
    QList<QList<qreal>> reactions;
    DataNBLD3D_LOAD load;
    DataNBLD3D_CMDL model;
    int numj;
    XYZ disp;

    JointData *joint;

    int maxketa,ild;
    int numvalue = 6;

    QVector<qreal> value(numvalue,0.0);
    QVector<QString> valuestr(numvalue,QString());
    QVector<QColor> textColor(numvalue,normalTextColor);

    switch(myFigureTemplate->getOutFigType()){
    case OUTFIGTYPE::COORDINATE:
        joint = UnifiedFixedData::getInstance()->getAnalysisData()->jointIDToPointer(myUuid);
        value[0] = joint->xPos();
        value[1] = joint->yPos();
        value[2] = joint->zPos();
        qDebug()<<"coord";
        maxketa = qMax(qMax((int)log10(qAbs(value[0]))+6,(int)log10(qAbs(value[1]))+6),(int)log10(qAbs(value[2]))+6);
        maxketa = qMax(maxketa,6);
        for(int i=0;i<3;i++){
            if(!isZero(value[i]))valuestr[i]=QString::number(value[i], 'f', 3).rightJustified(maxketa);
        }
        break;
    case OUTFIGTYPE::DISP:
        qDebug()<<"disp";
        disp = UnifiedFixedData::getInstance()->getPstnData()->getCombinationJointDisp(myUuid, type.stress.at(0));
        value[0] = disp.x*1000;
        value[1] = disp.y*1000;
        value[2] = disp.z*1000;//m->mm
        maxketa = qMax(qMax((int)log10(qAbs(value[0]))+1,(int)log10(qAbs(value[1]))+1),(int)log10(qAbs(value[2]))+1);
        maxketa = qMax(maxketa,1);
        for(int i=0;i<3;i++){
            if(!isZero(value[i]))valuestr[i]=QString::number(value[i], 'f',0).rightJustified(maxketa);
        }
        break;
    case OUTFIGTYPE::JOINTMASS:
        load = UnifiedFixedData::getInstance()->getPstnData()->getLoadData();
        numj = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
        value[0] = load.semass.at(numj).at(5);
        valuestr[0]=QString::number(value[0], 'f', 0);

        break;

    case OUTFIGTYPE::REACTMAP:
        qDebug()<<"reactmap";
        for(int i= 0;i<type.stress.count();i++){
            reactions << UnifiedFixedData::getInstance()->getPstnData()->getCombinationReaction(myUuid, type.stress.at(i));
        }
        value[0] = reactions.at(0).at(0);
        value[1] = reactions.at(0).at(1);
        value[2] = reactions.at(0).at(2);
        maxketa = qMax(qMax((int)log10(qAbs(value[0]))+1,(int)log10(qAbs(value[1]))+1),(int)log10(qAbs(value[2]))+1);
        maxketa = qMax(maxketa,4);
        for(int i=0;i<numvalue;i++){
            if(!isZero(value[i]))valuestr[i]=QString::number(value[i], 'f', 0).rightJustified(maxketa);
            if(value[i]<0)textColor[i] = warningTextColor;
        }
        break;

    case OUTFIGTYPE::COMBIREACTMAP:
        for(int i= 0;i<type.stress.count();i++){
            reactions << UnifiedFixedData::getInstance()->getPstnData()->getCombinationReaction(myUuid, type.stress.at(i));
        }
        value[3] = reactions.at(0).at(2);
        value[4] = reactions.at(1).at(2);
        value[5] = reactions.at(2).at(2);
        value[0] = reactions.at(3).at(2);
        value[1] = reactions.at(4).at(2);
        maxketa = qMax(qMax(qMax(qMax((int)log10(qAbs(reactions.at(0).at(2)))+1,
                                      (int)log10(qAbs(reactions.at(1).at(2)))+1),
                                 (int)log10(qAbs(reactions.at(2).at(2)))+1),
                            (int)log10(qAbs(reactions.at(3).at(2)))+1),
                       (int)log10(qAbs(reactions.at(4).at(2)))+1);
        maxketa = qMax(maxketa,1);
        for(int i=0;i<numvalue;i++){
            if(!isZero(value[i]))valuestr[i]=QString::number(value[i], 'f', 0).rightJustified(maxketa);
            if(value[i]<0)textColor[i] = warningTextColor;
        }

        break;
    case OUTFIGTYPE::OUTERFORCE://32
        qDebug()<<"outerforce!!";
        load = UnifiedFixedData::getInstance()->getPstnData()->getLoadData();
        numj = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
        //CaseDL -> RSTDL + RSTLL1 -> CDL,CLL1
        for(int numrst=0;numrst<type.stress.at(0).count();numrst++){
            ild=0;
            for(int i=0;i<load.numc;i++){
                qDebug()<<"OUTERFORCE:namel"<<load.namel.at(i).trimmed()<<type.stress.at(0).at(numrst).rst.midRef(3);
                if(load.namel.at(i).trimmed()=="C"+type.stress.at(0).at(numrst).rst.midRef(3)){
                    ild=i;break;
                }
            }
            value[0]+=load.fincj[ild][numj][0];
            value[1]+=load.fincj[ild][numj][1];
            value[2]+=load.fincj[ild][numj][2];
        }

        maxketa = qMax(qMax((int)log10(qAbs(value[0]))+1,(int)log10(qAbs(value[1]))+1),(int)log10(qAbs(value[2]))+1);
        maxketa = qMax(maxketa,3);
        for(int i=0;i<3;i++){
            if(!isZero(value[0]) || !isZero(value[1]) || !isZero(value[2]))valuestr[i]=QString::number(value[i], 'f', 0).rightJustified(maxketa);
        }
        break;
    case OUTFIGTYPE::OUTERMOMENT://33
        load = UnifiedFixedData::getInstance()->getPstnData()->getLoadData();
        numj = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(myUuid);
        //CaseDL -> RSTDL + RSTLL1 -> CDL,CLL1
        for(int numrst=0;numrst<type.stress.at(0).count();numrst++){
            ild=0;
            for(int i=0;i<load.numc;i++){
                qDebug()<<load.namel.at(i).trimmed();
                if(load.namel.at(i).trimmed()=="C"+type.stress.at(0).at(numrst).rst.midRef(3)){
                    ild=i;break;
                }
            }
            value[0]+=load.fincj[ild][numj][3];
            value[1]+=load.fincj[ild][numj][4];
            value[2]+=load.fincj[ild][numj][5];
        }

        maxketa = qMax(qMax((int)log10(qAbs(value[0]))+1,(int)log10(qAbs(value[1]))+1),(int)log10(qAbs(value[2]))+1);
        maxketa = qMax(maxketa,3);
        for(int i=0;i<3;i++){
            if(!isZero(value[0]) || !isZero(value[1]) || !isZero(value[2]))valuestr[i]=QString::number(value[i], 'f', 0).rightJustified(maxketa);
        }

        break;
    case OUTFIGTYPE::LAYERDEFORMMAP://56
        break;
    case OUTFIGTYPE::LAYERSTIFFMAP://57
        break;


    default:
        qDebug()<<"default";
        break;
    }
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, valuestr[0], textColor[0],
                        OFFSETPOINT::OFFSET_RIGHTBOTTOM, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, valuestr[1], textColor[1],
                        OFFSETPOINT::OFFSET_RIGHTBOTTOM2, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, valuestr[2], textColor[2],
                        OFFSETPOINT::OFFSET_RIGHTBOTTOM3, 999999,valueTextSize) );

    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, valuestr[3], textColor[3],
                        OFFSETPOINT::OFFSET_CENTERTOP, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, valuestr[4], textColor[4],
                        OFFSETPOINT::OFFSET_LEFTBOTTOM, 999999,valueTextSize) );
    OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, valuestr[5], textColor[5],
                        OFFSETPOINT::OFFSET_LEFTBOTTOM2, 999999,valueTextSize) );



    qDebug()<<"OutputJointItemNumeric::setDrawingStatusDirect";

}

} // namespace post3dapp
