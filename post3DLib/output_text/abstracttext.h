#ifndef ABSTRACTTEXT_H
#define ABSTRACTTEXT_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include "line_outputtext.h"

namespace post3dapp{
class FixedData;
class UnifiedOutput;

class POST3D_CLASS_DLLSPEC AbstractText
{
public:
    AbstractText(OutputTextStatus _status):status(_status){};
    virtual ~AbstractText(){};
    OutputTextStatus getStatus() const{return status;}
    virtual QString write() const{return getJapanese() + "\n";}
protected:
    const OutputTextStatus status;
    QSet<QUuid> uuidSet;
    virtual QPair<QPair<QString, QString>, QString> write(const FixedData&) const {return QPair<QPair<QString, QString>, QString>{};}
    QString getCommand() const {return LineOutputText::outputTextStatusToCommand(status);}
    QString getJapanese() const {return LineOutputText::outputTextStatusToJapanese(status);};
};

class POST3D_CLASS_DLLSPEC AbstractTextCallFortran : public AbstractText
{
public:
    AbstractTextCallFortran(OutputTextStatus _status):AbstractText(_status){};
    virtual ~AbstractTextCallFortran(){};
protected:
    const char* tempFile = "D:\\temp.txt";
    QString readTempFile(const QString&) const;
    virtual void CallFortran(const UnifiedOutput&) const = 0;
    QPair<QPair<QString, QString>, QString> write(const FixedData&) const override final;
};

#define TEXT_COMMAND(classname) \
    class POST3D_CLASS_DLLSPEC classname : public AbstractText \
{ \
    public: \
        classname(OutputTextStatus _status):AbstractText(_status){}; \
    protected: \
        QPair<QPair<QString, QString>, QString> write(const FixedData&) const override; \
};

#define TEXT_FORTRAN_COMMAND(classname) \
    class POST3D_CLASS_DLLSPEC classname : public AbstractTextCallFortran \
{ \
    public: \
        classname(OutputTextStatus _status):AbstractTextCallFortran(_status){}; \
    protected: \
        void CallFortran(const UnifiedOutput&) const override; \
};


TEXT_COMMAND(TextCcs);

TEXT_COMMAND(TextOutline);
TEXT_COMMAND(TextLsgir);
TEXT_COMMAND(TextLscol);
TEXT_COMMAND(TextLswal);
TEXT_COMMAND(TextLsswa);
TEXT_COMMAND(TextLsfwa);
TEXT_COMMAND(TextLsbra);
TEXT_COMMAND(TextLsbeam);
TEXT_COMMAND(TextLsslab);
TEXT_COMMAND(TextLsopen);
TEXT_COMMAND(TextLsbs);
TEXT_COMMAND(TextLsbsrc);
TEXT_COMMAND(TextLsdamp);
TEXT_COMMAND(TextLsmen);
TEXT_COMMAND(TextLdl);
TEXT_COMMAND(TextLdf);
TEXT_COMMAND(TextLdm);
TEXT_COMMAND(TextLdj);
TEXT_COMMAND(TextEstim);
TEXT_COMMAND(TextAnacon);

TEXT_COMMAND(TextFloor);
TEXT_COMMAND(TextStt);
TEXT_COMMAND(TextMatc);
TEXT_COMMAND(TextMatr);
TEXT_COMMAND(TextMats);
TEXT_COMMAND(TextAlist);
TEXT_COMMAND(TextJnt);
TEXT_COMMAND(TextIndex);
TEXT_COMMAND(TextD3);
TEXT_COMMAND(TextRfloor);
TEXT_COMMAND(TextD4Joint);
TEXT_COMMAND(TextMember);
TEXT_COMMAND(TextWall);
TEXT_COMMAND(TextZWall);
TEXT_COMMAND(TextSlab);
TEXT_COMMAND(TextPanel);
TEXT_COMMAND(TextSpringList);
TEXT_COMMAND(TextJspring);
TEXT_COMMAND(TextRlForSpring);
TEXT_COMMAND(TextAxialSpring);
TEXT_COMMAND(TextShearSpring);
TEXT_COMMAND(TextMssSpring);
TEXT_COMMAND(TextBsSpring);
TEXT_COMMAND(TextTorqueSpring);
TEXT_COMMAND(TextWallSpring);
TEXT_COMMAND(TextFrame);
TEXT_COMMAND(TextMemberEndSpring);
//CMODEL(PR032)
//EMODEL(PR033(0))
//ECAL
//CHKC(PR035)
//PCAL(PR033(1))
//CONFIG(PR061)
//AUTOLK OPEN+PR3DOUT
//AUTOPHI OPEN+PR3DOUT
//YOKOHOGOU OPEN+PR3DOUT
//QDPOS OPEN+PR3DOUT
//STECFACTOR(PR413)
//CGCMP1 OPEN+PR3DOUT
//CHECK(SUMMARY)-T PR501C
//CHECK(SUMMARY)-3 PR501D
//CHCUTOFF OPEN+PR3DOUT
//CHPS2 OPEN+PR3DOUT
//CHPRC2 OPEN+PR3DOUT
//CHPSRC2 OPEN+PR3DOUT
//CHPS3 OPEN+PR3DOUT
//CHPRC3 OPEN+PR3DOUT
//CHPSRC3 OPEN+PR3DOUT
//CHPCFT3 OPEN+PR3DOUT
//CHOPEN OPEN+PR3DOUT
//CHSRCBASE OPEN+PR3DOUT
//CHSBASE OPEN+PR3DOUT
//CHNEOBASE OPEN+PR3DOUT
//CHNEWBASE OPEN+PR3DOUT
//CHBPBASE OPEN+PR3DOUT
//CHBONDS1 OPEN+PR3DOUT
//CHBONDS2 OPEN+PR3DOUT
//FRAMEPLASTINFO(PR602)
//QU-CONDITION OPEN+PR3DOUT
//QU-C-RC OPEN+PR3DOUT
//QU-C-SRC OPEN+PR3DOUT
//QU-G-RC OPEN+PR3DOUT
//QU-G-SRC OPEN+PR3DOUT
//QU-P-S OPEN+PR3DOUT
//QU-P-RC OPEN+PR3DOUT
//QU-P-SRC OPEN+PR3DOUT
//QU-P-CFT OPEN+PR3DOUT
//QU-W-RC OPEN+PR3DOUT
//QU-W-SRC OPEN+PR3DOUT
//RN-C-S OPEN+PR3DOUT
//RN-C-RC OPEN+PR3DOUT
//RN-C-SRC OPEN+PR3DOUT
//RN-C-CFT OPEN+PR3DOUT
//RN-G-S OPEN+PR3DOUT
//RN-G-RC OPEN+PR3DOUT
//RN-G-SRC OPEN+PR3DOUT
//RN-P-RC OPEN+PR3DOUT
//RN-V-S OPEN+PR3DOUT
//RN-W-RC OPEN+PR3DOUT
//RN-W-SRC OPEN+PR3DOUT
//CGCMPM OPEN+PR3DOUT
//CGCMPF OPEN+PR3DOUT
//GRBEND OPEN+PR3DOUT
//GRBONDI OPEN+PR3DOUT
//GRBONDP OPEN+PR3DOUT
//MBCPR OPEN+PR3DOUT
//QBCPR OPEN+PR3DOUT
//DSSRCBASE OPEN+PR3DOUT
//DSSBASE OPEN+PR3DOUT
//DSNEOBASE OPEN+PR3DOUT
//DSNEWBASE OPEN+PR3DOUT
//DSBPBASE OPEN+PR3DOUT
//QUN PR693
//COND-ALL PRCONFIG
//COND-SHPE PRCONFIG
//COND-ECAL PRCONFIG
//COND-PCAL PRCONFIG
//COND-CHCK PRCONFIG
//COND-DSPO PRCONFIG
//COND-MRNK PRCONFIG
//COND-QUDE PRCONFIG
//QUDETRM OPEN+PR3DOUT
//QUDETAIL OPEN+PR3DOUT
//JOINT-ZREACT(PR814)
//JOINT-ZDISP(PR814)
//JOINT-ZREPRESS(PR814)
//SHG1-S OPEN+PR3DOUT
//SHG1-C OPEN+PR3DOUT
//SHG3-S OPEN+PR3DOUT
//SGH3-C OPEN+PR3DOUT

//D3
//D4-JOINT
//MEMBER
//+WALL
//+WALL-JOINT
//+WALL-MEMBER
//+WALL-MANGLE
//+ZWALL
//+ZWALL-JOINT
//+ZWALL-MEMBER
//+ZWALL-MANGLE
//+SLAB
//+PANEL
//SPRING-LIST
//RL-FOR-SPRING
//AXIAL-SPRING
//SHEAR-SPRING
//MSS-SPRING
//BS-SPRING
//TORQUE-SPRING
//WALL-SPRING
//CLK
//CLB
//+MEMBER-CMQ
//+WALL-CMQ
//+SLAB-CMQ
//+PANEL-MODEL
//STF-FACTOR
//ECC-FACTOR
//SHBASE-CHECK
//ADHESION-CHECK
//+MEMBER-SCMQ
//+WALL-SCMQ
//+SLAB-SCMQ
//JOINT-IFORCE
//MEMBER-DEFORM
//|FRAME-JSTRESS
//|FRAME-FSTRESS
//|FRAME-DEFORM
//DEFLEXTION
//WALL-STRESS
//JOINT-SPRING-STR
//AXIAL-SPRING-STR
//SHEAR-SPRING-STR
//MSS-SPRING-STR
//BS-SPRING-STR
//TORQUE-SPRING-STR
//WALL-SPRING-STR
//

TEXT_COMMAND(TextSeismicLoad);
TEXT_COMMAND(TextWindLoad);
TEXT_COMMAND(TextJointLoad);

TEXT_COMMAND(TextFrameModel);
TEXT_COMMAND(TextQuColumn);
TEXT_COMMAND(TextQuGirder);
TEXT_COMMAND(TextQuWall);
TEXT_COMMAND(TextNuBrace);
TEXT_COMMAND(TextPnlmdl);

TEXT_COMMAND(TextJointDisp);
TEXT_COMMAND(TextJointIForce);
TEXT_COMMAND(TextJointReact);
TEXT_COMMAND(TextMemberJStress);
TEXT_COMMAND(TextMemberFStress);
TEXT_COMMAND(TextWallStress);
TEXT_COMMAND(TextSlabStress);
TEXT_COMMAND(TextPanelStress);
TEXT_COMMAND(TextJointSpringStr);
TEXT_COMMAND(TextAxialSpringStr);
TEXT_COMMAND(TextShearSpringStr);
TEXT_COMMAND(TextMssSpringStr);
TEXT_COMMAND(TextBsSpringStr);
TEXT_COMMAND(TextTorqueSpringStr);
TEXT_COMMAND(TextWallSpringStr);

TEXT_FORTRAN_COMMAND(TextHfDistribution);
TEXT_COMMAND(TextStoryStiffness);
TEXT_COMMAND(TextMaximumDeform);
TEXT_COMMAND(TextStfFactor);
TEXT_COMMAND(TextEccFactor);
TEXT_COMMAND(TextFrameStiffness);
TEXT_COMMAND(TextFrDistribution);

TEXT_COMMAND(TextFrCheckSummary);
TEXT_COMMAND(TextCheckSummary1);
TEXT_COMMAND(TextCheckSummary2);
TEXT_COMMAND(TextColumnCheck);
TEXT_COMMAND(TextGirderCheck);
TEXT_COMMAND(TextBraceCheck);
TEXT_COMMAND(TextWallCheck);

TEXT_COMMAND(TextStoryPlastInfo);
TEXT_COMMAND(TextQdCurve);
TEXT_COMMAND(TextFrQdCurve);
TEXT_COMMAND(TextFrDistPlast);
TEXT_COMMAND(TextQuNeed);
TEXT_COMMAND(TextUltimateState);
TEXT_COMMAND(TextFrMemUlt);
TEXT_COMMAND(TextFrRank);
TEXT_COMMAND(TextFrMemR);
TEXT_COMMAND(TextGirderRank);
TEXT_COMMAND(TextColumnRank);

TEXT_COMMAND(TextCwArea);
TEXT_COMMAND(TextMessage);

} // namespace post3dapp

#endif // ABSTRACTTEXT_H
