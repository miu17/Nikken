#include "define_graph.h"

#include "fixed_data.h"
#include "unified_output.h"
namespace post3dapp{

Dot Dot::makeDot(GRAPH_SUBTYPE_PROPERTY::NMTYPE nmtype,
                        GRAPH_SUBTYPE_PROPERTY::ICJ icj,
                        QString label,
                        QUuid memberId,
                        QList<STRESS> stress){

//    MemberData *md = UnifiedFixedData::getInstance()->getAnalysisData()->memberIDToPointer(memberId);

    //応力(N,My,Mz)を求める
    qreal N=0;
    qreal My=0;
    qreal Mz=0;
    UnifiedOutput* ud = UnifiedFixedData::getInstance()->getPstnData();


    MemberForce mf = ud->getCombinationFaceMemberForce(memberId,stress);
    MemberForceByIL mfil_i = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(memberId,stress,1,0);
    MemberForceByIL mfil_c = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(memberId,stress,2,0);
    MemberForceByIL mfil_j = UnifiedFixedData::getInstance()->getPstnData()->getAnyPointMemberForce(memberId,stress,3,0);
    qDebug()<<"makeDot 8";
    if(icj == GRAPH_SUBTYPE_PROPERTY::I){
        My =  -mf.getMy(0)-mfil_i.getMy();
        Mz =  -mf.getMz(0)-mfil_i.getMz();
        N =  mf.N + mfil_i.getN();
    }else if(icj == GRAPH_SUBTYPE_PROPERTY::C){
        My =  -mf.getMy(0.5)-mfil_c.getMy();
        Mz =  -mf.getMz(0.5)-mfil_c.getMz();
        N =  mf.N + mfil_i.getN();
    }else if(icj == GRAPH_SUBTYPE_PROPERTY::J){
        My =  mf.getMy(1.0)+mfil_j.getMy();
        Mz =  mf.getMz(1.0)+mfil_j.getMz();
        N =  mf.N + mfil_i.getN();
    }
    qDebug()<<"makeDot 9";

    //対応するDotを作って返す
    DotType dottype=DotType::BlackRectangle;
    QPointF point=QPointF(0,0);
    if(nmtype == GRAPH_SUBTYPE_PROPERTY::NMy){
        point = QPointF(My,N);
    }else if(nmtype == GRAPH_SUBTYPE_PROPERTY::NMz){
        point = QPointF(Mz,N);
    }else if(nmtype == GRAPH_SUBTYPE_PROPERTY::MyMz){
        point = QPointF(My,Mz);
    }
    Dot newdot(point, dottype, label);
    qDebug()<<"N My Mz="<<N<<My<<Mz;

    return newdot;


}
void Dot::draw(QPainter* p,const Graph& parentGraph){
    /*
    QGraphicsTextItem *markItem = scene->addText(mark);
    markItem->setPos(QPointF(point.x() * scaleX, ymax * scaleY - point.y() * scaleY));
    markItem->setFont(font);
    */
    //座標計算がdpi依存なので、直接drawすることにした
    QPen savedPen = p->pen();
    QBrush savedBrush = p->brush();

    qreal dpi = p->device()->logicalDpiX();
    QPointF orig = QPointF(parentGraph.xPos(point.x(),dpi),
                           parentGraph.yPos(point.y(),dpi));
    double size = 4.0+symbolSize;
    QColor color = symbolColor;
    qDebug()<<"Dot::size,color"<<size<<color;

    QPointF offset(0.6*size,0);
    if (markType == DotType::None){

    }else if (markType == DotType::Triangle || markType == DotType::BlackTriangle){
        double h = qPow(3, 0.5) / 3.0 * size;
        QPointF p1=orig+QPointF(0, -h);
        QPointF p2=orig+QPointF(-size/2.0, h * 0.5);
        QPointF p3=orig+QPointF(size/2.0, h * 0.5);
        QVector<QPointF> v{p1, p2, p3};
        QPolygonF triangle(v);
        if (markType == DotType::Triangle){
            p->setBrush(Qt::NoBrush);
            p->setPen(QPen(symbolColor,0));
            p->drawPolygon(triangle);
        }else{
            p->setBrush(symbolColor);
            p->setPen(QPen(symbolColor,0));
            p->drawPolygon(triangle);
        }
    }else if (markType == DotType::Rectangle){
        p->setBrush(Qt::NoBrush);
        p->setPen(QPen(symbolColor,0));
        p->drawRect(QRectF(orig.x()-size/2.0, orig.y()-size/2.0, size, size));
    }else if (markType == DotType::BlackRectangle){
        p->setBrush(symbolColor);
        p->setPen(QPen(symbolColor,0));
        p->drawRect(QRectF(orig.x()-size/2.0, orig.y()-size/2.0, size, size));
    }else if (markType == DotType::Circle){
        p->setBrush(Qt::NoBrush);
        p->setPen(QPen(symbolColor,0));
        p->drawEllipse(orig,size/2.0,size/2.0);
    }else if (markType == DotType::BlackCircle){
        p->setBrush(symbolColor);
        p->setPen(QPen(symbolColor,0));
        p->drawEllipse(orig,size/2.0,size/2.0);
    }
    p->setBrush(savedBrush);
    p->setPen(savedPen);

    p->setFont(font);
    p->drawText(orig+offset,label);

}
void Series::draw(QPainter* p,const Graph& parentGraph){
    if(plotPoints.count()==0 && dots.count()==0)return;

    qreal dpi = p->device()->logicalDpiX();
    QRectF graphArea = parentGraph.graphArea(dpi);
    QRectF plotArea = parentGraph.plotArea(dpi);
    QPen origin = p->pen();
    p->setClipRect(plotArea);

    for (int i = 0; i < plotPoints.count() - 1; i++){
        QPointF p1 = QPointF(parentGraph.xPos(plotPoints.at(i).x(),dpi),
                             parentGraph.yPos(plotPoints.at(i).y(),dpi));
        QPointF p2 = QPointF(parentGraph.xPos(plotPoints.at(i+1).x(),dpi),
                             parentGraph.yPos(plotPoints.at(i+1).y(),dpi));
        p->setPen(pen);
        p->drawLine(QLineF(p1, p2));
    }
    p->setClipRect(graphArea);

    QPointF pn = QPointF(parentGraph.xPos(plotPoints.last().x(),dpi),
                         parentGraph.yPos(plotPoints.last().y(),dpi));
    p->setFont(font);
    p->drawText(pn,name);
    p->setPen(origin);

    Q_FOREACH(auto dot, dots){
        dot.draw(p,parentGraph);
    }
}


Graph::Graph(const QList<Series>& s, const GRAPH_SIZE_OPTION& sz, const QString& tit, const QString& labX, const QString& labY)
    :series(s),size(sz), title(tit), labelX(labX), labelY(labY){

    axisvaluefont = QFont("Meiryo", 8);
    axisTitleFont = QFont("Meiryo", 8);
    titlefont = QFont("Meiryo", 12);
    rulerPen = QPen(Qt::black);
    titleVisible = true;
    axisColor=Qt::black;
    if(s.count()==0){
            xmin = 0;
            xmax = 1000;
            ymin = 0;
            ymax = 1000;
            return;
    }

    double tempXMin = DBL_MAX;
    double tempYMin = DBL_MAX;
    double tempXMax = DBL_MIN;
    double tempYMax = DBL_MIN;
    bool X_strictBorderMin=false;
    bool X_strictBorderMax=false;
    bool Y_strictBorderMin=false;
    bool Y_strictBorderMax=false;
    if(sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_TOPFIX){X_strictBorderMax = true;}
    if(sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_BOTTOMFIX){X_strictBorderMin = true;}
    if(sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_FIX){X_strictBorderMin = true;X_strictBorderMax = true;}
    if(sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_TAISYOU){X_strictBorderMin = false;X_strictBorderMax = false;}
    if(sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_TOPFIX){Y_strictBorderMax = true;}
    if(sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_BOTTOMFIX){Y_strictBorderMin = true;}
    if(sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_FIX){Y_strictBorderMin = true;Y_strictBorderMax = true;}
    if(sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_TAISYOU){Y_strictBorderMin = false;Y_strictBorderMax = false;}
    //X最小
    if(sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_AUTO
            || sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_TOPFIX
            || sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_TAISYOU){
        Q_FOREACH(auto sr, series){
            Q_FOREACH(auto point, sr.plotPoints){
                tempXMin = qMin(tempXMin, point.x());
            }
        }

    }else{
        tempXMin = sz.fixed_xmin;
    }
    //X最大
    if(sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_AUTO
            || sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_BOTTOMFIX
            || sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_TAISYOU){
        Q_FOREACH(auto sr, series){
            Q_FOREACH(auto point, sr.plotPoints){
                tempXMax = qMax(tempXMax, point.x());
            }
        }

    }else{
        tempXMax = sz.fixed_xmax;
    }
    if(sz.scaleAutoX==GRAPH_SIZE_OPTION::JIKU_TAISYOU){
        tempXMin = -qMax(qAbs(tempXMax), qAbs(tempXMin));
        tempXMax = qMax(qAbs(tempXMax), qAbs(tempXMin));
    }
    //Y最小
    if(sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_AUTO
            || sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_TOPFIX
            || sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_TAISYOU){
        Q_FOREACH(auto sr, series){
            Q_FOREACH(auto point, sr.plotPoints){
                tempYMin = qMin(tempYMin, point.y());
            }
        }

    }else{
        tempYMin = sz.fixed_ymin;
    }

    //Y最大
    if(sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_AUTO
            || sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_BOTTOMFIX
            || sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_TAISYOU){
        Q_FOREACH(auto sr, series){
            Q_FOREACH(auto point, sr.plotPoints){
                tempYMax = qMax(tempYMax, point.y());
            }
        }

    }else{
        tempYMax = sz.fixed_ymax;
    }
    if(sz.scaleAutoY==GRAPH_SIZE_OPTION::JIKU_TAISYOU){
        tempYMin = -qMax(qAbs(tempYMax), qAbs(tempYMin));
        tempYMax = qMax(qAbs(tempYMax), qAbs(tempYMin));
    }

//    if(sz.scaleAuto){
//        Q_FOREACH(auto sr, series){
//            Q_FOREACH(auto point, sr.plotPoints){
//                tempXMin = qMin(tempXMin, point.x());
//                tempYMin = qMin(tempYMin, point.y());
//                tempXMax = qMax(tempXMax, point.x());
//                tempYMax = qMax(tempYMax, point.y());
//            }
//        }
//    }else{
//        tempXMin = sz.fixed_xmin;
//        tempYMin = sz.fixed_ymin;
//        tempXMax = sz.fixed_xmax;
//        tempYMax = sz.fixed_ymax;
//    }
    if(sz.logScaleX && tempXMin<=0){
        tempXMin = 1;
        tempXMax = qMax(tempXMax,tempXMin*10.0);
    }
    if(sz.logScaleY && tempYMin<=0){
        tempYMin = 1;
        tempYMax = qMax(tempYMax,tempYMin*10.0);
    }

    if(size.useStringJikuX){
        for(int i=0;i<size.stringJikuX.count();i++)subAxisX<<i;
    }else{
        subAxisX = calcSubAxis(tempXMin , tempXMax,X_strictBorderMin,X_strictBorderMax,1);
    }
    if(size.useStringJikuY){
        for(int i=0;i<size.stringJikuY.count();i++)subAxisY<<i;
    }else{
        subAxisY = calcSubAxis(tempYMin , tempYMax,Y_strictBorderMin,Y_strictBorderMax,2);
    }

    if (subAxisX.count() == 0 ||subAxisY.count() == 0){
        xmin = 0;
        xmax = 1000;
        ymin = 0;
        ymax = 1000;

    }else{
        xmin = subAxisX.first();
        xmax = subAxisX.last();
        ymin = subAxisY.first();
        ymax = subAxisY.last();
    }


}

void Graph::draw(QPainter* p){

    if(xmin >= xmax)return;
    if(ymin >= ymax)return;
    QPen savedPen = p->pen();
    QPen axisValuePen = QPen(axisColor,0);
    //10^n表示したい
//    if(!size.logScaleX){
//        if(subAxisX.last() > 100 || subAxisX.last()<0.01){
//            int keta2 = (int)(log10(subAxisX.last())+1);
//            QString ketastr = u8"10^"+QString::number(keta2-1);
//            labelX += ketastr;
//            for(int i=0;i<subAxisX.count();i++){
//                subAxisX[i] /= qPow(10,keta2-1);
//                //最大最小も変更必要あり
//            }
//        }
//    }
//    if(!size.logScaleY){
//        if(subAxisY.last() > 100 || subAxisY.last()<0.01){
//            int keta2 = (int)(log10(subAxisY.last())+1);
//            QString ketastr = u8"10^"+QString::number(keta2-1);
//            labelY += ketastr;
//            for(int i=0;i<subAxisY.count();i++){
//                subAxisY[i] /= qPow(10,keta2-1);
//            }
//        }
//    }

    p->setPen(rulerPen);
    QFont temp(axisvaluefont.family(), axisvaluefont.pointSize() / p->transform().m11());
    qDebug() << p->transform().m11();
    //p->setFont(axisvaluefont);
    p->setFont(temp);

    QFontMetrics fm(p->font());
    qreal dpi = p->device()->logicalDpiX();
    int keta = 0;
    if (subAxisY.count() > 2){
        double log = log10(subAxisY.at(1) - subAxisY.at(0));
        if (log < 1){
            keta = qAbs(log) + 1;
        }
    }
    QRectF graphArea = size.graphArea(dpi);
    QRectF plotArea = size.plotArea(dpi);
    p->setClipRect(graphArea);


    for(int i = 0; i < subAxisY.count(); i++){
        QPointF p1 = QPointF(xPos(xmin,dpi), yPos(subAxisY.at(i),dpi));
        QPointF p2 = QPointF(xPos(xmax,dpi), yPos(subAxisY.at(i),dpi));
        QString str = size.useStringJikuY ? size.stringJikuY.at(i) : QString::number(subAxisY.at(i), 'f', keta);
        int strw = fm.width(str);
        int strh = fm.height();
        QPointF p3 = p1 + QPointF(-strw,0.33*strh);
        if(size.logScaleY){
            //対数軸の時は10^nの時だけ数字プロット、それ以外は破線
            const qreal eps = 0.000001;
            if(i==subAxisY.count()-1){
                p->setPen(axisValuePen);
                p->drawText(p3,str);
                p->setPen(rulerPen);
                p->drawLine(QLineF(p1, p2));
            }else if(qAbs(subAxisY.at(i+1) / subAxisY.at(i))+eps > 2.0){
                p->setPen(axisValuePen);
                p->drawText(p3,str);
                p->setPen(rulerPen);
                p->drawLine(QLineF(p1, p2));
            }else{
                p->setPen(Qt::DashLine);
                p->drawLine(QLineF(p1, p2));
                p->setPen(Qt::SolidLine);
            }
        }else{
            p->setPen(axisValuePen);
            if(!size.useStringJikuY && subAxisY.count()>=10){
                if(i%2 == 0)p->drawText(p3,str);
            }else{
                p->drawText(p3,str);
            }
            p->setPen(rulerPen);
//            QString copy(str);
//            copy.remove(QChar('0'));
//            copy.remove(QChar('-'));
//            copy.remove(QChar('.'));
//            if(copy.length()<2 ||copy.midRef(1,1)!="5"){
//                p->drawText(p3,str);
//            }
            p->drawLine(QLineF(p1, p2));
        }
    }

    keta = 0;
    if (subAxisX.count() > 2){
        double log = log10(subAxisX.at(1) - subAxisX.at(0));
        if (log < 1){
            keta = qAbs(log) + 1;
        }
    }
    for(int i = 0; i < subAxisX.count(); i++){
        QPointF p1 = QPointF(xPos(subAxisX.at(i),dpi), yPos(ymin,dpi));
        QPointF p2 = QPointF(xPos(subAxisX.at(i),dpi), yPos(ymax,dpi));

        QString str = size.useStringJikuX ? size.stringJikuX.at(i) :  QString::number(subAxisX.at(i), 'f', keta);
        int strw = fm.width(str);
        int strh = fm.height();
        QPointF p3 = p1 + QPointF(-0.5*strw,strh);
//        p->drawText(p3,str);
//        p->drawLine(QLineF(p1, p2));
        if(size.logScaleX){
            const qreal eps = 0.000001;
            if(i==subAxisX.count()-1){
                p->setPen(axisValuePen);
                p->drawText(p3,str);
                p->setPen(rulerPen);
                p->drawLine(QLineF(p1, p2));
            }else if(qAbs(subAxisX.at(i+1) / subAxisX.at(i))+eps > 2.0){
                p->setPen(axisValuePen);
                p->drawText(p3,str);
                p->setPen(rulerPen);
                p->drawLine(QLineF(p1, p2));
            }else{
                p->setPen(Qt::DashLine);
                p->drawLine(QLineF(p1, p2));
                p->setPen(Qt::SolidLine);
            }
        }else{
            p->setPen(axisValuePen);
            if(!size.useStringJikuX && subAxisX.count()>=10){
                if(i%2 == 0)p->drawText(p3,str);
            }else{
                p->drawText(p3,str);
            }
            p->setPen(rulerPen);
//            QString copy(str);
//            copy.remove(QChar('0'));
//            copy.remove(QChar('-'));
//            copy.remove(QChar('.'));
//            if(copy.length()<2 || copy.midRef(1,1)!="5"){
//                p->drawText(p3,str);
//            }
            p->drawLine(QLineF(p1, p2));
        }

    }


    p->setPen(savedPen);
    Q_FOREACH(auto s, series){
        s.draw(p,*this);
    }

//    QRectF graphArea = size.graphArea(dpi);
    p->setPen(savedPen);
    p->drawRect(plotArea);
    p->drawRect(graphArea);

//    QRectF plotArea = size.plotArea(dpi);
    p->setFont(axisTitleFont);
    QFontMetrics fma(p->font());

    int labelXWidth = fma.width(labelX);
    int labelXHeight = fma.height();
    int labelYWidth = fma.width(labelY);
    int labelYHeight = fma.height();

    p->drawText(QPointF(plotArea.x()+ 0.5*plotArea.width()  - 0.5*labelXWidth,
                        graphArea.bottom()- 0.33*labelXHeight),labelX);

    QTransform trans = p->transform();
    p->translate(graphArea.x() + labelYHeight,
                 plotArea.y() + 0.5 * plotArea.height() + 0.5 * labelYWidth);
    p->rotate(-90);
    p->drawText(0,0, labelY);
    p->setTransform(trans);

    if(titleVisible){
        p->setFont(titlefont);
        QFontMetrics fpm(p->font());
        int titleWidth = fpm.width(title);
        int titleHeight = fpm.height();
        p->drawText(QPointF(graphArea.x()+ 0.5*graphArea.width()  - 0.5*titleWidth,
                            graphArea.y()+titleHeight ),title);
    }

}

QList<qreal> Graph::calcSubAxis(qreal ymin , qreal ymax,bool strictBorderMin,bool strictBorderMax,int direction){
    //上下界それぞれ、固定値とするか、それを包含する切りのいい数値とするかでフラグ
    QList<qreal> axis;
    if(strictBorderMin)axis<<ymin;

    if((direction == 1 && size.logScaleX) ||
            (direction == 2 && size.logScaleY)){
        int yketa = int(log10(ymin)) ;
        qreal val = qPow(10,yketa-1);
        qreal add = qPow(10,yketa-1);//log10 0が未定義
        int count = 0;

        while(val <= 2.0*ymax){
            qDebug()<<"culcSubAxis"<<ymin<<yketa<<val<<add<<ymax;

            if(strictBorderMin){
                if(ymin < val){
                    axis << val;
                }
            }else{
                //一個前から入れたい。
                if(val < ymin && axis.count()>=1)axis.pop_back();
                axis << val;
            }
            if(strictBorderMax){
                if(val > ymax){
                    axis.pop_back();
                    break;
                }
            }else{
                if(val > ymax)break;
            }
            val += add;
            count ++;
            if(count % 9 == 0)add *= 10;
        }
    }
    else {

        int yketa = int(log10(ymax-ymin) - 0.2); // 10^0.2 = 1.585
        qreal start = int(ymin/(qPow(10,yketa)) -1) * qPow(10,yketa);
        //startはymin以下でqPow(10,yketa)の倍数
        for (int i = 0; i <= 100; i++) {
            if (log10(ymax-ymin) - 0.2 - yketa > 0.5 && qAbs(i % 2) == 1)continue;
                qreal val = start + 0.5 * i * qPow(10, yketa);

            if(strictBorderMin){
                if(ymin < val){
                    axis << val;
                }
            }else{
                //一個前から入れたい
                if(val < ymin && axis.count()>=1)axis.pop_back();
                axis << val;
            }
            if(strictBorderMax){
                if(val > ymax){
                    axis.pop_back();
                    break;
                }
            }else{
                if(val > ymax)break;
            }
        }
        //yminが0に十分近いのに、0未満の軸があるときは、pop
        if(!strictBorderMin && qAbs(ymin/ymax)<0.05 && axis.at(0) < -0.01 )axis.pop_front();


    }
    if(strictBorderMax)axis<<ymax;

    qDebug()<<"axx"<<axis;

    return axis;
}


} // namespace post3dapp
