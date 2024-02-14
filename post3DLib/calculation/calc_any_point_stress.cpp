#include "calc_any_point_stress.h"

#include <QVector3D>

#include "define_npstn.h"
#include "fixed_data.h"
#include "fortranFunc.h"
#include "member_force.h"
#include "unified_output.h"

namespace post3dapp{
QString calcAnyPointStress::currentfilename=QString();
//QString calcAnyPointStress::currentmodelname=QString();
QHash<QString,int*> calcAnyPointStress::nlhmmList ;
QHash<QString,int*> calcAnyPointStress::ilhmmList ;
QHash<QString,double*> calcAnyPointStress::dlhmmList;
QHash<QString,int*> calcAnyPointStress::lndlhList ;
QHash<QString,int*> calcAnyPointStress::nlemmList ;
QHash<QString,int*> calcAnyPointStress::ilemmList ;
QHash<QString,double*> calcAnyPointStress::dlemmList ;
QHash<QString,int*> calcAnyPointStress::lndleList ;
QHash<QString,int*> calcAnyPointStress::ifpmmList ;
QHash<QString,double*> calcAnyPointStress::debmmList ;
QHash<QString,int*> calcAnyPointStress::indmList ;
QHash<QString,double*> calcAnyPointStress::evmemList;

calcAnyPointStress::calcAnyPointStress()
{
}
void calcAnyPointStress::readData(QString modelName){
    qDebug()<<"resetdata";
    DataNBLD3D_LOAD load = UnifiedFixedData::getInstance()->getPstnData()->getLoadData();
    QHash<QString, DataNBLD3D_PREA> preaRaw=UnifiedFixedData::getInstance()->getPstnData()->getPreaData();
    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
//    QString modelName = currentmodelname;
    DataNBLD3D_PREA prea = preaRaw.value(modelName);
    int* nlhmm = new int[load.nlhmm.count()];
    int* ilhmm = new int[load.ilhmm.count()*load.ilhmm.first().count()*load.ilhmm.first().first().count()];
    double* dlhmm = new double[load.dlhmm.count()*load.dlhmm.first().count()*load.dlhmm.first().first().count()];
    int* lndlh = new int[load.lndlh.count()*load.lndlh.first().count()*load.lndlh.first().first().count()];

    int* nlemm = new int[load.nlemm.count()];
    int* ilemm = new int[load.ilemm.count()*load.ilemm.first().count()*load.ilemm.first().first().count()];
    double* dlemm = new double[load.dlemm.count()*load.dlemm.first().count()*load.dlemm.first().first().count()];
    int* lndle = new int[load.lndle.count()*load.lndle.first().count()*load.lndle.first().first().count()];

    int* ifpmm = new int[model.ifpmm.count()*model.ifpmm.first().count()];
    double* debmm = new double[prea.debmm.count()*prea.debmm.first().count()];
    int* indm = new int[model.indm.count()*model.indm.first().count()];
    double* evmem = new double[model.evmem.count()*model.evmem.first().count()*model.evmem.first().first().count()];

    vecToArray(nlhmm,load.nlhmm);
    vecToArray(ilhmm,load.ilhmm);
    vecToArray(dlhmm,load.dlhmm);
    vecToArray(lndlh,load.lndlh);

    //qDebug()<<"toArray";
    vecToArray(nlemm,load.nlemm);
    vecToArray(ilemm,load.ilemm);
    vecToArray(dlemm,load.dlemm);
    vecToArray(lndle,load.lndle);

    //qDebug()<<"toArray";
    vecToArray(ifpmm,model.ifpmm);
    vecToArray(debmm,prea.debmm);
    vecToArray(indm,model.indm);
    vecToArray(evmem,model.evmem);
    nlhmmList.insert(modelName,nlhmm);
    ilhmmList.insert(modelName,ilhmm);
    dlhmmList.insert(modelName,dlhmm);
    lndlhList.insert(modelName,lndlh);
    nlemmList.insert(modelName,nlemm);
    ilemmList.insert(modelName,ilemm);
    dlemmList.insert(modelName,dlemm);
    lndleList.insert(modelName,lndle);
    ifpmmList.insert(modelName,ifpmm);
    debmmList.insert(modelName,debmm);
    indmList.insert(modelName,indm);
    evmemList.insert(modelName,evmem);
}
void calcAnyPointStress::deleteData(){
    qDebug()<<"deletedata";
    foreach(int* p,nlhmmList)delete[] p;
    nlhmmList.clear();
    foreach(int* p,ilhmmList)delete[] p;
    ilhmmList.clear();
    foreach(double* p,dlhmmList)delete[] p;
    dlhmmList.clear();
    foreach(int* p,lndlhList)delete[] p;
    lndlhList.clear();
    foreach(int* p,nlemmList)delete[] p;
    nlemmList.clear();
    foreach(int* p,ilemmList)delete[] p;
    ilemmList.clear();
    foreach(double* p,dlemmList)delete[] p;
    dlemmList.clear();
    foreach(int* p,lndleList)delete[] p;
    lndleList.clear();
    foreach(int* p,ifpmmList)delete[] p;
    ifpmmList.clear();
    foreach(double* p,debmmList)delete[] p;
    debmmList.clear();
    foreach(int* p,indmList)delete[] p;
    indmList.clear();
    foreach(double* p,evmemList)delete[] p;
    evmemList.clear();

}
QVector<double> calcAnyPointStress::calcMemberForce(const QUuid &member_id, const int &iPos,const qreal &xPos, const QString &combi, const int &step)
{


    //xPos mm -> m
    qreal xpos = xPos/1000.0;

    int ipos = iPos;//1:i,2:c,3:j


    QStringList keys = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstData().keys();
    int maxstep = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstData().value(combi).stressSteps.count();
    int stepid = step-1;

    QVector<double> ret(6,0.0);
    double posrst[6];

    //長期のSTEP１と短期のLASTSTEPを渡したい。
    //U1のステップ０は、長期

    //step,makeOutfigTypeOptionで、widgetのstepの値か、getMaxStepが入っている。
    if(step == 0 || !keys.contains(combi)){qDebug()<<"invalid stress. step,combi="<<step<<combi;return ret;}
    if(step < 0 || step >= maxstep)stepid = maxstep-1;


    //MemberForceは自分のキーやインデックスを知らない。キーは組み合わせケース名、インデックスはステップ。
    //中間荷重から任意点の応力を計算する
    //節点からの距離。一方で、フェイス位置応力図はフェイスを0・1だと思っているので、修正が必要

    //部材中間荷重を考慮する場合
    //端部応力のみの場合
    int IM = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(member_id)+1;

    auto rst_of_step = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstData().value(combi).stressSteps.at(stepid);
    DataNBLD3D_LOAD load = UnifiedFixedData::getInstance()->getPstnData()->getLoadData();
    QHash<QString, DataNBLD3D_PREA> preaRaw=UnifiedFixedData::getInstance()->getPstnData()->getPreaData();
    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();

    //modelNameは、何と紐づいている？　→　組み合わせ最終ケースのモデルとする
    //モデル番号はMKCCS(IS,ISTR)。IS=NRSCS(ISTR)。書き出されていない？


    QString modelName = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstData().value(combi).nameam;
    QString newfile = UnifiedFixedData::getInstance()->getPstnData()->currentFilePath();

    if(newfile != currentfilename &&currentfilename != QString())deleteData();
    currentfilename = newfile;
    if(!evmemList.keys().contains(modelName))readData(modelName);

    DataNBLD3D_PREA prea = preaRaw.value(modelName);

    //coefは一つ上の関数で考慮
    const qreal eps= 0.00000001;
    for (int IC = 0; IC < rst_of_step.kcasol; IC++) {
        int icase = rst_of_step.icase[IC]; //cl番目の荷重ケースの番号
        if(icase>0){
            int ICL=load.indle[icase-1];
//            qreal CO =rst.corscs[ISTR][IS];
//            qreal C1 = rst_of_step.ccase[IC]; //係数
            qreal CC = 1.0 ;//* C1;//coefは一つ上の関数で加味
            if(qAbs(CC)<eps)CC=1.0;
            //C0=CORSCS、C1=CKCCSがどこかで拾える？

            int* nlhmm = nlhmmList.value(modelName);
            int* ilhmm = ilhmmList.value(modelName);
            double* dlhmm=dlhmmList.value(modelName);
            int* lndlh = lndlhList.value(modelName);

            int* nlemm = nlemmList.value(modelName);
            int* ilemm = ilemmList.value(modelName);
            double* dlemm=dlemmList.value(modelName);
            int* lndle = lndleList.value(modelName);

            int* ifpmm = ifpmmList.value(modelName);
            double* debmm=debmmList.value(modelName);
            int* indm = indmList.value(modelName);
            double* evmem=evmemList.value(modelName);

            SETML2(posrst,
                   &IM    ,&ICL   ,&CC    ,&ipos  ,&xpos   ,
                   nlhmm,
                   ilhmm,
                   dlhmm,
                   lndlh,
                   &load.mxlhmm,
                   ifpmm,
                   debmm,
                   indm,
                   evmem ,
                   &model.numm  );
            SETML2(posrst,
                   &IM    ,&ICL   ,&CC    ,&ipos  ,&xpos   ,
                   nlemm,
                   ilemm,
                   dlemm,
                   lndle,
                   &load.mxlemm,
                   ifpmm ,
                   debmm,
                   indm,
                   evmem ,
                   &model.numm  );
        }
    }

    for(int i=0;i<6;i++){
        ret[i]=posrst[i];
    }
    //    //rstmmの足し合わせ、ここでやったほうがシンプルか？ →　CMQなど、基本応力だけ欲しいときちょっと困る
//    qreal iFace = model.dmm1[IM-1][6];//faceの取り方はポリシーによって異なる
//    qreal jFace = model.dmm1[IM-1][7];//faceの取り方はポリシーによって異なる
//    qreal pos = xPos;
//    if(iPos == 1)pos = iFace;
//    if(iPos == 2)pos = 0.5*iFace+0.5*jFace;
//    if(iPos == 3)pos = jFace;
//    ret[0] = posrst[0] + rst_of_step.rstmm[IM-1][6];
//    ret[1] = posrst[1] + rst_of_step.rstmm[IM-1][4];
//    ret[2] = posrst[2] + rst_of_step.rstmm[IM-1][5];
//    ret[3] = posrst[3] + rst_of_step.rstmm[IM-1][7];
//    qreal Qy = rst_of_step.rstmm[IM-1][4];
//    qreal Qz = rst_of_step.rstmm[IM-1][5];
//    ret[4] = posrst[4] + rst_of_step.rstmm[IM-1][0] + (pos-iFace)*Qz;
//    ret[5] = posrst[5] + rst_of_step.rstmm[IM-1][1] - (pos-iFace)*Qy;

    return ret;

}


void calcAnyPointStress::vecToArray(double* p,QVector<qreal> vec)
{
 //   qDebug()<<"1d double";
    int size = vec.count();
    for(int i=0;i<size;i++){
        p[i] = vec.at(i);
    }
    return;
}
void calcAnyPointStress::vecToArray(double* p,QVector<QVector<qreal>> vec)
{
 //   qDebug()<<"2d double";
    int size1 = vec.count();
    int size2 = vec.first().count();
    int count = 0;
    for(int i=0;i<size1;i++){
        for(int j=0;j<size2;j++){
            p[count] = vec.at(i).at(j);
            count++;
        }
    }
    return;
}
void calcAnyPointStress::vecToArray(double* p,QVector<QVector<QVector<qreal>>> vec)
{
 //   qDebug()<<"3d double";
    int size1 = vec.count();
    int size2 = vec.first().count();
    int size3 = vec.first().first().count();
    int count = 0;
    for(int i=0;i<size1;i++){
        for(int j=0;j<size2;j++){
            for(int k=0;k<size3;k++){
                p[count] = vec.at(i).at(j).at(k);
                count++;
            }
        }
    }
    return;
}
void calcAnyPointStress::vecToArray(double* p,QVector<QVector<QVector<QVector<qreal>>>> vec)
{
//    qDebug()<<"4d double";
    int size1 = vec.count();
    int size2 = vec.first().count();
    int size3 = vec.first().first().count();
    int size4 = vec.first().first().first().count();
    int count = 0;
    for(int i=0;i<size1;i++){
        for(int j=0;j<size2;j++){
            for(int k=0;k<size3;k++){
                for(int l=0;l<size4;l++){
                    p[count] = vec.at(i).at(j).at(k).at(l);
                    count++;
                }
            }
        }
    }
    return;
}
void calcAnyPointStress::vecToArray(int* p,QVector<int> vec)
{
//    qDebug()<<"1d int";

    int size = vec.count();
    for(int i=0;i<size;i++){
        p[i] = vec.at(i);
    }
    return;
}
void calcAnyPointStress::vecToArray(int* p,QVector<QVector<int>> vec)
{
//    qDebug()<<"2d int";
    int size1 = vec.count();
    int size2 = vec.first().count();
    int count = 0;
    for(int i=0;i<size1;i++){
        for(int j=0;j<size2;j++){
            p[count] = vec.at(i).at(j);
            count++;
        }
    }
    return;
}
void calcAnyPointStress::vecToArray(int* p,QVector<QVector<QVector<int>>> vec)
{

//    qDebug()<<"3d int";
    int size1 = vec.count();
    int size2 = vec.first().count();
    int size3 = vec.first().first().count();
//    qDebug()<<size1<<size2<<size3;
    int count = 0;
    for(int i=0;i<size1;i++){
        for(int j=0;j<size2;j++){
            for(int k=0;k<size3;k++){
                p[count] = vec.at(i).at(j).at(k);
                count++;
            }
        }
    }
    return;
}
void calcAnyPointStress::vecToArray(int* p,QVector<QVector<QVector<QVector<int>>>> vec)
{
//    qDebug()<<"4d int";
    int size1 = vec.count();
    int size2 = vec.first().count();
    int size3 = vec.first().first().count();
    int size4 = vec.first().first().first().count();
    int count = 0;
    for(int i=0;i<size1;i++){
        for(int j=0;j<size2;j++){
            for(int k=0;k<size3;k++){
                for(int l=0;l<size4;l++){
                    p[count] = vec.at(i).at(j).at(k).at(l);
                    count++;
                }
            }
        }
    }
    return;
}
}//post3dapp
