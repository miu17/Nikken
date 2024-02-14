#include "output_control.h"

#include <QDebug>
#include <typeinfo>

#include "define_draw_figure.h"
#include "factory_unifieddata.h"
#include "fixed_data.h"
#include "unified_analysisdata.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "unified_output.h"
#include "unified_project.h"


namespace post3dapp{
OutputControl *OutputControl::myInstance = nullptr;

QString OutFigString::japanese() const
{
    return OutputFigureText::outfigtypeToString(type);
}

QString OutFigString::english() const
{
    return OutputFigureText::outfigtypeToEnglish(type);
}


const QMap<OUTFIGTYPE, OutFigString> OutputControl::outfigStrings{
    // OUTFIGTYPE                    key                                            Output  Plan    Sec     Pers    Graph
    {OUTFIGTYPE::MODEL,              OutFigString{OUTFIGTYPE::MODEL,                true,   true,   true,   true,   false}},
    {OUTFIGTYPE::LENGTH,             OutFigString{OUTFIGTYPE::LENGTH,               true,   true,   true,   true,   false}},
    {OUTFIGTYPE::FACELENGTH,         OutFigString{OUTFIGTYPE::FACELENGTH,           true,   true,   true,   true,   false}},
    {OUTFIGTYPE::COORDINATE,         OutFigString{OUTFIGTYPE::COORDINATE,           true,   true,   true,   true,   false}},
    {OUTFIGTYPE::RIGIDLENGTH,        OutFigString{OUTFIGTYPE::RIGIDLENGTH,          true,   true,   true,   true,   false}},
    {OUTFIGTYPE::LATERALBUCKLEN,     OutFigString{OUTFIGTYPE::LATERALBUCKLEN,       true,   true,   true,   true,   false}},
    {OUTFIGTYPE::EULERBUCKLEN,       OutFigString{OUTFIGTYPE::EULERBUCKLEN,         true,   true,   true,   true,   false}},
    {OUTFIGTYPE::JOINTMASS,          OutFigString{OUTFIGTYPE::JOINTMASS,            true,   true,   true,   true,   false}},
    {OUTFIGTYPE::OUTERFORCE,         OutFigString{OUTFIGTYPE::OUTERFORCE,           true,   true,   true,   true,   false}},
    {OUTFIGTYPE::OUTERMOMENT,        OutFigString{OUTFIGTYPE::OUTERMOMENT,          true,   true,   true,   true,   false}},
    {OUTFIGTYPE::RIGIDITYPHI,        OutFigString{OUTFIGTYPE::RIGIDITYPHI,          true,   true,   true,   true,   false}},
    {OUTFIGTYPE::CMQ,                OutFigString{OUTFIGTYPE::CMQ,                  true,   true,   true,   true,   false}},
    {OUTFIGTYPE::AXIALMAP,           OutFigString{OUTFIGTYPE::AXIALMAP,             true,   true,   false,  false,  false}},
    {OUTFIGTYPE::REACTMAP,           OutFigString{OUTFIGTYPE::REACTMAP,             true,   true,   false,  false,  false}},
    {OUTFIGTYPE::COMBIREACTMAP,      OutFigString{OUTFIGTYPE::COMBIREACTMAP,        true,   true,   false,  false,  false}},
    {OUTFIGTYPE::SHEARMAP,           OutFigString{OUTFIGTYPE::SHEARMAP,             true,   true,   false,  false,  false}},
    {OUTFIGTYPE::VERTICALLOADMAP,    OutFigString{OUTFIGTYPE::VERTICALLOADMAP,      true,   true,   false,  false,  false}},
    {OUTFIGTYPE::LAYERDEFORMMAP,     OutFigString{OUTFIGTYPE::LAYERDEFORMMAP,       true,   true,   false,  false,  false}},
    {OUTFIGTYPE::LAYERSTIFFMAP,      OutFigString{OUTFIGTYPE::LAYERSTIFFMAP,        true,   true,   false,  false,  false}},
    //    {OUTFIGTYPE::GCKCP,              OutFigString{"a",                  u8"", true, true, false, false, false}},//重心剛心点
    {OUTFIGTYPE::NSTRESS,            OutFigString{OUTFIGTYPE::NSTRESS,              true,   true,   true,   true,   false}},
    {OUTFIGTYPE::FSTRESS,            OutFigString{OUTFIGTYPE::FSTRESS,              true,   true,   true,   true,   false}},
    {OUTFIGTYPE::RSTRESS,            OutFigString{OUTFIGTYPE::RSTRESS,              true,   true,   true,   true,   false}},
    {OUTFIGTYPE::AXIAL,              OutFigString{OUTFIGTYPE::AXIAL,                true,   true,   true,   true,   false}},
    {OUTFIGTYPE::TORSION,            OutFigString{OUTFIGTYPE::TORSION,              true,   true,   true,   true,   false}},
    {OUTFIGTYPE::DEFORM,             OutFigString{OUTFIGTYPE::DEFORM,               true,   true,   true,   true,   false}},
    {OUTFIGTYPE::DISP,               OutFigString{OUTFIGTYPE::DISP,                 true,   true,   true,   true,   false}},
    {OUTFIGTYPE::CHECKRATIO,         OutFigString{OUTFIGTYPE::CHECKRATIO,           true,   true,   true,   true,   false}},
    {OUTFIGTYPE::ALLOWRATIOE,        OutFigString{OUTFIGTYPE::ALLOWRATIOE,          true,   true,   true,   true,   false}},
    {OUTFIGTYPE::ALLOWRATIOU,        OutFigString{OUTFIGTYPE::ALLOWRATIOU,          true,   true,   true,   true,   false}},
    {OUTFIGTYPE::PLASTICRATIO,       OutFigString{OUTFIGTYPE::PLASTICRATIO,         true,   true,   true,   true,   false}},
    {OUTFIGTYPE::RANK,               OutFigString{OUTFIGTYPE::RANK,                 true,   true,   true,   true,   false}},
    {OUTFIGTYPE::ALLOW,              OutFigString{OUTFIGTYPE::ALLOW,                true,   true,   true,   true,   false}},
    {OUTFIGTYPE::STRENGTH,           OutFigString{OUTFIGTYPE::STRENGTH,             true,   true,   true,   true,   false}},
    {OUTFIGTYPE::YIELDSTEP,          OutFigString{OUTFIGTYPE::YIELDSTEP,            true,   true,   true,   true,   false}},
    {OUTFIGTYPE::YIELDLOADMAG,       OutFigString{OUTFIGTYPE::YIELDLOADMAG,         true,   true,   true,   true,   false}},
    {OUTFIGTYPE::CenterOfRigidity,   OutFigString{OUTFIGTYPE::CenterOfRigidity,     true,   true,   false,  false,  false}},
    {OUTFIGTYPE::QDelta,             OutFigString{OUTFIGTYPE::QDelta,               true,   false,  false,  false,  true}},
    {OUTFIGTYPE::QDeltaFrameFloors,  OutFigString{OUTFIGTYPE::QDeltaFrameFloors,    true,   false,  false,  false,  true}},
    {OUTFIGTYPE::QDeltaFloorFrames,  OutFigString{OUTFIGTYPE::QDeltaFloorFrames,    true,   false,  false,  false,  true}},
    {OUTFIGTYPE::QTheta,             OutFigString{OUTFIGTYPE::QTheta,               true,   false,  false,  false,  true}},
    {OUTFIGTYPE::QThetaFrameFloors,  OutFigString{OUTFIGTYPE::QThetaFrameFloors,    true,   false,  false,  false,  true}},
    {OUTFIGTYPE::QThetaFloorFrames,  OutFigString{OUTFIGTYPE::QThetaFloorFrames,    true,   false,  false,  false,  true}},
    {OUTFIGTYPE::QuNeedf,           OutFigString{OUTFIGTYPE::QuNeedf,               true,   false,  false,  false,  true}},
    {OUTFIGTYPE::NMINT,              OutFigString{OUTFIGTYPE::NMINT,                true,   false,  false,  false,  false}},
};


OutputControl::OutputControl()
{
    outfigPulldownList.insert(static_cast<int>(LISTTYPE::LOAD),QStringList{"DL", "LL1", "LL2", "LL3", "LL4", "LLE"});
    outfigPulldownList.insert(static_cast<int>(LISTTYPE::LOADTYPE),QStringList{u8"全て", u8"伝達荷重", u8"手動入力", u8"自重"});
    outfigPulldownList.insert(static_cast<int>(LISTTYPE::BUZAIJIKU),QStringList{u8"yz軸方向", u8"x軸方向"});
    outfigPulldownList.insert(static_cast<int>(LISTTYPE::INOUT),QStringList{u8"面内方向", u8"面外方向", u8"材軸方向"});
}
void OutputControl::slotOutputFileRead()
{
    qDebug() << "slotOutputFileRead init";
    outfigPulldownList.insert(static_cast<int>(LISTTYPE::CHCK),UnifiedFixedData::getInstance()->getPstnData()->getChckCaseList());
    outfigPulldownList.insert(static_cast<int>(LISTTYPE::MODEL),UnifiedFixedData::getInstance()->getPstnData()->getModelCaseList());
    outfigPulldownList.insert(static_cast<int>(LISTTYPE::QUDS),UnifiedFixedData::getInstance()->getPstnData()->getQuDsCaseList());
    outfigPulldownList.insert(static_cast<int>(LISTTYPE::COMBIMAP),UnifiedFixedData::getInstance()->getPstnData()->getChckCaseList());

    QStringList stresscase = UnifiedFixedData::getInstance()->getPstnData()->getStressCaseList();
    QStringList stresscombi;//stresscombiから、stresscaseに含まれないものをはじく。
    STRESSCOMBISETTINGS combisettings = UnifiedProject::getInstance()->getStressCombinationSetting();
    Q_FOREACH(QString combi,combisettings.combi){
        bool isValid = true;
        QStringList all_st = combisettings.stress.value(combi);
        for(int i=0;i<all_st.count();i++){
            if(!stresscase.contains(all_st.at(i))){
                isValid=false;
            }
        }
        if(isValid)stresscombi<<combi;
    }
    outfigPulldownList.insert(static_cast<int>(LISTTYPE::STRESS),stresscombi);

}

QStringList OutputControl::outputTypeStringList(OUTFIGCATEGORY cat)
{
    QStringList strlist;
    strlist.append(u8"（出図の種類）");
    Q_FOREACH(auto type, outputTypeList(cat)){
        strlist.append(outfigStrings[type].japanese());
    }
    return strlist;
}
QList<OUTFIGTYPE> OutputControl::outputTypeList(OUTFIGCATEGORY cat)
{
    QList<OUTFIGTYPE> list;
    if(cat == OUTFIGCATEGORY::UNDEFINED){
        return list;
    }else if( cat == OUTFIGCATEGORY::PLAN){
        QMapIterator<OUTFIGTYPE, OutFigString> i(outfigStrings);
        while(i.hasNext()){
            i.next();
            if (i.value().isOutput && i.value().hasPlan){
                list.append(i.key());
            }
        }
    }else if(cat == OUTFIGCATEGORY::ELEVATION){
        QMapIterator<OUTFIGTYPE, OutFigString> i(outfigStrings);
        while(i.hasNext()){
            i.next();
            if (i.value().isOutput && i.value().hasSection){
                list.append(i.key());
            }
        }
    }else if(  cat == OUTFIGCATEGORY::PERS){
        QMapIterator<OUTFIGTYPE, OutFigString> i(outfigStrings);
        while(i.hasNext()){
            i.next();
            if (i.value().isOutput && i.value().hasPers){
                list.append(i.key());
            }
        }
    }else if(cat == OUTFIGCATEGORY::GRAPH){
        QMapIterator<OUTFIGTYPE, OutFigString> i(outfigStrings);
        while(i.hasNext()){
            i.next();
            if (i.value().isOutput && i.value().hasGraph){
                list.append(i.key());
            }
        }
    }else if(cat == OUTFIGCATEGORY::TEXT){
    }

    return list;
}


LISTTYPE OutputControl::listType(int idx,OUTFIGCATEGORY cat)
{
    return FIGURE_TYPE_PROPERTY::listType(outputTypeList(cat).at(idx)) ;
}

QStringList OutputControl::getOutfigStringList(LISTTYPE lt,int idx)const
{
    if(idx == 1 && lt != LISTTYPE::COMBIMAP)return outfigPulldownList.value(static_cast<int>(LISTTYPE::INOUT));
    if(idx == 3 && lt != LISTTYPE::COMBIMAP)return outfigPulldownList.value(static_cast<int>(LISTTYPE::MODEL));
    if(idx == 4 && lt != LISTTYPE::COMBIMAP)return outfigPulldownList.value(static_cast<int>(LISTTYPE::MODEL));
    if(idx == 5 && lt != LISTTYPE::COMBIMAP)return outfigPulldownList.value(static_cast<int>(LISTTYPE::MODEL));
    return outfigPulldownList.value(static_cast<int>(lt));
}

FIGURE_TYPE_PROPERTY OutputControl::makeOutputFigureTypeProperty(SCENETYPE scene, const QUuid &uid,OUTFIGTYPE myFigureType,
                                                           const QVector<int> &idx, int step) const
{
    //    int idx_a = idx.at(0);
    //    int idx_b = idx.at(1);
    //    int idx_c = idx.at(2);
    //    int idx_d = idx.at(3);
    //    int idx_e = idx.at(4);
    UnifiedAnalysisData* ud = UnifiedFixedData::getInstance()->getAnalysisData();
    FIGURE_TYPE_PROPERTY type;
    type.type = myFigureType;

    //OUTFIGTYPEによって異なるidxの意味を吸収
    //chck,solsでのmodelはどこから引いてくる？

    //プルダウンの先頭にブランクがない
    //タイトルとファイル名
    type.filename=UnifiedFixedData::getInstance()->getAnalysisData()->fileName();

    if(type.listtype() == LISTTYPE::MODEL){
        type.viewdirection = static_cast<OUTPUTDIR>(idx.at(0));
        type.viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
        //        type.isNormalDirection = (static_cast<OUTPUTDIR>(idx_a) == ODIR_PLOUT);//面内・面外のほかに部材軸方向とかある？
        QStringList model = getOutfigStringList(LISTTYPE::MODEL,2);
        if ( idx.at(1)<model.count()) {
            type.model = model.at(idx.at(1));
            type.scenetype = scene;//==SCENETYPE::PLANSCENE ? true : false;
            if(type.scenetype == SCENETYPE::PLANSCENE){
                type.name=ud->idToName(uid,DATATYPE::TPFLOOR);
            }else if(type.scenetype == SCENETYPE::ELEVATIONSCENE){
                type.name=ud->idToName(uid,DATATYPE::TPFRAMEP);
            }else{type.name = u8"";}
        }
        return type;
    }

    if(type.listtype() == LISTTYPE::STRESS){
        type.viewdirection = static_cast<OUTPUTDIR>(idx.at(0));
        type.viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
        //        type.isNormalDirection = (static_cast<OUTPUTDIR>(idx_a) == ODIR_PLOUT);

        //idx_a : 面内面外
        //idx_b : CaseE1とか
        //step  : ステップ数
        //combiのvalidチェックは文字列登録時に行う
        STRESSCOMBISETTINGS scset = UnifiedProject::getInstance()->getStressCombinationSetting();
        QStringList validcombi = getOutfigStringList(LISTTYPE::STRESS,2);//CaseDL...の中で、Pstnに含まれるもの
        QStringList allcombi =scset.combi;//CaseDL,CaseE1...
        QList<int> steplist = scset.step;//0,1,1,3,3,3,...
        QList<STRESS> sts;
        QString stress_text;
        if (true) {
            qDebug()<<"validcombi count="<<validcombi.count();
            QString key = validcombi.at(idx.at(1));//CaseE1とか
            type.caseName=key;

            qDebug()<<"selected case name = "<<key;
            QList<qreal> coef = scset.coef.value(key);//1.0*RSTDL+1.0*RSTLL1...のreal列
            qDebug()<<"coef count="<<coef.count()<<coef;
            QStringList stressList = scset.stress.value(key);//1.0*RSTDL+1.0*RSTLL1...のRST列
            qDebug()<<"stressList count="<<stressList.count();
            //            steplist.at(idx_b)==0 -> nashi;
            QString stressStepName=QString();
            if(steplist.at(idx.at(1)) > 0) stressStepName=stressList.at(steplist.at(idx.at(1)) - 1);

            for(int i = 0;i<stressList.count();i++){
                STRESS st;
                st.rst = stressList.at(i);
                st.coef = coef.at(i);

                if(stressList.at(i)==stressStepName){
                    st.setStep(step, UnifiedFixedData::getInstance()->getPstnData()->getMaxStep(st.rst));
                }else{
                }
                sts << st;
            }
            type.stress.clear();
            type.stress << sts;
            type.scenetype = scene;//==SCENETYPE::PLANSCENE ? true : false;
            if(type.scenetype == SCENETYPE::PLANSCENE){
                type.name=ud->idToName(uid,DATATYPE::TPFLOOR);;
            }else if(type.scenetype == SCENETYPE::ELEVATIONSCENE){
                type.name=ud->idToName(uid,DATATYPE::TPFRAMEP);
            }else{type.name = u8"";}

        }
        return type;
    }

    if(type.listtype() == LISTTYPE::CHCK){
        type.viewdirection = static_cast<OUTPUTDIR>(idx.at(0));
        type.viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
        //        type.isNormalDirection = (static_cast<OUTPUTDIR>(idx_a) == ODIR_PLOUT);
        QStringList chck = getOutfigStringList(LISTTYPE::CHCK,2);
        if ( idx.at(1)<=chck.count()) {
            type.caseName=chck.at(idx.at(1));
            type.scenetype = scene;//==SCENETYPE::PLANSCENE ? true : false;
            if(type.scenetype == SCENETYPE::PLANSCENE){
                type.name=ud->idToName(uid,DATATYPE::TPFLOOR);
            }else if(type.scenetype == SCENETYPE::ELEVATIONSCENE){
                type.name=ud->idToName(uid,DATATYPE::TPFRAMEP);
            }else{type.name = u8"";}
            //type.model;
            //mdlch(0) - 固定分応力解析モデル、mdlch(1) - 変動分応力解析モデル、mkcch()・・・モデル番号？
        }

        return type;
    }
    if(type.listtype() == LISTTYPE::COMBIMAP){
        QStringList chcklist = getOutfigStringList(LISTTYPE::COMBIMAP,2);
        if ( idx.at(0)<=chcklist.count()&&idx.at(1)<=chcklist.count()&&idx.at(2)<=chcklist.count()) {
            QString case1=chcklist.at(idx.at(0));
            QString case2=chcklist.at(idx.at(1));
            QString case3=chcklist.at(idx.at(2));
            type.scenetype = scene;
            if(type.scenetype == SCENETYPE::PLANSCENE){
                type.name=ud->idToName(uid,DATATYPE::TPFLOOR);
            }else{type.name = u8"";}
            //type.model;
            //mdlch(0) - 固定分応力解析モデル、mdlch(1) - 変動分応力解析モデル、mkcch()・・・モデル番号？
        }


        auto chck = UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();
        //        QList<int> idx=QList<int>()<<idx_a<<idx_b<<idx_c<<idx_d<<idx_e;
        QList<QList<STRESS>> stressx5;
        for(int loop=0;loop<5;loop++){
            QList<STRESS> stress;
            int nch = idx.at(loop);
            for(int nrsch=0;nrsch<chck.nrsch.at(nch);nrsch++){
                STRESS st;
                st.coef = chck.corsch.at(nch).at(nrsch);
                st.rst = chck.nmrsch.at(nch).at(nrsch);
                stress<<st;
            }
            stressx5<<stress;
        }
        type.stress = stressx5;
    }



    if(type.listtype() == LISTTYPE::QUDS){
        qDebug()<<"OutputControl::makeFigureTypeProperty QUDS";
        type.viewdirection = static_cast<OUTPUTDIR>(idx.at(0));
        type.viewtype = MODELVIEWTYPE::OUTPUT2DMODEL;
        //        type.isNormalDirection = (static_cast<OUTPUTDIR>(idx_a) == ODIR_PLOUT);
        QStringList quds = getOutfigStringList(LISTTYPE::QUDS,2);

        qDebug()<<"OutputControl::makeFigureTypeProperty QUDS get string list";
        if (idx.at(1)<=quds.count()) {
            type.caseName=quds.at(idx.at(1));
            type.scenetype = scene;//==SCENETYPE::PLANSCENE ? true : false;
            if(type.scenetype == SCENETYPE::PLANSCENE){
                type.name=ud->idToName(uid,DATATYPE::TPFLOOR);
            }else if(type.scenetype == SCENETYPE::ELEVATIONSCENE){
                type.name=ud->idToName(uid,DATATYPE::TPFRAMEP);
            }else{type.name = u8"";}
            //type.stress; ケース名とtypeだけで特定できる？
            //UnifiedOutput::getInstance()->getQuDsData(type.caseName).dsFileName;
        }
        qDebug()<<"OutputControl::makeFigureTypeProperty QUDS return";

        return type;
    }
    return type;
    //rank図のとき,bがcasename
    //    if(myFigureType == OUTFIG_RANK){
    //        QString fileName;
    //        if(idx_b == 0){
    //            appendFaceMemberForce(  uid, st,  type.isNormalDirection );
    //        }else{
    //            QString casename = UnifiedOutput::getInstance()->getQuDsCaseList().at(idx_b-1);
    //            fileName = UnifiedOutput::getInstance()->getQuDsData(casename).dsFileName;
    //            appendFaceMemberForce(  uid, getQuDsStressList(fileName),  type.isNormalDirection );
    //        }
    //        return;
    //    }}


}

FIGURE_TYPE_PROPERTY OutputControl::ChangeCalcLoadFigureTypeProperty(const FIGURE_TYPE_PROPERTY &proprety, int loadType, int loadSum, int direction)
{
    FIGURE_TYPE_PROPERTY figureTypeProperty(proprety);
    figureTypeProperty.loadType = static_cast<LoadType::LOADTYPE>(loadType);
    figureTypeProperty.loadSumType = static_cast<LOADSUMTYPE>(loadSum);
    figureTypeProperty.viewdirection = static_cast<OUTPUTDIR>(direction);
    return figureTypeProperty;
}



} // namespace post3dapp
