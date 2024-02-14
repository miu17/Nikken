#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include <QImage>
#include "define_draw_figure.h"
#include "define_graph.h"
#include "define_analysis2dsettings.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
struct GRAPH_SUBTYPE_PROPERTY;
class Graph;

class POST3D_CLASS_DLLSPEC GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GraphScene(const int dpi, bool _syncSetting, QObject* parent = 0);
    virtual ~GraphScene(){};
    void setValues(const QList<GRAPH_SUBTYPE_PROPERTY>& opt,
//                   const QList<GRAPH_SIZE_OPTION>& size,
                   const GRAPH_PROPERTY& prop,int graph_per_row,
                   const Analysis2DViewSettings& setting);
    void setImage(const QImage &image);
    QList<GRAPH_SUBTYPE_PROPERTY> graphTypeOptions()const;
//    QList<GRAPH_SIZE_OPTION> graphSizeOptions()const{return graphSizes;}
    GRAPH_PROPERTY graphProperty()const{return myProperty;}
    int graphsPerRow()const{return numberOfGraphsPerRow;}

    void drawForeground(QPainter *painter, const QRectF &rect)override;
    static QSize legendSize(OUTFIGTYPE);

public slots:
    void updateViewSettings();
protected:
    void resetGraphs();
    QList<Graph> mygraphs;
    QImage myImage;

    int myDpi;

    QList<GRAPH_SUBTYPE_PROPERTY> graphOptions;
//    QList<GRAPH_SIZE_OPTION> graphSizes;
    GRAPH_PROPERTY myProperty;
    int numberOfGraphsPerRow;

    Analysis2DViewSettings myViewSetting;
    bool syncSetting;
};



} // namespace post3dapp

#endif // GRAPHSCENE_H
