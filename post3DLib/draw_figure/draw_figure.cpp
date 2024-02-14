#include "draw_figure.h"

#include <QApplication>
#include <QPainter>

#include "base_background.h"
#include "define_draw_figure.h"
#include "define_unifieddata.h"
#include "elevationCI_grscene.h"
#include "elevationCL_grscene.h"
#include "elevationOT_grscene.h"
#include "elevationUI_grscene.h"
#include "fixed_data.h"
#include "planCI_grscene.h"
#include "planCL_grscene.h"
#include "planOT_grscene.h"
#include "planUI_grscene.h"
#include "unified_data.h"
#include "unified_inputdata.h"

namespace post3dapp{
//enum INOUT{INPUT,OUTPUT,CALCLOAD,INPUTCOPY};
//enum ELEVPLAN{ELEV,PLAN};

const qreal DrawFigure::inchToMm = 25.4;

DrawFigure::DrawFigure(){
    //計算書
    //デバイスは渡される、シーンを作る、スケール・分割数は別途求める、その中で指定の１ページだけ描画する

    //svg
    //デバイスを作る、シーンを作る、スケール・分割数を計算する、ループの中で1ページずつ描画・保存する

    //画面
    //デバイスは渡される、シーンは渡される、スケールはSceneから取得、1ページだけ描画する

    //印刷ダイアログ
    //デバイスは渡される、シーンは渡される、スケール・分割数を計算する、ループで１ページずつ描画・改ページする
}

void DrawFigure::drawForCalcPaper(QPainter* painter, const QRect& rect, CustomDrawGrScene* scene,
                                  int tate, int yoko, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &prop)
{
    drawOnePage(scene,painter,tate,yoko,size,prop);
}

void DrawFigure::drawForOutputFigure(QPainter* painter, CustomDrawGrScene* scene,
                                  const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &prop)
{
    /*
    QRect pixelRect = viewRect(prop,painter->device()->logicalDpiX());
    painter->setViewport(pixelRect);
    */
    for(int tate=0;tate<prop.divide.tate;tate++){
        for(int yoko=0;yoko<prop.divide.yoko;yoko++){
            drawOnePage(scene,painter,tate,yoko,size,prop);
            if(!(yoko == prop.divide.yoko-1 && tate==prop.divide.tate-1)){static_cast<QPrinter*>(painter->device())->newPage();}
        }
    }
}


/*
void DrawFigure::drawForSvgFile(BASEVIEWSETTINGS* setting,UnifiedData* data,
                                const FIGURE_SIZE_POLICY& policy,const FIGURE_TYPE_PROPERTY& type)
{
    //コードはイメージ。動作未確認
    FIGURE_PROPERTY prop;
    CustomDrawGrScene* scene = createScene(data,type,setting,false);
    FIGURE_SIZE_RESULT size = calcScaleAndDivision(scene,data,policy,type,prop,72.0);//svgのdpi
    for(int tate=0;tate<size.divide.tate;tate++){
        for(int yoko=0;yoko<size.divide.yoko;yoko++){
            QString filename = QApplication::applicationDirPath() + u8"/aaa_"+ QString::number(tate)+ u8"_"+QString::number(yoko)+".svg";
            QSvgGenerator* device = makeSvgFile(filename,size);
            QPainter painter;
            painter.begin(device);
            drawOnePage(scene,&painter,tate,yoko,size,prop,type,painter.device()->logicalDpiX());
            painter.end();
        }
    }
    return;
}
*/

QPair<FIGURE_PROPERTY, FIGURE_SIZE_RESULT> DrawFigure::drawForPrintDialog(QPainter* painter, CustomDrawGrScene* scene,
                                                  const FIGURE_SIZE_POLICY& policy)
{
    auto size = calcScaleAndDivision(scene,policy);
    drawForOutputFigure(painter, scene, size.second, size.first);
    return size;
}


OutputGrScene* DrawFigure::createOutScene(const FIGURE_TYPE_PROPERTY &type,  const Analysis2DViewSettings& setting, QObject *parent)
{
    if(type.viewtype==MODELVIEWTYPE::OUTPUT2DMODEL && type.scenetype==SCENETYPE::PLANSCENE){
        PlanOutputGrScene* scene = new PlanOutputGrScene(setting, false, type, parent);
        qDebug()<<"Uuid:"<<scene->getFrameFloorId();
        scene->resetItems();
        return scene;
     }
    else if(type.viewtype==MODELVIEWTYPE::OUTPUT2DMODEL && type.scenetype==SCENETYPE::ELEVATIONSCENE){
        ElevationOutputGrScene* scene = new ElevationOutputGrScene(setting, false, type, parent);
        qDebug()<<"Uuid:"<<scene->getFrameFloorId();
        scene->resetItems();
        return scene;
    }else{
        throw;
    }
}
CalcLoadGrScene* DrawFigure::createLoadScene(const FIGURE_TYPE_PROPERTY &type,  const CalculationLoad2DViewSettings& setting, QObject *parent)
{
    if(type.viewtype==MODELVIEWTYPE::CALC2DMODEL && type.scenetype==SCENETYPE::PLANSCENE){
        PlanCalcLoadGrScene* scene = new PlanCalcLoadGrScene(setting, type, false, parent);
        scene->setStatus(UnifiedFixedData::getInstance()->getInputData()->nameToID(type.name,DATATYPE::TPFLOOR));
        return scene;
    }else if(type.viewtype==MODELVIEWTYPE::CALC2DMODEL && type.scenetype==SCENETYPE::ELEVATIONSCENE){
        ElevationCalcLoadGrScene* scene = new ElevationCalcLoadGrScene(setting, type, false, parent);
        scene->setStatus(UnifiedFixedData::getInstance()->getInputData()->nameToID(type.name,DATATYPE::TPFRAMEP));
        return scene;
    }else{
        throw;
    }
}
InputGrScene* DrawFigure::createInputScene(const FIGURE_TYPE_PROPERTY &type,  const Input2DViewSettings& setting, QObject *parent)
{
    if(type.viewtype==MODELVIEWTYPE::INPUT2DMODEL && type.scenetype==SCENETYPE::PLANSCENE){
        PlanInterfaceGrScene* scene = new PlanInterfaceGrScene(LIMITSELECTTERM(), setting, false, parent);
        scene->setStatus(UnifiedInputData::getInInstance()->nameToID(type.name,DATATYPE::TPFLOOR));
        scene->resetItems();
        return scene;
    }else if(type.viewtype==MODELVIEWTYPE::INPUT2DMODEL && type.scenetype==SCENETYPE::ELEVATIONSCENE){
        ElevationInterfaceGrScene* scene = new ElevationInterfaceGrScene(LIMITSELECTTERM(), setting, false, parent);
        scene->setStatus(UnifiedInputData::getInInstance()->nameToID(type.name,DATATYPE::TPFRAMEP));
        scene->resetItems();
        return scene;
    }else{
        throw;
    }
}
InputGrScene* DrawFigure::createCopyScene(UnifiedInputData* data, const FIGURE_TYPE_PROPERTY &type,  const Input2DViewSettings& setting, QObject *parent)
{
    if(type.viewtype==MODELVIEWTYPE::INPUT2DMODELCOPY && type.scenetype==SCENETYPE::PLANSCENE){
        PlanCopyInterfaceGrScene* scene = new PlanCopyInterfaceGrScene(LIMITSELECTTERM(), data, setting, false, parent);
        scene->setStatus(UnifiedFixedData::getInstance()->getInputData()->nameToID(type.name,DATATYPE::TPFLOOR));
        scene->resetItems();
        return scene;
    }else if(type.viewtype==MODELVIEWTYPE::INPUT2DMODELCOPY && type.scenetype==SCENETYPE::ELEVATIONSCENE){
        ElevationCopyInterfaceGrScene* scene = new ElevationCopyInterfaceGrScene(LIMITSELECTTERM(), data, setting, false, parent);
        scene->setStatus(UnifiedFixedData::getInstance()->getInputData()->nameToID(type.name,DATATYPE::TPFRAMEP));
        scene->resetItems();
        return scene;
    }else{
        throw;
    }
}

void post3dapp::DrawFigure::setSourceRect(CustomDrawGrScene* scene, FIGURE_PROPERTY* property, const FIGURE_SIZE_POLICY &policy)
{
    QList<QList<QRectF>> rectsrects;
    QRectF bounding;
    if(!policy.includeBackGround){
        bounding = scene->itemsBoundingRect();//左下原点の座標系
    }else{
        bounding = scene->getBackGround()->boundingRect();//左下原点の座標系
    }    //mm系

    for(int tate=0; tate < property->divide.tate; tate++){
        QList<QRectF> rects;
        for(int yoko=0; yoko < property->divide.yoko; yoko++){
            qreal xx = bounding.x() - policy.modelMargin_left + property->TargetSize().width() * yoko / property->scale - (yoko) * policy.modelMargin_overlap;
            qreal yy = bounding.y() + bounding.height() + policy.modelMargin_top - property->TargetSize().height() * (tate+1) / property->scale + (tate) * policy.modelMargin_overlap;
            qreal ww = property->TargetSize().width() / property->scale;
            qreal hh = property->TargetSize().height() / property->scale;
            rects<<QRectF(xx,yy,ww,hh);
        }
        rectsrects<<rects;
    }
    property->sourceRects=rectsrects;
}

QPair<FIGURE_PROPERTY, FIGURE_SIZE_RESULT> DrawFigure::calcScaleAndDivision(CustomDrawGrScene* scene,
                                                    const FIGURE_SIZE_POLICY &policy)
{
    FIGURE_PROPERTY property(policy);
    switch(policy.policy){
    case DividePolicy::AutoScale:
    {
        property.scale = DefineDrawFigure::calcAutoScale(scene, policy);
        property.divide = policy.initial_divide;
        property.state = 0;
        property.unitPaper = policy.initial_paper;
        break;
    }
    case DividePolicy::PAPERSIZE_SCALE_DIVIDE:
    {
        //スケール優先。指定のスケールで分割数を増やす。XかYが最大分割数を超える場合、最大分割数でぎりぎり入る縮尺にする。
        //最大分割数でのスケール算定し、指定の初期スケールでおさまらない場合はそのまま描画
        qreal scale = DefineDrawFigure::calcAutoScale(scene, policy);
        qDebug()<<"scale auto"<<scale<<policy.upperBound_divide.tate<<policy.upperBound_divide.yoko;
        if (policy.initial_scale <= scale){
            //収まった
            property.divide = policy.initial_divide;
            property.scale = scale;
            property.state = 0;
        }else{
            // 上限枚数に収まるかの判定
            auto upperScale = DefineDrawFigure::calcUpperAutoScale(scene, policy);
            if (upperScale < policy.upperBound_scale){
                // 上限スケールにも収まらない場合
                // 算定したスケールで無理やりおさめる
                property.divide = policy.upperBound_divide;
                property.scale = upperScale;
                property.state = 2;
            }else{
                // 上限スケールには収まる場合は、分割数を再決定する
                int nYoko = 0;
                for (int yoko = 1; yoko <= policy.upperBound_divide.yoko; yoko++){
                    qreal scalex = DefineDrawFigure::calcYokoAutoScale(scene, policy.initialPaperSize(), yoko, policy);
                    if(policy.initial_scale <= scalex){
                        nYoko = yoko;
                        break;
                    }
                }
                int nTate = 0;
                for ( int tate = 1; tate <= policy.upperBound_divide.tate; tate++){
                    qreal scaley = DefineDrawFigure::calcTateAutoScale(scene, policy.initialPaperSize(), tate, policy);
                    if(policy.initial_scale <= scaley){
                        nTate = tate;
                        break;
                    }
                }
                property.divide = PAPERDIVIDE(nTate, nYoko);
                property.scale = policy.upperBound_scale;
                property.state = 1;
            }
        }

        property.unitPaper = policy.initial_paper;
        break;
    }
    case DividePolicy::PAPERSIZE_DIVIDE_SCALE:
    {
        //分割優先。指定の分割数でスケールが最大となるように描画。
        property.unitPaper =policy.initial_paper;
        property.divide = policy.initial_divide;
        qreal scale = DefineDrawFigure::calcAutoScale(scene, policy);
        property.scale = scale;
        if(scale < policy.upperBound_scale){
            property.state = 2;//上限スケール違反
        }else{
            property.state = 1;
        }
        break;
    }
    case DividePolicy::DIVIDE_SCALE_PAPERSIZE:
    {
        qreal scale = DefineDrawFigure::calcAutoScale(scene, policy);
        property.divide = policy.initial_divide;
        if (scale < policy.upperBound_scale){
            bool isOk = false;
            Q_FOREACH(auto size, policy.upperBound_paperSize){
                scale = DefineDrawFigure::calcAutoScale(scene, size, policy);
                property.unitPaper = PAPER(size);
                if (scale >= policy.upperBound_scale){
                    isOk = true;
                    break;
                }
            }
            if (isOk){
                property.state = 1;
            }else{
                property.state = 2;
            }
        }else{
            property.state = 0;
        }
        break;
    }
    case DividePolicy::A4A3Auto:
    {
        qreal scale = DefineDrawFigure::calcAutoScale(scene, policy);
        if (policy.initial_scale <= scale){
            // A4タテに収まる
            return calcScaleAndDivision(scene, FIGURE_SIZE_POLICY::Recreate(policy, PAPER(QPagedPaintDevice::A4, QPageLayout::Portrait), DividePolicy::PAPERSIZE_DIVIDE_SCALE));
        }
        else if (policy.upperBound_scale <= scale){
            // policyのinitial_scaleをupperBound_scaleに変更して再度A4A3AutoScale
            auto newPolicy = FIGURE_SIZE_POLICY::Recreate(policy, policy.initial_paper, DividePolicy::A4A3Auto);
            newPolicy.initial_scale = policy.upperBound_scale;
            return calcScaleAndDivision(scene, newPolicy);
        }
        else{
            // A3ヨコに収まらない
            return calcScaleAndDivision(scene, FIGURE_SIZE_POLICY::Recreate(policy, PAPER(QPagedPaintDevice::A3, QPageLayout::Landscape), DividePolicy::PAPERSIZE_SCALE_DIVIDE));
        }
        break;
    }
    case DividePolicy::A4A3Scale:
    {
        qreal scale = DefineDrawFigure::calcAutoScale(scene, policy);
        if (scale < policy.upperBound_scale){
            return calcScaleAndDivision(scene, FIGURE_SIZE_POLICY::Recreate(policy, PAPER(QPagedPaintDevice::A3, QPageLayout::Landscape), DividePolicy::PAPERSIZE_SCALE_DIVIDE));
        }
        else{
            return calcScaleAndDivision(scene, FIGURE_SIZE_POLICY::Recreate(policy, PAPER(QPagedPaintDevice::A4, QPageLayout::Portrait), DividePolicy::PAPERSIZE_SCALE_DIVIDE));
        }
        break;
    }
    case DividePolicy::MANUAL:
        property.divide=policy.initial_divide;
        property.scale=policy.initial_scale;
        property.state=0;
        property.unitPaper = policy.initial_paper;
        break;
    default:
        throw;
    }

    setSourceRect(scene, &property, policy);
    qDebug()<<"unitPaperSize"<<property.unitPaper.Size();
    return QPair<FIGURE_PROPERTY, FIGURE_SIZE_RESULT>(property, FIGURE_SIZE_RESULT(policy));
}


void DrawFigure::drawOnePage(CustomDrawGrScene* scene, QPainter* painter, const int tate, const int yoko,const  FIGURE_SIZE_RESULT& size,
                             const FIGURE_PROPERTY& prop)
{
    painter->save();
    qDebug()<<"drawPaperItems";
    scene->exportRenderPaperItems(painter, tate,yoko,size,prop,scene->getFigureTypeProperty());
    qDebug()<<"drawBackGroundItems";
    scene->exportRenderBackGround(painter, tate,yoko,size,prop);
    qDebug()<<"drawItems";
    scene->exportRenderItems(painter, tate,yoko,size,prop);
    qDebug()<<"drawItems Done";
    painter->restore();
}
QRect DrawFigure::viewRect(const FIGURE_PROPERTY& size, int dpi)
{
    QRect pixelRect = QRect(0,
                            0,
                            static_cast<int>(size.unitPaper.Size().width()/inchToMm*(qreal)dpi),//rect.drawArea.width()/inchToMm*dpi),
                            static_cast<int>(size.unitPaper.Size().height()/inchToMm*(qreal)dpi));//2.0*rect.drawArea.height()/inchToMm*dpi));
    return pixelRect;
}

QSvgGenerator* DrawFigure::makeSvgFile(const QString filename,const FIGURE_PROPERTY& size){
    QSvgGenerator* svggen = new QSvgGenerator;
    qreal mmToDotX = size.unitPaper.Size().width() / inchToMm * svggen->logicalDpiX();
    qreal mmToDotY = size.unitPaper.Size().height() / inchToMm * svggen->logicalDpiY();
    //    qreal mmToDotXshift = size.shift.x() / 25.4 * svggen.logicalDpiX();
    //    qreal mmToDotYshift = size.shift.y() / 25.4 * svggen.logicalDpiY();
    svggen->setSize(QSize(mmToDotX, mmToDotY));
    //    svggen.setViewBox(QRect(-mmToDotXshift, -mmToDotYshift, mmToDotX, mmToDotY));
    svggen->setTitle("sample");
    svggen->setDescription("create svg sample");
    //    QFileInfo fileinfo(QDir(filedirpath), filename);
    //    QString newfile = filedirpath + "/" + fileinfo.baseName() + "_" + QString::number(nh) + "_" + QString::number(nw) + "." + fileinfo.suffix();
    svggen->setFileName(filename);
    return svggen;
}

}
