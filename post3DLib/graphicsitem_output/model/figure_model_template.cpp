#include "figure_model_template.h"

#include "output_joint_item_model.h"
#include "output_line_item_model.h"
#include "output_lineplaneitem.h"
#include "output_plane_item_model.h"
namespace  post3dapp {
FigureModelTemplate::FigureModelTemplate(const Analysis2DViewSettings &_setting, const FIGURE_TYPE_PROPERTY &property, QUuid id)
    : FigureTemplate(_setting, property, id)
{

}

QColor FigureModelTemplate::valueTextColor(DATATYPE dataType, ATTENTION_STATUS status) const
{
    Q_UNUSED(dataType);
    return getViewSettings()->getValueColor();
}

QColor FigureModelTemplate::figureColor(DATATYPE dataType, STRUCTTYPE material, ATTENTION_STATUS status) const
{
    Q_UNUSED(status);
    if ( dataType == DATATYPE::TPJOINT ) {
        return getViewSettings()->getJointColor();
    } else if ( dataType == DATATYPE::TPSLAB || dataType == DATATYPE::TPWALL  ) {
        if(material == STRUCTTYPE::STP_S){
            return getViewSettings()->getPlaneSColor();
        }else{
            return getViewSettings()->getPlaneRcColor();
        }
    } else if ( dataType == DATATYPE::TPDAMPER ) {
        return getViewSettings()->getLineDamperColor();
    } else if ( dataType == DATATYPE::TPISO ) {
        return getViewSettings()->getLineIsoColor();
    }else if(dataType == DATATYPE::TPSPRING){
        if(status == ATTENTION_STATUS::STATUS_WARNING){
             return getViewSettings()->getLineIsoColor();
         }else{
             return getViewSettings()->getSupportColor();
         }
    } else {
        if(material == STRUCTTYPE::STP_RC){
            return getViewSettings()->getLineRcColor();
        }else if(material == STRUCTTYPE::STP_S){
            return getViewSettings()->getLineSColor();
        }else if(material == STRUCTTYPE::STP_SRC){
            return getViewSettings()->getLineSrcColor();
        }else if(material == STRUCTTYPE::STP_CFT){
            return getViewSettings()->getLineCftColor();
        }
    }
    return QColor{};
}

QColor FigureModelTemplate::selectColor(DATATYPE) const
{
    return getViewSettings()->getSelectedColor();
}

bool FigureModelTemplate::isLineDirVisible() const
{
    return getViewSettings()->getDirectionVisible();
}

int FigureModelTemplate::nameTextSize(DATATYPE dataType) const
{
    switch(dataType){
    case DATATYPE::TPJOINT:
        return getViewSettings()->getJointNumberSize();
    default:
        return getViewSettings()->getMarkSize();
    }
}

int FigureModelTemplate::valueTextSize(DATATYPE) const
{
    return getViewSettings()->getValueTextSize();
}

bool FigureModelTemplate::nameVisible(DATATYPE dataType) const
{
    switch(dataType){
    case DATATYPE::TPJOINT:
        return getViewSettings()->getJointNumberVisible();
    case DATATYPE::TPCOLUMN:
        return getViewSettings()->getColumnMarkVisible();
    case DATATYPE::TPGIRDER:
        return getViewSettings()->getGirderMarkVisible();
    case DATATYPE::TPBRACE:
        return getViewSettings()->getBraceMarkVisible();
    case DATATYPE::TPWALL:
        return getViewSettings()->getWallMarkVisible();
    case DATATYPE::TPUWALL:
        return getViewSettings()->getFoundationWallMarkVisible();
    case DATATYPE::TPSLAB:
        return getViewSettings()->getSlabMarkVisible();
    case DATATYPE::TPDAMPER:
        return getViewSettings()->getDamperMarkVisible();
    case DATATYPE::TPISO:
        return getViewSettings()->getIsoMarkVisible();
    default:
        return true;
    }
}

bool FigureModelTemplate::figureVisible(DATATYPE) const
{
    // TODO
    return true;
}

bool FigureModelTemplate::valueVisible(DATATYPE) const
{
    // TODO
    return true;
}

QColor FigureModelTemplate::scaleColor() const
{
    return getViewSettings()->getScaleColor();
}

QColor FigureModelTemplate::supportColor() const
{
    return getViewSettings()->getSupportColor();
}

int FigureModelTemplate::jointSize() const
{
    return getViewSettings()->getJointSize();
}

QColor FigureModelTemplate::noteColor() const
{
    return getViewSettings()->getNoteColor();
}

int FigureModelTemplate::noteSize() const
{
    return getViewSettings()->getNoteTextSize();
}

int FigureModelTemplate::scaleTextSize() const
{
    return getViewSettings()->getScaleSize();
}

bool FigureModelTemplate::scaleFloorTextVisible() const
{
    return getViewSettings()->getFloorStringVisible();
}

bool FigureModelTemplate::scaleGridTextVisible() const
{
    return getViewSettings()->getGridStringVisible();
}


} // namespace post3dapp
