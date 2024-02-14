#include "output_joint_item_react.h"
#include "fixed_data.h"
#include "figure_template.h"
namespace post3dapp{

OutputJointItemReact::OutputJointItemReact(FigureTemplate* fig, DATATYPE dtype, STRUCTTYPE stype,const QUuid &uid,VEC3D mypvec,VEC3D myxvec)
    : OutputJointItem( fig, dtype,stype, uid,mypvec,myxvec ){}



void OutputJointItemReact::setDrawingStatusDirect(const OUTFIG_TYPE_OPTION &type)
{
    QColor color = stringColor(ATTENTION_STATUS::STATUS_NORMAL);
    QList<qreal> reaction = UnifiedFixedData::getInstance()->getPstnData()->getCombinationReaction(myUuid, type.stress1);//loadName,coeflist, steplist);

    if (qAbs(reaction.at(0)) + qAbs(reaction.at(1)) + qAbs(reaction.at(2)) > 0.00001) {
        OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, QString::number(reaction.at(0), 'f', 3),
                                          color, OFFSETPOINT::OFFSET_RIGHTTOP, 999999,valueTextSize) );
        OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, QString::number(reaction.at(1), 'f', 3),
                                          color, OFFSETPOINT::OFFSET_RIGHTMIDDLE, 999999,valueTextSize) );
        OTvaluetext.append( POINT_AND_TEXT(QPointF(0, 0), 0.0, QString::number(reaction.at(2), 'f', 3),
                                          color, OFFSETPOINT::OFFSET_RIGHTBOTTOM, 999999,valueTextSize) );
    }


}

} // namespace post3dapp
