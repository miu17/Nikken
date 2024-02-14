#include "manage_sectiondata.h"

#include <QtMath>
#include <QDebug>
#include <QMessageBox>

#include "define_material_struct.h"
#include "factory_unifieddata.h"
#include "unified_data.h"
namespace post3dapp
{
    ManageSectionData::ManageSectionData(UnifiedDataType isout, QObject *parent)
        : QObject(parent), isOutput(isout)
    {
    }

    ManageSectionData::ManageSectionData(const ManageSectionData &section, UnifiedDataType isout, QObject *parent)
        : QObject(parent), isOutput(isout)
    {
        registerTempData(DATATYPE::TPCOLUMN, section.getSectionValues(DATATYPE::TPCOLUMN));
        registerTempData(DATATYPE::TPGIRDER, section.getSectionValues(DATATYPE::TPGIRDER));
        registerTempData(DATATYPE::TPWALL, section.getSectionValues(DATATYPE::TPWALL));
        registerTempData(DATATYPE::TPBRACE, section.getSectionValues(DATATYPE::TPBRACE));
        registerTempData(DATATYPE::TPBEAM, section.getSectionValues(DATATYPE::TPBEAM));
        registerTempData(DATATYPE::TPSLAB, section.getSectionValues(DATATYPE::TPSLAB));
        registerTempData(DATATYPE::TPUWALL, section.getSectionValues(DATATYPE::TPUWALL));
        registerTempData(DATATYPE::TPBASE, section.getSectionValues(DATATYPE::TPBASE));
        registerTempData(DATATYPE::TPDAMPER, section.getSectionValues(DATATYPE::TPDAMPER));
        registerTempData(DATATYPE::TPISO, section.getSectionValues(DATATYPE::TPISO));
        registerTempData(DATATYPE::TPWALLOPEN, section.getSectionValues(DATATYPE::TPWALLOPEN));
    }

    int ManageSectionData::dataCount(DATATYPE type) const
    {
        if (type == DATATYPE::TPCOLUMN)
        {
            return columnNameList.count();
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            return girderNameList.count();
        }
        else if (type == DATATYPE::TPBRACE)
        {
            return braceNameList.count();
        }
        else if (type == DATATYPE::TPBEAM)
        {
            return beamNameList.count();
        }
        else if (type == DATATYPE::TPSLAB)
        {
            return slabNameList.count();
        }
        else if (type == DATATYPE::TPWALL)
        {
            return wallNameList.count();
        }
        else if (type == DATATYPE::TPUWALL)
        {
            return uwallNameList.count();
        }
        else if (type == DATATYPE::TPWALLOPEN)
        {
            return openNameList.count();
        }
        else if (type == DATATYPE::TPBASE)
        {
            return baseNameList.count();
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            return damperNameList.count();
        }
        else if (type == DATATYPE::TPISO)
        {
            return isoNameList.count();
        }
        return 0;
    }

    QStringList ManageSectionData::getNameList(DATATYPE type) const
    {
        if (type == DATATYPE::TPCOLUMN)
        {
            return columnNameList;
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            return girderNameList;
        }
        else if (type == DATATYPE::TPBRACE)
        {
            return braceNameList;
        }
        else if (type == DATATYPE::TPBEAM)
        {
            return beamNameList;
        }
        else if (type == DATATYPE::TPSLAB)
        {
            return slabNameList;
        }
        else if (type == DATATYPE::TPWALL)
        {
            return wallNameList;
        }
        else if (type == DATATYPE::TPUWALL)
        {
            return uwallNameList;
        }
        else if (type == DATATYPE::TPWALLOPEN)
        {
            return openNameList;
        }
        else if (type == DATATYPE::TPBASE)
        {
            return baseNameList;
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            return damperNameList;
        }
        else if (type == DATATYPE::TPISO)
        {
            return isoNameList;
        }
        return QStringList();
    }

    bool ManageSectionData::isExistedName(const QString &str, DATATYPE type) const
    {
        if (type == DATATYPE::TPCOLUMN)
        {
            return columnNameList.contains(str);
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            return girderNameList.contains(str);
        }
        else if (type == DATATYPE::TPBRACE)
        {
            return braceNameList.contains(str);
        }
        else if (type == DATATYPE::TPBEAM)
        {
            return beamNameList.contains(str);
        }
        else if (type == DATATYPE::TPSLAB)
        {
            return slabNameList.contains(str);
        }
        else if (type == DATATYPE::TPWALL)
        {
            return wallNameList.contains(str);
        }
        else if (type == DATATYPE::TPUWALL)
        {
            return uwallNameList.contains(str);
        }
        else if (type == DATATYPE::TPWALLOPEN)
        {
            return openNameList.contains(str);
        }
        else if (type == DATATYPE::TPBASE)
        {
            return baseNameList.contains(str);
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            return damperNameList.contains(str);
        }
        else if (type == DATATYPE::TPISO)
        {
            return isoNameList.contains(str);
        }
        return false;
    }

    LINESHAPE_IN ManageSectionData::sectionNameToLineShape(const UnifiedData &unified, DATATYPE type, const QString &str,
                                                           qreal hh) const
    {
        qDebug() << "sectionNameToLineShape" << (int)type << str;
        if (type == DATATYPE::TPCOLUMN)
        {
            if (!columnNameList.contains(str))
                return LINESHAPE_IN();
            FloorOrderData *cd = columnTable.value(str);
            SectionValues sv = cd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return LINESHAPE_IN(str); // ERROR List
            return judgeRenderShape(DATATYPE::TPCOLUMN, sv);
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            if (!girderNameList.contains(str))
            {
                qDebug() << "ManageSectionData::sectionNameToLineShape()  girderNameList not contain" << str << girderNameList;
                return LINESHAPE_IN();
            }
            FloorOrderData *gd = girderTable.value(str);
            SectionValues sv = gd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
            {
                qDebug() << "ManageSectionData::sectionNameToLineShape()  sectionName is Empty";
                return LINESHAPE_IN(str); // ERROR List
            }
            return judgeRenderShape(DATATYPE::TPGIRDER, sv);
        }
        else if (type == DATATYPE::TPBRACE)
        {
            if (!braceNameList.contains(str))
                return LINESHAPE_IN();
            SectionValues sv = braceTable.value(str);
            return judgeRenderShape(DATATYPE::TPBRACE, sv);
        }
        else if (type == DATATYPE::TPBEAM)
        {
            if (!beamNameList.contains(str))
                return LINESHAPE_IN();
            SectionValues sv = beamTable.value(str);
            return judgeRenderShape(DATATYPE::TPBEAM, sv);
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            if (!damperNameList.contains(str))
                return LINESHAPE_IN();
            return LINESHAPE_IN(str, QString(), RENDERSHAPE::SHAPE_PIPE, 300.0, 300.0, 300.0, 300.0, 300.0, 300.0,
                                PARTINGLEN::PART_FULL, PARTINGRULE::PART_RATIO, 1500, 1500, 1.0, 0.0);
            //        SectionValues sv=damperTable.value(str);
            //        return judgeRenderShape(sv);
        }
        else if (type == DATATYPE::TPISO)
        {
            if (!isoNameList.contains(str))
                return LINESHAPE_IN();
            return LINESHAPE_IN(str, QString(), RENDERSHAPE::SHAPE_PIPE, 600.0, 600.0, 600.0, 600.0, 600.0, 600.0,
                                PARTINGLEN::PART_FULL, PARTINGRULE::PART_RATIO, 1500, 1500, 1.0, 0.0);
            //        SectionValues sv=isoTable.value(str);
            //        return judgeRenderShape(sv);
        }
        else
        {
            return LINESHAPE_IN();
        }

        return LINESHAPE_IN();
    }

    LINEPROPERTY ManageSectionData::sectionNameToLineProperty(const UnifiedData &unified, DATATYPE type, const QString &str,
                                                              qreal hh) const
    {
        qDebug() << "ManageSectionData::sectionNameToLineProperty()" << (int)type << str << hh;
        if (type == DATATYPE::TPCOLUMN)
        {
            if (!columnNameList.contains(str))
                return LINEPROPERTY();
            FloorOrderData *cd = columnTable.value(str);
            SectionValues sv = cd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return LINEPROPERTY();
            return judgeLineValue(sv);
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            qDebug() << "ManageSectionData::sectionNameToLineProperty() GIRDER";
            if (!girderNameList.contains(str))
            {
                qDebug() << "girderNameList not contains section" << str << girderNameList;
                return LINEPROPERTY();
            }
            FloorOrderData *gd = girderTable.value(str);
            SectionValues sv = gd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
            {
                qDebug() << "sectionName is Empty.";
                return LINEPROPERTY();
            }
            return judgeLineValue(sv);
        }
        else if (type == DATATYPE::TPBRACE)
        {
            if (!braceNameList.contains(str))
                return LINEPROPERTY();
            SectionValues sv = braceTable.value(str);
            return judgeLineValue(sv);
        }
        else if (type == DATATYPE::TPBEAM)
        {
            if (!beamNameList.contains(str))
                return LINEPROPERTY();
            SectionValues sv = beamTable.value(str);
            return judgeLineValue(sv);
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            return LINEPROPERTY();
        }
        else if (type == DATATYPE::TPISO)
        {
            return LINEPROPERTY();
        }
        else
        {
            return LINEPROPERTY();
        }
    }

    PLANEPROPERTY ManageSectionData::sectionNameToPlaneProperty(const UnifiedData &unified, DATATYPE type, const QString &str,
                                                                qreal hh) const
    {
        SectionValues sec_v;
        if (type == DATATYPE::TPWALL)
        {
            if (!wallNameList.contains(str))
                return PLANEPROPERTY();
            FloorOrderData *wd = wallTable.value(str);
            sec_v = wd->getSectionValueOfHeight(unified, hh);
            if (sec_v.sectionName.isEmpty())
                return PLANEPROPERTY();
        }
        else if (type == DATATYPE::TPUWALL)
        {
            if (!uwallNameList.contains(str))
                return PLANEPROPERTY();
            sec_v = uwallTable.value(str);
        }
        else if (type == DATATYPE::TPSLAB)
        {
            if (!slabNameList.contains(str))
                return PLANEPROPERTY();
            sec_v = slabTable.value(str);
        }
        else
        {
            return PLANEPROPERTY();
        }

        PlaneType::Type ptype = sec_v.plane_param.planeType;
        PlaneType2::Type ptype2 = sec_v.plane_param.planeType2;
        PLANEMODEL pmodel = (ptype == PlaneType::Type::UNDEFINED)
                                ? PLANEMODEL::NONSTRUCT_MODEL
                                : static_cast<PLANEMODEL>(ptype);
        PLANESUBMODEL psubmodel = (ptype2 == PlaneType2::Type::UNDEFINED)
                                      ? PLANESUBMODEL::NONSTRUCT_SUBMODEL
                                      : static_cast<PLANESUBMODEL>(ptype2);
        qreal tc = (sec_v.plane_param.concMaterial.isEmpty()) ? 0.0 : sec_v.plane_param.concThickness;
        qreal ts = (sec_v.plane_param.steelMaterial.isEmpty()) ? 0.0 : sec_v.plane_param.steelThickness;
        return PLANEPROPERTY(sec_v.sectionName, pmodel, psubmodel, qMax(tc, ts), sec_v.sectionType);
    }
    SectionValues ManageSectionData::sectionNameToValue(const UnifiedData &unified, DATATYPE type, const QString &str,
                                                        qreal hh) const
    {
        qDebug() << "sectionNameToValue init";
        if (type == DATATYPE::TPCOLUMN)
        {
            qDebug() << "sectionNameToValue a";
            if (!columnNameList.contains(str))
                return SectionValues();
            qDebug() << "sectionNameToValue b";
            FloorOrderData *cd = columnTable.value(str);
            qDebug() << "sectionNameToValue c";
            SectionValues sv = cd->getSectionValueOfHeight(unified, hh);
            qDebug() << "sectionNameToValue d";
            if (sv.sectionName.isEmpty())
                return SectionValues();
            return sv;
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            if (!girderNameList.contains(str))
                return SectionValues();
            FloorOrderData *gd = girderTable.value(str);
            SectionValues sv = gd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return SectionValues();
            return sv;
        }
        else if (type == DATATYPE::TPBRACE)
        {
            if (!braceNameList.contains(str))
                return SectionValues();
            SectionValues sv = braceTable.value(str);
            return sv;
        }
        else if (type == DATATYPE::TPBEAM)
        {
            if (!beamNameList.contains(str))
                return SectionValues();
            SectionValues sv = beamTable.value(str);
            return sv;
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            if (!damperNameList.contains(str))
                return SectionValues();
            SectionValues sv = damperTable.value(str);
            return sv;
        }
        else if (type == DATATYPE::TPISO)
        {
            if (!isoNameList.contains(str))
                return SectionValues();
            SectionValues sv = isoTable.value(str);
            return sv;
        }
        else if (type == DATATYPE::TPWALL)
        {
            if (!wallNameList.contains(str))
                return SectionValues();
            FloorOrderData *wd = wallTable.value(str);
            SectionValues sv = wd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return SectionValues();
            return sv;
        }
        else if (type == DATATYPE::TPUWALL)
        {
            if (!uwallNameList.contains(str))
                return SectionValues();
            SectionValues sv = uwallTable.value(str);
            return sv;
        }
        else if (type == DATATYPE::TPSLAB)
        {
            if (!slabNameList.contains(str))
                return SectionValues();
            SectionValues sv = slabTable.value(str);
            return sv;
        }
        return SectionValues();
    }
    SECTIONWEIGHT ManageSectionData::sectionNameToUnitWeight(const UnifiedData &unified, DATATYPE type, const QString &str,
                                                             qreal hh) const
    {
        if (type == DATATYPE::TPCOLUMN)
        {
            if (!columnNameList.contains(str))
                return SECTIONWEIGHT();
            FloorOrderData *cd = columnTable.value(str);
            SectionValues sv = cd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return SECTIONWEIGHT();
            return judgeLineWeight(unified, sv);
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            if (!girderNameList.contains(str))
                return SECTIONWEIGHT();
            FloorOrderData *gd = girderTable.value(str);
            SectionValues sv = gd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return SECTIONWEIGHT();
            return judgeLineWeight(unified, sv);
        }
        else if (type == DATATYPE::TPBRACE)
        {
            if (!braceNameList.contains(str))
                return SECTIONWEIGHT();
            SectionValues sv = braceTable.value(str);
            return judgeLineWeight(unified, sv);
        }
        else if (type == DATATYPE::TPBEAM)
        {
            if (!beamNameList.contains(str))
                return SECTIONWEIGHT();
            SectionValues sv = beamTable.value(str);
            return judgeLineWeight(unified, sv);
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            if (!damperNameList.contains(str))
                return SECTIONWEIGHT();
            SectionValues sv = damperTable.value(str);
            return SECTIONWEIGHT(sv.damper_param.selfWeight);
        }
        else if (type == DATATYPE::TPISO)
        {
            if (!isoNameList.contains(str))
                return SECTIONWEIGHT();
            SectionValues sv = isoTable.value(str);
            return SECTIONWEIGHT(sv.isolator_param.selfWeight);
        }
        else if (type == DATATYPE::TPWALL)
        {
            if (!wallNameList.contains(str))
                return SECTIONWEIGHT();
            FloorOrderData *wd = wallTable.value(str);
            SectionValues sv = wd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return SECTIONWEIGHT();
            return judgePlaneWeight(unified, sv);
        }
        else if (type == DATATYPE::TPUWALL)
        {
            if (!uwallNameList.contains(str))
                return SECTIONWEIGHT();
            SectionValues sv = uwallTable.value(str);
            return judgePlaneWeight(unified, sv);
        }
        else if (type == DATATYPE::TPSLAB)
        {
            if (!slabNameList.contains(str))
                return SECTIONWEIGHT();
            SectionValues sv = slabTable.value(str);
            return judgePlaneWeight(unified, sv);
        }
        return SECTIONWEIGHT();
    }

    STRUCTTYPE ManageSectionData::sectionNameToStructureType(const UnifiedData &unified, DATATYPE type, const QString &str,
                                                             qreal hh) const
    {
        if (type == DATATYPE::TPCOLUMN)
        {
            if (!columnNameList.contains(str))
                return STRUCTTYPE::NOSTRUCTTYPE;
            FloorOrderData *cd = columnTable.value(str);
            SectionValues sv = cd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return STRUCTTYPE::NOSTRUCTTYPE;
            return sv.sectionType;
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            if (!girderNameList.contains(str))
                return STRUCTTYPE::NOSTRUCTTYPE;
            FloorOrderData *gd = girderTable.value(str);
            SectionValues sv = gd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return STRUCTTYPE::NOSTRUCTTYPE;
            return sv.sectionType;
        }
        else if (type == DATATYPE::TPBRACE)
        {
            if (!braceNameList.contains(str))
                return STRUCTTYPE::NOSTRUCTTYPE;
            SectionValues sv = braceTable.value(str);
            return sv.sectionType;
        }
        else if (type == DATATYPE::TPBEAM)
        {
            if (!beamNameList.contains(str))
                return STRUCTTYPE::NOSTRUCTTYPE;
            SectionValues sv = beamTable.value(str);
            return sv.sectionType;
        }
        else if (type == DATATYPE::TPDAMPER || type == DATATYPE::TPISO)
        {
            return STRUCTTYPE::NOSTRUCTTYPE;
        }
        else if (type == DATATYPE::TPWALL)
        {
            if (!wallNameList.contains(str))
                return STRUCTTYPE::NOSTRUCTTYPE;
            FloorOrderData *wd = wallTable.value(str);
            SectionValues sv = wd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return STRUCTTYPE::NOSTRUCTTYPE;
            return sv.sectionType;
        }
        else if (type == DATATYPE::TPUWALL)
        {
            if (!uwallNameList.contains(str))
                return STRUCTTYPE::NOSTRUCTTYPE;
            SectionValues sv = uwallTable.value(str);
            return sv.sectionType;
        }
        else if (type == DATATYPE::TPSLAB)
        {
            if (!slabNameList.contains(str))
                return STRUCTTYPE::NOSTRUCTTYPE;
            SectionValues sv = slabTable.value(str);
            return sv.sectionType;
        }
        return STRUCTTYPE::NOSTRUCTTYPE;
    }

    BASEVALUES ManageSectionData::getBaseValues(const QString &str) const
    {
        if (!baseTable.contains(str))
            return BASEVALUES();
        SectionValues sv = baseTable.value(str);
        return BASEVALUES(sv.sectionName, sv.base_param.sizeB,
                          sv.base_param.sizeD, sv.base_param.sizeT);
    }

    QList<WallOpenParameter> ManageSectionData::getWallOpenValues(const QString &str) const
    {
        if (!openTable.contains(str))
            return QList<WallOpenParameter>();
        MultiSectionData *ms = openTable.value(str);
        QList<WallOpenParameter> ovs;
        Q_FOREACH (SectionValues sv, ms->getSetionValues())
            ovs.append(sv.wopen_param);
        return ovs;
    }

    qreal ManageSectionData::getSlabLoadDir(const QString &str) const
    {
        if (!slabTable.contains(str))
            return -1.0;
        return slabTable.value(str).plane_param.loadDir;
    }

    bool ManageSectionData::isStructualSection(const UnifiedData &unified, DATATYPE dtype, const QString &str, qreal hh) const
    {
        if (dtype == DATATYPE::TPCOLUMN || dtype == DATATYPE::TPGIRDER || dtype == DATATYPE::TPBRACE ||
            dtype == DATATYPE::TPUWALL || dtype == DATATYPE::TPDAMPER || dtype == DATATYPE::TPISO)
        {
            return true;
        }
        else if (dtype == DATATYPE::TPWALL)
        {
            if (!wallNameList.contains(str))
                return false;
            FloorOrderData *wd = wallTable.value(str);
            SectionValues sv = wd->getSectionValueOfHeight(unified, hh);
            if (sv.sectionName.isEmpty())
                return false;
            return (sv.plane_param.planeType != PlaneType::Type::NONSTRUCT);
        }
        else if (dtype == DATATYPE::TPSLAB)
        {
            if (!slabTable.contains(str))
                return false;
            return (slabTable.value(str).plane_param.planeType != PlaneType::Type::NONSTRUCT);
        }
        return false;
    }

    /* 柱　チェック */
    bool ManageSectionData::checkColumnData(QWidget *parent, const QList<SectionValues> &slist)
    {
        Q_UNUSED(parent);
        //
        // 名前未定義　材料未定義
        //
        //
        cash_SecList = slist;
        return true;
    }

    /* 大梁　チェック */
    bool ManageSectionData::checkGirderData(QWidget *parent, const QList<SectionValues> &slist)
    {
        Q_UNUSED(parent);
        //
        // 名前未定義　材料未定義
        //
        //
        cash_SecList = slist;
        return true;
    }

    /* 壁　チェック */
    bool ManageSectionData::checkWallData(QWidget *parent, const QList<SectionValues> &slist)
    {
        Q_UNUSED(parent);
        //
        // 名前未定義　材料未定義
        //
        //
        cash_SecList = slist;
        return true;
    }



    /* ブレース　チェック */
    bool ManageSectionData::checkBraceData(QWidget *parent, const QList<SectionValues> &slist)
    {
        QStringList nameList, overlapList;
        Q_FOREACH (SectionValues sv, slist)
        {
            QString name = sv.sectionName;
            if (nameList.contains(name))
            {
                if (!overlapList.contains(name))
                    overlapList.append(name);
            }
            else
            {
                nameList.append(name);
            }
        }
        if (overlapList.count() > 0)
        {
            errorOverlapNames(parent, overlapList);
            return false;
        }
        cash_SecList = slist;
        return true;
    }

    /* 小梁　チェック */
    bool ManageSectionData::checkBeamData(QWidget *parent, const QList<SectionValues> &slist)
    {
        QStringList nameList, overlapList;
        Q_FOREACH (SectionValues sv, slist)
        {
            QString name = sv.sectionName;
            if (nameList.contains(name))
            {
                if (!overlapList.contains(name))
                    overlapList.append(name);
            }
            else
            {
                nameList.append(name);
            }
        }
        if (overlapList.count() > 0)
        {
            errorOverlapNames(parent, overlapList);
            return false;
        }
        cash_SecList = slist;
        return true;
    }

    /* スラブ　チェック */
    bool ManageSectionData::checkSlabData(QWidget *parent, const QList<SectionValues> &slist)
    {
        QStringList nameList, overlapList;
        Q_FOREACH (SectionValues sv, slist)
        {
            QString name = sv.sectionName;
            if (nameList.contains(name))
            {
                if (!overlapList.contains(name))
                    overlapList.append(name);
            }
            else
            {
                nameList.append(name);
            }
        }
        if (overlapList.count() > 0)
        {
            errorOverlapNames(parent, overlapList);
            return false;
        }
        cash_SecList = slist;
        return true;
    }

    /* 地下外壁　チェック */
    bool ManageSectionData::checkUWallData(QWidget *parent, const QList<SectionValues> &slist)
    {
        QStringList nameList, overlapList;
        Q_FOREACH (SectionValues sv, slist)
        {
            QString name = sv.sectionName;
            if (nameList.contains(name))
            {
                if (!overlapList.contains(name))
                    overlapList.append(name);
            }
            else
            {
                nameList.append(name);
            }
        }
        if (overlapList.count() > 0)
        {
            errorOverlapNames(parent, overlapList);
            return false;
        }
        cash_SecList = slist;
        return true;
    }

    /* 開口　チェック　*/
    bool ManageSectionData::checkOpenData(QWidget *parent, const QList<SectionValues> &slist)
    {
        Q_UNUSED(parent);
        //
        //
        //
        //
        cash_SecList = slist;
        return true;
        /*
            QStringList nameList,overlapList;
            Q_FOREACH( SectionValues sv, slist ){
                QString name=sv.sectionName;
                if( nameList.contains(name) ){
                    if( !overlapList.contains(name) ) overlapList.append(name);
                }else{
                    nameList.append(name);
                }
            }
            if( overlapList.count()>0 ){
                errorOverlapNames(parent,overlapList);
                return false;
            }
            cash_SecList=slist;
            return true;
        */
    }

    /* 柱脚　チェック */
    bool ManageSectionData::checkBaseData(QWidget *parent, const QList<SectionValues> &slist)
    {
        QStringList nameList, overlapList;
        Q_FOREACH (SectionValues sv, slist)
        {
            QString name = sv.sectionName;
            if (nameList.contains(name))
            {
                if (!overlapList.contains(name))
                    overlapList.append(name);
            }
            else
            {
                nameList.append(name);
            }
        }
        if (overlapList.count() > 0)
        {
            errorOverlapNames(parent, overlapList);
            return false;
        }
        cash_SecList = slist;
        return true;
    }

    /* 制振要素　チェック */
    bool ManageSectionData::checkDamperData(QWidget *parent, const QList<SectionValues> &slist)
    {
        QStringList nameList, overlapList;
        Q_FOREACH (SectionValues sv, slist)
        {
            QString name = sv.sectionName;
            if (nameList.contains(name))
            {
                if (!overlapList.contains(name))
                    overlapList.append(name);
            }
            else
            {
                nameList.append(name);
            }
        }
        if (overlapList.count() > 0)
        {
            errorOverlapNames(parent, overlapList);
            return false;
        }
        cash_SecList = slist;
        return true;
    }

    /* 免震要素　チェック */
    bool ManageSectionData::checkIsoData(QWidget *parent, const QList<SectionValues> &slist)
    {
        QStringList nameList, overlapList;
        Q_FOREACH (SectionValues sv, slist)
        {
            QString name = sv.sectionName;
            if (nameList.contains(name))
            {
                if (!overlapList.contains(name))
                    overlapList.append(name);
            }
            else
            {
                nameList.append(name);
            }
        }
        if (overlapList.count() > 0)
        {
            errorOverlapNames(parent, overlapList);
            return false;
        }
        cash_SecList = slist;
        return true;
    }

    /* キャッシュデータの登録 */

    void ManageSectionData::registerColumnData()
    {
        QHash<QString, int> cash_Count;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);

            if (cash_Count.contains(sv.sectionName))
            {
                int ii = cash_Count.value(sv.sectionName);
                cash_Count.insert(sv.sectionName, ii++);
            }
            else
            {
                cash_Count.insert(sv.sectionName, 1);
            }

            if (!changedList.contains(sv.sectionName))
            {
                if (!columnNameList.contains(sv.sectionName))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
                FloorOrderData *cd = columnTable.value(sv.sectionName);
                SectionValues cv = cd->getValueOfLimitedFloors(sv.startFloorID, sv.endFloorID);
                if (cv.sectionName != sv.sectionName)
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
                if (!compareSectionData(cv, sv, DATATYPE::TPCOLUMN))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }
        for (int i = 0; i < columnNameList.count(); i++)
        {
            QString name = columnNameList.at(i);
            FloorOrderData *cd = columnTable.value(name);
            if (!cash_Count.contains(name))
            {
                deletedList.append(name);
            }
            else if (cash_Count.value(name) != cd->listCount())
            {
                if (!changedList.contains(name))
                    changedList.append(name);
            }
        }
        // 既存データのクリア
        Q_FOREACH (FloorOrderData *cd, columnTable.values())
            delete cd;
        columnTable.clear();
        columnNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                QString name = sv.sectionName;
                if (columnTable.contains(name))
                {
                    FloorOrderData *cd = columnTable.value(name);
                    cd->appendValue(sv);
                }
                else
                {
                    FloorOrderData *cd = new FloorOrderData(isOutput, sv.sectionName);
                    cd->appendValue(sv);
                    columnTable.insert(name, cd);
                    columnNameList.append(name);
                }
            }
        }
        qDebug() << "registerColumnData:columnNameList" << columnNameList;

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPCOLUMN, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPCOLUMN, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerGirderData()
    {
        QHash<QString, int> cash_Count;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);

            if (cash_Count.contains(sv.sectionName))
            {
                int ii = cash_Count.value(sv.sectionName);
                cash_Count.insert(sv.sectionName, ii++);
            }
            else
            {
                cash_Count.insert(sv.sectionName, 1);
            }

            if (!changedList.contains(sv.sectionName))
            {
                if (!girderNameList.contains(sv.sectionName))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
                FloorOrderData *cd = girderTable.value(sv.sectionName);
                SectionValues cv = cd->getValueOfLimitedFloors(sv.startFloorID, sv.endFloorID);
                if (cv.sectionName != sv.sectionName)
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
                if (!compareSectionData(cv, sv, DATATYPE::TPGIRDER))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }

        for (int i = 0; i < girderNameList.count(); i++)
        {
            QString name = girderNameList.at(i);
            FloorOrderData *cd = girderTable.value(name);
            if (!cash_Count.contains(name))
            {
                deletedList.append(name);
            }
            else if (cash_Count.value(name) != cd->listCount())
            {
                if (!changedList.contains(name))
                    changedList.append(name);
            }
        }

        // 既存データのクリア
        Q_FOREACH (FloorOrderData *cd, girderTable.values())
            delete cd;
        girderTable.clear();
        girderNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                QString name = sv.sectionName;
                if (girderTable.contains(name))
                {
                    FloorOrderData *cd = girderTable.value(name);
                    cd->appendValue(sv);
                }
                else
                {
                    FloorOrderData *cd = new FloorOrderData(isOutput, sv.sectionName);
                    cd->appendValue(sv);
                    girderTable.insert(name, cd);
                    girderNameList.append(name);
                }
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPGIRDER, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPGIRDER, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerBraceData()
    {
        QStringList cashNameList;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);
            cashNameList.append(sv.sectionName);

            if (!braceNameList.contains(sv.sectionName))
            {
                changedList.append(sv.sectionName);
            }
            else
            {
                SectionValues bv = braceTable.value(sv.sectionName);
                if (!compareSectionData(bv, sv, DATATYPE::TPBRACE))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }

        for (int i = 0; i < braceNameList.count(); i++)
        {
            QString name = braceNameList.at(i);
            if (!cashNameList.contains(name))
                deletedList.append(name);
        }

        // 既存データのクリア
        braceTable.clear();
        braceNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                braceTable.insert(sv.sectionName, sv);
                braceNameList.append(sv.sectionName);
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPBRACE, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPBRACE, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerBeamData()
    {
        QStringList cashNameList;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);
            cashNameList.append(sv.sectionName);

            if (!beamNameList.contains(sv.sectionName))
            {
                changedList.append(sv.sectionName);
            }
            else
            {
                SectionValues bv = beamTable.value(sv.sectionName);
                if (!compareSectionData(bv, sv, DATATYPE::TPBEAM))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }

        for (int i = 0; i < beamNameList.count(); i++)
        {
            QString name = beamNameList.at(i);
            if (!cashNameList.contains(name))
                deletedList.append(name);
        }

        // 既存データのクリア
        beamTable.clear();
        beamNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                beamTable.insert(sv.sectionName, sv);
                beamNameList.append(sv.sectionName);
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPBEAM, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPBEAM, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerSlabData()
    {
        QStringList cashNameList;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);
            cashNameList.append(sv.sectionName);

            if (!slabNameList.contains(sv.sectionName))
            {
                changedList.append(sv.sectionName);
            }
            else
            {
                SectionValues bv = slabTable.value(sv.sectionName);
                if (!compareSectionData(bv, sv, DATATYPE::TPSLAB))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }

        for (int i = 0; i < slabNameList.count(); i++)
        {
            QString name = slabNameList.at(i);
            if (!cashNameList.contains(name))
                deletedList.append(name);
        }

        // 既存データのクリア
        slabTable.clear();
        slabNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                slabTable.insert(sv.sectionName, sv);
                slabNameList.append(sv.sectionName);
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPSLAB, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPSLAB, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerWallData()
    {
        QHash<QString, int> cash_Count;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);

            if (cash_Count.contains(sv.sectionName))
            {
                int ii = cash_Count.value(sv.sectionName);
                cash_Count.insert(sv.sectionName, ii++);
            }
            else
            {
                cash_Count.insert(sv.sectionName, 1);
            }

            if (!changedList.contains(sv.sectionName))
            {
                if (!wallNameList.contains(sv.sectionName))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
                FloorOrderData *cd = wallTable.value(sv.sectionName);
                SectionValues cv = cd->getValueOfLimitedFloors(sv.startFloorID, sv.endFloorID);
                if (cv.sectionName != sv.sectionName)
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
                if (!compareSectionData(cv, sv, DATATYPE::TPWALL))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }

        for (int i = 0; i < wallNameList.count(); i++)
        {
            QString name = wallNameList.at(i);
            FloorOrderData *cd = wallTable.value(name);
            if (!cash_Count.contains(name))
            {
                deletedList.append(name);
            }
            else if (cash_Count.value(name) != cd->listCount())
            {
                if (!changedList.contains(name))
                    changedList.append(name);
            }
        }

        // 既存データのクリア
        Q_FOREACH (FloorOrderData *cd, wallTable.values())
            delete cd;
        wallTable.clear();
        wallNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                QString name = sv.sectionName;
                if (wallTable.contains(name))
                {
                    FloorOrderData *cd = wallTable.value(name);
                    cd->appendValue(sv);
                }
                else
                {
                    FloorOrderData *cd = new FloorOrderData(isOutput, sv.sectionName);
                    cd->appendValue(sv);
                    wallTable.insert(name, cd);
                    wallNameList.append(name);
                }
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPWALL, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPWALL, changedList);

        cash_SecList.clear();
    }


    void ManageSectionData::registerUWallData()
    {
        QStringList cashNameList;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);
            cashNameList.append(sv.sectionName);

            if (!uwallNameList.contains(sv.sectionName))
            {
                changedList.append(sv.sectionName);
            }
            else
            {
                SectionValues bv = uwallTable.value(sv.sectionName);
                if (!compareSectionData(bv, sv, DATATYPE::TPUWALL))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }

        for (int i = 0; i < uwallNameList.count(); i++)
        {
            QString name = uwallNameList.at(i);
            if (!cashNameList.contains(name))
                deletedList.append(name);
        }

        // 既存データのクリア
        uwallTable.clear();
        uwallNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                uwallTable.insert(sv.sectionName, sv);
                uwallNameList.append(sv.sectionName);
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPUWALL, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPUWALL, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerBaseData()
    {
        QStringList cashNameList;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);
            cashNameList.append(sv.sectionName);

            if (!baseNameList.contains(sv.sectionName))
            {
                changedList.append(sv.sectionName);
            }
            else
            {
                SectionValues bv = baseTable.value(sv.sectionName);
                if (!compareSectionData(bv, sv, DATATYPE::TPBASE))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }

        for (int i = 0; i < baseNameList.count(); i++)
        {
            QString name = baseNameList.at(i);
            if (!cashNameList.contains(name))
                deletedList.append(name);
        }

        // 既存データのクリア
        baseTable.clear();
        baseNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                baseTable.insert(sv.sectionName, sv);
                baseNameList.append(sv.sectionName);
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPBASE, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPBASE, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerOpenData()
    {
        /*
            QStringList cashNameList;
            QStringList changedList,deletedList;

            // 既存データとキャッシュデータの差分把握

            for(int i=0; i<cash_SecList.count(); i++){
                SectionValues sv=cash_SecList.at(i);
                cashNameList.append(sv.sectionName);

                if( !openNameList.contains(sv.sectionName) ){
                    changedList.append( sv.sectionName );
                }else{
                    SectionValues bv=openTable.value(sv.sectionName);
                    if( !compareSectionData(bv,sv,TPWALLOPEN) ){
                        changedList.append( sv.sectionName );
                        continue;
                    }
                }
            }

            for( int i=0; i<openNameList.count() ; i++){
                QString name=openNameList.at(i);
                if( !cashNameList.contains(name) ) deletedList.append(name);
            }

            // 既存データのクリア
            openTable.clear();
            openNameList.clear();

            // キャッシュデータの登録

            if( !cash_SecList.isEmpty() ){

                Q_FOREACH(SectionValues sv, cash_SecList){
                    openTable.insert(sv.sectionName,sv);
                    openNameList.append(sv.sectionName);
                }
            }

            if( deletedList.count()>0 ) emit sectionDeleted(TPWALLOPEN,deletedList);
            if( changedList.count()>0 ) emit sectionChanged(TPWALLOPEN,changedList);

            cash_SecList.clear();

        */

        //    QHash<QString,int> cash_Count;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);
            /*
                    if( cash_Count.contains(sv.sectionName) ){
                        int ii=cash_Count.value(sv.sectionName);
                        cash_Count.insert(sv.sectionName,ii++);
                    }else{
                        cash_Count.insert(sv.sectionName,1);
                    }
            */

            if (!changedList.contains(sv.sectionName))
            {

                changedList.append(sv.sectionName);
                continue;
                /*
                            if( !openNameList.contains(sv.sectionName) ){
                                changedList.append( sv.sectionName );
                                continue;
                            }
                            MultiSectionData *cd=openTable.value( sv.sectionName );
                            SectionValues cv=cd->getValueOfLimitedFloors( sv.startFloorID, sv.endFloorID );
                            if( cv.sectionName!=sv.sectionName ){
                                changedList.append( sv.sectionName );
                                continue;
                            }
                            if( !compareSectionData(cv,sv,TPWALLOPEN) ){
                                changedList.append( sv.sectionName );
                                continue;
                            }
                */
            }
        }

        for (int i = 0; i < openNameList.count(); i++)
        {
            QString name = openNameList.at(i);
            /*
                    MultiSectionData *cd=openTable.value( name );
                    if( !cash_Count.contains(name) ){
                        deletedList.append(name);
                    }else if( cash_Count.value(name)!=cd->listCount() ){
                        if( !changedList.contains(name) ) changedList.append(name);
                    }
            */
            if (!changedList.contains(name))
                deletedList.append(name);
        }

        // 既存データのクリア
        Q_FOREACH (MultiSectionData *cd, openTable.values())
            delete cd;
        openTable.clear();
        openNameList.clear();

        // キャッシュデータの登録
        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                QString name = sv.sectionName;
                if (openTable.contains(name))
                {
                    MultiSectionData *cd = openTable.value(name);
                    cd->appendValue(sv);
                }
                else
                {
                    MultiSectionData *cd = new MultiSectionData(sv.sectionName);
                    cd->appendValue(sv);
                    openTable.insert(name, cd);
                    openNameList.append(name);
                }
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPWALLOPEN, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPWALLOPEN, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerDamperData()
    {
        QStringList cashNameList;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);
            cashNameList.append(sv.sectionName);

            if (!damperNameList.contains(sv.sectionName))
            {
                changedList.append(sv.sectionName);
            }
            else
            {
                SectionValues bv = damperTable.value(sv.sectionName);
                if (!compareSectionData(bv, sv, DATATYPE::TPDAMPER))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }

        for (int i = 0; i < damperNameList.count(); i++)
        {
            QString name = damperNameList.at(i);
            if (!cashNameList.contains(name))
                deletedList.append(name);
        }

        // 既存データのクリア
        damperTable.clear();
        damperNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                damperTable.insert(sv.sectionName, sv);
                damperNameList.append(sv.sectionName);
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPDAMPER, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPDAMPER, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerIsoData()
    {
        QStringList cashNameList;
        QStringList changedList, deletedList;

        // 既存データとキャッシュデータの差分把握

        for (int i = 0; i < cash_SecList.count(); i++)
        {
            SectionValues sv = cash_SecList.at(i);
            cashNameList.append(sv.sectionName);

            if (!isoNameList.contains(sv.sectionName))
            {
                changedList.append(sv.sectionName);
            }
            else
            {
                SectionValues bv = isoTable.value(sv.sectionName);
                if (!compareSectionData(bv, sv, DATATYPE::TPISO))
                {
                    changedList.append(sv.sectionName);
                    continue;
                }
            }
        }

        for (int i = 0; i < isoNameList.count(); i++)
        {
            QString name = isoNameList.at(i);
            if (!cashNameList.contains(name))
                deletedList.append(name);
        }

        // 既存データのクリア
        isoTable.clear();
        isoNameList.clear();

        // キャッシュデータの登録

        if (!cash_SecList.isEmpty())
        {

            Q_FOREACH (SectionValues sv, cash_SecList)
            {
                isoTable.insert(sv.sectionName, sv);
                isoNameList.append(sv.sectionName);
            }
        }

        if (deletedList.count() > 0)
            emit sectionDeleted(DATATYPE::TPISO, deletedList);
        if (changedList.count() > 0)
            emit sectionChanged(DATATYPE::TPISO, changedList);

        cash_SecList.clear();
    }

    void ManageSectionData::registerTempData(DATATYPE type, const QList<SectionValues> &slist)
    {
        cash_SecList = slist;
        if (type == DATATYPE::TPCOLUMN)
        {
            registerColumnData();
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            registerGirderData();
        }
        else if (type == DATATYPE::TPBRACE)
        {
            registerBraceData();
        }
        else if (type == DATATYPE::TPBEAM)
        {
            registerBeamData();
        }
        else if (type == DATATYPE::TPSLAB)
        {
            registerSlabData();
        }
        else if (type == DATATYPE::TPWALL)
        {
            registerWallData();
        }
        else if (type == DATATYPE::TPUWALL)
        {
            registerUWallData();
        }
        else if (type == DATATYPE::TPBASE)
        {
            registerBaseData();
        }
        else if (type == DATATYPE::TPWALLOPEN)
        {
            registerOpenData();
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            registerDamperData();
        }
        else if (type == DATATYPE::TPISO)
        {
            registerIsoData();
        }
        else
        {
            cash_SecList.clear();
        }
    }

    void ManageSectionData::replaceSectionName(DATATYPE dtype, const QString &str_a,
                                               const QString &str_b)
    {
        if (dtype == DATATYPE::TPCOLUMN)
        {
            int idx = columnNameList.indexOf(str_a);
            if (idx < 0)
                return;
            columnNameList.replace(idx, str_b);
            FloorOrderData *sv = columnTable.value(str_a);
            sv->changeSectionName(str_b);
            columnTable.remove(str_a);
            columnTable.insert(str_b, sv);
        }
        else if (dtype == DATATYPE::TPGIRDER)
        {
            int idx = girderNameList.indexOf(str_a);
            if (idx < 0)
                return;
            girderNameList.replace(idx, str_b);
            FloorOrderData *sv = girderTable.value(str_a);
            sv->changeSectionName(str_b);
            girderTable.remove(str_a);
            girderTable.insert(str_b, sv);
        }
        else if (dtype == DATATYPE::TPBRACE)
        {
            int idx = braceNameList.indexOf(str_a);
            if (idx < 0)
                return;
            braceNameList.replace(idx, str_b);
            SectionValues sv = braceTable.value(str_a);
            sv.sectionName = str_b;
            braceTable.remove(str_a);
            braceTable.insert(str_b, sv);
        }
        else if (dtype == DATATYPE::TPBEAM)
        {
            int idx = beamNameList.indexOf(str_a);
            if (idx < 0)
                return;
            beamNameList.replace(idx, str_b);
            SectionValues sv = beamTable.value(str_a);
            sv.sectionName = str_b;
            beamTable.remove(str_a);
            beamTable.insert(str_b, sv);
        }
        else if (dtype == DATATYPE::TPSLAB)
        {
            int idx = slabNameList.indexOf(str_a);
            if (idx < 0)
                return;
            slabNameList.replace(idx, str_b);
            SectionValues sv = slabTable.value(str_a);
            sv.sectionName = str_b;
            slabTable.remove(str_a);
            slabTable.insert(str_b, sv);
        }
        else if (dtype == DATATYPE::TPWALL)
        {
            int idx = wallNameList.indexOf(str_a);
            if (idx < 0)
                return;
            wallNameList.replace(idx, str_b);
            FloorOrderData *sv = wallTable.value(str_a);
            sv->changeSectionName(str_b);
            wallTable.remove(str_a);
            wallTable.insert(str_b, sv);
        }
        else if (dtype == DATATYPE::TPUWALL)
        {
            int idx = uwallNameList.indexOf(str_a);
            if (idx < 0)
                return;
            uwallNameList.replace(idx, str_b);
            SectionValues sv = uwallTable.value(str_a);
            sv.sectionName = str_b;
            uwallTable.remove(str_a);
            uwallTable.insert(str_b, sv);
        }
        else if (dtype == DATATYPE::TPBASE)
        {
            int idx = baseNameList.indexOf(str_a);
            if (idx < 0)
                return;
            baseNameList.replace(idx, str_b);
            SectionValues sv = baseTable.value(str_a);
            sv.sectionName = str_b;
            baseTable.remove(str_a);
            baseTable.insert(str_b, sv);
            for (int i = 0; i < columnNameList.count(); i++)
            {
                FloorOrderData *sv = columnTable.value(columnNameList.at(i));
                sv->replaceBaseName(str_a, str_b);
            }
        }
        else if (dtype == DATATYPE::TPWALLOPEN)
        {
            int idx = openNameList.indexOf(str_a);
            if (idx < 0)
                return;
            openNameList.replace(idx, str_b);
            MultiSectionData *sv = openTable.value(str_a);
            sv->changeSectionName(str_b);
            openTable.remove(str_a);
            openTable.insert(str_b, sv);
        }
        else if (dtype == DATATYPE::TPDAMPER)
        {
            int idx = damperNameList.indexOf(str_a);
            if (idx < 0)
                return;
            damperNameList.replace(idx, str_b);
            SectionValues sv = damperTable.value(str_a);
            sv.sectionName = str_b;
            damperTable.remove(str_a);
            damperTable.insert(str_b, sv);
        }
        else if (dtype == DATATYPE::TPISO)
        {
            int idx = isoNameList.indexOf(str_a);
            if (idx < 0)
                return;
            isoNameList.replace(idx, str_b);
            SectionValues sv = isoTable.value(str_a);
            sv.sectionName = str_b;
            isoTable.remove(str_a);
            isoTable.insert(str_b, sv);
        }
    }

    void ManageSectionData::clearData()
    {
        Q_FOREACH (FloorOrderData *cd, columnTable.values())
            delete cd;
        columnTable.clear();
        columnNameList.clear();
        Q_FOREACH (FloorOrderData *gd, girderTable.values())
            delete gd;
        girderTable.clear();
        girderNameList.clear();
        Q_FOREACH (FloorOrderData *wd, wallTable.values())
            delete wd;
        wallTable.clear();
        wallNameList.clear();


        braceTable.clear();
        braceNameList.clear();
        beamTable.clear();
        beamNameList.clear();
        slabTable.clear();
        slabNameList.clear();
        uwallTable.clear();
        uwallNameList.clear();
        openTable.clear();
        openNameList.clear();
        baseTable.clear();
        baseNameList.clear();
        damperTable.clear();
        damperNameList.clear();
        isoTable.clear();
        isoNameList.clear();
    }

    bool ManageSectionData::compareSectionData(const SectionValues &sv1, const SectionValues &sv2,
                                               DATATYPE dtype) const
    {
        if (sv1.sectionType != sv2.sectionType)
            return false;

        if (dtype == DATATYPE::TPCOLUMN)
        {
            if (!compareLineCommonParameter(sv1.line_param, sv2.line_param))
                return false;
            if (!compareLinePartsParameter(sv1.i_param, sv2.i_param))
                return false;
            if (!compareLinePartsParameter(sv1.j_param, sv2.j_param))
                return false;
        }
        else if (dtype == DATATYPE::TPGIRDER || dtype == DATATYPE::TPBRACE || dtype == DATATYPE::TPBEAM)
        {
            if (!compareLineCommonParameter(sv1.line_param, sv2.line_param))
                return false;
            if (!compareLinePartsParameter(sv1.i_param, sv2.i_param))
                return false;
            if (!compareLinePartsParameter(sv1.c_param, sv2.c_param))
                return false;
            if (!compareLinePartsParameter(sv1.j_param, sv2.j_param))
                return false;
        }
        else if (dtype == DATATYPE::TPSLAB || dtype == DATATYPE::TPWALL || dtype == DATATYPE::TPUWALL)
        {
            if (!comparePlaneParameter(sv1.plane_param, sv2.plane_param))
                return false;
        }
        else if (dtype == DATATYPE::TPBASE)
        {
            if (!compareBaseParameter(sv1.base_param, sv2.base_param))
                return false;
        }
        else if (dtype == DATATYPE::TPWALLOPEN)
        {
            if (!compareWallOpenParameter(sv1.wopen_param, sv2.wopen_param))
                return false;
        }
        else if (dtype == DATATYPE::TPDAMPER)
        {
            if (!compareDamperParameter(sv1.damper_param, sv2.damper_param))
                return false;
        }
        else if (dtype == DATATYPE::TPISO)
        {
            if (!compareIsoParameter(sv1.isolator_param, sv2.isolator_param))
                return false;
            if (!compareSpringParameter(sv1.hspring_param, sv2.hspring_param))
                return false;
            if (!compareSpringParameter(sv1.vspring_param, sv2.vspring_param))
                return false;
        }
        return true;
    }

    bool ManageSectionData::compareLineCommonParameter(const LineCommonParameter &pm1,
                                                       const LineCommonParameter &pm2) const
    {
        if (pm1.isCantilever != pm2.isCantilever)
            return false;
        if (qAbs(pm1.i_length - pm2.i_length) > 1.0e-5)
            return false;
        if (qAbs(pm1.j_length - pm2.j_length) > 1.0e-5)
            return false;
        if (pm1.basePlate != pm2.basePlate)
            return false;
        if (qAbs(pm1.direct_area - pm2.direct_area) > 1.0e-5)
            return false;
        return true;
    }

    bool ManageSectionData::compareLinePartsParameter(const LinePartsParameter &pm1,
                                                      const LinePartsParameter &pm2) const
    {
        if (qAbs(pm1.sizeB - pm2.sizeB) > 1.0e-5)
            return false;
        if (qAbs(pm1.sizeH - pm2.sizeH) > 1.0e-5)
            return false;
        if (qAbs(pm1.Dy - pm2.Dy) > 1.0e-5)
            return false;
        if (qAbs(pm1.Dz - pm2.Dz) > 1.0e-5)
            return false;
        return true;
    }

    bool ManageSectionData::comparePlaneParameter(const PlaneSectionParameter &pm1,
                                                  const PlaneSectionParameter &pm2) const
    {
        if (qAbs(pm1.concThickness - pm2.concThickness) > 1.0e-5)
            return false;
        if (qAbs(pm1.steelThickness - pm2.steelThickness) > 1.0e-5)
            return false;
        if (pm1.planeType != pm2.planeType)
            return false;
        return true;
    }

    bool ManageSectionData::compareBaseParameter(const BaseSectionParameter &pm1,
                                                 const BaseSectionParameter &pm2) const
    {
        if (qAbs(pm1.sizeB - pm2.sizeB) > 1.0e-5)
            return false;
        if (qAbs(pm1.sizeD - pm2.sizeD) > 1.0e-5)
            return false;
        if (qAbs(pm1.sizeT - pm2.sizeT) > 1.0e-5)
            return false;
        if (qAbs(pm1.sizeBc - pm2.sizeBc) > 1.0e-5)
            return false;
        if (qAbs(pm1.sizeDc - pm2.sizeDc) > 1.0e-5)
            return false;
        if (qAbs(pm1.sizeH - pm2.sizeH) > 1.0e-5)
            return false;
        return true;
    }

    bool ManageSectionData::compareWallOpenParameter(const WallOpenParameter &pm1,
                                                     const WallOpenParameter &pm2) const
    {
        if (pm1.openType != pm2.openType)
            return false;
        if (pm1.isHorizontalFace != pm2.isHorizontalFace)
            return false;
        if (pm1.hPosition != pm2.hPosition)
            return false;
        if (pm1.isVerticalFace != pm2.isVerticalFace)
            return false;
        if (pm1.vPosition != pm2.vPosition)
            return false;
        if (qAbs(pm1.ox - pm2.ox) > 1.0e-5)
            return false;
        if (qAbs(pm1.oy - pm2.oy) > 1.0e-5)
            return false;
        if (qAbs(pm1.width - pm2.width) > 1.0e-5)
            return false;
        if (qAbs(pm1.height - pm2.height) > 1.0e-5)
            return false;
        if (qAbs(pm1.slitR - pm2.slitR) > 1.0e-5)
            return false;
        if (qAbs(pm1.slitL - pm2.slitL) > 1.0e-5)
            return false;
        if (qAbs(pm1.slitT - pm2.slitT) > 1.0e-5)
            return false;
        if (qAbs(pm1.slitB - pm2.slitB) > 1.0e-5)
            return false;
        return true;
    }

    bool ManageSectionData::compareDamperParameter(const DamperParameter &,
                                                   const DamperParameter &) const
    {
        return true;
    }

    bool ManageSectionData::compareIsoParameter(const IsolatorParameter &pm1,
                                                const IsolatorParameter &pm2) const
    {
        if (qAbs(pm1.sizeD - pm2.sizeD) > 1.0e-5)
            return false;
        if (qAbs(pm1.sizeH - pm2.sizeH) > 1.0e-5)
            return false;
        if (qAbs(pm1.settingLevel - pm2.settingLevel) > 1.0e-5)
            return false;
        return true;
    }

    bool ManageSectionData::compareSpringParameter(const SpringParameter &,
                                                   const SpringParameter &) const
    {
        return true;
    }

    LINESHAPE_IN ManageSectionData::judgeRenderShape(DATATYPE dtype, const SectionValues &sv) const
    {
        QString name = sv.sectionName;
        STRUCTTYPE stype = sv.sectionType;
        RENDERSHAPE shape = RENDERSHAPE::SHAPE_BOX;

        qDebug() << u8"ManageSectionData::judgeRenderShape()" << sv.sectionName << (int)sv.sectionType << (int)sv.i_param.steelType;
        if (stype == STRUCTTYPE::STP_S || stype == STRUCTTYPE::STP_CFT)
        {

            shape = sectionTypeToSteelRenderShape(sv.i_param.steelType);
            if (dtype == DATATYPE::TPCOLUMN || sv.line_param.isCantilever)
            {
                if (sv.line_param.i_length > 1.0e-3)
                {
                    return LINESHAPE_IN(name, sv.line_param.basePlate, shape,
                                        sv.i_param.sizeB, sv.i_param.sizeH,
                                        sv.c_param.sizeB, sv.c_param.sizeH,
                                        sv.j_param.sizeB, sv.j_param.sizeH,
                                        PARTINGLEN::PART_FULL, PARTINGRULE::PART_VALUE,
                                        sv.line_param.i_length, -1.0, 0, 0,
                                        sv.i_param.B_T, sv.i_param.H_T,
                                        sv.c_param.B_T, sv.c_param.H_T,
                                        sv.j_param.B_T, sv.j_param.H_T);
                }
                else
                {
                    return LINESHAPE_IN(name, sv.line_param.basePlate, shape,
                                        sv.i_param.sizeB, sv.i_param.sizeH,
                                        sv.c_param.sizeB, sv.c_param.sizeH,
                                        sv.j_param.sizeB, sv.j_param.sizeH,
                                        PARTINGLEN::PART_FULL, PARTINGRULE::PART_RATIO,
                                        1200.0, 1200.0, 0.5, 0.5,
                                        sv.i_param.B_T, sv.i_param.H_T,
                                        sv.c_param.B_T, sv.c_param.H_T,
                                        sv.j_param.B_T, sv.j_param.H_T);
                }
            }
            else
            {
                qreal ilen = (sv.line_param.i_length > 1.0e-3) ? sv.line_param.i_length : 1200.0;
                qreal jlen = (sv.line_param.j_length > 1.0e-3) ? sv.line_param.j_length : 1200.0;
                return LINESHAPE_IN(name, QString(), shape,
                                    sv.i_param.sizeB, sv.i_param.sizeH,
                                    sv.c_param.sizeB, sv.c_param.sizeH,
                                    sv.j_param.sizeB, sv.j_param.sizeH,
                                    PARTINGLEN::PART_FULL, PARTINGRULE::PART_VALUE, ilen, jlen);
            }
        }
        else if (stype == STRUCTTYPE::STP_RC || stype == STRUCTTYPE::STP_SRC)
        {

            shape = sectionTypeToConcRenderShape(sv.i_param.concType);
            if (dtype == DATATYPE::TPCOLUMN || sv.line_param.isCantilever)
            {
                if (shape == RENDERSHAPE::SHAPE_PIPE)
                {
                    return LINESHAPE_IN(name, QString(), shape,
                                        sv.i_param.Dy, sv.i_param.Dy,
                                        sv.c_param.Dy, sv.c_param.Dy,
                                        sv.j_param.Dy, sv.j_param.Dy,
                                        PARTINGLEN::PART_INTERNAL, PARTINGRULE::PART_RATIO, 1200.0, 1200.0, 0.5, 0.5);
                }
                else
                {
                    return LINESHAPE_IN(name, QString(), shape,
                                        sv.i_param.Dy, sv.i_param.Dz,
                                        sv.c_param.Dy, sv.c_param.Dz,
                                        sv.j_param.Dy, sv.j_param.Dz,
                                        PARTINGLEN::PART_INTERNAL, PARTINGRULE::PART_RATIO, 1200.0, 1200.0, 0.5, 0.5);
                }
            }
            else
            {
                return LINESHAPE_IN(name, QString(), shape,
                                    sv.i_param.Dy, sv.i_param.Dz,
                                    sv.c_param.Dy, sv.c_param.Dz,
                                    sv.j_param.Dy, sv.j_param.Dz,
                                    PARTINGLEN::PART_INTERNAL, PARTINGRULE::PART_RATIO, 1200.0, 1200.0, 0.25, 0.25);
            }
        }
        return LINESHAPE_IN(name);
    }

    LINEPROPERTY ManageSectionData::judgeLineValue(const SectionValues &sv) const
    {
        qDebug() << "ManageSectionData::judgeLineValue";
        STRUCTTYPE stype = sv.sectionType;

        if (stype == STRUCTTYPE::STP_S || stype == STRUCTTYPE::STP_CFT)
        {

            qDebug() << "ManageSectionData::judgeLineValue S or CFT";
            RENDERSHAPE i_shape = sectionTypeToSteelRenderShape(sv.i_param.steelType);
            RENDERSHAPE c_shape = sectionTypeToSteelRenderShape(sv.c_param.steelType);
            RENDERSHAPE j_shape = sectionTypeToSteelRenderShape(sv.j_param.steelType);
            qDebug() << "ManageSectionData::judgeLineValue shape" << (int)sv.i_param.steelType << (int)sv.c_param.steelType << (int)sv.j_param.steelType;
            qDebug() << "ManageSectionData::judgeLineValue shape" << (int)i_shape << (int)c_shape << (int)j_shape;

            return LINEPROPERTY(sv.sectionName, sv.line_param.basePlate, stype,
                                i_shape, c_shape, j_shape,
                                sv.i_param.steelMaterial, sv.c_param.steelMaterial, sv.j_param.steelMaterial,
                                sv.i_param.sizeB, sv.i_param.sizeH, sv.i_param.t1, sv.i_param.t2,
                                sv.c_param.sizeB, sv.c_param.sizeH, sv.c_param.t1, sv.c_param.t2,
                                sv.j_param.sizeB, sv.j_param.sizeH, sv.j_param.t1, sv.j_param.t2);
        }
        else if (stype == STRUCTTYPE::STP_RC || stype == STRUCTTYPE::STP_SRC)
        {

            RENDERSHAPE i_shape = sectionTypeToConcRenderShape(sv.i_param.concType);
            RENDERSHAPE c_shape = sectionTypeToConcRenderShape(sv.c_param.concType);
            RENDERSHAPE j_shape = sectionTypeToConcRenderShape(sv.j_param.concType);
            QString cmat = sv.line_param.concMaterial;

            return LINEPROPERTY(sv.sectionName, sv.line_param.basePlate, stype,
                                i_shape, c_shape, j_shape,
                                cmat, cmat, cmat,
                                sv.i_param.Dy, sv.i_param.Dz, 0.0, 0.0,
                                sv.c_param.Dy, sv.c_param.Dz, 0.0, 0.0,
                                sv.j_param.Dy, sv.j_param.Dz, 0.0, 0.0);
        }
        return LINEPROPERTY();
    }

    RENDERSHAPE ManageSectionData::sectionTypeToSteelRenderShape(SteelType::Type type) const
    {
        if (type == SteelType::Type::ST_H || type == SteelType::Type::ST_WH)
        {
            return RENDERSHAPE::SHAPE_H;
        }
        else if (type == SteelType::Type::ST_I || type == SteelType::Type::ST_WI)
        {
            return RENDERSHAPE::SHAPE_I;
        }
        else if (type == SteelType::Type::ST_BX || type == SteelType::Type::ST_WBX || type == SteelType::Type::ST_WPL)
        {
            return RENDERSHAPE::SHAPE_BOX;
        }
        else if (type == SteelType::Type::ST_P || type == SteelType::Type::ST_WP || type == SteelType::Type::ST_WBR)
        {
            return RENDERSHAPE::SHAPE_PIPE;
        }
        else if (type == SteelType::Type::ST_T1 || type == SteelType::Type::ST_WT1)
        {
            return RENDERSHAPE::SHAPE_T1;
        }
        else if (type == SteelType::Type::ST_T2 || type == SteelType::Type::ST_WT2)
        {
            return RENDERSHAPE::SHAPE_T2;
        }
        else if (type == SteelType::Type::ST_T3 || type == SteelType::Type::ST_WT3)
        {
            return RENDERSHAPE::SHAPE_T3;
        }
        else if (type == SteelType::Type::ST_T4 || type == SteelType::Type::ST_WT4)
        {
            return RENDERSHAPE::SHAPE_T4;
        }
        else if (type == SteelType::Type::ST_C1 || type == SteelType::Type::ST_WC1)
        {
            return RENDERSHAPE::SHAPE_C1;
        }
        else if (type == SteelType::Type::ST_C2 || type == SteelType::Type::ST_WC2)
        {
            return RENDERSHAPE::SHAPE_C2;
        }
        else if (type == SteelType::Type::ST_C3 || type == SteelType::Type::ST_WC3)
        {
            return RENDERSHAPE::SHAPE_C3;
        }
        else if (type == SteelType::Type::ST_C4 || type == SteelType::Type::ST_WC4)
        {
            return RENDERSHAPE::SHAPE_C4;
        }
        else if (type == SteelType::Type::ST_L)
        {
            return RENDERSHAPE::SHAPE_L;
        }
        else if (type == SteelType::Type::ST_CROSS)
        {
            return RENDERSHAPE::SHAPE_CROSS;
        }
        else if (type == SteelType::Type::ST_CROSSH)
        {
            return RENDERSHAPE::SHAPE_CROSSH;
        }
        return RENDERSHAPE::SHAPE_BOX;
    }

    SECTIONWEIGHT ManageSectionData::judgeLineWeight(const UnifiedData &unified, const SectionValues &sv) const
    {
        DATATYPE dtype = sv.memberType;
        STRUCTTYPE stype = sv.sectionType;

        if (stype == STRUCTTYPE::STP_RC)
        {

            qreal sl = sv.line_param.finishLoad;
            qreal uw = unified.getMaterialValue(DATATYPE::TPMACONC, sv.line_param.concMaterial).unitW1;
            qreal a1 = calcArea_RC(sv.i_param.concType, sv.i_param.Dy, sv.i_param.Dz) / (1.0e+6);
            qreal a2 = (dtype == DATATYPE::TPCOLUMN) ? a1 : calcArea_RC(sv.c_param.concType, sv.c_param.Dy, sv.c_param.Dz) / (1.0e+6);
            qreal a3 = calcArea_RC(sv.j_param.concType, sv.j_param.Dy, sv.j_param.Dz) / (1.0e+6);
            qreal w1 = uw * a1 + sl / (1.0e+3);
            qreal w2 = (dtype == DATATYPE::TPCOLUMN) ? w1 : uw * a2 + sl / (1.0e+3);
            qreal w3 = uw * a3 + sl / (1.0e+3);
            return SECTIONWEIGHT(w1, w2, w3);
        }
        else if (stype == STRUCTTYPE::STP_SRC)
        {

            qreal sl = sv.line_param.finishLoad;
            qreal uw = unified.getMaterialValue(DATATYPE::TPMACONC,
                                                sv.line_param.concMaterial)
                           .unitW2;
            qreal a1 = calcArea_RC(sv.i_param.concType, sv.i_param.Dy, sv.i_param.Dz) / (1.0e+6);
            qreal a2 = (dtype == DATATYPE::TPCOLUMN) ? a1 : calcArea_RC(sv.c_param.concType, sv.c_param.Dy, sv.c_param.Dz) / (1.0e+6);
            qreal a3 = calcArea_RC(sv.j_param.concType, sv.j_param.Dy, sv.j_param.Dz) / (1.0e+6);
            qreal w1 = uw * a1 + sl / (1.0e+3);
            qreal w2 = (dtype == DATATYPE::TPCOLUMN) ? w1 : uw * a2 + sl / (1.0e+3);
            qreal w3 = uw * a3 + sl / (1.0e+3);
            return SECTIONWEIGHT(w1, w2, w3);
        }
        else if (stype == STRUCTTYPE::STP_S)
        {

            qreal sl = sv.line_param.finishLoad;
            qreal uw1 = unified.getMaterialValue(DATATYPE::TPMASTEEL,
                                                 sv.i_param.steelMaterial)
                            .unitW1;
            qreal uw2 = (dtype == DATATYPE::TPCOLUMN) ? uw1 : unified.getMaterialValue(DATATYPE::TPMASTEEL, sv.c_param.steelMaterial).unitW1;
            qreal uw3 = unified.getMaterialValue(DATATYPE::TPMASTEEL,
                                                 sv.j_param.steelMaterial)
                            .unitW1;
            qreal a1 = calcArea_S(sv.i_param) / (1.0e+6);
            qreal a2 = (dtype == DATATYPE::TPCOLUMN) ? a1 : calcArea_S(sv.c_param) / (1.0e+6);
            qreal a3 = calcArea_S(sv.j_param) / (1.0e+6);
            qreal w1 = uw1 * a1 + sl / (1.0e+3);
            qreal w2 = (dtype == DATATYPE::TPCOLUMN) ? w1 : uw2 * a2 + sl / (1.0e+3);
            qreal w3 = uw3 * a3 + sl / (1.0e+3);
            return SECTIONWEIGHT(w1, w2, w3);
        }
        else if (stype == STRUCTTYPE::STP_CFT)
        {

            qreal sl = sv.line_param.finishLoad;
            qreal uw1 = unified.getMaterialValue(DATATYPE::TPMASTEEL,
                                                 sv.i_param.steelMaterial)
                            .unitW1;
            qreal uw2 = unified.getMaterialValue(DATATYPE::TPMASTEEL,
                                                 sv.j_param.steelMaterial)
                            .unitW1;
            qreal a1 = calcArea_S(sv.i_param) / (1.0e+6);
            qreal a2 = calcArea_S(sv.j_param) / (1.0e+6);

            SteelType::Type stype1 = sv.i_param.steelType;
            SteelType::Type stype2 = sv.j_param.steelType;
            qreal uw3 = 23.0; // コンクリート比重 kN/m^3
            qreal b1 = 0.0, b2 = 0.0;
            if (stype1 == SteelType::Type::ST_WBX || stype1 == SteelType::Type::ST_BX)
            {
                b1 = (sv.i_param.sizeB - 2.0 * sv.i_param.t1) * (sv.i_param.sizeH - 2.0 * sv.i_param.t2) / (1.0e+6);
            }
            else if (stype1 == SteelType::Type::ST_WP || stype1 == SteelType::Type::ST_P)
            {
                b1 = M_PI * (sv.i_param.sizeH - 2.0 * sv.i_param.t1) * (sv.i_param.sizeH - 2.0 * sv.i_param.t1) /
                     4.0 / (1.0e+6);
            }
            if (stype2 == SteelType::Type::ST_WBX || stype2 == SteelType::Type::ST_BX)
            {
                b2 = (sv.j_param.sizeB - 2.0 * sv.j_param.t1) * (sv.j_param.sizeH - 2.0 * sv.j_param.t2) / (1.0e+6);
            }
            else if (stype2 == SteelType::Type::ST_WP || stype2 == SteelType::Type::ST_P)
            {
                b2 = M_PI * (sv.j_param.sizeH - 2.0 * sv.j_param.t1) * (sv.j_param.sizeH - 2.0 * sv.j_param.t1) /
                     4.0 / (1.0e+6);
            }

            qreal w1 = uw1 * a1 + uw3 * b1 + sl / (1.0e+3);
            qreal w2 = uw2 * a2 + uw3 * b2 + sl / (1.0e+3);
            return SECTIONWEIGHT(w1, w1, w2);
        }

        return SECTIONWEIGHT();
    }

    SECTIONWEIGHT ManageSectionData::judgePlaneWeight(const UnifiedData &unified, const SectionValues &sv) const
    {
        qreal uwc = 0.0, uws = 0.0;
        QString c_str = sv.plane_param.concMaterial;
        QString s_str = sv.plane_param.steelMaterial;
        qreal sl = sv.plane_param.finishLoad / 1000.0;

        if (!c_str.isEmpty())
        {
            qreal uw = unified.getMaterialValue(DATATYPE::TPMACONC, c_str).unitW1;
            uwc = uw * sv.plane_param.concThickness / 1000.0 + sl;
        }
        if (!s_str.isEmpty())
        {
            qreal uw = unified.getMaterialValue(DATATYPE::TPMASTEEL, s_str).unitW1;
            uws = uw * sv.plane_param.steelThickness / 1000.0 + sl;
        }

        return SECTIONWEIGHT(qMax(uwc, uws));
    }

    qreal ManageSectionData::calcArea_S(const LinePartsParameterSRC &param) const
    {
        SteelType::Type stype = param.steelType;
        qreal area_s = 0.0;

        if (SteelType::isStandardType(stype))
        {
            return param.standardArea();
        }

        qreal H = param.sizeH;
        qreal B = param.sizeB;
        qreal t1 = param.t1;
        qreal t2 = param.t2;

        switch (stype)
        {
        case SteelType::Type::ST_WH:  // WH - HxBxt1xt2
        case SteelType::Type::ST_WI:  // WI - HxBxt1xt2  Hが幅方向
        case SteelType::Type::ST_WC1: // WC - HxBxt1xt2
        case SteelType::Type::ST_WC2: // WC - HxBxt1xt2
        case SteelType::Type::ST_WC3: // WC - HxBxt1xt2
        case SteelType::Type::ST_WC4: // WC - HxBxt1xt2
            area_s = 2.0 * B * t2 + (H - 2.0 * t2) * t1;
            break;
        case SteelType::Type::ST_WT1: // WT - HxBxt1xt2
        case SteelType::Type::ST_WT2: // WT - HxBxt1xt2
        case SteelType::Type::ST_WT3: // WT - HxBxt1xt2
        case SteelType::Type::ST_WT4: // WT - HxBxt1xt2
            area_s = B * t2 + (H - t2) * t1;
            break;
        case SteelType::Type::ST_WBX: // BX - HxBxt1xt2
            area_s = 2.0 * B * t2 + 2.0 * (H - 2.0 * t2) * t1;
            break;
        case SteelType::Type::ST_WP: // P  - Hxt1
            area_s = M_PI * t1 * (H - t1);
            break;
        case SteelType::Type::ST_WPL: // PL - HxB
            area_s = H * B;
            break;
        case SteelType::Type::ST_WBR: // R  - H
            area_s = M_PI * H * H / 4.0;
            break;
        default:
            qFatal(QString{u8"calcArea_Sに定義されていないタイプ:" +  SteelType::enumToString(stype)}.toUtf8().data());
            break;
        }
        return area_s;
    }

    qreal ManageSectionData::calcArea_RC(ConcreteType::Type ctype, qreal Dx, qreal Dy) const
    {
        if (ctype == ConcreteType::Type::RECTANGLE)
            return Dx * Dy;
        return M_PI * Dx * Dx / 4.0;
    }

    RENDERSHAPE ManageSectionData::sectionTypeToConcRenderShape(ConcreteType::Type type) const
    {
        if (type == ConcreteType::Type::RECTANGLE)
        {
            return RENDERSHAPE::SHAPE_BOX;
        }
        else if (type == ConcreteType::Type::CIRCLE)
        {
            return RENDERSHAPE::SHAPE_PIPE;
        }
        return RENDERSHAPE::SHAPE_BOX;
    }

    QList<SectionValues> ManageSectionData::getSectionValues(DATATYPE dtype) const
    {
        QList<SectionValues> sv;
        if (dtype == DATATYPE::TPCOLUMN)
        {
            Q_FOREACH (QString column, columnNameList)
            {
                sv.append(columnTable.value(column)->getSectionValues());
            }
        }
        else if (dtype == DATATYPE::TPGIRDER)
        {
            Q_FOREACH (QString girder, girderNameList)
            {
                sv.append(girderTable.value(girder)->getSectionValues());
            }
        }
        else if (dtype == DATATYPE::TPBRACE)
        {
            Q_FOREACH (QString brace, braceNameList)
            {
                sv.append(braceTable.value(brace));
            }
        }
        else if (dtype == DATATYPE::TPBEAM)
        {
            Q_FOREACH (QString beam, beamNameList)
            {
                sv.append(beamTable.value(beam));
            }
        }
        else if (dtype == DATATYPE::TPSLAB)
        {
            Q_FOREACH (QString slab, slabNameList)
            {
                sv.append(slabTable.value(slab));
            }
        }
        else if (dtype == DATATYPE::TPWALL)
        {
            Q_FOREACH (QString wall, wallNameList)
            {
                sv.append(wallTable.value(wall)->getSectionValues());
            }
        }
        else if (dtype == DATATYPE::TPUWALL)
        {
            Q_FOREACH (QString uwall, uwallNameList)
            {
                sv.append(uwallTable.value(uwall));
            }
        }
        else if (dtype == DATATYPE::TPBASE)
        {
            Q_FOREACH (QString base, baseNameList)
            {
                sv.append(baseTable.value(base));
            }
        }
        else if (dtype == DATATYPE::TPWALLOPEN)
        {
            Q_FOREACH (QString open, openNameList)
            {
                sv.append(openTable.value(open)->getSetionValues());
            }
        }
        else if (dtype == DATATYPE::TPDAMPER)
        {
            Q_FOREACH (QString damper, damperNameList)
            {
                sv.append(damperTable.value(damper));
            }
        }
        else if (dtype == DATATYPE::TPISO)
        {
            Q_FOREACH (QString iso, isoNameList)
            {
                sv.append(isoTable.value(iso));
            }
        }
        return sv;
    }

    /* エラーメッセージ */
    void ManageSectionData::errorUndefinedName(QWidget *parent, const QStringList &names)
    {
        QMessageBox::critical(parent, u8"エラー -- 未定義な部材名", u8"未定義な部材名が使用されている。\n" + names.join(","), QMessageBox::Ok);
    }

    void ManageSectionData::errorOverlapNames(QWidget *parent, const QStringList &names)
    {
        QMessageBox::critical(parent, u8"エラー -- 重複した部材名", u8"部材名が重複している。\n" + names.join(","), QMessageBox::Ok);
    }

    /* 階別のリストクラス */
    SectionValues FloorOrderData::getValueOfLimitedFloors(const QUuid &f1, const QUuid &f2) const
    {
        Q_FOREACH (SectionValues cv, mySectionList)
        {
            if (cv.startFloorID == f1 && cv.endFloorID == f2)
                return cv;
        }
        return SectionValues(); // 名前が空欄でエラーを判別
    }

    SectionValues FloorOrderData::getSectionValueOfHeight(const UnifiedData &unified, qreal hh) const
    {
        /* hhが階位置に含まれない場合、直下階を所属とする。 */
        QUuid near_fid = unified.getNearlyDownFloorID(hh);

        Q_FOREACH (SectionValues cv, mySectionList)
        {
            qDebug() << "FloorOrderData::getSectionValueOfHeight" << cv.sectionName << cv.startFloorID << cv.endFloorID << near_fid;
            qDebug() << unified.getFloorHeight(cv.startFloorID) << unified.getFloorHeight(cv.endFloorID) << unified.getFloorHeight(near_fid);
            if (unified.withinFloors(cv.startFloorID, cv.endFloorID, near_fid))
                return cv;
        }
        qDebug() << "FloorOrderData::getSectionValueOfHeight section not found" << hh;

        return SectionValues(QString());
    }

    void FloorOrderData::updateFloorID()
    {

        for (int i = 0; i < mySectionList.count(); i++)
        {
            SectionValues cv = mySectionList.at(i);
            bool err = false;
            if (!FactoryUnifiedData::getInstance(isOutput)->isExistedID(cv.startFloorID, DATATYPE::TPFLOOR))
            {
                err = true;
                cv.startFloorID = QUuid();
            }
            if (!FactoryUnifiedData::getInstance(isOutput)->isExistedID(cv.endFloorID, DATATYPE::TPFLOOR))
            {
                err = true;
                cv.endFloorID = QUuid();
            }
            mySectionList.replace(i, cv);
        }
    }

    void FloorOrderData::changeSectionName(const QString &str)
    {
        mySectionName = str;
        for (int i = 0; i < mySectionList.count(); i++)
        {
            SectionValues cv = mySectionList.at(i);
            cv.sectionName = str;
            mySectionList.replace(i, cv);
        }
    }

    void FloorOrderData::replaceBaseName(const QString &str_a, const QString &str_b)
    {
        for (int i = 0; i < mySectionList.count(); i++)
        {
            SectionValues cv = mySectionList.at(i);
            if (cv.line_param.basePlate == str_a)
            {
                cv.line_param.basePlate = str_b;
                mySectionList.replace(i, cv);
            }
        }
    }

    void MultiSectionData::changeSectionName(const QString &str)
    {
        mySectionName = str;
        for (int i = 0; i < mySectionList.count(); i++)
        {
            SectionValues cv = mySectionList.at(i);
            cv.sectionName = str;
            mySectionList.replace(i, cv);
        }
    }

    QString ManageSectionData::writeData(QSharedPointer<N3DUuidTable> table, bool isNsv) const
    {
        QString text;
        text.append("*SECTION\n");
        text.append(writeColumnData(table));
        text.append(writeGirderData(table));
        text.append(writeBraceData(table));
        if (!isNsv)
        {
            text.append(writeBeamData(table));
        }
        text.append(writeSlabData(table, isNsv));
        text.append(writeWallData(table, isNsv));
        text.append(writeBasePlateData(table));
        text.append(writeWallOpenData(table, isNsv));
        text.append(writeDamperData(table));
        text.append(writeIsolatorData(table));
        return text;
    }

    QString ManageSectionData::writePsvData(QSharedPointer<N3DUuidTable> table) const
    {
        return writeData(table, false);
    }

    QString ManageSectionData::writeNsvData(QSharedPointer<N3DUuidTable> table) const
    {
        return writeData(table, true);
    }

    QString ManageSectionData::writeColumnData(QSharedPointer<N3DUuidTable> table) const
    {
        QString text;
        QString rcText, sText, srcText, cftText;
        int rcCount = 0;
        int sCount = 0;
        int srcCount = 0;
        int cftCount = 0;
        Q_FOREACH (QString column, columnNameList)
        {
            FloorOrderData *order = columnTable.value(column);
            Q_FOREACH (SectionValues sv, order->getSectionValues())
            {
                QString sec;
                sec.append(sv.writeData(table)).append("\n");
                switch (sv.sectionType)
                {
                case STRUCTTYPE::STP_RC:
                    rcText.append(sec);
                    rcCount++;
                    break;
                case STRUCTTYPE::STP_S:
                    sText.append(sec);
                    sCount++;
                    break;
                case STRUCTTYPE::STP_SRC:
                    srcText.append(sec);
                    srcCount++;
                    break;
                case STRUCTTYPE::STP_CFT:
                    cftText.append(sec);
                    cftCount++;
                    break;
                }
            }
        }
        text.append("**COLUMN\n");
        text.append("***RC").append(",").append(QString::number(rcCount)).append("\n");
        text.append(u8"#符号, 階範囲(階～), 階範囲(～階), 仕上重量(N/m), コンクリート材種, ");
        text.append(u8"柱脚Dy(mm), 柱脚Dz(mm), 柱脚主筋本数(y), 柱脚主筋本数(z), 柱脚主筋径(y), 柱脚主筋径(z), 柱脚芯鉄筋位置ay(mm), 柱脚芯鉄筋位置az(mm), 柱脚芯鉄筋aDy(mm), 柱脚芯鉄筋aDz(mm), 柱脚芯鉄筋本数(y), 柱脚芯鉄筋本数(z), 柱脚芯鉄筋径(y), 柱脚芯鉄筋径(z), 柱脚帯筋本数(y), 柱脚帯筋本数(z), 柱脚帯筋径, 柱脚帯筋ピッチ(mm), 柱脚部材表面-主筋芯の距離RK(mm), 柱脚寄せ筋・段筋の距離(芯芯)RY(mm), ");
        text.append(u8"柱頭Dy(mm), 柱頭Dz(mm), 柱頭主筋本数(y), 柱頭主筋本数(z), 柱頭主筋径(y), 柱頭主筋径(z), 柱頭芯鉄筋位置ay(mm), 柱頭芯鉄筋位置az(mm), 柱頭芯鉄筋aDy(mm), 柱頭芯鉄筋aDz(mm), 柱頭芯鉄筋本数(y), 柱頭芯鉄筋本数(z), 柱頭芯鉄筋径(y), 柱頭芯鉄筋径(z), 柱頭帯筋本数(y), 柱頭帯筋本数(z), 柱頭帯筋径, 柱脚帯筋ピッチ(mm), 柱脚部材表面-主筋芯の距離RK(mm), 柱脚寄せ筋・段筋の距離(芯芯)RY(mm)\n");
        text.append(rcText);
        text.append("***S").append(",").append(QString::number(sCount)).append("\n");
        text.append(u8"#符号, 階範囲(階～), 階範囲(～階), 仕上重量(N/m), 鉄骨断面切り替え高さ(mm), 柱脚リスト, ");
        text.append(u8"柱脚鋼材種, 柱脚断面タイプ, 柱脚断面選択, 柱脚鉄骨断面H(mm), 柱脚鉄骨断面B(mm), 柱脚鉄骨断面t1(mm), 柱脚鉄骨断面t2(mm), 柱脚十字断面H(mm), 柱脚十字断面B(mm), 柱脚十字断面t1(mm), 柱脚十字断面t2(mm), ");
        text.append(u8"柱頭鋼材種, 柱頭断面タイプ, 柱頭断面選択, 柱頭鉄骨断面H(mm), 柱頭鉄骨断面B(mm), 柱頭鉄骨断面t1(mm), 柱頭鉄骨断面t2(mm), 柱頭十字断面H(mm), 柱頭十字断面B(mm), 柱頭十字断面t1(mm), 柱頭十字断面t2(mm)\n");
        text.append(sText);
        text.append("***SRC").append(",").append(QString::number(srcCount)).append("\n");
        text.append(u8"#符号, 階範囲(階～), 階範囲(～階), 仕上重量(N/m), コンクリート材種, 中詰めコンクリート材種, 鉄骨断面切り替え高さ(mm), 柱脚リスト, ");
        text.append(u8"柱脚Dy(mm), 柱脚Dz(mm), 柱脚主筋本数(y), 柱脚主筋本数(z), 柱脚主筋径(y), 柱脚主筋径(z), 柱脚芯鉄筋位置ay(mm), 柱脚芯鉄筋位置az(mm), 柱脚芯鉄筋aDy(mm), 柱脚芯鉄筋aDz(mm), 柱脚芯鉄筋本数(y), 柱脚芯鉄筋本数(z), 柱脚芯鉄筋径(y), 柱脚芯鉄筋径(z), 柱脚帯筋本数(y), 柱脚帯筋本数(z), 柱脚帯筋本数(y), 柱脚帯筋本数(z), 柱脚帯筋ピッチ(mm), 柱脚部材表面-主筋芯の距離間隔RK(mm), 柱脚寄せ筋・段筋の距離(芯芯)RY(mm), ");
        text.append(u8"柱脚鋼材種, 柱脚断面タイプ(y), 柱脚断面選択(y), 柱脚鉄骨断面(y)H(mm), 柱脚鉄骨断面(y)B(mm), 柱脚鉄骨断面(y)t1(mm), 柱脚鉄骨断面(y)t2(mm), 柱脚鉄骨断面(y)Ry(mm), 柱脚鉄骨断面(y)Rz(mm), 柱脚断面タイプ(z), 柱脚断面選択(z), 柱脚鉄骨断面(z)H(mm), 柱脚鉄骨断面(z)B(mm), 柱脚鉄骨断面(z)t1(mm), 柱脚鉄骨断面(z)t2(mm), 柱脚鉄骨位置(z)Ry(mm), 柱脚鉄骨位置(z)Rz(mm), ");
        text.append(u8"柱頭Dy(mm), 柱頭Dz(mm), 柱頭主筋本数(y), 柱頭主筋本数(z), 柱頭主筋径(y), 柱頭主筋径(z), 柱頭芯鉄筋位置ay(mm), 柱頭芯鉄筋位置az(mm), 柱頭芯鉄筋aDy(mm), 柱頭芯鉄筋aDz(mm), 柱頭芯鉄筋本数(y), 柱頭芯鉄筋本数(z), 柱頭芯鉄筋径(y), 柱頭芯鉄筋径(z), 柱頭帯筋本数(y), 柱頭帯筋本数(z), 柱頭帯筋本数(y), 柱頭帯筋本数(z), 柱頭帯筋ピッチ(mm), 柱頭部材表面-主筋芯の距離間隔RK(mm), 柱頭寄せ筋・段筋の距離(芯芯)RY(mm), ");
        text.append(u8"柱頭鋼材種, 柱頭断面タイプ(y), 柱頭断面選択(y), 柱頭鉄骨断面(y)H(mm), 柱頭鉄骨断面(y)B(mm), 柱頭鉄骨断面(y)t1(mm), 柱頭鉄骨断面(y)t2(mm), 柱頭鉄骨断面(y)Ry(mm), 柱頭鉄骨断面(y)Rz(mm), 柱頭断面タイプ(z), 柱頭断面選択(z), 柱頭鉄骨断面(z)H(mm), 柱頭鉄骨断面(z)B(mm), 柱頭鉄骨断面(z)t1(mm), 柱頭鉄骨断面(z)t2(mm), 柱頭鉄骨位置(z)Ry(mm), 柱頭鉄骨位置(z)Rz(mm)\n");
        text.append(srcText);
        text.append("***CFT").append(",").append(QString::number(cftCount)).append("\n");
        text.append(u8"#符号, 階範囲(階～), 階範囲(～階), 仕上重量(N/m), 中詰めコンクリート材種, 鉄骨断面切り替え高さ(mm), 柱脚リスト, ");
        text.append(u8"柱脚鋼材種, 柱脚断面タイプ, 柱脚断面選択, 柱脚鉄骨断面H(mm), 柱脚鉄骨断面B(mm), 柱脚鉄骨断面t1(mm), 柱脚鉄骨断面t2(mm), ");
        text.append(u8"柱頭鋼材種, 柱頭断面タイプ, 柱頭断面選択, 柱頭鉄骨断面H(mm), 柱頭鉄骨断面B(mm), 柱頭鉄骨断面t1(mm), 柱頭鉄骨断面t2(mm)\n");
        text.append(cftText);
        return text;
    }

    QString ManageSectionData::writeGirderData(QSharedPointer<N3DUuidTable> table) const
    {
        QString text;
        QString rcText, sText, srcText;
        int rcCount = 0;
        int sCount = 0;
        int srcCount = 0;
        Q_FOREACH (QString girder, girderNameList)
        {
            FloorOrderData *order = girderTable.value(girder);
            Q_FOREACH (SectionValues sv, order->getSectionValues())
            {
                QString sec;
                sec.append(sv.writeData(table)).append("\n");
                switch (sv.sectionType)
                {
                case STRUCTTYPE::STP_RC:
                    rcText.append(sec);
                    rcCount++;
                    break;
                case STRUCTTYPE::STP_S:
                    sText.append(sec);
                    sCount++;
                    break;
                case STRUCTTYPE::STP_SRC:
                    srcText.append(sec);
                    srcCount++;
                    break;
                }
            }
        }
        text.append("**GIRDER\n");
        text.append("***RC").append(",").append(QString::number(rcCount)).append("\n");
        text.append(u8"#符号, 階範囲(階～), 階範囲(～階), 一般／片持, 仕上重量(N/m), コンクリート材種, ");
        text.append(u8"左端Dy(mm), 左端Dz(mm), 左端上端筋, 左端下端筋, 左端上端筋径, 左端下端筋径, 左端帯筋本数, 左端帯筋径, 左端帯筋ピッチ(mm), 左端部材表面-主筋芯の距離RK(mm), 左端部材表面-主筋芯の距離（上面）RKu(mm), 左端部材表面-主筋芯の距離（下面）RKb(mm), 左端主筋距離RY(mm), ");
        text.append(u8"中央Dy(mm), 中央Dz(mm), 中央上端筋, 中央下端筋, 中央上端筋径, 中央下端筋径, 中央帯筋本数, 中央帯筋径, 中央帯筋ピッチ(mm), 中央部材表面-主筋芯の距離RK(mm), 中央部材表面-主筋芯の距離（上面）RKu(mm), 中央部材表面-主筋芯の距離（下面）RKb(mm), 中央主筋距離RY(mm), ");
        text.append(u8"右端Dy(mm), 右端Dz(mm), 右端上端筋, 右端下端筋, 右端上端筋径, 右端下端筋径, 右端帯筋本数, 右端帯筋径, 右端帯筋ピッチ(mm), 右端部材表面-主筋芯の距離RK(mm), 右端部材表面-主筋芯の距離（上面）RKu(mm), 右端部材表面-主筋芯の距離（下面）RKb(mm), 右端主筋距離RY(mm)\n");
        text.append(rcText);
        text.append("***S").append(",").append(QString::number(sCount)).append("\n");
        text.append(u8"#符号, 階範囲(階～), 階範囲(～階), 一般／片持, 仕上重量(N/m), i端長さ(mm), j端長さ(mm),　i端補剛あり, j端補剛あり,");
        text.append(u8"左端鋼材種, 左端鋼材種(ウェブ), 左端鉄骨形状, 左端規格断面名, 左端H(mm), 左端B(mm), 左端t1(mm), 左端t2(mm),");
        text.append(u8"中央鋼材種, 中央鋼材種(ウェブ), 中央鉄骨形状, 中央規格断面名, 中央H(mm), 中央B(mm), 中央t1(mm), 中央t2(mm),");
        text.append(u8"右端鋼材種, 右端鋼材種(ウェブ), 右端鉄骨形状, 右端規格断面名, 右端H(mm), 右端B(mm), 右端t1(mm), 右端t2(mm)\n");
        text.append(sText);
        text.append("***SRC").append(",").append(QString::number(srcCount)).append("\n");
        text.append(u8"#符号, 階範囲(階～), 階範囲(～階), 一般／片持, 仕上重量(N/m), コンクリート材種, i端長さ(mm), j端長さ(mm),");
        text.append(u8"左端Dy(mm), 左端Dz(mm), 左端上端筋, 左端下端筋, 左端上端筋径, 左端下端筋径, 左端帯筋本数, 左端帯筋径, 左端帯筋ピッチ(mm), 左端部材表面-主筋芯の距離RK(mm), 左端部材表面-主筋芯の距離（上面）RKu(mm), 左端部材表面-主筋芯の距離（下面）RKb(mm), 左端主筋距離RY(mm), 左端段筋の距離(芯芯、一段目)RY1(mm),　i左端鋼材種, 左端鋼材種(ウェブ), 左端鉄骨形状, 左端規格断面名, 左端H(mm), 左端B(mm), 左端t1(mm), 左端t2(mm), 左端鉄骨位置X(mm), 左端鉄骨位置Y(mm),");
        text.append(u8"中央Dy(mm), 中央Dz(mm), 中央上端筋, 中央下端筋, 中央上端筋径, 中央下端筋径, 中央帯筋本数, 中央帯筋径, 中央帯筋ピッチ(mm), 中央部材表面-主筋芯の距離RK(mm), 中央部材表面-主筋芯の距離（上面）RKu(mm), 中央部材表面-主筋芯の距離（下面）RKb(mm), 中央主筋距離RY(mm), 中央段筋の距離(芯芯、一段目)RY1(mm),　i中央鋼材種, 中央鋼材種(ウェブ), 中央鉄骨形状, 中央規格断面名, 中央H(mm), 中央B(mm), 中央t1(mm), 中央t2(mm), 中央鉄骨位置X(mm), 中央鉄骨位置Y(mm),");
        text.append(u8"右端Dy(mm), 右端Dz(mm), 右端上端筋, 右端下端筋, 右端上端筋径, 右端下端筋径, 右端帯筋本数, 右端帯筋径, 右端帯筋ピッチ(mm), 右端部材表面-主筋芯の距離RK(mm), 右端部材表面-主筋芯の距離（上面）RKu(mm), 右端部材表面-主筋芯の距離（下面）RKb(mm), 右端主筋距離RY(mm), 右端段筋の距離(芯芯、一段目)RY1(mm),　i右端鋼材種, 右端鋼材種(ウェブ), 右端鉄骨形状, 右端規格断面名, 右端H(mm), 右端B(mm), 右端t1(mm), 右端t2(mm), 右端鉄骨位置X(mm), 右端鉄骨位置Y(mm)\n");
        text.append(srcText);
        return text;
    }

    QString ManageSectionData::writeBraceData(QSharedPointer<N3DUuidTable> table) const
    {
        QString sText;
        sText.append("**BRACE\n");
        sText.append("***S").append(",").append(QString::number(braceNameList.count())).append("\n");
        sText.append(u8"#符号, 階範囲(階～),　階範囲(～階),　仕上重量(N/m),　圧縮引張, i端長さ(mm), j端長さ(mm), 直接入力断面積(cm2), 直接入力有効断面積(cm2), 直接入力細長比λ, 直接入力単位重量(N/m),");
        sText.append(u8"左端鋼材種, 左端鉄骨形状, 左端規格断面名, 左端H(mm), 左端B(mm), 左端t1(mm), 左端t2(mm),");
        sText.append(u8"中央鋼材種, 中央鉄骨形状, 中央規格断面名, 中央H(mm), 中央B(mm), 中央t1(mm), 中央t2(mm),");
        sText.append(u8"右端鋼材種, 右端鉄骨形状, 右端規格断面名, 右端H(mm), 右端B(mm), 右端t1(mm), 右端t2(mm)\n");
        Q_FOREACH (SectionValues sv, braceTable)
        {
            sText.append(sv.writeData(table)).append("\n");
        }
        return sText;
    }

    QString ManageSectionData::writeBeamData(QSharedPointer<N3DUuidTable> table) const
    {
        QString text;
        QString rcText, sText, srcText;
        int rcCount = 0;
        int sCount = 0;
        int srcCount = 0;
        Q_FOREACH (SectionValues sv, beamTable)
        {
            QString sec;
            sec.append(sv.writeData(table)).append("\n");
            switch (sv.sectionType)
            {
            case STRUCTTYPE::STP_RC:
                rcText.append(sec);
                rcCount++;
                break;
            case STRUCTTYPE::STP_S:
                sText.append(sec);
                sCount++;
                break;
            case STRUCTTYPE::STP_SRC:
                srcText.append(sec);
                srcCount++;
                break;
            }
        }
        text.append("**BEAM\n");
        text.append("***RC").append(",").append(QString::number(rcCount)).append("\n");
        text.append(u8"#符号, 一般／片持, 仕上重量(N/m), コンクリート材種,");
        text.append(u8"左端Dy(mm), 左端Dz(mm), 左端上端筋, 左端下端筋, 左端上端筋径, 左端下端筋径, 左端帯筋本数, 左端帯筋径, 左端帯筋ピッチ(mm), 左端部材表面-主筋芯の距離RK(mm), 左端部材表面-主筋芯の距離（上面）RKu(mm), 左端部材表面-主筋芯の距離（下面）RKb(mm), 左端主筋距離RY(mm), ");
        text.append(u8"中央Dy(mm), 中央Dz(mm), 中央上端筋, 中央下端筋, 中央上端筋径, 中央下端筋径, 中央帯筋本数, 中央帯筋径, 中央帯筋ピッチ(mm), 中央部材表面-主筋芯の距離RK(mm), 中央部材表面-主筋芯の距離（上面）RKu(mm), 中央部材表面-主筋芯の距離（下面）RKb(mm), 中央主筋距離RY(mm), ");
        text.append(u8"右端Dy(mm), 右端Dz(mm), 右端上端筋, 右端下端筋, 右端上端筋径, 右端下端筋径, 右端帯筋本数, 右端帯筋径, 右端帯筋ピッチ(mm), 右端部材表面-主筋芯の距離RK(mm), 右端部材表面-主筋芯の距離（上面）RKu(mm), 右端部材表面-主筋芯の距離（下面）RKb(mm), 右端主筋距離RY(mm)\n");
        text.append(rcText);
        text.append("***S").append(",").append(QString::number(sCount)).append("\n");
        text.append(u8"#符号, 一般／片持, 仕上重量(N/m), i端長さ(mm), j端長さ(mm),");
        text.append(u8"左端鋼材種, 左端鋼材種(ウェブ), 左端鉄骨形状, 左端規格断面名, 左端H(mm), 左端B(mm), 左端t1(mm), 左端t2(mm),");
        text.append(u8"中央鋼材種, 中央鋼材種(ウェブ), 中央鉄骨形状, 中央規格断面名, 中央H(mm), 中央B(mm), 中央t1(mm), 中央t2(mm),");
        text.append(u8"右端鋼材種, 右端鋼材種(ウェブ), 右端鉄骨形状, 右端規格断面名, 右端H(mm), 右端B(mm), 右端t1(mm), 右端t2(mm)\n");
        text.append(sText);
        text.append("***SRC").append(",").append(QString::number(srcCount)).append("\n");
        text.append(u8"#符号, 一般／片持, 仕上重量(N/m), コンクリート材種, i端長さ(mm), j端長さ(mm),");
        text.append(u8"左端Dy(mm), 左端Dz(mm), 左端上端筋, 左端下端筋, 左端上端筋径, 左端下端筋径, 左端帯筋本数, 左端帯筋径, 左端帯筋ピッチ(mm), 左端部材表面-主筋芯の距離RK(mm), 左端部材表面-主筋芯の距離（上面）RKu(mm), 左端部材表面-主筋芯の距離（下面）RKb(mm), 左端主筋距離RY(mm), 左端段筋の距離(芯芯、一段目)RY1(mm),　i左端鋼材種, 左端鋼材種(ウェブ), 左端鉄骨形状, 左端規格断面名, 左端H(mm), 左端B(mm), 左端t1(mm), 左端t2(mm), 左端鉄骨位置X(mm), 左端鉄骨位置Y(mm),");
        text.append(u8"中央Dy(mm), 中央Dz(mm), 中央上端筋, 中央下端筋, 中央上端筋径, 中央下端筋径, 中央帯筋本数, 中央帯筋径, 中央帯筋ピッチ(mm), 中央部材表面-主筋芯の距離RK(mm), 中央部材表面-主筋芯の距離（上面）RKu(mm), 中央部材表面-主筋芯の距離（下面）RKb(mm), 中央主筋距離RY(mm), 中央段筋の距離(芯芯、一段目)RY1(mm),　i中央鋼材種, 中央鋼材種(ウェブ), 中央鉄骨形状, 中央規格断面名, 中央H(mm), 中央B(mm), 中央t1(mm), 中央t2(mm), 中央鉄骨位置X(mm), 中央鉄骨位置Y(mm),");
        text.append(u8"右端Dy(mm), 右端Dz(mm), 右端上端筋, 右端下端筋, 右端上端筋径, 右端下端筋径, 右端帯筋本数, 右端帯筋径, 右端帯筋ピッチ(mm), 右端部材表面-主筋芯の距離RK(mm), 右端部材表面-主筋芯の距離（上面）RKu(mm), 右端部材表面-主筋芯の距離（下面）RKb(mm), 右端主筋距離RY(mm), 右端段筋の距離(芯芯、一段目)RY1(mm),　i右端鋼材種, 右端鋼材種(ウェブ), 右端鉄骨形状, 右端規格断面名, 右端H(mm), 右端B(mm), 右端t1(mm), 右端t2(mm), 右端鉄骨位置X(mm), 右端鉄骨位置Y(mm)\n");
        text.append(srcText);
        return text;
    }

    QString ManageSectionData::writeSlabData(QSharedPointer<N3DUuidTable> table, bool isNsv) const
    {
        //slabの板要素・平面応力要素はこちら
        bool isPsv = table == nullptr;
        QString tempSlab;
        int slabListCount = 0;
        Q_FOREACH (SectionValues sv, slabTable)
        {
            if (!isPsv && sv.plane_param.planeType == PlaneType::Type::UNDEFINED)
                continue;
            if (!isPsv && sv.plane_param.planeType == PlaneType::Type::NONSTRUCT)
                continue;
            tempSlab.append(sv.writeData(table)).append("\n");
            slabListCount++;
        }
        QString sText;
        sText.append("**SLAB").append(",").append(QString::number(slabListCount)).append("\n");
        sText.append(u8"#符号, スラブ種別, コンクリート材種, コンクリート厚(mm), 鋼材種,　鋼板厚(mm), 仕上げ重量(N/m2), 荷重方向(度),");
        sText.append(u8"短辺A部-端部 上側径, 短辺A部-端部 上側ピッチ(mm), 短辺A部-端部 下側径, 短辺A部-端部 下側ピッチ(mm),");
        sText.append(u8"短辺A部-中央 上側径, 短辺A部-中央 上側ピッチ(mm), 短辺A部-中央 下側径, 短辺A部-中央 下側ピッチ(mm),");
        sText.append(u8"短辺B部-端中 上側径, 短辺B部-端中 上側ピッチ(mm), 短辺B部-端中 下側径, 短辺B部-端中 下側ピッチ(mm),");
        sText.append(u8"長辺A部-端部 上側径, 長辺A部-端部 上側ピッチ(mm), 長辺A部-端部 下側径, 長辺A部-端部 下側ピッチ(mm),");
        sText.append(u8"長辺A部-中央 上側径, 長辺A部-中央 上側ピッチ(mm), 長辺A部-中央 下側径, 長辺A部-中央 下側ピッチ(mm),");
        sText.append(u8"長辺B部-端中 上側径, 長辺B部-端中 上側ピッチ(mm), 長辺B部-端中 下側径, 長辺B部-端中 下側ピッチ(mm)\n");
        sText.append(tempSlab);
        return sText;
    }

    QString ManageSectionData::writeWallData(QSharedPointer<N3DUuidTable> table, bool isNsv) const
    {
        //TODO:isNsvのとき、板要素・平面応力要素は別で計上？
        QString rcText;
        QString sText;
        QString plateText;
        QString membraneText;
        int rcCount = 0;
        int sCount = 0;
        int plateCount = 0;
        int membraneCount = 0;
        Q_FOREACH (QString wall, wallNameList)
        {
            FloorOrderData *order = wallTable.value(wall);
            Q_FOREACH (SectionValues sv, order->getSectionValues())
            {
                QString sec;
                sec.append(sv.writeData(table)).append("\n");
                if (isNsv && sv.plane_param.planeType == PlaneType::Type::UNDEFINED)
                    continue;
                if (isNsv && sv.plane_param.planeType == PlaneType::Type::NONSTRUCT)
                    continue;
                if (isNsv && sv.plane_param.planeType == PlaneType::Type::PLATE)
                    plateText.append(sec);
                    plateCount++;
                    continue;
                if (isNsv && sv.plane_param.planeType == PlaneType::Type::MEMBRANE)
                    membraneText.append(sec);
                    membraneCount++;
                    continue;

                switch (sv.sectionType)
                {
                case STRUCTTYPE::STP_RC:
                    rcText.append(sec);
                    rcCount++;
                    break;
                case STRUCTTYPE::STP_S:
                    sText.append(sec);
                    sCount++;
                    break;
                default:
                    rcText.append(sec);
                    rcCount++;
                    break;
                }
            }
        }
        QString text;
        text.append("**WALL\n");
        text.append("***NormalWall").append(",").append(QString::number(rcCount)).append("\n");
        text.append(u8"#符号, 階範囲(階～), 階範囲(～階),　壁種別, コンクリート材種, コンクリート厚(mm), 鋼材種, 鋼板厚(mm), 仕上げ重量(N/m2), 配筋タイプ,　縦筋径, 縦筋ピッチ(mm), 横筋径, 横筋ピッチ(mm),");
        text.append(u8"小開口補強筋 縦筋本数, 小開口補強筋 縦筋径, 小開口補強筋　横筋本数, 小開口補強筋 横筋径,　小開口補強筋 斜め筋本数, 小開口補強筋 斜め筋径,");
        text.append(u8"大開口補強筋 縦筋本数, 大開口補強筋 縦筋径, 大開口補強筋　横筋本数, 大開口補強筋 横筋径,　大開口補強筋 斜め筋本数, 大開口補強筋 斜め筋径\n");
        text.append(rcText);

        text.append("***SteelWall").append(",").append(QString::number(sCount)).append("\n");
        text.append(u8"#符号, 階範囲(階～), 階範囲(～階),　壁種別, 鋼材種, 鋼板厚(mm), 仕上げ重量(N/m2),");
        text.append(u8"縦リブピッチ(mm), 縦リブタイプ, 縦リブ厚(mm), 縦リブ幅(mm),");
        text.append(u8"横リブピッチ(mm), 横リブタイプ, 横リブ厚(mm), 横リブ幅(mm)\n");
        text.append(sText);

        //TODO:板要素・平面応力要素の出力
        text.append("***plateWall").append(",").append(QString::number(plateCount)).append("\n");
        text.append(plateText);

        text.append("***membraneWall").append(",").append(QString::number(membraneCount)).append("\n");
        text.append(membraneText);


        QString tempUwall;
        int uwallCount = 0;
        Q_FOREACH (SectionValues sv, uwallTable)
        {
            tempUwall.append(sv.writeData(table)).append("\n");
            uwallCount++;
        }
        text.append("***FoundationWall").append(",").append(QString::number(uwallCount)).append("\n");
        text.append(u8"#符号, コンクリート材種, コンクリート厚(mm), 鋼材種, 鋼板厚(mm), 仕上げ重量(N/m2),");
        text.append(u8"外側縦筋上部径, 外側縦筋上部ピッチ(mm), 外側縦筋中部径, 外側縦筋中部ピッチ(mm), 外側縦筋下部径, 外側縦筋下部ピッチ(mm), 外側横筋端部径, 外側横筋端部ピッチ(mm), 外側横筋中央径, 外側横筋中央ピッチ(mm),");
        text.append(u8"内側縦筋上部径, 内側縦筋上部ピッチ(mm), 内側縦筋中部径, 内側縦筋中部ピッチ(mm), 内側縦筋下部径, 内側縦筋下部ピッチ(mm), 内側横筋端部径, 内側横筋端部ピッチ(mm), 内側横筋中央径, 内側横筋中央ピッチ(mm)\n");
        text.append(tempUwall);

        return text;


    }

    QString ManageSectionData::writeBasePlateData(QSharedPointer<N3DUuidTable> table) const
    {
        int sCount = 0;
        int srcCount = 0;
        QString sText;
        QString srcText;
        Q_FOREACH (SectionValues sv, baseTable)
        {
            if (sv.sectionType == STRUCTTYPE::STP_S)
            {
                sText.append(sv.writeData(table)).append("\n");
                sCount++;
            }
            else if (sv.sectionType == STRUCTTYPE::STP_SRC)
            {
                srcText.append(sv.writeData(table)).append("\n");
                srcCount++;
            }
        }
        QString text;
        text.append("**BASEPLATE\n");
        text.append("***S").append(",").append(QString::number(sCount)).append("\n");
        text.append(u8"#符号, 在来品/既製品, 規格名, B(mm), D(mm), t(mm), 鋼材種, 縁端距離dty(mm), 縁端距離dtz(mm), ボルト本数ny(本), ボルト本数nz(本), ボルト呼び径(mm), ボルト材種, アンカー長さlb(mm)\n");
        text.append(sText);
        text.append("***SRC").append(",").append(QString::number(srcCount)).append("\n");
        text.append(u8"#符号, BPL端あきB0(mm), BPL端あきD0(mm), コンクリート材種, 形状タイプ, 部材幅B(mm), 部材せいD(mm), 部材厚t(mm), 隅切り長SML(mm), 縁端距離hy1(mm), 縁端距離hy2(mm), 縁端距離hz1(mm), 縁端距離hz2(mm), 配置タイプ, ボルト本数ny1(本), ボルト本数ny2(本), ボルト本数nz1(本), ボルト本数nz2(本), ボルト呼び径, ボルト材種\n");
        text.append(srcText);
        return text;
    }

    QString ManageSectionData::writeWallOpenData(QSharedPointer<N3DUuidTable> table, bool isNsv) const
    {
        QString openText;
        Q_FOREACH (QString open, openNameList)
        {
            MultiSectionData *data = openTable.value(open);
            Q_FOREACH (SectionValues sv, data->getSetionValues())
            {
                openText.append(sv.writeData(table)).append("\n");
            }
        }
        QString sText;
        sText.append("**WALLOPEN").append(",").append(QString::number(openNameList.count())).append("\n");
        sText.append(u8"#符号, 開口種別, 開口位置左右基準, 開口位置左右, 開口位置上下基準, 開口位置上下,　開口原点Ox(mm), 開口原点Oy(mm), 開口幅(mm), 開口高さ(mm), スリットL(mm), スリットR(mm), スリットB(mm), スリットT(mm)\n");
        sText.append(openText);
        if (!isNsv)
        {
            // 開口のスリット
        }
        return sText;
    }

    QString ManageSectionData::writeDamperData(QSharedPointer<N3DUuidTable> table) const
    {
        QString sText;
        sText.append("**DAMPER").append(",").append(QString::number(damperNameList.count())).append("\n");
        sText.append(u8"#符号, 種類, 重量(kN), 方向ベクトルX, 方向ベクトルY, 方向ベクトルZ, パラメータ(/区切り),静的解析用固有周期T1(s), 静的解析用最大変形, モデル化フラグ, 付帯梁に自動設定する左右剛域長の和,　ダンパーの作用方向, 状態T解析に用いるダンパー部荷重(kN), 履歴面積低減係数, 架構抵抗剛性\n");
        Q_FOREACH (SectionValues sv, damperTable)
        {
            sText.append(sv.writeData(table)).append("\n");
        }
        return sText;
    }

    QString ManageSectionData::writeIsolatorData(QSharedPointer<N3DUuidTable> table) const
    {
        QString sText;
        sText.append("**ISOLATOR").append(",").append(QString::number(isoNameList.count())).append("\n");
        sText.append(u8"#符号, 種類, 規格名, 復元力特性(せん断), 初期剛性hK1(kN/m), 二次剛性hK2(kN/m), 三次剛性hK3(kN/m), 降伏荷重hFy1p(kN), 降伏荷重hFy1m(kN), 降伏荷重hFy2p(kN), 降伏荷重hFy2m(kN), 復元力特性(軸), 初期剛性vK1(kN/m), 引張二次剛性vK2+(kN/m), 引張三次剛性vK3+(kN/m), 引張降伏荷重vFy1+(kN), 引張降伏荷重vFy2+(kN), 部材重量(kN), 表示形状, 表示サイズD(mm), 表示サイズH(mm), 設置高さL(mm), 割線剛性計算変形(mm), せん断モデルのばね数, 反曲点高さ比\n");
        Q_FOREACH (SectionValues sv, isoTable)
        {
            sText.append(sv.writeData(table)).append("\n");
        }
        return sText;
    }

} // namespace post3dapp
