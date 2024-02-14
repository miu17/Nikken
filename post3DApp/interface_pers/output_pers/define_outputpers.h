#ifndef DEFINEDDATA_H
#define DEFINEDDATA_H

#include <QObject>
#include <QStringList>
#include <QBrush>
#include <QFont>
#include <QList>

#include <QDebug>
#include "qmath.h"
//#include "member_force.h"

#include "define_unifieddata.h"
#include "unified_data.h"
namespace post3dapp{

//enum SECTIONTYPE{COLUMN=0,GIRDER,BRACE,WALL};
//enum DRAWSHAPE{DRAWSHAPE_BOX=0,DRAWSHAPE_H,DRAWSHAPE_I,DRAWSHAPE_PIPE,DRAWSHAPE_T,
//               DRAWSHAPE_C,DRAWSHAPE_RECTANGLE,DRAWSHAPE_CIRCLE,DRAWSHAPE_CROSSH,DRAWSHAPE_CROSS};
//enum STRUCTURE{STRUCTURE_S=0,STRUCTURE_RC,STRUCTURE_SRC,STRUCTURE_CFT};
//enum DIRECTION{DIRECTION_Y=0,DIRECTION_Z};
//enum CAMERATYPE{ORTHOGRAPHIC=0,PERSPECTIVE};

///* 断面データ */

//struct SECTIONDIM
//{

//    explicit SECTIONDIM(
//            const QString & _name=QString(),
//            SECTIONTYPE _type=COLUMN,
//            DRAWSHAPE _shape=DRAWSHAPE_BOX,
//            STRUCTURE _structure=STRUCTURE_S,
//            qreal _sizeH=0.0,
//            qreal _sizeB=0.0,
//            qreal _size1_T1=0.0,
//            qreal _size1_T2=0.0,
//            const QColor & _line= Qt::black,
//            const QColor & _paint= Qt::lightGray,
//            qreal _sizeH2=0.0,
//            qreal _sizeB2=0.0,
//            qreal _size2_T1=0.0,
//            qreal _size2_T2=0.0,
//            qreal _area=0.0
//            )
//        : sectionName(_name),memberType(_type),drawingShape(_shape),
//          structureType(_structure),sizeH(_sizeH),sizeB(_sizeB),size1_T1(_size1_T1),size1_T2(_size1_T2),
//          sizeH2(_sizeH2),sizeB2(_sizeB2),size2_T1(_size2_T1),size2_T2(_size2_T2),area(_area),lineColor(_line),paintColor(_paint)
//          { }

//    qreal danmenseki_S(){
//        //鉄骨 SRC
//        if(structureType==STRUCTURE_S || structureType==STRUCTURE_SRC){
//            //※該当しない方向の寸法は0が入っていると思っている
//            qreal y_af=size1_T2*sizeB;
//            qreal y_aw=size1_T1*sizeH;
//            qreal y_lap=size1_T1*size1_T2;
//            qreal z_af=size2_T2*sizeB2;
//            qreal z_aw=size2_T1*sizeH2;
//            qreal z_lap=size2_T1*size2_T2;
//            qreal yz_lap=size1_T1*size2_T1;
//            if(drawingShape==DRAWSHAPE_H || drawingShape==DRAWSHAPE_CROSSH){
//                return 2.0*y_af+y_aw + 2.0*z_af+z_aw  - 2.0*y_lap - 2.0*z_lap;
//            }else if(drawingShape==DRAWSHAPE_BOX){
//                return 2.0*y_af+2.0*y_aw - 4.0*y_lap;
//            }else if(drawingShape==DRAWSHAPE_CIRCLE){
//                return 3.1415926*sizeH*sizeH/4.0 - 3.1415926*(sizeH-2.0*size1_T1)*(sizeH-2.0*size1_T1)/4.0;
//            }else if(drawingShape==DRAWSHAPE_T){
//                return y_af+y_aw + z_af+z_aw - 1.0*y_lap - 1.0*z_lap;
//            }else if(drawingShape==DRAWSHAPE_C){
//                return 2.0*y_af+y_aw + 2.0*z_af+z_aw - 2.0*y_lap - 2.0*z_lap;
//            }else if(drawingShape==DRAWSHAPE_CROSS){
//                return y_aw + z_aw - 1.0*yz_lap;
//            }
//            return 0.0;
//        }
//        //コンクリート
//        if(structureType==STRUCTURE_RC){
//            return 0.0;
//        }
//        //CFT
//        if(structureType==STRUCTURE_CFT){
//            if(drawingShape==DRAWSHAPE_CIRCLE){
//                return 3.1415926*sizeH*sizeH/4.0 - 3.1415926*(sizeH-2.0*size1_T1)*(sizeH-2.0*size1_T1)/4.0;
//            }
//            if(drawingShape==DRAWSHAPE_RECTANGLE){
//                return sizeH*sizeB - (sizeH-2.0*size1_T2)*(sizeB-2.0*size1_T1);
//            }
//            return 0.0;
//        }
//        return 0.0;

//        //全断面積？フランジ、ウェブ、X方向、Y方向、鉄骨、コンクリート？
//    }
//    qreal danmenseki_C(){
//        //鉄骨
//        if(structureType==STRUCTURE_S){
//            return 0.0;
//        }
//        //コンクリート
//        if(structureType==STRUCTURE_RC){
//            if(drawingShape==DRAWSHAPE_CIRCLE){
//                return 3.1415926*sizeH2*sizeH2/4.0;
//            }
//            if(drawingShape==DRAWSHAPE_RECTANGLE){
//                return sizeH2*sizeB2;
//            }
//            return 0.0;
//        }
//        //SRC
//        if(structureType==STRUCTURE_SRC){
//            if(drawingShape==DRAWSHAPE_CIRCLE){
//                qreal area = 3.1415926*sizeH2*sizeH2/4.0;
//                return area-danmenseki_S();
//            }
//            if(drawingShape==DRAWSHAPE_RECTANGLE){
//                qreal area = sizeH2*sizeB2;
//                return area-danmenseki_S();
//            }
//            return 0.0;
//        }

//        //CFT
//        if(structureType==STRUCTURE_CFT){
//            if(drawingShape==DRAWSHAPE_CIRCLE){
//                return 3.1415926*(sizeH-2.0*size1_T1)*(sizeH-2.0*size1_T1)/4.0;
//            }
//            if(drawingShape==DRAWSHAPE_RECTANGLE){
//                return (sizeH-2.0*size1_T2)*(sizeB-2.0*size1_T1);
//            }
//            return 0.0;
//        }
//        return 0.0;

//        //全断面積？フランジ、ウェブ、X方向、Y方向、鉄骨、コンクリート？
//    }
//    qreal danmenseki(){
//        return danmenseki_C()+danmenseki_S();
//    }
//    QString sectionName;
//    SECTIONTYPE memberType;
//    DRAWSHAPE drawingShape;
//    STRUCTURE structureType;
//    qreal sizeH;
//    qreal sizeB;
//    qreal size1_T1;
//    qreal size1_T2;
//    qreal sizeH2;
//    qreal sizeB2;
//    qreal size2_T1;
//    qreal size2_T2;
//    QColor lineColor;
//    QColor paintColor;
//    qreal area;

//};


///* XYZ */

//struct visual_XYZ
//{
//    explicit visual_XYZ( qreal _x=0.0, qreal _y=0.0, qreal _z=0.0 )
//        : x(_x),y(_y),z(_z){ }

//    qreal x;
//    qreal y;
//    qreal z;
//    visual_XYZ operator+(const visual_XYZ& other) const{return visual_XYZ(x+other.x,y+other.y,z+other.z);}
//    visual_XYZ operator*(const qreal& coef) const{return visual_XYZ(x*coef,y*coef,z*coef);}
//    //qgl::vecとQVector3DとQPointFとXYZが混在。使う段階でCASTしてXYZは最低限で良いか
//};


//class ddFloor
//{
//public:
//    enum FLTYPE{CHILD=0,UNDERGROUND,OVERGROUND,PENTHOUSE};
//    explicit ddFloor(QString _name=QString(),qreal _height=0.0,FLTYPE _type=OVERGROUND):name(_name),height(_height),type(_type){}
//    QString name;
//    qreal height;
//    FLTYPE type;
//    bool operator==(const ddFloor& other) const { return (name==other.name && height==other.height); }
//    bool operator<(const ddFloor& other) const { return (height<other.height); }
//};
//class ddFrame
//{//Frameに属するデータとFrameGroupに属するデータが混在しているので別ける？
//public:
//    enum FRTYPE{STRAIGHT=0,CIRCLE,RADIUS,UNKNOWN};
//    explicit ddFrame(QString _name=QString(),qreal _coordinate=0.0,int _gridgroup=1,
//            FRTYPE _type=STRAIGHT,int _count=0,QPointF _base=QPointF(),qreal _ths=0.0,qreal _the=0.0,qreal _span=0.0)
//        :name(_name),coordinate(_coordinate),gridgroup(_gridgroup),type(_type),count(_count),basePoint(_base),
//        thetaStart(_ths),thetaEnd(_the),nextSpan(_span){}
//    QString name;
//    qreal coordinate;
//    int gridgroup;
//    FRTYPE type;
//    int count;//所属通り数
//    QPointF basePoint;
//    qreal thetaStart;
//    qreal thetaEnd;
//    qreal nextSpan;

//    bool operator==(const ddFrame& other) const {
//        if (name==other.name && gridgroup==other.gridgroup && coordinate==other.coordinate){return true;}
//        else{return false;}
//    }
//    bool operator<(const ddFrame& other) const {
//        if (gridgroup<other.gridgroup){
//            return true;
//        }else if(gridgroup>other.gridgroup){
//            return false;
//        }else if(coordinate<other.coordinate){
//            return true;
//        }else{return false;}
//    }
//};

///* 部材データ */

//class visual_JointData;
//class MemberStress;
//class OptionalMemberData
//{
//public:
//    enum IWALL_OR_HWALL{IWALL=0,HWALL};
//    enum YIELD_STATE{NOT=0,YIELD,HINGE,SHEAR_CRACK,SHEAR_YIELD,AXIAL_YIELD};
//    OptionalMemberData( int _openingNo=-1, bool _isStructuralWall=false, IWALL_OR_HWALL _walldirection=IWALL)
//        :wallDirection(_walldirection),isStructuralWall(_isStructuralWall),openingNo(_openingNo){}
//    IWALL_OR_HWALL wallDirection;
//    bool isStructuralWall;
//    int openingNo;
//    QHash<QString,QList<QList<YIELD_STATE> > > yieldstate;//key=stresscase,list=[ijc][step]
//};

//class visual_MemberData
//{
//public:
//    visual_MemberData( int _no,const QList<visual_JointData*>& jdlist, SECTIONDIM* section, qreal ag=0.0 ,OptionalMemberData _option=OptionalMemberData());
//    ~visual_MemberData();

//    SECTIONDIM* getSection() const { return section; }
//    qreal getCodeAngle() const { return codeAngle; }
//    QList<visual_JointData*> getJointList() const { return jointList; }
//    int getOpeningNo()const{return option.openingNo;}

//    void removeJoint(visual_JointData *jd){ if( jointList.contains(jd) ) jointList.removeAll(jd) ;}

//    visual_JointData* getIJoint() const{ return jointList.first(); }
//    visual_JointData* getJJoint() const{ return jointList.last(); }

//    visual_XYZ ixyz()const;
//    visual_XYZ jxyz()const;

//    qreal getMinX() const;
//    qreal getMinY() const;
//    qreal getMinZ() const;
//    qreal getMaxX() const;
//    qreal getMaxY() const;
//    qreal getMaxZ() const;

//    qreal getFaceI() const{return faceLength.at(0);}
//    qreal getFaceJ() const{return faceLength.at(1);}
//    qreal getTotalLength() const{
//        return sqrt((ixyz().x-jxyz().x)*(ixyz().x-jxyz().x)
//                    +(ixyz().y-jxyz().y)*(ixyz().y-jxyz().y)
//                    +(ixyz().z-jxyz().z)*(ixyz().z-jxyz().z));}

//    bool belongsListedFloorFully(const QStringList& namelist) const;
//    bool belongsListedFrameFully(const QStringList& namelist) const;
//    bool belongsListedFloorPartly(const QStringList& namelist) const;
//    bool belongsListedFramePartly(const QStringList& namelist) const;

//    void setFaceMemberForce(const QString loadcase,const QList<MemberForce>& st){
//        facesectionalforce[loadcase]=st;
//    }
//    void setNodeMemberForce(const QString loadcase,const QList<MemberForce>& st){
//        nodesectionalforce[loadcase]=st;
//    }
//    MemberForce getFaceMemberForce(QString loadcase,int step=0){
//        //stepがjointdisp[loadcase].countより大のとき記述
////        qDebug()<<"getFaceMemberForce"<<loadcase<<step;
//        if(loadcase.isEmpty() || step==0){
//            return MemberForce();
//        }else if(!facesectionalforce.contains(loadcase)){
////            qDebug()<<"not contain";
//            return MemberForce();
//        }else if(step>facesectionalforce[loadcase].count()){
////            qDebug()<<"step=0 facesectionalforce.count="<<facesectionalforce[loadcase].count();
//            return facesectionalforce[loadcase].last();
//        }else{
//            return facesectionalforce[loadcase][step-1];
//        }
//    }
//    MemberForce getNodeMemberForce(QString loadcase,int step=0){
//        if(loadcase.isEmpty() || step==0){
//            return MemberForce();
//        }else if(!nodesectionalforce.contains(loadcase)){
//            return MemberForce();
//        }else if(step>nodesectionalforce[loadcase].count()){
//            return nodesectionalforce[loadcase].last();
//        }else{
//            return nodesectionalforce[loadcase][step-1];
//        }
//    }
//    MemberForce getCombinationFaceMemberForce(QList<qreal> coef,QStringList loadcase,QList<int> step){
//        int numcase=qMin(coef.count(),loadcase.count());
//        MemberForce combiforce;
//        for(int i=0;i<numcase;i++){
//            combiforce=combiforce+getFaceMemberForce(loadcase.at(i),step.at(i))*coef.at(i);
//        }
//        return combiforce;
//    }
//    MemberForce getCombinationNodeMemberForce(QList<qreal> coef,QStringList loadcase,QList<int> step){
//        int numcase=qMin(coef.count(),loadcase.count());
//        MemberForce combiforce;
//        for(int i=0;i<numcase;i++){
//            combiforce=combiforce+getNodeMemberForce(loadcase.at(i),step.at(i))*coef.at(i);
//        }
//        return combiforce;
//    }
//    QList<OptionalMemberData::YIELD_STATE> getYieldState(QString loadcase,int step=0){
//        if(!option.yieldstate.contains(loadcase) || step==0){
//            QList<OptionalMemberData::YIELD_STATE> ystate;
//            ystate<<OptionalMemberData::NOT<<OptionalMemberData::NOT<<OptionalMemberData::NOT;
//            return ystate;
//        }else if(step>option.yieldstate[loadcase].count()){return option.yieldstate[loadcase].last();}
//        else{return option.yieldstate[loadcase][step-1];}
//    }
//    OptionalMemberData option;
//    QList<qreal> faceLength;
//    int no(){return NO;}

//        qreal eiy;
//        qreal eiz;
//        qreal gay;
//        qreal gaz;
//        qreal ea;
//        qreal gj;
//        qreal ix_rigid;
//        qreal iy_rigid;
//        qreal iz_rigid;
//        qreal jx_rigid;
//        qreal jy_rigid;
//        qreal jz_rigid;


//private:
//    int NO;//0から始まる
//    QList<visual_JointData*> jointList;
//    SECTIONDIM* section;
//    qreal codeAngle;
//    QHash<QString,QList<MemberForce> > facesectionalforce;
//    QHash<QString,QList<MemberForce> > nodesectionalforce;
//};


///* 節点データ */

//class visual_JointData
//{
//public:
//    visual_JointData( int no,visual_XYZ v, const ddFloor & fl=ddFloor(), const QList<ddFrame> & fr=QList<ddFrame>() )
//        : NO(no),jointXYZ(v),floor(fl),frame(fr) { }
//    ~visual_JointData(){
//        Q_FOREACH(visual_MemberData *md, memberList) md->removeJoint(this);
//    }

//    void appendMD(visual_MemberData* md){ if( !memberList.contains(md) ) memberList.append(md); }
//    void removeMD(visual_MemberData* md){
//        if( memberList.contains(md) ){
//            md->removeJoint(this);
//            memberList.removeAll(md);
//        }
//    }
//    bool containsMD(visual_MemberData* md) const { return memberList.contains(md); }
//    QList<visual_MemberData*> getMemberList() const { return memberList; }
//    void setJointDisp(const QString loadcase,const QList<visual_XYZ> jd){jointDisp[loadcase]=jd;}
//    visual_XYZ getJointDisp(const QString loadcase,int step=0) const{
//        //stepがjointdisp[loadcase].countより大のとき記述
//        if(loadcase.isEmpty() || step==0){return visual_XYZ(0.0,0.0,0.0);}
//        else if(!jointDisp.contains(loadcase)){
//            return visual_XYZ(0.0,0.0,0.0);
//        }else if(step>jointDisp[loadcase].count()){
//            return jointDisp[loadcase].last();
//        }else{
//            return jointDisp[loadcase][step-1];
//        }
//    }
//    visual_XYZ getCombinationJointDisp(const QStringList& loadcase,const QList<qreal>& coef,const QList<int>& step){
//        visual_XYZ disp;
//        int numcase=qMin(loadcase.count(),coef.count());
//        for(int i=0;i<numcase;i++){
//            if(loadcase.at(i).isEmpty())continue;
//            disp=disp+getJointDisp(loadcase.at(i),step.at(i))*coef.at(i);
//        }
//    }

//    void setReaction(const QString loadcase,const QList<QList<qreal> >& _react){react[loadcase]=_react;}
//    QList<qreal> getReaction(const QString loadcase,int step=0){
//        if(loadcase.isEmpty() || !react.contains(loadcase) || step==0){
//            QList<qreal> zerolist;
//            zerolist<<0.0<<0.0<<0.0<<0.0<<0.0<<0.0;
//            return zerolist;
//        }else if(step>react[loadcase].count()){
//            return react[loadcase].last();
//        }else{
//            return  react[loadcase][step-1];
//        }
//    }
//    QList<qreal> getCombinationReaction(const QStringList& loadcase,const QList<qreal>& coef,const QList<int> &step){
//        qDebug()<<loadcase;
//        int mxdof=6;
//        int numcase=qMin(loadcase.count(),coef.count());
//        QList<qreal> combiReaction=getReaction(QString());
//        for(int i=0;i<numcase;i++){
//            if(loadcase.at(i).isEmpty())continue;
//             QList<qreal> reaction=getReaction(loadcase.at(i),step.at(i));
//             for(int dof=0;dof<mxdof;dof++){
//                combiReaction[dof]+=coef.at(i)*reaction.at(dof);
//             }
//        }
//        return combiReaction;
//    }

//    bool belongsListedFloor(const QStringList& namelist) const;
//    bool belongsListedFrame(const QStringList& namelist) const;

//    visual_XYZ xyz() const { return jointXYZ; }
//    qreal xPos() const{ return jointXYZ.x; }
//    qreal yPos() const{ return  jointXYZ.y; }
//    qreal zPos() const{ return jointXYZ.z; }
//    qreal xDisp(QString loadcase,int step=0) const{ return (step==0 || step>jointDisp[loadcase].count()) ? jointDisp[loadcase].last().x : jointDisp[loadcase][step-1].x; }
//    qreal yDisp(QString loadcase,int step=0) const{ return (step==0 || step>jointDisp[loadcase].count()) ? jointDisp[loadcase].last().y : jointDisp[loadcase][step-1].y; }
//    qreal zDisp(QString loadcase,int step=0) const{ return (step==0 || step>jointDisp[loadcase].count()) ? jointDisp[loadcase].last().z : jointDisp[loadcase][step-1].z; }
//    ddFloor getFloor() const{return floor;}
//    QList<ddFrame> getFrame()const{return frame;}

//    int no() const { return NO; }

//    void setParentJoint( visual_JointData* jd ){ jd->appendChildJoint(this);  }
//    void appendChildJoint( visual_JointData* jd );
//    QHash<visual_JointData*,QPointF> getChildJoints() const{ return childJoints; }

//private:
//    int NO;//0から始まる
//    QList<visual_MemberData*> memberList;
//    visual_XYZ jointXYZ;
//    QHash<QString,QList<visual_XYZ> > jointDisp;//stepのlist
//    QHash<QString,QList<QList<qreal> > >react;//react[loadcase][step][dof]
//    ddFloor floor;
//    QList<ddFrame> frame;
//    QHash<visual_JointData*,QPointF> childJoints;
//};

///*開口データ*/
//class Opening{
//public:
//    // enum OPENINGTYPE{SHAPE=0,ANALYTICAL};->3Dのどこで定義しているか確認
//    enum LEFT_OR_RIGHT{WFULL=0,LEFT,RIGHT,CENTER};
//    enum TOP_OR_BOTTOM{HFULL=0,BOTTOM,TOP,MIDDLE};
//    // OPENINGTYPE type;
//    Opening(){}
//    Opening(LEFT_OR_RIGHT _lr,TOP_OR_BOTTOM _tb,qreal _px,qreal _py,qreal _w,qreal _h)
//        :lr(_lr),tb(_tb),positionx(_px),positiony(_py),width(_w),height(_h){}
////    ~Opening();
//    LEFT_OR_RIGHT lr;
//    TOP_OR_BOTTOM tb;
//    qreal positionx;
//    qreal positiony;
//    qreal width;
//    qreal height;
//};
////QList<OpeningSet*> OpeningList;
////OpeningSet* opset;
//struct OpeningSet{//OpeningList.at(int)でlabelと開口の組を得られるようにしたい
//public:
//    OpeningSet(){}
//    OpeningSet(QString _label,const QList<Opening*>& _openings):label(_label),openings(_openings){}
////    ~OpeningSet();
//    QString label;
//    QList<Opening*> openings;
//};

struct ValueMinMax{
public:
    ValueMinMax(){
        AllMax["All"]=0.0;AllMax["N"]=0.0;AllMax["Qy"]=0.0;AllMax["Qz"]=0.0;
        AllMax["Mx"]=0.0;AllMax["My"]=0.0;AllMax["Mz"]=0.0;
        AllMin["All"]=0.0;AllMin["N"]=0.0;AllMin["Qy"]=0.0;AllMin["Qz"]=0.0;
        AllMin["Mx"]=0.0;AllMin["My"]=0.0;AllMin["Mz"]=0.0;
        DisplayMax["All"]=0.0;DisplayMax["N"]=0.0;DisplayMax["Qy"]=0.0;DisplayMax["Qz"]=0.0;
        DisplayMax["Mx"]=0.0;DisplayMax["My"]=0.0;DisplayMax["Mz"]=0.0;
        DisplayMin["All"]=0.0;DisplayMin["N"]=0.0;DisplayMin["Qy"]=0.0;DisplayMin["Qz"]=0.0;
        DisplayMin["Mx"]=0.0;DisplayMin["My"]=0.0;DisplayMin["Mz"]=0.0;
    }
    QHash<QString,qreal> AllMax;
    QHash<QString,qreal> AllMin;
    QHash<QString,qreal> DisplayMax;
    QHash<QString,qreal> DisplayMin;
};


///* ウィジェットの状態 */
struct MODELSETTINGS
{
    explicit MODELSETTINGS(
            QString _title=QString(),
            int _style=2,
            bool _drawContour=true,
            bool _drawN=false,
            bool _drawM=true,
            bool _drawQ=false,
            bool _drawNumber=false,
            bool _drawDisp=false,
            bool _drawReact=false,
            bool _drawMemberLabel=true,
            bool _drawYield=false,
            bool _drawLegend=true,


            int _flck=0,
            const QStringList &_floors=QStringList(),
            int _frck=0,
            const QStringList &_frames=QStringList(),
            const QStringList &_section=QStringList(),

            XYZ _pcam=XYZ(10.0,10.0,10.0),
            XYZ _ucam=XYZ(0.0,0.0,1.0),
            XYZ _dcam=XYZ(0.0,1.0,0.0)

            )
        : title(_title),drawStyle(_style),drawContour(_drawContour),
          drawN(_drawN),drawM(_drawM),drawQ(_drawQ),
          drawNumber(_drawNumber),drawDisp(_drawDisp),drawReact(_drawReact),
          drawMemberLabel(_drawMemberLabel),drawYield(_drawYield),drawLegend(_drawLegend),
          limitedFloorID(_flck),limitedFloors(_floors),
          limitedFrameID(_frck),limitedFrames(_frames),limitedSection(_section),
          cameraPos(_pcam),cameraUpVec(_ucam),cameraDir(_dcam){}
    //ウィジェット内で設定（扱いを変更するものもあり）
//    QStringList loadCase;
//    QList<qreal> coefload;
//    QList<int> loadStep;
//    QList<int> loadMaxStep;
    QString title;
//    QString filename;
    int drawStyle;//ポリゴン
//    bool drawGraph;
    bool drawContour;
//    bool drawReactNumber;
//    bool drawNumberLabel;
//    bool drawDispNumber;
//    int selectDisp;
//    int selectReact;
    bool drawN;
    bool drawM;
    bool drawQ;
    bool drawNumber;
    bool drawDisp;
    bool drawReact;
    bool drawMemberLabel;
    bool drawYield;
    bool drawLegend;

//    QHash<QString,QList<bool>> selectForce;

    //範囲設定
    int limitedFloorID;
    QStringList limitedFloors;
    int limitedFrameID;
    QStringList limitedFrames;
    QStringList limitedSection;

    //カメラ設定
    XYZ cameraPos;
    XYZ cameraUpVec;
    XYZ cameraDir;

    //個別設定：一般

    //Analysis3DSettingsへ移動
//    QColor backColor;
//    int lineWidth;
//    qreal graphScale;
//    bool drawLegend;
//    bool drawFileName;
//    int fontSize;
//    bool drawMemberLabel;
//    bool drawCoordinate;
//    bool drawLogo;
//    bool drawPoint;
//    bool drawZWall;
//    bool drawOpening;
//    bool drawYield;
//    bool smoothing;
//    bool shade;
//    int selectDrawContour;
//    qreal manualLegendMin;
//    qreal manualLegendMax;
//    int selectGraphScale;
//    int renderType;//アイソメ
//    int nodeOrFace;
//    int valueCalcMethod;
//    qreal reactScale;
//    qreal drawRatioXYZ;

};
//class ModeData
//{
//public:
//    enum MODE_TYPE{MODE=0,BETAX_U,BETAY_U};
//    ModeData(){}
//    ModeData(QList<QList<visual_XYZ> > _mode){
//        mode=_mode;
//        calcStandardizedMode();
//    }
//    void setModeData(QList<QList<visual_XYZ> > _mode){
//        mode=_mode;
//        calcStandardizedMode();
//    }
//    int nummode;//正の整数
//    int numj;//正の整数
//    QList<qreal> naturalPeriod;//size=mode数
//    QList<QList<qreal> > beta;//X Y Z rx ry rz * mode数
//    QList<qreal> totalMass;//X Y Z rz ry rz
//    QList<QList<qreal> > effectiveMassRatio;//beta*beta/totalmass
//    QList<QList<visual_XYZ> > mode; //mode[nummode][jno]
//    QList<QList<visual_XYZ> > standardizedMode;
//    visual_XYZ getModeDisp(MODE_TYPE type,int jno,int nmode,qreal dispratio=1,qreal time=0.0,qreal timespeedratio=1.0)
//    {
////        qDebug()<<"getModeDisp"<<type<<jno<<nmode<<dispratio<<time<<timespeedratio;
////        qDebug()<<mode.count()<<mode.at(0).count()<<beta.count()<<naturalPeriod.count();
////        qDebug()<<beta.at(0);
////        return XYZ();
//        if(nmode<=0 || nmode>nummode)return visual_XYZ();//nmodeは正の整数
//        visual_XYZ basedisp;
//        if(type==MODE){basedisp=standardizedMode.at(nmode-1).at(jno);}
//        else if(type==BETAX_U){basedisp=mode.at(nmode-1).at(jno)*beta.at(nmode-1).at(0);}
//        else if(type==BETAY_U){basedisp=mode.at(nmode-1).at(jno)*beta.at(nmode-1).at(1);}
////        return XYZ();
//        visual_XYZ disp = basedisp*dispratio*qCos(2.0*M_PI*qreal(time)*qreal(timespeedratio)/qreal(naturalPeriod.at(nmode-1)));
//        return disp;
//    }
//    //最大最小値の絶対値が1になるように倍率をかける
//    void calcStandardizedMode(){
//        standardizedMode.clear();
//        for(int i=0;i<nummode;i++){
//            qreal min=0.0;
//            qreal max=0.0;
//            QList<visual_XYZ> xyzlist;
//            for(int j=0;j<numj;j++){
//                if(mode.at(i).at(j).x<min)min=mode.at(i).at(j).x;
//                if(mode.at(i).at(j).y<min)min=mode.at(i).at(j).y;
//                if(mode.at(i).at(j).z<min)min=mode.at(i).at(j).z;
//                if(mode.at(i).at(j).x>max)max=mode.at(i).at(j).x;
//                if(mode.at(i).at(j).y>max)max=mode.at(i).at(j).y;
//                if(mode.at(i).at(j).z>max)max=mode.at(i).at(j).z;
//            }
//            qreal coef=qMax(qAbs(min),qAbs(max));
//            for(int j=0;j<numj;j++){
//                if(coef==0.0)xyzlist<<visual_XYZ(0.0,0.0,0.0);
//                else{xyzlist<<visual_XYZ(mode.at(i).at(j).x/coef,mode.at(i).at(j).y/coef,mode.at(i).at(j).z/coef);}
//            }
//            standardizedMode<<xyzlist;
//        }
//    }
//    void calcEffectiveMassRatio(){
//        effectiveMassRatio.clear();
//        QList<qreal> emratio;
//        for(int i=0;i<nummode;i++){
//            emratio.clear();
//            for(int j=0;j<6;j++){
//                if(totalMass.at(j)==0.0){emratio<<0.0;}
//                else{emratio<<beta.at(i).at(j)*beta.at(i).at(j)/totalMass.at(j)*100.0;}
//            }
//            effectiveMassRatio<<emratio;
//        }
//    }
//    void clear(){
//        naturalPeriod.clear();beta.clear();totalMass.clear();effectiveMassRatio.clear();
//        mode.clear();standardizedMode.clear();
//    }
//};
//struct ChckData{
//    qint32 numch;
//    QStringList namech;
//    QList<QList<QList<float> > > allowanceRateBending;//曲げ余裕率[numch][numm+numww][icj]
//    QList<QList<QList<float> > > verificationRatioShear;//せん断余裕率（SRCの場合はRC部分）[numch][numm+numww][icj]
//    QList<QList<QList<float> > > verificationRatioShear2;//せん断余裕率(SRC部材のS部分,RC部材の付着)[numch][numm+numww][icj]
//    QList<QList<QList<float> > > verificationRatioBending;//検定比[numch][numm+numww][icj]
//    void clear(){namech.clear();allowanceRateBending.clear();verificationRatioBending.clear();}
//};

//struct CHCK_MODELSETTINGS
//{
//    enum VALUETYPE{ALLOWANCERATEBENDING=0,VERIFICATIONRATIOBENDING,separate1,VERIFICATIONRATIOSHEAR,separate2,VERIFICATIONRATIOMAX};
//    enum SRCSHEARALLOWANCE{SRC_MAX=0,SRC_S,SRC_RC};
//    explicit CHCK_MODELSETTINGS(
//            QString _filename=QString(),
//            QString _combiloadcase=QString(),
//            int _numcombiloadcase=0,
//            int _valuetype=0,
//            int _drawstyle=0,
//            bool _drawcontour=true,
//            qreal _legendmin=0.0,
//            qreal _legendmax=1.0,
//            qreal _legendmask=0.0,
//            int _srchandling=0,
//            bool _drawnumberlabel=false,
//            int _flck=0,
//            const QStringList &_floors=QStringList(),
//            int _frck=0,
//            const QStringList &_frames=QStringList(),
//            const QStringList &_section=QStringList(),
//            bool _drawlegend=true,
//            bool _drawfilename=true,
//            bool _drawpoint=false,
//            bool _drawmemberlabel=false,
//            int _fontsize=3,
//            bool _shade=false,
//            int _renderType=0,
//            bool _smoothing=false)
//        : fileName(_filename),backColor(Qt::white),lineWidth(1),combiLoadCase(_combiloadcase),
//          numcombiLoadCase(_numcombiloadcase),valueType((VALUETYPE)_valuetype),drawStyle(_drawstyle),
//          drawContour(_drawcontour),legendMin(_legendmin),legendMax(_legendmax),legendMask(_legendmask),
//          srcHandling((SRCSHEARALLOWANCE)_srchandling),drawNumberLabel(_drawnumberlabel),
//          limitedFloorID(_flck),limitedFloors(_floors),
//          limitedFrameID(_frck),limitedFrames(_frames),limitedSection(_section),

//          drawLegend(_drawlegend),drawFileName(_drawfilename),drawPoint(_drawpoint),
//          drawMemberLabel(_drawmemberlabel),fontSize(_fontsize),shade(_shade),renderType(_renderType),smoothing(_smoothing)
//          {}
//    QString fileName;
//    QColor backColor;
//    int lineWidth;
//    QString combiLoadCase;
//    int numcombiLoadCase;
//    VALUETYPE valueType;
//    int drawStyle;
//    bool drawContour;
//    qreal legendMin;
//    qreal legendMax;
//    qreal legendMask;
//    SRCSHEARALLOWANCE srcHandling;
//    bool drawNumberLabel;
//    int limitedFloorID;
//    QStringList limitedFloors;
//    int limitedFrameID;
//    QStringList limitedFrames;
//    QStringList limitedSection;
//    bool drawLegend;
//    bool drawFileName;
//    bool drawPoint;
//    bool drawMemberLabel;
//    int fontSize;
//    bool shade;
//    int renderType;
//    bool smoothing;
//};
//struct MODE_MODELSETTINGS
//{
//    explicit MODE_MODELSETTINGS(
//            QString _filename=QString(),
//            int _modenumber=1,
//            int _modetype=0,
//            bool _drawlegend=true,
//            bool _draworiginalmodel=true,
//            bool _drawfilename=true,
//            int _drawstyle=1,
//            qreal _dispmagnitude=3.0,
//            qreal _animationspeed=1.0,
//            visual_XYZ _pcam=visual_XYZ(10.0,10.0,10.0),
//            visual_XYZ _ucam=visual_XYZ(0.0,0.0,1.0),
//            visual_XYZ _dcam=visual_XYZ(0.0,1.0,0.0),
//    int _flck=0,
//    const QStringList &_floors=QStringList(),
//    int _frck=0,
//    const QStringList &_frames=QStringList(),
//    const QStringList &_section=QStringList())

//        : fileName(_filename),backColor(Qt::white),lineWidth(2),modeNumber(_modenumber),modeType(static_cast<ModeData::MODE_TYPE>(_modetype)),drawLegend(_drawlegend),//drawAnimation(_drawanimation),
//          drawOriginalModel(_draworiginalmodel),drawFileName(_drawfilename),drawStyle(_drawstyle),
//          dispMagnitude(_dispmagnitude),animationSpeed(_animationspeed),
//          cameraPos(_pcam),cameraUpVec(_ucam),cameraDir(_dcam), limitedFloorID(_flck),limitedFloors(_floors),
//          limitedFrameID(_frck),limitedFrames(_frames),limitedSection(_section){}
//    QString fileName;
//    QColor backColor;
//    int lineWidth;
//    int modeNumber;
//    ModeData::MODE_TYPE modeType;
//    bool drawLegend;
////    bool drawAnimation;
//    bool drawOriginalModel;
//    bool drawFileName;
//    int drawStyle;
//    qreal dispMagnitude;
//    qreal animationSpeed;
//    visual_XYZ cameraPos;
//    visual_XYZ cameraUpVec;
//    visual_XYZ cameraDir;
//    int limitedFloorID;
//    QStringList limitedFloors;
//    int limitedFrameID;
//    QStringList limitedFrames;
//    QStringList limitedSection;
//};
//struct POS5_MODELSETTINGS
//{
//    explicit POS5_MODELSETTINGS(
//            QString _filename=QString(),
//            int _ratio=100,
//            int _selectdisp=1,
//            QStringList _loadcase=QStringList(),
//            QList<qreal> _coefload=QList<qreal>(),
//            QList<int> _loadstep=QList<int>(),
//            QList<int> _loadmaxstep=QList<int>(),
//            QStringList _loadcase2=QStringList(),
//            QList<qreal> _coefload2=QList<qreal>(),
//            QList<int> _loadstep2=QList<int>(),
//            QList<int> _loadmaxstep2=QList<int>(),
//            QString _virtualloadcase=QString(),
//            QString _virtualloadcase2=QString(),
//            int _cont_or_opt=0,
//            int _kiyomode=0,
//            int _optimode=0,
//            int _optimode2=0,
//            qreal _controldisp=1.0,
//            int _syuukeiflck=0,
//            const QStringList &_syuukeifloors=QStringList(),
//            int _syuukeifrck=0,
//            const QStringList &_syuukeiframes=QStringList(),
//            const QStringList &_syuukeisection=QStringList(),
//            qreal _bairitsu=10.0,
//            bool _drawnumberlabel=true,
//            int _flck=0,
//            const QStringList &_floors=QStringList(),
//            int _frck=0,
//            const QStringList &_frames=QStringList(),
//            const QStringList &_section=QStringList(),
//            bool _drawcolumny = true,
//            bool _drawcolumnz = true,
//            bool _drawgirdery = false,
//            bool _drawgirderz = true,
//            bool _drawbracey = false,
//            bool _drawbracez = true,
//            bool _drawjiku = true,
//            bool _drawmage = true,
//            bool _drawsendan = true,
//            bool _drawnejire = false,
//            bool _drawlegend=true,
//            bool _drawfilename=true,
//            bool _drawpoint=false,
//            bool _drawmemberlabel=false,
//            int _fontsize=3,
//            bool _shade=false,
//            int _renderType=0,
//            bool _smoothing=false)
//        : fileName(_filename),drawRatioXYZ(_ratio),backColor(Qt::white),lineWidth(1),selectDisp(_selectdisp),
//          loadCase(_loadcase),coefload(_coefload),loadStep(_loadstep),loadMaxStep(_loadmaxstep),
//          loadCase2(_loadcase2),coefload2(_coefload2),loadStep2(_loadstep2),loadMaxStep2(_loadmaxstep2),
//          virtualLoadCase(_virtualloadcase),virtualLoadCase2(_virtualloadcase2),
//          cont_or_opt(_cont_or_opt),kiyoMode(_kiyomode),optiMode(_optimode),optiMode2(_optimode2),controlDisp(_controldisp),
//          syuukeiFloorID(_syuukeiflck),syuukeiFloors(_syuukeifloors),
//          syuukeiFrameID(_syuukeifrck),syuukeiFrames(_syuukeiframes),syuukeiSection(_syuukeisection),
//          bairitsu(_bairitsu),drawNumberLabel(_drawnumberlabel),
//          limitedFloorID(_flck),limitedFloors(_floors),
//          limitedFrameID(_frck),limitedFrames(_frames),limitedSection(_section),
//          drawColumnY(_drawcolumny),drawColumnZ(_drawcolumnz),drawGirderY(_drawgirdery),
//          drawGirderZ(_drawgirderz),drawBraceY(_drawbracey),drawBraceZ(_drawbracez),
//          drawJiku(_drawjiku),drawMage(_drawmage),drawSendan(_drawsendan),drawNejire(_drawnejire),
//          drawLegend(_drawlegend),drawFileName(_drawfilename),drawPoint(_drawpoint),
//          drawMemberLabel(_drawmemberlabel),fontSize(_fontsize),shade(_shade),renderType(_renderType),smoothing(_smoothing)
//          {
//        if(_loadcase.isEmpty()){loadCase.clear();loadCase<<QString()<<QString()<<QString()<<QString();}else{loadCase.clear();loadCase=_loadcase;}
//        if(_coefload.isEmpty()){coefload.clear();coefload<<1.0<<1.0<<1.0<<1.0;}else{coefload.clear();coefload=_coefload;qDebug()<<"MODELSETTINGS"<<coefload;}
//        if(_loadstep.isEmpty()){loadStep.clear();loadStep<<0<<0<<0<<0;}else{loadStep.clear();loadStep=_loadstep;}
//        if(_loadstep.isEmpty()){loadMaxStep.clear();loadMaxStep<<0<<0<<0<<0;}else{loadMaxStep.clear();loadMaxStep=_loadmaxstep;}
//        if(_loadcase2.isEmpty()){loadCase2.clear();loadCase2<<QString()<<QString()<<QString()<<QString();}else{loadCase2.clear();loadCase2=_loadcase2;}
//        if(_coefload2.isEmpty()){coefload2.clear();coefload2<<1.0<<1.0<<1.0<<1.0;}else{coefload2.clear();coefload2=_coefload2;qDebug()<<"MODELSETTINGS"<<coefload2;}
//        if(_loadstep2.isEmpty()){loadStep2.clear();loadStep2<<0<<0<<0<<0;}else{loadStep2.clear();loadStep2=_loadstep2;}
//        if(_loadstep2.isEmpty()){loadMaxStep2.clear();loadMaxStep2<<0<<0<<0<<0;}else{loadMaxStep2.clear();loadMaxStep2=_loadmaxstep2;}

//    }
//    QString fileName;
//    int drawRatioXYZ;
//    QColor backColor;
//    int lineWidth;
//    int selectDisp;
//    QStringList loadCase;
//    QList<qreal> coefload;
//    QList<int> loadStep;
//    QList<int> loadMaxStep;
//    QStringList loadCase2;
//    QList<qreal> coefload2;
//    QList<int> loadStep2;
//    QList<int> loadMaxStep2;
//    QString virtualLoadCase;
//    QString virtualLoadCase2;
//    int cont_or_opt;
//    int kiyoMode;
//    int optiMode;
//    int optiMode2;
//    qreal controlDisp;
//    int syuukeiFloorID;
//    QStringList syuukeiFloors;
//    int syuukeiFrameID;
//    QStringList syuukeiFrames;
//    QStringList syuukeiSection;
//    qreal bairitsu;
//    bool drawNumberLabel;
//    int limitedFloorID;
//    QStringList limitedFloors;
//    int limitedFrameID;
//    QStringList limitedFrames;
//    QStringList limitedSection;
//    bool drawColumnY;
//    bool drawColumnZ;
//    bool drawGirderY;
//    bool drawGirderZ;
//    bool drawBraceY;
//    bool drawBraceZ;
//    bool drawJiku;
//    bool drawMage;
//    bool drawSendan;
//    bool drawNejire;
//    bool drawLegend;
//    bool drawFileName;
//    bool drawPoint;
//    bool drawMemberLabel;
//    int fontSize;
//    bool shade;
//    int renderType;
//    bool smoothing;

//};
}
#endif
