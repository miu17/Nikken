#ifndef DRAWOUTPUTFIGURE_H
#define DRAWOUTPUTFIGURE_H

#include "figureinterface.h"
#include "define_draw_figure.h"
#include "define_settings.h"

#ifdef CALCULATIONMAKER_LIB
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace CalculationMakerLib{

class CALCULATIONMAKER_CLASS_DLLSPEC DrawOutputFigure : public FigureInterface
{
public:
    DrawOutputFigure(const QString &command, post3dapp::FIGURE_TYPE_PROPERTY _typeOption, post3dapp::FIGURE_PROPERTY _sizeOption,post3dapp::Analysis2DViewSettings _view)
        : FigureInterface(command), typeOption(_typeOption), sizeOption(_sizeOption), viewSetting(_view){}
    void draw(QPainter*, const QRect&, const QString&, post3dapp::PAPERDIVIDE, int) override;
    bool isPlan() override;
private:
    post3dapp::FIGURE_TYPE_PROPERTY typeOption;
    post3dapp::FIGURE_PROPERTY sizeOption;
    const post3dapp::Analysis2DViewSettings viewSetting;
};
} // namespace CalculationMakerLib

#endif // DRAWOUTPUTFIGURE_H
