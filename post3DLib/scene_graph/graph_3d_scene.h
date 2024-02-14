#ifndef GRAPH3DSCENE_H
#define GRAPH3DSCENE_H

#include <QGLViewer/qglviewer.h>
#include "ascii_table.h"
#include "define_graph.h"
#include "define_unifieddata.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
//class PersViewer;
//enum CAMERATYPE{ORTHOGRAPHIC=0,PERSPECTIVE};

class PointParameter
{
public:
    PointParameter(
            const QString & _name=QString()

            )
        : name(_name)
          { }
    enum PointType{NONE=0,PLOT,PLOTWITHNAME,PLOTWITHLINE};
    PointType type;

    QString name;
};
class LineParameter
{
public:
    LineParameter(
            const QString & _name=QString(),
            const QColor & _line= Qt::black,
            const QColor & _paint= Qt::red,
            const int & _width=1,
            const int & _style =1
            )
        : name(_name),lineColor(_line),paintColor(_paint),lineWidth(_width),lineStyle(_style)
          { }
    QString name;
    QColor lineColor;
    QColor paintColor;
    int lineWidth;
    int lineStyle;//1=solid,2=dash
};

class PlaneParameter
{
public:
    PlaneParameter(
            const QString & _name=QString(),
            const QColor & _line= Qt::darkYellow,
            const QColor & _paint= Qt::blue
            )
        : name(_name),lineColor(_line),paintColor(_paint)
          { }
    QString name;
    QColor lineColor;
    QColor paintColor;
};






/* 節点データ */

class Graph3DPoint
{
public:
    Graph3DPoint( XYZ coord,PointParameter param)
        : coordinate(coord),pointParameter(param) { }
//    ~Graph3DPoint(){}



    XYZ xyz() const { return coordinate; }
    qreal x() const{ return coordinate.x; }
    qreal y() const{ return  coordinate.y; }
    qreal z() const{ return coordinate.z; }
    PointParameter parameter() const{return pointParameter;}

private:
    XYZ coordinate;
    PointParameter pointParameter;

};
class Graph3DLine
{
public:
    Graph3DLine( const QList<Graph3DPoint>& _points, LineParameter _param)
    : points(_points),lineParameter(_param) { }
//    ~Graph3DLine(){}

    LineParameter getLineParameter() const { return lineParameter; }
    QList<Graph3DPoint> getPoints() const { return points; }

//    void removePoint(Graph3DPoint jd){ if( points.contains(jd) ) points.removeAll(jd) ;}

    Graph3DPoint getIPoint() const{ return points.first(); }
    Graph3DPoint getJPoint() const{ return points.last(); }

    XYZ ixyz()const;
    XYZ jxyz()const;

    qreal getMinX() const;
    qreal getMinY() const;
    qreal getMinZ() const;
    qreal getMaxX() const;
    qreal getMaxY() const;
    qreal getMaxZ() const;



private:
//    int NO;
    QList<Graph3DPoint> points;
    LineParameter lineParameter;
};





class Graph3DPlane
{
public:
    Graph3DPlane( const QList<Graph3DPoint>& _points, PlaneParameter _param): points(_points),planeParameter(_param) { }
//    ~Graph3DPlane(){}

    PlaneParameter getPlaneParameter() const { return planeParameter; }
    QList<Graph3DPoint> getPoints() const { return points; }
    QList<qglviewer::Vec> getTriangles() const {return planeElementTriangles;}

//    void removePoint(Graph3DPoint jd){ if( points.contains(jd) ) points.removeAll(jd) ;}


    qreal getMinX() const;
    qreal getMinY() const;
    qreal getMinZ() const;
    qreal getMaxX() const;
    qreal getMaxY() const;
    qreal getMaxZ() const;

    void divideIntoTriangles();

private:
    QList<Graph3DPoint> points;
    PlaneParameter planeParameter;
    QList<qglviewer::Vec> planeElementTriangles;

};


class POST3D_CLASS_DLLSPEC Graph3dScene : public QObject
{

    Q_OBJECT
signals:
public:

    Graph3dScene( QObject* parent=nullptr ) : QObject(parent){
        xScale=1.0;
        yScale=1.0;
        zScale=0.1;
        jikuStrX = u8"X";
        jikuStrY = u8"Y";
        jikuStrZ = u8"Z";
    }
    void drawSolid();
    void drawAxis();//const qglviewer::Vec &v_camera_right, const qglviewer::Vec &v_camera_up);
//    void drawText( const qglviewer::Vec &v_camera_right, const qglviewer::Vec &v_camera_up );
    void setCameraVec(const qglviewer::Vec &v_camera_right, const qglviewer::Vec &v_camera_up){
        camera_right = v_camera_right;
        camera_up = v_camera_up;
    }

    void clear(){
        allPoints.clear();
        allLines.clear();
        allPlanes.clear();
        xAxisValues.clear();
        yAxisValues.clear();
        zAxisValues.clear();
    }
    void setXAxis(QVector<qreal>values,QString label= QString()){xAxisValues=values;jikuStrX = label;}
    void setYAxis(QVector<qreal>values,QString label= QString()){yAxisValues=values;jikuStrY = label;}
    void setZAxis(QVector<qreal>values,QString label= QString()){zAxisValues=values;jikuStrZ = label;}
    void setXScale(qreal scl){xScale = scl;}
    void setYScale(qreal scl){yScale = scl;}
    void setZScale(qreal scl){zScale = scl;}

    void addPoint(Graph3DPoint elem){allPoints.append(elem);}
    void addLine(Graph3DLine elem){allLines.append(elem);}
    void addPlane(Graph3DPlane elem){allPlanes.append(elem);}

    qglviewer::Vec getMinVec(){return qglviewer::Vec(xAxisValues.first()*xScale,yAxisValues.first()*yScale,zAxisValues.first()*zScale);}
    qglviewer::Vec getMaxVec(){return qglviewer::Vec(xAxisValues.last()*xScale,yAxisValues.last()*yScale,zAxisValues.last()*zScale);}

public slots:

protected :

    QList<Graph3DPoint> allPoints;
    QList<Graph3DLine> allLines;
    QList<Graph3DPlane> allPlanes;

    QVector<qreal> xAxisValues;
    QVector<qreal> yAxisValues;
    QVector<qreal> zAxisValues;

    qreal xScale;
    qreal yScale;
    qreal zScale;

    QString jikuStrX;
    QString jikuStrY;
    QString jikuStrZ;

    void setLocalMatrix(qreal x1,  qreal y1,  qreal z1,
                        qreal x2,  qreal y2,  qreal z2
                        );
    void drawXZText(const QString &str, qreal swidth, qreal shx, qreal shy);
    void drawText( const QString& str,const XYZ pos);//,const qglviewer::Vec &v_camera_right, const qglviewer::Vec &v_camera_up );

    void createPlotWithLineElement(qreal _x,qreal _y,qreal _z,
                           const QString& str,const QColor& color,int size);
    void createLineElement(const QColor& col1, const QColor& col2, int weight, int style,
                          qreal l1, qreal l2);
    void createPlaneElement(const QColor& col1, const QColor& col2,
                            const QList<qglviewer::Vec> &plist, const QList<qglviewer::Vec> &tplist);

    AsciiTable ascii_table;//3DVisualのやつに置き換えてもよい
    qglviewer::Vec camera_right;
    qglviewer::Vec camera_up;

};
}
#endif // GRAPH3DSCENE_H
