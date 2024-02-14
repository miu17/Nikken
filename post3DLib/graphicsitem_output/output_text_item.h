#ifndef OUTPUTTEXTITEM_H
#define OUTPUTTEXTITEM_H


#include "output_graphicsitem.h"

namespace post3dapp {
enum class DATATYPE;
enum class STRUCTTYPE;
class FigureCommonTemplate;

class OutputTextItem : public OutputGraphicsItem
{
public:

    //指定された座標に指定されたスタイルでプロット。文字列を表示
    OutputTextItem(FigureCommonTemplate* fig, DATATYPE dtype,QPointF point,QString str );
    QPainterPath shape() const override;


protected:

    void createTextShapePath();
private:
    QString text;

};
} // namespace post3dapp
#endif // OUTPUTTEXTITEM_H
