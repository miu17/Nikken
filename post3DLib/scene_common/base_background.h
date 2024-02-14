#ifndef BASEBACKGROUND_H
#define BASEBACKGROUND_H

#include "define_unifieddata.h"
#include "global_point.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QPainter;

namespace post3dapp{
class FIGURE_PROPERTY;
class FIGURE_SIZE_RESULT;
class UnifiedData;
struct GLOBALLINE;

/* 通り名・描画中心 */
struct FRNAMEANDPOINT {
    explicit FRNAMEANDPOINT(
        const QString &_name = QString(),
        const QPointF &_point = QPointF() )
        : frameName(_name), nameCenter(_point) { }

    QString frameName;
    QPointF nameCenter;
};

class POST3D_CLASS_DLLSPEC BaseBackGround
{
public:
    BaseBackGround(bool _useScaledFont);
    virtual ~BaseBackGround(){};

    void SetUseScaledFont(bool _useScaledFont){useScaledFont = _useScaledFont;}

    virtual void completeGlobalXYZ(GlobalPoint &gp) = 0;
    virtual void resetFramePoints(const QUuid &fr_id)=0;
    virtual QRectF boundingRect()const =0;
    virtual QList<GlobalPoint> getCrossPoints() const =0;
    virtual QList<GlobalPoint> getDefinePoints() const =0;

    void setColor( const QColor &col1, const QColor &col2 = Qt::black );
    void setScaleTextSize(int);
    void setScaleTextVisible(bool _FloorTextVisible, bool _GridTextVisible);
    void setDrawCrossMode(bool onoff);
    void setSpanColorMode(bool onoff);

    void drawWhole(QPainter *painter);
    void drawWholeWithScale(QPainter *painter, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &pos);
    void drawPart(QPainter *painter,const int tate,const int yoko,const FIGURE_SIZE_RESULT& size,
                          const FIGURE_PROPERTY& pos, bool const tatediv, bool const yokodiv);

protected:
    virtual void clearDrawingData() = 0;
    virtual void drawCore(QPainter *painter) = 0;
    virtual void drawDivide(QPainter *painter,const int tate,const int yoko,const FIGURE_SIZE_RESULT& size,
                          const FIGURE_PROPERTY& pos, bool const tatediv, bool const yokodiv)=0;
    void setScaleAndUpSideDown(QPainter *painter, const int tate, const int yoko, const FIGURE_SIZE_RESULT& size, const FIGURE_PROPERTY& pos);
    qreal FontSize(const QPainter&, int) const;
    qreal DpiScale(const QPainter&) const;


    bool useScaledFont;
    QUuid myFloorFrameID;
    UnifiedData* myData;

    QColor defColor;
    QColor curColor;

    int scaleTextSize;
    bool floorTextVisible;
    bool gridTextVisible;

    bool drawingCross;
    bool coloringSpan;

    QList<QUuid> currentFrames;

};

class POST3D_CLASS_DLLSPEC BaseBackGroundFloor : public BaseBackGround
{
public:

    BaseBackGroundFloor(bool _useScaledFont, UnifiedData* data);
    virtual ~BaseBackGroundFloor(){}

    void completeGlobalXYZ(GlobalPoint &gp)override;
    void resetFramePoints(const QUuid &fl_id)override;
    void resetFramePoints(const QList<FRAMEPOINTSVALUE> &);
    QList<GlobalPoint> getCrossPoints() const override;
    QList<GlobalPoint> getDefinePoints() const override;
    QRectF boundingRect()const override;

    QList<GLOBALLINE> calcBatchColumnLines();
    void setCurrentFrame(const QList<QUuid> &);
    void clearCurrentFrame();
    void setCurrentGID(const QUuid &);
    void clearCurrentGID();

protected:
    void clearDrawingData()override;
    void drawCore(QPainter *painter) override;
    void drawDivide(QPainter *painter,const int tate,const int yoko,const FIGURE_SIZE_RESULT& size,
                          const FIGURE_PROPERTY& pos, bool const tatediv, bool const yokodiv) override;

private:
    QPointF checkIntersect(const QPointF p1, const QPointF p2, const QPointF p3, const QPointF p4)const;

    QUuid currentGID;
    QList<FRAMEPOINTSVALUE> drawFrameList;
    QList<GlobalPoint> crossPointList;
    QList<GlobalPoint> definePointList;
};

class POST3D_CLASS_DLLSPEC BaseBackGroundFrame : public BaseBackGround
{
public:
    BaseBackGroundFrame(bool _useScaledFont, UnifiedData* data);
    virtual ~BaseBackGroundFrame(){}

    void completeGlobalXYZ(GlobalPoint &gp)override;
    void resetFramePoints(const QUuid &fr_id)override;
    QList<GlobalPoint> getCrossPoints() const override;
    QList<GlobalPoint> getDefinePoints() const override;
    QRectF boundingRect()const override;

    void setCurrentFloors(const QList<QUuid> &);
    void setCurrentFrames(const QList<QUuid> &);
    void clearCurrentFloors();
    void clearCurrentFrames();

protected:
    void clearDrawingData()override;
    void drawCore(QPainter *painter) override;
    void drawDivide(QPainter *painter,const int tate,const int yoko,const FIGURE_SIZE_RESULT& size,
                          const FIGURE_PROPERTY& pos, bool const tatediv, bool const yokodiv) override;
    virtual bool IsAnalysisFigure(){return false;}

private:
    QList<GlobalPoint> calcCrossPoints( const QList<QPointF> &, const QList<QPointF> & );
    void calcCrossFrameLines();
    int indexOfGlobalPoint(const QList<GlobalPoint> &gplist, const GlobalPoint &gp, qreal eps);
    QList<QUuid> currentFloors;

    /* 描画処理のためのデータ */
    QList<QUuid> floorIDList;
    QList<QUuid> drawFloorIDList;
    QMultiHash< QUuid, QLineF > crossFrameLines;
    QMultiHash< QUuid, FRNAMEANDPOINT > crossFrameNames;
    QList<QPointF> bottomFloorPoints;
    QHash< QUuid, QList<GlobalPoint> > crossPointList;
    QHash< QUuid, QList<GlobalPoint> > definePointList;
};

}


#endif // BASEBACKGROUND_H
