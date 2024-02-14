#ifndef GRAPHFACTORY_H
#define GRAPHFACTORY_H

#include "define_graph.h" // GRAPH_TYPE_OPTION::ICJ

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif


class QPainter;
class QRectF;

namespace post3dapp {
class Analysis2DViewSettings;
class Graph;
class Graph3dScene;
class AnalysisGraphHorizontalCapacity2DViewSetting;
class AnalysisGraphNmint2DViewSetting;
class AnalysisGraphQDelta2DViewSetting;
struct GRAPH_SIZE_OPTION;

class POST3D_CLASS_DLLSPEC GraphFactory
{
    public:
        GraphFactory();
        //3dグラフはポインタを受け取って値をセットする
        //2dグラフはグラフで返す
        static QString makeGraphTitle(const GRAPH_SUBTYPE_PROPERTY& type);
        static void drawLegend(QPainter* p,QRectF target,OUTFIGTYPE figureType,Analysis2DViewSettings myViewSetting);
        static void drawLegendLine(QPainter* p,QRectF target,int line,int linesum,QPen pen,QString str);
        static void drawLegendMark(QPainter* p,QRectF target,int line,int linesum,DotType markType,int symbolSize,QColor symbolColor);

        static Graph makeOneGraph(const Analysis2DViewSettings &setting, const GRAPH_SUBTYPE_PROPERTY& type,
                          const GRAPH_SIZE_OPTION& size);
        static Graph makeQDeltaGraph(const AnalysisGraphQDelta2DViewSetting &setting, const GRAPH_SUBTYPE_PROPERTY& type, const GRAPH_SIZE_OPTION& size);
        static Graph makeQDeltaFrameFloorsGraph(const AnalysisGraphQDelta2DViewSetting& setting,const GRAPH_SUBTYPE_PROPERTY& type, const GRAPH_SIZE_OPTION& size);
        static Graph makeNmintGraph_CHCK(const AnalysisGraphNmint2DViewSetting& setting,const GRAPH_SUBTYPE_PROPERTY& type, const GRAPH_SIZE_OPTION& size);
//        static Graph makeNmintGraph_NMIN(const OutputGraphNmint2DViewSetting& setting,const GRAPH_TYPE_OPTION& type, const GRAPH_SIZE_OPTION& size);

        static Graph makeHorizontalCapacityGraph(const AnalysisGraphHorizontalCapacity2DViewSetting& setting,const GRAPH_SUBTYPE_PROPERTY& type,const GRAPH_SIZE_OPTION& size);
        static Graph makeQDeltaFloorFramesGraph(const AnalysisGraphQDelta2DViewSetting& setting,const GRAPH_SUBTYPE_PROPERTY& type,const GRAPH_SIZE_OPTION& size);

        static void makeNmint3dGraph_CHCK(const AnalysisGraphNmint2DViewSetting& setting,Graph3dScene* s,GRAPH_SUBTYPE_PROPERTY opt);
//        static void makeNmint3dGraph_NMIN(const OutputGraphNmint2DViewSetting& setting,Graph3dScene* s,GRAPH_TYPE_OPTION opt);
        static Graph makeNullGraph(const GRAPH_SIZE_OPTION& size);
private:
        static void sub_make3dNMSurface(const AnalysisGraphNmint2DViewSetting& setting,Graph3dScene* s,int nrstn,int nrstm,
                                         const QVector<QVector<double>>& mCurve_x,
                                         const QVector<QVector<double>>& mCurve_y,
                                         const QVector<double>& nCurve,
                                         const QColor& lineColor,const QColor& planeColor);
        static Series sub_makeNMyCurve(const QPen &pen,  int nrstn, int nrstm,
                                               const QVector<QVector<double>>& mCurve, const QVector<double>& nCurve);
        static Series sub_makeNMzCurve(const QPen &pen,  int nrstn, int nrstm,
                                                const QVector<QVector<double>>& mCurve,const QVector<double>& nCurve);
        static Series sub_makeMyMzCurve(const QPen &pen,  qreal N,int nrstn, int nrstm,
                                                const QVector<double>& nCurve,
                                                const QVector<QVector<double>>& mCurvex,
                                                const QVector<QVector<double>>& mCurvey);

        static void sub_make3dNMPlotPoint(const AnalysisGraphNmint2DViewSetting& setting, Graph3dScene* s,
                                                 GRAPH_SUBTYPE_PROPERTY::ICJ icj,
                                                 QUuid memberId,
                                                 QString stressCombiName, QList<STRESS> stress_kotei, QList<STRESS> stress);
};
}
#endif // GRAPHFACTORY_H
