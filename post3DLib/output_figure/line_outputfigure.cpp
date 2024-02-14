#include "line_outputfigure.h"

#include "define_draw_figure.h"
namespace post3dapp{

LineOutputFigure::LineOutputFigure(OutputFigureStatus _status,
                                   const QList<TextDataUnit> &_unit,
                                   const QList<OutputFigureStatus> &_premiseStatus)
    : AbstractLineData(outputFigureStatusToCommand(_status), combineDataUnit(_status, _unit)),
      myStatus(_status),
      premiseTypes(_premiseStatus)
{
}

QString LineOutputFigure::outputFigureStatusToCommand(OutputFigureStatus status)
{
    switch(status){
    case OutputFigureStatus::OutgroupP:             return u8"*OUTGROUPP";
    case OutputFigureStatus::OutgroupS:             return u8"*OUTGROUPS";
    case OutputFigureStatus::Ccs:                   return u8"*CCS";
    case OutputFigureStatus::Plan:                  return u8"*PLAN";
    case OutputFigureStatus::Section:               return u8"*SECTION";
    case OutputFigureStatus::Lplan:                 return u8"*LPLAN";
    case OutputFigureStatus::Lsection:              return u8"*LSECTION";
    case OutputFigureStatus::PlanInput:             return u8"*PLANINPUT";
    case OutputFigureStatus::SectionInput:          return u8"*SECTIONINPUT";
    case OutputFigureStatus::SumLoadP:              return u8"*SUMLOADP";
    case OutputFigureStatus::SumLoadS:              return u8"*SUMLOADS";
    // case OutputFigureStatus::SumLoad3d:            return u8"*SUMLOADS";
    case OutputFigureStatus::LcmqP:                 return u8"*LCMQP";
    case OutputFigureStatus::LcmqS:                 return u8"*LCMQS";
    case OutputFigureStatus::JmassP:                return u8"*JMASSP";
    case OutputFigureStatus::JmassS:                return u8"*JMASSS";
    case OutputFigureStatus::ModelP:                return u8"*MODELP";
    case OutputFigureStatus::ModelS:                return u8"*MODELS";
    case OutputFigureStatus::LengthP:               return u8"*LENGTHP";
    case OutputFigureStatus::LengthS:               return u8"*LENGTHS";
    case OutputFigureStatus::FaceLengthP:           return u8"*FACELENGTHP";
    case OutputFigureStatus::FaceLengthS:           return u8"*FACELENGTHS";
    case OutputFigureStatus::CoordinateP:           return u8"*COORDINATEP";
    case OutputFigureStatus::CoordinateS:           return u8"*COORDINATES";
    case OutputFigureStatus::MphaiP:                return u8"*MPHAIP";
    case OutputFigureStatus::MphaiS:                return u8"*MPHAIS";
    case OutputFigureStatus::MrlP:                  return u8"*MRLP";
    case OutputFigureStatus::MrlS:                  return u8"*MRLS";
    case OutputFigureStatus::MclP:                  return u8"*MCLP";
    case OutputFigureStatus::MclS:                  return u8"*MCLS";
    case OutputFigureStatus::MblP:                  return u8"*MBLP";
    case OutputFigureStatus::MblS:                  return u8"*MBLS";
    case OutputFigureStatus::NodeOuterForceP:       return u8"*NODEOUTERFORCEP";
    case OutputFigureStatus::NodeOuterForceS:       return u8"*NODEOUTERFORCES";
    case OutputFigureStatus::NodeOuterMomentP:      return u8"*NODEOUTERMOMENTP";
    case OutputFigureStatus::NodeOuterMomentS:      return u8"*NODEOUTERMOMENTS";
    case OutputFigureStatus::JstrP:                 return u8"*JSTRP";
    case OutputFigureStatus::JstrS:                 return u8"*JSTRS";
    case OutputFigureStatus::FstrP:                 return u8"*FSTRP";
    case OutputFigureStatus::FstrS:                 return u8"*FSTRS";
    case OutputFigureStatus::RstrP:                 return u8"*RSTRP";
    case OutputFigureStatus::RstrS:                 return u8"*RSTRS";
    case OutputFigureStatus::Crez:                  return u8"*CREZ";
    case OutputFigureStatus::AxialForceMap:         return u8"*AXIALFORCEMAP";
    case OutputFigureStatus::React:                 return u8"*REACT";
    case OutputFigureStatus::Qmap:                  return u8"*QMAP";
    case OutputFigureStatus::VerticalLoadMap:       return u8"*VERTICALLOADMAP";
    case OutputFigureStatus::TorsionMomentP:        return u8"*TORSIONMOMENTP";
    case OutputFigureStatus::TorsionMomentS:        return u8"*TORSIONMOMENTS";
    case OutputFigureStatus::FloorDeformationMap:   return u8"*FLOORDEFORMATIONMAP";
    case OutputFigureStatus::FloorRigidityMap:      return u8"*FLOORRIGIDITYMAP";
    case OutputFigureStatus::DeformationP:          return u8"*DEFORMATIONP";
    case OutputFigureStatus::DeformationS:          return u8"*DEFORMATIONS";
    case OutputFigureStatus::DisplacementP:         return u8"*DISPLACEMENTP";
    case OutputFigureStatus::DisplacementS:         return u8"*DISPLACEMENTS";
    case OutputFigureStatus::ChP:                   return u8"*CHP";
    case OutputFigureStatus::ChS:                   return u8"*CHS";
    case OutputFigureStatus::AllowableStressP:      return u8"*ALLOWABLESTRESSP";
    case OutputFigureStatus::AllowableStressS:      return u8"*ALLOWABLESTRESSS";
    case OutputFigureStatus::Gckcp:                 return u8"*GCKCP";
    case OutputFigureStatus::Quds:                  return u8"*QUDS";
    case OutputFigureStatus::QudsCase:              return u8"CASE";
    case OutputFigureStatus::QudsFloor:             return u8"FLOOR";
    case OutputFigureStatus::QudsGrid:              return u8"GRID";
    case OutputFigureStatus::MemberStrengthP:       return u8"*MEMBERSTRENGTHP";
    case OutputFigureStatus::MemberStrengthS:       return u8"*MEMBERSTRENGTHS";
    case OutputFigureStatus::AllowRatioEP:          return u8"*ALLOWRATIOEP";
    case OutputFigureStatus::AllowRatioES:          return u8"*ALLOWRATIOES";
    case OutputFigureStatus::AllowRatioUP:          return u8"*ALLOWRATIOUP";
    case OutputFigureStatus::AllowRatioUS:          return u8"*ALLOWRATIOUS";
    case OutputFigureStatus::Dscurve:               return u8"*DSCURVE";
    case OutputFigureStatus::DscurveFrame:          return u8"*DSCURVEFRAME";
    case OutputFigureStatus::DscurveFloor:          return u8"*DSCURVEFLOOR";
    case OutputFigureStatus::DsQThetaCurve:         return u8"*DSQTHETACURVE";
    case OutputFigureStatus::DsQThetaCurveFrame:    return u8"*DSQTHETACURVEFRAME";
    case OutputFigureStatus::DsQThetaCurveFloor:    return u8"*DSQTHETACURVEFLOOR";
    case OutputFigureStatus::DsYieldStepP:          return u8"*DSYIELDSTEPP";
    case OutputFigureStatus::DsYieldStepS:          return u8"*DSYIELDSTEPS";
    case OutputFigureStatus::DsYieldRatioP:         return u8"*DSYIELDRATIOP";
    case OutputFigureStatus::DsYieldRatioS:         return u8"*DSYIELDRATIOS";
    case OutputFigureStatus::DsPlasticRatioP:       return u8"*DSPLASTICRATIOP";
    case OutputFigureStatus::DsPlasticRatioS:       return u8"*DSPLASTICRATIOS";
    case OutputFigureStatus::Dsstr:                 return u8"*DSSTR";
    case OutputFigureStatus::Mrnk:                  return u8"*MRNK";
    case OutputFigureStatus::Dssh:                  return u8"*DSSH";
    case OutputFigureStatus::Dsmu:                  return u8"*DSMU";
    case OutputFigureStatus::Qucurve:               return u8"*QUCURVE";
    case OutputFigureStatus::QucurveFrame:          return u8"*QUCURVEFRAME";
    case OutputFigureStatus::QucurveFloor:          return u8"*QUCURVEFLOOR";
    case OutputFigureStatus::QuQThetaCurve:         return u8"*QUQTHETACURVE";
    case OutputFigureStatus::QuQThetaCurveFrame:    return u8"*QUQTHETACURVEFRAME";
    case OutputFigureStatus::QuQThetaCurveFloor:    return u8"*QUQTHETACURVEFLOOR";
    case OutputFigureStatus::QuYieldStepP:          return u8"*QUYIELDSTEPP";
    case OutputFigureStatus::QuYieldStepS:          return u8"*QUYIELDSTEPS";
    case OutputFigureStatus::QuYieldRatioP:         return u8"*QUYIELDRATIOP";
    case OutputFigureStatus::QuYieldRatioS:         return u8"*QUYIELDRATIOS";
    case OutputFigureStatus::QuPlasticRatioP:       return u8"*QUPLASTICRATIOP";
    case OutputFigureStatus::QuPlasticRatioS:       return u8"*QUPLASTICRATIOS";
    case OutputFigureStatus::Qustr:                 return u8"*QUSTR";
    case OutputFigureStatus::Qush:                  return u8"*QUSH";
    case OutputFigureStatus::Dsqucurve:             return u8"*DSQUCURVE";
    case OutputFigureStatus::Quneedf:               return u8"*QUNEEDF";
    case OutputFigureStatus::Nmint:                 return u8"*NMINT";
    case OutputFigureStatus::FinalEnd:              return u8"*FINALEND";
    default:
        throw;
    }
}

BASE_TYPE_PROPERTY::DSQU LineOutputFigure::makeDsQu(OutputFigureStatus status)
{
    switch(status){
    case OutputFigureStatus::MemberStrengthP:
    case OutputFigureStatus::MemberStrengthS:
    case OutputFigureStatus::AllowRatioEP:
    case OutputFigureStatus::AllowRatioES:
    case OutputFigureStatus::AllowRatioUP:
    case OutputFigureStatus::AllowRatioUS:
    case OutputFigureStatus::Dscurve:
    case OutputFigureStatus::DscurveFrame:
    case OutputFigureStatus::DscurveFloor:
    case OutputFigureStatus::DsQThetaCurve:
    case OutputFigureStatus::DsQThetaCurveFrame:
    case OutputFigureStatus::DsQThetaCurveFloor:
    case OutputFigureStatus::DsYieldStepP:
    case OutputFigureStatus::DsYieldStepS:
    case OutputFigureStatus::DsYieldRatioP:
    case OutputFigureStatus::DsYieldRatioS:
    case OutputFigureStatus::DsPlasticRatioP:
    case OutputFigureStatus::DsPlasticRatioS:
    case OutputFigureStatus::Dsstr:
    case OutputFigureStatus::Mrnk:
    case OutputFigureStatus::Dssh:
    case OutputFigureStatus::Dsmu:
        return BASE_TYPE_PROPERTY::DSQU::DS;
    case OutputFigureStatus::Qucurve:
    case OutputFigureStatus::QucurveFrame:
    case OutputFigureStatus::QucurveFloor:
    case OutputFigureStatus::QuQThetaCurve:
    case OutputFigureStatus::QuQThetaCurveFrame:
    case OutputFigureStatus::QuQThetaCurveFloor:
    case OutputFigureStatus::QuYieldStepP:
    case OutputFigureStatus::QuYieldStepS:
    case OutputFigureStatus::QuYieldRatioP:
    case OutputFigureStatus::QuYieldRatioS:
    case OutputFigureStatus::QuPlasticRatioP:
    case OutputFigureStatus::QuPlasticRatioS:
    case OutputFigureStatus::Qustr:
    case OutputFigureStatus::Qush:
    case OutputFigureStatus::Dsqucurve:
        return BASE_TYPE_PROPERTY::DSQU::QU;
    default:
        throw;
    }
}

OUTFIGTYPE LineOutputFigure::makeOutFigType(OutputFigureStatus status)
{
    switch(status){
    case OutputFigureStatus::Plan:                  return OUTFIGTYPE::PLAN;
    case OutputFigureStatus::Section:               return OUTFIGTYPE::SECTION;
    case OutputFigureStatus::Lplan:                 return OUTFIGTYPE::LOADPLAN;
    case OutputFigureStatus::Lsection:              return OUTFIGTYPE::LOADSECTION;
    case OutputFigureStatus::PlanInput:             return OUTFIGTYPE::PLANINPUT;
    case OutputFigureStatus::SectionInput:          return OUTFIGTYPE::SECTIONINPUT;
    case OutputFigureStatus::SumLoadP:              return OUTFIGTYPE::CALCLOAD2D;
    case OutputFigureStatus::SumLoadS:              return OUTFIGTYPE::CALCLOAD2D;
    // case OutputFigureStatus::SumLoad3d:            return OUTFIGTYPE::CALCLOAD3D;
    case OutputFigureStatus::LcmqP:                 return OUTFIGTYPE::CMQ;
    case OutputFigureStatus::LcmqS:                 return OUTFIGTYPE::CMQ;
    case OutputFigureStatus::JmassP:                return OUTFIGTYPE::JOINTMASS;
    case OutputFigureStatus::JmassS:                return OUTFIGTYPE::JOINTMASS;
    case OutputFigureStatus::ModelP:                return OUTFIGTYPE::MODEL;
    case OutputFigureStatus::ModelS:                return OUTFIGTYPE::MODEL;
    case OutputFigureStatus::LengthP:               return OUTFIGTYPE::LENGTH;
    case OutputFigureStatus::LengthS:               return OUTFIGTYPE::LENGTH;
    case OutputFigureStatus::FaceLengthP:           return OUTFIGTYPE::FACELENGTH;
    case OutputFigureStatus::FaceLengthS:           return OUTFIGTYPE::FACELENGTH;
    case OutputFigureStatus::CoordinateP:           return OUTFIGTYPE::COORDINATE;
    case OutputFigureStatus::CoordinateS:           return OUTFIGTYPE::COORDINATE;
    case OutputFigureStatus::MphaiP:                return OUTFIGTYPE::RIGIDITYPHI;
    case OutputFigureStatus::MphaiS:                return OUTFIGTYPE::RIGIDITYPHI;
    case OutputFigureStatus::MrlP:                  return OUTFIGTYPE::RIGIDLENGTH;
    case OutputFigureStatus::MrlS:                  return OUTFIGTYPE::RIGIDLENGTH;
    case OutputFigureStatus::MclP:                  return OUTFIGTYPE::EULERBUCKLEN;
    case OutputFigureStatus::MclS:                  return OUTFIGTYPE::EULERBUCKLEN;
    case OutputFigureStatus::MblP:                  return OUTFIGTYPE::LATERALBUCKLEN;
    case OutputFigureStatus::MblS:                  return OUTFIGTYPE::LATERALBUCKLEN;
    case OutputFigureStatus::NodeOuterForceP:       return OUTFIGTYPE::OUTERFORCE;
    case OutputFigureStatus::NodeOuterForceS:       return OUTFIGTYPE::OUTERFORCE;
    case OutputFigureStatus::NodeOuterMomentP:      return OUTFIGTYPE::OUTERMOMENT;
    case OutputFigureStatus::NodeOuterMomentS:      return OUTFIGTYPE::OUTERMOMENT;
    case OutputFigureStatus::JstrP:                 return OUTFIGTYPE::NSTRESS;
    case OutputFigureStatus::JstrS:                 return OUTFIGTYPE::NSTRESS;
    case OutputFigureStatus::FstrP:                 return OUTFIGTYPE::FSTRESS;
    case OutputFigureStatus::FstrS:                 return OUTFIGTYPE::FSTRESS;
    case OutputFigureStatus::RstrP:                 return OUTFIGTYPE::RSTRESS;
    case OutputFigureStatus::RstrS:                 return OUTFIGTYPE::RSTRESS;
    case OutputFigureStatus::Crez:                  return OUTFIGTYPE::COMBIREACTMAP;
    case OutputFigureStatus::AxialForceMap:         return OUTFIGTYPE::AXIALMAP;
    case OutputFigureStatus::React:                 return OUTFIGTYPE::REACTMAP;
    case OutputFigureStatus::Qmap:                  return OUTFIGTYPE::SHEARMAP;
    case OutputFigureStatus::VerticalLoadMap:       return OUTFIGTYPE::VERTICALLOADMAP;
    case OutputFigureStatus::TorsionMomentP:        return OUTFIGTYPE::TORSION;
    case OutputFigureStatus::TorsionMomentS:        return OUTFIGTYPE::TORSION;
    case OutputFigureStatus::FloorDeformationMap:   return OUTFIGTYPE::LAYERDEFORMMAP;
    case OutputFigureStatus::FloorRigidityMap:      return OUTFIGTYPE::LAYERSTIFFMAP;
    case OutputFigureStatus::DeformationP:          return OUTFIGTYPE::DEFORM;
    case OutputFigureStatus::DeformationS:          return OUTFIGTYPE::DEFORM;
    case OutputFigureStatus::DisplacementP:         return OUTFIGTYPE::DISP;
    case OutputFigureStatus::DisplacementS:         return OUTFIGTYPE::DISP;
    case OutputFigureStatus::ChP:                   return OUTFIGTYPE::CHECKRATIO;
    case OutputFigureStatus::ChS:                   return OUTFIGTYPE::CHECKRATIO;
    case OutputFigureStatus::AllowableStressP:      return OUTFIGTYPE::ALLOW;
    case OutputFigureStatus::AllowableStressS:      return OUTFIGTYPE::ALLOW;
    case OutputFigureStatus::Gckcp:                 return OUTFIGTYPE::CenterOfRigidity;
    case OutputFigureStatus::MemberStrengthP:       return OUTFIGTYPE::STRENGTH;
    case OutputFigureStatus::MemberStrengthS:       return OUTFIGTYPE::STRENGTH;
    case OutputFigureStatus::AllowRatioEP:          return OUTFIGTYPE::ALLOWRATIOE;
    case OutputFigureStatus::AllowRatioES:          return OUTFIGTYPE::ALLOWRATIOE;
    case OutputFigureStatus::AllowRatioUP:          return OUTFIGTYPE::ALLOWRATIOU;
    case OutputFigureStatus::AllowRatioUS:          return OUTFIGTYPE::ALLOWRATIOU;
    case OutputFigureStatus::Dscurve:               return OUTFIGTYPE::QDelta;
    case OutputFigureStatus::DscurveFrame:          return OUTFIGTYPE::QDeltaFrameFloors;
    case OutputFigureStatus::DscurveFloor:          return OUTFIGTYPE::QDeltaFloorFrames;
    case OutputFigureStatus::DsQThetaCurve:         return OUTFIGTYPE::QTheta;
    case OutputFigureStatus::DsQThetaCurveFrame:    return OUTFIGTYPE::QThetaFrameFloors;
    case OutputFigureStatus::DsQThetaCurveFloor:    return OUTFIGTYPE::QThetaFloorFrames;
    case OutputFigureStatus::DsYieldStepP:          return OUTFIGTYPE::YIELDSTEP;
    case OutputFigureStatus::DsYieldStepS:          return OUTFIGTYPE::YIELDSTEP;
    case OutputFigureStatus::DsYieldRatioP:         return OUTFIGTYPE::YIELDLOADMAG;
    case OutputFigureStatus::DsYieldRatioS:         return OUTFIGTYPE::YIELDLOADMAG;
    case OutputFigureStatus::DsPlasticRatioP:       return OUTFIGTYPE::PLASTICRATIO;
    case OutputFigureStatus::DsPlasticRatioS:       return OUTFIGTYPE::PLASTICRATIO;
    case OutputFigureStatus::Dsstr:                 return OUTFIGTYPE::FSTRESS; // フェイス？
    case OutputFigureStatus::Mrnk:                  return OUTFIGTYPE::RANK;
    //case OutputFigureStatus::Dssh:                  return u8"*DSSH";
    case OutputFigureStatus::Dsmu:                  return OUTFIGTYPE::STRENGTH;
    case OutputFigureStatus::Qucurve:               return OUTFIGTYPE::QDelta;
    case OutputFigureStatus::QucurveFrame:          return OUTFIGTYPE::QDeltaFrameFloors;
    case OutputFigureStatus::QucurveFloor:          return OUTFIGTYPE::QDeltaFloorFrames;
    case OutputFigureStatus::QuQThetaCurve:         return OUTFIGTYPE::QTheta;
    case OutputFigureStatus::QuQThetaCurveFrame:    return OUTFIGTYPE::QThetaFrameFloors;
    case OutputFigureStatus::QuQThetaCurveFloor:    return OUTFIGTYPE::QThetaFloorFrames;
    case OutputFigureStatus::QuYieldStepP:          return OUTFIGTYPE::YIELDSTEP;
    case OutputFigureStatus::QuYieldStepS:          return OUTFIGTYPE::YIELDSTEP;
    case OutputFigureStatus::QuYieldRatioP:         return OUTFIGTYPE::YIELDLOADMAG;
    case OutputFigureStatus::QuYieldRatioS:         return OUTFIGTYPE::YIELDLOADMAG;
    case OutputFigureStatus::QuPlasticRatioP:       return OUTFIGTYPE::PLASTICRATIO;
    case OutputFigureStatus::QuPlasticRatioS:       return OUTFIGTYPE::PLASTICRATIO;
    case OutputFigureStatus::Qustr:                 return OUTFIGTYPE::FSTRESS; // フェイス？
    //case OutputFigureStatus::Qush:                  return u8"*QUSH";
    case OutputFigureStatus::Quneedf:               return OUTFIGTYPE::QuNeedf;
    case OutputFigureStatus::Nmint:                 return OUTFIGTYPE::NMINT;
    default:
        throw;
    }
}

QList<TextDataUnit> LineOutputFigure::combineDataUnit(OutputFigureStatus status, const QList<TextDataUnit> &unitList)
{
    TextDataUnit unit = TextDataUnit(TEXTTYPE::TYPE_TEXT, 1, outputFigureStatusToCommand(status).length());
    QList<TextDataUnit> ret{unit};
    if (unitList.count() != 0)
        ret << unitList;
    return ret;
}

bool LineOutputFigure::MatchKeyword(OutputFigureStatus status, const QString& text){
    if (premiseTypes.count() > 0 && !premiseTypes.contains(status))
        return false;

    if (key == "" || key == "-"){
        return text.startsWith(key);
    }else{
        auto split = text.trimmed().split(' ');
        return split[0] == key;
    }
}

FIGURE_TYPE_PROPERTY* SumLoadOutputFigure::makeProperty() const
{
    FIGURE_TYPE_PROPERTY* property;
    switch(this->getStatus()){
    case OutputFigureStatus::SumLoadP:
        property = new FIGURE_TYPE_PROPERTY();
        property->viewtype = MODELVIEWTYPE::CALC2DMODEL;
        property->scenetype = SCENETYPE::PLANSCENE;
        property->type = OUTFIGTYPE::CALCLOAD2D;
        property->loadType = loadType;
        property->loadSumType = loadSumType;
        property->viewdirection = outputDir;
        break;
    case OutputFigureStatus::SumLoadS:
        property = new FIGURE_TYPE_PROPERTY();
        property->viewtype = MODELVIEWTYPE::CALC2DMODEL;
        property->scenetype = SCENETYPE::ELEVATIONSCENE;
        property->type = OUTFIGTYPE::CALCLOAD2D;
        property->loadType = loadType;
        property->loadSumType = loadSumType;
        property->viewdirection = outputDir;
        break;
    default:
        throw;
    }
    return property;
}

bool SumLoadOutputFigure::hasFormatError(const QString &text) const
{
    if (AbstractLineData::hasFormatError(text) == true)
        return true;

    auto loadType = text.mid(10, 3).trimmed();
    if (loadType != "DL"  &&
        loadType != "LL1" &&
        loadType != "LL2" &&
        loadType != "LL3" &&
        loadType != "LL4" &&
        loadType != "LLE" &&
        loadType != "LL0")
        return true;

    auto loadSumType = text.mid(20, 6).trimmed();
    if (loadSumType != "ALL" &&
        loadSumType != "AUTO" &&
        loadSumType != "MANUAL" &&
        loadSumType != "SELF")
        return true;

    auto loadOutputDir = text.mid(30, 5).trimmed();
    if (loadOutputDir != "PLIN" &&
        loadOutputDir != "PLOUT"  &&
        loadOutputDir != "AXIAL")
        return true;

    return false;
}

void SumLoadOutputFigure::setOption(const QString &text)
{
    loadType = LoadType::stringToEnum(text.mid(10, 3).trimmed());
    auto sum = text.mid(20, 6).trimmed();
    if (sum == "ALL")
        loadSumType = LOADSUMTYPE::LOADSUM_ALL;
    else if(sum == "AUTO")
        loadSumType = LOADSUMTYPE::LOADSUM_AUTO;
    else if(sum == "MANUAL")
        loadSumType = LOADSUMTYPE::LOADSUM_MANUAL;
    else if(sum == "SELF")
        loadSumType = LOADSUMTYPE::LOADSUM_SELF;
    else
        throw;

    auto dir = text.mid(30, 5).trimmed();
    if (dir == "PLIN")
        outputDir = OUTPUTDIR::ODIR_PLIN;
    else if(dir == "PLOUT")
        outputDir = OUTPUTDIR::ODIR_PLOUT;
    else if(dir == "AXIAL")
        outputDir = OUTPUTDIR::ODIR_AXIAL;
    else
        throw;
}

} // namespace post3dapp
