#include "graph_view.h"

#include "graph_scene.h"

namespace post3dapp{

GraphView::GraphView(OUTFIGTYPE figtype, bool _syncSetting,QWidget *parent)
    :QGraphicsView(parent), type(figtype)
{

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene = new GraphScene( this->logicalDpiX(),_syncSetting);
    setScene(scene);
}

GraphView::~GraphView(){
    delete scene;
}
void GraphView::resetSceneValue(const QList<GRAPH_SUBTYPE_PROPERTY>& opt,
//                       const QList<GRAPH_SIZE_OPTION>& size,
                       const GRAPH_PROPERTY& prop,
                     int graph_per_row,
                       const Analysis2DViewSettings& setting)
{
    qDebug()<<"GraphView::resetSceneValue";
    scene->clear();
    scene->setValues(opt,prop,graph_per_row,setting);
}

GraphScene *GraphView::getScene() const{
    return scene;
}

} // namespace post3dapp
