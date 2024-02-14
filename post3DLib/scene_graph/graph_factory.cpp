#include "graph_factory.h"

#include "fixed_data.h"
#include "graph_3d_scene.h"
#include "handle_npstn.h"
#include "manage_memberdata.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp{
GraphFactory::GraphFactory()
{

}

Graph GraphFactory::makeOneGraph(const Analysis2DViewSettings& setting,const GRAPH_SUBTYPE_PROPERTY& type,
                                 const GRAPH_SIZE_OPTION& size){
    qDebug()<<"GraphScene::makeOneGraph"<<size.targetRect
           <<(int)type.type;
    switch(type.type){
    case OUTFIGTYPE::UNDEFINED:
        return makeNullGraph(size);
    case OUTFIGTYPE::QDelta:
        return makeQDeltaGraph(*static_cast<AnalysisGraphQDelta2DViewSetting*>(setting.getSetting(OUTFIGTYPE::QDelta)),type,size);
    case OUTFIGTYPE::QTheta:
        return makeQDeltaGraph(*static_cast<AnalysisGraphQDelta2DViewSetting*>(setting.getSetting(OUTFIGTYPE::QTheta)),type,size);
    case OUTFIGTYPE::QDeltaFrameFloors:
        return makeQDeltaFrameFloorsGraph(*static_cast<AnalysisGraphQDelta2DViewSetting*>(setting.getSetting(OUTFIGTYPE::QDeltaFrameFloors)),type,size);
    case OUTFIGTYPE::QThetaFrameFloors:
        return makeQDeltaFrameFloorsGraph(*static_cast<AnalysisGraphQDelta2DViewSetting*>(setting.getSetting(OUTFIGTYPE::QThetaFrameFloors)),type,size);
    case OUTFIGTYPE::QDeltaFloorFrames:
        return makeQDeltaFloorFramesGraph(*static_cast<AnalysisGraphQDelta2DViewSetting*>(setting.getSetting(OUTFIGTYPE::QDeltaFloorFrames)),type,size);
    case OUTFIGTYPE::QThetaFloorFrames:
        return makeQDeltaFloorFramesGraph(*static_cast<AnalysisGraphQDelta2DViewSetting*>(setting.getSetting(OUTFIGTYPE::QThetaFloorFrames)),type,size);
    case OUTFIGTYPE::QuNeedf:
        return makeHorizontalCapacityGraph(*static_cast<AnalysisGraphHorizontalCapacity2DViewSetting*>(setting.getSetting(OUTFIGTYPE::QuNeedf)),type,size);
    case OUTFIGTYPE::NMINT:
        qDebug()<<"nmint graph";
        return makeNmintGraph_CHCK(*static_cast<AnalysisGraphNmint2DViewSetting*>(setting.getSetting(OUTFIGTYPE::NMINT)),type,size);
        //        return makeNmintGraph(*static_cast<OutputGraphNmint2DViewSetting*>(setting.getSetting(OUTFIGTYPE::NMINT)),type,size);
    default:
        qDebug()<<"unknown graph";
        throw;

    }
}
QString GraphFactory::makeGraphTitle(const GRAPH_SUBTYPE_PROPERTY& type)
{

    //    return u8"dummy title";

    bool isTheta = false;
    if(type.type==OUTFIGTYPE::QTheta)isTheta=true;

    auto rstqdHash = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData();
    QUDSDATA quds = UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(type.caseName);
    qreal angle = quds.angle;
    QString filename = type.qu_or_ds == BASE_TYPE_PROPERTY::DSQU::QU ? quds.quFileName : quds.dsFileName;
    QString addstr = type.qu_or_ds == BASE_TYPE_PROPERTY::DSQU::QU ?u8"保有水平耐力算定用":u8"DS算定用";
    QString frame=type.frameName;
    QString floor=type.floorName;

    switch(type.type){
    case OUTFIGTYPE::QTheta:
        return u8"層せん断力-層間変形角関係 ("+filename+u8" ： "+QString::number(angle)+u8"°方向)"+addstr;
    case OUTFIGTYPE::QDelta:
        return u8"層せん断力-層間変位関係 ("+filename+u8" ： "+QString::number(angle)+u8"°方向)"+addstr;
    case OUTFIGTYPE::QThetaFrameFloors:
        return u8"層せん断力-層間変形角関係 ("+filename+u8" : "+frame+u8" 通り "+QString::number(angle)+u8"°方向)";
    case OUTFIGTYPE::QDeltaFrameFloors:
        return u8"層せん断力-層間変位関係 ("+filename+u8" : "+frame+u8" 通り "+QString::number(angle)+u8"°方向)";
    case OUTFIGTYPE::QThetaFloorFrames:
        return u8"層せん断力-層間変形角関係 ("+filename+u8" : "+floor+u8" 階 "+QString::number(angle)+u8"°方向)";
    case OUTFIGTYPE::QDeltaFloorFrames:
        return u8"層せん断力-層間変位関係 ("+filename+u8" : "+floor+u8" 階 "+QString::number(angle)+u8"°方向)";
    case OUTFIGTYPE::QuNeedf:
        return u8"必要保有水平耐力と保有水平耐力の比較 ("+filename+u8" ： "+QString::number(angle)+u8"°方向)";
    case OUTFIGTYPE::NMINT:
    {
        QString icjstr = QString();
        if(type.icj == GRAPH_SUBTYPE_PROPERTY::I)icjstr = u8"（I端フェイス位置）";
        if(type.icj == GRAPH_SUBTYPE_PROPERTY::C)icjstr = u8"（中央位置）";
        if(type.icj == GRAPH_SUBTYPE_PROPERTY::J)icjstr = u8"（J端フェイス位置）";
        MemberData* md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(type.memberId);
        QString secname ;
        QString buzaistr ;
        if(md!=nullptr){
            secname = md->sectionName();
            buzaistr = u8"部材番号"+QString::number(md->getMemberNo());
        }
        return u8"NM-インタラクション表示 "+secname +u8" "+buzaistr+ u8" "+icjstr + type.caseName;
    }
    default:
        return QString();
    }

}
Graph GraphFactory::makeNullGraph(const GRAPH_SIZE_OPTION& size){
    return Graph(QList<Series>(), size,
                 u8"DUMMY GRAPH", u8"X", u8"Y");

}
Graph GraphFactory::makeQDeltaGraph(const AnalysisGraphQDelta2DViewSetting& setting,const GRAPH_SUBTYPE_PROPERTY& type,
                                    const GRAPH_SIZE_OPTION& size){
    //pstnの所定のデータがない場合はダミーを返す
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_QUDS))
        return makeNullGraph(size);
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_RSTQD))
        return makeNullGraph(size);

    qDebug()<<"GraphScene::makeQDeltaGraph"<<size.targetRect;
    //通りごとのQDelta、QTheta
    bool isTheta = false;
    if(type.type==OUTFIGTYPE::QTheta)
        isTheta=true;

    auto rstqdHash = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData();
    QUDSDATA quds = UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(type.caseName);
    qreal angle = quds.angle;
    QString filename = type.qu_or_ds == BASE_TYPE_PROPERTY::DSQU::QU ? quds.quFileName : quds.dsFileName;
    QString addstr = type.qu_or_ds == BASE_TYPE_PROPERTY::DSQU::QU ?u8"保有水平耐力算定用":u8"DS算定用";

    if (!rstqdHash.contains(filename)){
        return makeNullGraph(size);
    }
    else{
        auto rstqd = rstqdHash.value(filename);
        QList<Series> series;
        Series ss;
        ss.pen=QPen(setting.getPlasticStepColor(),0,Qt::DashLine);

        /* Q-Delta each floor */
        for (int i = 0; i < rstqd.ualst.at(0).at(0).count() - 1; i++){
            Series s;
            bool added_qun = false;

            s.name = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().at(i).floorName;
            s.plotPoints.append(QPointF());

            int frameMp = rstqd.iflhng.at(0).at(i).at(3);
            int frameQy = rstqd.iflhng.at(0).at(i).at(4);
            int wallQcrack = rstqd.iflhng.at(0).at(i).at(6);
            int wallQy = rstqd.iflhng.at(0).at(i).at(8);
            qreal qq0=0;
            qreal dd0=0;
            for (int j = 0; j < rstqd.nstp; j++){
                auto floor = rstqd.ualst.at(j).at(0).at(i);
                double dx = floor.at(7) * 1000.; //m -> mm
                double dy = floor.at(8) * 1000.; //m -> mm
                double th = floor.at(2);
                double dd = dx * qCos(th * 180. / M_PI) + dy * qSin(th * 180. / M_PI);
                if(isTheta)dd=floor.at(11) * 1000.0;//δ/H
                double qq = floor.at(3); //kN
                s.plotPoints.append(QPointF(dd, qq));
                //                if(qq>570 && !added_qun){
                if(qq>rstqd.rfgqun.at(0).at(i).at(15) && !added_qun){
                    //内分点
                    qreal t=(qq-rstqd.rfgqun.at(0).at(i).at(15))/(qq-qq0);
                    QPointF mid=t*QPointF(dd0,qq0)+(1.0-t)*QPointF(dd,qq);
                    ss.plotPoints.append(mid);
                    added_qun = true;
                }
                qq0=qq;
                dd0=dd;
                //Dotの属性
                if (0 < frameMp && j == frameMp){
                    Dot d=Dot( QPointF(dd, qq), DotType::Circle);
                    d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                    d.symbolSize=setting.getSymbolSize();
                    d.symbolColor=setting.getPlasticSymbolColor();
                    s.dots.append(d);
                }
                if (0 < frameQy && j == frameQy){
                    Dot d=Dot( QPointF(dd, qq), DotType::BlackCircle);
                    d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                    d.symbolSize=setting.getSymbolSize();
                    d.symbolColor=setting.getPlasticSymbolColor();
                    s.dots.append(d);
                }
                if (0 < wallQcrack && j == wallQcrack){
                    Dot d=Dot( QPointF(dd, qq), DotType::Rectangle);
                    d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                    d.symbolSize=setting.getSymbolSize();
                    d.symbolColor=setting.getPlasticSymbolColor();
                    s.dots.append(d);
                }
                if (0 < wallQy && j == wallQy){
                    Dot d=Dot( QPointF(dd, qq), DotType::BlackRectangle);
                    d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                    d.symbolSize=setting.getSymbolSize();
                    d.symbolColor=setting.getPlasticSymbolColor();
                    s.dots.append(d);
                }
            }
            series.append(s);
        }
        series.append(ss);

        Graph graph;
        GRAPH_SIZE_OPTION sizecopy(size);

        if(isTheta){
            sizecopy.scaleAutoX = setting.getMinMaxAutoRad() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_xmin = setting.getMinMaxMinRad();
            sizecopy.fixed_xmax = setting.getMinMaxMaxRad();
            sizecopy.scaleAutoY = setting.getMinMaxAutoQ() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_ymin = setting.getMinMaxMinQ();
            sizecopy.fixed_ymax = setting.getMinMaxMaxQ();
            graph = Graph(series, sizecopy,
                          u8"Q-θ("+filename+u8" ： "+QString::number(angle)+u8"°方向)"+addstr, u8"θ(10^3 rad)", u8"Q(kN)");
        }else{
            sizecopy.scaleAutoX = setting.getMinMaxAutoD() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_xmin = setting.getMinMaxMinD();
            sizecopy.fixed_xmax = setting.getMinMaxMaxD();
            sizecopy.scaleAutoY = setting.getMinMaxAutoQ() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_ymin = setting.getMinMaxMinQ();
            sizecopy.fixed_ymax = setting.getMinMaxMaxQ();
            graph = Graph(series, sizecopy,
                          u8"Q-D("+filename+u8" ： "+QString::number(angle)+u8"°方向)"+addstr, u8"D(mm)", u8"Q(kN)");
        }

        //targetRectなどは上で計算されて渡ってくる。ユーザーが決めるものは基本的にsettingに落とし込む。
        //GRAPH_SIZE_OPTIONはウィジェット系のインターフェースから消して、自動計算、settingの内容、図によって固定のものを統合して最終的にGraphに渡す箱にする
        //Graph共通の属性
        graph.setTitleFont(QFont("Meiryo",1.5*(5+setting.getTitleTextSize())));
        graph.setTitleVisible(setting.getTitleVisible());
        graph.setAxisTextFont(QFont("Meiryo",(5+setting.getAxisScaleTextSize())));
        graph.setAxisValueFont(QFont("Meiryo",(5+setting.getAxisLabelSize())));
        if(setting.getRuledLineVisible()){
            graph.setRulerPen(QPen(setting.getRuledLineColor(),0,setting.getRuledLineStyle()));
        }else{
            graph.setRulerPen(QPen(setting.getRuledLineColor(),0,Qt::NoPen));
        }

        return graph;


    }
}
Graph GraphFactory::makeQDeltaFrameFloorsGraph(const AnalysisGraphQDelta2DViewSetting& setting,const GRAPH_SUBTYPE_PROPERTY& type,
                                               const GRAPH_SIZE_OPTION& size){
    //pstnの所定のデータがない場合はダミーを返す
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_QUDS)){return makeNullGraph(size);}
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_RSTQD)){return makeNullGraph(size);}
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_MODEL)){return makeNullGraph(size);}

    //通りごとのQDelta、QTheta
    bool isTheta = false;
    if(type.type==OUTFIGTYPE::QThetaFrameFloors)isTheta=true;
    auto rstqdHash = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData();
    QUDSDATA quds = UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(type.caseName);
    qreal angle = quds.angle;
    int sign = 1.0;
    if(qSin(angle*2.0*M_PI/360) < -qPow(2.0,-0.5) || qCos(angle*2.0*M_PI/360) < -qPow(2.0,-0.5))sign = -1;

    QString filename = type.qu_or_ds == BASE_TYPE_PROPERTY::DSQU::QU ? quds.quFileName : quds.dsFileName;
    QString frame=type.frameName;

    if (!rstqdHash.contains(filename)){

        return makeNullGraph(size);
    }
    else{
        auto rstqd = rstqdHash.value(filename);
        QList<Series> series;
        //        QString frame = "X1";
        int frameGroup = 0;
        int frameNumber = 0;
        auto cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
        for (int i = 0; i < cmdl.namegr.count(); i++){
            for (int j = 0; j < cmdl.namegr.at(i).count(); j++){
                if (frame == cmdl.namegr.at(i).at(j)){
                    frameGroup = i;
                    frameNumber = j;
                }
            }
        }

        for (int i = 0; i < UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().count() - 1; i++){
            Series s;
            s.name = UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList().at(i).floorName;
            s.plotPoints.append(QPointF());

            int frameMp = rstqd.igrhng.at(frameGroup).at(frameNumber).at(0).at(i).at(3);
            int frameQy = rstqd.igrhng.at(frameGroup).at(frameNumber).at(0).at(i).at(4);
            int wallQcrack = rstqd.igrhng.at(frameGroup).at(frameNumber).at(0).at(i).at(6);
            int wallQy = rstqd.igrhng.at(frameGroup).at(frameNumber).at(0).at(i).at(8);
            for (int j = 0; j < rstqd.nstp; j++){
                auto grid = rstqd.ualgr.at(j).at(frameGroup).at(frameNumber).at(0).at(i);
                double dx = sign*grid.at(6) * 1000.; //m -> mm
                if(isTheta)dx = qAbs(grid.at(7)<0.000000001) ? 0.0 : sign*1000.0/grid.at(7); //δ/H
                double qq = sign*grid.at(2); //kN
                s.plotPoints.append(QPointF(dx, qq));


                //Dotの属性
                if (0 < frameMp && j == frameMp){
                    Dot d=Dot( QPointF(dx, qq), DotType::Circle);
                    d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                    d.symbolSize=setting.getSymbolSize();
                    d.symbolColor=setting.getPlasticSymbolColor();
                    s.dots.append(d);
                }
                if (0 < frameQy && j == frameQy){
                    Dot d=Dot( QPointF(dx, qq), DotType::BlackCircle);
                    d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                    d.symbolSize=setting.getSymbolSize();
                    d.symbolColor=setting.getPlasticSymbolColor();
                    s.dots.append(d);
                }
                if (0 < wallQcrack && j == wallQcrack){
                    Dot d=Dot( QPointF(dx, qq), DotType::Rectangle);
                    d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                    d.symbolSize=setting.getSymbolSize();
                    d.symbolColor=setting.getPlasticSymbolColor();
                    s.dots.append(d);
                }
                if (0 < wallQy && j == wallQy){
                    Dot d=Dot( QPointF(dx, qq), DotType::BlackRectangle);
                    d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                    d.symbolSize=setting.getSymbolSize();
                    d.symbolColor=setting.getPlasticSymbolColor();
                    s.dots.append(d);
                }
            }
            series.append(s);
        }

        Graph graph;
        GRAPH_SIZE_OPTION sizecopy(size);

        if(isTheta){
            sizecopy.scaleAutoX = setting.getMinMaxAutoRad() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_xmin = setting.getMinMaxMinRad();
            sizecopy.fixed_xmax = setting.getMinMaxMaxRad();
            sizecopy.scaleAutoY = setting.getMinMaxAutoQ() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_ymin = setting.getMinMaxMinQ();
            sizecopy.fixed_ymax = setting.getMinMaxMaxQ();
            graph = Graph(series, sizecopy,
                          u8"Q-θ ("+filename+u8" : "+frame+u8" 通り "+QString::number(angle)+u8"°方向)", u8"θ(10^3 rad)", u8"Q(kN)");
        }else{
            sizecopy.scaleAutoX = setting.getMinMaxAutoD() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_xmin = setting.getMinMaxMinD();
            sizecopy.fixed_xmax = setting.getMinMaxMaxD();
            sizecopy.scaleAutoY = setting.getMinMaxAutoQ() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_ymin = setting.getMinMaxMinQ();
            sizecopy.fixed_ymax = setting.getMinMaxMaxQ();
            graph = Graph(series, sizecopy,
                          u8"Q-D ("+filename+u8" : "+frame+u8" 通り "+QString::number(angle)+u8"°方向)", u8"D(mm)", u8"Q(kN)");
        }

        //targetRectなどは上で計算されて渡ってくる。ユーザーが決めるものは基本的にsettingに落とし込む。
        //GRAPH_SIZE_OPTIONはウィジェット系のインターフェースから消して、自動計算、settingの内容、図によって固定のものを統合して最終的にGraphに渡す箱にする
        //Graph共通の属性
        graph.setTitleFont(QFont("Meiryo",1.5*(5+setting.getTitleTextSize())));
        graph.setTitleVisible(setting.getTitleVisible());
        graph.setAxisTextFont(QFont("Meiryo",(5+setting.getAxisScaleTextSize())));
        graph.setAxisValueFont(QFont("Meiryo",(5+setting.getAxisLabelSize())));
        if(setting.getRuledLineVisible()){
            graph.setRulerPen(QPen(setting.getRuledLineColor(),0,setting.getRuledLineStyle()));
        }else{
            graph.setRulerPen(QPen(setting.getRuledLineColor(),0,Qt::NoPen));
        }

        return graph;

    }
}
Graph GraphFactory::makeQDeltaFloorFramesGraph(const AnalysisGraphQDelta2DViewSetting& setting,const GRAPH_SUBTYPE_PROPERTY& type,
                                               const GRAPH_SIZE_OPTION& size){
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_MODEL)){return makeNullGraph(size);}
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_QUDS)){return makeNullGraph(size);}
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_RSTQD)){return makeNullGraph(size);}

    //階ごとのQDelta、QTheta
    bool isTheta = false;
    if(type.type==OUTFIGTYPE::QThetaFloorFrames)isTheta=true;

    auto rstqdHash = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData();
    QUDSDATA quds = UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(type.caseName);
    qreal angle = quds.angle;//-45 ～ +135
    int sign = 1.0;
    if(qSin(angle*2.0*M_PI/360) < -qPow(2.0,-0.5) || qCos(angle*2.0*M_PI/360) < -qPow(2.0,-0.5))sign = -1;
    QString filename = type.qu_or_ds == BASE_TYPE_PROPERTY::DSQU::QU ? quds.quFileName : quds.dsFileName;
    QString floor=type.floorName;
    QString frameGroupName = type.frameGroupName;

    if (!rstqdHash.contains(filename)){

        return makeNullGraph(size);
    }
    else{
        auto rstqd = rstqdHash.value(filename);
        QList<Series> series;
        int floorGroup = 0;
        int floorNumber = 0;
        auto cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();

        for (int i = 0; i < cmdl.namefl.count(); i++){
            for (int j = 0; j < cmdl.namefl.at(i).count(); j++){
                if (floor == cmdl.namefl.at(i).at(j)){
                    floorGroup = i;
                    floorNumber = j;
                }
            }
        }


        for (int frameGroup=0; frameGroup < cmdl.namegr.count(); frameGroup++){
            //            if(frameGroup != 0)break;
            for (int frameNumber=0; frameNumber < cmdl.namegr.at(frameGroup).count(); frameNumber++){
                if(cmdl.namegr.at(frameGroup).at(frameNumber).left(1) != frameGroupName)continue;
                Series s;
                s.name = cmdl.namegr.at(frameGroup).at(frameNumber);
                s.plotPoints.append(QPointF());

                int frameMp = rstqd.igrhng.at(frameGroup).at(frameNumber).at(floorGroup).at(floorNumber).at(3);
                int frameQy = rstqd.igrhng.at(frameGroup).at(frameNumber).at(floorGroup).at(floorNumber).at(4);
                int wallQcrack = rstqd.igrhng.at(frameGroup).at(frameNumber).at(floorGroup).at(floorNumber).at(6);
                int wallQy = rstqd.igrhng.at(frameGroup).at(frameNumber).at(floorGroup).at(floorNumber).at(8);
                for (int j = 0; j < rstqd.nstp; j++){
                    auto grid = rstqd.ualgr.at(j).at(frameGroup).at(frameNumber).at(floorGroup).at(floorNumber);
                    double dx = sign*grid.at(6) * 1000.; //m -> mm
                    if(isTheta)dx = qAbs(grid.at(7)<0.000000001) ? 0.0 : sign*1000.0/grid.at(7); //δ/H
                    double qq = sign*grid.at(2); //kN
                    s.plotPoints.append(QPointF(dx, qq));

                    //Dotの属性
                    if (0 < frameMp && j == frameMp){
                        Dot d=Dot( QPointF(dx, qq), DotType::Circle);
                        d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                        d.symbolSize=setting.getSymbolSize();
                        d.symbolColor=setting.getPlasticSymbolColor();
                        s.dots.append(d);
                    }
                    if (0 < frameQy && j == frameQy){
                        Dot d=Dot( QPointF(dx, qq), DotType::BlackCircle);
                        d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                        d.symbolSize=setting.getSymbolSize();
                        d.symbolColor=setting.getPlasticSymbolColor();
                        s.dots.append(d);
                    }
                    if (0 < wallQcrack && j == wallQcrack){
                        Dot d=Dot( QPointF(dx, qq), DotType::Rectangle);
                        d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                        d.symbolSize=setting.getSymbolSize();
                        d.symbolColor=setting.getPlasticSymbolColor();
                        s.dots.append(d);
                    }
                    if (0 < wallQy && j == wallQy){
                        Dot d=Dot( QPointF(dx, qq), DotType::BlackRectangle);
                        d.font = QFont("Meiryo", 5+setting.getSymbolLabelSize());
                        d.symbolSize=setting.getSymbolSize();
                        d.symbolColor=setting.getPlasticSymbolColor();
                        s.dots.append(d);
                    }
                }
                series.append(s);
            }
        }
        Graph graph;
        GRAPH_SIZE_OPTION sizecopy(size);

        if(isTheta){
            sizecopy.scaleAutoX = setting.getMinMaxAutoRad() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_xmin = setting.getMinMaxMinRad();
            sizecopy.fixed_xmax = setting.getMinMaxMaxRad();
            sizecopy.scaleAutoY = setting.getMinMaxAutoQ() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_ymin = setting.getMinMaxMinQ();
            sizecopy.fixed_ymax = setting.getMinMaxMaxQ();
            graph = Graph(series, sizecopy,
                          u8"Q-θ ("+filename+u8" : "+floor+u8" 階 "+QString::number(angle)+u8"°方向)", u8"θ(10^3 rad)", u8"Q(kN)");
        }else{
            sizecopy.scaleAutoX = setting.getMinMaxAutoD() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_xmin = setting.getMinMaxMinD();
            sizecopy.fixed_xmax = setting.getMinMaxMaxD();
            sizecopy.scaleAutoY = setting.getMinMaxAutoQ() ? GRAPH_SIZE_OPTION::JIKU_AUTO : GRAPH_SIZE_OPTION::JIKU_FIX;
            sizecopy.fixed_ymin = setting.getMinMaxMinQ();
            sizecopy.fixed_ymax = setting.getMinMaxMaxQ();
            graph = Graph(series, sizecopy,
                          u8"Q-D ("+filename+u8" : "+floor+u8" 階 "+QString::number(angle)+u8"°方向)", u8"D(mm)", u8"Q(kN)");
        }

        //targetRectなどは上で計算されて渡ってくる。ユーザーが決めるものは基本的にsettingに落とし込む。
        //GRAPH_SIZE_OPTIONはウィジェット系のインターフェースから消して、自動計算、settingの内容、図によって固定のものを統合して最終的にGraphに渡す箱にする
        //Graph共通の属性
        graph.setTitleFont(QFont("Meiryo",1.5*(5+setting.getTitleTextSize())));
        graph.setTitleVisible(setting.getTitleVisible());
        graph.setAxisTextFont(QFont("Meiryo",(5+setting.getAxisScaleTextSize())));
        graph.setAxisValueFont(QFont("Meiryo",(5+setting.getAxisLabelSize())));
        if(setting.getRuledLineVisible()){
            graph.setRulerPen(QPen(setting.getRuledLineColor(),0,setting.getRuledLineStyle()));
        }else{
            graph.setRulerPen(QPen(setting.getRuledLineColor(),0,Qt::NoPen));
        }

        return graph;
    }
}
Graph GraphFactory::makeHorizontalCapacityGraph(const AnalysisGraphHorizontalCapacity2DViewSetting& setting,const GRAPH_SUBTYPE_PROPERTY& type,
                                                const GRAPH_SIZE_OPTION& size){
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_MODEL)){return makeNullGraph(size);}
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_QUDS)){return makeNullGraph(size);}
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_RSTQD)){return makeNullGraph(size);}
    //X軸：層せん断力(kN)
    //Y軸：階名
    //縦軸がStringの散布図。プロット点は階と階の中間点と、必要保有水平耐力・保有水平耐力
    //通りごとのQDelta、QTheta




    auto rstqdHash = UnifiedFixedData::getInstance()->getPstnData()->getNbldRstQDData();
    QUDSDATA quds = UnifiedFixedData::getInstance()->getPstnData()->getQuDsData(type.caseName);
    qreal angle = quds.angle;
    QString filename = quds.quFileName ;

    if (!rstqdHash.contains(filename)){

        return makeNullGraph(size);
    }
    else{
        QList<Series> series;

        auto rstqd = rstqdHash.value(filename);
        auto cmdl = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
        Series s_qun;
        Series s_qu;
        QList<QStringList> floors;
        QStringList tmp;
        for (int i = 0; i < cmdl.namefl.count(); i++){
            for (int j = 0; j < cmdl.namefl.at(i).count(); j++){
                tmp<<cmdl.namefl.at(i).at(j);
            }
            floors<<tmp;
            tmp.clear();
        }


        for(int floorGroup=0;floorGroup<floors.count();floorGroup++){
            for(int floorNumber=0;floorNumber<floors.at(floorGroup).count()-1;floorNumber++){
                qreal qun = rstqd.rfgqun.at(floorGroup).at(floorNumber).at(15);//qun
                qreal qu = rstqd.rfgqun.at(floorGroup).at(floorNumber).at(16);//qu
                s_qun.plotPoints.append(QPointF(qun, floorNumber+0.5));
                s_qu.plotPoints.append(QPointF(qu, floorNumber+0.5));

            }
            if(setting.getVisibleUn()){
                s_qun.pen = QPen(setting.getColorUn(),0,setting.getStyleUn());
            }else{
                s_qun.pen = QPen(Qt::NoPen);

            }
            if(setting.getVisibleU()){
                s_qu.pen = QPen(setting.getColorU(),0,setting.getStyleU());
            }else{
                s_qu.pen = QPen(Qt::NoPen);
            }
            series.append(s_qun);
            series.append(s_qu);
        }
        GRAPH_SIZE_OPTION copy(size);
        copy.useStringJikuY=true;
        copy.stringJikuY=floors.at(0);

        if(setting.getMinMaxAutoQ()){
            copy.scaleAutoX=GRAPH_SIZE_OPTION::JIKU_BOTTOMFIX;
            copy.fixed_xmin=0.0;
        }else{
            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_xmin = setting.getMinMaxMinQ();
            copy.fixed_xmax = setting.getMinMaxMaxQ();
        }

        Graph graph(series, copy,
                    u8"Qu,Qun 比較図("+filename+u8" ： "+QString::number(angle)+u8"°方向)", u8"層せん断力(kN)", u8"階");
        graph.setTitleFont(QFont("Meiryo",1.5*(5+setting.getTitleLabelSize())));
        graph.setTitleVisible(setting.getTitleLabelSize());
        graph.setAxisTextFont(QFont("Meiryo",(5+setting.getAxisLabelSize())));
        graph.setAxisValueFont(QFont("Meiryo",(5+setting.getAxisScaleTextSize())));
        if(setting.getVisibleRuler()){
            graph.setRulerPen(QPen(setting.getColorRuler(),0,setting.getStyleRuler()));
        }else{
            graph.setRulerPen(QPen(setting.getColorRuler(),0,Qt::NoPen));
        }

        return graph;

    }

}
Graph GraphFactory::makeNmintGraph_CHCK(const AnalysisGraphNmint2DViewSetting& setting,const GRAPH_SUBTYPE_PROPERTY& type,
                                        const GRAPH_SIZE_OPTION& size){
    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_CHCK)){return makeNullGraph(size);}

    //CHCKベースのNMINTグラフ作成
    int cnt=0;
    cnt++;qDebug()<<"NmintGraph"<<cnt;
    QString namech=type.caseName;
    auto chck = UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();

    cnt++;qDebug()<<"NmintGraph"<<cnt;
    QPen pen_L=QPen(Qt::NoPen);
    QPen pen_S=QPen(Qt::NoPen);
    QPen pen_U=QPen(Qt::NoPen);
    QPen pen_Kotei=QPen(Qt::NoPen);
    QPen pen_Hendou=QPen(Qt::NoPen);
    if(setting.getOutputLVisible())pen_L=QPen(setting.getOutputLColor(),0,setting.getOutputLStyle());
    if(setting.getOutputSVisible())pen_S=QPen(setting.getOutputSColor(),0,setting.getOutputSStyle());
    if(setting.getOutputUVisible())pen_U=QPen(setting.getOutputUColor(),0,setting.getOutputUStyle());
    if(setting.getKoteiVisible())pen_Kotei=QPen(setting.getKoteiColor(),0,setting.getKoteiStyle());
    if(setting.getHendouVisible())pen_Hendou=QPen(setting.getHendoColor(),0,setting.getHendouStyle());
    QList<QPen> pens;pens<<pen_L<<pen_S<<pen_U;

    QList<Series> series;


    cnt++;qDebug()<<"NmintGraph"<<cnt;
    if(type.memberId == QUuid())return Graph();
    MemberData* md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(type.memberId);
    int icj = static_cast<int>(type.icj);

    cnt++;qDebug()<<"NmintGraph"<<cnt;

    int nch = chck.namech.indexOf(namech);
    int lsu = chck.ich.at(nch).at(0);
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(md->getUuid());
    QVector<QVector<double>> mCurve_x;
    QVector<QVector<double>> mCurve_y;
    QVector<double> nCurve;
    for(int i= 0;i<chck.nrstm;i++){
        QVector<double> mTmpx;
        QVector<double> mTmpy;
        for(int j= 0;j<chck.nrstn;j++){
            mTmpx<<chck.tmsec.at(nch).at(numm).at(icj).at(i).at(j).at(0);
            mTmpy<<chck.tmsec.at(nch).at(numm).at(icj).at(i).at(j).at(1);
            nCurve<<chck.tnsec.at(nch).at(numm).at(icj).at(j);
        }
        mCurve_x<<mTmpx;
        mCurve_y<<mTmpy;
    }

    //MyとMzのグラフの最大値を合わせるため、ここで最大最小を拾ってキリよく切り上げておく
    qreal mmax=-1.0;
    for(int i=0;i<chck.nrstn;i++){
        for(int j=0;j<chck.nrstm;j++){
            mmax = qMax(mmax,qAbs(mCurve_x.at(j).at(i)));
            mmax = qMax(mmax,qAbs(mCurve_y.at(j).at(i)));
        }
    }
    int keta = (int)log10(mmax)+1;
    int delta = qPow(10,keta-1);
    qreal mm=0;
    for(int i=0;i<1000;i++){
        mm+=delta;
        if(mm>mmax)break;
    }
    mmax=mm;

    if(type.nm_type==GRAPH_SUBTYPE_PROPERTY::NMy){
        cnt++;qDebug()<<"NmintGraph"<<cnt;
        Series ss = sub_makeNMyCurve(pens.at(lsu-1),chck.nrstn,chck.nrstm,mCurve_x,nCurve);

        QList<Dot> dots;
        //応力組み合わせも、コンボボックスから選んだものを出すべき。VL+E1とか。
        QList<STRESS> stress;
        QList<STRESS> stress_kotei;

        for(int nrsch=0;nrsch<chck.nrsch.at(nch);nrsch++){
            STRESS st;
            st.coef = chck.corsch.at(nch).at(nrsch);
            st.rst = chck.nmrsch.at(nch).at(nrsch);
            stress<<st;
            if(0==chck.irsch.at(nch).at(nrsch)){//0...kotei,1...hendou
                stress_kotei<<st;
            }
        }
        //原点から固定分を足した点と、そこから変動分を足した点を線で結ぶ
        cnt++;qDebug()<<"NmintGraph"<<cnt;
        Dot dot_kotei=Dot::makeDot(type.nm_type,type.icj,
                                   namech,
                                   type.memberId,
                                   stress_kotei);

        cnt++;qDebug()<<"NmintGraph"<<cnt;
        Dot dot=Dot::makeDot(type.nm_type,type.icj,
                             namech,
                             type.memberId,
                             stress);
        dot.symbolColor=setting.getSymbolColor();
        dot.symbolSize=setting.getSymbolSize();
        dot.font=QFont("Meiryo",5+setting.getSymbolLabelSize());
        dots<<dot;
        ss.dots = dots;
        series.append(ss);

        Series line_kotei;
        Series line_hendou;
        line_kotei.pen=pen_Kotei;
        line_hendou.pen=pen_Hendou;
        line_kotei.plotPoints=QList<QPointF>()<<QPointF()<<dot_kotei.point;
        line_hendou.plotPoints=QList<QPointF>()<<dot_kotei.point<<dot.point;
        series.append(line_kotei);
        series.append(line_hendou);
        cnt++;qDebug()<<"NmintGraph"<<cnt;


        GRAPH_SIZE_OPTION copy(size);
        //        copy.plotAreaPaddings=QMarginsF(5,5,5,5);
        copy.aspectPolicy = GRAPH_SIZE_OPTION::FIX;
        copy.aspectRatio = 1.0;

        if(setting.getMinMaxAutoMy()){
            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_xmin = -qAbs(mmax);
            copy.fixed_xmax = qAbs(mmax);
//            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_TAISYOU;
        }else{
            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_xmin = setting.getMinMyValue();
            copy.fixed_xmax = setting.getMaxMyValue();
        }
        if(setting.getMinMaxAutoN()){
            copy.scaleAutoY = GRAPH_SIZE_OPTION::JIKU_TAISYOU;
        }else{
            copy.scaleAutoY = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_ymin = setting.getMinNValue();
            copy.fixed_ymax = setting.getMaxNValue();
        }
        cnt++;qDebug()<<"NmintGraph"<<cnt;

        Graph graph(series, copy,
                    u8"My-N ", u8"My(kNm)", u8"N(kN)");
        graph.setTitleFont(QFont("Meiryo",1.5*(5+setting.getTitleTextSize())));
        graph.setTitleVisible(setting.getTitleVisible());
        graph.setAxisTextFont(QFont("Meiryo",(5+setting.getAxisScaleTextSize())));
        graph.setAxisValueFont(QFont("Meiryo",(5+setting.getAxisLabelSize())));
        graph.setAxisColor(setting.getAxisColor());

        cnt++;qDebug()<<"NmintGraph"<<cnt;
        if(setting.getRulerVisible()){
            graph.setRulerPen(QPen(setting.getRulerColor(),0,setting.getRulerStyle()));
        }else{
            graph.setRulerPen(QPen(setting.getRulerColor(),0,Qt::NoPen));
        }
        cnt++;qDebug()<<"NmintGraph"<<cnt;

        return graph;

    }else if(type.nm_type==GRAPH_SUBTYPE_PROPERTY::NMz){
//        int nch = chck.namech.indexOf(namech);
//        int lsu = chck.ich.at(nch).at(0);
//        int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(md->getUuid())-1;
//        QVector<QVector<double>> mCurve;
//        QVector<double> nCurve;
//        for(int i= 0;i<chck.nrstm;i++){
//            QVector<double> mTmp;
//            for(int j= 0;j<chck.nrstn;j++){
//                mTmp<<chck.tmsec.at(nch).at(numm).at(icj).at(i).at(j).at(1);
//                nCurve<<chck.tnsec.at(nch).at(numm).at(icj).at(j);
//            }
//            mCurve<<mTmp;
//        }
        Series ss = sub_makeNMzCurve(pens.at(lsu-1),chck.nrstn,chck.nrstm,mCurve_y,nCurve);

        QList<Dot> dots;
        QList<STRESS> stress;
        QList<STRESS> stress_kotei;

        for(int nrsch=0;nrsch<chck.nrsch.at(nch);nrsch++){
            STRESS st;
            st.coef = chck.corsch.at(nch).at(nrsch);
            st.rst = chck.nmrsch.at(nch).at(nrsch);
            stress<<st;
            if(0==chck.irsch.at(nch).at(nrsch)){//0...kotei,1...hendou
                stress_kotei<<st;
            }
        }
        //原点から固定分を足した点と、そこから変動分を足した点を線で結ぶ
        Dot dot_kotei=Dot::makeDot(type.nm_type,type.icj,
                                   namech,
                                   type.memberId,
                                   stress_kotei);

        Dot dot=Dot::makeDot(type.nm_type,type.icj,
                             namech,
                             type.memberId,
                             stress);
        dot.symbolColor=setting.getSymbolColor();
        dot.symbolSize=setting.getSymbolSize();
        dot.font=QFont("Meiryo",5+setting.getSymbolLabelSize());
        dots<<dot;
        ss.dots = dots;
        series.append(ss);

        Series line_kotei;
        Series line_hendou;
        line_kotei.pen=pen_Kotei;
        line_hendou.pen=pen_Hendou;
        line_kotei.plotPoints=QList<QPointF>()<<QPointF()<<dot_kotei.point;
        line_hendou.plotPoints=QList<QPointF>()<<dot_kotei.point<<dot.point;
        series.append(line_kotei);
        series.append(line_hendou);


        GRAPH_SIZE_OPTION copy(size);
        copy.aspectPolicy = GRAPH_SIZE_OPTION::FIX;
        copy.aspectRatio = 1.0;
        if(setting.getMinMaxAutoMz()){
            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_xmin = -qAbs(mmax);
            copy.fixed_xmax = qAbs(mmax);
//            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_TAISYOU;
        }else{
            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_xmin = setting.getMinMzValue();
            copy.fixed_xmax = setting.getMaxMzValue();
        }
        if(setting.getMinMaxAutoN()){
            copy.scaleAutoY = GRAPH_SIZE_OPTION::JIKU_TAISYOU;
        }else{
            copy.scaleAutoY = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_ymin = setting.getMinNValue();
            copy.fixed_ymax = setting.getMaxNValue();
        }

        Graph graph(series, copy,
                    u8"Mz-N ", u8"Mz(kNm)", u8"N(kN)");
        graph.setTitleFont(QFont("Meiryo",1.5*(5+setting.getTitleTextSize())));
        graph.setTitleVisible(setting.getTitleVisible());
        graph.setAxisTextFont(QFont("Meiryo",(5+setting.getAxisScaleTextSize())));
        graph.setAxisValueFont(QFont("Meiryo",(5+setting.getAxisLabelSize())));
        graph.setAxisColor(setting.getAxisColor());
        if(setting.getRulerVisible()){
            graph.setRulerPen(QPen(setting.getRulerColor(),0,setting.getRulerStyle()));
        }else{
            graph.setRulerPen(QPen(setting.getRulerColor(),0,Qt::NoPen));
        }
        return graph;

    }else if(type.nm_type==GRAPH_SUBTYPE_PROPERTY::MyMz){
        //Nすべてについて描画しているので、切りのいいところとか、現在のNとかだけ切り出す
        //切りのいいところ→グラフの軸と同じ求め方でOK。内分点を求める。nmplot2？
        //現在のN→与える場合と不要な場合。makeDot、drawStressとかでそれぞれ実装しているので、どこかで実装？
        //
        int nch = chck.namech.indexOf(namech);
        int lsu = chck.ich.at(nch).at(0);
        int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(md->getUuid());
        qreal minN = chck.tnsec.at(nch).at(numm).at(icj).first();
        qreal maxN = chck.tnsec.at(nch).at(numm).at(icj).last();
        for(int i=-100;i<100;i++){
            qreal n=i*2000;
            if(minN >= n || maxN <= n)continue;

            QVector<QVector<double>> mCurve_x;
            QVector<QVector<double>> mCurve_y;
            QVector<double> nCurve;
            for(int i= 0;i<chck.nrstm;i++){
                QVector<double> mTmpx;
                QVector<double> mTmpy;
                for(int j= 0;j<chck.nrstn;j++){
                    mTmpx<<chck.tmsec.at(nch).at(numm).at(icj).at(i).at(j).at(0);
                    mTmpy<<chck.tmsec.at(nch).at(numm).at(icj).at(i).at(j).at(1);
                    nCurve<<chck.tnsec.at(nch).at(numm).at(icj).at(j);
                }
                mCurve_x<<mTmpx;
                mCurve_y<<mTmpy;
            }
            Series ss = sub_makeMyMzCurve(pens.at(lsu-1),n,chck.nrstn,chck.nrstm,nCurve,mCurve_x,mCurve_y);

            series.append(ss);
        }

        QList<Dot> dots;
        QList<STRESS> stress;
        QList<STRESS> stress_kotei;

        for(int nrsch=0;nrsch<chck.nrsch.at(nch);nrsch++){
            STRESS st;
            st.coef = chck.corsch.at(nch).at(nrsch);
            st.rst = chck.nmrsch.at(nch).at(nrsch);
            stress<<st;
            if(0==chck.irsch.at(nch).at(nrsch)){//0...kotei,1...hendou
                stress_kotei<<st;
            }
        }
        //原点から固定分を足した点と、そこから変動分を足した点を線で結ぶ
        Dot dot_kotei=Dot::makeDot(type.nm_type,type.icj,
                                   namech,
                                   type.memberId,
                                   stress_kotei);

        Dot dot=Dot::makeDot(type.nm_type,type.icj,
                             namech,
                             type.memberId,
                             stress);
        dot.symbolColor=setting.getSymbolColor();
        dot.symbolSize=setting.getSymbolSize();
        dot.font=QFont("Meiryo",5+setting.getSymbolLabelSize());
        dots<<dot;

        Series line_kotei;
        Series line_hendou;
        line_kotei.pen=pen_Kotei;
        line_hendou.pen=pen_Hendou;
        line_kotei.plotPoints=QList<QPointF>()<<QPointF()<<dot_kotei.point;
        line_hendou.plotPoints=QList<QPointF>()<<dot_kotei.point<<dot.point;
        series.append(line_kotei);
        series.append(line_hendou);

        UnifiedOutput* ud = UnifiedFixedData::getInstance()->getPstnData();
        MemberForce mf = ud->getCombinationFaceMemberForce(type.memberId,stress);
        MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(type.memberId,stress,1,0);
        qreal N_forPlotPoint = mf.N + mfil_i.getN();
        QPen pen_plot=QPen(Qt::black,0,Qt::DashDotLine);

        QVector<QVector<double>> mCurve_x;
        QVector<QVector<double>> mCurve_y;
        QVector<double> nCurve;
        for(int i= 0;i<chck.nrstm;i++){
            QVector<double> mTmpx;
            QVector<double> mTmpy;
            for(int j= 0;j<chck.nrstn;j++){
                mTmpx<<chck.tmsec.at(nch).at(numm).at(icj).at(i).at(j).at(0);
                mTmpy<<chck.tmsec.at(nch).at(numm).at(icj).at(i).at(j).at(1);
                nCurve<<chck.tnsec.at(nch).at(numm).at(icj).at(j);
            }
            mCurve_x<<mTmpx;
            mCurve_y<<mTmpy;
        }
        Series ss0 = sub_makeMyMzCurve(pen_plot,N_forPlotPoint,chck.nrstn,chck.nrstm,nCurve,mCurve_x,mCurve_y);
        ss0.dots = dots;
        series.append(ss0);

        qreal mmax=-1.0;
        for(int i=0;i<chck.nrstn;i++){
            for(int j=0;j<chck.nrstm;j++){
                mmax = qMax(mmax,qAbs(mCurve_x.at(j).at(i)));
                mmax = qMax(mmax,qAbs(mCurve_y.at(j).at(i)));
            }
        }
        int keta = (int)log10(mmax)+1;
        int delta = qPow(10,keta-1);
        qreal mm=0;
        for(int i=0;i<1000;i++){
            mm+=delta;
            if(mm>mmax)break;
        }

        GRAPH_SIZE_OPTION copy(size);
        copy.aspectPolicy = GRAPH_SIZE_OPTION::FIX;
        copy.aspectRatio = 1.0;

        if(setting.getMinMaxAutoMy()){
            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_xmin = -qAbs(mm);
            copy.fixed_xmax = qAbs(mm);
//            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_TAISYOU;
        }else{
            copy.scaleAutoX = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_xmin = setting.getMinMyValue();
            copy.fixed_xmax = setting.getMaxMyValue();
        }
        if(setting.getMinMaxAutoMz()){
            copy.scaleAutoY = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_ymin = -qAbs(mm);
            copy.fixed_ymax = qAbs(mm);
//            copy.scaleAutoY = GRAPH_SIZE_OPTION::JIKU_TAISYOU;
        }else{
            copy.scaleAutoY = GRAPH_SIZE_OPTION::JIKU_FIX;
            copy.fixed_ymin = setting.getMinMzValue();
            copy.fixed_ymax = setting.getMaxMzValue();
        }

        Graph graph(series, copy,
                    u8"My-Mz ", u8"My(kNm)", u8"Mz(kNm)");
        graph.setTitleFont(QFont("Meiryo",1.5*(5+setting.getTitleTextSize())));
        graph.setTitleVisible(setting.getTitleVisible());
        graph.setAxisTextFont(QFont("Meiryo",(5+setting.getAxisScaleTextSize())));
        graph.setAxisValueFont(QFont("Meiryo",(5+setting.getAxisLabelSize())));
        graph.setAxisColor(setting.getAxisColor());
        if(setting.getRulerVisible()){
            graph.setRulerPen(QPen(setting.getRulerColor(),0,setting.getRulerStyle()));
        }else{
            graph.setRulerPen(QPen(setting.getRulerColor(),0,Qt::NoPen));
        }
        return graph;

    }else{
        return makeNullGraph(size);
    }
}

Series GraphFactory::sub_makeNMyCurve(const QPen& pen,int nrstn,int nrstm,
                                      const QVector<QVector<double>>& mCurve,
                                      const QVector<double>& nCurve){
    Series s;
    s.pen=pen;
    int i=0;
    for(int j= 0;j<nrstn;j++){
        qreal x=mCurve.at(i).at(j);
        qreal y=nCurve.at(j);
        s.plotPoints.append(QPointF(x, y));
    }
    i= 12;
    for(int j= nrstn-1;j>=0;j--){
        qreal x=mCurve.at(i).at(j);
        qreal y=nCurve.at(j);
        s.plotPoints.append(QPointF(x, y));
    }
    return s;
}
Series GraphFactory::sub_makeNMzCurve(const QPen &pen,int nrstn,int nrstm,
                                      const QVector<QVector<double>>& mCurve,
                                      const QVector<double>& nCurve){
    Series s;
    s.pen=pen;
    int i=6;
    for(int j= 0;j<nrstn;j++){
        qreal x=mCurve.at(i).at(j);
        qreal y=nCurve.at(j);
        s.plotPoints.append(QPointF(x, y));
    }
    i=18;
    for(int j= nrstn-1;j>=0;j--){
        qreal x=mCurve.at(i).at(j);
        qreal y=nCurve.at(j);
        s.plotPoints.append(QPointF(x, y));
    }
    return s;
}
Series GraphFactory::sub_makeMyMzCurve(const QPen &pen,  qreal N,int nrstn, int nrstm,
                                       const QVector<double>& nCurve,
                                       const QVector<QVector<double>>& mCurvex,
                                       const QVector<QVector<double>>& mCurvey){
    //軸力Nに対応するカーブを描く
    Series s;
    s.pen=pen;
    qreal n0=nCurve.at(0);
    qreal nmax=nCurve.at(nrstn-1);
    if(N < n0 || N > nmax)return Series();
    for(int i= 1;i<nrstn-1;i++){
        qreal n1=nCurve.at(i);
        if(n0 < N && N < n1){
            qreal t = (n1-N)/(n1-n0);
            for(int j= 0;j<nrstm;j++){
                qreal x0=mCurvex.at(j).at(i-1);
                qreal y0=mCurvey.at(j).at(i-1);
                qreal x=mCurvex.at(j).at(i);
                qreal y=mCurvey.at(j).at(i);
                s.plotPoints.append(QPointF((1.0-t)*x0+t*x, (1.0-t)*y0+t*y));
            }
            qreal x0=mCurvex.at(0).at(i-1);
            qreal y0=mCurvey.at(0).at(i-1);
            qreal x=mCurvex.at(0).at(i);
            qreal y=mCurvey.at(0).at(i);
            s.plotPoints.append(QPointF((1.0-t)*x0+t*x, (1.0-t)*y0+t*y));
            qDebug()<<"makeMM end1";
            return s;
        }
        n0=n1;
    }
    qDebug()<<"makeMM end2";
    return s;
}



void GraphFactory::makeNmint3dGraph_CHCK(const AnalysisGraphNmint2DViewSetting& setting,Graph3dScene* s,GRAPH_SUBTYPE_PROPERTY type){

    if(READING_STATUS::STATUS_SUCCESS != UnifiedFixedData::getInstance()->getPstnData()->getReadStatus(OFILE_CHCK)){return ;}
    //部材ベースのNMINT。軸力の影響とかを加味
    s->clear();
    if(type.memberId == QUuid())return;
    MemberData* md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(type.memberId);
    int icj = static_cast<int>(type.icj);
    auto chck = UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();
    int nch = chck.namech.indexOf(type.caseName);
    int numm = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(md->getUuid());

    QVector<QVector<double>> mCurve_x;
    QVector<QVector<double>> mCurve_y;
    QVector<double> nCurve;
    for(int i= 0;i<chck.nrstm;i++){
        QVector<double> mTmp_x;
        QVector<double> mTmp_y;
        for(int j= 0;j<chck.nrstn;j++){
            mTmp_x<<chck.tmsec.at(nch).at(numm).at(icj).at(i).at(j).at(0);
            mTmp_y<<chck.tmsec.at(nch).at(numm).at(icj).at(i).at(j).at(1);
            nCurve<<chck.tnsec.at(nch).at(numm).at(icj).at(j);
        }
        mCurve_x<<mTmp_x;
        mCurve_y<<mTmp_y;
    }

    sub_make3dNMSurface(setting,s,chck.nrstn,chck.nrstm,mCurve_x,mCurve_y,nCurve,Qt::black,Qt::darkYellow);

    QList<STRESS> stress;
    QList<STRESS> stress_kotei;
    for(int nrsch=0;nrsch<chck.nrsch.at(nch);nrsch++){
        STRESS st;
        st.coef = chck.corsch.at(nch).at(nrsch);
        st.rst = chck.nmrsch.at(nch).at(nrsch);
        stress<<st;
        if(0==chck.irsch.at(nch).at(nrsch)){//0...kotei,1...hendou
            stress_kotei<<st;
        }

    }
    sub_make3dNMPlotPoint( setting,s,type.icj, type.memberId, type.caseName, stress_kotei,stress);

    qreal mmax=1.0;
    for(int i=0;i<chck.nrstn;i++){
        for(int j=0;j<chck.nrstm;j++){
            mmax = qMax(mmax,qAbs(mCurve_x.at(j).at(i)));
            mmax = qMax(mmax,qAbs(mCurve_y.at(j).at(i)));
        }
    }
    qreal nmax = qMax(1.0,qMax(qAbs(nCurve.first()),qAbs(nCurve.last())));
    s->setXScale(1000.0/mmax);
    s->setYScale(1000.0/mmax);
    s->setZScale(1000.0/nmax);

    QVector<qreal> mAxis;
    QVector<qreal> nAxis;
    for(int i=-20;i<=20;i++){
        if( -2.0*mmax < i*100 && i*100 < 2.0*mmax){
            mAxis<<i*100.0;
        }
        if( -2.0*nmax < i*1000 && i*1000 < 2.0*nmax){
            nAxis<<i*1000.0;
        }
    }
    s->setXAxis(mAxis,u8"My[kNm]");
    s->setYAxis(mAxis,u8"Mz[kNm]");
    s->setZAxis(nAxis,u8"N[kN]");
}
void GraphFactory::sub_make3dNMSurface(const AnalysisGraphNmint2DViewSetting& setting,Graph3dScene* s,int nrstn,int nrstm,
                                       const QVector<QVector<double>>& mCurve_x,
                                       const QVector<QVector<double>>& mCurve_y,
                                       const QVector<double>& nCurve,
                                       const QColor& lineColor,const QColor& planeColor){
    int cnt = 0;
    cnt++;qDebug()<<"nmint 3d surface"<<cnt;
    LineParameter lineparam;
    PlaneParameter planeparam;
    PointParameter pointparam;
    planeparam.lineColor=Qt::transparent;
    planeparam.paintColor=planeColor;
    lineparam.lineColor=lineColor;
    lineparam.paintColor=lineColor;

    cnt++;qDebug()<<"nmint 3d surface"<<cnt;
    for(int i= 0;i<nrstn-1;i++){
        for(int j= 0;j<nrstm;j++){
            QList<Graph3DPoint> planepoints;
            QList<Graph3DPoint> linepoints;
            int jj=j+1;
            if(j+1==nrstm)jj=0;
            //ls,icj,m,n,xy
            XYZ coord1=XYZ(mCurve_x.at(j).at(i),
                           mCurve_y.at(j).at(i),
                           nCurve.at(i));
            Graph3DPoint point1(coord1,pointparam);
            planepoints.append(point1);

            XYZ coord2=XYZ(mCurve_x.at(jj).at(i),
                           mCurve_y.at(jj).at(i),
                           nCurve.at(i));
            Graph3DPoint point2(coord2,pointparam);
            planepoints.append(point2);

            XYZ coord3=XYZ(mCurve_x.at(jj).at(i+1),
                           mCurve_y.at(jj).at(i+1),
                           nCurve.at(i+1));
            Graph3DPoint point3(coord3,pointparam);
            planepoints.append(point3);

            XYZ coord4=XYZ(mCurve_x.at(j).at(i+1),
                           mCurve_y.at(j).at(i+1),
                           nCurve.at(i+1));
            Graph3DPoint point4(coord4,pointparam);
            planepoints.append(point4);
            Graph3DPlane plane(planepoints,planeparam);
            s->addPlane(plane);

            linepoints.append(point1);
            linepoints.append(point2);
            Graph3DLine line(linepoints,lineparam);
            s->addLine(line);

        }
    }
    cnt++;qDebug()<<"nmint 3d surface"<<cnt;

}
void GraphFactory::sub_make3dNMPlotPoint(const AnalysisGraphNmint2DViewSetting &setting, Graph3dScene* s,
                                         GRAPH_SUBTYPE_PROPERTY::ICJ icj,
                                         QUuid memberId,
                                         QString stressCombiName, QList<STRESS> stress_kotei, QList<STRESS> stress)
{
    UnifiedOutput* ud = UnifiedFixedData::getInstance()->getPstnData();

    //応力(N,My,Mz)を求める
    qreal Nf=0;
    qreal Myf=0;
    qreal Mzf=0;

    MemberForce mff = ud->getCombinationFaceMemberForce(memberId,stress_kotei);
    MemberForceByIL mfil_if = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(memberId,stress_kotei,1,0);
    MemberForceByIL mfil_cf = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(memberId,stress_kotei,2,0);
    MemberForceByIL mfil_jf = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(memberId,stress_kotei,3,0);
    qDebug()<<"makeDot 8";
    if(icj == GRAPH_SUBTYPE_PROPERTY::I){
        Myf =  -mff.getMy(0)-mfil_if.getMy();
        Mzf =  -mff.getMz(0)-mfil_if.getMz();
        Nf =  mff.N + mfil_if.getN();
    }else if(icj == GRAPH_SUBTYPE_PROPERTY::C){
        Myf =  -mff.getMy(0.5)-mfil_cf.getMy();
        Mzf =  -mff.getMz(0.5)-mfil_cf.getMz();
        Nf =  mff.N + mfil_if.getN();
    }else if(icj == GRAPH_SUBTYPE_PROPERTY::J){
        Myf =  mff.getMy(1.0)+mfil_jf.getMy();
        Mzf =  mff.getMz(1.0)+mfil_jf.getMz();
        Nf =  mff.N + mfil_if.getN();
    }
    //応力(N,My,Mz)を求める
    qreal N=0;
    qreal My=0;
    qreal Mz=0;

    MemberForce mf = ud->getCombinationFaceMemberForce(memberId,stress);
    MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(memberId,stress,1,0);
    MemberForceByIL mfil_c = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(memberId,stress,2,0);
    MemberForceByIL mfil_j = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(memberId,stress,3,0);
    qDebug()<<"makeDot 8";
    if(icj == GRAPH_SUBTYPE_PROPERTY::I){
        My =  -mf.getMy(0)-mfil_i.getMy();
        Mz =  -mf.getMz(0)-mfil_i.getMz();
        N =  mf.N + mfil_i.getN();
    }else if(icj == GRAPH_SUBTYPE_PROPERTY::C){
        My =  -mf.getMy(0.5)-mfil_c.getMy();
        Mz =  -mf.getMz(0.5)-mfil_c.getMz();
        N =  mf.N + mfil_i.getN();
    }else if(icj == GRAPH_SUBTYPE_PROPERTY::J){
        My =  mf.getMy(1.0)+mfil_j.getMy();
        Mz =  mf.getMz(1.0)+mfil_j.getMz();
        N =  mf.N + mfil_i.getN();
    }


    PointParameter param_kotei;
    param_kotei.type = PointParameter::NONE;
    Graph3DPoint pt_kotei(XYZ(Myf,Mzf,Nf),param_kotei);

    PointParameter param;
    param.name = stressCombiName;
    param.type = PointParameter::PLOTWITHLINE;
    Graph3DPoint pt(XYZ(My,Mz,N),param);
    qDebug()<<"My,Mz,N="<<My<<Mz<<N;
    s->addPoint(pt);

    LineParameter param_line;
    param_line.lineColor=Qt::red;
    param_line.lineWidth=2;

    Graph3DLine ln1(QList<Graph3DPoint>()<<Graph3DPoint(XYZ(0,0,0),param_kotei)<<pt_kotei,param_line);
    param_line.lineStyle=2;
    Graph3DLine ln2(QList<Graph3DPoint>()<<pt_kotei<<pt,param_line);

    s->addLine(ln1);
    s->addLine(ln2);

}
void GraphFactory::drawLegend(QPainter* p,QRectF target,OUTFIGTYPE figureType,Analysis2DViewSettings myViewSetting){
    //QDelta
    //QTheta
    //QDelta(hoka
    //QTheta(hoka
    qDebug()<<"drawLegend in";
    if(figureType==OUTFIGTYPE::QDelta||figureType==OUTFIGTYPE::QTheta
            ||figureType==OUTFIGTYPE::QDeltaFloorFrames||figureType==OUTFIGTYPE::QDeltaFrameFloors||
           figureType==OUTFIGTYPE::QThetaFloorFrames||figureType==OUTFIGTYPE::QThetaFrameFloors ){
        qDebug()<<"drawLegend QDelta" << target;
        AnalysisGraphQDelta2DViewSetting setting=*static_cast<AnalysisGraphQDelta2DViewSetting*>(myViewSetting.getSetting(figureType));
        int nline=4;
//        QPen origPen = p->pen();
        int cnt = 0;
            cnt++;
            drawLegendMark(p,target,cnt,nline,DotType::Circle,setting.getSymbolSize(),setting.getPlasticSymbolColor());
            drawLegendLine(p,target,cnt,nline,QPen(Qt::black,0,Qt::SolidLine),u8"フレーム曲げ降伏");
            cnt++;
            drawLegendMark(p,target,cnt,nline,DotType::BlackCircle,setting.getSymbolSize(),setting.getPlasticSymbolColor());
            drawLegendLine(p,target,cnt,nline,QPen(Qt::black,0,Qt::SolidLine),u8"フレームせん断降伏");
            cnt++;
            drawLegendMark(p,target,cnt,nline,DotType::Rectangle,setting.getSymbolSize(),setting.getPlasticSymbolColor());
            drawLegendLine(p,target,cnt,nline,QPen(Qt::black,0,Qt::SolidLine),u8"壁せん断ひび割れ");
            cnt++;
            drawLegendMark(p,target,cnt,nline,DotType::BlackRectangle,setting.getSymbolSize(),setting.getPlasticSymbolColor());
            drawLegendLine(p,target,cnt,nline,QPen(Qt::black,0,Qt::SolidLine),u8"壁せん断降伏");

    }
    //Qu,Qun
    if(figureType==OUTFIGTYPE::QuNeedf){
        qDebug()<<"drawLegend HorizontalCapacity"<< target;
        AnalysisGraphHorizontalCapacity2DViewSetting setting=*static_cast<AnalysisGraphHorizontalCapacity2DViewSetting*>(myViewSetting.getSetting(figureType));
        //保有水平耐力
        //必要保有水平耐力
        int nline=0;
        if(setting.getVisibleU())nline++;
        if(setting.getVisibleUn())nline++;
        if(nline==0)return;
//        QPen origPen = p->pen();
        int cnt = 0;
        if(setting.getVisibleU()){
            cnt++;
            drawLegendLine(p,target,cnt,nline,QPen(setting.getColorU(),0,setting.getStyleU()),u8"保有水平耐力");
        }
        if(setting.getVisibleUn()){
            cnt++;
            drawLegendLine(p,target,cnt,nline,QPen(setting.getColorUn(),0,setting.getStyleUn()),u8"必要保有水平耐力");
        }
    }

    //NM
    if(figureType==OUTFIGTYPE::NMINT){
        //長期→　線（表示時）、プロット、変動分（表示時）、固定分（表示時）//N=0
//        int nch = chck.namech.indexOf(namech);
//        int lsu = chck.ich.at(nch).at(0);

        qDebug()<<"drawLegend NM";
        AnalysisGraphNmint2DViewSetting setting=*static_cast<AnalysisGraphNmint2DViewSetting*>(myViewSetting.getSetting(figureType));
        int nline=0;
        if(setting.getOutputLVisible())nline++;
        if(setting.getOutputSVisible())nline++;
        if(setting.getOutputUVisible())nline++;
        if(setting.getKoteiVisible())nline++;
        if(setting.getHendouVisible())nline++;
        if(nline==0)return;
//        QPen origPen = p->pen();
        int cnt = 0;
        if(setting.getOutputLVisible()){
            cnt++;
            drawLegendLine(p,target,cnt,nline,QPen(setting.getOutputLColor(),0,setting.getOutputLStyle()),u8"長期許容応力");
        }
        if(setting.getOutputSVisible()){
            cnt++;
            drawLegendLine(p,target,cnt,nline,QPen(setting.getOutputSColor(),0,setting.getOutputSStyle()),u8"短期許容応力");
        }
        if(setting.getOutputUVisible()){
            cnt++;
            drawLegendLine(p,target,cnt,nline,QPen(setting.getOutputUColor(),0,setting.getOutputUStyle()),u8"終局耐力");
        }
        if(setting.getKoteiVisible()){
            cnt++;
            drawLegendLine(p,target,cnt,nline,QPen(setting.getKoteiColor(),0,setting.getKoteiStyle()),u8"固定成分");
        }
        if(setting.getHendouVisible()){
            cnt++;
            drawLegendLine(p,target,cnt,nline,QPen(setting.getHendoColor(),0,setting.getHendouStyle()),u8"変動成分");
        }
    }
    qDebug()<<"drawLegend done";

}
void GraphFactory::drawLegendLine(QPainter* p,QRectF target,int line,int linesum,QPen pen,QString str)
{
    QPen origPen = p->pen();
    p->setPen(pen);
    p->drawLine(QPointF(target.x()+target.width()*0.1,target.y()+line*target.height()/(linesum+1)),QPointF(target.x()+target.width()*0.4,target.y()+line*target.height()/(linesum+1)));
    p->setPen(origPen);
    p->setFont(QFont("Meiryo",8));
    qreal hh = QFontMetrics(QFont("Meiryo",8)).height();
    p->drawText(QRectF(target.x()+target.width()*0.5,
                       target.y()+line*target.height()/(linesum+1)-0.5*hh,
                       target.width()*0.4,hh),str);

}
void GraphFactory::drawLegendMark(QPainter* p,QRectF target,int line,int linesum,DotType markType,int symbolSize,QColor symbolColor)
{
    QPen savedPen = p->pen();
    QBrush savedBrush = p->brush();

//    qreal dpi = p->device()->logicalDpiX();
    QPointF orig = QPointF(target.x()+target.width()*0.25,target.y()+line*target.height()/(linesum+1));
    double size = 4.0+symbolSize;

    QPointF offset(0.6*size,0);
    if (markType == DotType::None){

    }else if (markType == DotType::Triangle || markType == DotType::BlackTriangle){
        double h = qPow(3, 0.5) / 3.0 * size;
        QPointF p1=orig+QPointF(0, -h);
        QPointF p2=orig+QPointF(-size/2.0, h * 0.5);
        QPointF p3=orig+QPointF(size/2.0, h * 0.5);
        QVector<QPointF> v{p1, p2, p3};
        QPolygonF triangle(v);
        if (markType == DotType::Triangle){
            p->setBrush(Qt::NoBrush);
            p->setPen(QPen(symbolColor,0));
            p->drawPolygon(triangle);
        }else{
            p->setBrush(symbolColor);
            p->setPen(QPen(symbolColor,0));
            p->drawPolygon(triangle);
        }
    }else if (markType == DotType::Rectangle){
        p->setBrush(Qt::NoBrush);
        p->setPen(QPen(symbolColor,0));
        p->drawRect(QRectF(orig.x()-size/2.0, orig.y()-size/2.0, size, size));
    }else if (markType == DotType::BlackRectangle){
        p->setBrush(symbolColor);
        p->setPen(QPen(symbolColor,0));
        p->drawRect(QRectF(orig.x()-size/2.0, orig.y()-size/2.0, size, size));
    }else if (markType == DotType::Circle){
        p->setBrush(Qt::NoBrush);
        p->setPen(QPen(symbolColor,0));
        p->drawEllipse(orig,size/2.0,size/2.0);
    }else if (markType == DotType::BlackCircle){
        p->setBrush(symbolColor);
        p->setPen(QPen(symbolColor,0));
        p->drawEllipse(orig,size/2.0,size/2.0);
    }
    p->setBrush(savedBrush);
    p->setPen(savedPen);
//    p->drawText(orig+offset,label);
}

}
