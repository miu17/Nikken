#ifndef FIGURE_TEMPLATE_FACTORY_H
#define FIGURE_TEMPLATE_FACTORY_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class Analysis2DViewSettings;
class FigureTemplate;
struct FIGURE_TYPE_PROPERTY;

class POST3D_CLASS_DLLSPEC FigureTemplateFactory
{
public:
    static FigureTemplate* CreateTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    static FigureTemplate* CreatePlanTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    static FigureTemplate* CreateElevTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);

};
} // namespace post3dapp

#endif // FIGURE_TEMPLATE_FACTORY_H
