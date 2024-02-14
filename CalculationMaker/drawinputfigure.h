#ifndef DRAWINPUTFIGURE_H
#define DRAWINPUTFIGURE_H

#include "figureinterface.h"
#include "define_settings.h"

#ifdef CALCULATIONMAKER_LIB
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define CALCULATIONMAKER_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace CalculationMakerLib{

class CALCULATIONMAKER_CLASS_DLLSPEC DrawInputFigure : public FigureInterface
{
public:
    DrawInputFigure(const QString &command, bool _plan, const post3dapp::FIGURE_PROPERTY& _option, const post3dapp::Input2DViewSettings& _view)
        : FigureInterface(command), plan(_plan), option(_option), viewSetting(_view){};
    void draw(QPainter*, const QRect&, const QString&, post3dapp::PAPERDIVIDE, int) override;
    bool isPlan() override{return plan;};
private:
    const bool plan;
    post3dapp::FIGURE_PROPERTY option;
    const post3dapp::Input2DViewSettings viewSetting;

};
} // namespace CalculationMakerLib

#endif // DRAWINPUTFIGURE_H
