#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include "define_output.h"

namespace post3dapp{
struct GRAPH_SUBTYPE_PROPERTY;
class Analysis2DViewSettings;
class GRAPH_PROPERTY;

class GraphScene;

class GraphView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphView(OUTFIGTYPE, bool _syncSetting, QWidget *parent = nullptr);
    ~GraphView();
    void resetSceneValue(const QList<GRAPH_SUBTYPE_PROPERTY>& opt,
//                           const QList<GRAPH_SIZE_OPTION>& size,
                           const GRAPH_PROPERTY& prop,
                         int graph_per_row,
                           const Analysis2DViewSettings& setting
                          );


//    void paintEvent(QPaintEvent* e)override{
//        if(scene != nullptr) scene->draw(this->paintEngine()->painter());
//    }
    GraphScene* getScene()const;

private:
    GraphScene *scene;
    const OUTFIGTYPE type;
};
} // namespace post3dapp

#endif // GRAPHVIEW_H
