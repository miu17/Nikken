#include "output_pers_viewer.h"
#include <QMouseEvent>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <memory>
#include <QDebug>
#include "unified_settings.h"
#include "output_pers_widget.h"

namespace post3dapp{
using namespace qglviewer;
using namespace std;

OutputPersViewer::OutputPersViewer( OutputPersScene* s, QWidget* parent, const QGLWidget* shareWidget)
  : QGLViewer(parent, shareWidget), myScene(s)
{
    camera()->setUpVector(Vec(-1.,1.,1.));
    camera()->setViewDirection(Vec(-1.,1.,-1.));
    camera()->fitBoundingBox(Vec(0.0,0.0,0.0),Vec(50.0,50.0,50.0));
    camera()->setType(Camera::ORTHOGRAPHIC);

    camera()->fitBoundingBox(Vec(-5.0f,-5.0f,-5.0f),Vec(15.0f,15.0f,15.0f));

    camera()->setZNearCoefficient(-1000.);
    camera()->setZClippingCoefficient(1000.);

    qDebug()<<"myOutputPersViewer created";
}

void OutputPersViewer::loadViewSettings(const Analysis3DViewSettings &settings )
{
//    myViewSettings=settings;
//    setBackgroundColor( myViewSettings.backColor );
//    myScene->setLoadCase( myViewSettings.loadCase,myViewSettings.coefload,myViewSettings.loadStep );
//    myScene->setDispOnOff( myViewSettings.selectDisp );
//    myScene->setDispRatio( myViewSettings.drawRatioXYZ );
//    myScene->setFloorFrameLimited( myViewSettings.limitedFloorID, myViewSettings.limitedFloors,
//                                   myViewSettings.limitedFrameID, myViewSettings.limitedFrames );
//    myScene->setSectionLimited(myViewSettings.limitedSection);
//    myScene->setForceType(myViewSettings.selectForce);
//    myScene->setValueCalcMethod(myViewSettings.valueCalcMethod);
//    myScene->setNodeOrFace(myViewSettings.nodeOrFace);
//    myScene->setColorContour(legendMin,legendMax);
//    myScene->setDrawPoint(myViewSettings.drawPoint);
//    myScene->setDrawZWall(myViewSettings.drawZWall);
//    myScene->setDrawOpening(myViewSettings.drawOpening);

    qDebug()<<"setting done";
//    update();
//    glFlush();
}

void OutputPersViewer::init()
{
Analysis3DViewSettings  myViewSettings=UnifiedSettings::getInstance()->getOutput3DViewSettings();
    setBackgroundColor( myViewSettings.getBackGroundColor() );
//    setBackgroundColor( myViewSettings.getBackGroundColor() );
//    setBackgroundColor( Qt::lightGray );

    //  アルファブレンドの係数を設定する
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    //glEnable(GL_CULL_FACE);

    if(myViewSettings.getHasShade()){
        glEnable(GL_LIGHTING);//陰影処理
    }
    else{
        glDisable(GL_LIGHTING);
    }

    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(myViewSettings.getUseSmoothing()){
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
        glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
        glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    }
    else{
        glDisable(GL_POINT_SMOOTH);
        glDisable(GL_LINE_SMOOTH);
        glDisable(GL_POLYGON_SMOOTH);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
        glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
        glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
    }
    if(myViewSettings.getIsOrthoGraphic()){
        camera()->setType(Camera::ORTHOGRAPHIC);
        camera()->setZNearCoefficient(-1000.);
        camera()->setZClippingCoefficient(1000.);
    }else{
        camera()->setType(Camera::PERSPECTIVE);
        camera()->setZNearCoefficient(0.001);
        camera()->setZClippingCoefficient(100.);
    }
}

void OutputPersViewer::draw()
{

    Analysis3DViewSettings  myViewSettings=UnifiedSettings::getInstance()->getOutput3DViewSettings();

    qreal pw=qreal(myViewSettings.getLineWidth());
//    qreal pw = 2;
    glLineWidth( pw );

    GLfloat refm[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, refm);

    glEnable(GL_BLEND);

//    DRAWINGSTYLE dstyle=static_cast<DRAWINGSTYLE>(widgetState.drawStyle);
    myScene->setWidgetState(static_cast<OutputPersWidget*>(parent())->getWidgetState());
    myScene->drawSolid(false);

    bool drawGraph = false;
    if(widgetState.drawN ||widgetState.drawM||widgetState.drawQ)drawGraph = true;
//    if(widgetState.drawYield)myScene->drawYieldMarks(dstyle);
    if(drawGraph)
        myScene->drawGraph(myViewSettings.getGraphScaleType(),myViewSettings.getGraphScale());


//    if(widgetState.drawNumber && drawGraph)myScene->drawNumberLabel(myViewSettings.getFontSize());
//    if(widgetState.drawMemberLabel)myScene->drawMemberLabel(myViewSettings.getFontSize());
//    if(widgetState.drawNumber && widgetState.drawDisp)myScene->drawDispNumber(myViewSettings.getFontSize());

//    QList<bool>drawarrow;
//    if(widgetState.drawReact){

//    myScene->drawReaction(myViewSettings.getDrawReactionDOF(),
//                          widgetState.drawNumber,
//                          myViewSettings.getReactScale(),
//                          myViewSettings.getFontSize());
//    }

//    if(myViewSettings.getDrawCoordinate())myScene->drawCoordinate();
//    if(myViewSettings.getDrawLogo())myScene->drawLogo();
    glDisable(GL_BLEND);
}

void OutputPersViewer::fastDraw()
{

//    qreal pw=qreal(myViewSettings.lineWidth);
//    glLineWidth( pw );

//    GLfloat refm[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
//    glShadeModel(GL_FLAT);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, refm);

//    glEnable(GL_BLEND);

////    glPushMatrix();
////    gluPerspective(120.0, 1.0, 1.0, 10000000000.0);
//    DRAWINGSTYLE dstyle=static_cast<DRAWINGSTYLE>(myViewSettings.drawStyle);
//    myScene->drawSolid(dstyle,true);
////    if(myViewSettings.drawYield==true)myScene->drawYieldMarks(dstyle);
//    if(myViewSettings.drawGraph==true)myScene->drawGraph(myViewSettings.selectGraphScale,myViewSettings.graphScale);
////    if(myViewSettings.drawNumberLabel==true)myScene->drawNumberLabel();

//    QList<bool>drawarrow;
//    switch (myViewSettings.selectReact) {
//    case 0:
//        drawarrow<<false<<false<<false<<false<<false<<false;
//        break;
//    case 1:
//        drawarrow<<true<<false<<false<<false<<false<<false;
//        break;
//    case 2:
//        drawarrow<<false<<true<<false<<false<<false<<false;
//        break;
//    case 3:
//        drawarrow<<false<<false<<true<<false<<false<<false;
//        break;
//    case 4:
//        drawarrow<<true<<true<<true<<false<<false<<false;
//        break;
//    case 5:
//        drawarrow<<false<<false<<false<<true<<false<<false;
//        break;
//    case 6:
//        drawarrow<<false<<false<<false<<false<<true<<false;
//        break;
//    case 7:
//        drawarrow<<false<<false<<false<<false<<false<<true;
//        break;
//    case 8:
//        drawarrow<<false<<false<<false<<true<<true<<true;
//        break;
//    default:
//        drawarrow<<false<<false<<false<<false<<false<<false;
//        break;
//    }
//    myScene->drawReaction(drawarrow,myViewSettings.drawReactNumber,myViewSettings.reactScale,myViewSettings.fontSize);
//    if(myViewSettings.drawCoordinate)myScene->drawCoordinate();
//    if(myViewSettings.drawLogo)myScene->drawLogo();
////    glPopMatrix();
//    glDisable(GL_BLEND);

}
void OutputPersViewer::fitBoundingBox(XYZ min,XYZ max)
{
    camera()->fitBoundingBox(Vec(min.x,min.y,min.z),Vec(max.x,max.y,max.z));
}
void OutputPersViewer::keyPressEvent(QKeyEvent* event){

//    QGLViewer::keyPressEvent(event);
//     if(event->type() == QKeyEvent::KeyPress) {
//        if(event->matches(QKeySequence::Copy)) {

//            QImage image=this->grabFrameBuffer(false);
//            QClipboard * clipboad = QApplication::clipboard();
//            clipboad->setImage( image );
////            clipboad->setPixmap( pixmap );
//        }
//     }
}

void OutputPersViewer::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter;
    painter.begin((QGLViewer*)this);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();


    init();
    preDraw();
    draw();
    postDraw();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();

//    if(myScene->getDrawLegend())drawLegend(&painter);
//    if(myScene->getDrawFileName())drawFileName(&painter);
    painter.end();

}
void OutputPersViewer::drawFileName(QPainter* painter)
{
    int xPos=30;
    int yPos=30;
//    int height=70;
    int height=40;
    int width=500;
//    QString title=myViewSettings.title;
//    QString filename=myViewSettings.filename;
    painter->save();
//    painter->fillRect(0,0,width,height,Qt::white);
    painter->setPen(QPen(Qt::darkGray,1,Qt::SolidLine));
//    painter->drawText(xPos,yPos,QString::fromLocal8Bit("ファイル名:")+filename);
//    painter->drawText(xPos,yPos+20,QString::fromLocal8Bit("タイトル:")+title);
    painter->restore();

}

void OutputPersViewer::drawLegend(QPainter* painter)
{
//    painter->save();
//    painter->setPen(QPen(Qt::darkGray,1,Qt::SolidLine));

//    int legendwidth=210;
//    int legendheight=height();
//    int xPos=width()-legendwidth;
//    int yPos=0;
//    int rectnum=10;
//    int topmargin=20;
//    int leftmargin=10;
//    int rectwidth=50;
//    int rectheight=20;
//    int fontsize=10;
//    int textYPos=0;
//    ColorPicker* colorpicker=myScene->getColorPicker();
//    QFont originfont=painter->font();
////    QFont serifFont("Meiryo UI", fontsize);
////    QFont fixedPitchFont("Courier", fontsize);
//    QFont fixedPitchFont("ＭＳ ゴシック", fontsize);

//    QFont serifFont("Meiryo UI", fontsize);
//    fixedPitchFont.setFixedPitch(true);
//    qreal drawratio=myViewSettings.drawRatioXYZ;
//    painter->setFont(fixedPitchFont);
////    if(!myViewSettings.loadCase.at(1).isEmpty())legendheight+=3.0*fontsize;
////    if(!myViewSettings.loadCase.at(2).isEmpty())legendheight+=3.0*fontsize;
////    if(!myViewSettings.loadCase.at(3).isEmpty())legendheight+=3.0*fontsize;
//    painter->fillRect(xPos,yPos,legendwidth,legendheight,Qt::white);//四角の高さ、文字の表示位置を数えていく
//    qreal minValue=colorpicker->getMin();
//    qreal maxValue=colorpicker->getMax();

//    //柱、梁、ブレースの応力選択内容
//    QList<bool> forceC=myViewSettings.selectForce["C"];
//    QList<bool> forceG=myViewSettings.selectForce["G"];
//    QList<bool> forceB=myViewSettings.selectForce["B"];
//    QList<bool> forceW=myViewSettings.selectForce["W"];
//    bool haveForce=true;
//    bool haveMultiForce=false;//複数応力選択があるか判定
//    int forceCnum=0,forceGnum=0,forceBnum=0,forceWnum=0;
//    QString strForceC;
//    QString strForceG;
//    QString strForceB;
//    QString strForceW;
//    QStringList strforce,strforceW;
//    strforce<<QString::fromLocal8Bit(" N")<<QString::fromLocal8Bit(" Qy")<<QString::fromLocal8Bit(" Qz")<<QString::fromLocal8Bit(" Mx")<<QString::fromLocal8Bit(" My")<<QString::fromLocal8Bit(" Mz");
//    strforceW<<QString::fromLocal8Bit(" N")<<QString::fromLocal8Bit(" Q")<<QString::fromLocal8Bit(" M");
//    for(int i=0;i<6;i++){
//        if(forceC.at(i)==true){forceCnum++;strForceC=strForceC+strforce.at(i);}
//        if(forceG.at(i)==true){forceGnum++;strForceG=strForceG+strforce.at(i);}
//        if(forceB.at(i)==true){forceBnum++;strForceB=strForceB+strforce.at(i);}
//    }
//    for(int i=0;i<3;i++){
//        if(forceW.at(i)==true){forceWnum++;strForceW=strForceW+strforceW.at(i);}
//    }
//    if(forceCnum==0 && forceGnum==0 && forceBnum==0 && forceWnum==0)haveForce=false;
//    if(forceCnum>=2 || forceGnum>=2 || forceBnum>=2 || forceWnum>=2)haveMultiForce=true;

//    if(haveForce && myViewSettings.drawContour==true){
//        for(int i=1;i<=rectnum;i++){
//            qreal displayvalue=minValue+(maxValue-minValue)/rectnum*(rectnum-i+1);
//            qreal colorvalue=minValue+(maxValue-minValue)/rectnum*(rectnum-i+0.5);
//            QColor color=colorpicker->getColor(colorvalue);
//            painter->fillRect(xPos+leftmargin,yPos+topmargin+(i-1)*rectheight,
//                              rectwidth,rectheight,color);
//            painter->drawRect(xPos+leftmargin,yPos+topmargin+(i-1)*rectheight,
//                              rectwidth,rectheight);
////            QString strdispvalue=QString::number(displayvalue,'e');
//            QString strdispvalue=moldDoubleToText(displayvalue,11,2);
//            painter->drawText(xPos+leftmargin+rectwidth+10,yPos+topmargin+(i-1)*rectheight+0.5*fontsize,
//                              strdispvalue);
//        }
//        QString strdispvalue=moldDoubleToText(minValue,11,2);
////        if(minValue>=0)strdispvalue=' '+strdispvalue;
//        painter->drawText(xPos+leftmargin+rectwidth+10,yPos+topmargin+rectnum*rectheight+0.5*fontsize,
//                          strdispvalue);

//        textYPos+=topmargin+rectnum*rectheight+3.0*fontsize;
//        painter->setFont(serifFont);
//        painter->drawText(xPos+leftmargin,textYPos,QString::fromLocal8Bit("単位:kN,kN-m"));
//    }
//    textYPos+=3.0*fontsize;

//    //node or face
//    if(myViewSettings.nodeOrFace==0){painter->drawText(xPos+leftmargin,textYPos,QString::fromLocal8Bit("節点応力"));textYPos+=3.0*fontsize;}
//    if(myViewSettings.nodeOrFace==1){painter->drawText(xPos+leftmargin,textYPos,QString::fromLocal8Bit("フェイス応力"));textYPos+=3.0*fontsize;}

//    QString strmethod1=QString::fromLocal8Bit("複数応力選択時の扱い：");
//    QString strmethod2;
//    if(myViewSettings.valueCalcMethod==0){strmethod2=QString::fromLocal8Bit("最大値");}
//    else if(myViewSettings.valueCalcMethod==1){strmethod2=QString::fromLocal8Bit("最小値");}
//    else if(myViewSettings.valueCalcMethod==2){strmethod2=QString::fromLocal8Bit("絶対値最大");}

//    if(myViewSettings.drawContour==true && haveMultiForce){painter->drawText(xPos+leftmargin,textYPos,strmethod1);textYPos+=3.0*fontsize;}
//    if(myViewSettings.drawContour==true && haveMultiForce){painter->drawText(xPos+leftmargin,textYPos,strmethod2);textYPos+=3.0*fontsize;}
//    painter->setFont(fixedPitchFont);
//    if(forceCnum!=0){painter->drawText(xPos+leftmargin,textYPos,QString::fromLocal8Bit("Col."));painter->drawText(xPos+leftmargin+27,textYPos,strForceC);textYPos+=3.0*fontsize;}
//    if(forceGnum!=0){painter->drawText(xPos+leftmargin,textYPos,QString::fromLocal8Bit("Gir."));painter->drawText(xPos+leftmargin+27,textYPos,strForceG);textYPos+=3.0*fontsize;}
//    if(forceBnum!=0){painter->drawText(xPos+leftmargin,textYPos,QString::fromLocal8Bit("Bra."));painter->drawText(xPos+leftmargin+27,textYPos,strForceB);textYPos+=3.0*fontsize;}
//    if(forceWnum!=0){painter->drawText(xPos+leftmargin,textYPos,QString::fromLocal8Bit("Wal."));painter->drawText(xPos+leftmargin+27,textYPos,strForceW);textYPos+=3.0*fontsize;}
//    if(myViewSettings.selectDisp==0){painter->drawText(xPos+leftmargin,textYPos,QString::fromLocal8Bit("変形倍率:×")+QString::number(drawratio,'f',2));textYPos+=3.0*fontsize;}

//    bool loadcaseselected=false;
//    for(int loadcase=0;loadcase<myViewSettings.loadCase.count();loadcase++){if(!myViewSettings.loadCase.at(loadcase).isEmpty())loadcaseselected=true;}
////    painter->setFont(serifFont);
//    if(loadcaseselected)painter->drawText(xPos+leftmargin,textYPos,QString::fromLocal8Bit("係数:荷重ケース：ステップ"));
////    painter->setFont(fixedPitchFont);
//    textYPos+=3.0*fontsize;
//    for(int loadcase=0;loadcase<myViewSettings.loadCase.count();loadcase++){
//        if(!myViewSettings.loadCase.at(loadcase).isEmpty()){
//            painter->drawText(xPos+leftmargin,textYPos,
//                              QString::number(myViewSettings.coefload.at(loadcase),'f',2));
//            painter->drawText(xPos+leftmargin+40,textYPos,
//                              QString::fromLocal8Bit("×")+myViewSettings.loadCase.at(loadcase));
//            painter->drawText(xPos+leftmargin+120,textYPos,
//                              QString::number(myViewSettings.loadStep.at(loadcase))+"/"+QString::number(myViewSettings.loadMaxStep.at(loadcase)));
//            textYPos+=3.0*fontsize;
//        }
//    }
//    painter->setFont(originfont);
//    painter->restore();
}
QString OutputPersViewer::moldDoubleToText(qreal value,int length,int precision){
    QString moldedValue;
    QString str=QString::number(value,'f',precision);
    bool isMinus=str.startsWith("-");
    if(isMinus){
        moldedValue="-";
        str.remove("-");
    }else{
        moldedValue="+";
    }
    if(str.length()>length){
        QString estr=QString::number(value,'e',qMax(1,length-4-2));
        if(value>=0.0)estr="+"+estr;
        return estr;
    }
    str=str.rightJustified(length,' ',false);
    moldedValue=moldedValue+str;
    return moldedValue;
}
}
