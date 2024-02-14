#ifndef OUTPUTPOINTITEM_H
#define OUTPUTPOINTITEM_H

#include "output_graphicsitem.h"

namespace post3dapp {
enum class DATATYPE;
enum class STRUCTTYPE;
class FigureCommonTemplate;

class OutputPointItem : public OutputGraphicsItem
{
public:

    //指定された座標に指定されたスタイルでプロット。文字列を表示
    OutputPointItem(FigureTemplate* fig, DATATYPE dtype,QPointF point,QString str,QColor color,int size );
    QPainterPath shape() const override;


protected:

    void createPointShapePath();
private:
    QPointF point;
    QString text;
    QColor color;
    int size;

};
} // namespace post3dapp

#endif // OUTPUTPOINTITEM_H
