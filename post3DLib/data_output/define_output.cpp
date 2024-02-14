#include "define_output.h"

namespace post3dapp{

QString OutputFigureText::outfigtypeToString(OUTFIGTYPE otype)
{
    switch(otype){
    case OUTFIGTYPE::PLAN:              return u8"伏図";
    case OUTFIGTYPE::PLANINPUT:         return u8"伏図(入力中画面)";
    case OUTFIGTYPE::LOADPLAN:          return u8"荷重伏図";
    case OUTFIGTYPE::SECTION:           return u8"軸組図";
    case OUTFIGTYPE::SECTIONINPUT:      return u8"軸組図(入力中画面)";
    case OUTFIGTYPE::LOADSECTION:       return u8"荷重軸組図";
    case OUTFIGTYPE::CALCLOAD2D:        return u8"荷重拾い伏図";
    case OUTFIGTYPE::MODEL:             return u8"モデル図";
    case OUTFIGTYPE::LENGTH:            return u8"部材長図";
    case OUTFIGTYPE::FACELENGTH:        return u8"フェイス長図";
    case OUTFIGTYPE::COORDINATE:        return u8"節点座標図";
    case OUTFIGTYPE::RIGIDLENGTH:       return u8"剛域長図";
    case OUTFIGTYPE::LATERALBUCKLEN:    return u8"横座屈長さ図";
    case OUTFIGTYPE::EULERBUCKLEN:      return u8"圧縮座屈長さ図";
    case OUTFIGTYPE::JOINTMASS:         return u8"節点重量図";
    case OUTFIGTYPE::OUTERFORCE:        return u8"節点外力図";
    case OUTFIGTYPE::OUTERMOMENT:       return u8"節点外力M図";
    case OUTFIGTYPE::RIGIDITYPHI:       return u8"剛比・曲げ剛性増大率図";
    case OUTFIGTYPE::CMQ:               return u8"CMQ図";
    case OUTFIGTYPE::AXIALMAP:          return u8"軸力マップ";
    case OUTFIGTYPE::REACTMAP:          return u8"反力マップ";
    case OUTFIGTYPE::COMBIREACTMAP:     return u8"組合せ鉛直反力マップ";
    case OUTFIGTYPE::SHEARMAP:          return u8"せん断力マップ";
    case OUTFIGTYPE::VERTICALLOADMAP:   return u8"鉛直荷重マップ";
    case OUTFIGTYPE::LAYERDEFORMMAP:    return u8"層間変形マップ";
    case OUTFIGTYPE::LAYERSTIFFMAP:     return u8"層剛性マップ";
    case OUTFIGTYPE::GCKCP:             return u8"層せん断力中心・剛心伏図"; // New3DでCenterOfRigidityに移行？
    case OUTFIGTYPE::NSTRESS:           return u8"節点応力図";
    case OUTFIGTYPE::FSTRESS:           return u8"フェイス応力図";
    case OUTFIGTYPE::RSTRESS:           return u8"剛域端応力図";
    case OUTFIGTYPE::AXIAL:             return u8"軸力図";
    case OUTFIGTYPE::TORSION:           return u8"ねじれモーメント図";
    case OUTFIGTYPE::DEFORM:            return u8"変形図";
    case OUTFIGTYPE::DISP:              return u8"節点変位図";
    case OUTFIGTYPE::CHECKRATIO:        return u8"断面検討余裕率図";
    case OUTFIGTYPE::ALLOWRATIOE:       return u8"耐力余裕率E図";
    case OUTFIGTYPE::ALLOWRATIOU:       return u8"耐力余裕率U図";
    case OUTFIGTYPE::PLASTICRATIO:      return u8"塑性率図";
    case OUTFIGTYPE::RANK:              return u8"ランク図";
    case OUTFIGTYPE::ALLOW:             return u8"許容応力図";
    case OUTFIGTYPE::STRENGTH:          return u8"部材耐力図";
    case OUTFIGTYPE::YIELDSTEP:         return u8"降伏ステップ図";
    case OUTFIGTYPE::YIELDLOADMAG:      return u8"降伏荷重倍率図";
    case OUTFIGTYPE::CenterOfRigidity:  return u8"層せん断力中心・剛心伏図";
    case OUTFIGTYPE::QDelta:            return u8"層せん断力-層間変位関係/各階";
    case OUTFIGTYPE::QDeltaFrameFloors: return u8"層せん断力-層間変位関係/ある架構の階毎";
    case OUTFIGTYPE::QDeltaFloorFrames: return u8"層せん断力-層間変位関係/ある階の架構毎";
    case OUTFIGTYPE::QTheta:            return u8"層せん断力-層間変形角関係/各階";
    case OUTFIGTYPE::QThetaFrameFloors: return u8"層せん断力-層間変形角関係/ある架構の階毎";
    case OUTFIGTYPE::QThetaFloorFrames: return u8"層せん断力-層間変形角関係/ある階の架構毎";
    case OUTFIGTYPE::QuNeedf:           return u8"必要保有水平耐力と保有水平耐力の比較図";
    case OUTFIGTYPE::NMINT:             return u8"N-MY-MZ耐力曲面";
    default:
        throw;
    }

}

QString OutputFigureText::outfigtypeToEnglish(OUTFIGTYPE otype)
{
    switch(otype){
    case OUTFIGTYPE::PLANINPUT:         return u8"PlanInput";
    case OUTFIGTYPE::LOADPLAN:          return u8"LoadPlan";
    case OUTFIGTYPE::SECTIONINPUT:      return u8"SectionInput";
    case OUTFIGTYPE::LOADSECTION:       return u8"LoadSection";
    case OUTFIGTYPE::CALCLOAD2D:        return u8"CalcLoad2D";
    case OUTFIGTYPE::MODEL:             return u8"Model";
    case OUTFIGTYPE::LENGTH:            return u8"Length";
    case OUTFIGTYPE::FACELENGTH:        return u8"FaceLength";
    case OUTFIGTYPE::COORDINATE:        return u8"Coordinate";
    case OUTFIGTYPE::RIGIDLENGTH:       return u8"RigidLength";
    case OUTFIGTYPE::LATERALBUCKLEN:    return u8"LateralBucklen";
    case OUTFIGTYPE::EULERBUCKLEN:      return u8"EulerBucklen";
    case OUTFIGTYPE::JOINTMASS:         return u8"JointMass";
    case OUTFIGTYPE::OUTERFORCE:        return u8"OuterForce";
    case OUTFIGTYPE::OUTERMOMENT:       return u8"OuterMoment";
    case OUTFIGTYPE::RIGIDITYPHI:       return u8"RigidityPhi";
    case OUTFIGTYPE::CMQ:               return u8"CMQ";
    case OUTFIGTYPE::AXIALMAP:          return u8"AxialMap";
    case OUTFIGTYPE::REACTMAP:          return u8"ReactMap";
    case OUTFIGTYPE::COMBIREACTMAP:     return u8"CombiReactMap";
    case OUTFIGTYPE::SHEARMAP:          return u8"ShearMap";
    case OUTFIGTYPE::VERTICALLOADMAP:   return u8"VerticalLoadMap";
    case OUTFIGTYPE::LAYERDEFORMMAP:    return u8"LayerDeformMap";
    case OUTFIGTYPE::LAYERSTIFFMAP:     return u8"LayerStiffMap";
    case OUTFIGTYPE::GCKCP:             return u8"Gckcp";
    case OUTFIGTYPE::NSTRESS:           return u8"NStress";
    case OUTFIGTYPE::FSTRESS:           return u8"FStress";
    case OUTFIGTYPE::RSTRESS:           return u8"GStress";
    case OUTFIGTYPE::AXIAL:             return u8"Axial";
    case OUTFIGTYPE::TORSION:           return u8"Torison";
    case OUTFIGTYPE::DEFORM:            return u8"Deform";
    case OUTFIGTYPE::DISP:              return u8"Disp";
    case OUTFIGTYPE::CHECKRATIO:        return u8"CheckRatio";
    case OUTFIGTYPE::ALLOWRATIOE:       return u8"AllowRatioE";
    case OUTFIGTYPE::ALLOWRATIOU:       return u8"AllowRatioU";
    case OUTFIGTYPE::PLASTICRATIO:      return u8"PlasticRatio";
    case OUTFIGTYPE::RANK:              return u8"Rank";
    case OUTFIGTYPE::ALLOW:             return u8"Allow";
    case OUTFIGTYPE::STRENGTH:          return u8"Strength";
    case OUTFIGTYPE::YIELDSTEP:         return u8"YieldStep";
    case OUTFIGTYPE::YIELDLOADMAG:      return u8"YieldLoadMag";
    case OUTFIGTYPE::CenterOfRigidity:  return u8"CenterOfRigidity";
    case OUTFIGTYPE::QDelta:            return u8"QDelta";
    case OUTFIGTYPE::QDeltaFrameFloors: return u8"QDeltaFrameFloors";
    case OUTFIGTYPE::QDeltaFloorFrames: return u8"QDeltaFloorFrames";
    case OUTFIGTYPE::QTheta:            return u8"QTheta";
    case OUTFIGTYPE::QThetaFrameFloors: return u8"QThetaFrameFloors";
    case OUTFIGTYPE::QThetaFloorFrames: return u8"QThetaFloorFrames";
    case OUTFIGTYPE::QuNeedf:           return u8"QuNeedf";
    case OUTFIGTYPE::NMINT:             return u8"Nmint";
    default:
        throw;
    }

}

void MODEDATA::setModeData(QHash<QUuid, QVector<XYZ> > _mode, QVector<QVector<qreal> > _beta, QVector<qreal> _naturalPeriod, QVector<qreal> _totalMass)
{
    mode = _mode;
    nummode = _mode.keys().count();
    beta = _beta;
    naturalPeriod = _naturalPeriod;
    totalMass = _totalMass;
    resetStandardizedMode();
    resetEffectiveMassRatio();
}

XYZ MODEDATA::getModeDisp(MODE_TYPE type, QUuid uid, int nmode, qreal dispratio, qreal time, qreal timespeedratio)
{
    if (nmode <= 0 || nmode > nummode)return XYZ(); //nmodeは正の整数
    XYZ basedisp;
    if (type == MODE) {
        basedisp = standardizedMode[uid].at(nmode - 1);
    } else if (type == BETAX_U) {
        basedisp = mode[uid].at(nmode - 1) * beta.at(nmode - 1).at(0);
    } else if (type == BETAY_U) {
        basedisp = mode[uid].at(nmode - 1) * beta.at(nmode - 1).at(1);
    }
    return basedisp * dispratio * qCos(2.0 * M_PI * time * timespeedratio / naturalPeriod.at(
                                           nmode - 1));
}

void MODEDATA::clear()
{
    naturalPeriod.clear();
    beta.clear();
    totalMass.clear();
    effectiveMassRatio.clear();
    mode.clear();
    standardizedMode.clear();
}

void MODEDATA::resetStandardizedMode()
{
    standardizedMode.clear();
    for (int i = 0; i < nummode; i++) {
        qreal min = 0.0;
        qreal max = 0.0;

        foreach (QVector<XYZ> modeofjoint, mode) {
            if (modeofjoint.at(i).x < min)min = modeofjoint.at(i).x;
            if (modeofjoint.at(i).y < min)min = modeofjoint.at(i).y;
            if (modeofjoint.at(i).z < min)min = modeofjoint.at(i).z;
            if (modeofjoint.at(i).x > max)max = modeofjoint.at(i).x;
            if (modeofjoint.at(i).y > max)max = modeofjoint.at(i).y;
            if (modeofjoint.at(i).z > max)max = modeofjoint.at(i).z;
        }
        qreal coef = qMax(qAbs(min), qAbs(max));
        foreach (QUuid key, mode.keys()) {
            QVector<XYZ> xyz = mode[key];
            if (coef == 0.0)standardizedMode[key][i] = XYZ(0.0, 0.0, 0.0);
            else {
                standardizedMode[key][i] = XYZ(xyz.at(i).x / coef, xyz.at(i).y / coef, xyz.at(i).z / coef);
            }

        }
    }
}

void MODEDATA::resetEffectiveMassRatio()
{
    effectiveMassRatio.clear();
    QVector<qreal> emratio;
    for (int i = 0; i < nummode; i++) {
        emratio.clear();
        for (int j = 0; j < 6; j++) {
            if (totalMass.at(j) == 0.0) {
                emratio << 0.0;
            } else {
                emratio << beta.at(i).at(j)*beta.at(i).at(j) / totalMass.at(j) * 100.0;
            }
        }
        effectiveMassRatio << emratio;
    }
}

void CHCKDATA::clear()
{
    allowanceRateBending.clear();
    verificationRatioBending.clear();
    verificationRatioShear.clear();
    verificationRatioShear2.clear();
}


} // namespace post3dapp
