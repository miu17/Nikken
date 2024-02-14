#ifndef OutputPersViewer_H
#define OutputPersViewer_H

#include "output_pers_scene.h"
#include "define_analysis3dsettings.h"
#include "define_outputpers.h"
#include "customdraw_grview.h"

namespace post3dapp{
class OutputPersViewer : public QGLViewer
{

    Q_OBJECT

public:

    OutputPersViewer( OutputPersScene* s, QWidget* parent,
                const QGLWidget* shareWidget=NULL);

    void loadViewSettings(const Analysis3DViewSettings  & );
//    void loadModeViewSettings( const MODE_MODELSETTINGS & );
//    void setDrawContour(bool _drawContour){myScene->setDrawContour(_drawContour);}
//    void setDrawLegend(bool _drawLegend){myScene->setDrawLegend(_drawLegend);}
//    void setDrawFileName(bool _drawFilename){myScene->setDrawFileName(_drawFilename);}
//    void setColorContourMinMax(qreal _min,qreal _max){
//        qDebug()<<"viewer color min max="<<_min<<_max;
//        myScene->setColorContour(_min,_max);
//        legendMin=_min;
//        legendMax=_max;
//    }

//    void setValueMinMax(qreal _min,qreal _max){
//        myScene->setValueMinMax(_min,_max);
//    }
//    void setValueMinMax(ValueMinMax _minmax){
//        myScene->setValueMinMax(_minmax);
//    }
    void fitBoundingBox(XYZ min,XYZ max);
    QString moldDoubleToText(qreal value,int length,int precision);

signals:

public slots:
//    void slotDrawString(QString str);
protected :

    virtual void init();
    virtual void draw();
    virtual void fastDraw();

    virtual void paintGL(){
        init();
        QGLViewer::paintGL();//draw or fastDraw
        QGLViewer::update();//paintEventが呼ばれる→drawが2重に呼ばれる？
    }
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void paintEvent(QPaintEvent* event);
    virtual void drawFileName(QPainter* painter);
    virtual void drawLegend(QPainter* painter);

private:

    OutputPersScene* myScene;
    MODELSETTINGS widgetState;
    //コンストラクタがプライベート。設定変更でウィジェットの破棄はしないので、変更シグナルを受けて同期をとりに行くか、UnifiedSettingsを直接参照する
//    Analysis3DViewSettings myViewSettings;
    //設定は、パース一般に関する部分、表現に固有の部分、ウィジェット上で持ちたい部分がある。ウィジェット上で持ちたい部分は、ウィジェットの状態を適宜反映するシグナルスロット。
    //UnifiedSettingsで持つもの→　適宜アクセスして最新を得る
    //ウィジェット上→変更時に同期をとる
    //描画範囲
    //カメラ
    qreal legendMin;
    qreal legendMax;

};
}
#endif
