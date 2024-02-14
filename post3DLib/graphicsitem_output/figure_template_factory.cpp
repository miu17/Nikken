#include "figure_template_factory.h"

#include "axial_elevation_template.h"
#include "axial_plan_template.h"
#include "centerofrigidity_map_plan_template.h"
#include "check_elevation_template.h"
#include "check_plan_template.h"
#include "define_draw_figure.h"
#include "deformation_plan_template.h"
#include "deformation_elevation_template.h"
#include "face_length_elevation_template.h"
#include "face_length_plan_template.h"
#include "figure_template_elevation.h"
#include "figure_template_plan.h"
#include "model_elevation_template.h"
#include "model_plan_template.h"
#include "map_plan_template.h"
#include "numeric_elevation_template.h"
#include "numeric_plan_template.h"
#include "reactionmap_plan_template.h"
#include "stress_elevation_template.h"
#include "stress_plan_template.h"
#include "torsion_elevation_template.h"
#include "torsion_plan_template.h"
#include "verticalloadmap_plan_template.h"

namespace post3dapp{

FigureTemplate* FigureTemplateFactory::CreateTemplate(const Analysis2DViewSettings& setting, const FIGURE_TYPE_PROPERTY& property, QUuid uuid)
{
    if(property.scenetype == SCENETYPE::ELEVATIONSCENE){
        return CreateElevTemplate(setting, property,uuid);
    }else if(property.scenetype == SCENETYPE::PLANSCENE){
        return CreatePlanTemplate(setting, property,uuid);
    }else{
        qDebug()<<"OutputControl::CreateTemplate() : Elevation demo Plan demo nai";
        return nullptr;
    }
}

FigureTemplate* FigureTemplateFactory::CreatePlanTemplate(const Analysis2DViewSettings& setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
{
    switch(type.type){
    case OUTFIGTYPE::MODEL:
        qDebug()<<"modelPlanTemplate";
        return new ModelPlanTemplate(setting, type,uuid);
        break;
    case OUTFIGTYPE::FSTRESS:
    case OUTFIGTYPE::NSTRESS:
    case OUTFIGTYPE::RSTRESS:
    case OUTFIGTYPE::CMQ:
    case OUTFIGTYPE::RANK:
        return new StressPlanTemplate(setting, type,uuid);
        break;
    case OUTFIGTYPE::AXIAL:
        return new AxialPlanTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::DISP:
    case OUTFIGTYPE::COORDINATE:
    case OUTFIGTYPE::LENGTH:
    case OUTFIGTYPE::LATERALBUCKLEN:
    case OUTFIGTYPE::EULERBUCKLEN:
    case OUTFIGTYPE::RIGIDITYPHI:
    case OUTFIGTYPE::GCKCP:
    case OUTFIGTYPE::JOINTMASS:

    case OUTFIGTYPE::ALLOW:
    case OUTFIGTYPE::STRENGTH:
    case OUTFIGTYPE::PLASTICRATIO:
    case OUTFIGTYPE::YIELDSTEP:
    case OUTFIGTYPE::YIELDLOADMAG:

    case OUTFIGTYPE::OUTERFORCE:
    case OUTFIGTYPE::OUTERMOMENT:
        return new NumericPlanTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::CHECKRATIO:
    case OUTFIGTYPE::ALLOWRATIOE:
    case OUTFIGTYPE::ALLOWRATIOU:
        return new CheckPlanTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::VERTICALLOADMAP:
        return new VerticalLoadMapPlanTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::TORSION:
        return new TorsionPlanTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::COMBIREACTMAP:
    case OUTFIGTYPE::REACTMAP:
        return new ReactionMapPlanTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::FACELENGTH:
    case OUTFIGTYPE::RIGIDLENGTH:
        return new FaceLengthPlanTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::SHEARMAP:
    case OUTFIGTYPE::AXIALMAP:
    case OUTFIGTYPE::LAYERDEFORMMAP:
    case OUTFIGTYPE::LAYERSTIFFMAP:
        return new MapPlanTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::CenterOfRigidity:
        return new CenterOfRigidityPlanTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::DEFORM:
        return new DeformationPlanTemplate(setting, type, uuid);
        break;
    default :
        return new NumericPlanTemplate(setting, type,uuid);
    }
    return nullptr;

}
FigureTemplate* FigureTemplateFactory::CreateElevTemplate(const Analysis2DViewSettings& setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
{
    //テンプレートをどの程度細分化するかはもう少し考える
    //全部は網羅していないので適宜追加
    switch(type.type){
    case OUTFIGTYPE::MODEL:
        qDebug()<<"modelElevationTemplate";
        return new ModelElevationTemplate(setting, type,uuid);
        break;
    case OUTFIGTYPE::FSTRESS:
    case OUTFIGTYPE::NSTRESS:
    case OUTFIGTYPE::RSTRESS:
    case OUTFIGTYPE::CMQ:
    case OUTFIGTYPE::RANK:
        qDebug()<<"stressElevationTemplate";
        return new StressElevationTemplate(setting, type,uuid);
        break;
    case OUTFIGTYPE::AXIAL:
        return new AxialElevationTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::DISP:
    case OUTFIGTYPE::COORDINATE:
    case OUTFIGTYPE::LENGTH:
    case OUTFIGTYPE::LATERALBUCKLEN:
    case OUTFIGTYPE::EULERBUCKLEN:
    case OUTFIGTYPE::RIGIDITYPHI:
    case OUTFIGTYPE::JOINTMASS:

    case OUTFIGTYPE::ALLOW:
    case OUTFIGTYPE::STRENGTH:
    case OUTFIGTYPE::PLASTICRATIO:
    case OUTFIGTYPE::YIELDSTEP:
    case OUTFIGTYPE::YIELDLOADMAG:

    case OUTFIGTYPE::OUTERFORCE:
    case OUTFIGTYPE::OUTERMOMENT:
        return new NumericElevationTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::CHECKRATIO:
    case OUTFIGTYPE::ALLOWRATIOE:
    case OUTFIGTYPE::ALLOWRATIOU:
        return new CheckElevationTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::TORSION:
        return new TorsionElevationTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::FACELENGTH:
    case OUTFIGTYPE::RIGIDLENGTH:
        return new FaceLengthElevationTemplate(setting, type, uuid);
        break;
    case OUTFIGTYPE::DEFORM:
        return new DeformationElevationTemplate(setting, type, uuid);
        break;
    default :
        return new NumericElevationTemplate(setting, type,uuid);
    }
    return nullptr;
}



} // namespace post3dapp
