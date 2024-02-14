#ifndef DEFINE_DRAW_FIGURE_H
#define DEFINE_DRAW_FIGURE_H

#include <QtCore>
#include <QPrinter>

#include "define_analysis2dsettings.h"
#include "define_analysis3dsettings.h"
#include "define_calculationload2dsettings.h"
#include "define_calculationload3dsettings.h"
#include "define_input2dsettings.h"
#include "define_input3dsettings.h"
#include "define_output.h"
#include "define_settings.h"
#include "unified_settings.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{

enum class SCENETYPE {PLANSCENE = 0, ELEVATIONSCENE};
enum class ITEMDIVRULE {IDVEQUAL = 0, IDVRATIO, IDVILEN, IDVJLEN};
enum class LISTTYPE { MODEL=0,INOUT,CHCK,STRESS,BUZAIJIKU,LOAD,LOADTYPE,QUDS,GRAPH,COMBIMAP};

enum SELECTFLAG { COL_SELECT = 0, GIR_SELECT, BRA_SELECT, BEA_SELECT,
                  SLA_SELECT, WAL_SELECT, UWA_SELECT, DAM_SELECT,
                  ISO_SELECT, JLD_SELECT, MLD_SELECT, SLD_SELECT
                };

enum VISIBLEFLAG { COL_VISIBLE = 0, GIR_VISIBLE, BRA_VISIBLE, BEA_VISIBLE,
                   SLA_VISIBLE, WAL_VISIBLE, UWA_VISIBLE, DAM_VISIBLE,
                   ISO_VISIBLE, JLD_VISIBLE, MLD_VISIBLE, SLD_VISIBLE
                 };
enum class PaperSize{DefaultSetting, A4Portrait, Custom};
enum class DividePolicy{AutoScale, // 紙サイズ・分割数を指定。それに収まるようにスケールを調整する。
                        PAPERSIZE_SCALE_DIVIDE, // 紙サイズを指定。スケール優先で紙数を増やす。　例)A3ヨコで1/400
                        PAPERSIZE_DIVIDE_SCALE, // 紙サイズを指定。分割数優先でスケールを調整する。　例)A3ヨコの1x1でAutoScale
                        SCALE_DIVIDE_PAPERSIZE, // スケールを指定。分割数優先で紙サイズを調整する。　例)1/400で1x1で紙サイズを調整。 下との使い分けがちょっと難しい？
                        DIVIDE_SCALE_PAPERSIZE, // 分割数を指定。スケールを優先して紙サイズを変更。　例)1x1で1/400で紙サイズを調整。
                        A4A3Auto, // A4タテに収まるようなオートスケール。オートスケールが小さすぎる場合はA3ヨコでオートスケール。
                                  // A3ヨコのオートスケールが小さすぎる場合はA3ヨコに収まるようにオートスケールだが警告。
                                  // 伏図と軸組図で縮尺が変更になる。
                        A4A3Scale,// 指定の縮尺でA4タテ。A4タテに収まらない場合はA3ヨコ。A3ヨコに収まらない場合は
                                  // 伏図と軸組図で縮尺同じ。
                        MANUAL}; // 指定された縮尺・枚数・紙サイズで描画する

struct PAPERDIVIDE
{
    PAPERDIVIDE(){}
    PAPERDIVIDE(const PAPERDIVIDE& _divide):tate(_divide.tate), yoko(_divide.yoko){}
    PAPERDIVIDE(int _tate,int _yoko):tate(_tate),yoko(_yoko){}
    int tate;
    int yoko;
    int figureCount() const {return tate * yoko;}
};

struct PAPER{
    explicit PAPER(QPagedPaintDevice::PageSize size, QPageLayout::Orientation ori){
        paperSize = size;
        orientation = ori;
    }
    explicit PAPER(const QSize& size){
        paperSize = QPagedPaintDevice::PageSize::Custom;
        manual = size;
    }

    static QSize getPaperSize(QPagedPaintDevice::PageSize size, QPageLayout::Orientation orient){
        if (size == QPagedPaintDevice::A4 && orient == QPageLayout::Portrait) return QSize(210, 297);
        if (size == QPagedPaintDevice::A4 && orient == QPageLayout::Landscape) return QSize(297, 210);
        if (size == QPagedPaintDevice::A3 && orient == QPageLayout::Portrait) return QSize(297, 420);
        if (size == QPagedPaintDevice::A3 && orient == QPageLayout::Landscape) return QSize(420, 297);
        if (size == QPagedPaintDevice::A2 && orient == QPageLayout::Portrait) return QSize(420, 594);
        if (size == QPagedPaintDevice::A2 && orient == QPageLayout::Landscape) return QSize(594, 420);
        if (size == QPagedPaintDevice::A1 && orient == QPageLayout::Portrait) return QSize(594, 841);
        if (size == QPagedPaintDevice::A1 && orient == QPageLayout::Landscape) return QSize(841, 594);
        return QSize(0, 0);
    }

    QSize Size() const
    {
        if (paperSize == QPagedPaintDevice::PageSize::Custom){
            return manual;
        }
        return getPaperSize(paperSize, orientation);
    }

    QPagedPaintDevice::PageSize PaperSize() const{return paperSize;}
    QPageLayout::Orientation Orientation() const{return orientation;}
    void setPrinter(QPrinter* printer){
        if (PaperSize() != QPrinter::PageSize::Custom){
            printer->setPageOrientation(Orientation());
            printer->setPageSize(PaperSize());
        }
        else{
            if (Size().width() > Size().height()){
                printer->setPageOrientation(QPageLayout::Landscape);
            }else{
                printer->setPageOrientation(QPageLayout::Portrait);
            }
            if (Size() == QSize{210, 297} || Size() == QSize{297, 210}){
                printer->setPageSize(QPagedPaintDevice::A4);
            }else if (Size() == QSize{297, 420} || Size() == QSize{420, 297}){
                printer->setPageSize(QPagedPaintDevice::A3);
            }else if (Size() == QSize{420, 594} || Size() == QSize{594, 420}){
                printer->setPageSize(QPagedPaintDevice::A2);
            }else if (Size() == QSize{594, 841} || Size() == QSize{841, 594}){
                printer->setPageSize(QPagedPaintDevice::A1);
            }else {
                throw;
            }
        }
    }

private:
    QPagedPaintDevice::PageSize paperSize;
    QPageLayout::Orientation orientation;
    QSize manual;


};

struct PAPERSETTING
{
    explicit PAPERSETTING(
            PAPER _paper = PAPER(QPagedPaintDevice::A4, QPageLayout::Portrait),
            PAPERDIVIDE _separate = PAPERDIVIDE(1, 1) )
        : paper(_paper),separate(_separate){ }

    PAPER paper;
    PAPERDIVIDE separate;

    QSize getPaperSize() const{
        return paper.Size();
    }
};

enum class DRAWMARKER{IfDivided=0,Always,NotDisplay};

class POST3D_CLASS_DLLSPEC FIGURE_SIZE_POLICY {
public:
    explicit FIGURE_SIZE_POLICY()
        :initial_paper(PAPER(QPagedPaintDevice::A4, QPageLayout::Portrait))
    {
        includeBackGround = true;
        policy = DividePolicy::PAPERSIZE_SCALE_DIVIDE;

        initial_divide = PAPERDIVIDE(1,1);
        initial_scale = 1 / 100.0;

        upperBound_divide = PAPERDIVIDE(1,1);
        upperBound_scale = 1 / 200.0;
        upperBound_paperSize = QList<QSize>{PAPER::getPaperSize(QPagedPaintDevice::A3, QPageLayout::Landscape)};

        modelMargin_left = 1000.0;
        modelMargin_right = 1000.0;
        modelMargin_top = 1000.0;
        modelMargin_bottom = 1000.0;
        modelMargin_overlap = 1000.0;

        markerSize = 10;
        drawMarker = DRAWMARKER::IfDivided;

        drawWaku = true;
        drawHeader = false;
        drawFooter = false;
        headerSize = 6.0;
        footerSize = 6.0;

        paperMargin_left=10.0;
        paperMargin_right=10.0;
        paperMargin_top=10.0;
        paperMargin_bottom=10.0;

        headerSize = 6;//mm
        footerSize = 6;//mm
    }

    static FIGURE_SIZE_POLICY Recreate(const FIGURE_SIZE_POLICY& old, const PAPER& paper, DividePolicy dividePolicy){
        FIGURE_SIZE_POLICY newPolicy;
        newPolicy.includeBackGround = old.includeBackGround;
        newPolicy.policy = dividePolicy;
        newPolicy.initial_divide = old.initial_divide;
        newPolicy.initial_scale = old.initial_scale;
        newPolicy.initial_paper = paper;
        newPolicy.upperBound_divide = old.upperBound_divide;
        newPolicy.upperBound_scale = old.upperBound_scale;
        newPolicy.upperBound_paperSize = old.upperBound_paperSize;
        newPolicy.modelMargin_left = old.modelMargin_left;
        newPolicy.modelMargin_right = old.modelMargin_right;
        newPolicy.modelMargin_top = old.modelMargin_top;
        newPolicy.modelMargin_bottom = old.modelMargin_bottom;
        newPolicy.modelMargin_overlap = old.modelMargin_overlap;

        newPolicy.markerSize = old.markerSize;
        newPolicy.drawMarker = old.drawMarker;

        newPolicy.drawWaku = old.drawWaku;
        newPolicy.drawHeader = old.drawHeader;
        newPolicy.drawFooter = old.drawFooter;
        newPolicy.headerSize = old.headerSize;
        newPolicy.footerSize = old.footerSize;

        newPolicy.paperMargin_left = old.paperMargin_left;
        newPolicy.paperMargin_right = old.paperMargin_right;
        newPolicy.paperMargin_top = old.paperMargin_top;
        newPolicy.paperMargin_bottom = old.paperMargin_bottom;
        newPolicy.headerSize = old.headerSize;
        newPolicy.footerSize = old.footerSize;

        return newPolicy;
    }
    QSize initialPaperSize() const {return initial_paper.Size();}

    bool includeBackGround;
    DividePolicy policy;

    //希望値
    PAPERDIVIDE initial_divide;//tatexyoko
    qreal initial_scale; //　0.01等　ユーザー入力値の逆数
    PAPER initial_paper;

    //上限値
    PAPERDIVIDE upperBound_divide;//tatexyoko
    qreal upperBound_scale; // ユーザー入力値の逆数
    QList<QSize> upperBound_paperSize; // 大きくしていくサイズを順に

    qreal modelMargin_left;
    qreal modelMargin_right;
    qreal modelMargin_top;
    qreal modelMargin_bottom;
    qreal modelMargin_overlap; //分割したときに少しラップさせて描画

    qreal markerSize; //分割したときに表示する補助通り芯エリアの大きさ
    DRAWMARKER drawMarker;

    bool drawWaku;
    bool drawHeader;
    bool drawFooter;
    qreal headerSize;
    qreal footerSize;

    // マージンmm系
    qreal paperMargin_left;
    qreal paperMargin_right;
    qreal paperMargin_top;
    qreal paperMargin_bottom;
};

class POST3D_CLASS_DLLSPEC FIGURE_PROPERTY {
    // 分割全体の結果
public:
    explicit FIGURE_PROPERTY()
        :unitPaper(PAPER(QPagedPaintDevice::A4, QPageLayout::Portrait)){
//        drawPosition = PAPERDIVIDE(1,1);
        markerSize = 10;//10mm,paper座標系
        drawMarker = DRAWMARKER::IfDivided;

        drawWaku = true;
        drawHeader = true;
        drawFooter = true;
        headerSize = 6;//mm
        footerSize = 6;//mm

        modelMargin_left = 2000;
        modelMargin_right = 2000;
        modelMargin_top = 2000;
        modelMargin_bottom = 2000;
        modelMargin_overlap = 1000;//1m,model座標系

        outerTargetSize = unitPaper.Size() + QSizeF(-20, -20);
        sourceRects.append(QList<QRectF>{QRectF(-10000,-10000,50000,50000)});
        divide = PAPERDIVIDE(1,1);
        scale = 1 / 100.0;

        state = 0;
    }
    FIGURE_PROPERTY(const FIGURE_SIZE_POLICY& policy)
        :unitPaper(policy.initialPaperSize()){
        modelMargin_left = policy.modelMargin_left;
        modelMargin_right = policy.modelMargin_right;
        modelMargin_top = policy.modelMargin_top;
        modelMargin_bottom = policy.modelMargin_bottom;
        modelMargin_overlap = policy.modelMargin_overlap;
        markerSize = policy.markerSize;
        drawMarker = policy.drawMarker;

        drawWaku = policy.drawWaku;
        drawHeader = policy.drawHeader;
        drawFooter = policy.drawFooter;
        headerSize = policy.headerSize;
        footerSize = policy.footerSize;

        outerTargetSize = QSizeF(unitPaper.Size().width() - policy.paperMargin_left - policy.paperMargin_right,
                                 unitPaper.Size().height() -policy.paperMargin_top - policy.paperMargin_bottom);
    }
//    PAPERDIVIDE drawPosition;//tate x yoko
    //margin global座標系(mm)
    qreal modelMargin_left;
    qreal modelMargin_right;
    qreal modelMargin_top;
    qreal modelMargin_bottom;
    qreal modelMargin_overlap; //分割したときに少しラップさせて描画

    qreal markerSize; //分割したときに表示する補助通り芯エリアの大きさ
    DRAWMARKER drawMarker;

    bool drawWaku;
    bool drawHeader;
    bool drawFooter;
    qreal headerSize;
    qreal footerSize;

    PAPER unitPaper;
    PAPERDIVIDE divide;//tatexyoko
    qreal scale;

    QSizeF outerTargetSize; // 紙面の幅×高さ 全分割で共通
    QSizeF TargetSize() const // 紙面の幅×高さから分割マーカーサイズを引いたもの 全分割で共通
    {
        QSizeF size = outerTargetSize;
        if(drawHeader)
            size -= QSizeF(0, headerSize);
        if(drawFooter)
            size -= QSizeF(0, footerSize);

        if((divide.tate == 1 && divide.yoko == 1 && drawMarker==DRAWMARKER::IfDivided)
                || (drawMarker==DRAWMARKER::NotDisplay)){
            //マーカーなし
        }else if((divide.tate > 1 && divide.yoko ==1 && drawMarker == DRAWMARKER::IfDivided)){
            //マーカー上下
            size -= QSizeF(0, 2.0 * markerSize);
        }else if((divide.tate == 1 && divide.yoko > 1 && drawMarker == DRAWMARKER::IfDivided)){
            //マーカー左右
            size -= QSizeF(2.0 * markerSize, 0);
        }else{
            //マーカー上下左右
            size -= QSizeF(2.0 * markerSize, 2.0 * markerSize);
        }
        return size;
    }
    QList<QList<QRectF>> sourceRects; // 左下原点 mm系
    QRectF sourceRect(const int tate,const int yoko)const {
        if (tate >= divide.tate || yoko >= divide.yoko)
            return QRectF(-10000,-10000,50000,50000);
        return sourceRects[tate][yoko];
    }

    int state;//0=OK. 1=ひとつ上限に引っかかり2つ目の変数で対応　2=1つめの上限値に引っかかり、２つめの上限値を違反
};

class POST3D_CLASS_DLLSPEC FIGURE_SIZE_RESULT {
    // 分割個別の結果
public:
    FIGURE_SIZE_RESULT(){}
    FIGURE_SIZE_RESULT(const QPointF& _outerOrigin)
        :outerOrigin(_outerOrigin){}
    FIGURE_SIZE_RESULT(const FIGURE_SIZE_POLICY& _policy)
        :outerOrigin(QPointF(_policy.paperMargin_left, _policy.paperMargin_top)){}

    QRectF getInnerPixcelRect(const FIGURE_PROPERTY&, const int dpi) const;
    QRectF getOuterPixcelRect(const FIGURE_PROPERTY&, const int dpi) const;

private:
    QPointF outerOrigin; // 紙面上の描画範囲 mm系 左上原点
};

struct POST3D_CLASS_DLLSPEC STRESS {
    STRESS(){
        rst = QString();
        coef = 1.0;
    }
    QString rst;
    qreal coef;
    void setStep(int _step, int _maxStep){step = _step; maxStep = _maxStep; isSet = true;}
    int getStep() const {return isSet ? step : -1;} // 最終値を読み込んでいない場合は-1を返して、図側で読み込む
    QString text() const; // 例：1.0RSTDL,  1.0RSTE1:N/Nstep
private:
    bool isSet = false;
    int step;
    int maxStep;
};

class CustomDrawGrScene;

struct POST3D_CLASS_DLLSPEC BASE_TYPE_PROPERTY
{
    enum class DSQU{DS=0, QU};

    BASE_TYPE_PROPERTY(){}
    virtual ~BASE_TYPE_PROPERTY(){}
    MODELVIEWTYPE viewtype;//INOUTCALC・・・
    QString filename;
    virtual QString title() const = 0;
};

struct POST3D_CLASS_DLLSPEC FIGURE_TYPE_PROPERTY : public BASE_TYPE_PROPERTY
{
    FIGURE_TYPE_PROPERTY()
    {
        viewtype = MODELVIEWTYPE::INPUT2DMODEL;
        scenetype = SCENETYPE::PLANSCENE;
        name = QString();

        filename = QString();

        type = OUTFIGTYPE::UNDEFINED;
        viewdirection = OUTPUTDIR::ODIR_PLIN;
        stress.clear();

        model = QString();
        caseName = QString();
        ds_or_qu = DSQU::DS;
        floors_for_quds_curve = QStringList();
        frames_for_quds_curve = QStringList();

        loadType = LoadType::LOADTYPE::LTYPE_DL;
        loadSumType = LOADSUMTYPE::LOADSUM_ALL;
        outputDir = CALCOUTPUTDIR::CALC_ODIR_X;
    }

    FIGURE_TYPE_PROPERTY(const FIGURE_TYPE_PROPERTY& copy)
    {
        viewtype = copy.viewtype;
        scenetype = copy.scenetype;
        name = copy.name;
        filename = copy.filename;
        type = copy.type;
        viewdirection = copy.viewdirection;
        stress = QList<QList<STRESS>>{copy.stress};
        model = copy.model;
        caseName = copy.caseName;
        ds_or_qu = copy.ds_or_qu;
        floors_for_quds_curve = QStringList{copy.floors_for_quds_curve};
        frames_for_quds_curve = QStringList{copy.frames_for_quds_curve};
        loadType = copy.loadType;
        loadSumType = copy.loadSumType;
        outputDir = copy.outputDir;
    }


    //共通
    SCENETYPE scenetype;//伏軸パース・・・
    LISTTYPE listtype() const { return listType(type);};//モデル系、応力図系、グラフ系etc・・・
    QString name;//通り名階名

    //応力系
    OUTFIGTYPE type;
    OUTPUTDIR viewdirection;//面内面外軸方向
    QList<QList<STRESS>> stress;

    //モデル系
    QString model;

    //二次設計系
    QString caseName;//断面検討ケースやQUDSのケース名
    DSQU ds_or_qu;
    QStringList floors_for_quds_curve;
    QStringList frames_for_quds_curve;

    //　荷重拾い結果
    LoadType::LOADTYPE loadType;
    LOADSUMTYPE loadSumType;
    CALCOUTPUTDIR outputDir; //3D
    CustomDrawGrScene* makeScene(const Input2DViewSettings&,
                                 const Input3DViewSettings&,
                                 const CalculationLoad2DViewSettings&,
                                 const CalculationLoad3DViewSettings&,
                                 const Analysis2DViewSettings&,
                                 const Analysis3DViewSettings&,
                                 QObject *parent = nullptr) const;

    static LISTTYPE listType(OUTFIGTYPE);
    QString title() const override; // 例：（1.0RSTDL+1.0RSTLL1+1.0RSTE1:N/Nstep）
private:
    QString stressText() const;
};

class POST3D_CLASS_DLLSPEC OutputPaperSetting{
public:
    OutputPaperSetting(const Input2DViewSettings& input2D,
                       const Input3DViewSettings& input3D,
                       const CalculationLoad2DViewSettings& load2D,
                       const CalculationLoad3DViewSettings& load3D,
                       const Analysis2DViewSettings& analysis2D,
                       const Analysis3DViewSettings& analysis3D,
                       const FIGURE_SIZE_POLICY& policy)
        : input2DViewSetting(input2D),
          input3DViewSetting(input3D),
          calcLoad2DViewSetting(load2D),
          calcLoad3DViewSetting(load3D),
          analysis2DViewSetting(analysis2D),
          analysis3DViewSetting(analysis3D),
          figure_size_policy(policy){}

    const Input2DViewSettings input2DViewSetting;
    const Input3DViewSettings input3DViewSetting;
    const CalculationLoad2DViewSettings calcLoad2DViewSetting;
    const CalculationLoad3DViewSettings calcLoad3DViewSetting;
    const Analysis2DViewSettings analysis2DViewSetting;
    const Analysis3DViewSettings analysis3DViewSetting;
    const FIGURE_SIZE_POLICY figure_size_policy;

    /*
    BASEVIEWSETTINGS getSettingByProperty(OUTFIGTYPE figtype) const{
        switch(figtype){
        case OUTFIG_LOAD:
            // 本当は荷重表示のON/OFF設定が必要
            return input2DViewSetting;
        case OUTFIG_MODEL:
            return output2DViewSetting.model;
        case OUTFIG_LENGTH:
            return output2DViewSetting.memberLength;
        case OUTFIG_FACELENGTH:
            return output2DViewSetting.faceLength;
        case OUTFIG_COORDINATE:
            return output2DViewSetting.nodeCoordinate;
        case OUTFIG_RIGIDLENGTH:
            return output2DViewSetting.rigidLength;
        case OUTFIG_LATERALBUCKLEN:
            return output2DViewSetting.sideBucklingLength;
        case OUTFIG_EULERBUCKLEN:
            return output2DViewSetting.compressionBucklingLength;
        case OUTFIG_WEIGHT:
            return output2DViewSetting.nodeWeight;
        case OUTFIG_RIGIDITYINCREASE:
            return output2DViewSetting.rigidityRatio;
        case OUTFIG_CMQ:
            return output2DViewSetting.cmq;
            // 軸力図Mapは？
        case OUTFIG_REACTMAP:
            return output2DViewSetting.reactionForce;
        case OUTFIG_COMBIREACTMAP:
            return output2DViewSetting.combinatedReactionForce;
        case OUTFIG_SHEARMAP:
            return output2DViewSetting.shearForceMap;
        case OUTFIG_VERTICALLOADMAP:
            return output2DViewSetting.verticalLoadMap;
        case OUTFIG_GCKCP:
            return output2DViewSetting.floorRigidityMap; //?
        case OUTFIG_NSTRESS:
            return output2DViewSetting.nodeStress;
        case OUTFIG_FSTRESS:
            return output2DViewSetting.faceStress;
        case OUTFIG_AXIAL:
            return output2DViewSetting.axialForce;
        case OUTFIG_TORSION:
            return output2DViewSetting.torisionMoment;
        case OUTFIG_DEFORM:
            return output2DViewSetting.deformation;
        case OUTFIG_DISP:
            return output2DViewSetting.displacement;
        case OUTFIG_CHECKRATIO:
            return output2DViewSetting.check;
        case OUTFIG_ALLOWRATIOE:
            return output2DViewSetting.shearCheckE;
        case OUTFIG_ALLOWRATIOU:
            return output2DViewSetting.shearCheckU;
            //plasticrate
        case OUTFIG_RANK:
            return output2DViewSetting.rank;
        case OUTFIG_ALLOW:
            throw;
        case OUTFIG_QDelta:
            return output2DViewSetting.graphQDelta;
        case OUTFIG_QDeltaFrameFloors:
            return output2DViewSetting.graphQDeltaFloor;
        case OUTFIG_QDeltaFloorFrames:
            reutrn output2DViewSetting.graphQDeltaFrame;
        case OUTFIG_QTheta:
            return output2DViewSetting.graphQTheta;
        case OUTFIG_QThetaFrameFloors:
            return output2DViewSetting.graphQThetaFloor;
        case OUTFIG_QThetaFloorFrames:
            return output2DViewSetting.graphQThetaFrame;
        case OUTFIG_NMINT:
            return output2DViewSetting.graphNmint;
        case OUTFIG_DsQuCurve:
            throw;
        case OUTFIG_Quneedf:
            throw;
        case OUTFIG_END:
            break;
        }
    }
    */

};

class POST3D_CLASS_DLLSPEC DefineDrawFigure : public QObject
{
    Q_OBJECT

public:

    DefineDrawFigure(QObject *parent = nullptr):QObject(parent){}


    static qreal calcAutoScale(CustomDrawGrScene*, const FIGURE_SIZE_POLICY& policy);
    static qreal calcAutoScale(CustomDrawGrScene*, const QSize& peperSize, const FIGURE_SIZE_POLICY& policy);
    static qreal calcTateAutoScale(CustomDrawGrScene* , const QSize unitPaperSize,const int nTate, const FIGURE_SIZE_POLICY& policy);
    static qreal calcYokoAutoScale(CustomDrawGrScene*,const QSize unitPaperSize,const  int nYoko,const FIGURE_SIZE_POLICY& policy);
    static qreal calcUpperAutoScale(CustomDrawGrScene*, const FIGURE_SIZE_POLICY& policy);
};

class POST3D_CLASS_DLLSPEC GRAPH_PROPERTY {
    // グラフ用
    // FIGURE_SIZE_RESULTとの合体？
public:
    GRAPH_PROPERTY()
        :unitPaper(PAPER(QSize(210, 297)))
    {
        drawWaku = false;
        drawHeader = false;
        drawFooter = false;
        drawLegend = false;
        drawImage = false;
        legendType = OUTFIGTYPE::UNDEFINED;
        headerSize = 6;//mm
        footerSize = 6;//mm
        // マージンmm系
        paperMargin_left =10;
        paperMargin_right=10;
        paperMargin_top=10;
        paperMargin_bottom=10;

        sceneTitle = QString();
        state = 0;
    }
    GRAPH_PROPERTY(const FIGURE_SIZE_POLICY& policy)
        :unitPaper(policy.initial_paper)
    {
        drawWaku = policy.drawWaku;
        drawHeader = policy.drawHeader;
        drawFooter = policy.drawFooter;
        drawLegend = false;
        drawImage = false;
        legendType = OUTFIGTYPE::UNDEFINED;
        headerSize = policy.headerSize;
        footerSize = policy.footerSize;
        paperMargin_left = policy.paperMargin_left;
        paperMargin_right = policy.paperMargin_right;
        paperMargin_top = policy.paperMargin_top;
        paperMargin_bottom = policy.paperMargin_bottom;
        sceneTitle = QString{};
        state = 0;
    }

    bool drawWaku;
    bool drawHeader;
    bool drawFooter;
    bool drawLegend;
    bool drawImage;
    OUTFIGTYPE legendType;

    qreal headerSize;
    qreal footerSize;

    // マージンmm系
    qreal paperMargin_left;
    qreal paperMargin_right;
    qreal paperMargin_top;
    qreal paperMargin_bottom;

    PAPER unitPaper;
    QSizeF TargetSize() const;
    QPointF Origin() const;
    QSizeF ImageSize() const;

    QString sceneTitle;
    int state;//0=OK. 1=???
};

struct POST3D_CLASS_DLLSPEC GRAPH_SUBTYPE_PROPERTY
{
    // グラフ(例：必要保有水平耐力と保有水平耐力の比較図、NMINT)のうちの一つのグラフ
    GRAPH_SUBTYPE_PROPERTY(){
        type = OUTFIGTYPE::UNDEFINED;
        floorName = QString();
        frameName = QString();
        frameGroupName = QString();
        caseName = QString();
        subTitle = QString();
        qu_or_ds = BASE_TYPE_PROPERTY::DSQU::DS;
        icj = C;
        nm_type = NMy;
        memberId = QUuid();
    }
    OUTFIGTYPE type;
    QString frameName;//通り名
    QString frameGroupName;//通りグループ名
    QString floorName;//階名
    QString caseName;//QUDSケース名（XP,XMとか）　・　検討ケース名（VLとか）
    QString subTitle;//グラフ名
    BASE_TYPE_PROPERTY::DSQU qu_or_ds;

    //NMINTとかだと・・・
    //icj端
    //部材番号・UUIDなど
    enum ICJ{I=0,C,J};
    enum NMTYPE{NMy=0,NMz,MyMz};
    ICJ icj;    //i=0,c=1,j=2
    NMTYPE nm_type;//NMy=0,NMz=1,MyMz=2
    QUuid memberId;
//    QStringList stressCombiNames_forPlotPoints;
//    QList<QList<STRESS>>  stressCombi_forPlotPoints;
    static QList<GRAPH_SUBTYPE_PROPERTY> makeList(const FIGURE_TYPE_PROPERTY&);
};

enum class OutputFigureStatus; // 前方宣言
struct POST3D_CLASS_DLLSPEC GRAPH_TYPE_PROPERTY : public BASE_TYPE_PROPERTY
{
    // グラフ(例：必要保有水平耐力と保有水平耐力の比較図、NMINT)全体
public:
    GRAPH_TYPE_PROPERTY(OutputFigureStatus ostatus)
        :status(ostatus){
        viewtype = MODELVIEWTYPE::OUTPUTGRAPH;
    }
    QList<GRAPH_SUBTYPE_PROPERTY> makeSubType() const;
    void setNameList(const QStringList& list){nameList = list;}
    void setBaseName(const QString& name) {baseName = name;}
    void setDsQu(DSQU dsqu){ds_or_qu = dsqu;}
    QString caseName;
    QString title() const override;
private:
    const OutputFigureStatus status;
    DSQU ds_or_qu;
    QString baseName; // QDeltFrameFloorsのFrameなど基準となる通り／階
    QStringList nameList; // 通り名又は階名のリスト
};

struct POST3D_CLASS_DLLSPEC GRAPH_SIZE_OPTION
{
    GRAPH_SIZE_OPTION(){
        targetRect = QRectF(20,20,150.0,100.0);
        aspectPolicy =FIX;
        aspectRatio =0.75;
        hAlign=CENTER;
        vAlign=MIDDLE;
        outFramePaddings = QMarginsF(1,1,1,1);
        plotAreaPaddings = QMarginsF(10,10,5,10);

        scaleAutoX = JIKU_AUTO;
        scaleAutoY = JIKU_AUTO;
        fixed_xmin = 0;
        fixed_xmax = 1000;
        fixed_ymin = 0;
        fixed_ymax = 1000;
        logScaleX = false;
        logScaleY = false;
        useStringJikuX = false;
        useStringJikuY = false;
        stringJikuX = QStringList();
        stringJikuY = QStringList();
    }
    enum ASPECT_POLICY{FIX=0,EXPAND};
    enum H_ALIGN{LEFT=0,CENTER,RIGHT};
    enum V_ALIGN{TOP=0,MIDDLE,BOTTOM};
    enum JIKU_POLICY{JIKU_AUTO=0,JIKU_FIX,JIKU_BOTTOMFIX,JIKU_TOPFIX,JIKU_TAISYOU};
    //こっちで指定
    QRectF targetRect;//mm系
    int aspectPolicy;//アスペクト比　固定/拡大。
    qreal aspectRatio;// tate/yoko;
    int hAlign;//水平位置　左/センター/右
    int vAlign;//垂直位置　上/センター/下
    QMarginsF outFramePaddings;//パディング　mm系
    QMarginsF plotAreaPaddings;//パディング　mm系

    JIKU_POLICY scaleAutoX;//自動/原点対称/下側指定/上側指定
    JIKU_POLICY scaleAutoY;//自動/原点対称/下側指定/上側指定

    qreal fixed_xmin;
    qreal fixed_xmax;
    qreal fixed_ymin;
    qreal fixed_ymax;
    bool logScaleX;
    bool logScaleY;
    bool useStringJikuX;
    bool useStringJikuY;
    QStringList stringJikuX;
    QStringList stringJikuY;
    //内で計算
    //targetRect＝描画可能最大サイズ、そこからoutFramePaddingを除いたサイズの中で、graphArea、plotAreaをどうするかはアス比とアライメント、plotAreaPaddingで決める

    QRectF graphArea(qreal dpi)const;
    QRectF plotArea(qreal dpi)const;

};

} // namespace post3dapp
#endif // DEFINE_DRAW_FIGURE_H
