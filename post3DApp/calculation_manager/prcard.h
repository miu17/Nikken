#ifndef PRCARD_H
#define PRCARD_H

#include <QStringList>

class Prcard
{
public:
    Prcard();
    static QStringList getPr(const QString& type){return QStringList{};}
    static QStringList getSection(int, const QString& title, const QString& subTitle){return QStringList{};}
    static QStringList getTitle(){return QStringList{};}
    static QStringList getType(){return QStringList{};}
    static QStringList getList();
    static QStringList getColumnList();
    static QStringList getGirderList();
    static QStringList getWallList();
    static QStringList getSteelWallList();
    static QStringList getFoundationWallList();
    static QStringList getBraceList();
    static QStringList getBeamList();
    static QStringList getSlabList();
    static QStringList getOpeningList();
    static QStringList getSteelBaseList();
    static QStringList getSrcBaseList();
    static QStringList getDamperList();
    static QStringList getIsolatorList();

    static QStringList getEstimate();
    static QStringList getAnalysis();
    static QStringList getFloor();
    static QStringList getMaterial();
    static QStringList getSteelMaterial();
    static QStringList getConcreteMaterial();
    static QStringList getRebarMaterial();

    static QStringList getAList();
    static QStringList getJoint();
    static QStringList getIndex();
    static QStringList getD3();
    static QStringList getD4();
    static QStringList getD4Joint();
    static QStringList getMember();
    static QStringList getWall();
    static QStringList getZwall();
    static QStringList getSlab();
    static QStringList getPanel();

    static QStringList getSpringList();
    static QStringList getJointSpring();
    static QStringList getRlForSpring();
    static QStringList getAxialSpring();
    static QStringList getShearSpring();
    static QStringList getMssSpring();
    static QStringList getBsSpring();
    static QStringList getTorqueSpring();
    static QStringList getWallSpring();
    static QStringList getFrame();
    static QStringList getMemberEndSpring();
    static QStringList getPrPres(const QString& type, const QString& load){return QStringList{};}
    static QStringList getSeismicLoad();
    //static QStringList getWindLoad(){return QStringList{};}
    static QStringList getJointLoad();
    static QStringList getPrPrea(const QString& type, const QString& model1, const QString& model2){return QStringList{};}
    static QStringList getFrameModel();
    static QStringList getQuColumn();

    static QStringList getQuGirder();

    static QStringList getNuBrace();
    static QStringList getQuWall();

    static QStringList getPanelModel();

    static QStringList getPrSols(const QString& type, const QString& stress){return QStringList{};}
    static QStringList getJointDisp(){return QStringList{};}
    static QStringList getJointIForce(){return QStringList{};}
    static QStringList getJointReact(){return QStringList{};}
    static QStringList getMemberJStress(){return QStringList{};}
    static QStringList getMemberFStress(){return QStringList{};}
    static QStringList getWallStress(){return QStringList{};}
    static QStringList getSlabStress(){return QStringList{};}
    static QStringList getPanelStress(){return QStringList{};}
    static QStringList getJointSpringStr(){return QStringList{};}
    static QStringList getAxialSpringStr(){return QStringList{};}
    static QStringList getShearSpringStr(){return QStringList{};}
    static QStringList getMssSpringStr(){return QStringList{};}
    static QStringList getBsSpringStr(){return QStringList{};}
    static QStringList getTorqueSpringStr(){return QStringList{};}
    static QStringList getWallSpringStr(){return QStringList{};}

    static QStringList getHfDistribution();
    static QStringList getStoryStiffness();
    static QStringList getMaximumDeform();
    static QStringList getStfFactor();
    static QStringList getEccFactor();
    static QStringList getFrameStiffNess();
    static QStringList getFrDistribution();
    static QStringList getFrCheckSummary();
    static QStringList getCheckSummary1();
    static QStringList getCheckSummary2();
    static QStringList getColumnCheck();
    static QStringList getGirderCheck();
    static QStringList getBraceCheck();
    static QStringList getWallCheck();

    static QStringList getStoryPlastInfo();
    static QStringList getQrCurve();
    static QStringList getFrQdCurve();
    static QStringList getFrDistPlast();
    static QStringList getQuNeed();
    static QStringList getUltimateState();
    static QStringList getFrMemUlt();
    static QStringList getFrRank();
    static QStringList getFrMemR();
    static QStringList getGirderRank();
    static QStringList getColumnRnak();
    static QStringList getCwArea();
    static QStringList getMessage();

    /*
XXX ZZ- ZZ  座屈長さの自動計算結果 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXX ZZ- ZZ  スラブによる大梁の曲げ剛性増大率の自動計算結果 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXX ZZ- ZZ  鉄骨大梁の保有耐力横補剛の検討 XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXX ZZ- ZZ  柱座屈長さ XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXX ZZ- ZZ  大梁座屈長さ XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
XXX ZZ- ZZ  荷重増分解析の層せん断力－層間変形関係における層間変形の計算位置 XXXXXXXXXXXXXXXXXXXXXX [ 03/02/02 18:09:28 ] XXXXXXXXXX
    */
};

#endif // PRCARD_H
