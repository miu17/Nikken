#ifndef REACTIONMAPPLANTEMPLATE_H
#define REACTIONMAPPLANTEMPLATE_H

#include "numeric_plan_template.h"
#include "output_graphicsitem.h"

namespace post3dapp{

class ReactionMapPlanTemplate :public NumericPlanTemplate
{
    Q_OBJECT
public:
    ReactionMapPlanTemplate(const Analysis2DViewSettings& set, const FIGURE_TYPE_PROPERTY& prop, QUuid uuid)
        :NumericPlanTemplate(set, prop,uuid){
    }
    QColor valueTextColor(DATATYPE,ATTENTION_STATUS stat) const override{
        if(stat == ATTENTION_STATUS::STATUS_NORMAL){
            return getViewSettings()->getPlusColor();
        }else{
            return getViewSettings()->getMinusColor();
        }
    }
protected:
    AnalysisReactionForce2DViewSetting* getViewSettings() const override {return static_cast<AnalysisReactionForce2DViewSetting*>(setting.getSetting(option.type));}

};
} // namespace post3dapp
#endif // REACTIONMAPPLANTEMPLATE_H
