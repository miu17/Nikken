#ifndef CUSTOMDRAWGRSCENE_H
#define CUSTOMDRAWGRSCENE_H

#include <QGraphicsScene>


#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class BaseBackGround;
class FIGURE_PROPERTY;
class FIGURE_SIZE_RESULT;
struct BASEVIEWSETTINGS;
struct FIGURE_TYPE_PROPERTY;

//2Dの骨組み図系
class POST3D_CLASS_DLLSPEC CustomDrawGrScene : public QGraphicsScene
{
    Q_OBJECT
public:
    CustomDrawGrScene(bool syncSetting, QObject *parent = nullptr);

    //exportとdrawは統合したい。

    virtual void resetItems() =0;
    virtual void exportRenderBackGround(QPainter *painter, const int tate, const int yoko, const FIGURE_SIZE_RESULT& size, const FIGURE_PROPERTY& pos);
    // Sceneを使用しない
    static void exportRenderPaperItems(QPainter *painter, const int tate, const int yoko, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &prop, const FIGURE_TYPE_PROPERTY &type);
    virtual void exportRenderItems(QPainter *painter, const int tate,const int yoko, const FIGURE_SIZE_RESULT& size,const FIGURE_PROPERTY& pos);

    virtual BaseBackGround* getBackGround() const = 0;
    virtual FIGURE_TYPE_PROPERTY getFigureTypeProperty() const = 0;
    virtual void SetUseScaledFont(bool) = 0;

protected:
    const bool syncSetting;
};
}

#endif // CUSTOMDRAWGRSCENE_H
