#ifndef AXIALPLANTEMPLATE_H
#define AXIALPLANTEMPLATE_H
#include <QtMath>
#include "figure_template_plan.h"
#include "output_graphicsitem.h"
namespace post3dapp{

class OutputGrScene;
class AxialPlanTemplate :public FigureCommonTemplatePlan
{
    Q_OBJECT

public:
    AxialPlanTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    void createLineItem(OutputGrScene*, MemberData *, QList<QPointF>)override;
//    QColor figureColor(DATATYPE,STRUCTTYPE,ATTENTION_STATUS stat)override{
//        qDebug()<<"axial figure color called";
//        switch(stat){
//        case ATTENTION_STATUS::STATUS_NORMAL:
//            return viewSetting.axialMinusColor;
//            break;
//        case ATTENTION_STATUS::STATUS_WARNING:
//            return viewSetting.axialPlusColor;
//            break;
//        default:
//            break;
//        }
//        return QColor(Qt::black);
//    }
    qreal getDiagramLoadPerMeter(){return  qMax(10.0, getViewSettings()->getAxialScale());}
//    bool valueVisible(DATATYPE)override{return viewSetting->valueVisible;}
//protected:
    AnalysisAxialForce2DViewSetting* getViewSettings() const override{return static_cast<AnalysisAxialForce2DViewSetting*>(setting.getSetting(option.type));}
private:
//    qreal diagramLoadPerMeter;
};
} // namespace post3dapp

#endif // AXIALELEVATIONTEMPLATE_H
