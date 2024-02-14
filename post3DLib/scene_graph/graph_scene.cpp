#include "graph_scene.h"

#include "define_graph.h"
#include "fixed_data.h"
#include "graph_factory.h"
#include "unified_analysisdata.h"
#include "unified_data.h"
#include "unified_output.h"
#include "unified_settings.h"


namespace post3dapp{
GraphScene::GraphScene( const int dpi, bool _syncSetting, QObject* parent )
    : QGraphicsScene(parent),myDpi(dpi),syncSetting(_syncSetting), myViewSetting(Analysis2DViewSettings::getDefault())
{
    if (syncSetting){
//        connect( UnifiedSettings::getInstance(), SIGNAL(out2D_SettingChanged()),
//                 this, SLOT(updateViewSettings()) );
        connect( UnifiedSettings::getInstance(), &UnifiedSettings::out2D_SettingChanged,
                 this, &GraphScene::updateViewSettings );
    }

    connect( UnifiedFixedData::getInstance()->getPstnData(), &UnifiedOutput::outputDataUpdated,
             this, &GraphScene::resetGraphs );
}
void GraphScene::updateViewSettings()
{
    myViewSetting = UnifiedSettings::getInstance()->getOutput2DViewSettings();
    resetGraphs();
    update();
}
void GraphScene::setValues(const QList<GRAPH_SUBTYPE_PROPERTY> &opt,
//                           const QList<GRAPH_SIZE_OPTION> &size,
                           const GRAPH_PROPERTY &prop,
                           int graph_per_row,
                           const Analysis2DViewSettings &setting)

{
    qDebug()<<"GraphScene::setValues";
    graphOptions=opt;
//    graphSizes=size;
    myProperty=prop;
    numberOfGraphsPerRow=graph_per_row;
    myViewSetting=setting;
    setSceneRect(myProperty.Origin().x()*myDpi/DrawFigure::inchToMm,
                 myProperty.Origin().y()*myDpi/DrawFigure::inchToMm,
                 myProperty.TargetSize().width()*myDpi/DrawFigure::inchToMm,
                 myProperty.TargetSize().height()*myDpi/DrawFigure::inchToMm);
    resetGraphs();
    update();


}
void GraphScene::setImage(const QImage& image){
    myImage = image;
}

QList<GRAPH_SUBTYPE_PROPERTY> GraphScene::graphTypeOptions() const
{
    return graphOptions;
}

void GraphScene::resetGraphs(){
    mygraphs.clear();

    //    if(graphSizes.count()<graphOptions.count())return;

    for(int i=0;i<graphOptions.count();i++){
        GRAPH_SIZE_OPTION graphSize;
        graphSize.hAlign=GRAPH_SIZE_OPTION::CENTER;
        graphSize.vAlign=GRAPH_SIZE_OPTION::MIDDLE;
        graphSize.aspectPolicy=GRAPH_SIZE_OPTION::FIX;
        graphSize.aspectRatio=0.6;
        int row = i / qMax(1,numberOfGraphsPerRow);
        int col = i % qMax(1,numberOfGraphsPerRow);
        int maxRow=(graphOptions.count()-1) / qMax(1,numberOfGraphsPerRow)+1;
        QSizeF unitsize=QSizeF(myProperty.TargetSize().width()/qMax(1,numberOfGraphsPerRow),
                               myProperty.TargetSize().height()/qMax(1,maxRow));
        graphSize.targetRect=QRectF(myProperty.Origin().x()+unitsize.width()*col,
                                        myProperty.Origin().y()+unitsize.height()*row,
                                        unitsize.width(),
                                        unitsize.height());
        mygraphs << GraphFactory::makeOneGraph(UnifiedSettings::getInstance()->getOutput2DViewSettings(),graphOptions.at(i),graphSize);
    }

}


void GraphScene::drawForeground(QPainter *painter, const QRectF &rect){
    qreal scale = painter->device()->logicalDpiX()/DrawFigure::inchToMm;
    //image
    if(myProperty.drawImage){
        QSizeF imageSize=myProperty.ImageSize();
        qDebug()<<"drawForeGround imageSize"<<imageSize;
//        if(false){
        if(myImage != QImage()){
            //アスペクト比
            qreal yoko,tate;
            qDebug()<<imageSize.height()/imageSize.width()<<imageSize.height()<<imageSize.width();
            qDebug()<<myImage.height()/myImage.width()<<myImage.height()<<myImage.width();
            if((qreal)imageSize.height()/(qreal)imageSize.width() > (qreal)myImage.height()/(qreal)myImage.width()){
                yoko = imageSize.width();
                tate = yoko * myImage.height()/myImage.width();
            }else{
                tate = imageSize.height();
                yoko = tate * myImage.width()/myImage.height();
            }
            qreal ff = myProperty.drawFooter ? myProperty.footerSize : 0.0;
            QRectF imageTarget((0.5*myProperty.unitPaper.Size().width()-0.5*yoko) * scale,
                               (myProperty.unitPaper.Size().height() -myProperty.paperMargin_bottom - ff -0.5*imageSize.height()-0.5*tate) * scale,
                               yoko * scale,
                               tate* scale);
            painter->drawImage(imageTarget,myImage);
            painter->drawRect(imageTarget);
        }else{
            qreal ff = myProperty.drawFooter ? myProperty.footerSize : 0.0;
            QRectF imageTarget2((0.5*myProperty.unitPaper.Size().width()-0.5*imageSize.width()) * scale,
                               (myProperty.unitPaper.Size().height() -myProperty.paperMargin_bottom - ff-imageSize.height()) * scale,
                               imageSize.width() * scale,
                               imageSize.height()* scale);
            painter->drawRect(imageTarget2);
            painter->drawText(imageTarget2
                              ,Qt::AlignHCenter | Qt::AlignVCenter| Qt::TextDontClip, u8"NO IMAGE");

        }

    }
    //waku
    if(myProperty.drawWaku){
        painter->drawRect(myProperty.paperMargin_left * scale,
                          myProperty.paperMargin_top * scale,
                          (myProperty.unitPaper.Size().width()-myProperty.paperMargin_left - myProperty.paperMargin_right) * scale,
                          (myProperty.unitPaper.Size().height()-myProperty.paperMargin_top - myProperty.paperMargin_bottom)* scale
                          );
    }
    //header
    if(myProperty.drawHeader){
        QString header = myProperty.sceneTitle;
        QFontMetrics fm(painter->font());
        qreal sx =  (myProperty.unitPaper.Size().width()-myProperty.paperMargin_left - myProperty.paperMargin_right)*scale/ fm.width(header);
        qreal sy = myProperty.headerSize*scale / fm.height();
        qreal scale2 = qMin(sx,sy);
        qreal offset = 2.0;

        QRectF newRect = QRectF(myProperty.paperMargin_left*scale + offset,
                                myProperty.paperMargin_top*scale + offset,
                                fm.width(header) * scale2- 2.0*offset,
                                fm.height() * scale2- 2.0*offset);
        painter->save();
        painter->translate(newRect.center());
        painter->scale(scale2, scale2);
        painter->translate(-newRect.center());
        painter->drawText(newRect, Qt::AlignHCenter | Qt::AlignVCenter| Qt::TextDontClip, header);
        painter->restore();

    }

    //footer
    if(myProperty.drawFooter){

        QString footer = UnifiedFixedData::getInstance()->getAnalysisData()->fileName();
        QFontMetrics fm(painter->font());
        qreal sx = (myProperty.unitPaper.Size().width()-myProperty.paperMargin_left - myProperty.paperMargin_right)*scale / fm.width(footer);
        qreal sy = myProperty.footerSize*scale / fm.height();
//        qreal sy = (myProperty.unitPaperSize.width()-myProperty.paperMargin_left - myProperty.paperMargin_right)*scale / fm.height();
        qreal scale2 = qMin(sx,sy);
        qreal offset = 2.0;

        QRectF newRect = QRectF(myProperty.paperMargin_left*scale + offset,
                                ( myProperty.unitPaper.Size().height() - myProperty.paperMargin_bottom - myProperty.footerSize)*scale + offset,
                                fm.width(footer) * scale2 - 2.0*offset,
                                fm.height() * scale2- 2.0*offset);
        painter->save();
        painter->translate(newRect.center());
        painter->scale(scale2, scale2);
        painter->translate(-newRect.center());
        painter->drawText(newRect, Qt::AlignHCenter | Qt::AlignVCenter| Qt::TextDontClip, footer);
        painter->restore();

    }

    //legend
    QSize legend = legendSize(myProperty.legendType);
    if(myProperty.drawLegend){
        //myProperty.legendTypeによって描画内容を決める
        qreal hh = myProperty.drawHeader ? myProperty.headerSize : 0.0;
        qreal legendMargin = 2;
        QRectF legendTarget((myProperty.unitPaper.Size().width() - myProperty.paperMargin_right-legend.width()+legendMargin) * scale,
                            (myProperty.paperMargin_top + hh + legendMargin) * scale,
                            (legend.width()-legendMargin*2) * scale,
                            (legend.height()-legendMargin*2)* scale
                            );
        painter->drawRect(legendTarget);
//        painter->drawText(legendTarget
//                          ,Qt::AlignHCenter | Qt::AlignVCenter| Qt::TextDontClip, u8"DUMMY LEGEND");

       GraphFactory::drawLegend(painter,legendTarget,myProperty.legendType,myViewSetting);
    }



    //graph
    foreach(Graph graph,mygraphs){
        graph.draw(painter);
    }
}

QSize GraphScene::legendSize(OUTFIGTYPE)
{
    return QSize(60,40);//mm
}

} // namespace post3dapp
