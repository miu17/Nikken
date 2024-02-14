#ifndef OUTPUT_PERS_SCENE_H
#define OUTPUT_PERS_SCENE_H

#include <QGLViewer/qglviewer.h>
#include <QtWidgets>

#include "customdraw_grscene.h"
#include "define_textpos.h"
#include "define_outputpers.h"
#include "define_glsetting.h"
#include "color_picker.h"
#include "manage_memberdata.h"
#include "manage_jointdata.h"

namespace post3dapp{
//enum DRAWINGSTYLE{ SOLIDWIRE=0, LINEONLY,SOLIDONLY, WIREONLY };
enum DIRECTION{DIRECTION_Y=0,DIRECTION_Z};
class OutputPersViewer;
class OpeningSet;

class OutputPersScene :public QObject
{

    Q_OBJECT
signals:
//    void drawString(QString str);
public:

    OutputPersScene( QObject* parent=0 ) :margin(0.005){
//        importData( QList<MemberData*>() ,QList<OpeningSet*>());dispRatioXY=10.0;
    }

//    void importData(const QList<MemberData*>& mdlist, const QList<OpeningSet*>& oplist);
//    void setDispRatio( qreal r);
//    void setDispOnOff( int i);
//    void setDrawContour(bool _drawContour){drawContour=_drawContour;}
//    void setDrawLegend(bool _drawLegend){drawLegend=_drawLegend;}
//    bool getDrawLegend()const{return drawLegend;}
//    void setDrawFileName(bool _drawFileName){drawFileName=_drawFileName;}
//    bool getDrawFileName()const{return drawFileName;}
//    void setDrawPoint(bool _drawPoint){drawPoint=_drawPoint;}
//    void setDrawZWall(bool _drawZWall){drawZWall=_drawZWall;}
//    void setDrawOpening(bool _drawOpening){drawOpening=_drawOpening;}
////    bool getDrawZWall()const{return drawZWall;}
//    void setLoadCase(QStringList str, QList<qreal> coef,QList<int> step);
//    void setColorContour(qreal _min,qreal _max){colorpicker.setMinMax(_min,_max);}
////    void setValueMinMax(qreal _min,qreal _max){minValue=_min;maxValue=_max;}
////    qreal getMinValue(){return minValue;}
////    qreal getMaxValue(){return maxValue;}
//    void setValueMinMax(ValueMinMax _minmax){minmax=_minmax;}
//    ValueMinMax getValueMinMax(){return minmax;}

//    ColorPicker* getColorPicker(){return &colorpicker;}
//    void setForceType(QHash<QString,QList<bool> > type){forceType=type;}
//    void setValueCalcMethod(int _method){valueCalcMethod=_method;}
//    void setNodeOrFace(int _norf){nodeOrFace=_norf;}

//    void setFloorFrameLimited(int, const QStringList &, int, const QStringList & );
//    void setSectionLimited(const QStringList & );

//    void initDisplayList();
//    QList<MemberData*> getFilteredMembers(){return filteredMembers;}

    void setWidgetState(const MODELSETTINGS& _){widgetState= _;}
    void drawSolid( bool simplified=false);
//    void drawNumberLabel(int fontsize=12.0);
//    void drawMemberLabel(int fontsize=12.0);
    void drawGraph(int selectGraphScale=0, qreal unitforce =100.0);
//    void drawReaction(QList<bool> drawarrow, bool drawvalue=true, qreal scale=1.0, int fontsize=20.0);
//    void drawYieldMarks(DRAWINGSTYLE dstyle);

//    void drawCoordinate();
//    void drawLogo();
//    void drawDispNumber(int fontsize=20.0);

//public slots:
//    void delSphere();

protected :
//    QRawFont rawFont;
    MODELSETTINGS widgetState;
    const qreal margin;

    QList<MemberData*> totalMembers;
    QList<MemberData*> filteredMembers;
//    QList<OpeningSet*> openingList;
    QList<GLUquadricObj*> sphereList;

//    bool drawDisp;
//    bool drawContour;
//    bool drawLegend;
//    bool drawFileName;
//    bool drawZWall;
//    bool drawOpening;
//    bool drawPoint;
//    qreal dispRatioXY;
//    qreal dispRatioTH;
//    QStringList loadcase;
//    QList<qreal> loadcoef;
//    QList<int> loadstep;
//    QHash<QString,QList<bool> > forceType;
//    int valueCalcMethod;
//    int nodeOrFace;
    ColorPicker colorpicker;
    ValueMinMax minmax;

    QHash<MemberData*,QList<qglviewer::Vec> > closedMemberTriangles;

//    int floorLimitedStatus;
//    int frameLimitedStatus;
//    QStringList limitedFloors;
//    QStringList limitedFrames;
//    QStringList limitedSection;

    QList<qglviewer::Vec> divideClosedMember( MemberData* md ) const;
    bool isMemberOkToDisplayLimited( MemberData* md ) const;
    XYZ getJointDisp(JointData*, qreal, QString loadcase) const;
    XYZ getCombinationDisp(JointData* jd, qreal r, QStringList loadcase, QList<qreal> coefload,QList<int> loadstep)const;
    void setLocalMatrix(qreal x1,  qreal y1,  qreal z1,
                        qreal x2,  qreal y2,  qreal z2,
                        qreal h0,  qreal h1,  qreal angle, DATATYPE dtype, DIRECTION direction, bool isVertical);
    QColor calcColor(MemberData* md,int i,int div);
    void createLineMember(MemberData* md,const QColor& lcolor,const QColor& scolor, qreal l1, qreal l2, qreal bb, qreal hh,
                                     DRAWINGSTYLE dstyle, bool simplified=false , bool isColored=true);

    void createLineMemberPart(DATATYPE stype, RENDERSHAPE dshape,
                          const QColor& linecolor, const QColor &surfcolor,
                          qreal l1, qreal l2, qreal bb, qreal hh,
                          DRAWINGSTYLE dstyle, bool simplified=false , bool isStart=true, bool isEnd=true);
    void createContouredClosedMember(const QColor& linecolor, const qreal ivalue, const qreal jvalue, const QList<qglviewer::Vec> vertexList,
                                     const QList<qglviewer::Vec> triangleList, DRAWINGSTYLE dstyle,PLANESUBMODEL iorh);

    void createClosedMember(const QColor& col1, const QColor& col2,
                            const QList<qglviewer::Vec> &plist, const QList<qglviewer::Vec> &tplist,
                            DRAWINGSTYLE dstyle );
    void drawHSteelSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawHSteelWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn ,bool isStart=true,bool isEnd=true);
    void drawISteelSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawISteelWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn ,bool isStart=true,bool isEnd=true);
    void drawBoxSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawBoxWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn ,bool isStart=true,bool isEnd=true);
    void drawPipeSolid( qreal l1, qreal l2, qreal dd, bool isColumn, bool simplified=false );
    void drawPipeWire( qreal l1, qreal l2, qreal dd, bool isColumn, bool simplified=false ,bool isStart=true,bool isEnd=true);
    void drawTSteelSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawTSteelWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn ,bool isStart=true,bool isEnd=true);
    void drawCSteelSolid( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn );
    void drawCSteelWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn ,bool isStart=true,bool isEnd=true);
    void drawCrossSolid( qreal l1, qreal l2, qreal bb, qreal hh,  bool isColumn );
    void drawCrossWire( qreal l1, qreal l2, qreal bb, qreal hh, bool isColumn ,bool isStart=true,bool isEnd=true);
    void drawArrow(JointData* jd, qreal scale, bool drawValue, int dof, int fontsize);
    void drawText(XYZ pos, QFont font, QColor color, QString str, TEXTPOS pos2);
//    void drawText2(XYZ pos, QFont font, QColor color, QString str, bool centering=false);

    void drawYieldPoint(qreal center,qreal radius,QColor color=Qt::black,bool isColumn=false);
    void drawAxialYield(qreal start,qreal end,bool isTension=true);
    void drawShearCrackMark(qreal center,bool isColumn=false);
    void drawShearYieldMark(qreal center,bool isColumn=false);

    void addSphere(XYZ pos,qreal radius,QColor color);
};
}
#endif
