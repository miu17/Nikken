#include "define_draw_figure.h"

#include <QGraphicsScene>
#include <QSize>
#include "base_background.h"
#include "calcload_grscene.h"
#include "draw_figure.h"
#include "elevationOT_grscene.h"
#include "elevationUI_grscene.h"
#include "fixed_data.h"
#include "graph_scene.h"
#include "line_outputfigure.h"
#include "planOT_grscene.h"
#include "planUI_grscene.h"

namespace post3dapp{

qreal DefineDrawFigure::calcAutoScale(CustomDrawGrScene* scene, const FIGURE_SIZE_POLICY& policy){
    return qMin(calcYokoAutoScale(scene,policy.initialPaperSize(), policy.initial_divide.yoko, policy),
                calcTateAutoScale(scene,policy.initialPaperSize(),policy.initial_divide.tate, policy));
}

qreal DefineDrawFigure::calcAutoScale(CustomDrawGrScene* scene, const QSize& peperSize, const FIGURE_SIZE_POLICY& policy){
    return qMin(calcYokoAutoScale(scene, peperSize, policy.initial_divide.yoko, policy),
                calcTateAutoScale(scene,policy.initialPaperSize(),policy.initial_divide.tate, policy));
}

qreal DefineDrawFigure::calcTateAutoScale(CustomDrawGrScene* scene,const QSize unitPaperSize,const int nTate,const FIGURE_SIZE_POLICY& policy){
    double paper_topMargin = policy.paperMargin_top;
    double paper_bottomMargin = policy.paperMargin_bottom;

    if(policy.drawHeader)paper_topMargin += policy.headerSize;
    if(policy.drawFooter)paper_bottomMargin += policy.footerSize;

    QRectF bounding;
    if(!policy.includeBackGround){
        bounding = scene->itemsBoundingRect();
    }else {
        bounding = scene->getBackGround()->boundingRect();
    }

    qreal papersize = nTate*(unitPaperSize.height()-paper_topMargin-paper_bottomMargin);
    if((nTate > 1&& policy.drawMarker==DRAWMARKER::IfDivided)
            || (policy.drawMarker==DRAWMARKER::Always)){
        papersize -= nTate*(2.0 * policy.markerSize);
    }
    qreal modelsize = bounding.height()+(nTate-1.0)* policy.modelMargin_overlap + policy.modelMargin_top + policy.modelMargin_bottom;
    return papersize/modelsize;
}

qreal DefineDrawFigure::calcYokoAutoScale(CustomDrawGrScene* scene, const QSize unitPaperSize,const  int nYoko, const FIGURE_SIZE_POLICY& policy){
    QRectF bounding;
    if(!policy.includeBackGround){
        bounding = scene->itemsBoundingRect();
    }else{
        bounding = scene->getBackGround()->boundingRect();
    }

    qreal papersize = nYoko*(unitPaperSize.width()-policy.paperMargin_left-policy.paperMargin_right);
    if((nYoko > 1&& policy.drawMarker==DRAWMARKER::IfDivided)
            || (policy.drawMarker==DRAWMARKER::Always)){
        papersize -= nYoko*(2.0*policy.markerSize);
    }
    qreal modelsize = bounding.width()+(nYoko-1.0)* policy.modelMargin_overlap + policy.modelMargin_left + policy.modelMargin_right;
    qreal scale = papersize/modelsize;
    return scale;
}

qreal DefineDrawFigure::calcUpperAutoScale(CustomDrawGrScene *scene, const FIGURE_SIZE_POLICY &policy)
{
    return qMin(calcYokoAutoScale(scene,policy.initialPaperSize(), policy.upperBound_divide.yoko, policy),
                calcTateAutoScale(scene,policy.initialPaperSize(),policy.upperBound_divide.tate, policy));
}


QRectF FIGURE_SIZE_RESULT::getInnerPixcelRect(const FIGURE_PROPERTY& property, const int dpi) const{

    QPointF origin = outerOrigin;
    if (property.drawHeader)
        origin += QPoint(0, property.headerSize);


    if((property.divide.tate == 1 && property.divide.yoko == 1 && property.drawMarker == DRAWMARKER::IfDivided)
            || property.drawMarker == DRAWMARKER::NotDisplay){
        //マーカーなし
    }else if((property.divide.tate > 1 && property.divide.yoko == 1 && property.drawMarker == DRAWMARKER::IfDivided)){
        //マーカー上下
        origin += QPointF(0, property.markerSize);
    }else if((property.divide.tate == 1 &&property.divide.yoko > 1 && property.drawMarker==DRAWMARKER::IfDivided)){
        //マーカー左右
        origin += QPointF(property.markerSize, 0);
    }else{
        //マーカー上下左右
        origin += QPointF(property.markerSize, property.markerSize);
    }

    return QRectF((qreal)dpi * origin.x() / DrawFigure::inchToMm,
                  (qreal)dpi * origin.y() / DrawFigure::inchToMm,
                  (qreal)dpi * property.TargetSize().width() / DrawFigure::inchToMm,
                  (qreal)dpi * property.TargetSize().height() / DrawFigure::inchToMm);
}

QRectF FIGURE_SIZE_RESULT::getOuterPixcelRect(const FIGURE_PROPERTY& property, const int dpi) const{
    return QRectF((qreal)dpi * outerOrigin.x() / DrawFigure::inchToMm,
                  (qreal)dpi * outerOrigin.y() / DrawFigure::inchToMm,
                  (qreal)dpi * property.outerTargetSize.width() / DrawFigure::inchToMm,
                  (qreal)dpi * property.outerTargetSize.height() / DrawFigure::inchToMm);
}

CustomDrawGrScene *FIGURE_TYPE_PROPERTY::makeScene(const Input2DViewSettings& input2D,
                                                   const Input3DViewSettings& input3D,
                                                   const CalculationLoad2DViewSettings& load2D,
                                                   const CalculationLoad3DViewSettings& load3D,
                                                   const Analysis2DViewSettings& analysis2D,
                                                   const Analysis3DViewSettings& analysis3D,
                                                   QObject *parent) const
{
    if (viewtype == MODELVIEWTYPE::INPUT2DMODEL){
        switch(type){
        case OUTFIGTYPE::PLANINPUT:
        case OUTFIGTYPE::SECTIONINPUT:
            return DrawFigure::createInputScene(*this, input2D, parent);
        default:
            throw;
        }
    }else if(viewtype == MODELVIEWTYPE::INPUT2DMODELCOPY) {
        switch(type){
        case OUTFIGTYPE::PLAN:
            return DrawFigure::createCopyScene(UnifiedFixedData::getInstance()->getInputData(), *this, Input2DViewSettings::makeSymbolSetting(input2D), parent);
        case OUTFIGTYPE::SECTION:
            return DrawFigure::createCopyScene(UnifiedFixedData::getInstance()->getInputData(), *this, Input2DViewSettings::makeSymbolSetting(input2D), parent);
        case OUTFIGTYPE::LOADPLAN:
            return DrawFigure::createCopyScene(UnifiedFixedData::getInstance()->getInputData(), *this, Input2DViewSettings::makeLoadSetting(input2D), parent);
        case OUTFIGTYPE::LOADSECTION:
            return DrawFigure::createCopyScene(UnifiedFixedData::getInstance()->getInputData(), *this, Input2DViewSettings::makeLoadSetting(input2D), parent);
        default:
            throw;
        }
    }else if(viewtype == MODELVIEWTYPE::OUTPUT2DMODEL) {
        return DrawFigure::createOutScene(*this, analysis2D, parent);
    }else if (viewtype == MODELVIEWTYPE::INPUT2DMODELCOPY){
        switch (type) {
        case OUTFIGTYPE::PLAN:
        case OUTFIGTYPE::SECTION:
            return DrawFigure::createCopyScene(
                        UnifiedFixedData::getInstance()->getInputData(),
                        *this,
                        Input2DViewSettings::makeSymbolSetting(input2D),
                        parent);
        case OUTFIGTYPE::LOADPLAN:
        case OUTFIGTYPE::LOADSECTION:
            return DrawFigure::createCopyScene(
                        UnifiedFixedData::getInstance()->getInputData(),
                        *this,
                        Input2DViewSettings::makeLoadSetting(input2D),
                        parent);
        default:
            throw;
        }
    }else if (viewtype == MODELVIEWTYPE::CALC2DMODEL){
        return DrawFigure::createLoadScene(*this, load2D, parent);
    }else{
        throw;
    }

}

LISTTYPE FIGURE_TYPE_PROPERTY::listType(OUTFIGTYPE otype)
{
    //節点外力図・外力M図はタイプあってる？
    switch(otype){
    case OUTFIGTYPE::UNDEFINED:
    case OUTFIGTYPE::MODEL:
    case OUTFIGTYPE::LENGTH:
    case OUTFIGTYPE::FACELENGTH:
    case OUTFIGTYPE::COORDINATE:
    case OUTFIGTYPE::RIGIDLENGTH:
    case OUTFIGTYPE::LATERALBUCKLEN:
    case OUTFIGTYPE::EULERBUCKLEN:
    case OUTFIGTYPE::JOINTMASS:
    case OUTFIGTYPE::RIGIDITYPHI:
        return LISTTYPE::MODEL;
    case OUTFIGTYPE::LAYERDEFORMMAP:
    case OUTFIGTYPE::LAYERSTIFFMAP:
    case OUTFIGTYPE::CMQ:
    case OUTFIGTYPE::NSTRESS:
    case OUTFIGTYPE::FSTRESS:
    case OUTFIGTYPE::RSTRESS:
    case OUTFIGTYPE::AXIAL:
    case OUTFIGTYPE::TORSION:
    case OUTFIGTYPE::DEFORM:
    case OUTFIGTYPE::DISP:
    case OUTFIGTYPE::AXIALMAP:
    case OUTFIGTYPE::REACTMAP:
    case OUTFIGTYPE::SHEARMAP:
    case OUTFIGTYPE::VERTICALLOADMAP:
    case OUTFIGTYPE::OUTERFORCE:
    case OUTFIGTYPE::OUTERMOMENT:
        return LISTTYPE::STRESS;
    case OUTFIGTYPE::ALLOW:
    case OUTFIGTYPE::CHECKRATIO:
        return LISTTYPE::CHCK;
    case OUTFIGTYPE::ALLOWRATIOE:
    case OUTFIGTYPE::ALLOWRATIOU:
    case OUTFIGTYPE::RANK:
    case OUTFIGTYPE::PLASTICRATIO:
    case OUTFIGTYPE::YIELDSTEP:
    case OUTFIGTYPE::YIELDLOADMAG:
    case OUTFIGTYPE::STRENGTH:
        return LISTTYPE::QUDS;
    case OUTFIGTYPE::QDelta:
    case OUTFIGTYPE::QDeltaFrameFloors:
    case OUTFIGTYPE::QDeltaFloorFrames:
    case OUTFIGTYPE::QTheta:
    case OUTFIGTYPE::QThetaFrameFloors:
    case OUTFIGTYPE::QThetaFloorFrames:
    case OUTFIGTYPE::QuNeedf:
    case OUTFIGTYPE::NMINT:
        return LISTTYPE::GRAPH;
    case OUTFIGTYPE::COMBIREACTMAP:
        return LISTTYPE::COMBIMAP;
    default:
        return LISTTYPE::MODEL;
    }
}

QString FIGURE_TYPE_PROPERTY::title() const
{
    QString ret;
    // 階・通り
    if (name != QString{}){
        if (scenetype == SCENETYPE::PLANSCENE)
            ret += name + u8"階 ";
        else if (scenetype == SCENETYPE::ELEVATIONSCENE)
            ret += name + u8"通り ";
    }

    ret += OutputFigureText::outfigtypeToString(type);

    // 荷重拾い
    if (type == OUTFIGTYPE::CALCLOAD2D){
        ret += u8" (" + LoadType::enumToString(loadType) + " ";
        switch(viewdirection){
        case OUTPUTDIR::ODIR_PLIN:
            ret += u8"面内 ";
            break;
        case OUTPUTDIR::ODIR_PLOUT:
            ret += u8"面外 ";
            break;
        case OUTPUTDIR::ODIR_AXIAL:
            ret += u8"軸方向 ";
            break;
        }
        switch(loadSumType){
        case LOADSUMTYPE::LOADSUM_ALL:
            ret += u8"全て";
            break;
        case LOADSUMTYPE::LOADSUM_AUTO:
            ret += u8"伝達荷重";
            break;
        case LOADSUMTYPE::LOADSUM_MANUAL:
            ret += u8"手動入力";
            break;
        case LOADSUMTYPE::LOADSUM_SELF:
            ret += u8"自重";
            break;
        }

        ret += ")";
    }


    // モデル
    if (model != QString{})
        ret += u8" (" + model + u8")"; // (MODEL2)

    // Case
    if (caseName != QString{}){
        ret += u8" (" + caseName + u8")"; // (VL+E1)
    }

    // Stress
    ret += stressText();

    return ret;
}

QString FIGURE_TYPE_PROPERTY::stressText() const
{
    if (stress.count() == 0)
        return QString{};

    QString ret;
    ret += u8" (";
    for (int s = 0; s < stress.count(); s++){
        if (s != 0)
            ret += u8"/"; // COMBINATION (1.0RSTDL/1.0RSTDL + 1.0RSTE1/ ... )
        for (int i = 0 ; i < stress.at(s).count(); i++){
            if (i != 0){
                if (stress.at(s).at(i).coef >= 0)
                    ret += u8" + ";
                else
                    ret += u8"  ";
            }
            ret += stress.at(s).at(i).text();
        }
    }
    ret += u8")";
    return ret;
}

QRectF GRAPH_SIZE_OPTION::graphArea(qreal dpi) const{//pixcel系
    QRectF graphArea_mm=targetRect-outFramePaddings;
    if(targetRect.width()-outFramePaddings.left()-outFramePaddings.right()<=0)return QRectF(0,0,0,0);
    if(targetRect.height()-outFramePaddings.top()-outFramePaddings.bottom()<=0)return QRectF(0,0,0,0);
    switch(aspectPolicy){
    case EXPAND:
        return QRectF(graphArea_mm.x()*dpi/DrawFigure::inchToMm,
                      graphArea_mm.y()*dpi/DrawFigure::inchToMm,
                      graphArea_mm.width()*dpi/DrawFigure::inchToMm,
                      graphArea_mm.height()*dpi/DrawFigure::inchToMm);
        break;
    case FIX:
        //targetRectからpaddingを引いたアスペクト比と、指定のアスペクト比の大小を比べる。
        //指定の方が縦長 → 縦はいっぱいに、横はhAlignに従う
        if(aspectRatio - graphArea_mm.height() / graphArea_mm.width()  >= 0){
            if(hAlign==H_ALIGN::LEFT){
                return QRectF(graphArea_mm.x()*dpi/DrawFigure::inchToMm,
                              graphArea_mm.y()*dpi/DrawFigure::inchToMm,
                              graphArea_mm.height()/aspectRatio*dpi/DrawFigure::inchToMm,
                              graphArea_mm.height()*dpi/DrawFigure::inchToMm);
            }else if(hAlign==H_ALIGN::CENTER){
                return QRectF((graphArea_mm.x() + 0.5*graphArea_mm.width() - 0.5*graphArea_mm.height()/aspectRatio)*dpi/DrawFigure::inchToMm,
                              graphArea_mm.y()*dpi/DrawFigure::inchToMm,
                              graphArea_mm.height()/aspectRatio*dpi/DrawFigure::inchToMm,
                              graphArea_mm.height()*dpi/DrawFigure::inchToMm);
            }else if(hAlign==H_ALIGN::RIGHT){
                return QRectF((graphArea_mm.x() + graphArea_mm.width() - graphArea_mm.height()/aspectRatio)*dpi/DrawFigure::inchToMm,
                              graphArea_mm.y()*dpi/DrawFigure::inchToMm,
                              graphArea_mm.height()/aspectRatio*dpi/DrawFigure::inchToMm,
                              graphArea_mm.height()*dpi/DrawFigure::inchToMm);
            }
        }else{
            //指定の方が横長　→　横はいっぱいに、縦はvAlignに従う
            if(vAlign==V_ALIGN::TOP){
                return QRectF(graphArea_mm.x()*dpi/DrawFigure::inchToMm,
                              graphArea_mm.y()*dpi/DrawFigure::inchToMm,
                              graphArea_mm.width()*dpi/DrawFigure::inchToMm,
                              graphArea_mm.width()*aspectRatio*dpi/DrawFigure::inchToMm);
            }else if(vAlign==V_ALIGN::MIDDLE){
                return QRectF(graphArea_mm.x()*dpi/DrawFigure::inchToMm,
                              (graphArea_mm.y()+ 0.5*graphArea_mm.height() - 0.5*graphArea_mm.width()*aspectRatio)*dpi/DrawFigure::inchToMm,
                              graphArea_mm.width()*dpi/DrawFigure::inchToMm,
                              graphArea_mm.width()*aspectRatio*dpi/DrawFigure::inchToMm);
            }else if(vAlign==V_ALIGN::BOTTOM){
                return QRectF(graphArea_mm.x()*dpi/DrawFigure::inchToMm,
                              (graphArea_mm.y()+ graphArea_mm.height() - graphArea_mm.width()*aspectRatio)*dpi/DrawFigure::inchToMm,
                              graphArea_mm.width()*dpi/DrawFigure::inchToMm,
                              graphArea_mm.width()*aspectRatio*dpi/DrawFigure::inchToMm);
            }
        }
        break;
    }
    return QRectF(graphArea_mm.x()*dpi/DrawFigure::inchToMm,
                  graphArea_mm.y()*dpi/DrawFigure::inchToMm,
                  graphArea_mm.width()*dpi/DrawFigure::inchToMm,
                  graphArea_mm.height()*dpi/DrawFigure::inchToMm);
}

QRectF GRAPH_SIZE_OPTION::plotArea(qreal dpi) const{//pixcel系
    QRectF ga = graphArea(dpi);
    if(ga.width()-(plotAreaPaddings.left()+plotAreaPaddings.right())*dpi/DrawFigure::inchToMm<=0)return QRectF(0,0,0,0);
    if(ga.height()-(plotAreaPaddings.top()+plotAreaPaddings.bottom())*dpi/DrawFigure::inchToMm<=0)return QRectF(0,0,0,0);

    return ga - plotAreaPaddings * dpi / DrawFigure::inchToMm;
}

QSizeF GRAPH_PROPERTY::TargetSize() const
{
    qreal bb = paperMargin_bottom;
    if(drawFooter)
        bb += footerSize;
    if(drawImage){
        bb += ImageSize().height();
    }
    return QSizeF(
                unitPaper.Size().width() - paperMargin_left - paperMargin_right,
                unitPaper.Size().height() - Origin().y() - bb);
}

QPointF GRAPH_PROPERTY::Origin() const
{
    qreal tt = paperMargin_top;
    if(drawHeader)
        tt += headerSize;
    if(drawLegend)
        tt += GraphScene::legendSize(legendType).height();

    return QPointF(paperMargin_left, tt);
}

QSizeF GRAPH_PROPERTY::ImageSize() const
{
    qreal bb = paperMargin_bottom;
    if(drawFooter)
        bb += footerSize;
    qreal width = qMin(180.0, unitPaper.Size().width() - paperMargin_left - paperMargin_right);
    qreal height = qMin(200.0, 0.5 * (unitPaper.Size().height() - Origin().y() - bb));
    return QSizeF(width, height);
}

QList<GRAPH_SUBTYPE_PROPERTY> GRAPH_TYPE_PROPERTY::makeSubType() const
{
    QList<GRAPH_SUBTYPE_PROPERTY> ret;
    switch(status){
    case OutputFigureStatus::Dscurve:
    case OutputFigureStatus::Qucurve:
    {
        GRAPH_SUBTYPE_PROPERTY sub;
        sub.type = OUTFIGTYPE::QDelta;
        sub.caseName = caseName;
        sub.qu_or_ds = ds_or_qu;
        ret.append(sub);
    }break;
    case OutputFigureStatus::DscurveFloor:
    case OutputFigureStatus::QucurveFloor:
    {
        GRAPH_SUBTYPE_PROPERTY sub;
        sub.type = OUTFIGTYPE::QDeltaFloorFrames;
        sub.floorName = baseName;
        // sub.frameGroupName = TODO
        sub.caseName = caseName;
        sub.qu_or_ds = ds_or_qu;
        ret.append(sub);
    }break;
    case OutputFigureStatus::DscurveFrame:
    case OutputFigureStatus::QucurveFrame:
    {
        GRAPH_SUBTYPE_PROPERTY sub;
        sub.type = OUTFIGTYPE::QDeltaFrameFloors;
        sub.frameName = baseName;
        sub.caseName = caseName;
        sub.qu_or_ds = ds_or_qu;
        ret.append(sub);
    }break;
    case OutputFigureStatus::DsQThetaCurve:
    case OutputFigureStatus::QuQThetaCurve:
    {
        GRAPH_SUBTYPE_PROPERTY sub;
        sub.type = OUTFIGTYPE::QTheta;
        sub.caseName = caseName;
        sub.qu_or_ds = ds_or_qu;
        ret.append(sub);
    }break;
    case OutputFigureStatus::DsQThetaCurveFloor:
    case OutputFigureStatus::QuQThetaCurveFloor:
    {
        GRAPH_SUBTYPE_PROPERTY sub;
        sub.type = OUTFIGTYPE::QThetaFloorFrames;
        sub.floorName = baseName;
        sub.caseName = caseName;
        sub.qu_or_ds = ds_or_qu;
        ret.append(sub);
    }break;
    case OutputFigureStatus::DsQThetaCurveFrame:
    case OutputFigureStatus::QuQThetaCurveFrame:
    {
        GRAPH_SUBTYPE_PROPERTY sub;
        sub.type = OUTFIGTYPE::QThetaFrameFloors;
        sub.frameName = baseName;
        sub.caseName = caseName;
        sub.qu_or_ds = ds_or_qu;
        ret.append(sub);
    }break;
    case OutputFigureStatus::Dsqucurve:
    {
        GRAPH_SUBTYPE_PROPERTY subDs;
        subDs.type = OUTFIGTYPE::QDelta;
        subDs.caseName = caseName;
        subDs.qu_or_ds = BASE_TYPE_PROPERTY::DSQU::DS;
        ret.append(subDs);

        GRAPH_SUBTYPE_PROPERTY subQu;
        subQu.type = OUTFIGTYPE::QDelta;
        subQu.caseName = caseName;
        subQu.qu_or_ds = BASE_TYPE_PROPERTY::DSQU::QU;
        ret.append(subQu);
    }break;
    case OutputFigureStatus::Quneedf:
    {
        GRAPH_SUBTYPE_PROPERTY subDs;
        subDs.type = OUTFIGTYPE::QuNeedf;
        subDs.caseName = caseName;
        subDs.qu_or_ds = BASE_TYPE_PROPERTY::DSQU::DS;
        ret.append(subDs);

        GRAPH_SUBTYPE_PROPERTY subQu;
        subQu.type = OUTFIGTYPE::QuNeedf;
        subQu.caseName = caseName;
        subQu.qu_or_ds = BASE_TYPE_PROPERTY::DSQU::QU;
        ret.append(subQu);
    }break;
//    case OUTFIGTYPE::NMINT:
//        break;
    default:
        throw;
    }

    return ret;
}

QString GRAPH_TYPE_PROPERTY::title() const
{
    return QString{};
}

QString STRESS::text() const
{
    QString ret = QString::number(coef,'f',1) +u8"x"+ rst;
    if (isSet == false){
        // stepを指定しない場合。
    }else{
        ret += u8":" + QString::number(step) + u8"/" + QString::number(maxStep) + u8"step";
    }

    return ret;
}


} // namespace post3dapp
