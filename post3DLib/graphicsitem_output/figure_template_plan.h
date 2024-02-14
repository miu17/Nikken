#ifndef FIGURETEMPLATEPLAN_H
#define FIGURETEMPLATEPLAN_H

#include "figure_template.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{


class POST3D_CLASS_DLLSPEC FigureCommonTemplatePlan:public FigureCommonTemplate
{
public:
    FigureCommonTemplatePlan(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY& type, QUuid uuid);
protected:
    virtual void preCreateItems(OutputGrScene *)override {qDebug()<<"called FigureTemplatePlan::preCreateItems";};
    void createJointItems(OutputGrScene*)final;
    void createMemberItems(OutputGrScene*)final;
};
}

#endif // FIGURETEMPLATEPLAN_H
