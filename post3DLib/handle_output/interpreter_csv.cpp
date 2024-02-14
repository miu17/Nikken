#include "interpreter_csv.h"

#include "calc_vector2d.h"
#include "define_unifieddata.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_inputdata.h"
#include "unified_project.h"

namespace post3dapp{

InterpreterCsv::InterpreterCsv()
{
    uuidTable.insert(QUuid(), 0);
}


void InterpreterCsv::makeDefinedData() const
{
    UnifiedInputData::getInInstance()->clearDefinedData();

    UnifiedInputData::getInInstance()->registerTempData(sortedFloors);
    UnifiedInputData::getInInstance()->registerAnalysisFloorData(AnalysisFloors, baseFloor, roofFloor);
    UnifiedInputData::getInInstance()->registerTempData(framegroups, frames);
    UnifiedInputData::getInInstance()->registerTempData(materials);

    QList<SectionValues> columnValues;
    columnValues.append(rcColumnList);
    columnValues.append(sColumnList);
    columnValues.append(srcColumnList);
    columnValues.append(cftColumnList);
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPCOLUMN, columnValues);

    QList<SectionValues> girderValues;
    girderValues.append(rcGirderList);
    girderValues.append(sGirderList);
    girderValues.append(srcGirderList);
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPGIRDER, girderValues);

    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPBRACE, sBraceList);
    QList<SectionValues> beamValues;
    beamValues.append(rcBeamList);
    beamValues.append(sBeamList);
    beamValues.append(srcBeamList);
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPBEAM, beamValues);
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPSLAB, rcSlabList);
    QList<SectionValues> wallValues;
    wallValues.append(rcWallList);
    wallValues.append(sWallList);
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPWALL, wallValues);
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPUWALL, rcUwallList);
    QList<SectionValues> baseValues;
    baseValues.append(sBaseList);
    baseValues.append(srcBaseList);
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPBASE, baseValues);


    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPWALLOPEN, wallOpenList);
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPDAMPER, damperList);
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPISO, isolatorList);

    UnifiedInputData::getInInstance()->registerTempData(jointLoadList);
    UnifiedInputData::getInInstance()->registerTempData(memberLoadList);
    UnifiedInputData::getInInstance()->registerTempData(slabLoadList);

    UnifiedProject::getInstance()->registerTempAnalyisisData(gene, analysisCondition);

}


QUuid InterpreterCsv::getOrCreateUuid(QString id)
{
    if (!uuidTable.values().contains(id)){
        QUuid newOne = QUuid::createUuid();
        uuidTable.insert(newOne, id);
        return newOne;
    }
    else{
        return uuidTable.key(id);
    }

}

FRAMEPOINTSVALUE InterpreterCsv::CalculateFramePointsValue(const FRAMEGROUPVALUE &fgv, double& totalspan, const QString& mySpan, bool isMm, bool& isFirst, const QString& name, const QUuid& uuid) const
{
    QList<QPointF> plist;
    STRAIGHTFRAMEVALUE st_value;
    QPointF npoint;
    if (fgv.frameType == FrameType::FRAMETYPE::STRAIGHT){
        QPointF po = fgv.basePoint;
        qreal theta = fgv.startAngle / 180.0 * M_PI;
        qreal p_len = fgv.plusLength;
        qreal m_len = fgv.minusLength;
        QPointF p1, p2, p3, p4;
        QPointF spoint;
        qreal m_span = isMm ? mySpan.toDouble() / 1000.0 : mySpan.toDouble();
        qreal s_angle;
        if (fgv.startAngle < 45.0){
            p1 = QPointF(-m_len * qCos(theta) - totalspan * qSin(theta),
                         -m_len * qSin(theta) + totalspan * qCos(theta)) + po;
            p2 = QPointF( p_len * qCos(theta) - totalspan * qSin(theta),
                          p_len * qSin(theta) + totalspan * qCos(theta)) + po;
            npoint = CalcVector2D::extendPoint(p1, p2, 10.0);
            s_angle = fgv.startAngle + 90.;
            if (mySpan == "") {
                p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
                p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                spoint = CalcVector2D::extendPoint(p3, p4, 5.0);
            } else {
                spoint = CalcVector2D::extendPoint(p1, p2, 3.0);
            }
        } else {
            p1 = QPointF(-m_len * qCos(theta) + totalspan * qSin(theta),
                         -m_len * qSin(theta) - totalspan * qCos(theta)) + po;
            p2 = QPointF( p_len * qCos(theta) + totalspan * qSin(theta),
                          p_len * qSin(theta) - totalspan * qCos(theta)) + po;
            npoint = CalcVector2D::extendPoint(p2, p1, 10.0);
            s_angle = fgv.startAngle - 90.;
            if (mySpan == "") {
                p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
                p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                spoint = CalcVector2D::extendPoint(p4, p3, 5.0);
            } else {
                spoint = CalcVector2D::extendPoint(p2, p1, 3.0);
            }
        }
        plist.append(p1);
        plist.append(p2);

        bool isLast = false;
        if (mySpan == ""){
            m_span = totalspan;
            isLast = true;
        }
        st_value = STRAIGHTFRAMEVALUE(m_span, s_angle, spoint, isFirst, isLast);
        totalspan += isMm ? mySpan.toDouble() / 1000.0 : mySpan.toDouble();
        isFirst = false;
        if (isLast){
            totalspan = 0.0;
            isFirst = true;
        }
    }else if (fgv.frameType == FrameType::FRAMETYPE::CIRCLE){
        QPointF po = fgv.basePoint;
        qreal s_angle = fgv.startAngle;
        qreal e_angle = fgv.endAngle;
        int div = fgv.division;
        st_value.span = isMm ? mySpan.toDouble() / 1000.0 - totalspan : mySpan.toDouble() - totalspan;
        totalspan = isMm ? mySpan.toDouble() / 1000.0 : mySpan.toDouble();

        if ( (s_angle < e_angle) && (e_angle - s_angle < 360.0) && div > 0 ) {
            qreal s_theta = s_angle / 180.0 * M_PI;
            qreal e_theta = e_angle / 180.0 * M_PI;
            qreal d_theta = (e_theta - s_theta) / qreal(div);
            for ( int j = 0; j < div + 1; j++ ) {
                qreal j_theta = s_theta + d_theta * qreal(j);
                QPointF p1 = QPointF(po.x() + totalspan * qCos(j_theta), po.y() + totalspan * qSin(j_theta));
                plist.append(p1);
            }
        }

        if (plist.count() > 1)
            npoint = CalcVector2D::extendPoint(plist.at(1), plist.at(0), 3.0);

    }else if (fgv.frameType == FrameType::FRAMETYPE::RADIUS){
        QPointF po = fgv.basePoint;
        qreal angle = fgv.startAngle + totalspan;
        qreal theta = angle / 180.0 * M_PI;
        qreal p_len = fgv.plusLength;

        plist.append(po);
        QPointF p1 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
        plist.append(p1);

        npoint = CalcVector2D::extendPoint(po, p1, 3.0);
        if (mySpan != "")
            totalspan += mySpan.toDouble();
        else
            st_value.isLast = true;
        st_value.angle =  mySpan.toDouble();
    }

    if (uuid == nullptr){
        return FRAMEPOINTSVALUE(fgv.GID, plist, name, npoint, fgv.frameType, st_value, QUuid::createUuid());
    }else{
        return FRAMEPOINTSVALUE(fgv.GID, plist, name, npoint, fgv.frameType, st_value, uuid);
    }
}

} // namespace post3dapp
