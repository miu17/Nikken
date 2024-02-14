#ifndef GRAPH3DVIEW_H
#define GRAPH3DVIEW_H

#include "graph_3d_scene.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp {

class POST3D_CLASS_DLLSPEC Graph3dView : public QGLViewer
{

    Q_OBJECT

public:

    Graph3dView( Graph3dScene* s, QWidget* parent,
                const QGLWidget* shareWidget=NULL);

//    void loadViewSettings(const MODELSETTINGS & );

    void fitBoundingBox(XYZ min,XYZ max);
    QString moldDoubleToText(qreal value,int length,int precision);

signals:

public slots:
protected :

    virtual void init();
    virtual void draw();

    virtual void paintGL(){
        init();
        QGLViewer::paintGL();
        update();
    }
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void paintEvent(QPaintEvent* event);

private:

    Graph3dScene* myScene;
//    MODELSETTINGS myViewSettings;
    qreal legendMin;
    qreal legendMax;

};
}
#endif // GRAPH3DVIEW_H
