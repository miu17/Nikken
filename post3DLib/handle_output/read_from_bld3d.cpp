#include "read_from_bld3d.h"

#include <QtCore>
#include <QFile>
#include <QStringList>

#include "calc_vector2d.h"
#include "define_material_struct.h"
#include "interpreter_csv.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "unified_inputdata.h"
#include "unified_output.h"

namespace post3dapp
{
    void readFromBld3d::readCsvData(QFile *file)
    {
        readCsv2(file);
        QSharedPointer<InterpreterCsv> interpreter = interpretCsv();
        interpreter->makeDefinedData();
        makeUnifiedData();
        divideMembersByJoints();
        deleteIndependentJoints();
    }

    QSharedPointer<InterpreterCsv> readFromBld3d::interpretCsv()
    {
        QSharedPointer<InterpreterCsv> interpreter = QSharedPointer<InterpreterCsv>(new InterpreterCsv());
        floorHash = interpretFloors(interpreter);
        interpreter->framegroups = interpretFrames();
        frameHash = setFramePointList(interpreter);
        interpreter->materials = interpretMaterials();
        mergedJointHash = interpretJoints();
        interpretColumnSectionValues(interpreter);
        interpretGirderSectionValues(interpreter);
        interpretBraceSectionValues(interpreter);
        interpretBeamSectionValues(interpreter);
        interpretPostSectionValues(interpreter);
        interpretSlabSectionValues(interpreter);
        interpretUwallSectionValues(interpreter);
        interpretWallSectionValues(interpreter);
        interpretSteelWallSectionValues(interpreter);
        interpretWallOpen(interpreter);
        interpretJointLoadSectionValues(interpreter);
        interpretMemberLoadSectionValues(interpreter);
        interpretMemberCsv();
        interpretPlateCsv(interpreter);

        // 組み合わせる必要があるのでslabLoadは後から
        interpretSlabLoadSectionValues(interpreter);

        return interpreter;
    }

    QHash<int, FLOORVALUES> readFromBld3d::interpretFloors(QSharedPointer<InterpreterCsv> interpreter) const
    {
        QHash<int, FLOORVALUES> hash;
        Q_FOREACH (QStringList strlist, floorLines)
        {
            int floorId = strlist.at(1).toInt();
            FLOORVALUES fl;
            fl.UUID = QUuid::createUuid();
            fl.floorName = strlist.at(3);
            Q_FOREACH (QStringList str, strctLines)
            {
                QString floor = str.at(2);
                if (fl.floorName == str.at(2))
                {
                    fl.beamConcrete = str.at(7);
                    fl.slabConcrete = str.at(7);
                    break;
                }
            }
            fl.floorHeight = strlist.at(4).toDouble() / 1000.0;
            switch (strlist.at(5).toInt())
            {
            case 1:
                fl.floorType = FLOORTYPE::FL_UNDERGROUND;
                break;
            case 2:
                fl.floorType = FLOORTYPE::FL_OVERGROUND;
                break;
            case 4:
                fl.floorType = FLOORTYPE::FL_PENTHOUSE;
                break;
            default:
                fl.floorType = FLOORTYPE::FL_NOTYPE;
                break;
            }
            hash.insert(floorId, fl);
        }

        // QList<FLOORVALUES>は下の階から順に渡さないといけない
        Q_FOREACH (FLOORVALUES f, hash.values())
        {
            if (interpreter->sortedFloors.count() == 0)
            {
                interpreter->sortedFloors.append(f);
            }
            else
            {
                bool isHeighest = true;
                for (int i = 0; i < interpreter->sortedFloors.count(); i++)
                {
                    if (f.floorHeight < interpreter->sortedFloors.at(i).floorHeight)
                    {
                        interpreter->sortedFloors.insert(i, f);
                        isHeighest = false;
                        break;
                    }
                }
                if (isHeighest)
                    interpreter->sortedFloors.append(f);
            }
        }

        Q_FOREACH (FLOORVALUES f, interpreter->sortedFloors)
        {
            if (f.floorType == FLOORTYPE::FL_OVERGROUND)
            {
                interpreter->baseFloor = f.UUID;
            }
            if (f.floorType == FLOORTYPE::FL_PENTHOUSE)
            {
                interpreter->roofFloor = f.UUID;
            }
        }

        return hash;
    }

    QList<FRAMEGROUPVALUE> readFromBld3d::interpretFrames() const
    {
        QList<FRAMEGROUPVALUE> temp;
        // 先に数と最大spanを確認する。
        QHash<QUuid, int> count;    // framegroupsの数
        QHash<QUuid, qreal> length; // framegroupsの最大span

        QStringList groupNames;
        Q_FOREACH (QStringList strlist, gridLines)
        {
            QString groupName = strlist.at(2);
            if (!groupNames.contains(groupName))
            {
                groupNames.append(groupName);
                FRAMEGROUPVALUE fgv;
                fgv.GID = QUuid::createUuid();
                fgv.groupName = groupName;
                fgv.basePoint = QPointF(strlist.at(6).toDouble() / 1000.0, strlist.at(7).toDouble() / 1000.0);
                fgv.startAngle = strlist.at(8).toDouble();
                switch (strlist.at(5).toInt())
                {
                case 0: // 直線通り
                    fgv.frameType = FrameType::FRAMETYPE::STRAIGHT;
                    fgv.plusLength = 50;
                    fgv.minusLength = 10;
                    break;
                case 1: // 円周通り
                    fgv.frameType = FrameType::FRAMETYPE::CIRCLE;
                    // fgv.endAngle = strlist.at(9).toDouble(); csvが角度を持っていない
                    fgv.endAngle = 359.0; // 暫定
                    break;
                case 2: // 半径通り
                    fgv.frameType = FrameType::FRAMETYPE::RADIUS;
                    fgv.plusLength = 50;
                    break;
                case 3: // 直径
                case 4: // 任意点列
                case 5: // 閉じた円
                case 6: // 閉じた点列
                    break;
                }
                temp.append(fgv);
                count.insert(fgv.GID, 1);
            }
            else
            {
                FRAMEGROUPVALUE fgv;
                Q_FOREACH (FRAMEGROUPVALUE f, temp)
                {
                    if (f.groupName == groupName)
                    {
                        fgv = f;
                        break;
                    }
                }
                if (fgv.frameType == FrameType::FRAMETYPE::CIRCLE)
                {
                    qreal totalspan = length.value(fgv.GID);
                    qreal span = strlist.at(4).toDouble() / 1000.0;
                    if (totalspan < span)
                    {
                        length.remove(fgv.GID);
                        length.insert(fgv.GID, span);
                    }
                }

                int no = count.value(fgv.GID) + 1;
                count.insert(fgv.GID, no);
            }

            qDebug() << strlist.at(2) << strlist.at(3);
        }

        // division数
        QList<FRAMEGROUPVALUE> groups;
        Q_FOREACH (FRAMEGROUPVALUE f, temp)
        {
            f.count = count.value(f.GID);
            if (f.frameType == FrameType::FRAMETYPE::CIRCLE)
            {
                Q_FOREACH (FRAMEGROUPVALUE fgv, temp)
                {
                    if (fgv.GID == f.GID)
                        continue;
                    if (fgv.frameType == FrameType::FRAMETYPE::RADIUS && f.basePoint == fgv.basePoint)
                    {
                        f.division = count.value(fgv.GID) * 6; // 仮に6倍にしておく
                        f.startAngle = fgv.startAngle;         // radius側からもってくる
                        groups.append(f);
                        break;
                    }
                }
            }
            else if (f.frameType == FrameType::FRAMETYPE::RADIUS)
            {
                Q_FOREACH (FRAMEGROUPVALUE fgv, temp)
                {
                    if (fgv.GID == f.GID)
                        continue;
                    if (fgv.frameType == FrameType::FRAMETYPE::CIRCLE && f.basePoint == fgv.basePoint)
                    {
                        f.plusLength = length.value(fgv.GID);
                        groups.append(f);
                        break;
                    }
                }
            }
            else
            {
                groups.append(f);
            }
        }

        return groups;
    }

    QList<MATERIALVALUES> readFromBld3d::interpretMaterials() const
    {
        QList<MATERIALVALUES> temp;
        Q_FOREACH (QStringList strlist, mtralLines)
        {
            MATERIALVALUES material;
            QString str = strlist.at(1).trimmed();
            if (str == u8"コン")
            {
                material.materialType = MaterialType::Type::CONCRETE;
                // TODO : csvへの出力追加を依頼する必要がある
                // material.unitW1 = strlist.at(5).toDouble();
                // material.unitW2 = strlist.at(6).toDouble();
                // material.E = strlist.at(7).toDouble();
                // material.G = strlist.at(8).toDouble();
                // material.F = strlist.at(9).toDouble();
                // material.Ft_L = strlist.at(10).toDouble();
                // material.Ft_S = strlist.at(11).toDouble();
                // material.Ft_U = strlist.at(12).toDouble();
                // material.Fc_L = strlist.at(13).toDouble();
                // material.Fc_S = strlist.at(14).toDouble();
                // material.Fc_U = strlist.at(15).toDouble();
                // material.Fs_L = strlist.at(16).toDouble();
                // material.Fs_S = strlist.at(17).toDouble();
                // material.Fs_U = strlist.at(18).toDouble();
                // material.Fa_L = strlist.at(19).toDouble();
                // material.Fa_S = strlist.at(20).toDouble();
                // material.Fa_U = strlist.at(21).toDouble();
                // material.Fa_Lu
                // material.Fa_Su
                // material.modularRatio = strlist.at(22).toDouble(); //
            }
            else if (str == u8"鉄筋")
            {
                material.materialType = MaterialType::Type::REINFORCE;
                QString min = strlist.at(3);
                QString replace;
                if (min.startsWith(u8"D"))
                {
                    material.symbol = u8"D";
                    replace = u8"^D";
                }
                else if (min.startsWith(u8"E"))
                {
                    material.symbol = u8"E";
                    replace = u8"^E";
                }
                else if (min.startsWith(u8"F"))
                {
                    material.symbol = u8"F";
                    replace = u8"^E";
                }
                else if (min.startsWith(u8"S"))
                {
                    material.symbol = u8"S";
                    replace = u8"^S";
                }
                else if (min.startsWith(u8"R"))
                {
                    material.symbol = u8"R";
                    replace = u8"^R";
                }
                else if (min.startsWith(u8"U"))
                {
                    material.symbol = u8"U";
                    replace = u8"^U";
                }
                else
                {
                    material.symbol = u8"D";
                }

                material.barMin = min.replace(replace, QString()).toInt();
                QString max = strlist.at(4);
                material.barMax = max.replace(replace, QString()).toInt();

                material.materialType = MaterialType::Type::REINFORCE;
                // TODO
                // material.E = strlist.at(7).toDouble();
                // material.Ft_L = strlist.at(10).toDouble();
                // material.Ft_S = strlist.at(11).toDouble();
                // material.Ft_U = strlist.at(12).toDouble();
                // material.Fs_L = strlist.at(16).toDouble();
                // material.Fs_S = strlist.at(17).toDouble();
                // material.Fs_U = strlist.at(18).toDouble();
            }
            else if (str == u8"鉄骨")
            {
                material.materialType = MaterialType::Type::STEEL;
                // TODO
                // material.unitW1 = strlist.at(5).toDouble();
                // material.E = strlist.at(7).toDouble();
                // material.G = strlist.at(8).toDouble();
                // material.F = strlist.at(9).toDouble();
                // material.Ft_L = strlist.at(10).toDouble();
                // material.Ft_S = strlist.at(11).toDouble();
                // material.Ft_U = strlist.at(12).toDouble();
                // material.Fs_L = strlist.at(16).toDouble();
                // material.Fs_S = strlist.at(17).toDouble();
                // material.Fs_U = strlist.at(18).toDouble();
            }
            material.materialName = strlist.at(2).trimmed();
            temp.append(material);
        }
        return temp;
    }

    void readFromBld3d::readCsv2(QFile *file)
    {
        qDebug() << "read init";
        QTextStream infile(file);
        QTextCodec *codec = QTextCodec::codecForName("Shift-JIS");
        infile.setCodec(codec);

        while (!infile.atEnd())
        {
            QString str = infile.readLine(0);
            QStringList strlist = str.split(",");
            QStringList value;
            for (int i = 0; i < strlist.count(); i++)
            {
                value.append(strlist.at(i).trimmed());
            }
            QString keyword = value.at(0);
            if (keyword == "STRCT")
            {
                strctLines.append(value);
            }
            else if (keyword == "FLOOR")
            {
                floorLines.append(value);
            }
            else if (keyword == "GRID")
            {
                gridLines.append(value);
            }
            else if (keyword == "MTRAL")
            {
                mtralLines.append(value);
            }
            else if (keyword == "JOINT")
            {
                jointLines.append(value);
            }
            else if (keyword == "MEMBR")
            {
                memberLines.append(value);
            }
            else if (keyword == "PLATE")
            {
                plateLines.append(value);
            }
            else if (keyword == "OPEN")
            {
                openLines.append(value);
            }
            else if (keyword == "MLOAD")
            {
                mloadvalue.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "JLOAD")
            {
                jloadvalue.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "CLST1")
            {
                colvalue1.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "CLST2")
            {
                colvalue2.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "CLST3")
            {
                colvalue3.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "CLST4")
            {
                colvalue4.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "CLST5")
            {
                colvalue5.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "GLST1")
            {
                girvalue1.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "GLST2")
            {
                girvalue2.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "GLST3")
            {
                girvalue3.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "GLST4")
            {
                girvalue4.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "GLST5")
            {
                girvalue5.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "GLST6")
            {
                girvalue6.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "GLST7")
            {
                girvalue7.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "BLST1")
            {
                beamvalue1.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "BLST2")
            {
                beamvalue2.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "BLST3")
            {
                beamvalue3.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "BLST4")
            {
                beamvalue4.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "BLST5")
            {
                beamvalue5.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "BLST6")
            {
                beamvalue6.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "BLST7")
            {
                beamvalue7.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "BRLS1")
            {
                bracevalue1.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "BRLS2")
            {
                bracevalue2.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "SLST1")
            {
                slabvalue1.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "SLST2")
            {
                slabvalue2.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "SLST3")
            {
                slabvalue3.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "WLST1")
            {
                wallvalue1.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "WLST2")
            {
                wallvalue2.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "FWLST1")
            {
                uwallvalue1.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "FWLST2")
            {
                uwallvalue2.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "FWLST3")
            {
                uwallvalue3.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "OLIST")
            {
                wolistvalue.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "PLIST1")
            {
                plistvalue1.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "PLIST2")
            {
                plistvalue2.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "PLIST3")
            {
                plistvalue3.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "PLIST4")
            {
                plistvalue4.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "PLIST5")
            {
                plistvalue5.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "SWLS1")
            {
                swallvalue1.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "SWLS2")
            {
                swallvalue2.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "FLLIST")
            {
                fllistvalue.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "LLLIST")
            {
                lllistvalue.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "MLLIST")
            {
                mllistvalue.insert(value.at(1).toInt(), value);
            }
            else if (keyword == "JLLIST")
            {
                jllistvalue.insert(value.at(1).toInt(), value);
            }
        }
    }

    /*
    QString readFromBld3d::getFloorName(const QUuid& uuid) const{
        Q_FOREACH(FLOORVALUES fv, sortedFloors){
            if (fv.UUID == uuid)
                return fv.floorName;
        }
        return QString();
    }

    QString readFromBld3d::getFrameName(const QUuid &uuid) const{
        Q_FOREACH(FRAMEPOINTSVALUE fp, frameHash.values()){
            if (fp.UUID == uuid)
                return  fp.frameName;
        }
        return QString();
    }
    */

    void readFromBld3d::divideMembersByJoints() const
    {
        QList<JointData *> jdlist = UnifiedInputData::getInInstance()->jointListOfAll();

        QList<MemberData *> m_mdlist;
        QList<QList<JointData *>> m_jdlist;
        Q_FOREACH (JointData *jd, jdlist)
        {
            QList<MemberData *> mdlist = searchContainMembers(jd);
            Q_FOREACH (MemberData *md, mdlist)
            {
                if (!m_mdlist.contains(md))
                {
                    m_mdlist.append(md);
                    m_jdlist.append(QList<JointData *>() << jd);
                }
                else
                {
                    int idx = m_mdlist.indexOf(md);
                    QList<JointData *> jlist = m_jdlist.at(idx);
                    jlist.append(jd);
                    m_jdlist.replace(idx, jlist);
                }
            }
        }
        if (m_mdlist.isEmpty())
            return;

        QList<QUuid> stackedMembers;
        QList<QList<QUuid>> stackedInteriorJoints;

        for (int i = 0; i < m_mdlist.count(); i++)
        {
            stackedMembers.append(m_mdlist.at(i)->getUuid());
            QList<QUuid> j_idlist;
            Q_FOREACH (JointData *jj, m_jdlist.at(i))
                j_idlist.append(jj->getUuid());
            stackedInteriorJoints.append(j_idlist);
        }

        QList<MemberData *> mdlist = UnifiedInputData::getInInstance()->memberIDsToPointers(stackedMembers);
        QList<int> countList;
        Q_FOREACH (MemberData *md, mdlist)
            countList.append(md->jointCount());

        QList<MemberData *> changed_mdlist;
        QList<JointData *> changed_jdlist;

        for (int i = 0; i < mdlist.count(); i++)
        {

            MemberData *md = mdlist.at(i);
            QList<JointData *> jdlist = UnifiedInputData::getInInstance()->jointIDsToPointers(
                stackedInteriorJoints.at(i));

            if (UnifiedInputData::getInInstance()->interiorMember(md, jdlist))
            {

                if (!changed_mdlist.contains(md))
                    changed_mdlist.append(md);

                if (md->dataType() == DATATYPE::TPCOLUMN || md->isGirderType())
                {
                    Q_FOREACH (JointData *jd, jdlist)
                    {
                        Q_FOREACH (MemberData *mm, jd->getComposeMembers())
                        {
                            if (!mm->isGirderType())
                                continue;
                            if (!changed_mdlist.contains(mm))
                                changed_mdlist.append(mm);
                        }
                    }
                }

                Q_FOREACH (JointData *jd, jdlist)
                    if (!changed_jdlist.contains(jd))
                        changed_jdlist.append(jd);
            }
        }

        if (!changed_mdlist.isEmpty())
        {
            Q_FOREACH (MemberData *md, changed_mdlist)
                md->recalcLineLength();
        }

        // 内分できなかった部材をスタックから消去
        for (int i = mdlist.count() - 1; i >= 0; i--)
        {
            if (countList.at(i) == mdlist.at(i)->jointCount())
            {
                stackedMembers.removeAt(i);
                stackedInteriorJoints.removeAt(i);
            }
        }
    }
    QList<MemberData *> readFromBld3d::searchContainMembers(JointData *jd) const
    {
        QList<MemberData *> mdlist;

        Q_FOREACH (MemberData *md, UnifiedInputData::getInInstance()->memberListOfType(DATATYPE::TPCOLUMN))
        {
            if (isOnLinePoint(jd, md) == true)
                mdlist.append(md);
        }
        Q_FOREACH (MemberData *md, UnifiedInputData::getInInstance()->memberListOfType(DATATYPE::TPGIRDER))
        {
            if (isOnLinePoint(jd, md) == true)
                mdlist.append(md);
        }
        Q_FOREACH (MemberData *md, UnifiedInputData::getInInstance()->memberListOfType(DATATYPE::TPBRACE))
        {
            if (isOnLinePoint(jd, md) == true)
                mdlist.append(md);
        }
        Q_FOREACH (MemberData *md, UnifiedInputData::getInInstance()->memberListOfType(DATATYPE::TPBEAM))
        {
            if (isOnLinePoint(jd, md) == true)
                mdlist.append(md);
        }

        return mdlist;
    }
    bool readFromBld3d::isOnLinePoint(JointData *jd, MemberData *md) const
    {
        QList<JointData *> jdlist = md->getJointList();
        foreach (JointData *endjd, jdlist)
        {
            if (jd == endjd)
                return false;
        }
        qreal eps = 0.001;
        qreal x1 = md->iJoint()->xPos();
        qreal y1 = md->iJoint()->yPos();
        qreal z1 = md->iJoint()->zPos();
        qreal x2 = md->jJoint()->xPos();
        qreal y2 = md->jJoint()->yPos();
        qreal z2 = md->jJoint()->zPos();
        qreal x0 = jd->xPos();
        qreal y0 = jd->yPos();
        qreal z0 = jd->zPos();

        if (abs((x2 - x1) * (y0 - y1) - (y2 - y1) * (x0 - x1)) > eps)
            return false;
        if (abs((x2 - x1) * (z0 - z1) - (z2 - z1) * (x0 - x1)) > eps)
            return false;
        if ((qMin(x1, x2) - eps <= x0 && x0 <= qMax(x1, x2) + eps) && (qMin(y1, y2) - eps <= y0 && y0 <= qMax(y1, y2) + eps) && (qMin(z1, z2) - eps <= z0 && z0 <= qMax(z1, z2) + eps))
            return true;
        return false;
    }
    void readFromBld3d::deleteIndependentJoints() const
    {
        QList<JointData *> delJoints;
        Q_FOREACH (JointData *jd, UnifiedInputData::getInInstance()->jointListOfAll())
        {
            if (jd->isIndependentJoint())
                delJoints.append(jd);
        }

        QList<QUuid> stackedJoints;
        QList<GlobalPoint> stackedPoints;

        QList<QUuid> stackedMembers;
        QList<QList<QUuid>> stackedMemberJoints;
        QList<MEMBERVALUES> stackedMemberValues;

        Q_FOREACH (JointData *jd, delJoints)
        {
            stackedJoints.append(jd->getUuid());
            stackedPoints.append(jd->toGlobalPoint());
            Q_FOREACH (MemberData *md, jd->getRelatedMembers())
            {
                QUuid m_id = md->getUuid();
                if (!stackedMembers.contains(m_id))
                {
                    stackedMembers.append(m_id);
                    stackedMemberValues.append(md->getMemberValues());
                    QList<QUuid> j_idlist;
                    Q_FOREACH (JointData *jj, md->getJointList())
                        j_idlist.append(jj->getUuid());
                    stackedMemberJoints.append(j_idlist);
                }
            }
        }

        QList<JointData *> jdlist = UnifiedInputData::getInInstance()->jointIDsToPointers(stackedJoints);
        UnifiedInputData::getInInstance()->removeJoints(jdlist, false);
    }

    // <<<<<<< HEAD:post3DLib/handle_output/read_from_bld3d.cpp
    // QPair<QUuid, QUuid> readFromBld3d::getFloorUuid(const QHash<int, FLOORVALUES>& floorHash, const QString& first, const QString& second) const{
    // =======
//    void readFromBld3d::makeDefinedData()
//    {

//        UnifiedInputData::getInInstance()->clearDefinedData();

//        UnifiedInputData::getInInstance()->registerTempData(sortedFloors);
//        QUuid base;
//        QUuid roof;
//        Q_FOREACH (FLOORVALUES f, sortedFloors)
//        {
//            if (f.floorType == FLOORTYPE::FL_OVERGROUND)
//            {
//                base = f.UUID;
//            }
//            if (f.floorType == FLOORTYPE::FL_PENTHOUSE)
//            {
//                roof = f.UUID;
//            }
//        }

//        UnifiedInputData::getInInstance()->registerAnalysisFloorData(AnalysisFloors, base, roof);
//        UnifiedInputData::getInInstance()->registerTempData(framegroups, frames);
//        UnifiedInputData::getInInstance()->registerTempData(materials);

//        QList<SectionValues> columnValues;
//        columnValues.append(rcColumnHash.values());
//        columnValues.append(sColumnHash.values());
//        columnValues.append(srcColumnHash.values());
//        columnValues.append(cftColumnHash.values());
//        UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPCOLUMN, columnValues);

//        QList<SectionValues> girderValues;
//        girderValues.append(rcGirderHash.values());
//        girderValues.append(sGirderHash.values());
//        girderValues.append(srcGirderHash.values());
//        UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPGIRDER, girderValues);

//        UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPBRACE, sBraceHash.values());
//        QList<SectionValues> beamValues;
//        beamValues.append(rcBeamHash.values());
//        beamValues.append(sBeamHash.values());
//        beamValues.append(srcBeamHash.values());
//        UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPBEAM, beamValues);
//        UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPSLAB, rcSlabHash.values());
//        UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPUWALL, rcUwallHash.values());
//        UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPWALL, rcAndSWallHash.values());
//        UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPWALLOPEN, wallOpenHash.values());

//        UnifiedInputData::getInInstance()->registerTempData(jointLoadListHash.values());
//        /*
//        FactoryUnifiedData::getInstance(isOutput)->registerTempData(memberLoadValues);
//       */
//        UnifiedInputData::getInInstance()->registerTempData(slabLoadListHash.values());
//    }

//    void readFromBld3d::makeUnifiedData()
//    {
//        UnifiedInputData::getInInstance()->clearUnifiedData();

//        QHashIterator<int, GlobalPoint> global(globalPointHash);
//        while (global.hasNext())
//        {
//            global.next();
//            JointData *jd = UnifiedInputData::getInInstance()->createAnalysisJoint(global.value(), global.key(), false, QUuid());
//            jointDataHash.insert(global.key(), jd);
//        }

//        QHashIterator<int, QList<int>> column(columnJointHash);
//        while (column.hasNext())
//        {
//            column.next();
//            QList<JointData *> jd;
//            Q_FOREACH (int no, column.value())
//            {
//                jd.append(jointDataHash.value(no));
//            }
//            MEMBERVALUES mem = columnHash.value(column.key());
//            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPCOLUMN, mem.sectionName, QUuid());
//            md->setCodeAngle(mem.codeAngle);
//        }

//        QHashIterator<int, QList<int>> girder(girderJointHash);
//        while (girder.hasNext())
//        {
//            girder.next();
//            QList<JointData *> jd;
//            Q_FOREACH (int no, girder.value())
//            {
//                jd.append(jointDataHash.value(no));
//            }
//            MEMBERVALUES mem = girderHash.value(girder.key());
//            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPGIRDER, mem.sectionName, QUuid());
//            md->setCodeAngle(mem.codeAngle);
//        }

//        QHashIterator<int, QList<int>> brace(braceJointHash);
//        while (brace.hasNext())
//        {
//            brace.next();
//            QList<JointData *> jd;
//            Q_FOREACH (int no, brace.value())
//            {
//                jd.append(jointDataHash.value(no));
//            }
//            MEMBERVALUES mem = braceHash.value(brace.key());
//            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPBRACE, mem.sectionName, QUuid());
//            md->setCodeAngle(mem.codeAngle);
//        }

//        QHashIterator<int, QList<int>> beam(beamJointHash);
//        while (beam.hasNext())
//        {
//            beam.next();
//            QList<JointData *> jd;
//            Q_FOREACH (int no, beam.value())
//            {
//                jd.append(jointDataHash.value(no));
//            }
//            MEMBERVALUES mem = beamHash.value(beam.key());
//            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPBEAM, mem.sectionName, QUuid());
//            md->setCodeAngle(mem.codeAngle);
//        }

//        QHashIterator<int, QList<int>> slab(slabJointHash);
//        while (slab.hasNext())
//        {
//            slab.next();
//            QList<JointData *> jd;
//            Q_FOREACH (int no, slab.value())
//            {
//                jd.append(jointDataHash.value(no));
//            }
//            MEMBERVALUES mem = slabHash.value(slab.key());
//            UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPSLAB, mem.sectionName, QUuid());
//        }

//        QHashIterator<int, QList<int>> uwall(uwallJointHash);
//        while (uwall.hasNext())
//        {
//            uwall.next();
//            QList<JointData *> jd;
//            Q_FOREACH (int no, uwall.value())
//            {
//                jd.append(jointDataHash.value(no));
//            }
//            MEMBERVALUES mem = uwallHash.value(uwall.key());
//            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPUWALL, mem.sectionName, QUuid());
//            md->setWallOpen(mem.wallOpenName);
//        }

//        QHashIterator<int, QList<int>> wall(wallJointHash);
//        while (wall.hasNext())
//        {
//            wall.next();
//            QList<JointData *> jd;
//            Q_FOREACH (int no, wall.value())
//            {
//                jd.append(jointDataHash.value(no));
//            }
//            MEMBERVALUES mem = wallHash.value(wall.key());
//            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPWALL, mem.sectionName, QUuid());
//            md->setWallOpen(mem.wallOpenName);
//        }

//        QHashIterator<int, QStringList> jointLoad(jloadvalue);
//        while (jointLoad.hasNext())
//        {
//            jointLoad.next();
//            QStringList list = jointLoad.value();
//            QList<JointData *> jlist;
//            jlist.append(jointDataHash.value(list.at(4).toInt()));
//            QString name = jointLoadListHash.value(list.at(3).toInt()).loadName;
//            UnifiedInputData::getInInstance()->createMember(jlist, DATATYPE::TPJOINTLOAD, name, QUuid());
//        }

//        /*
//        QHashIterator<int, QList<int>> memberLoad(memberLoadJointHash);
//        while (memberLoad.hasNext()) {
//            memberLoad.next();
//            QList<JointData *> jd;
//            Q_FOREACH(int no, memberLoad.value()){jd.append(jointDataHash.value(no));}
//            QString name = listNoToName.value(memberNoToListNo.value(memberLoad.key()));
//             FactoryUnifiedData::getInstance(isOutput)->createMember(jd, DATATYPE::TPMEMBERLOAD, name, QUuid());
//        }
//        */

//        QHashIterator<int, QList<int>> slabLoad(slabLoadJointHash);
//        while (slabLoad.hasNext())
//        {
//            slabLoad.next();
//            QList<JointData *> jd;
//            Q_FOREACH (int no, slabLoad.value())
//            {
//                jd.append(jointDataHash.value(no));
//            }
//            MEMBERVALUES mem = slabLoadHash.value(slabLoad.key());
//            UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPSLABLOAD, mem.sectionName, QUuid());
//        }

//        QList<JointData *> delJoints;
//        Q_FOREACH (JointData *jd, UnifiedInputData::getInInstance()->jointListOfAll())
//        {
//            if (jd->isIndependentJoint())
//                delJoints.append(jd);
//        }
//        UnifiedInputData::getInInstance()->removeJoints(delJoints, false);

//        QList<JointData *> jdlist = UnifiedInputData::getInInstance()->jointListOfAll();
//        QList<MemberData *> m_mdlist;
//        QList<QList<JointData *>> m_jdlist;
//        Q_FOREACH (JointData *jd, jdlist)
//        {
//            QList<MemberData *> mdlist = searchContainMembers(jd);
//            Q_FOREACH (MemberData *md, mdlist)
//            {
//                if (!m_mdlist.contains(md))
//                {
//                    m_mdlist.append(md);
//                    m_jdlist.append(QList<JointData *>() << jd);
//                }
//                else
//                {
//                    int idx = m_mdlist.indexOf(md);
//                    QList<JointData *> jlist = m_jdlist.at(idx);
//                    jlist.append(jd);
//                    m_jdlist.replace(idx, jlist);
//                }
//            }
//        }
//        // TODO:内分点が2つ以上ある場合のjointList内の順序が適当
//        for (int i = 0; i < m_mdlist.count(); i++)
//        {
//            QList<JointData *> memberJoints = m_mdlist.at(i)->getJointList();
//            QList<JointData *> temp;
//            temp << memberJoints.first() << m_jdlist.at(i) << memberJoints.last();
//            UnifiedInputData::getInInstance()->interiorMember(m_mdlist.at(i), temp);
//        }
//    }
    QPair<QUuid, QUuid> readFromBld3d::getFloorUuid(const QHash<int, FLOORVALUES> &floorHash, const QString &first, const QString &second) const
    {

        //     QPair<QUuid, QUuid> readFromBld3d::getFloorUuid(const QString &first, const QString &second) const
        //     {
        // >>>>>>> OutputDiagrams:post3DLib/data_unified/read_from_bld3d.cpp
        // start, endの順番に返す
        FLOORVALUES f;
        FLOORVALUES s;
        Q_FOREACH (FLOORVALUES floor, floorHash.values())
        {
            if (floor.floorName == first)
                f = floor;
            if (floor.floorName == second)
                s = floor;
        }
        if (f.floorHeight < s.floorHeight)
            return QPair<QUuid, QUuid>(f.UUID, s.UUID);
        else
            return QPair<QUuid, QUuid>(s.UUID, f.UUID);
    }

    void readFromBld3d::interpretColumnSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        QHashIterator<int, QStringList> column(colvalue1);
        while (column.hasNext())
        {
            column.next();
            int key = column.key();
            QStringList list = column.value();
            SectionValues sectionValues;
            sectionValues.memberType = DATATYPE::TPCOLUMN;
            QString type = list.at(6);
            if (type == "RC")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_RC;
                sectionValues.line_param.concMaterial = list.at(9);
            }
            else if (type == "S")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_S;
            }
            else if (type == "SRC")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_SRC;
                sectionValues.line_param.concMaterial = list.at(9);
            }
            else if (type == "SC")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_CFT;
                sectionValues.line_param.filledConcMaterial = list.at(10);
            }
            sectionValues.sectionName = list.at(2);
            sectionNameHash.insert(key, list.at(2));
            QPair<QUuid, QUuid> floors = getFloorUuid(floorHash, list.at(4), list.at(5));
            sectionValues.startFloorID = floors.first;
            sectionValues.endFloorID = floors.second;

            if (colvalue2.keys().contains(key))
            {
                sectionValues.j_param = SetColumnRcParameter(colvalue2.value(key));
            }
            if (colvalue3.keys().contains(key))
            {
                sectionValues.i_param = SetColumnRcParameter(colvalue3.value(key));
            }
            if (colvalue4.keys().contains(key))
            {
                sectionValues.j_param = SetColumnSteelParameter(colvalue4.value(key), sectionValues.j_param);
            }
            if (colvalue5.keys().contains(key))
            {
                sectionValues.i_param = SetColumnSteelParameter(colvalue5.value(key), sectionValues.i_param);
            }
            if (sectionValues.sectionType == STRUCTTYPE::STP_RC)
            {
                interpreter->rcColumnList.append(sectionValues);
            }
            else if (sectionValues.sectionType == STRUCTTYPE::STP_S)
            {
                interpreter->sColumnList.append(sectionValues);
            }
            else if (sectionValues.sectionType == STRUCTTYPE::STP_SRC)
            {
                interpreter->srcColumnList.append(sectionValues);
            }
            else if (sectionValues.sectionType == STRUCTTYPE::STP_CFT)
            {
                interpreter->cftColumnList.append(sectionValues);
            }
        }
    }
    void readFromBld3d::interpretGirderSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        QHashIterator<int, QStringList> girder(girvalue1);
        while (girder.hasNext())
        {
            girder.next();
            int key = girder.key();
            QStringList list = girder.value();
            SectionValues sectionValues;
            sectionValues.memberType = DATATYPE::TPGIRDER;
            sectionValues.sectionName = list.at(2);
            sectionNameHash.insert(key, list.at(2));
            QPair<QUuid, QUuid> floors = getFloorUuid(floorHash, list.at(4), list.at(5));
            sectionValues.startFloorID = floors.first;
            sectionValues.endFloorID = floors.second;

            if (girvalue2.keys().contains(key))
            {
                sectionValues.i_param = SetGirderRcParameter(girvalue2.value(key));
            }
            if (girvalue3.keys().contains(key))
            {
                sectionValues.c_param = SetGirderRcParameter(girvalue3.value(key));
            }
            if (girvalue4.keys().contains(key))
            {
                sectionValues.j_param = SetGirderRcParameter(girvalue4.value(key));
            }
            if (girvalue5.keys().contains(key))
            {
                sectionValues.i_param = SetGirderSteelParameter(girvalue5.value(key), sectionValues.i_param);
            }
            if (girvalue6.keys().contains(key))
            {
                sectionValues.c_param = SetGirderSteelParameter(girvalue6.value(key), sectionValues.c_param);
            }
            if (girvalue7.keys().contains(key))
            {
                sectionValues.j_param = SetGirderSteelParameter(girvalue7.value(key), sectionValues.j_param);
            }

            QString type = list.at(6);
            if (type == "RC")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_RC;
                sectionValues.line_param.concMaterial = list.at(8);
                interpreter->rcGirderList.append(sectionValues);
            }
            else if (type == "S")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_S;
                interpreter->sGirderList.append(sectionValues);
            }
            else if (type == "SRC")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_SRC;
                sectionValues.line_param.concMaterial = list.at(8);
                interpreter->srcGirderList.append(sectionValues);
            }
        }
    }
    void readFromBld3d::interpretBraceSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        QHashIterator<int, QStringList> brace(bracevalue1);
        while (brace.hasNext())
        {
            brace.next();
            int key = brace.key();
            QStringList list = brace.value();
            SectionValues sectionValues;
            sectionValues.memberType = DATATYPE::TPBRACE;
            QString type = list.at(3);
            if (type == "S")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_S;
            }
            sectionValues.sectionName = list.at(2);
            sectionNameHash.insert(key, list.at(2));

            QStringList list2 = bracevalue2.value(key);
            LinePartsParameter p;
            p.steelMaterial = list2.at(2);
            if (list2.at(3) == "H" || list2.at(3) == "WH")
            {
                p.steelType = SteelType::Type::ST_WH;
                p.sizeH = list2.at(4).toDouble();
                p.sizeB = list2.at(5).toDouble();
                p.t1 = list2.at(6).toDouble();
                p.t2 = list2.at(7).toDouble();
            }
            else if (list2.at(3) == "P")
            {
                p.steelType = SteelType::Type::ST_P;
                p.sizeH = list2.at(4).toDouble();
                p.t1 = list2.at(6).toDouble();
            }
            sectionValues.i_param = p;
            sectionValues.c_param = p;
            sectionValues.j_param = p;
            interpreter->sBraceList.append(sectionValues);
        }
    }

    void readFromBld3d::interpretBeamSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        QHashIterator<int, QStringList> beam(beamvalue1);
        while (beam.hasNext())
        {
            beam.next();
            int key = beam.key();
            QStringList list = beam.value();
            SectionValues sectionValues;
            sectionValues.memberType = DATATYPE::TPBEAM;
            sectionValues.sectionName = list.at(2);
            sectionNameHash.insert(key, list.at(2));

            if (beamvalue2.keys().contains(key))
            {
                sectionValues.i_param = SetGirderRcParameter(beamvalue2.value(key));
            }
            if (beamvalue3.keys().contains(key))
            {
                sectionValues.c_param = SetGirderRcParameter(beamvalue3.value(key));
            }
            if (beamvalue4.keys().contains(key))
            {
                sectionValues.j_param = SetGirderRcParameter(beamvalue4.value(key));
            }
            if (beamvalue5.keys().contains(key))
            {
                sectionValues.i_param = SetGirderSteelParameter(beamvalue5.value(key), sectionValues.i_param);
            }
            if (beamvalue6.keys().contains(key))
            {
                sectionValues.c_param = SetGirderSteelParameter(beamvalue6.value(key), sectionValues.c_param);
            }
            if (beamvalue7.keys().contains(key))
            {
                sectionValues.j_param = SetGirderSteelParameter(beamvalue7.value(key), sectionValues.j_param);
            }

            QString type = list.at(3);
            if (type == "RC")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_RC;
                interpreter->rcBeamList.append(sectionValues);
            }
            else if (type == "S")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_S;
                interpreter->sBeamList.append(sectionValues);
            }
            else if (type == "SRC")
            {
                sectionValues.sectionType = STRUCTTYPE::STP_SRC;
                interpreter->srcBeamList.append(sectionValues);
            }
        }
    }
    void readFromBld3d::interpretPostSectionValues(QSharedPointer<InterpreterCsv> interpreter) {}
    void readFromBld3d::interpretSlabSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        QHashIterator<int, QStringList> slab(slabvalue1);
        while (slab.hasNext())
        {
            slab.next();
            int key = slab.key();
            QStringList list = slab.value();
            SectionValues sectionValues;
            sectionValues.memberType = DATATYPE::TPSLAB;
            sectionValues.sectionType = STRUCTTYPE::STP_RC;
            sectionValues.sectionName = list.at(2);
            sectionNameHash.insert(key, list.at(2));
            // TODO
            // sectionValues.plane_param.finishLoad = list.at(7).toDouble(); // 未対応
            // sectionValues.plane_param.concMaterial = "FC24";
            sectionValues.plane_param.concThickness = list.at(3).toDouble();
            sectionValues.plane_param.planeType = PlaneType::Type::MEMBRANE;
            // TODO 鉄筋
            QStringList list2 = slabvalue2.value(key);
            sectionValues.plane_param.sBar_lowerEdgeA = PlaneSectionParameter::Bar(list2.at(2) + list2.at(3), list2.at(4).toDouble());
            sectionValues.plane_param.sBar_lowerCenterA = PlaneSectionParameter::Bar(list2.at(5) + list2.at(6), list2.at(7).toDouble());
            sectionValues.plane_param.sBar_lowerEdgeCenterB = PlaneSectionParameter::Bar(list2.at(8) + list2.at(9), list2.at(10).toDouble());
            sectionValues.plane_param.lBar_lowerEdgeA = PlaneSectionParameter::Bar(list2.at(11) + list2.at(12), list2.at(13).toDouble());
            sectionValues.plane_param.lBar_lowerCenterA = PlaneSectionParameter::Bar(list2.at(14) + list2.at(15), list2.at(16).toDouble());
            sectionValues.plane_param.lBar_lowerEdgeCenterB = PlaneSectionParameter::Bar(list2.at(17) + list2.at(18), list2.at(19).toDouble());
            QStringList list3 = slabvalue3.value(key);
            sectionValues.plane_param.sBar_upperEdgeA = PlaneSectionParameter::Bar(list3.at(2) + list3.at(3), list3.at(4).toDouble());
            sectionValues.plane_param.sBar_upperCenterA = PlaneSectionParameter::Bar(list3.at(5) + list3.at(6), list3.at(7).toDouble());
            sectionValues.plane_param.sBar_upperEdgeCenterB = PlaneSectionParameter::Bar(list3.at(8) + list3.at(9), list3.at(10).toDouble());
            sectionValues.plane_param.lBar_upperEdgeA = PlaneSectionParameter::Bar(list3.at(11) + list3.at(12), list3.at(13).toDouble());
            sectionValues.plane_param.lBar_upperCenterA = PlaneSectionParameter::Bar(list3.at(14) + list3.at(15), list3.at(16).toDouble());
            sectionValues.plane_param.lBar_upperEdgeCenterB = PlaneSectionParameter::Bar(list3.at(17) + list3.at(18), list3.at(19).toDouble());

            interpreter->rcSlabList.append(sectionValues);
        }
    }
    void readFromBld3d::interpretUwallSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        QHashIterator<int, QStringList> uwall(uwallvalue1);
        while (uwall.hasNext())
        {
            uwall.next();
            int key = uwall.key();
            QStringList list = uwall.value();
            SectionValues sectionValues;
            sectionValues.memberType = DATATYPE::TPUWALL;
            sectionValues.sectionType = STRUCTTYPE::STP_RC;
            sectionValues.sectionName = list.at(2);
            sectionNameHash.insert(key, list.at(2));

            sectionValues.plane_param.concMaterial = list.at(3);
            sectionValues.plane_param.concThickness = list.at(4).toDouble();
            sectionValues.plane_param.finishLoad = list.at(5).toDouble();
            if (sectionValues.sectionName.startsWith("Z"))
            {
                sectionValues.plane_param.planeType = PlaneType::Type::ZWALL;
            }
            else
            {
                sectionValues.plane_param.planeType = PlaneType::Type::WALL;
            }
            QStringList list2 = uwallvalue2.value(key);
            sectionValues.plane_param.vBar_outerTop = PlaneSectionParameter::Bar(list2.at(2) + list2.at(3), list2.at(4).toDouble());
            sectionValues.plane_param.vBar_outerMiddle = PlaneSectionParameter::Bar(list2.at(5) + list2.at(6), list2.at(7).toDouble());
            sectionValues.plane_param.vBar_outerBottom = PlaneSectionParameter::Bar(list2.at(8) + list2.at(9), list2.at(10).toDouble());
            sectionValues.plane_param.hBar_outerEdge = PlaneSectionParameter::Bar(list2.at(11) + list2.at(12), list2.at(13).toDouble());
            sectionValues.plane_param.hBar_outerCenter = PlaneSectionParameter::Bar(list2.at(14) + list2.at(15), list2.at(16).toDouble());

            QStringList list3 = uwallvalue3.value(key);
            sectionValues.plane_param.vBar_innerTop = PlaneSectionParameter::Bar(list3.at(2) + list3.at(3), list3.at(4).toDouble());
            sectionValues.plane_param.vBar_innerMiddle = PlaneSectionParameter::Bar(list3.at(5) + list3.at(6), list3.at(7).toDouble());
            sectionValues.plane_param.vBar_innerBottom = PlaneSectionParameter::Bar(list3.at(8) + list3.at(9), list3.at(10).toDouble());
            sectionValues.plane_param.hBar_innerEdge = PlaneSectionParameter::Bar(list3.at(11) + list3.at(12), list3.at(13).toDouble());
            sectionValues.plane_param.hBar_innerCenter = PlaneSectionParameter::Bar(list3.at(14) + list3.at(15), list3.at(16).toDouble());
            interpreter->rcUwallList.append(sectionValues);
        }
    }
    void readFromBld3d::interpretWallSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        QHashIterator<int, QStringList> wall(wallvalue1);
        while (wall.hasNext())
        {
            wall.next();
            int key = wall.key();
            QStringList list = wall.value();
            SectionValues sectionValues;
            sectionValues.memberType = DATATYPE::TPWALL;
            sectionValues.sectionType = STRUCTTYPE::STP_RC;
            sectionValues.sectionName = list.at(2);
            sectionNameHash.insert(key, list.at(2));

            sectionValues.plane_param.concMaterial = list.at(3);
            sectionValues.plane_param.concThickness = list.at(4).toDouble();
            if (sectionValues.sectionName.startsWith("Z"))
            {
                sectionValues.plane_param.planeType = PlaneType::Type::ZWALL;
            }
            else
            {
                sectionValues.plane_param.planeType = PlaneType::Type::WALL;
            }
            QStringList list2 = wallvalue2.value(key);
            sectionValues.plane_param.vBar = PlaneSectionParameter::Bar(list2.at(2) + list2.at(3), list2.at(4).toDouble());
            sectionValues.plane_param.hBar = PlaneSectionParameter::Bar(list2.at(5) + list2.at(6), list2.at(7).toDouble());
            sectionValues.plane_param.vOpeningBar_small = PlaneSectionParameter::OpeningBar(list2.at(8), list2.at(9).toInt());
            sectionValues.plane_param.hOpeningBar_small = PlaneSectionParameter::OpeningBar(list2.at(10), list2.at(11).toInt());
            sectionValues.plane_param.dOpeningBar_small = PlaneSectionParameter::OpeningBar(list2.at(12), list2.at(13).toInt());
            sectionValues.plane_param.vOpeningBar_large = PlaneSectionParameter::OpeningBar(list2.at(14), list2.at(15).toInt());
            sectionValues.plane_param.hOpeningBar_large = PlaneSectionParameter::OpeningBar(list2.at(16), list2.at(17).toInt());
            sectionValues.plane_param.dOpeningBar_large = PlaneSectionParameter::OpeningBar(list2.at(18), list2.at(19).toInt());

            interpreter->rcWallList.append(sectionValues);
        }
    }
    void readFromBld3d::interpretSteelWallSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        QHashIterator<int, QStringList> swall(swallvalue1);
        while (swall.hasNext())
        {
            swall.next();
            int key = swall.key();
            QStringList list = swall.value();
            SectionValues sectionValues;
            sectionValues.memberType = DATATYPE::TPWALL;
            sectionValues.sectionType = STRUCTTYPE::STP_S;
            sectionValues.sectionName = list.at(2);
            sectionNameHash.insert(key, list.at(2));

            sectionValues.plane_param.planeType = PlaneType::Type::WALL;
            sectionValues.plane_param.steelMaterial = list.at(3);
            sectionValues.plane_param.steelThickness = list.at(4).toDouble();
            QStringList list2 = swallvalue2.value(key);
            // list2.at(2).toDouble();
            // list2.at(3).toDouble();
            // list2.at(4).toDouble();
            // list2.at(5).toDouble();
            // list2.at(6).toDouble();
            // list2.at(7).toDouble();
            // list2.at(8).toDouble();
            // list2.at(9).toDouble();

            interpreter->sWallList.append(sectionValues);
        }
    }
    void readFromBld3d::interpretWallOpenSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        // 　PlateNumberとListRecordのセット
        QHash<QSet<int>, int> combinedWallOpen;
        int id = 0;
        QHashIterator<int, QSet<int>> open(openPlateHash);
        while (open.hasNext())
        {
            open.next();
            if (!combinedWallOpen.contains(open.value()))
            {
                combinedWallOpen.insert(open.value(), id);
                QString name;
                Q_FOREACH (int list, open.value())
                {
                    name += wolistvalue.value(list).at(0) + "_";
                }
                name.chop(1);
                wopenNoToName.insert(id, name);
                id++;
            }
        }

        QHashIterator<QSet<int>, int> combined(combinedWallOpen);
        int count = 0;
        while (combined.hasNext())
        {
            combined.next();
            QSet<int> temp = combined.key();
            QSet<int>::iterator i;
            for (i = temp.begin(); i != temp.end(); i++)
            {
                QStringList list = wolistvalue.value(*i);
                SectionValues sectionValue;
                sectionValue.memberType = DATATYPE::TPWALLOPEN;
                sectionValue.sectionName = wopenNoToName.value(combined.value());
                sectionValue.wopen_param.openType = WallOpenType::Type::NORMAL;
                sectionValue.wopen_param.isHorizontalFace = true;
                sectionValue.wopen_param.hPosition = WallOpenType::HPosition::CENTER;
                sectionValue.wopen_param.isVerticalFace = true;
                sectionValue.wopen_param.vPosition = WallOpenType::VPosition::MIDDLE;
                sectionValue.wopen_param.ox = list.at(3).toDouble();
                sectionValue.wopen_param.oy = list.at(4).toDouble();
                sectionValue.wopen_param.width = list.at(5).toDouble();
                sectionValue.wopen_param.height = list.at(6).toDouble();
                sectionValue.wopen_param.comment = QString();
                interpreter->wallOpenList.append(sectionValue);
                count++;
            }
        }
    }

    void readFromBld3d::interpretJointLoadSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        // 参照があるので、一度全部読み込み
        struct tempJList
        {
            tempJList() {}
            QList<QString> refList;
            int no;
            QString name;
            LoadType::LOADTYPE type;
            JOINTLOADDIM dim;
            QHash<LoadType::LOADTYPE, JOINTLOADDIM> expand(QHash<QString, tempJList> const &list)
            {
                QHash<LoadType::LOADTYPE, JOINTLOADDIM> hash;
                if (hash.contains(type))
                {
                    hash.insert(type, dim.Add(hash.value(type)));
                }
                else
                {
                    hash.insert(type, dim);
                }
                if (refList.count() == 0)
                {
                    return hash;
                }
                else
                {
                    Q_FOREACH (QString ref, refList)
                    {
                        tempJList tm = list.value(ref);
                        QHash<LoadType::LOADTYPE, JOINTLOADDIM> recursion = tm.expand(list);
                        QHashIterator<LoadType::LOADTYPE, JOINTLOADDIM> rec(recursion);
                        while (rec.hasNext())
                        {
                            rec.next();
                            LoadType::LOADTYPE other = rec.key();
                            if (hash.contains(other))
                            {
                                JOINTLOADDIM old = hash.value(other);
                                hash.insert(other, old.Add(rec.value()));
                            }
                            else
                            {
                                hash.insert(other, rec.value());
                            }
                        }
                    }
                }
                return hash;
            }
        };
        QHash<QString, tempJList> tempJlist;
        QHashIterator<int, QStringList> jllist(jllistvalue);
        while (jllist.hasNext())
        {
            jllist.next();
            QStringList list = jllist.value();
            tempJList tm;
            tm.no = jllist.key();
            if (list.at(3) != "")
            {
                tm.refList.append(list.at(3));
            }
            if (list.at(4) != "")
            {
                tm.refList.append(list.at(4));
            }
            if (list.at(5) != "")
            {
                tm.refList.append(list.at(5));
            }
            tm.name = list.at(2);
            if (list.at(6) == "DL")
            {
                tm.type = LoadType::LOADTYPE::LTYPE_DL;
            }
            else if (list.at(6) == "LL1")
            {
            }
            else if (list.at(6) == "LL2")
            {
            }
            else if (list.at(6) == "LL3")
            {
            }
            else if (list.at(6) == "LLE")
            {
            }
            tm.dim = JOINTLOADDIM(
                list.at(7).toDouble(),
                list.at(8).toDouble(),
                list.at(9).toDouble(),
                list.at(10).toDouble(),
                list.at(11).toDouble(),
                list.at(12).toDouble(),
                list.at(13));
            tempJlist.insert(tm.name, tm);
        }

        QHashIterator<QString, tempJList> jlist(tempJlist);
        while (jlist.hasNext())
        {
            jlist.next();
            tempJList tmj = jlist.value();
            JOINTLOADVALUES jlv;
            jlv.loadName = tmj.name;
            jlv.loadTable = tmj.expand(tempJlist);
            interpreter->jointLoadList.append(jlv);
        }
    }
    void readFromBld3d::interpretMemberLoadSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        // 参照があるので、一度全部読み込み
        QHashIterator<int, QStringList> mllist(mllistvalue);
        while (mllist.hasNext())
        {
            mllist.next();
            QStringList list = mllist.value();
        }
    }
    void readFromBld3d::interpretMemberCsv()
    {
        Q_FOREACH (QStringList strlist, memberLines)
        {
            int no = strlist.at(1).toInt();
            QList<int> tmpjds;
            int joint1 = strlist.at(4).toInt();
            if (mergedJointHash.contains(joint1))
                joint1 = mergedJointHash.value(joint1);
            tmpjds.append(joint1);

            int joint2 = strlist.at(5).toInt();
            if (mergedJointHash.contains(joint2))
                joint2 = mergedJointHash.value(joint2);
            tmpjds.append(joint2);

            MEMBERVALUES mem;
            if (strlist.at(2).toInt() == 60 || strlist.at(2).toInt() == 63)
            {
                columnJointHash.insert(no, tmpjds);
                mem.dataType = DATATYPE::TPCOLUMN;
                mem.codeAngle = strlist.at(6).toDouble();
                mem.sectionName = sectionNameHash.value(strlist.at(3).toInt());
                columnHash.insert(no, mem);
            }
            else if (strlist.at(2).toInt() == 61)
            {
                girderJointHash.insert(no, tmpjds);
                mem.dataType = DATATYPE::TPGIRDER;
                mem.codeAngle = strlist.at(6).toDouble();
                mem.sectionName = sectionNameHash.value(strlist.at(3).toInt());
                girderHash.insert(no, mem);
            }
            else if (strlist.at(2).toInt() == 62)
            {
                beamJointHash.insert(no, tmpjds);
                mem.dataType = DATATYPE::TPBEAM;
                mem.codeAngle = strlist.at(6).toDouble();
                mem.sectionName = sectionNameHash.value(strlist.at(3).toInt());
                beamHash.insert(no, mem);
            }
            else if (strlist.at(2).toInt() == 68)
            {
                braceJointHash.insert(no, tmpjds);
                mem.dataType = DATATYPE::TPBRACE;
                mem.codeAngle = strlist.at(6).toDouble();
                mem.sectionName = sectionNameHash.value(strlist.at(3).toInt());
                braceHash.insert(no, mem);
            }
        }
    }

    void readFromBld3d::interpretPlateCsv(QSharedPointer<InterpreterCsv> interpreter)
    {
        Q_FOREACH (QStringList strlist, plateLines)
        {
            int no = strlist.at(1).toInt();
            int count = strlist.at(4).toInt();
            QList<int> tmpjds;
            for (int i = 0; i < count; i++)
            {
                int joint = strlist.at(i + 6).toInt();
                if (mergedJointHash.contains(joint))
                    joint = mergedJointHash.value(joint);
                if (tmpjds.count() != 0 && tmpjds.last() == joint)
                    continue;
                tmpjds.append(joint);
            }
            MEMBERVALUES mem;
            if (strlist.at(2).toInt() == 64)
            {
                slabJointHash.insert(no, tmpjds);
                mem.dataType = DATATYPE::TPSLAB;
                mem.sectionName = sectionNameHash.value(strlist.at(3).toInt());
                slabHash.insert(no, mem);
            }
            else if (strlist.at(2).toInt() == 65 || strlist.at(2).toInt() == 66)
            {
                wallJointHash.insert(no, tmpjds);
                mem.dataType = DATATYPE::TPWALL;
                mem.sectionName = sectionNameHash.value(strlist.at(3).toInt());
                wallHash.insert(no, mem);
            }
            else if (strlist.at(2).toInt() == 67)
            {
                uwallJointHash.insert(no, tmpjds);
                mem.dataType = DATATYPE::TPUWALL;
                mem.sectionName = sectionNameHash.value(strlist.at(3).toInt());
                uwallHash.insert(no, mem);
            }
            else if (strlist.at(2).toInt() == 72)
            {
                tempDeadLoadJointHash.insert(tmpjds, strlist.at(3).toInt());
            }
            else if (strlist.at(2).toInt() == 73)
            {
                tempLiveLoadJointHash.insert(tmpjds, strlist.at(3).toInt());
            }
        }

        interpretWallOpen(interpreter);
    }

    void readFromBld3d::interpretWallOpen(QSharedPointer<InterpreterCsv> interpreter)
    {
        Q_FOREACH (QStringList strlist, openLines)
        {
            int listNo = strlist.at(3).toInt();
            int plateNo = strlist.at(4).toInt();
            if (!openPlateHash.contains(plateNo))
            {
                QSet<int> temp;
                temp.insert(listNo);
                openPlateHash.insert(plateNo, temp);
            }
            else
            {
                QSet<int> temp = openPlateHash.value(plateNo);
                temp.insert(listNo);
                openPlateHash[plateNo] = temp;
            }
        }

        // TODO:最初は同じ開口でもまとめない csvが整ってから
        int wallOpenListNumber = 0;
        int wallOpanNameNumber = 0;
        QHashIterator<int, QSet<int>> wallOpen(openPlateHash);
        while (wallOpen.hasNext())
        {
            wallOpen.next();
            // 壁に開口セット
            MEMBERVALUES plate;
            if (wallHash.contains(wallOpen.key()))
            {
                plate = wallHash.value(wallOpen.key());
                plate.wallOpenName = "W" + QString::number(wallOpanNameNumber);
                wallHash[wallOpen.key()] = plate;
            }
            else
            {
                plate = uwallHash.value(wallOpen.key());
                plate.wallOpenName = "W" + QString::number(wallOpanNameNumber);
                uwallHash[wallOpen.key()] = plate;
            }

            Q_FOREACH (int list, wallOpen.value())
            {
                QStringList wolist = wolistvalue.value(list);
                SectionValues open;
                open.memberType = DATATYPE::TPWALLOPEN;
                open.sectionName = plate.wallOpenName;
                open.wopen_param.openType = WallOpenType::Type::NORMAL;
                open.wopen_param.isHorizontalFace = true;
                open.wopen_param.hPosition = WallOpenType::HPosition::CENTER;
                open.wopen_param.isVerticalFace = true;
                open.wopen_param.vPosition = WallOpenType::VPosition::MIDDLE;
                open.wopen_param.ox = wolist.at(3).toDouble();
                open.wopen_param.oy = wolist.at(4).toDouble();
                open.wopen_param.width = wolist.at(5).toDouble();
                open.wopen_param.height = wolist.at(6).toDouble();
                open.wopen_param.comment = QString();
                interpreter->wallOpenList.append(open);
                wallOpenListNumber++;
            }
            wallOpanNameNumber++;
        }
    }

    void readFromBld3d::interpretSlabLoadSectionValues(QSharedPointer<InterpreterCsv> interpreter)
    {
        QHash<int, SLABLOADVALUES> tempDeadLoadHash;
        QHashIterator<int, QStringList> fllist(fllistvalue);
        while (fllist.hasNext())
        {
            fllist.next();
            SLABLOADVALUES values;
            values.loadName = fllist.value().at(2);
            values.sDL = fllist.value().at(3).toDouble();
            values.notes = fllist.value().at(4);
            tempDeadLoadHash.insert(fllist.key(), values);
        }

        QHash<int, SLABLOADVALUES> tempLiveLoadHash;
        QHashIterator<int, QStringList> lllist(lllistvalue);
        while (lllist.hasNext())
        {
            lllist.next();
            SLABLOADVALUES values;
            values.loadName = lllist.value().at(2);
            values.sLL0 = lllist.value().at(3).toDouble();
            values.sLL1 = lllist.value().at(4).toDouble();
            values.sLL2 = lllist.value().at(5).toDouble();
            values.sLL3 = lllist.value().at(6).toDouble();
            values.sLLE = lllist.value().at(7).toDouble();
            values.notes = lllist.value().at(8);
            tempLiveLoadHash.insert(lllist.key(), values);
        }

        int listNo = 0;
        int no = 0;
        MEMBERVALUES mem;
        mem.dataType = DATATYPE::TPSLABLOAD;
        QHash<QPair<int, int>, SLABLOADVALUES> slabloads;

        QHashIterator<QList<int>, int> deadLoad(tempDeadLoadJointHash);
        while (deadLoad.hasNext())
        {
            deadLoad.next();
            bool hasLiveLoad = false;
            QHashIterator<QList<int>, int> liveLoad(tempLiveLoadJointHash);
            while (liveLoad.hasNext())
            {
                liveLoad.next();
                if (deadLoad.key() == liveLoad.key())
                {
                    QPair<int, int> slabLoad(deadLoad.value(), liveLoad.value());
                    if (slabloads.contains(slabLoad))
                    {
                        slabLoadJointHash.insert(no, deadLoad.key());
                        mem.sectionName = slabloads[slabLoad].loadName;
                        slabLoadHash.insert(no, mem);
                    }
                    else
                    {
                        SLABLOADVALUES slab;
                        SLABLOADVALUES dead = tempDeadLoadHash.value(deadLoad.value());
                        SLABLOADVALUES live = tempLiveLoadHash.value(liveLoad.value());
                        slab.loadName = dead.loadName + "_" + live.loadName;
                        slab.sDL = dead.sDL;
                        slab.sLL0 = live.sLL0;
                        slab.sLL1 = live.sLL1;
                        slab.sLL2 = live.sLL2;
                        slab.sLL3 = live.sLL3;
                        slab.sLL4 = live.sLL4;
                        slab.sLLE = live.sLLE;
                        slab.notes = dead.notes + "_" + live.notes;
                        slabloads.insert(slabLoad, slab);
                        interpreter->slabLoadList.append(slab);
                        slabLoadJointHash.insert(no, deadLoad.key());
                        mem.sectionName = slab.loadName;
                        slabLoadHash.insert(no, mem);
                        listNo++;
                    }
                    hasLiveLoad = true;
                    break;
                }
            }

            if (hasLiveLoad == false)
            {
                QPair<int, int> slabLoad(deadLoad.value(), -1);
                if (slabloads.contains(slabLoad))
                {
                    slabLoadJointHash.insert(no, deadLoad.key());
                    mem.sectionName = slabloads[slabLoad].loadName;
                    slabLoadHash.insert(no, mem);
                }
                else
                {
                    SLABLOADVALUES slab;
                    SLABLOADVALUES dead = tempDeadLoadHash.value(deadLoad.value());
                    slab.loadName = dead.loadName + "_";
                    slab.sDL = dead.sDL;
                    slab.notes = dead.notes + "_";
                    slabloads.insert(slabLoad, slab);
                    interpreter->slabLoadList.append(slab);
                    slabLoadJointHash.insert(no, deadLoad.key());
                    mem.sectionName = slab.loadName;
                    slabLoadHash.insert(no, mem);
                    listNo++;
                }
            }
            no++;
        }

        QHashIterator<QList<int>, int> liveLoad(tempLiveLoadJointHash);
        while (liveLoad.hasNext())
        {
            liveLoad.next();
            if (!slabLoadJointHash.values().contains(liveLoad.key()))
            {
                QPair<int, int> slabLoad(-1, liveLoad.value());
                if (slabloads.contains(slabLoad))
                {
                    slabLoadJointHash.insert(no, deadLoad.key());
                }
                else
                {
                    SLABLOADVALUES slab;
                    SLABLOADVALUES live = tempLiveLoadHash.value(liveLoad.value());
                    slab.loadName = "_" + live.loadName;
                    slab.sLL0 = live.sLL0;
                    slab.sLL1 = live.sLL1;
                    slab.sLL2 = live.sLL2;
                    slab.sLL3 = live.sLL3;
                    slab.sLL4 = live.sLL4;
                    slab.sLLE = live.sLLE;
                    slab.notes = "_" + live.notes;
                    slabloads.insert(slabLoad, slab);
                    interpreter->slabLoadList.append(slab);
                    slabLoadJointHash.insert(no, deadLoad.key());
                    listNo++;
                }
                no++;
            }
        }
    }

    LinePartsParameter readFromBld3d::SetColumnRcParameter(const QStringList &list) const
    {
        LinePartsParameter param;
        if (list.at(1) == "")
        {
            param.concType = ConcreteType::Type::CIRCLE;
            param.Dy = list.at(2).toDouble();
            param.mainBarArrange1.numsum.append(list.at(4).toInt());
            param.hoopY = list.at(7).toInt();
        }
        else
        {
            param.concType = ConcreteType::Type::RECTANGLE;
            param.Dy = list.at(2).toDouble();
            param.Dz = list.at(3).toDouble();
            param.mainBarArrange1.numsum.append(list.at(4).toInt());
            param.mainBarArrange2.numsum.append(list.at(5).toInt());
            param.hoopY = list.at(7).toInt();
            param.hoopZ = list.at(8).toInt();
        }
        param.mainBar1 = u8"D" + list.at(6);
        param.hoopBar = u8"D" + list.at(9);
        param.hoopPitch = list.at(10).toInt();
        return param;
    }

    LinePartsParameter readFromBld3d::SetColumnSteelParameter(const QStringList &list, LinePartsParameter param) const
    {
        param.steelMaterial = list.at(2);
        // TODO
        QString typeX = list.at(3);
        QString typeY = list.at(8);
        if ((typeX == "H" || typeX == "WH") && typeY == "")
        {
            param.steelType = SteelType::Type::ST_WH;
            param.sizeH = list.at(4).toDouble();
            param.sizeB = list.at(5).toDouble();
            param.t1 = list.at(6).toDouble();
            param.t2 = list.at(7).toDouble();
        }
        else if (typeX == "" && (typeY == "H" || typeY == "WH"))
        {
            param.steelType = SteelType::Type::ST_WI;
            param.sizeH = list.at(9).toDouble();
            param.sizeB = list.at(10).toDouble();
            param.t1 = list.at(11).toDouble();
            param.t2 = list.at(12).toDouble();
        }
        else if (typeX == "BX" && typeY == "")
        {
            param.steelType = SteelType::Type::ST_WBX;
            param.sizeH = list.at(4).toDouble();
            param.sizeB = list.at(5).toDouble();
            param.t1 = list.at(6).toDouble();
            param.t2 = list.at(7).toDouble();
        }
        else if (typeX == "P" && typeY == "")
        {
            param.steelType = SteelType::Type::ST_WP;
            param.sizeH = list.at(4).toDouble();
            param.t1 = list.at(6).toDouble();
        }
        else if ((typeX == "H" || typeX == "WH") && (typeY == "H" || typeY == "WH"))
        {
            param.steelType = SteelType::Type::ST_CROSSH;
            param.sizeH = list.at(4).toDouble();
            param.sizeB = list.at(5).toDouble();
            param.t1 = list.at(6).toDouble();
            param.t2 = list.at(7).toDouble();
            param.H_T = list.at(9).toDouble();
            param.B_T = list.at(10).toDouble();
            param.t1_T = list.at(11).toDouble();
            param.t2_T = list.at(12).toDouble();
        }

        return param;
    }

    LinePartsParameter readFromBld3d::SetGirderRcParameter(const QStringList &list) const
    {
        LinePartsParameter param;
        param.concType = ConcreteType::Type::RECTANGLE;
        param.Dy = list.at(2).toDouble();
        param.Dz = list.at(3).toDouble();
        // ハンチ
        param.mainBarArrange1.numsum.append(list.at(5).toInt());
        if (list.at(6) != "")
            param.mainBarArrange1.numsum.append(list.at(6).toInt());
        if (list.at(7) != "")
            param.mainBarArrange1.numsum.append(list.at(7).toInt());
        param.mainBar1 = u8"D" + list.at(8);
        param.mainBarArrange2.numsum.append(list.at(9).toInt());
        if (list.at(10) != "")
            param.mainBarArrange2.numsum.append(list.at(10).toInt());
        if (list.at(11) != "")
            param.mainBarArrange2.numsum.append(list.at(11).toInt());
        param.hoopY = 2; // ?
        param.hoopZ = list.at(13).toInt();
        param.hoopBar = u8"D" + list.at(14);
        param.hoopPitch = list.at(15).toInt();
        return param;
    }

    LinePartsParameter readFromBld3d::SetGirderSteelParameter(const QStringList &list, LinePartsParameter param) const
    {
        param.steelMaterial = list.at(2);
        QString type(list.at(3));
        if (type == "H" || type == "WH")
        {
            param.steelType = SteelType::Type::ST_WH;
            param.sizeH = list.at(4).toDouble();
            param.sizeB = list.at(5).toDouble();
            param.t1 = list.at(6).toDouble();
            param.t2 = list.at(7).toDouble();
        }
        // TODO
        return param;
    }

    QHash<int, FRAMEPOINTSVALUE> readFromBld3d::setFramePointList(QSharedPointer<InterpreterCsv> interpreter) const
    {
        QHash<int, FRAMEPOINTSVALUE> pointsHash;
        qreal totalspan = 0.0;
        bool isFirst = true;
        Q_FOREACH (QStringList grid, gridLines)
        {
            // TODO:角度が違う場合がある。
            FRAMEGROUPVALUE fgv;
            QString groupName = grid.at(2);
            Q_FOREACH (FRAMEGROUPVALUE fg, interpreter->framegroups)
            {
                if (fg.groupName == groupName)
                {
                    fgv = fg;
                    break;
                }
            }

            if (fgv.frameType == FrameType::FRAMETYPE::CIRCLE && grid.at(4).toDouble() == 0)
            {
                isFirst = false;
                totalspan = 0.0;
                continue;
            }
            FRAMEPOINTSVALUE fpv = interpreter->CalculateFramePointsValue(fgv, totalspan, grid.at(4), true, isFirst, grid.at(3));
            /*
            QList<QPointF> plist;
            STRAIGHTFRAMEVALUE st_value;
            QPointF npoint;
            if (fgv.frameType == FRAMETYPE::STRAIGHT){
                QPointF po = fgv.basePoint;
                qreal theta = fgv.startAngle / 180.0 * M_PI;
                qreal p_len = 50; // 正方向に伸ばす長さ 50mがデフォルト
                qreal m_len = 10; // 負方向に伸ばす長さ　10mがデフォルト
                QPointF p1, p2, p3, p4;
                QPointF spoint;
                qreal m_span = grid.at(4).toDouble() / 1000.0;
                qreal s_angle;
                if (fgv.startAngle < 45.0){
                    p1 = QPointF(-m_len * qCos(theta) - totalspan * qSin(theta),
                                 -m_len * qSin(theta) + totalspan * qCos(theta)) + po;
                    p2 = QPointF( p_len * qCos(theta) - totalspan * qSin(theta),
                                  p_len * qSin(theta) + totalspan * qCos(theta)) + po;
                    npoint = CalcVector2D::extendPoint(p1, p2, 10.0);
                    s_angle = fgv.startAngle + 90.;
                    if (grid.at(4) == "") {
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
                    if (grid.at(4) == "") {
                        p3 = QPointF(-m_len * qCos(theta), -m_len * qSin(theta)) + po;
                        p4 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                        spoint = CalcVector2D::extendPoint(p4, p3, 5.0);
                    } else {
                        spoint = CalcVector2D::extendPoint(p2, p1, 3.0);
                    }
                }
                plist.append(p1);
                plist.append(p2);

                if (grid.at(4) == ""){
                    m_span = totalspan;
                    isLast = true;
                }
                st_value = STRAIGHTFRAMEVALUE(m_span, s_angle, spoint, isFirst, isLast);
                totalspan += grid.at(4).toDouble() / 1000.0;
                isFirst = false;
                if (isLast){
                    totalspan = 0.0;
                    isFirst = true;
                    isLast = false;
                }
            }else if (fgv.frameType == FRAMETYPE::CIRCLE){
                QPointF po = fgv.basePoint;
                qreal s_angle = fgv.startAngle;
                qreal e_angle = fgv.endAngle;
                int div = fgv.division;
                // span=0はskip
                if (grid.at(4).toDouble() == 0){
                    isFirst = false;
                    totalspan = 0.0;
                    continue;
                }
                else{
                    st_value.span = grid.at(4).toDouble() / 1000.0 - totalspan;
                    totalspan = grid.at(4).toDouble() / 1000.0;
                }

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

            }else if (fgv.frameType == FRAMETYPE::RADIUS){
                QPointF po = fgv.basePoint;
                qreal angle = fgv.startAngle + totalspan;
                qreal theta = angle / 180.0 * M_PI;
                qreal p_len = fgv.plusLength;

                plist.append(po);
                QPointF p1 = QPointF( p_len * qCos(theta), p_len * qSin(theta)) + po;
                plist.append(p1);

                npoint = CalcVector2D::extendPoint(po, p1, 3.0);
                if (grid.at(4) != "")
                    totalspan += grid.at(4).toDouble();
                else
                    st_value.isLast = true;
                st_value.angle =  grid.at(4).toDouble();
            }

            FRAMEPOINTSVALUE fpv = FRAMEPOINTSVALUE(fgv.GID, plist, grid.at(3), npoint, fgv.frameType, st_value, QUuid::createUuid());
            */
            interpreter->frames.append(fpv);
            pointsHash.insert(grid.at(1).toInt(), fpv);
        }
        return pointsHash;
    }

    QHash<int, int> readFromBld3d::interpretJoints()
    {
        QHash<int, int> mergedJoint;

        Q_FOREACH (QStringList strlist, jointLines)
        {
            int no = strlist.at(1).toInt();
            GlobalPoint gp(strlist.at(3).toDouble() / 1000.0,
                           strlist.at(4).toDouble() / 1000.0,
                           strlist.at(5).toDouble() / 1000.0);
            QUuid floor = floorHash.value(strlist.at(6).toInt()).UUID;
            QList<QUuid> frames;
            for (int i = 7; i < strlist.count(); i++)
            {
                if (strlist.at(i) != QString())
                    frames.append(frameHash.value(strlist.at(i).toInt()).UUID);
            }

            bool hasSamePoint = false;
            int jointNo;
            QHashIterator<int, GlobalPoint> joint(globalPointHash);
            while (joint.hasNext())
            {
                joint.next();
                if (joint.value().x() == gp.x() &&
                    joint.value().y() == gp.y() &&
                    joint.value().z() == gp.z())
                {
                    mergedJoint.insert(no, joint.key());
                    jointNo = joint.key();
                    hasSamePoint = true;
                    break;
                }
            }
            if (hasSamePoint)
            {
                globalPointHash[jointNo].appendAttachedFloor(floor);
                globalPointHash[jointNo].appendAttachedFrame(frames);
                continue;
            }

            gp.appendAttachedFloor(floor);
            gp.appendAttachedFrame(frames);
            globalPointHash.insert(no, gp);
        }
        return mergedJoint;
    }

    void readFromBld3d::makeUnifiedData()
    {
        QHash<int, JointData *> jointDataHash;
        QHashIterator<int, GlobalPoint> global(globalPointHash);
        while (global.hasNext())
        {
            global.next();
            JointData *jd = UnifiedInputData::getInInstance()->createAnalysisJoint(global.value(), global.key(), false, QUuid());
            jointDataHash.insert(global.key(), jd);
        }

        QHashIterator<int, QList<int>> column(columnJointHash);
        while (column.hasNext())
        {
            column.next();
            QList<JointData *> jd;
            Q_FOREACH (int no, column.value())
            {
                jd.append(jointDataHash.value(no));
            }
            MEMBERVALUES mem = columnHash.value(column.key());
            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPCOLUMN, mem.sectionName, QUuid());
            md->setCodeAngle(mem.codeAngle);
        }

        QHashIterator<int, QList<int>> girder(girderJointHash);
        while (girder.hasNext())
        {
            girder.next();
            QList<JointData *> jd;
            Q_FOREACH (int no, girder.value())
            {
                jd.append(jointDataHash.value(no));
            }
            MEMBERVALUES mem = girderHash.value(girder.key());
            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPGIRDER, mem.sectionName, QUuid());
            md->setCodeAngle(mem.codeAngle);
        }

        QHashIterator<int, QList<int>> brace(braceJointHash);
        while (brace.hasNext())
        {
            brace.next();
            QList<JointData *> jd;
            Q_FOREACH (int no, brace.value())
            {
                jd.append(jointDataHash.value(no));
            }
            MEMBERVALUES mem = braceHash.value(brace.key());
            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPBRACE, mem.sectionName, QUuid());
            md->setCodeAngle(mem.codeAngle);
        }

        QHashIterator<int, QList<int>> beam(beamJointHash);
        while (beam.hasNext())
        {
            beam.next();
            QList<JointData *> jd;
            Q_FOREACH (int no, beam.value())
            {
                jd.append(jointDataHash.value(no));
            }
            MEMBERVALUES mem = beamHash.value(beam.key());
            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPBEAM, mem.sectionName, QUuid());
            md->setCodeAngle(mem.codeAngle);
        }

        QHashIterator<int, QList<int>> slab(slabJointHash);
        while (slab.hasNext())
        {
            slab.next();
            QList<JointData *> jd;
            Q_FOREACH (int no, slab.value())
            {
                jd.append(jointDataHash.value(no));
            }
            MEMBERVALUES mem = slabHash.value(slab.key());
            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPSLAB, mem.sectionName, QUuid());
        }

        QHashIterator<int, QList<int>> uwall(uwallJointHash);
        while (uwall.hasNext())
        {
            uwall.next();
            QList<JointData *> jd;
            Q_FOREACH (int no, uwall.value())
            {
                jd.append(jointDataHash.value(no));
            }
            MEMBERVALUES mem = uwallHash.value(uwall.key());
            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPUWALL, mem.sectionName, QUuid());
            md->setWallOpen(mem.wallOpenName);
        }

        QHashIterator<int, QList<int>> wall(wallJointHash);
        while (wall.hasNext())
        {
            wall.next();
            QList<JointData *> jd;
            Q_FOREACH (int no, wall.value())
            {
                jd.append(jointDataHash.value(no));
            }
            MEMBERVALUES mem = wallHash.value(wall.key());
            MemberData *md = UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPWALL, mem.sectionName, QUuid());
            md->setWallOpen(mem.wallOpenName);
        }

        /*
        QHashIterator<int, QStringList> jointLoad(jloadvalue);
        while (jointLoad.hasNext()) {
            jointLoad.next();
            QStringList list = jointLoad.value();
            QList<JointData *> jlist;
            jlist.append(jointDataHash.value(list.at(4).toInt()));
            QString name = jointLoadListHash.value(list.at(3).toInt()).loadName;
            UnifiedInputData::getInInstance()->createMember(jlist, DATATYPE::TPJOINTLOAD, name, QUuid());
        }
        */

        /*
       QHashIterator<int, QList<int>> memberLoad(memberLoadJointHash);
       while (memberLoad.hasNext()) {
           memberLoad.next();
           QList<JointData *> jd;
           Q_FOREACH(int no, memberLoad.value()){jd.append(jointDataHash.value(no));}
           QString name = listNoToName.value(memberNoToListNo.value(memberLoad.key()));
            FactoryUnifiedData::getInstance(isOutput)->createMember(jd, DATATYPE::TPMEMBERLOAD, name, QUuid());
       }
       */

        QHashIterator<int, QList<int>> slabLoad(slabLoadJointHash);
        while (slabLoad.hasNext())
        {
            slabLoad.next();
            QList<JointData *> jd;
            Q_FOREACH (int no, slabLoad.value())
            {
                jd.append(jointDataHash.value(no));
            }
            MEMBERVALUES mem = slabLoadHash.value(slabLoad.key());
            UnifiedInputData::getInInstance()->createMember(jd, DATATYPE::TPSLABLOAD, mem.sectionName, QUuid());
        }
    }

} // namespace post3dapp
