#ifndef FIGUREMODELTEMPLATE_H
#define FIGUREMODELTEMPLATE_H

#include "figure_template.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class POST3D_CLASS_DLLSPEC FigureModelTemplate : public FigureTemplate
{
    Q_OBJECT
public:
    void createSpringItem(OutputGrScene* scene, SpringData* sd, QList<QPointF> points) override{}

    QColor valueTextColor(DATATYPE,ATTENTION_STATUS) const final;
    QColor figureColor(DATATYPE, STRUCTTYPE material, ATTENTION_STATUS) const final;
    QColor selectColor(DATATYPE) const final;
    bool isLineDirVisible() const final;
    int nameTextSize(DATATYPE) const final;
    int valueTextSize(DATATYPE) const final;
    bool nameVisible(DATATYPE) const final;
    bool figureVisible(DATATYPE) const final;
    bool valueVisible(DATATYPE) const final;

    QColor scaleColor() const final;
    QColor supportColor() const final;
    int jointSize() const final;
    QColor noteColor() const final;
    int noteSize() const final;
    QColor plasticSymbolColor() const final{throw;}
    int plasticSymbolSize() const final{throw;}
    int scaleTextSize() const final;
    bool scaleFloorTextVisible() const final;
    bool scaleGridTextVisible() const final;

protected:
    FigureModelTemplate(const Analysis2DViewSettings&, const FIGURE_TYPE_PROPERTY&, QUuid);
    AnalysisModel2DViewSetting* getViewSettings() const override{return static_cast<AnalysisModel2DViewSetting*>(setting.getSetting(option.type));}
};
} // namespace post3dapp
#endif // FIGUREMODELTEMPLATE_H
