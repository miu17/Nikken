#ifndef DRAW_FIGURE_H
#define DRAW_FIGURE_H

#include <QObject>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

class QSvgGenerator;

namespace post3dapp{
class Analysis2DViewSettings;
class CalcLoadGrScene;
class CalculationLoad2DViewSettings;
class CustomDrawGrScene;
class FIGURE_PROPERTY;
class FIGURE_SIZE_POLICY;
class FIGURE_SIZE_RESULT;
class Input2DViewSettings;
class InputGrScene;
class OutputGrScene;
class UnifiedInputData;
struct FIGURE_TYPE_PROPERTY;

class POST3D_CLASS_DLLSPEC DrawFigure : public QObject
{
public:
    DrawFigure();
    static const qreal inchToMm;
    static void drawForCalcPaper(QPainter *painter, const QRect& rect,  CustomDrawGrScene* scene,
                                                    const int tate,const int yoko, const FIGURE_SIZE_RESULT& size, const FIGURE_PROPERTY& prop);
    static void drawForOutputFigure(QPainter* painter, CustomDrawGrScene* scene, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &prop);

    static QPair<FIGURE_PROPERTY, FIGURE_SIZE_RESULT> drawForPrintDialog(QPainter* painter, CustomDrawGrScene* scene, const FIGURE_SIZE_POLICY& policy);
/*
    static void drawForSvgFile(BASEVIEWSETTINGS* setting, UnifiedData *data,
                                                    const FIGURE_SIZE_POLICY& policy, const FIGURE_TYPE_PROPERTY& type);
*/
    static OutputGrScene* createOutScene(const FIGURE_TYPE_PROPERTY &type,  const Analysis2DViewSettings& setting, QObject *parent = nullptr);
    static CalcLoadGrScene* createLoadScene(const FIGURE_TYPE_PROPERTY &type,  const CalculationLoad2DViewSettings& setting, QObject *parent = nullptr);
    static InputGrScene* createInputScene(const FIGURE_TYPE_PROPERTY &type,  const Input2DViewSettings& setting, QObject *parent = nullptr);
    static InputGrScene* createCopyScene(UnifiedInputData* data, const FIGURE_TYPE_PROPERTY &type,  const Input2DViewSettings& setting, QObject *parent = nullptr);
    static QPair<FIGURE_PROPERTY, FIGURE_SIZE_RESULT> calcScaleAndDivision(CustomDrawGrScene *scene, const FIGURE_SIZE_POLICY& policy);
    static void drawOnePage(CustomDrawGrScene *scene, QPainter *painter, const  int tate, const  int yoko, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &prop);

//    static QPair<int, int> countPaperPlan(QRectF boundingRect, QSize unitPaperSize,qreal scale) ;

//    static QRect clipRect(FIGURE_SIZE_RESULT size, int);
    static QRect viewRect(const FIGURE_PROPERTY& size,const int);
    static QSvgGenerator *makeSvgFile(const QString filename,const FIGURE_PROPERTY& size);
private:
    static void setSourceRect(CustomDrawGrScene*, FIGURE_PROPERTY*, const FIGURE_SIZE_POLICY&);
};
}
#endif // DRAW_FIGURE_H
