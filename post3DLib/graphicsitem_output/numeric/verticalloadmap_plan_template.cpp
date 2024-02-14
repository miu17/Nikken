#include "verticalloadmap_plan_template.h"

#include "define_npstn.h"
#include "fixed_data.h"
#include "manage_jointdata.h"
#include "output_joint_item_verticalloadmap.h"
#include "planOT_grscene.h"
#include "unified_output.h"

namespace post3dapp{

VerticalLoadMapPlanTemplate::VerticalLoadMapPlanTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    :FigureCommonTemplatePlan(_setting, type, uuid)
{
}
void VerticalLoadMapPlanTemplate::createJointItem(OutputGrScene *scene, JointData *jd, int ino, const QPointF &pp)
{
    Q_UNUSED(ino);
    qDebug() << "VerticalLoadMapPlanTemplate::createJointItem";
    VEC3D v1(1.0, 0.0, 0.0);
    VEC3D v3(0.0, 0.0, 1.0);
    OutputGraphicsItem *item = new OutputJointItemVerticalLoadMap(this, DATATYPE::TPJOINT,STRUCTTYPE::NOSTRUCTTYPE,jd->getUuid(),v3,v1); //,ino,v3,v1,myFrameID);
    item->setDrawingStatusDirect();
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);


    item->setPos(pp);
    item->setZValue(-100.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);

}
void VerticalLoadMapPlanTemplate::preCreateItems(OutputGrScene*)
{
    qDebug()<<"VerticalLoadMapPlanTemplate::preCreateItems init";
    //親クラスのポインタから呼ぶと、zloadのメモリが上手く確保されない？

    int nzload=3;
    zload.clear();
    DataNBLD3D_LOAD load = UnifiedFixedData::getInstance()->getPstnData()->getLoadData();
    DataNBLD3D_CMDL model= UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();

    zload=QVector<QVector<qreal>>(model.numj,QVector<qreal>(nzload,0.0));

    int numpt = 0;
    QVector<QVector<int>> jrow;
    QVector<int> numpgr;
    QVector<QVector<QVector<int>>> lpgr;
    for(int jnt=0;jnt<model.numj;jnt++){//710
        //JROWのサイズはnumjというわけでもなくてnumptだけ必要。データベースのサイズから求めるのもやりすぎ。numptと一緒に動的に確保することにする
        if(numpgr.count()<=numpt)numpgr<<0;
        if(lpgr.count()<=numpt)lpgr<<QVector<QVector<int>>(model.mxjgr,QVector<int>(model.njgk,0));
        if(jrow.count()<=numpt)jrow<<QVector<int>(model.mxnfl*model.numfg,0);

        if(model.njmem.at(jnt)==0)continue;
        if(model.njmem.at(jnt)==1){
            //片持ち梁は戻る
            int msyu = model.imm1.at(model.jmem.at(jnt).at(0)).at(1);
            if(msyu == 2)continue;
        }
        bool finished = false;
        for(int ip=0;ip<numpt;ip++){//720
            int nn=0;

            for(int i=0;i<numpgr.at(ip);i++){//740
                for(int j=0;j<model.njgr.at(jnt);j++){//742
                    if(lpgr.count()-1<ip){qDebug()<<"lpgr-1 , ip , i , j"<<lpgr.count()-1<<ip<<i<<j;}
                    if(lpgr.at(ip).count()-1<i){qDebug()<<"lpgr.at(ip)-1 ,ip , i , j"<<lpgr.at(ip).count()-1<<ip<<i<<j;}

                    if(lpgr.at(ip).at(i).at(0)==model.jntgr.at(jnt).at(j).at(0)
                            && lpgr.at(ip).at(i).at(1)==model.jntgr.at(jnt).at(j).at(1)){
                        nn++;
                        if(nn==model.njgr.at(jnt)){
                            int kg=model.jntfl.at(jnt).at(0).at(0);
                            int kl=model.jntfl.at(jnt).at(0).at(1);
                            if(jrow.count()-1<ip){qDebug()<<"jrow-1 ,ip , kg,kl,nn,jnt"<<jrow.at(ip).count()-1<<ip<<kg<<kl<<nn<<jnt;}
                            if(jrow.at(ip).count()-1<model.kfl.at(kg).at(kl)-1){qDebug()<<"jrow.at(ip)-1 ,model.kfl.at(kg).at(kl)-1,ip , kg,kl,nn,jnt"<<jrow.at(ip).count()-1<<model.kfl.at(kg).at(kl)-1<<ip<<kg<<kl<<nn<<jnt;}

                            jrow[ip][model.kfl.at(kg).at(kl)-1]=jnt;
                            //GOTO 710//ループを3つ抜けたい
                            finished = true;
                            break;
                        }
                    }
                }
                break;
            }
            if(finished)break;
        }
        numpgr[numpt]=model.njgr.at(jnt);
        for(int i=0;i<model.njgr.at(jnt);i++){//760
            if(lpgr.count()-1<numpt){qDebug()<<"lpgr-1 , numpt , i , j"<<lpgr.count()-1<<numpt<<i;}
            if(lpgr.at(numpt).count()-1<i){qDebug()<<"lpgr.at(numpt)-1 ,ip , i , j"<<lpgr.at(numpt).count()-1<<numpt<<i;}

            lpgr[numpt][i][0]=model.jntgr.at(jnt).at(i).at(0);
            lpgr[numpt][i][1]=model.jntgr.at(jnt).at(i).at(1);
        }
        int kg=model.jntfl.at(jnt).at(0).at(0)-1;
        int kl=model.jntfl.at(jnt).at(0).at(1)-1;
        if(jrow.count()-1<numpt){qDebug()<<"jrow-1 ,numpt , kg,kl,nn,jnt"<<jrow.at(numpt).count()-1<<numpt<<kg<<kl<<jnt;}
        if(jrow.at(numpt).count()<model.kfl.at(kg).at(kl)){qDebug()<<"jrow.at(numpt) ,model.kfl.at(kg).at(kl),numpt , kg,kl,nn,jnt"<<jrow.at(numpt).count()<<model.kfl.at(kg).at(kl)<<numpt<<kg<<kl<<jnt;}

        jrow[numpt][model.kfl.at(kg).at(kl)-1]=jnt;
        numpt++;
    }
    return;

    //CDLとCLL1にあたるCの番号をサーチ
    int ild1=0;int ild2=0;
    for(int i=0;i<load.numc;i++){
        qDebug()<<load.namel.at(i).trimmed();
        if(load.namel.at(i).trimmed()=="CDL"){
            ild1=i;break;
        }
    }
    for(int i=0;i<load.numc;i++){
        qDebug()<<load.namel.at(i).trimmed();
        if(load.namel.at(i).trimmed()=="CLL1"){
            ild2=i;break;
        }
    }


    for(int i=0;i<model.numj;i++){//300
        if(model.njmem.at(i)==0)continue;
        if(model.njmem.at(i)==1){
            //片持ち梁の荷重処理
            int im=model.jmem.at(i).at(0)-1;
            int msyu=model.imm1.at(im).at(1);
            if(msyu==2){
                if(model.indm.at(im).at(0)-1 == i){
                    int jj=model.indm.at(im).at(1)-1;
                    zload[jj][0] -= load.fincj[ild1][i][2];
                    zload[jj][1] -= load.fincj[ild2][i][2];
                    zload[jj][2] -= (load.fincj[ild1][i][2]+load.fincj[ild2][i][2]);
                   continue;
                }else if(model.indm.at(im).at(1)-1 == i){
                    int jj=model.indm.at(im).at(0)-1;
                    zload[jj][0] -= load.fincj[ild1][i][2];
                    zload[jj][1] -= load.fincj[ild2][i][2];
                    zload[jj][2] -= (load.fincj[ild1][i][2]+load.fincj[ild2][i][2]);
                    continue;
                }else{
                    continue;
                }
                continue;
            }
        }
        zload[i][0] -= load.fincj.at(ild1).at(i).at(2);
        zload[i][1] -= load.fincj.at(ild2).at(i).at(2);
        zload[i][2] -= (load.fincj.at(ild1).at(i).at(2)+load.fincj.at(ild2).at(i).at(2));
    }
    //追加の処理
    for(int ip=0;ip<numpt;ip++){//310
        for(int i=model.mxnfl*model.numfg-1;i>0;i--){
            int j1=jrow[ip][i];
            if(j1==0)continue;
            qreal add = zload[j1][0]+zload[j1][1];
            for(int j=i-1;j>=0;j--){
                int j2=jrow[ip][j];
                if(j2==0)continue;
                zload[j2][2]+=add;
            }

        }
    }
    qDebug()<<"VerticalLoadMapPlanTemplate::preCreateItems return";
    return;

}
}
