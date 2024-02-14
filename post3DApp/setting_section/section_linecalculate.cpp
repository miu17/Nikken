#include "section_linecalculate.h"

#include <QPainter>

#include "section_calculations.h"
#include "section_tablemodel.h"

namespace post3dapp{
SectionLineCalculate::SectionLineCalculate(const SectionDataType &type, QWidget *parent)
    : QWidget(parent), mySectionType(type)
{
    hasCalced = false;
    table3input = false;
    table4input = false;

    mtype = mySectionType.getMemberType();
    stype = mySectionType.getStructType();
    isColumn = (mtype == DATATYPE::TPCOLUMN);
    isGirder = (mtype == DATATYPE::TPGIRDER);
    isBeam = (mtype == DATATYPE::TPBEAM);
    isBrace = (mtype == DATATYPE::TPBRACE);
    isWall = (mtype == DATATYPE::TPWALL);
    isUWall = (mtype == DATATYPE::TPUWALL);
    isSlab = (mtype == DATATYPE::TPSLAB);
    isS = (stype == STRUCTTYPE::STP_S);
    isRC = (stype == STRUCTTYPE::STP_RC);
    isSRC = (stype == STRUCTTYPE::STP_SRC);
    isCFT = (stype == STRUCTTYPE::STP_CFT);
    isIsolator = (mtype == DATATYPE::TPISO);
}

void SectionLineCalculate::setLineValues(const QList<QVariant> &values,
                                         const SectionValues &section)
{
    qDebug()<<"section line calc set value";
    //関数名はlinedraw等に合わせて"set"であるが、実態は数値の更新に応じて色々とやっている。

    //直接入力欄に入力があるかチェック
    hasDirectInput = false;
    qreal EPS = 1.0e-5;
    QList<int> dlist = mySectionType.getDirectInputColumns();
    foreach (int dcol, dlist) {
        if (values.at(dcol).toDouble() > EPS) hasDirectInput = true;
    }

    //中央・右端の入力がされているかチェック
    table3input = mySectionType.hasData(values, 3);
    table4input = mySectionType.hasData(values, 4);

    //メンバ変数のセット
    myValues = values;
    mySection = section;

    //入力内容に応じた計算
    //lineとplaneを統一して扱いたいが、上手い事コーディング出来なかったので取り急ぎ場合分けとする。
    if (isColumn || isGirder || isBeam || isBrace) {

        LineOutputData outputdata[3];
        if (isColumn && table3input) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.j_param);
        } else if (isColumn && !table3input) {
            outputdata[0] = calculateData(mySection.i_param);
        } else if (isGirder && table3input && mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.c_param);
        } else if (isGirder && !table3input && mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
        } else if (isGirder && table3input && table4input && !mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.c_param);
            outputdata[2] = calculateData(mySection.j_param);
        } else if (isGirder && !table3input && table4input && !mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.j_param);
        } else if (isGirder && table3input && !table4input && !mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.c_param);
        } else if (isGirder && !table3input && !table4input && !mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
        } else if (isBeam && table3input && mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.c_param);
        } else if (isBeam && !table3input && mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
        } else if (isBeam && table3input && table4input && !mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.c_param);
            outputdata[2] = calculateData(mySection.j_param);
        } else if (isBeam && !table3input && table4input && !mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.j_param);
        } else if (isBeam && table3input && !table4input && !mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.c_param);
        } else if (isBeam && !table3input && !table4input && !mySection.line_param.isCantilever) {
            outputdata[0] = calculateData(mySection.i_param);
        } else if (isBrace && hasDirectInput) { //関数化するのがベター
            outputdata[0].A = mySection.line_param.direct_area * 100; //cm2→mm2　単位系をハードコーディングするのは・・・
            outputdata[0].Ae = mySection.line_param.direct_effectiveArea * 100; //cm2→mm2
            outputdata[0].slendernessRatio = mySection.line_param.direct_slenderRatio;
            outputdata[0].unitmass = mySection.line_param.direct_unitmass;
        } else if (isBrace && table3input && table4input) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.c_param);
            outputdata[2] = calculateData(mySection.j_param);
        } else if (isBrace && !table3input && table4input) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.j_param);
        } else if (isBrace && table3input && !table4input) {
            outputdata[0] = calculateData(mySection.i_param);
            outputdata[1] = calculateData(mySection.c_param);
        } else if (isBrace && !table3input && !table4input) {
            outputdata[0] = calculateData(mySection.i_param);
        }
        formatoutput = formatResult(outputdata);//出力用に整形
    }//isLineSection
    else if (isWall || isUWall || isSlab) {
        PlaneOutputData outputdata[1];
        outputdata[0] = calculateData(mySection.plane_param);
        formatoutput = formatResult(outputdata);//出力用に整形
    }//isPlaneSection
    else if (isIsolator) {
        formatoutput = isolatorFormatResult();//出力用に整形
    }//isOtherSection

    hasCalced = true; //一度でも計算するとtrue
    qDebug()<<"has calc?";
    update();
    qDebug()<<"update";
    return;
}

void SectionLineCalculate::paintEvent(QPaintEvent *)
{
    //QPainter rect(this);
    //rect.drawRect(1,1,this->width()-2,this->height()-2);
    if (hasCalced) {
        QPainter resultpaint(this);
        drawResult(formatoutput, &resultpaint);
    }
}

LineOutputData SectionLineCalculate::calculateData(const LinePartsParameter &inputdata)
{
    qDebug()<<"section line calc data";
    LineOutputData outputdata;

    if (isS) {

        if ( SteelType::isStandardType( inputdata.steelType ) ) { //形鋼の場合
            QString stdS = inputdata.standardName;
            QString var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_A);
            if ( !var.isEmpty() ) outputdata.A = var.toDouble() * 100.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_Ix);
            if ( !var.isEmpty() ) outputdata.Ix = var.toDouble() * 10000.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_Iy);
            if ( !var.isEmpty() ) outputdata.Iy = var.toDouble() * 10000.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_ix);
            if ( !var.isEmpty() ) outputdata.Srsx = var.toDouble() * 10.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_iy);
            if ( !var.isEmpty() ) outputdata.Srsy = var.toDouble() * 10.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_Zx);
            if ( !var.isEmpty() ) outputdata.Zx = var.toDouble() * 1000.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_Zy);
            if ( !var.isEmpty() ) outputdata.Zy = var.toDouble() * 1000.;
            return outputdata;
        }
        qDebug()<< static_cast<int>(inputdata.steelType) <<
                  inputdata.standardName<<inputdata.t1_T<<inputdata.t2_T;
        outputdata.A = SectionCalculations::calcA_S(inputdata);
        qDebug()<<"area done";
        outputdata.Ae = SectionCalculations::calcAe_S(inputdata);
        outputdata.Ix = SectionCalculations::calcI_S(inputdata, 0);
        outputdata.Iy = SectionCalculations::calcI_S(inputdata, 1);
        outputdata.Zx = SectionCalculations::calcZ_S(inputdata, 0, outputdata.Ix);
        outputdata.Zy = SectionCalculations::calcZ_S(inputdata, 1, outputdata.Iy);
        outputdata.Zex = SectionCalculations::calcZe_S(inputdata, 0);
        outputdata.Zey = SectionCalculations::calcZe_S(inputdata, 1);
        outputdata.Zpx = SectionCalculations::calcZp_S(inputdata, 0);
        outputdata.Zpy = SectionCalculations::calcZp_S(inputdata, 1);
        outputdata.Srsx = SectionCalculations::calcSrs_S(outputdata.Ix, outputdata.A);
        outputdata.Srsy = SectionCalculations::calcSrs_S(outputdata.Iy, outputdata.A);

        outputdata.wtratiof = SectionCalculations::calcWTratio_S(inputdata, SectionPart::Part::FLANGE);
        outputdata.wtratiow = SectionCalculations::calcWTratio_S(inputdata, SectionPart::Part::WEB);
        outputdata.rankF = SectionCalculations::judgeRank(inputdata, mtype, SectionPart::Part::FLANGE);
        outputdata.rankW = SectionCalculations::judgeRank(inputdata, mtype, SectionPart::Part::WEB);
        outputdata.rankSN = SectionCalculations::judgeRankH_SN(inputdata, mtype);
        qDebug()<<outputdata.A<<outputdata.Zx<<outputdata.Ix;

    } else if (isRC) {

        outputdata.A = SectionCalculations::calcA_RC(inputdata);
        outputdata.As = SectionCalculations::calcAs_RC(inputdata, mtype);
        outputdata.Aeq = SectionCalculations::calcAeq_RC(outputdata.A, outputdata.As, 15);
        outputdata.Ix = SectionCalculations::calcI_RC(inputdata, 0);
        outputdata.Iy = SectionCalculations::calcI_RC(inputdata, 1);
        outputdata.Isx = SectionCalculations::calcIs_RC(inputdata, 0, mtype);
        outputdata.Isy = SectionCalculations::calcIs_RC(inputdata, 1, mtype);
        outputdata.Ieqx = SectionCalculations::calcIeq_RC(outputdata.Ix, outputdata.Isx, 15);
        outputdata.Ieqy = SectionCalculations::calcIeq_RC(outputdata.Iy, outputdata.Isy, 15);
        if (isColumn) {
            outputdata.pg = SectionCalculations::calcPg_RC(inputdata, mtype);
        }
        outputdata.pt1 = SectionCalculations::calcPt_RC(inputdata, 0, mtype);
        outputdata.pt2 = SectionCalculations::calcPt_RC(inputdata, 1, mtype);
        outputdata.pt3 = SectionCalculations::calcPt_RC(inputdata, 2, mtype);
        outputdata.pt4 = SectionCalculations::calcPt_RC(inputdata, 3, mtype);
        outputdata.pwx = SectionCalculations::calcPw_RC(inputdata, 0);
        outputdata.pwy = SectionCalculations::calcPw_RC(inputdata, 1);

    } else if (isSRC) {

        if ( SteelType::isStandardType( inputdata.steelType ) ) { //形鋼の場合
            QString stdS = inputdata.standardName;
            QString var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_A);
            if ( !var.isEmpty() ) outputdata.As = var.toDouble() * 100.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_Ix);
            if ( !var.isEmpty() ) outputdata.Isx = var.toDouble() * 10000.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_Iy);
            if ( !var.isEmpty() ) outputdata.Isy = var.toDouble() * 10000.;
        } else {
            outputdata.As = SectionCalculations::calcAs_SRC(inputdata, mtype);
            outputdata.Isx = SectionCalculations::calcIs_SRC(inputdata, 0, mtype);
            outputdata.Isy = SectionCalculations::calcIs_SRC(inputdata, 1, mtype);
        }

        bool isFilledConc = !mySection.line_param.filledConcMaterial.isEmpty();
        outputdata.A = SectionCalculations::calcA_SRC(inputdata, isFilledConc);
        outputdata.Aeq = SectionCalculations::calcAeq_SRC(outputdata.A, outputdata.As, 15);
        outputdata.Ix = SectionCalculations::calcI_SRC(inputdata, 0, isFilledConc);
        outputdata.Iy = SectionCalculations::calcI_SRC(inputdata, 1, isFilledConc);
        outputdata.Ieqx = SectionCalculations::calcIeq_SRC(outputdata.Ix, outputdata.Isx, 15);
        outputdata.Ieqy = SectionCalculations::calcIeq_SRC(outputdata.Iy, outputdata.Isy, 15);
        if (isColumn) {
            outputdata.pg = SectionCalculations::calcPg_SRC(inputdata, mtype, isFilledConc);
        }
        outputdata.pt1 = SectionCalculations::calcPt_SRC(inputdata, 0, mtype);
        outputdata.pt2 = SectionCalculations::calcPt_SRC(inputdata, 1, mtype);
        outputdata.pt3 = SectionCalculations::calcPt_SRC(inputdata, 2, mtype);
        outputdata.pt4 = SectionCalculations::calcPt_SRC(inputdata, 3, mtype);
        outputdata.pwx = SectionCalculations::calcPw_SRC(inputdata, 0);
        outputdata.pwy = SectionCalculations::calcPw_SRC(inputdata, 1);

    } else if (isCFT) {

        if ( SteelType::isStandardType( inputdata.steelType ) ) { //形鋼の場合
            QString stdS = inputdata.standardName;
            QString var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_A);
            if ( !var.isEmpty() ) outputdata.As = var.toDouble() * 100.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_Ix);
            if ( !var.isEmpty() ) outputdata.Isx = var.toDouble() * 10000.;
            var = SectionDataType::standardSteelSectionValue(stdS, StandardSectionTable::SteelFormat::ST_Iy);
            if ( !var.isEmpty() ) outputdata.Isy = var.toDouble() * 10000.;
        } else {
            outputdata.As = SectionCalculations::calcAs_CFT(inputdata);
            outputdata.Isx = SectionCalculations::calcIs_CFT(inputdata, 0);
            outputdata.Isy = SectionCalculations::calcIs_CFT(inputdata, 1);
        }
        outputdata.A = SectionCalculations::calcA_CFT(inputdata);
        outputdata.Aeq = SectionCalculations::calcAeq_CFT(outputdata.A, outputdata.As, 15);
        outputdata.Ix = SectionCalculations::calcI_CFT(inputdata, 0);
        outputdata.Iy = SectionCalculations::calcI_CFT(inputdata, 1);
        outputdata.Ieqx = SectionCalculations::calcIeq_CFT(outputdata.Ix, outputdata.Isx, 15);
        outputdata.Ieqy = SectionCalculations::calcIeq_CFT(outputdata.Iy, outputdata.Isy, 15);

    }
    qDebug()<<"sectioncalc data done";
    return outputdata;
}

PlaneOutputData SectionLineCalculate::calculateData(const PlaneSectionParameter &inputdata)
{
    PlaneOutputData outputdata;

    if (isWall) {
        outputdata.Psv = SectionCalculations::calcPs_Wall(inputdata, PlaneDirection::Direction::VERTICAL);
        outputdata.Psh = SectionCalculations::calcPs_Wall(inputdata, PlaneDirection::Direction::HORIZONTAL);
        outputdata.Arvs = SectionCalculations::calcAr_Wall(inputdata, PlaneDirection::Direction::VERTICAL,
                                                           WallOpenSize::Size::SMALL);
        outputdata.Arhs = SectionCalculations::calcAr_Wall(inputdata, PlaneDirection::Direction::HORIZONTAL,
                                                           WallOpenSize::Size::SMALL);
        outputdata.Ards = SectionCalculations::calcAr_Wall(inputdata, PlaneDirection::Direction::DIAGONAL,
                                                           WallOpenSize::Size::SMALL);
        outputdata.Arvl = SectionCalculations::calcAr_Wall(inputdata, PlaneDirection::Direction::VERTICAL,
                                                           WallOpenSize::Size::LARGE);
        outputdata.Arhl = SectionCalculations::calcAr_Wall(inputdata, PlaneDirection::Direction::HORIZONTAL,
                                                           WallOpenSize::Size::LARGE);
        outputdata.Ardl = SectionCalculations::calcAr_Wall(inputdata, PlaneDirection::Direction::DIAGONAL,
                                                           WallOpenSize::Size::LARGE);
    } else if (isUWall) {
        outputdata.Psv = SectionCalculations::calcPs_Wall(inputdata,
                                                          PlaneDirection::Direction::VERTICAL); //標準配筋
        outputdata.Psh = SectionCalculations::calcPs_Wall(inputdata,
                                                          PlaneDirection::Direction::HORIZONTAL); //標準配筋
        outputdata.Atot = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::OUTER, SectionPart::Part::TOP);
        outputdata.Atit = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::INNER, SectionPart::Part::TOP);
        outputdata.Atom = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::OUTER,
                                                           SectionPart::Part::MIDDLE);
        outputdata.Atim = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::INNER,
                                                           SectionPart::Part::MIDDLE);
        outputdata.Atob = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::OUTER,
                                                           SectionPart::Part::BOTTOM);
        outputdata.Atib = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::INNER,
                                                           SectionPart::Part::BOTTOM);
        outputdata.Atoe = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::OUTER, SectionPart::Part::EDGE);
        outputdata.Atie = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::INNER, SectionPart::Part::EDGE);
        outputdata.Atoc = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::OUTER,
                                                           SectionPart::Part::CENTER);
        outputdata.Atic = SectionCalculations::calcAt_Wall(inputdata, PlaneSide::Side::INNER,
                                                           SectionPart::Part::CENTER);
    } else if (isSlab) {
        outputdata.At_sUpperEdgeA  = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                      PlaneSide::Side::UPPER, SectionPart::Part::EDGE, PlanePart::Part::A_PART);
        outputdata.At_sUpperCenterA = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                       PlaneSide::Side::UPPER, SectionPart::Part::CENTER, PlanePart::Part::A_PART);
        outputdata.At_sUpperB      = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                      PlaneSide::Side::UPPER, SectionPart::Part::UNDEFINED, PlanePart::Part::B_PART);
        outputdata.At_sLowerEdgeA  = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                      PlaneSide::Side::LOWER, SectionPart::Part::EDGE, PlanePart::Part::A_PART);
        outputdata.At_sLowerCenterA = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                       PlaneSide::Side::LOWER, SectionPart::Part::CENTER, PlanePart::Part::A_PART);
        outputdata.At_sLowerB      = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                      PlaneSide::Side::LOWER, SectionPart::Part::UNDEFINED, PlanePart::Part::B_PART);
        outputdata.At_lUpperEdgeA  = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                      PlaneSide::Side::UPPER, SectionPart::Part::EDGE, PlanePart::Part::A_PART);
        outputdata.At_lUpperCenterA = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                       PlaneSide::Side::UPPER, SectionPart::Part::CENTER, PlanePart::Part::A_PART);
        outputdata.At_lUpperB      = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                      PlaneSide::Side::UPPER, SectionPart::Part::UNDEFINED, PlanePart::Part::B_PART);
        outputdata.At_lLowerEdgeA  = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                      PlaneSide::Side::LOWER, SectionPart::Part::EDGE, PlanePart::Part::A_PART);
        outputdata.At_lLowerCenterA = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                       PlaneSide::Side::LOWER, SectionPart::Part::CENTER, PlanePart::Part::A_PART);
        outputdata.At_lLowerB      = SectionCalculations::calcAt_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                      PlaneSide::Side::LOWER, SectionPart::Part::UNDEFINED, PlanePart::Part::B_PART);

        outputdata.Ma_sUpperEdgeA  = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                      PlaneSide::Side::UPPER, SectionPart::Part::EDGE, PlanePart::Part::A_PART);
        outputdata.Ma_sUpperCenterA = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                       PlaneSide::Side::UPPER, SectionPart::Part::CENTER, PlanePart::Part::A_PART);
        outputdata.Ma_sUpperB      = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                      PlaneSide::Side::UPPER, SectionPart::Part::UNDEFINED, PlanePart::Part::B_PART);
        outputdata.Ma_sLowerEdgeA  = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                      PlaneSide::Side::LOWER, SectionPart::Part::EDGE, PlanePart::Part::A_PART);
        outputdata.Ma_sLowerCenterA = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                       PlaneSide::Side::LOWER, SectionPart::Part::CENTER, PlanePart::Part::A_PART);
        outputdata.Ma_sLowerB      = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::SHORTSIDE,
                                                                      PlaneSide::Side::LOWER, SectionPart::Part::UNDEFINED, PlanePart::Part::B_PART);
        outputdata.Ma_lUpperEdgeA  = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                      PlaneSide::Side::UPPER, SectionPart::Part::EDGE, PlanePart::Part::A_PART);
        outputdata.Ma_lUpperCenterA = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                       PlaneSide::Side::UPPER, SectionPart::Part::CENTER, PlanePart::Part::A_PART);
        outputdata.Ma_lUpperB      = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                      PlaneSide::Side::UPPER, SectionPart::Part::UNDEFINED, PlanePart::Part::B_PART);
        outputdata.Ma_lLowerEdgeA  = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                      PlaneSide::Side::LOWER, SectionPart::Part::EDGE, PlanePart::Part::A_PART);
        outputdata.Ma_lLowerCenterA = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                       PlaneSide::Side::LOWER, SectionPart::Part::CENTER, PlanePart::Part::A_PART);
        outputdata.Ma_lLowerB      = SectionCalculations::calcMa_Slab(inputdata, PlaneDirection::Direction::LONGSIDE,
                                                                      PlaneSide::Side::LOWER, SectionPart::Part::UNDEFINED, PlanePart::Part::B_PART);
    }
    return outputdata;
}

QList<QList<QStringList> > SectionLineCalculate::formatResult(LineOutputData outputdata[])
{
    QVector<QList<QStringList> > results;
    int loop = 0;
    if (isColumn) {
        loop = 2;
    } else if (isGirder) {
        loop = 3;
    } else if (isBeam) {
        loop = 3;
    } else if (isBrace) {
        loop = 3;
    }
    results.resize(loop);

    const double EPS = 0.00001;
    QList<LinePartsParameter> inputdata;
    if (isColumn) {
        inputdata << mySection.i_param << mySection.j_param;
    } else if (isGirder) {
        inputdata << mySection.i_param << mySection.c_param << mySection.j_param;
    } else if (isBeam) {
        inputdata << mySection.i_param << mySection.c_param << mySection.j_param;
    } else if (isBrace) {
        inputdata << mySection.i_param << mySection.c_param << mySection.j_param;
    }

    if (isS) {
        for (int i = 0; i < loop; ++i) {
            if (outputdata[i].A > EPS) {
                results[i] << QStringList{u8"断面積", "A", "=", QString("%1").arg(outputdata[i].A / 100, 0, 'f', 2), "cm2"};
            }
            if ( isColumn || (isBrace && inputdata[i].steelType != SteelType::Type::ST_P) ) {
                if (outputdata[i].Ae > EPS) {
                    results[i] << QStringList{u8"有効断面積", "Ae", "=", QString("%1").arg(outputdata[i].Ae / 100, 0, 'f', 2), "cm2"};
                }
            }
            if (outputdata[i].Ix > EPS) {
                results[i] << QStringList{u8"断面二次M", "Ix", "=", QString("%1").arg(outputdata[i].Ix / 10000, 0,'f', 0), "cm4"};
            }
            if (outputdata[i].Iy > EPS) {
                results[i] << QStringList{u8"断面二次M", "Iy", "=", QString("%1").arg(outputdata[i].Iy / 10000, 0,'f', 0), "cm4"};
            }
            if (outputdata[i].Zx > EPS) {
                results[i] << QStringList{u8"断面係数", "Zx", "=", QString("%1").arg(outputdata[i].Zx / 1000, 0, 'f', 2), "cm3"};
            }
            if (outputdata[i].Zy > EPS) {
                results[i] << QStringList{u8"断面係数", "Zy", "=", QString("%1").arg(outputdata[i].Zy / 1000, 0, 'f', 2), "cm3"};
            }
            if (isGirder) {
                if (outputdata[i].Zex > EPS) {
                    results[i] << QStringList{u8"有効断面係数", "Zex", "=", QString("%1").arg(outputdata[i].Zex / 1000, 0, 'f', 2), "cm3"};
                }
                if (outputdata[i].Zey > EPS) {
                    results[i] << QStringList{u8"有効断面係数", "Zey", "=", QString("%1").arg(outputdata[i].Zey / 1000, 0, 'f', 2), "cm3"};
                }
            }
            if (outputdata[i].Zpx > EPS) {
                results[i] << QStringList{u8"塑性断面係数", "Zpx", "=", QString("%1").arg(outputdata[i].Zpx / 1000,0, 'f', 2), "cm3"};
            }
            if (outputdata[i].Zpy > EPS) {
                results[i] << QStringList{u8"塑性断面係数", "Zpy", "=", QString("%1").arg(outputdata[i].Zpy / 1000, 0, 'f', 2), "cm3"};
            }
            if (outputdata[i].Srsx > EPS) {
                results[i] << QStringList{u8"断面二次半径", "ix", "=", QString("%1").arg(outputdata[i].Srsx / 10, 0, 'f', 2), "cm"};
            }
            if (outputdata[i].Srsy > EPS) {
                results[i] << QStringList{u8"断面二次半径", "iy", "=", QString("%1").arg(outputdata[i].Srsy / 10, 0, 'f', 2), "cm"};
            }
            if (outputdata[i].rankF != SectionRank::Rank::UNDEFINED) {
                //H or BXのみ
                results[i] << QStringList{u8"幅厚比（フランジ）", "b/tf", "=", QString("%1").arg(outputdata[i].wtratiof, 0, 'f', 2), ""};
            }
            if (outputdata[i].rankW != SectionRank::Rank::UNDEFINED) {
                //H or BXのみ
                results[i] << QStringList{u8"幅厚比（ウェブ）", "h/tw", "=", QString("%1").arg(outputdata[i].wtratiow, 0, 'f', 2), ""};
            }
            if (outputdata[i].rankF != SectionRank::Rank::UNDEFINED) {
                //H or BXのみ
                results[i] << QStringList{u8"ランク（フランジ）", "rankf", "=", SectionRank::enumToString(outputdata[i].rankF), ""};
            }
            if (outputdata[i].rankW != SectionRank::Rank::UNDEFINED) {
                //H or BXのみ
                results[i] << QStringList{u8"ランク（ウェブ）", "rankw", "=", SectionRank::enumToString(outputdata[i].rankW), ""};
            }
            if (outputdata[i].rankSN != SectionRank::Rank::UNDEFINED) {
                //SN-Hのみ
                results[i] << QStringList{u8"ランク（H鋼SN材）", "rank", "=", SectionRank::enumToString(outputdata[i].rankSN), ""};
            }
            if (outputdata[i].slendernessRatio > EPS) {
                //直接入力時のみ
                results[i] << QStringList{u8"細長比", u8"λ", "=", QString("%1").arg(outputdata[i].slendernessRatio, 0, 'f', 2), ""};
            }
            if (outputdata[i].unitmass > EPS) {
                //直接入力時のみ
                results[i] << QStringList{u8"単位重量", "W", "=", QString("%1").arg(outputdata[i].unitmass, 0, 'f', 2), "N/m"};
            }
        }
    } else if (isRC || isSRC) {
        for (int i = 0; i < loop; ++i) {
            if (outputdata[i].A > EPS) {
                results[i] << QStringList{u8"断面積", "A", "=", QString("%1").arg(outputdata[i].A / 100, 0, 'f', 2), "cm2"};
            }
            if (outputdata[i].As > EPS) {
                results[i] << QStringList{u8"断面積(S)", "As", "=", QString("%1").arg(outputdata[i].As / 100, 0, 'f', 2), "cm2"};
            }
            if (outputdata[i].Aeq > EPS) {
                results[i] << QStringList{u8"等価断面積", "Aeq", "=", QString("%1").arg(outputdata[i].Aeq / 100, 0, 'f', 2), "cm2"};
            }
            if (outputdata[i].Ix > EPS) {
                results[i] << QStringList{u8"断面二次M", "Ix", "=", QString("%1").arg(outputdata[i].Ix / 10000, 0, 'f', 0), "cm4"};
            }
            if (outputdata[i].Iy > EPS) {
                results[i] << QStringList{u8"断面二次M", "Iy", "=", QString("%1").arg(outputdata[i].Iy / 10000, 0, 'f', 0), "cm4"};
            }
            if (outputdata[i].Isx > EPS) {
                results[i] << QStringList{u8"断面二次M(S)", "Isx", "=", QString("%1").arg(outputdata[i].Isx / 10000, 0, 'f', 0), "cm4"};
            }
            if (outputdata[i].Isy > EPS) {
                results[i] << QStringList{u8"断面二次M(S)", "Isy", "=", QString("%1").arg(outputdata[i].Isy / 10000, 0, 'f', 0), "cm4"};
            }
            if (outputdata[i].Ieqx > EPS) {
                results[i] << QStringList{u8"等価断面二次M", "Ieqx", "=", QString("%1").arg(outputdata[i].Ieqx / 10000, 0, 'f', 0), "cm4"};
            }
            if (outputdata[i].Ieqy > EPS) {
                results[i] << QStringList{u8"等価断面二次M", "Ieqy", "=", QString("%1").arg(outputdata[i].Ieqy / 10000, 0, 'f', 0), "cm4"};
            }
            if (isColumn && isRC && outputdata[i].pg > EPS) {
                results[i] << QStringList{u8"軸鉄筋比", "pg", "=", QString("%1").arg(outputdata[i].pg * 100, 0, 'f', 2), u8"％"};
            }
            if (isColumn && isSRC && outputdata[i].pg > EPS) {
                results[i] << QStringList{u8"軸鉄筋/鉄骨比", "pg", "=", QString("%1").arg(outputdata[i].pg * 100, 0, 'f', 2), u8"％"};
            }
            if (inputdata[i].concType == ConcreteType::Type::RECTANGLE) {
                if (outputdata[i].pt1 > EPS) {
                    results[i] << QStringList{u8"引張鉄筋比", "pt_l", "=", QString("%1").arg(outputdata[i].pt1 * 100, 0, 'f', 2), u8"％"};
                }
                if (outputdata[i].pt2 > EPS) {
                    results[i] << QStringList{u8"引張鉄筋比", "pt_r", "=", QString("%1").arg(outputdata[i].pt2 * 100, 0, 'f', 2), u8"％"};
                }
                if (outputdata[i].pt3 > EPS) {
                    results[i] << QStringList{u8"引張鉄筋比", "pt_u", "=", QString("%1").arg(outputdata[i].pt3 * 100, 0, 'f', 2), u8"％"};
                }
                if (outputdata[i].pt4 > EPS) {
                    results[i] << QStringList{u8"引張鉄筋比", "pt_b", "=", QString("%1").arg(outputdata[i].pt4 * 100, 0, 'f', 2), u8"％"};
                }
                if (outputdata[i].pwx > EPS) {
                    results[i] << QStringList{u8"フープ筋比", "pwx", "=", QString("%1").arg(outputdata[i].pwx * 100, 0, 'f', 2), u8"％"};
                }
                if (outputdata[i].pwy > EPS) {
                    results[i] << QStringList{u8"フープ筋比", "pwy", "=", QString("%1").arg(outputdata[i].pwy * 100, 0, 'f', 2), u8"％"};
                }
            }
        }
    } else if (isCFT) {
        for (int i = 0; i < loop; ++i) {
            switch (inputdata[i].steelType) {
            case SteelType::Type::ST_BX :
            case SteelType::Type::ST_P  :
                if (outputdata[i].A > EPS) {
                    results[i] << QStringList{u8"断面積", "A", "=", QString("%1").arg(outputdata[i].A / 100, 0, 'f', 2), "cm2"};
                }
                if (outputdata[i].As > EPS) {
                    results[i] << QStringList{u8"断面積(S)", "As", "=", QString("%1").arg(outputdata[i].As / 100, 0, 'f', 2), "cm2"};
                }
                if (outputdata[i].Aeq > EPS) {
                    results[i] << QStringList{ u8"等価断面積", "Aeq", "=", QString("%1").arg(outputdata[i].Aeq / 100, 0, 'f', 2), "cm2"};
                }
                if (outputdata[i].Ix > EPS) {
                    results[i] << QStringList{u8"断面二次M", "Ix", "=", QString("%1").arg(outputdata[i].Ix / 10000, 0, 'f', 0), "cm4"};
                }
                if (outputdata[i].Iy > EPS) {
                    results[i] << QStringList{u8"断面二次M", "Iy", "=", QString("%1").arg(outputdata[i].Iy / 10000, 0, 'f', 0), "cm4"};
                }
                if (outputdata[i].Isx > EPS) {
                    results[i] << QStringList{u8"断面二次M(S)", "Isx", "=", QString("%1").arg(outputdata[i].Isx / 10000, 0, 'f', 0), "cm4"};
                }
                if (outputdata[i].Isy > EPS) {
                    results[i] << QStringList{u8"断面二次M(S)", "Isy", "=", QString("%1").arg(outputdata[i].Isy / 10000, 0, 'f', 0), "cm4"};
                }
                if (outputdata[i].Ieqx > EPS) {
                    results[i] << QStringList{u8"等価断面二次M", "Ieqx", "=", QString("%1").arg(outputdata[i].Ieqx / 10000, 0, 'f', 0), "cm4"};
                }
                if (outputdata[i].Ieqy > EPS) {
                    results[i] << QStringList{u8"等価断面二次M", "Ieqy", "=", QString("%1").arg(outputdata[i].Ieqy / 10000, 0, 'f', 0), "cm4"};
                }
                break;
//            case SteelType::Type::ST_BCP :
//            case SteelType::Type::ST_BCR :
//                results[i]<<(tmp<<tr("未実装")<<""<<""<<""<<"");tmp.clear();
//                break;
            default:
                results[i] << QStringList{"Invalid Data", "", "", "", ""};
            }
        }
    }

    QList<QList<QStringList> > retresults;
    if     (isColumn) {
        retresults << results[0] << results[1];
    } else if (isGirder) {
        retresults << results[0] << results[1] << results[2];
    } else if (isBeam  ) {
        retresults << results[0] << results[1] << results[2];
    } else if (isBrace ) {
        retresults << results[0] << results[1] << results[2];
    }

    return retresults;
}
QList<QList<QStringList> > SectionLineCalculate::formatResult(PlaneOutputData outputdata[])
{
//    PlaneMemberSection myPlaneSection = dynamic_cast<PlaneMemberSection>(mySection);
    QVector<QList<QStringList>> results;
    int loop = 0;
    if     (isWall || isUWall) {
        loop = 1;
    } else if (isSlab) {
        loop = 1;
    }
    results.resize(loop);

    const double EPS = 0.00001;

    if (isWall) {
        for (int i = 0; i < loop; ++i) {
            if (outputdata[i].Psv > EPS) {
                results[i] << QStringList{u8"鉄筋比（縦方向）", "Psv", "=", QString("%1").arg(outputdata[i].Psv * 100, 0, 'f', 2), u8"％"};
            }
            if (outputdata[i].Psh > EPS) {
                results[i] << QStringList{u8"鉄筋比（横方向）", "Psh", "=", QString("%1").arg(outputdata[i].Psh * 100, 0, 'f', 2), u8"％"};
            }
            if (outputdata[i].Arvs > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（小開口 縦）", "Asv", "=", QString("%1").arg(outputdata[i].Arvs / 100, 0, 'f', 2), "cm2"};
            }
            if (outputdata[i].Arhs > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（小開口 横）", "Ash", "=", QString("%1").arg(outputdata[i].Arhs / 100, 0, 'f', 2), "cm2"};
            }
            if (outputdata[i].Ards > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（小開口 斜）", "Asd", "=", QString("%1").arg(outputdata[i].Ards / 100, 0, 'f', 2), "cm2"};
            }
            if (outputdata[i].Arvl > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（大開口 縦）", "Asv", "=", QString("%1").arg(outputdata[i].Arvl / 100, 0, 'f', 2), "cm2"};
            }
            if (outputdata[i].Arhl > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（大開口 横）", "Ash", "=", QString("%1").arg(outputdata[i].Arhl / 100, 0, 'f', 2), "cm2"};
            }
            if (outputdata[i].Ardl > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（大開口 斜）", "Asd", "=", QString("%1").arg(outputdata[i].Ardl / 100, 0, 'f', 2), "cm2"};
            }
        }
    } else if (isUWall) {
        for (int i = 0; i < loop; ++i) {
            if (outputdata[i].Psv > EPS) {
                results[i] << QStringList{u8"鉄筋比（標準配筋　縦）", "Psv", "=", QString("%1").arg(outputdata[i].Psv * 100, 0, 'f', 2), u8"％"};
            }
            if (outputdata[i].Psh > EPS) {
                results[i] << QStringList{u8"鉄筋比（標準配筋　横）", "Psh", "=", QString("%1").arg(outputdata[i].Psh * 100, 0, 'f', 2), u8"％"};
//            }if(outputdata[i].Atou>EPS){
//                tmp<<tr("鉄筋断面積（外側上部）")<<"Asv"<<"="<<QString("%1").arg(outputdata[i].Atou/100,0,'f',2)<<"cm2/m";
//                results[i]<<tmp;tmp.clear();
//            }if(outputdata[i].Atom>EPS){
//                tmp<<tr("鉄筋断面積（外側中部）")<<"Asv"<<"="<<QString("%1").arg(outputdata[i].Atom/100,0,'f',2)<<"cm2/m";
//                results[i]<<tmp;tmp.clear();
            }
            if (outputdata[i].Atob > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（外側下部）", "Asv", "=", QString("%1").arg(outputdata[i].Atob / 100, 0, 'f', 2), "cm2/m"};
//            }if(outputdata[i].Atiu>EPS){
//                tmp<<tr("鉄筋断面積（内側上部）")<<"Asv"<<"="<<QString("%1").arg(outputdata[i].Atiu/100,0,'f',2)<<"cm2/m";
//                results[i]<<tmp;tmp.clear();
//            }if(outputdata[i].Atim>EPS){
//                tmp<<tr("鉄筋断面積（内側中部）")<<"Asv"<<"="<<QString("%1").arg(outputdata[i].Atim/100,0,'f',2)<<"cm2/m";
//                results[i]<<tmp;tmp.clear();
//            }if(outputdata[i].Atil>EPS){
//                tmp<<tr("鉄筋断面積（内側下部）")<<"Asv"<<"="<<QString("%1").arg(outputdata[i].Atil/100,0,'f',2)<<"cm2/m";
//                results[i]<<tmp;tmp.clear();
            }
            if (outputdata[i].Atoe > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（外側端部）", "Ash", "=", QString("%1").arg(outputdata[i].Atoe / 100, 0, 'f', 2), "cm2/m"};
//            }if(outputdata[i].Atoc>EPS){
//                tmp<<tr("鉄筋断面積（外側中央）")<<"Ash"<<"="<<QString("%1").arg(outputdata[i].Atoc/100,0,'f',2)<<"cm2/m";
//                results[i]<<tmp;tmp.clear();
//            }if(outputdata[i].Atie>EPS){
//                tmp<<tr("鉄筋断面積（内側端部）")<<"Ash"<<"="<<QString("%1").arg(outputdata[i].Atie/100,0,'f',2)<<"cm2/m";
//                results[i]<<tmp;tmp.clear();
//            }if(outputdata[i].Atic>EPS){
//                tmp<<tr("鉄筋断面積（内側中央）")<<"Ash"<<"="<<QString("%1").arg(outputdata[i].Atic/100,0,'f',2)<<"cm2/m";
//                results[i]<<tmp;tmp.clear();
            }
        }
    } else if (isSlab) { // && !mySection.isCantilever){
        for (int i = 0; i < loop; ++i) {
            if (outputdata[i].At_sUpperEdgeA > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（短辺A端部上端）", "At", "=", QString("%1").arg(outputdata[i].At_sUpperEdgeA / 100, 0, 'f', 2), "cm2/m"};
            }
            if (outputdata[i].At_sLowerCenterA > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（短辺A中央下端）", "At", "=", QString("%1").arg(outputdata[i].At_sLowerCenterA / 100, 0, 'f', 2), "cm2/m"};
            }
            if (outputdata[i].At_lUpperEdgeA > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（長辺A端部上端）", "At", "=", QString("%1").arg(outputdata[i].At_lUpperEdgeA / 100, 0, 'f', 2), "cm2/m"};
            }
            if (outputdata[i].At_lLowerCenterA > EPS) {
                results[i] << QStringList{u8"鉄筋断面積（長辺A中央下端）", "At", "=", QString("%1").arg(outputdata[i].At_lLowerCenterA / 100, 0, 'f', 2), "cm2/m"};
            }
            if (outputdata[i].Ma_sUpperEdgeA > EPS) {
                results[i] << QStringList{u8"許容曲げM（短辺A端部上端）", "Ma", "=", QString("%1").arg(outputdata[i].Ma_sUpperEdgeA / 1000000, 0, 'f', 2), "kNm/m"};
            }
            if (outputdata[i].Ma_sLowerCenterA > EPS) {
                results[i] << QStringList{u8"許容曲げM（短辺A中央下端）", "Ma", "=", QString("%1").arg(outputdata[i].Ma_sLowerCenterA / 1000000, 0, 'f', 2), "kNm/m"};
            }
            if (outputdata[i].Ma_lUpperEdgeA > EPS) {
                results[i] << QStringList{u8"許容曲げM（長辺A端部上端）", "Ma", "=", QString("%1").arg(outputdata[i].Ma_lUpperEdgeA / 1000000, 0, 'f', 2), "kNm/m"};
            }
            if (outputdata[i].Ma_lLowerCenterA > EPS) {
                results[i] << QStringList{u8"許容曲げM（長辺A中央下端）", "Ma", "=", QString("%1").arg(outputdata[i].Ma_lLowerCenterA / 1000000, 0, 'f', 2), "kNm/m"};
            }
        }
//        qDebug() << "slab calc";
        /*
            }else if(isSlab && mySection.isCantilever){
                for(int i=0;i<loop;++i){
                    if(outputdata[i].At_sUpperEdgeA>EPS){
                        tmp<<tr("鉄筋断面積（元端上端）")<<"At"<<"="<<QString("%1").arg(outputdata[i].At_sUpperEdgeA/100,0,'f',2)<<tr("cm2/m");
                        results[i]<<tmp;tmp.clear();
                    }
                    if(outputdata[i].Ma_sUpperEdgeA>EPS){
                        tmp<<tr("許容曲げM（元端上端）")<<"Ma"<<"="<<QString("%1").arg(outputdata[i].Ma_sUpperEdgeA/1000000,0,'f',2)<<tr("kN・m/m");
                        results[i]<<tmp;tmp.clear();
                    }
                }
        */
    }

    QList<QList<QStringList>> retresults;
    if (isWall || isUWall ) {
        retresults << results[0];
    }
    if (isSlab ) {
        retresults << results[0];
    }

    return retresults;
}

QList<QList<QStringList> > SectionLineCalculate::isolatorFormatResult()
{
    if (!isIsolator) return QList<QList<QStringList> >();

    QVector<QList<QStringList> > results;
    int loop = 2;
    results.resize(loop);

    const double EPS = 0.00001;

    if (qAbs(mySection.hspring_param.K1) > EPS) {
        results[0] << QStringList{u8"初期剛性初期剛性", "K1", "=", QString("%1").arg(mySection.hspring_param.K1, 0, 'f', 2), "kN/m"};
    }
    if (qAbs(mySection.hspring_param.K2) > EPS) {
        results[0] << QStringList{u8"二次剛性", "K2", "=", QString("%1").arg(mySection.hspring_param.K2, 0,'f', 2), "kN/m"};
    }
    if (qAbs(mySection.hspring_param.K3) > EPS) {
        results[0] << QStringList{u8"三次剛性", "K3", "=", QString("%1").arg(mySection.hspring_param.K3, 0, 'f', 2), "kN/m"};
    }
    if (qAbs(mySection.hspring_param.Fy1p) > EPS) {
        results[0] << QStringList{u8"降伏耐力", "fy1p", "=", QString("%1").arg(mySection.hspring_param.Fy1p, 0, 'f', 2), "kN"};
    }
    if (qAbs(mySection.hspring_param.Fy1m) > EPS) {
        results[0] << QStringList{u8"降伏耐力", "fy1m", "=", QString("%1").arg(mySection.hspring_param.Fy1m, 0, 'f', 2), "kN"};
    }
    if (qAbs(mySection.hspring_param.Fy2p) > EPS) {
        results[0] << QStringList{u8"降伏耐力", "fy2p", "=", QString("%1").arg(mySection.hspring_param.Fy2p, 0, 'f', 2), "kN"};
    }
    if (qAbs(mySection.hspring_param.Fy2m) > EPS) {
        results[0] << QStringList{u8"降伏耐力", "fy2m", "=", QString("%1").arg(mySection.hspring_param.Fy2m, 0, 'f', 2), "kN"};
    }
    if (qAbs(mySection.vspring_param.K1) > EPS) {
        results[0] << QStringList{u8"鉛直剛性　圧縮", "Kvc", "=", QString("%1").arg(mySection.vspring_param.K1, 0, 'f', 2), "kN/m"};
    }
    if (qAbs(mySection.vspring_param.K2) > EPS) {
        results[0] << QStringList{u8"鉛直剛性　引張", "Kvt", "=", QString("%1").arg(mySection.vspring_param.K2, 0, 'f', 2), "kN/m"};
    }

    QString pname = mySection.isolator_param.productName;
    if ( !pname.isEmpty() && SectionDataType::standardSectionNameExists(pname) ) {
        results[1] << QStringList{u8"型番", ":", "", pname, ""};

        QString var = SectionDataType::standardIsolatorSectionValue(pname, StandardSectionTable::IsolatorFormat::IS_PN);
        if ( !var.isEmpty() ) {
            results[1] << QStringList{u8"メーカー名", ":", "", var, ""};
        }
        var = SectionDataType::standardIsolatorSectionValue(pname, StandardSectionTable::IsolatorFormat::IS_G);
        if ( !var.isEmpty() ) {
            results[1] << QStringList{u8"せん断弾性率", "G", "=", var, "N/mm2"};
        }
        var = SectionDataType::standardIsolatorSectionValue(pname, StandardSectionTable::IsolatorFormat::IS_R);
        if ( !var.isEmpty() ) {
            results[1] << QStringList{u8"ゴム外径", "Rr", "=", var, "mm"};
        }
        var = SectionDataType::standardIsolatorSectionValue(pname, StandardSectionTable::IsolatorFormat::IS_PR);
        if ( !var.isEmpty() ) {
            results[1] << QStringList{u8"鉛プラグ径", "Rp", "=", var, "mm"};
        }
        var = SectionDataType::standardIsolatorSectionValue(pname, StandardSectionTable::IsolatorFormat::IS_t);
        if ( !var.isEmpty() ) {
            results[1] << QStringList{u8"ゴム総厚", "t", "=", var, "mm"};
        }
        var = SectionDataType::standardIsolatorSectionValue(pname, StandardSectionTable::IsolatorFormat::IS_Fr);
        if ( !var.isEmpty() ) {
            results[1] << QStringList{u8"摩擦係数", "Fr", "=", var, ""};
        }
    }

    QList<QList<QStringList> > retresults;
    retresults << results[0] << results[1];

    return retresults;
}

void SectionLineCalculate::drawResult(const QList<QList<QStringList> > &foutp,
                                      QPainter *resultpaint)
{
//    LineMemberSection myLineSection;
//    if(isColumn || isBeam || isGirder || isBrace)
//        myLineSection=dynamic_cast<LineMemberSection>(mySection);

    //not constant
    int h = 20; //rowheight
    int ht = 20; //titleheight
    int ml = 20; //marginleft
    int mt = 0; //margintop
    int w1 = 95; //colwidth　名称
    int w2 = 45; //記号
    int w3 = 20; //イコール
    int w4 = 80; //値
    int w5 = 30; //単位
    int fgc = Qt::AlignHCenter;
    int fgl = Qt::AlignLeft;
    int fgr = Qt::AlignRight;
    int cwid = 290; //between large col

    int loop = 0;
    QStringList label;
    if (isColumn && !table3input) {
        loop = 1;
        label << u8"全断面";
    } else if (isColumn && table3input && !mySection.line_param.isCantilever) {
        loop = 2;
        label << u8"柱脚" << u8"柱頭";
    } else if (isColumn && table3input && mySection.line_param.isCantilever) {
        loop = 2;
        label << u8"元端" << u8"先端" ;
    } else if (isGirder && !table3input && mySection.line_param.isCantilever) {
        loop = 1;
        label << u8"全断面";
    } else if (isGirder && table3input && mySection.line_param.isCantilever) {
        loop = 1;
        label << u8"元端" << u8"先端";
    } else if (isGirder && !table3input && !table4input && !mySection.line_param.isCantilever) {
        loop = 1;
        label << u8"全断面";
    } else if (isGirder && !table3input && table4input && !mySection.line_param.isCantilever) {
        loop = 2;
        label << u8"左端中央" << u8"右端";
    } else if (isGirder && table3input && !table4input && !mySection.line_param.isCantilever) {
        loop = 2;
        label << u8"両端" << u8"中央";
    } else if (isGirder && table3input && table4input && !mySection.line_param.isCantilever) {
        loop = 3;
        label << u8"左端" << u8"中央" << u8"右端";
    } else if (isBeam && !table3input && mySection.line_param.isCantilever) {
        loop = 1;
        label << u8"全断面";
    } else if (isBeam && table3input && mySection.line_param.isCantilever) {
        loop = 1;
        label << u8"元端" << u8"先端";
    } else if (isBeam && !table3input && !table4input && !mySection.line_param.isCantilever) {
        loop = 1;
        label << u8"全断面";
    } else if (isBeam && !table3input && table4input && !mySection.line_param.isCantilever) {
        loop = 2;
        label << u8"左端中央" << u8"右端";
    } else if (isBeam && table3input && !table4input && !mySection.line_param.isCantilever) {
        loop = 2;
        label << u8"両端" << u8"中央";
    } else if (isBeam && table3input && table4input && !mySection.line_param.isCantilever) {
        loop = 3;
        label << u8"左端" << u8"中央" << u8"右端";
    } else if (isBrace && !table3input && !table4input) {
        loop = 1;
        label << u8"全断面";
    } else if (isBrace && !table3input && table4input) {
        loop = 2;
        label << u8"I端中央" << u8"J端";
    } else if (isBrace && table3input && !table4input) {
        loop = 2;
        label << u8"両端" << u8"中央";
    } else if (isBrace && table3input && table4input) {
        loop = 3;
        label << u8"I端" << u8"中央" << u8"J端";
    } else if (isWall || isUWall) {
        loop = 1;
        label << u8"断面情報";
        w1 += 35;
        w4 -= 20;
        w5 += 20;
        cwid += 35;
    } else if (isSlab) {
        loop = 1;
        label << u8"断面情報";
//        w1+=75;w2-=10;w4-=30;w5+=20;cwid+=55;
        w1 += 75;
        w2 -= 10;
        w4 -= 10;
        w5 += 35;
        cwid += 90;
    } else if (isIsolator) {
        loop = 2;
        label << u8"ばね特性" << u8"製品情報";
        w4 += 40;
        w5 += 20;
        cwid += 60;
    }

    resultpaint->setPen(Qt::NoPen);
    resultpaint->setBrush(QBrush(QColor(200, 220, 200)));
    resultpaint->drawRect(0, 0, 800, mt + ht);
    for (int row = 0; row < 25; ++row) {
        resultpaint->setPen(Qt::NoPen);
        resultpaint->setBrush(QBrush(QColor(220, 220, 220)));
        if (row % 2 == 0)resultpaint->drawRect(0, ht + mt + row * h, 800, h);
    }
    resultpaint->setPen(Qt::SolidLine);
    resultpaint->setBrush(Qt::NoBrush);
    QFont headerfont;
    headerfont.setPointSize(10);
    for (int col = 0; col < loop; ++col) {
        resultpaint->setFont(headerfont);
        resultpaint->drawText(col * cwid + ml, mt, w1 + w2 + w3 + w4 + w5, ht, fgc, label[col]);
        resultpaint->setFont(QFont());
        for (int row = 0; row < foutp[col].size(); ++row) {
            resultpaint->drawText(col * cwid + ml, ht + mt + row * h + 4, w1, h, fgl, foutp[col][row][0]);
            resultpaint->drawText(col * cwid + ml + w1, ht + mt + row * h + 4, w2, h, fgc, foutp[col][row][1]);
            resultpaint->drawText(col * cwid + ml + w1 + w2, ht + mt + row * h + 4, w3, h, fgl,
                                  foutp[col][row][2]);
            resultpaint->drawText(col * cwid + ml + w1 + w2 + w3, ht + mt + row * h + 4, w4, h, fgr,
                                  foutp[col][row][3]);
            resultpaint->drawText(col * cwid + ml + w1 + w2 + w3 + w4, ht + mt + row * h + 4, w5, h, fgc,
                                  foutp[col][row][4]);
        }
    }
    for (int col = 1; col < loop; ++col) {
        resultpaint->setPen(Qt::gray);
        int betweencol = ml + (col - 1) * cwid + (cwid + w1 + w2 + w3 + w4 + w5) / 2;
        resultpaint->drawLine(betweencol, 0, betweencol, 600);
    }
}

} // namespace post3dapp


