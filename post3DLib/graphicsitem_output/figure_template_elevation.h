#ifndef FIGURETEMPLATEELEVATION_H
#define FIGURETEMPLATEELEVATION_H

#include "figure_template.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{




class POST3D_CLASS_DLLSPEC FigureCommonTemplateElevation:public FigureCommonTemplate
{
public:
    FigureCommonTemplateElevation(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY& type, QUuid uuid);
protected:
    void createJointItems(OutputGrScene*)final;
    void createMemberItems(OutputGrScene*)final;
};
}
#endif // FIGURETEMPLATEELEVATION_H
