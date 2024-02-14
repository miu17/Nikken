#include "manage_memberdata.h"

#include <QDebug>

#include "define_section_struct.h"
#include "factory_unifieddata.h"
#include "manage_jointdata.h"
#include "n3d_uuidtable.h"
#include "unified_data.h"
#include "version.h"

namespace post3dapp
{
    ManageMemberData::ManageMemberData(UnifiedDataType isout, QObject *parent)
        : QObject(parent), isOutput(isout)
    {
    }

    ManageMemberData::ManageMemberData(const ManageMemberData &member, const ManageJointData &joint, const UnifiedData &unified, QObject *parent)
        : QObject(parent), isOutput(UnifiedDataType::FixedInput)
    {
        Q_FOREACH (auto m, member.columnList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            columnList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        Q_FOREACH (auto m, member.girderList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            girderList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        Q_FOREACH (auto m, member.braceList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            braceList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        Q_FOREACH (auto m, member.beamList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            beamList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }

        Q_FOREACH (auto m, member.wallList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            wallList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        Q_FOREACH (auto m, member.uwallList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            uwallList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        Q_FOREACH (auto m, member.slabList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            slabList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }

        Q_FOREACH (auto m, member.damperList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            damperList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }

        Q_FOREACH (auto m, member.isoList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            isoList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }

        Q_FOREACH (auto m, member.jointLoadList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            jointLoadList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        Q_FOREACH (auto m, member.memberLoadList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            memberLoadList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        Q_FOREACH (auto m, member.slabLoadList)
        {
            MemberData *md = new MemberData(*m, joint, unified);
            slabLoadList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }

        Q_FOREACH (MemberData *md, girderList)
            md->recalcLineLength();
        Q_FOREACH (MemberData *md, braceList)
            md->recalcLineLength();
        Q_FOREACH (MemberData *md, beamList)
            md->recalcLineLength();
    }

    void ManageMemberData::clearData()
    {
        Q_FOREACH (MemberData *md, columnList)
            delete md;
        Q_FOREACH (MemberData *md, girderList)
            delete md;
        Q_FOREACH (MemberData *md, braceList)
            delete md;
        Q_FOREACH (MemberData *md, beamList)
            delete md;
        Q_FOREACH (MemberData *md, slabList)
            delete md;
        Q_FOREACH (MemberData *md, wallList)
            delete md;
        Q_FOREACH (MemberData *md, uwallList)
            delete md;
        Q_FOREACH (MemberData *md, damperList)
            delete md;
        Q_FOREACH (MemberData *md, isoList)
            delete md;
        Q_FOREACH (MemberData *md, jointLoadList)
            delete md;
        Q_FOREACH (MemberData *md, memberLoadList)
            delete md;
        Q_FOREACH (MemberData *md, slabLoadList)
            delete md;
        columnList.clear();
        girderList.clear();
        braceList.clear();
        beamList.clear();
        slabList.clear();
        wallList.clear();
        uwallList.clear();
        damperList.clear();
        isoList.clear();
        jointLoadList.clear();
        memberLoadList.clear();
        slabLoadList.clear();
        uuidTable.clear();
    }

    void ManageMemberData::writeP3dData(QDataStream &outfile) const
    {
        outfile << columnList.count();
        Q_FOREACH (MemberData *md, columnList)
        {
            md->writeP3dData(outfile);
        }
        outfile << girderList.count();
        Q_FOREACH (MemberData *md, girderList)
        {
            md->writeP3dData(outfile);
        }
        outfile << braceList.count();
        Q_FOREACH (MemberData *md, braceList)
        {
            md->writeP3dData(outfile);
        }
        outfile << beamList.count();
        Q_FOREACH (MemberData *md, beamList)
        {
            md->writeP3dData(outfile);
        }
        outfile << slabList.count();
        Q_FOREACH (MemberData *md, slabList)
        {
            md->writeP3dData(outfile);
        }
        outfile << wallList.count();
        Q_FOREACH (MemberData *md, wallList)
        {
            md->writeP3dData(outfile);
        }
        outfile << uwallList.count();
        Q_FOREACH (MemberData *md, uwallList)
        {
            md->writeP3dData(outfile);
        }
        outfile << damperList.count();
        Q_FOREACH (MemberData *md, damperList)
        {
            md->writeP3dData(outfile);
        }
        outfile << isoList.count();
        Q_FOREACH (MemberData *md, isoList)
        {
            md->writeP3dData(outfile);
        }
        outfile << jointLoadList.count();
        Q_FOREACH (MemberData *md, jointLoadList)
        {
            md->writeP3dData(outfile);
        }
        outfile << memberLoadList.count();
        Q_FOREACH (MemberData *md, memberLoadList)
        {
            md->writeP3dData(outfile);
        }
        outfile << slabLoadList.count();
        Q_FOREACH (MemberData *md, slabLoadList)
        {
            md->writeP3dData(outfile);
        }
    }

    QString ManageMemberData::writeNsvData(QSharedPointer<N3DUuidTable> table) const
    {
        QString text;
        // TODO 中間節点がある場合のコードアングル
        // TODO 使用断面の判別

        QList<MemberData *> oneMember;
        text.append(u8"*MEMBER\n");
        text.append(writeColumnNsvData( table, &oneMember));
        text.append(writeGirderNsvData( table, &oneMember));
        text.append(writeBraceNsvData( table, &oneMember));
        text.append(writeDamperNsvData(table, &oneMember));
        text.append(writeIsolatorNsvData( table, &oneMember));

        // TODO:slab扱いのPlate,Membrane
        text.append(writeSlabNsvData( table));

        QList<MemberData *> cantiWall;
        QList<MemberData *> beamWall;
        // TODO:片持ち壁等の処理はダミー実装。Rc,S,Plate,Membraneのそれぞれを処理
        text.append(writeWallNsvData( table, &cantiWall, &beamWall));
        text.append(writeFoundationWallNsvData( table));

        // 1本部材
        text.append(u8"*MS, ").append(QString::number(oneMember.count())).append("\n");
        text.append(u8"#J1節点番号, J2節点番号, ..., Jn節点番号, \n");
        Q_FOREACH (auto md, oneMember)
        {
            QString temp;
            QList<JointData *> analysisJoint;
            Q_FOREACH (auto j, md->getJointList())
            {
                if (j->isAnalysisJoint(*FactoryUnifiedData::getInstance(isOutput)))
                    analysisJoint.append(j);
            }

            for (int i = 0; i < analysisJoint.count(); i++)
            {
                temp.append(table->GetOrCreateN3dNumber(analysisJoint.at(i)->getUuid(), DATATYPE::TPJOINT)).append(",");
                if (i != analysisJoint.count() - 1)
                    temp.append(",");
            }
            temp.append("\n");
            text.append(temp);
        }

        // 形状について大梁や小梁がとりつく位置の内分比
        int count = 0;
        QString temp;
        Q_FOREACH (auto md, girderList)
        {
            QList<JointData *> innerJointList;
            for (int i = 1; i < md->getJointList().count() - 1; i++)
            {
                bool hasBeamOrGirder = false;
                Q_FOREACH (auto m, md->getJointList().at(i)->getComposeMembers())
                {
                    if (m->dataType() == DATATYPE::TPBEAM || m->dataType() == DATATYPE::TPGIRDER)
                    {
                        hasBeamOrGirder = true;
                        break;
                    }
                }
                if (hasBeamOrGirder)
                {
                    innerJointList.append(md->getJointList().at(i));
                }
            }

            if (innerJointList.count() > 0)
            {
                count++;
                temp.append(table->GetOrCreateN3dNumber(md->getJointList().at(0)->getUuid(), DATATYPE::TPJOINT)).append(",");
                for (int i = 1; i < md->getJointList().count(); i++)
                {
                    if (md->getJointList().at(i)->isAnalysisJoint(*FactoryUnifiedData::getInstance(UnifiedDataType::FixedInput)))
                    {
                        temp.append(table->GetOrCreateN3dNumber(md->getJointList().at(i)->getUuid(), DATATYPE::TPJOINT)).append(",");
                        break;
                    }
                }

                QList<qreal> length;
                for (int i = 0; i < innerJointList.count(); i++)
                {
                    length.append(md->getJointLength(innerJointList.at(i)) / md->getTotalLength());
                }

                for (int i = 0; i < length.count(); i++)
                {
                    temp.append(QString::number(length.at(i)));
                    if (i != length.count() - 1)
                        temp.append(",");
                }

                temp.append("\n");
            }
        }
        text.append(u8"*BG, ").append(QString::number(count)).append("\n");
        text.append(u8"#J1節点番号, J2節点番号, 内分比xm, \n");
        text.append(temp);

        // 架構内片持ち壁
        text.append(u8"*WC, ").append(QString::number(cantiWall.count())).append("\n");
        text.append(u8"#ID, I端, J端, 符号, Θ, 片持ち壁せいh(m)\n");
        Q_FOREACH (MemberData *md, cantiWall)
        {
            text.append(table->GetOrCreateN3dNumber(md->getJointList().first()->getUuid(), DATATYPE::TPJOINT)).append(",");
            text.append(table->GetOrCreateN3dNumber(md->getJointList().last()->getUuid(), DATATYPE::TPJOINT)).append(",");
            text.append(md->sectionName()).append(",");
            text.append(QString::number(md->codeAngle())).append(","); // TODO　コードアングル
            text.append(QString::number(2.0)).append("\n");            // TODO
        }

        // 小梁上の壁は長さ・厚さ・高さ・所属階と壁の方向、コンクリート強度だけ吸い出す
        // WOデータ書式 WO 21-30　中心X座標 31-40　中心Y座標 41-50　方向(ラジアン)(0-パイ) 51-60 t(m) 61-70 l(平面的な長さ) 71-80 h(m)高さ FCは？
        text.append(u8"*WO, ").append(QString::number(beamWall.count())).append("\n");
        text.append(u8"#ID, X座標(m), Y座標(m), 方向(rad), t(m), l(m), h(m)\n");
        Q_FOREACH (MemberData *md, beamWall)
        {
            QString temp;
            temp.append(table->GetOrCreateN3dNumber(md->getUuid(), DATATYPE::TPWALL)).append(",");
            temp.append(u8"10.0, 20.0, 0.0, 0.18, 5.0, 4.0").append("\n");
        }

        return text;
    }

    void ManageMemberData::writeLimitedData(QDataStream &outfile,
                                            const QList<MemberData *> &limitedMembers) const
    {
        QList<MemberData *> mdlist;

        Q_FOREACH (MemberData *md, columnList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, girderList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, braceList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, beamList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, slabList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, wallList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, uwallList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, isoList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, damperList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, jointLoadList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, memberLoadList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
        mdlist.clear();

        Q_FOREACH (MemberData *md, slabLoadList)
            if (limitedMembers.contains(md))
                mdlist.append(md);
        outfile << mdlist.count();
        Q_FOREACH (MemberData *md, mdlist)
            md->writeP3dData(outfile);
    }

    bool ManageMemberData::checkData(QDataStream &infile, const LibraryVersion &ver) const
    {
        int mcount;
        QUuid id;
        QList<QUuid> idlist;
        QString str, wstr, iybstr, izbstr, jybstr, jzbstr;
        qreal angle, shy, shz, iface, jface, ilen, jlen, irr, jrr;
        int dtype, ifrule, jfrule, pltype, prule, mno;

        for (int itype = static_cast<int>(DATATYPE::TPCOLUMN); itype <= static_cast<int>(DATATYPE::TPSLABLOAD); itype++)
        { // 部材のDATATYPE分のループ
            if (itype == static_cast<int>(DATATYPE::TPBASE) || itype == static_cast<int>(DATATYPE::TPWALLOPEN))
                continue;
            if (infile.atEnd())
                return false;
            infile >> mcount;
            for (int i = 0; i < mcount; i++)
            {
                if (infile.atEnd())
                    return false;
                infile >> id;
                if (infile.atEnd())
                    return false;
                infile >> mno;
                if (infile.atEnd())
                    return false;
                infile >> idlist;
                if (infile.atEnd())
                    return false;
                infile >> dtype;
                if (infile.atEnd())
                    return false;
                infile >> str;
                if (infile.atEnd())
                    return false;
                infile >> angle;
                if (infile.atEnd())
                    return false;
                infile >> shy;
                if (infile.atEnd())
                    return false;
                infile >> shz;
                if (infile.atEnd())
                    return false;
                infile >> ifrule;
                if (infile.atEnd())
                    return false;
                infile >> jfrule;
                if (infile.atEnd())
                    return false;
                infile >> iface;
                if (infile.atEnd())
                    return false;
                infile >> jface;
                if (infile.atEnd())
                    return false;
                infile >> pltype;
                if (infile.atEnd())
                    return false;
                infile >> prule;
                if (infile.atEnd())
                    return false;
                infile >> ilen;
                if (infile.atEnd())
                    return false;
                infile >> jlen;
                if (infile.atEnd())
                    return false;
                infile >> irr;
                if (infile.atEnd())
                    return false;
                infile >> jrr;
                if (infile.atEnd())
                    return false;
                infile >> wstr;
                if (infile.atEnd())
                    return false;
                infile >> iybstr;
                if (infile.atEnd())
                    return false;
                infile >> izbstr;
                if (infile.atEnd())
                    return false;
                infile >> jybstr;
                if (infile.atEnd())
                    return false;
                infile >> jzbstr;
            }
        }
        return true;
    }

    void ManageMemberData::readData(QDataStream &infile, const LibraryVersion &ver)
    {
        int mcount;

        infile >> mcount;
        //    qDebug()<<"mcount="<<mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            //        qDebug()<<md->getUuid();

            columnList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            girderList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            braceList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            beamList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            slabList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            wallList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }

        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            uwallList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            damperList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            isoList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            jointLoadList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            memberLoadList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }
        infile >> mcount;
        for (int i = 0; i < mcount; i++)
        {
            MemberData *md = new MemberData(infile, ver, isOutput);
            slabLoadList.append(md);
            uuidTable.insert(md->getUuid(), md);
        }

        Q_FOREACH (MemberData *md, girderList)
            md->recalcLineLength();
        Q_FOREACH (MemberData *md, braceList)
            md->recalcLineLength();
        Q_FOREACH (MemberData *md, beamList)
            md->recalcLineLength();
    }
    void ManageMemberData::readData(QTextStream &infile, DATATYPE type, const LibraryVersion &ver)
    {
        int mcount;
        QString str = infile.readLine(0).split(",").at(0);

        mcount = str.toInt();

        str = infile.readLine(0);
        switch (type)
        {

        case DATATYPE::TPCOLUMN:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                //        qDebug()<<md->getUuid();

                columnList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPGIRDER:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                girderList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPBRACE:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                braceList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPBEAM:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                beamList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPSLAB:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                slabList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPWALL:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                wallList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPUWALL:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                uwallList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPDAMPER:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                damperList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPISO:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                isoList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPJOINTLOAD:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                jointLoadList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPMEMBERLOAD:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                memberLoadList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        case DATATYPE::TPSLABLOAD:
            for (int i = 0; i < mcount; i++)
            {
                MemberData *md = new MemberData(infile, ver, isOutput);
                slabLoadList.append(md);
                uuidTable.insert(md->getUuid(), md);
            }
            break;
        default:
            break;
        }

        Q_FOREACH (MemberData *md, girderList)
            md->recalcLineLength();
        Q_FOREACH (MemberData *md, braceList)
            md->recalcLineLength();
        Q_FOREACH (MemberData *md, beamList)
            md->recalcLineLength();
    }

    void ManageMemberData::writePsvData(QTextStream &outfile) const
    {
        outfile << "**COLUMN_MEMBER\n";
        outfile << columnList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, columnList)
        {
            md->writePsvData(outfile);
        }
        outfile << "**GIRDER_MEMBER\n";
        outfile << girderList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, girderList)
        {
            md->writePsvData(outfile);
        }
        outfile << "**BRACE_MEMBER\n";
        outfile << braceList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, braceList)
        {
            md->writePsvData(outfile);
        }
        outfile << "**BEAM_MEMBER\n";
        outfile << beamList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, beamList)
        {
            md->writePsvData(outfile);
        }
        outfile << "**SLAB_MEMBER\n";
        outfile << slabList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, slabList)
        {
            md->writePsvData(outfile);
        }
        // p3d-psvとしてはWALL_MEMBERでまとめてしまいたい。nsvとしては分けて考えたい
        // TopMenuWidget::readPsvFileや、ReadFromPsv::readPsvと一致させる。断面は構造種別ごとにwriteする
        outfile << "**WALL_MEMBER\n";
        outfile << wallList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, wallList)
        {
            md->writePsvData(outfile);
        }

        outfile << "**UWALL_MEMBER\n";
        outfile << uwallList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, uwallList)
        {
            md->writePsvData(outfile);
        }
        outfile << "**DAMPER_MEMBER\n";
        outfile << damperList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, damperList)
        {
            md->writePsvData(outfile);
        }
        outfile << "**ISOLATOR_MEMBER\n";
        outfile << isoList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, isoList)
        {
            md->writePsvData(outfile);
        }
        outfile << "**JOINTLOAD_MEMBER\n";
        outfile << jointLoadList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, jointLoadList)
        {
            md->writePsvData(outfile);
        }
        outfile << "**MEMBERLOAD_MEMBER\n";
        outfile << memberLoadList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, memberLoadList)
        {
            md->writePsvData(outfile);
        }
        outfile << "**SLABLOAD_MEMBER\n";
        outfile << slabLoadList.count() << "\n";
        outfile << u8"UUID" << "\t"
                << "No"
                << "\n";
        Q_FOREACH (MemberData *md, slabLoadList)
        {
            md->writePsvData(outfile);
        }
    }
    void ManageMemberData::readAdditionalData(QDataStream &infile, const LibraryVersion &ver,
                                              const QHash<QUuid, QUuid> &idToIds, const FILEREADPATTERN &fpt)
    {
        Q_UNUSED(ver);
        QList<QUuid> new_milist;
        QList<QList<JointData *>> new_mjlist;
        QList<MEMBERVALUES> new_mvlist;

        int mcount;

        for (int ii = static_cast<int>(DATATYPE::TPCOLUMN); ii <= static_cast<int>(DATATYPE::TPSLABLOAD); ii++)
        {

            if (ii == static_cast<int>(DATATYPE::TPBASE) || ii == static_cast<int>(DATATYPE::TPWALLOPEN))
                continue;

            infile >> mcount;
            for (int i = 0; i < mcount; i++)
            {

                QUuid muid;
                QList<QUuid> idlist;
                MEMBERVALUES mvar;
                int dtype, ifrule, jfrule, pltype, prule, mno;
                infile >> muid >> mno >> idlist >> dtype >> mvar.sectionName >> mvar.codeAngle >> mvar.shiftY >> mvar.shiftZ >> ifrule >> jfrule >> mvar.iFace >> mvar.jFace >> pltype >> prule >> mvar.iLength >> mvar.jLength >> mvar.iRatio >> mvar.jRatio >> mvar.wallOpenName;
                mvar.dataType = static_cast<DATATYPE>(dtype + static_cast<int>(DATATYPE::TPCOLUMN));
                mvar.iFaceRule = static_cast<FACERULE>(ifrule);
                mvar.jFaceRule = static_cast<FACERULE>(jfrule);
                mvar.partingLength = static_cast<PARTINGLEN>(pltype);
                mvar.partingRule = static_cast<PARTINGRULE>(prule);

                if (uuidTable.contains(muid))
                {
                    if (fpt.priorList != fpt.priorMID)
                    {
                        MemberData *md = uuidTable.value(muid);
                        bool isTypeChanged = (md->dataType() != mvar.dataType);
                        if (isTypeChanged)
                            removeDataList(md);
                        QList<QUuid> o_jidlist;
                        Q_FOREACH (QUuid uid, idlist)
                        {
                            (idToIds.contains(uid)) ? o_jidlist.append(idToIds.value(uid))
                                                    : o_jidlist.append(uid);
                        }
                        md->replaceJointList(FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(o_jidlist));
                        md->copyValues(*FactoryUnifiedData::getInstance(isOutput), mvar);
                        if (isTypeChanged)
                            appendDataList(md);
                    }
                    continue;
                }

                QList<QUuid> o_jidlist;
                Q_FOREACH (QUuid uid, idlist)
                {
                    (idToIds.contains(uid)) ? o_jidlist.append(idToIds.value(uid))
                                            : o_jidlist.append(uid);
                }

                new_milist.append(muid);
                new_mjlist.append(FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(o_jidlist));
                new_mvlist.append(mvar);
            }
        }

        for (int i = 0; i < new_milist.count(); i++)
        {

            QUuid muid = new_milist.at(i);
            QList<JointData *> mjdlist = new_mjlist.at(i);
            MEMBERVALUES mvar = new_mvlist.at(i);
            DATATYPE dtype = mvar.dataType;

            JointData *first_jd = (dtype == DATATYPE::TPSLAB || dtype == DATATYPE::TPWALL || dtype == DATATYPE::TPUWALL || dtype == DATATYPE::TPSLABLOAD)
                                      ? FactoryUnifiedData::getInstance(isOutput)->simplifiedMemberJoints(mjdlist, true).first()
                                      : mjdlist.first();

            MemberData *overlapMember = nullptr;
            Q_FOREACH (MemberData *r_md, first_jd->getRelatedMembers())
            {
                if (dtype != r_md->dataType())
                    continue;
                if (FactoryUnifiedData::getInstance(isOutput)->isOverlapJoints(mjdlist, r_md->getJointList(),
                                                                               r_md->isClosedMember()))
                {
                    overlapMember = r_md;
                    break;
                }
            }

            int max_index = 0;
            Q_FOREACH (MemberData *md, memberListOfAll())
            {
                int index = md->getMemberNo();
                if (max_index < index)
                    max_index = index;
            }

            // 重複部材が見つからなかった場合
            if (overlapMember == nullptr)
            {
                MemberData *md = new MemberData(mjdlist, mvar.dataType, mvar.sectionName, muid, max_index + 1, isOutput);
                md->copyValues(*FactoryUnifiedData::getInstance(isOutput), mvar);
                appendDataList(md);
                // 重複部材が見つかって、重複させたままの場合
            }
            else if (fpt.priorMXYZ == FILEPRIOR::PRIOR_AB)
            {
                MemberData *md = new MemberData(mjdlist, mvar.dataType, mvar.sectionName, muid, max_index + 1, isOutput);
                md->copyValues(*FactoryUnifiedData::getInstance(isOutput), mvar);
                appendDataList(md);
                // 重複部材が見つかって、追加ファイル優先の場合
            }
            else if (fpt.priorList != fpt.priorMXYZ)
            {
                removeMember(overlapMember);
                MemberData *md = new MemberData(mjdlist, mvar.dataType, mvar.sectionName, muid, max_index + 1, isOutput);
                md->copyValues(*FactoryUnifiedData::getInstance(isOutput), mvar);
                appendDataList(md);
            }
        }

        Q_FOREACH (MemberData *md, girderList)
            md->recalcLineLength();
        Q_FOREACH (MemberData *md, braceList)
            md->recalcLineLength();
        Q_FOREACH (MemberData *md, beamList)
            md->recalcLineLength();
    }

    int ManageMemberData::dataCount(DATATYPE type) const
    {
        if (type == DATATYPE::TPCOLUMN)
        {
            return columnList.count();
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            return girderList.count();
        }
        else if (type == DATATYPE::TPBRACE)
        {
            return braceList.count();
        }
        else if (type == DATATYPE::TPBEAM)
        {
            return beamList.count();
        }
        else if (type == DATATYPE::TPSLAB)
        {
            return slabList.count();
        }
        else if (type == DATATYPE::TPWALL)
        {
            return wallList.count();
        }
        else if (type == DATATYPE::TPUWALL)
        {
            return uwallList.count();
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            return damperList.count();
        }
        else if (type == DATATYPE::TPISO)
        {
            return isoList.count();
        }
        else if (type == DATATYPE::TPJOINTLOAD)
        {
            return jointLoadList.count();
        }
        else if (type == DATATYPE::TPMEMBERLOAD)
        {
            return memberLoadList.count();
        }
        else if (type == DATATYPE::TPSLABLOAD)
        {
            return slabLoadList.count();
        }
        return 0;
    }

    QHash<QString, int> ManageMemberData::getMemberSectionCount(const UnifiedData &unified, DATATYPE type)
    {
        QStringList nameList = unified.getNameList(type);
        int nameCount = nameList.count();
        int *counter = new int[nameCount + 1];
        for (int i = 0; i < nameCount + 1; i++)
            counter[i] = 0;

        if (type == DATATYPE::TPCOLUMN)
        {
            return countSectionName(columnList, nameList, counter);
        }
        else if (type == DATATYPE::TPGIRDER)
        {
            return countSectionName(girderList, nameList, counter);
        }
        else if (type == DATATYPE::TPBRACE)
        {
            return countSectionName(braceList, nameList, counter);
        }
        else if (type == DATATYPE::TPBEAM)
        {
            return countSectionName(beamList, nameList, counter);
        }
        else if (type == DATATYPE::TPSLAB)
        {
            return countSectionName(slabList, nameList, counter);
        }
        else if (type == DATATYPE::TPWALL)
        {
            return countSectionName(wallList, nameList, counter);
        }
        else if (type == DATATYPE::TPUWALL)
        {
            return countSectionName(uwallList, nameList, counter);
        }
        else if (type == DATATYPE::TPDAMPER)
        {
            return countSectionName(damperList, nameList, counter);
        }
        else if (type == DATATYPE::TPISO)
        {
            return countSectionName(isoList, nameList, counter);
        }
        else if (type == DATATYPE::TPJOINTLOAD)
        {
            return countSectionName(jointLoadList, nameList, counter);
        }
        else if (type == DATATYPE::TPMEMBERLOAD)
        {
            return countSectionName(memberLoadList, nameList, counter);
        }
        else if (type == DATATYPE::TPSLABLOAD)
        {
            return countSectionName(slabLoadList, nameList, counter);
        }

        delete[] counter;
        return QHash<QString, int>();
    }

    QHash<QString, int> ManageMemberData::countSectionName(const QList<MemberData *> &memberList,
                                                           const QStringList &nameList, int counter[])
    {
        int nameCount = nameList.count();
        Q_FOREACH (MemberData *md, memberList)
        {
            if (md->sectionName().isEmpty())
            {
                counter[nameCount]++;
                continue;
            }
            int idx = nameList.indexOf(md->sectionName());
            counter[idx]++;
        }
        QHash<QString, int> sectionCountTable;
        sectionCountTable.insert("(dummy)", counter[nameCount]);
        for (int i = 0; i < nameCount; i++)
        {
            sectionCountTable.insert(nameList.at(i), counter[i]);
        }
        return sectionCountTable;
    }

    QString ManageMemberData::writeColumnNsvData(QSharedPointer<N3DUuidTable> table, QList<MemberData *> *oneMember) const
    {
        QString text;
        // TODO 中間節点がある場合のコードアングル
        // TODO 使用断面の判別

        int count = 0;
        QString tempColumn;
        Q_FOREACH (MemberData *md, columnList)
        {
            auto list = md->writeNsvData(table, DATATYPE::TPCOLUMN);
            Q_FOREACH (auto t, list)
            {
                tempColumn.append(t).append("\n");
                count++;
            }
            if (list.count() > 1)
            {
                oneMember->append(md);
            }
        }
        text.append(u8"**COLUMN_MEMBER, ").append(QString::number(count)).append("\n");
        text.append(u8"#ID, I端節点番号, J端節点番号, 符号,　I端使用断面, 中央使用断面, J端使用断面, コードアングル(rad),　寄り長さY(mm), 寄り長さZ(mm), i端フェイス長さ(mm),　j端フェイス長さ(mm)\n");
        text.append(tempColumn);
        return text;
    }

    QString ManageMemberData::writeGirderNsvData(QSharedPointer<N3DUuidTable> table, QList<MemberData *> *oneMember) const
    {
        QString text;
        int count = 0;
        QString tempGirder;
        Q_FOREACH (MemberData *md, girderList)
        {
            // TODO
            // 大梁のとりつきは部材単位で一つ（解析単位ではないことにする)
            QString girderSlab = u8",2,127.0,127.0,150.0,FC21";
            auto list = md->writeNsvData(table, DATATYPE::TPGIRDER);
            Q_FOREACH (auto t, list)
            {
                tempGirder.append(t).append(girderSlab).append("\n");
                count++;
            }
            if (list.count() > 1)
            {
                oneMember->append(md);
            }
        }
        text.append(u8"**GIRDER_MEMBER, ").append(QString::number(count)).append("\n");
        text.append(u8"#ID, I端節点番号, J端節点番号, 符号,　I端使用断面, 中央使用断面, J端使用断面,　コードアングル(rad),　寄り長さY(mm), 寄り長さZ(mm), i端フェイス長さ(mm),　j端フェイス長さ(mm), スラブの付き方, a+(mm2), a-(mm2), t(mm), コンクリート材種\n");
        text.append(tempGirder);
        return text;
    }

    QString ManageMemberData::writeBraceNsvData(QSharedPointer<N3DUuidTable> table, QList<MemberData *> *oneMember) const
    {
        QString text;
        int count = 0;
        QString tempBrace;
        Q_FOREACH (MemberData *md, braceList)
        {
            auto list = md->writeNsvData(table, DATATYPE::TPBRACE);
            Q_FOREACH (auto t, list)
            {
                tempBrace.append(t).append("\n");
                count++;
            }
            if (list.count() > 1)
                oneMember->append(md);
        }
        text.append(u8"**BRACE_MEMBER, ").append(QString::number(count)).append("\n");
        text.append(u8"#ID, I端節点番号, J端節点番号, 符号,　I端使用断面, 中央使用断面, J端使用断面　,　コードアングル(rad),　寄り長さY(mm), 寄り長さZ(mm), i端フェイス長さ(mm),　j端フェイス長さ(mm)\n");
        text.append(tempBrace);
        return text;
    }

    QString ManageMemberData::writeDamperNsvData(QSharedPointer<N3DUuidTable> table, QList<MemberData *> *oneMember) const
    {
        QString text;
        int count = 0;
        QString tempDamper;
        Q_FOREACH (MemberData *md, damperList)
        {
            auto list = md->writeNsvData(table, DATATYPE::TPDAMPER);
            Q_FOREACH (auto t, list)
            {
                tempDamper.append(t).append("\n");
                count++;
            }
            if (list.count() > 1)
                oneMember->append(md);
        }
        text.append(u8"**DAMPER_MEMBER, ").append(QString::number(count)).append("\n");
        text.append(u8"#ID, I端節点番号, J端節点番号, 符号　,　コードアングル(rad),　寄り長さY(mm), 寄り長さZ(mm), i端フェイス長さ(mm),　j端フェイス長さ(mm)\n");
        text.append(tempDamper);
        return text;
    }

    QString ManageMemberData::writeIsolatorNsvData(QSharedPointer<N3DUuidTable> table, QList<MemberData *> *oneMember) const
    {
        QString text;
        int count = 0;
        QString tempIsolator;
        Q_FOREACH (MemberData *md, isoList)
        {
            auto list = md->writeNsvData(table, DATATYPE::TPISO);
            Q_FOREACH (auto t, list)
            {
                tempIsolator.append(t).append("\n");
                count++;
            }
            if (list.count() > 1)
                oneMember->append(md);
        }
        text.append(u8"**ISOLATOR_MEMBER, ").append(QString::number(count)).append("\n");
        text.append(u8"#ID, I端節点番号, J端節点番号, 符号　,　コードアングル(rad),　寄り長さY(mm), 寄り長さZ(mm), i端フェイス長さ(mm),　j端フェイス長さ(mm)\n");
        text.append(tempIsolator);
        return text;
    }

    QString ManageMemberData::writeSlabNsvData(QSharedPointer<N3DUuidTable> table) const
    {
        QString text;
        int count = 0;
        QString tempSlab;
        Q_FOREACH (MemberData *md, slabList)
        {
            // 解析スラブ以外は除く
            auto unified = FactoryUnifiedData::getInstance(UnifiedDataType::FixedInput);
            auto section = unified->getSectionValues(DATATYPE::TPSLAB, md->sectionName());
            if (section.plane_param.planeType == PlaneType::Type::UNDEFINED)
                continue;
            if (section.plane_param.planeType == PlaneType::Type::NONSTRUCT)
                continue;

            Q_FOREACH (auto t, md->writeNsvData(table, DATATYPE::TPSLAB))
            {
                tempSlab.append(t).append("\n");
                count++;
            }
        }
        text.append(u8"**SLAB_MEMBER, ").append(QString::number(count)).append("\n");
        text.append(u8"#ID, 符号, 節点×n\n");
        text.append(tempSlab);
        return text;
    }

    QString ManageMemberData::writeWallNsvData(QSharedPointer<N3DUuidTable> table, QList<MemberData *> *cantiWall, QList<MemberData *> *beamWall) const
    {
        QString text;
        int rcCount = 0;
        int sCount = 0;
        int plateCount = 0;
        int membraneCount = 0;
        QString rcWall;
        QString sWall;
        QString plateWall;
        QString membraneWall;
        Q_FOREACH (MemberData *md, wallList)
        {
            // 　解析壁以外は除く
            auto unified = FactoryUnifiedData::getInstance(UnifiedDataType::FixedInput);
            auto section = unified->getSectionValues(DATATYPE::TPWALL, md->sectionName(), md->getJointList().first()->zPos());
            if (section.plane_param.planeType == PlaneType::Type::UNDEFINED)
                continue;

            if (section.plane_param.planeType == PlaneType::Type::NONSTRUCT)
                continue;

            if (false)
            {
                cantiWall->append(md);
                continue;
            }
            if (false)
            {
                beamWall->append(md);
                continue;
            }
            Q_FOREACH (auto t, md->writeNsvData(table, DATATYPE::TPWALL))
            {
                if (section.plane_param.planeType == PlaneType::Type::PLATE)
                {
                    plateWall.append(t).append("\n");
                    plateCount++;
                    continue;
                }
                if (section.plane_param.planeType == PlaneType::Type::MEMBRANE)
                {
                    membraneWall.append(t).append("\n");
                    membraneCount++;
                    continue;
                }

                switch (section.sectionType)
                {
                case STRUCTTYPE::STP_RC:
                    rcWall.append(t).append("\n");
                    rcCount++;
                    break;
                case STRUCTTYPE::STP_S:
                    sWall.append(t).append("\n");
                    sCount++;
                    break;
                default:
                    rcWall.append(t).append("\n");
                    rcCount++;
                }
            }
        }
        text.append(u8"**WALL_MEMBER, ").append(QString::number(rcCount)).append("\n");
        text.append(u8"#ID, 符号, 開口符号, 節点×n\n");
        text.append(rcWall);

        text.append(u8"**SWALL_MEMBER, ").append(QString::number(sCount)).append("\n");
        text.append(u8"#ID, 符号, 開口符号, 節点×n\n");
        text.append(sWall);

        text.append(u8"**PLATE_WALL_MEMBER, ").append(QString::number(plateCount)).append("\n");
        text.append(u8"#ID, 符号, 開口符号, 節点×n\n");
        text.append(plateWall);

        text.append(u8"**MEMBRANE_WALL_MEMBER, ").append(QString::number(membraneCount)).append("\n");
        text.append(u8"#ID, 符号, 開口符号, 節点×n\n");
        text.append(membraneWall);

        return text;
    }

    QString ManageMemberData::writeFoundationWallNsvData(QSharedPointer<N3DUuidTable> table) const
    {
        QString text;
        int count = 0;
        QString tempUWall;
        Q_FOREACH (MemberData *md, uwallList)
        {
            auto unified = FactoryUnifiedData::getInstance(UnifiedDataType::FixedInput);
            auto section = unified->getSectionValues(DATATYPE::TPUWALL, md->sectionName(), md->getJointList().first()->zPos());
            if (section.plane_param.planeType == PlaneType::Type::UNDEFINED)
                continue;

            if (section.plane_param.planeType == PlaneType::Type::NONSTRUCT)
                continue;

            if (section.plane_param.planeType == PlaneType::Type::PLATE)
                continue;

            if (section.plane_param.planeType == PlaneType::Type::MEMBRANE)
                continue;

            Q_FOREACH (auto t, md->writeNsvData(table, DATATYPE::TPUWALL))
            {
                tempUWall.append(t).append("\n");
                count++;
            }
        }
        text.append(u8"**UWALL_MEMBER, ").append(QString::number(count)).append("\n");
        text.append(u8"#ID, 符号, 開口符号, 節点×n\n");
        text.append(tempUWall);
        return text;
    }

    MemberData *ManageMemberData::createMember(const QList<JointData *> &jlist, DATATYPE type,
                                               const QString &str,
                                               const QUuid &id)
    {
        if (type == DATATYPE::TPCOLUMN || type == DATATYPE::TPGIRDER || type == DATATYPE::TPBRACE || type == DATATYPE::TPBEAM || type == DATATYPE::TPDAMPER || type == DATATYPE::TPISO || type == DATATYPE::TPMEMBERLOAD)
        {
            if (jlist.count() < 2)
                return nullptr;
        }
        else if (type == DATATYPE::TPSLAB || type == DATATYPE::TPWALL || type == DATATYPE::TPUWALL || type == DATATYPE::TPSLABLOAD)
        {
            if (jlist.count() < 3)
                return nullptr;
        }
        else if (type == DATATYPE::TPJOINTLOAD)
        {
            if (jlist.count() != 1)
                return nullptr;
        }
        else
        {
            return nullptr;
        }

        Q_FOREACH (JointData *jd, jlist)
        {
            if (!FactoryUnifiedData::getInstance(isOutput)->isExistedJoint(jd) || jlist.count(jd) >= 2)
                return NULL;
        }

        int max_index = 0;
        Q_FOREACH (MemberData *md, memberListOfAll())
        {
            int index = md->getMemberNo();
            if (max_index < index)
                max_index = index;
        }

        MemberData *md = new MemberData(jlist, type, str, id, max_index + 1, isOutput);
        appendDataList(md);
        return md;
    }

    void ManageMemberData::removeMember(MemberData *md)
    {
        removeDataList(md);
        delete md;
    }

    void ManageMemberData::removeJointOfMember(JointData *jd, MemberData *md)
    {

        if (md->isLineMember() && md->jointCount() > 2)
        {
            //        qDebug() << "removeJointOfMember 1" << md->sectionName();
            md->removeOneJoint(jd);
        }
        else if (md->isClosedMember() && md->jointCount() > 3)
        {
            //        qDebug() << "removeJointOfMember 2" << md->sectionName();
            md->removeOneJoint(jd);
        }
        else
        {
            //        qDebug() << "removeJointOfMember 3" << md->sectionName();
            removeMember(md);
        }

        /*
            DATATYPE type=md->dataType();
            int min_count=0;
            if( type==TPCOLUMN || type==TPGIRDER || type==TPBRACE || type==TPBEAM
                     || type==TPDAMPER || type==TPISO || type==TPMEMBERLOAD ){
                min_count=2;
            }else if( type==TPWALL || type==TPUWALL || type==TPSLAB || type==TPSLABLOAD ){
                min_count=3;
            }else if( type==TPJOINTLOAD ){
                min_count=1;
            }

            if( md->jointCount() > min_count ){
                md->removeOneJoint(jd);
            }else{
                removeMember(md);
            }
        */
    }

    void ManageMemberData::removeJointOfMember(JointData *jd, const QList<MemberData *> &mdlist)
    {
        Q_FOREACH (MemberData *md, mdlist)
            removeJointOfMember(jd, md);
    }

    void ManageMemberData::removeJointsOfMember(const QList<JointData *> &jdlist, MemberData *md)
    {
        Q_FOREACH (JointData *jd, jdlist)
        {
            removeJointOfMember(jd, md);
        }
    }

    bool ManageMemberData::isExistedMember(MemberData *md) const
    {
        if (md == nullptr)
            return false;
        return (uuidTable.values().contains(md));
        /*
            if( columnList.contains(md) ) return true;
            if( girderList.contains(md) ) return true;
            if( braceList.contains(md) ) return true;
            if( beamList.contains(md) ) return true;
            if( slabList.contains(md) ) return true;
            if( wallList.contains(md) ) return true;
            if( uwallList.contains(md) ) return true;
            if( damperList.contains(md) ) return true;
            if( isoList.contains(md) ) return true;
            if( jointLoadList.contains(md) ) return true;
            if( memberLoadList.contains(md) ) return true;
            if( slabLoadList.contains(md) ) return true;

            return false;
        */
    }

    bool ManageMemberData::isExistedID(const QUuid &id) const
    {
        if (id.isNull())
            return false;
        return (uuidTable.contains(id));
    }

    QList<MemberData *> ManageMemberData::memberListOfAll() const
    {
        QList<MemberData *> mdlist;
        mdlist << columnList << girderList << braceList << beamList
               << slabList << wallList << uwallList << damperList << isoList
               << jointLoadList << memberLoadList << slabLoadList;
        return mdlist;
    }

    QList<MemberData *> ManageMemberData::memberListExceptLoad() const
    {
        QList<MemberData *> mdlist;
        mdlist << columnList << girderList << braceList << beamList
               << slabList << wallList << uwallList << damperList << isoList;
        return mdlist;
    }

    QList<MemberData *> ManageMemberData::memberListOfFloor(const QUuid &id) const
    {
        QList<MemberData *> mdlist;
        Q_FOREACH (MemberData *md, columnList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, girderList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, braceList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, beamList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, slabList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, wallList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, uwallList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, damperList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, isoList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, jointLoadList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, memberLoadList)
            if (md->touchFloor(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, slabLoadList)
            if (md->touchFloor(id))
                mdlist.append(md);
        return mdlist;
    }

    QList<MemberData *> ManageMemberData::memberListOfFrame(const QUuid &id) const
    {
        QList<MemberData *> mdlist;
        Q_FOREACH (MemberData *md, columnList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, girderList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, braceList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, beamList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, slabList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, wallList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, uwallList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, damperList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, isoList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, jointLoadList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, memberLoadList)
            if (md->touchFrame(id))
                mdlist.append(md);
        Q_FOREACH (MemberData *md, slabLoadList)
            if (md->touchFrame(id))
                mdlist.append(md);
        return mdlist;
    }

    QList<MemberData *> ManageMemberData::memberListOfType(DATATYPE dtype) const
    {
        if (dtype == DATATYPE::TPCOLUMN)
        {
            return columnList;
        }
        else if (dtype == DATATYPE::TPGIRDER)
        {
            return girderList;
        }
        else if (dtype == DATATYPE::TPBRACE)
        {
            return braceList;
        }
        else if (dtype == DATATYPE::TPBEAM)
        {
            return beamList;
        }
        else if (dtype == DATATYPE::TPSLAB)
        {
            return slabList;
        }
        else if (dtype == DATATYPE::TPWALL)
        {
            return wallList;
        }
        else if (dtype == DATATYPE::TPUWALL)
        {
            return uwallList;
        }
        else if (dtype == DATATYPE::TPDAMPER)
        {
            return damperList;
        }
        else if (dtype == DATATYPE::TPISO)
        {
            return isoList;
        }
        else if (dtype == DATATYPE::TPJOINTLOAD)
        {
            return jointLoadList;
        }
        else if (dtype == DATATYPE::TPMEMBERLOAD)
        {
            return memberLoadList;
        }
        else if (dtype == DATATYPE::TPSLABLOAD)
        {
            return slabLoadList;
        }
        return QList<MemberData *>();
    }

    QList<MemberData *> ManageMemberData::memberListOfType(const QList<DATATYPE> &types) const
    {
        QList<MemberData *> mdlist;

        if (types.contains(DATATYPE::TPCOLUMN))
        {
            mdlist.append(columnList);
        }
        if (types.contains(DATATYPE::TPGIRDER))
        {
            mdlist.append(girderList);
        }
        if (types.contains(DATATYPE::TPBRACE))
        {
            mdlist.append(braceList);
        }
        if (types.contains(DATATYPE::TPBEAM))
        {
            mdlist.append(beamList);
        }
        if (types.contains(DATATYPE::TPSLAB))
        {
            mdlist.append(slabList);
        }
        if (types.contains(DATATYPE::TPWALL))
        {
            mdlist.append(wallList);
        }
        if (types.contains(DATATYPE::TPUWALL))
        {
            mdlist.append(uwallList);
        }
        if (types.contains(DATATYPE::TPDAMPER))
        {
            mdlist.append(damperList);
        }
        if (types.contains(DATATYPE::TPISO))
        {
            mdlist.append(isoList);
        }
        if (types.contains(DATATYPE::TPJOINTLOAD))
        {
            mdlist.append(jointLoadList);
        }
        if (types.contains(DATATYPE::TPMEMBERLOAD))
        {
            mdlist.append(memberLoadList);
        }
        if (types.contains(DATATYPE::TPSLABLOAD))
        {
            mdlist.append(slabLoadList);
        }

        return mdlist;
    }

    MemberData *ManageMemberData::memberIDToPointer(const QUuid &id) const
    {
        if (id.isNull())
            return nullptr;
        return (uuidTable.contains(id)) ? uuidTable.value(id) : nullptr;
    }

    QList<MemberData *> ManageMemberData::memberIDsToPointers(const QList<QUuid> &idlist) const
    {
        QList<MemberData *> mdlist;
        Q_FOREACH (QUuid id, idlist)
            mdlist.append(memberIDToPointer(id));
        return mdlist;
    }

    void ManageMemberData::appendDataList(MemberData *md)
    {
        if (md->dataType() == DATATYPE::TPCOLUMN)
        {
            columnList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPGIRDER)
        {
            girderList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPBRACE)
        {
            braceList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPBEAM)
        {
            beamList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPSLAB)
        {
            slabList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPWALL)
        {
            wallList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPUWALL)
        {
            uwallList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPDAMPER)
        {
            damperList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPISO)
        {
            isoList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPJOINTLOAD)
        {
            jointLoadList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPMEMBERLOAD)
        {
            memberLoadList.append(md);
        }
        else if (md->dataType() == DATATYPE::TPSLABLOAD)
        {
            slabLoadList.append(md);
        }
        else
        {
            return;
        }

        uuidTable.insert(md->getUuid(), md);
    }

    void ManageMemberData::removeDataList(MemberData *md)
    {
        if (md->dataType() == DATATYPE::TPCOLUMN)
        {
            columnList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPGIRDER)
        {
            girderList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPBRACE)
        {
            braceList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPBEAM)
        {
            beamList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPSLAB)
        {
            slabList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPWALL)
        {
            wallList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPUWALL)
        {
            uwallList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPDAMPER)
        {
            damperList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPISO)
        {
            isoList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPJOINTLOAD)
        {
            jointLoadList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPMEMBERLOAD)
        {
            memberLoadList.removeOne(md);
        }
        else if (md->dataType() == DATATYPE::TPSLABLOAD)
        {
            slabLoadList.removeOne(md);
        }
        else
        {
            return;
        }

        QUuid id = uuidTable.key(md);
        if (!id.isNull())
            uuidTable.remove(id);
    }

    // 部材の線上に内分点を作る

    bool ManageMemberData::interiorMember(MemberData *md, QList<JointData *> jdlist)
    {
        QList<JointData *> m_jdlist = md->orderOnLineJoint(jdlist);

        if (m_jdlist.count() < 1)
            return false;

        for (int i = 1; i < m_jdlist.count() - 1; i++)
        {
            if (jdlist.contains(m_jdlist.at(i)))
            {
                if (md->isClosedMember())
                {
                    md->insertComposeJoint(i, m_jdlist.at(i));
                }
                else
                {
                    md->insertInteriorJoint(i, m_jdlist.at(i));
                }
            }
        }
        if (md->isClosedMember())
        {
            if (jdlist.contains(m_jdlist.last()))
            {
                md->insertComposeJoint(m_jdlist.count() - 1, m_jdlist.last());
            }
        }

        return true;
    }

    // 部材を分割する  元部材mdは削除せずにそのまま、mdを分割したリストを返す。重複状態のまま。
    // 分割点は内分点であってはいけない
    QList<MemberData *> ManageMemberData::divideMember(const UnifiedData &unified, MemberData *md, const QList<JointData *> &jdlist,
                                                       const QList<QUuid> &idlist)
    {
        if (md->isClosedMember())
            return QList<MemberData *>();
        QList<JointData *> m_jdlist = md->orderOnLineJoint(jdlist);
        if (m_jdlist.count() < 1)
            return QList<MemberData *>();

        MEMBERVALUES values = md->getMemberValues();
        QList<MemberData *> created_mdlist;
        int ipre = 0;
        for (int i = 1; i < m_jdlist.count() - 1; i++)
        {
            if (jdlist.contains(m_jdlist.at(i)))
            {
                QList<JointData *> n_jdlist = m_jdlist.mid(ipre, i - ipre + 1);
                QUuid id = (idlist.count() > created_mdlist.count()) ? idlist.at(created_mdlist.count()) : QUuid();
                MemberData *n_md = createMember(n_jdlist, values.dataType, values.sectionName, id);
                n_md->copyValues(unified, values);
                created_mdlist.append(n_md);
                ipre = i;
            }
        }
        QList<JointData *> n_jdlist = m_jdlist.mid(ipre);
        QUuid id = (idlist.count() > created_mdlist.count()) ? idlist.at(created_mdlist.count()) : QUuid();
        MemberData *n_md = createMember(n_jdlist, values.dataType, values.sectionName, id);
        n_md->copyValues(unified, values);
        created_mdlist.append(n_md);

        return created_mdlist;
    }

    // 部材を既存の内分点で分割する　元部材mdは削除せずにそのまま、mdを分割したリストを返す。重複状態のまま。
    QList<MemberData *> ManageMemberData::divideMemberByInteriorJoints(const UnifiedData &unified, MemberData *md,
                                                                       const QList<JointData *> &in_jdlist,
                                                                       const QList<QUuid> &idlist)
    {
        if (md->isClosedMember())
            return QList<MemberData *>();
        QList<JointData *> org_jdlist = md->getJointList();
        if (org_jdlist.count() < 3)
            return QList<MemberData *>();

        QList<int> indexList;
        indexList.append(0);
        Q_FOREACH (JointData *jd, in_jdlist)
        {
            int idx = org_jdlist.indexOf(jd);
            if (idx < 1 || idx == org_jdlist.count() - 1)
                return QList<MemberData *>();
            indexList.append(idx);
        }

        MEMBERVALUES values = md->getMemberValues();
        QList<MemberData *> created_mdlist;

        for (int i = 1; i < indexList.count(); i++)
        {
            int ii = indexList.at(i - 1);
            int jj = indexList.at(i) - ii + 1;
            QList<JointData *> new_jdlist = org_jdlist.mid(ii, jj);
            QUuid id = (idlist.count() > created_mdlist.count()) ? idlist.at(created_mdlist.count()) : QUuid();
            MemberData *n_md = createMember(new_jdlist, values.dataType, values.sectionName, id);
            n_md->copyValues(unified, values);
            created_mdlist.append(n_md);
        }

        int ii = indexList.last();
        int jj = org_jdlist.count() - ii;
        QList<JointData *> new_jdlist = org_jdlist.mid(ii, jj);
        QUuid id = (idlist.count() > created_mdlist.count()) ? idlist.at(created_mdlist.count()) : QUuid();
        MemberData *n_md = createMember(new_jdlist, values.dataType, values.sectionName, id);
        n_md->copyValues(unified, values);
        created_mdlist.append(n_md);

        return created_mdlist;
    }

    // 部材を１本化する
    void ManageMemberData::uniteMembers(const QList<MemberData *> &mdlist)
    {
        MemberData *remain_md = mdlist.first();
        QList<JointData *> jdlist = remain_md->getJointList();

        for (int i = 1; i < mdlist.count(); i++)
        {
            jdlist.removeLast();
            jdlist.append(mdlist.at(i)->getJointList());
            removeMember(mdlist.at(i));
        }

        remain_md->changeJointList(jdlist);
    }

    // 消去された断面情報を部材に反映
    QList<MemberData *> ManageMemberData::updateDeletedSection(const UnifiedData &unified, DATATYPE dtype, const QStringList &slist)
    {
        QList<MemberData *> mdlist;
        if (dtype == DATATYPE::TPCOLUMN)
        {
            Q_FOREACH (MemberData *md, columnList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPGIRDER)
        {
            Q_FOREACH (MemberData *md, girderList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPBRACE)
        {
            Q_FOREACH (MemberData *md, braceList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPBEAM)
        {
            Q_FOREACH (MemberData *md, beamList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPSLAB)
        {
            Q_FOREACH (MemberData *md, slabList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPWALL)
        {
            Q_FOREACH (MemberData *md, wallList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPUWALL)
        {
            Q_FOREACH (MemberData *md, uwallList)
            {
                if (slist.contains(md->wallOpenName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPWALLOPEN)
        {
            Q_FOREACH (MemberData *md, wallList)
            {
                if (slist.contains(md->wallOpenName()))
                {
                    md->setWallOpen("");
                    mdlist.append(md);
                }
            }
            Q_FOREACH (MemberData *md, uwallList)
            {
                if (slist.contains(md->wallOpenName()))
                {
                    md->setWallOpen("");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPBASE)
        {
            Q_FOREACH (MemberData *md, columnList)
            {
                if (slist.contains(md->basePlateName()))
                {
                    md->resetLineShape(unified);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPDAMPER)
        {
            Q_FOREACH (MemberData *md, damperList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPISO)
        {
            Q_FOREACH (MemberData *md, isoList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPJOINTLOAD)
        {
            Q_FOREACH (MemberData *md, jointLoadList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPMEMBERLOAD)
        {
            Q_FOREACH (MemberData *md, memberLoadList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPSLABLOAD)
        {
            Q_FOREACH (MemberData *md, slabLoadList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->setSectionName(unified, "");
                    mdlist.append(md);
                }
            }
        }
        return mdlist;
    }

    // 変更された断面情報を部材に反映
    QList<MemberData *> ManageMemberData::updateChangedSection(const UnifiedData &unified, DATATYPE dtype, const QStringList &slist)
    {
        QList<MemberData *> mdlist;
        if (dtype == DATATYPE::TPCOLUMN)
        {
            Q_FOREACH (MemberData *md, columnList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPGIRDER)
        {
            Q_FOREACH (MemberData *md, girderList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPBRACE)
        {
            Q_FOREACH (MemberData *md, braceList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPBEAM)
        {
            Q_FOREACH (MemberData *md, beamList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPSLAB)
        {
            Q_FOREACH (MemberData *md, slabList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified); // ????
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPWALL)
        {
            Q_FOREACH (MemberData *md, wallList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified); // ????
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPUWALL)
        {
            Q_FOREACH (MemberData *md, uwallList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified); // ????
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPWALLOPEN)
        {
            Q_FOREACH (MemberData *md, wallList)
            {
                if (slist.contains(md->wallOpenName()))
                {
                    mdlist.append(md);
                }
            }
            Q_FOREACH (MemberData *md, uwallList)
            {
                if (slist.contains(md->wallOpenName()))
                {
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPBASE)
        {
            Q_FOREACH (MemberData *md, columnList)
            {
                if (slist.contains(md->basePlateName()))
                {
                    md->resetLineShape(unified);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPDAMPER)
        {
            Q_FOREACH (MemberData *md, damperList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPISO)
        {
            Q_FOREACH (MemberData *md, isoList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPJOINTLOAD)
        {
            Q_FOREACH (MemberData *md, jointLoadList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified); // ????
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPMEMBERLOAD)
        {
            Q_FOREACH (MemberData *md, memberLoadList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified); // ????
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPSLABLOAD)
        {
            Q_FOREACH (MemberData *md, slabLoadList)
            {
                if (slist.contains(md->sectionName()))
                {
                    md->resetLineShape(unified); // ????
                    mdlist.append(md);
                }
            }
        }
        return mdlist;
    }

    void ManageMemberData::recalcMemberCoordinate()
    {
        Q_FOREACH (MemberData *md, columnList)
            md->recalcLocalVector();
        Q_FOREACH (MemberData *md, girderList)
            md->recalcLocalVector();
        Q_FOREACH (MemberData *md, braceList)
            md->recalcLocalVector();
        Q_FOREACH (MemberData *md, beamList)
            md->recalcLocalVector();
        Q_FOREACH (MemberData *md, slabList)
            md->recalcLocalVector();
        Q_FOREACH (MemberData *md, wallList)
            md->recalcLocalVector();
        Q_FOREACH (MemberData *md, uwallList)
            md->recalcLocalVector();
        Q_FOREACH (MemberData *md, damperList)
            md->recalcLocalVector();
        Q_FOREACH (MemberData *md, isoList)
            md->recalcLocalVector();
        Q_FOREACH (MemberData *md, slabLoadList)
            md->recalcLocalVector();
    }

    QList<MemberData *> ManageMemberData::replaceSectionName(const UnifiedData &unified, DATATYPE dtype, const QString &str_a,
                                                             const QString &str_b)
    {
        QList<MemberData *> mdlist;
        if (dtype == DATATYPE::TPCOLUMN)
        {
            Q_FOREACH (MemberData *md, columnList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPGIRDER)
        {
            Q_FOREACH (MemberData *md, girderList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPBRACE)
        {
            Q_FOREACH (MemberData *md, braceList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPBEAM)
        {
            Q_FOREACH (MemberData *md, beamList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPSLAB)
        {
            Q_FOREACH (MemberData *md, slabList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPWALL)
        {
            Q_FOREACH (MemberData *md, wallList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPUWALL)
        {
            Q_FOREACH (MemberData *md, uwallList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPWALLOPEN)
        {
            Q_FOREACH (MemberData *md, wallList)
            {
                if (md->wallOpenName() == str_a)
                {
                    md->setWallOpen(str_b);
                    mdlist.append(md);
                }
            }
            Q_FOREACH (MemberData *md, uwallList)
            {
                if (md->wallOpenName() == str_a)
                {
                    md->setWallOpen(str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPDAMPER)
        {
            Q_FOREACH (MemberData *md, damperList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPISO)
        {
            Q_FOREACH (MemberData *md, isoList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPJOINTLOAD)
        {
            Q_FOREACH (MemberData *md, jointLoadList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPMEMBERLOAD)
        {
            Q_FOREACH (MemberData *md, memberLoadList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        else if (dtype == DATATYPE::TPSLABLOAD)
        {
            Q_FOREACH (MemberData *md, slabLoadList)
            {
                if (md->sectionName() == str_a)
                {
                    md->setSectionName(unified, str_b);
                    mdlist.append(md);
                }
            }
        }
        return mdlist;
    }

    ///// MemberData クラス //////
    MemberData::MemberData(const MemberData &md, const ManageJointData &mjd, const UnifiedData &unified)
    {
        no = md.no;
        isOutput = UnifiedDataType::FixedInput;
        memberID = md.memberID;
        Q_FOREACH (auto j, md.jointList)
        {
            jointList.append(mjd.jointIDToPointer(j->getUuid()));
        }

        myMemberValues = MEMBERVALUES{
            md.getMemberValues().dataType,
            md.getMemberValues().sectionName,
            md.getMemberValues().codeAngle,
            md.getMemberValues().shiftY,
            md.getMemberValues().shiftZ,
            md.getMemberValues().iFaceRule,
            md.getMemberValues().jFaceRule,
            md.getMemberValues().iFace,
            md.getMemberValues().jFace,
            md.getMemberValues().partingLength,
            md.getMemberValues().partingRule,
            md.getMemberValues().iLength,
            md.getMemberValues().jLength,
            md.getMemberValues().iRatio,
            md.getMemberValues().jRatio,
            md.getMemberValues().wallOpenName,
            md.getMemberValues().iyBaneName,
            md.getMemberValues().izBaneName,
            md.getMemberValues().jyBaneName,
            md.getMemberValues().jzBaneName};

        if (dataType() == DATATYPE::TPCOLUMN)
        {
            U_Vec = VEC3D(0.0, 0.0, 1.0);
            V_Vec = VEC3D(1.0, 0.0, 0.0);
        }
        else
        {
            U_Vec = VEC3D(1.0, 0.0, 0.0);
            V_Vec = VEC3D(0.0, 1.0, 0.0);
        }

        if (isClosedMember())
        {
            Q_FOREACH (JointData *jd, jointList)
                jd->appendComposeMember(this);
        }
        else
        {
            jointList.first()->appendComposeMember(this);
            jointList.last()->appendComposeMember(this);
            if (jointList.count() > 2)
            {
                for (int i = 1; i < jointList.count() - 1; i++)
                {
                    jointList.at(i)->appendInteriorMember(this);
                }
            }
        }
        resetLineShape(unified);
        recalcLineLength();
        recalcLocalVector();
    }

    /*　jointListのカウントチェック、jointData*のexistはManageクラスでチェック */

    MemberData::MemberData(const QList<JointData *> &jdlist, DATATYPE type, const QString &iname,
                           const QUuid &id, int _no, UnifiedDataType isout)
    {
        no = _no;
        isOutput = isout;
        memberID = (id.isNull()) ? QUuid::createUuid() : id;

        jointList = jdlist;
        myMemberValues = MEMBERVALUES(type, iname);
        myPlaneValue = PLANEPROPERTY();

        if (type == DATATYPE::TPCOLUMN)
        {
            U_Vec = VEC3D(0.0, 0.0, 1.0);
            V_Vec = VEC3D(1.0, 0.0, 0.0);
        }
        else
        {
            U_Vec = VEC3D(1.0, 0.0, 0.0);
            V_Vec = VEC3D(0.0, 1.0, 0.0);
        }

        if (isClosedMember() || isJointMember())
        {
            Q_FOREACH (JointData *jd, jointList)
                jd->appendComposeMember(this);
        }
        else
        {
            jointList.first()->appendComposeMember(this);
            jointList.last()->appendComposeMember(this);
            if (jointList.count() > 2)
            {
                for (int i = 1; i < jointList.count() - 1; i++)
                {
                    jointList.at(i)->appendInteriorMember(this);
                }
            }
        }

        resetLineShape(*FactoryUnifiedData::getInstance(isOutput));
        recalcLineLength();
        recalcLocalVector();
        //    if( execRecalc ) recalcLineLength();
    }

    MemberData::MemberData(QDataStream &infile, const LibraryVersion &ver, UnifiedDataType isout)
    {

        isOutput = isout;
        QList<QUuid> idlist;
        int dtype, ifrule, jfrule, pltype, prule;
        infile >> memberID >> no >> idlist >> dtype >> myMemberValues.sectionName >> myMemberValues.codeAngle >> myMemberValues.shiftY >> myMemberValues.shiftZ >> ifrule >> jfrule >> myMemberValues.iFace >> myMemberValues.jFace >> pltype >> prule >> myMemberValues.iLength >> myMemberValues.jLength >> myMemberValues.iRatio >> myMemberValues.jRatio >> myMemberValues.wallOpenName >> myMemberValues.iyBaneName >> myMemberValues.izBaneName >> myMemberValues.jyBaneName >> myMemberValues.jzBaneName;

        myMemberValues.dataType = static_cast<DATATYPE>(dtype + static_cast<int>(DATATYPE::TPCOLUMN)); // enum
        myMemberValues.iFaceRule = static_cast<FACERULE>(ifrule);
        myMemberValues.jFaceRule = static_cast<FACERULE>(jfrule);
        myMemberValues.partingLength = static_cast<PARTINGLEN>(pltype);
        myMemberValues.partingRule = static_cast<PARTINGRULE>(prule);

        if (memberID.isNull())
            memberID = QUuid::createUuid();

        jointList = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(idlist);

        if (isClosedMember())
        {
            Q_FOREACH (JointData *jd, jointList)
                jd->appendComposeMember(this);
        }
        else
        {
            jointList.first()->appendComposeMember(this);
            jointList.last()->appendComposeMember(this);
            if (jointList.count() > 2)
            {
                for (int i = 1; i < jointList.count() - 1; i++)
                {
                    jointList.at(i)->appendInteriorMember(this);
                }
            }
        }
        resetLineShape(*FactoryUnifiedData::getInstance(isOutput));
        recalcLineLength();
        recalcLocalVector();
    }
    MemberData::MemberData(QTextStream &infile, const LibraryVersion &ver, UnifiedDataType isout)
    {
        Q_UNUSED(ver);

        isOutput = isout;
        // QList<QUuid> jointidlist =  FactoryUnifiedData::getInstance(isOutput)->getIDList(TPJOINT);
        QList<QUuid> idlist;
        int dtype, ifrule, jfrule, pltype, prule;
        QString str = infile.readLine(0);
        QStringList strlist = str.split("\t");

        memberID = QUuid(strlist.at(0));
        no = strlist.at(1).toInt();
        str = infile.readLine(0);
        strlist = str.split("\t");
        for (int i = 1; i <= strlist.at(0).toInt(); i++)
        {
            if (!strlist.at(i).isEmpty())
            {
                idlist.append(QUuid(strlist.at(i)));
            }
        }
        str = infile.readLine(0);
        strlist = str.split("\t");
        dtype = strlist.at(0).toInt();
        myMemberValues.sectionName = strlist.at(1);
        myMemberValues.codeAngle = strlist.at(2).toDouble();
        myMemberValues.shiftY = strlist.at(3).toDouble();
        myMemberValues.shiftZ = strlist.at(4).toDouble();
        ifrule = strlist.at(5).toInt();
        jfrule = strlist.at(6).toInt();
        myMemberValues.iFace = strlist.at(7).toDouble();
        myMemberValues.jFace = strlist.at(8).toDouble();
        pltype = strlist.at(9).toInt();
        prule = strlist.at(10).toInt();
        myMemberValues.iLength = strlist.at(11).toDouble();
        myMemberValues.jLength = strlist.at(12).toDouble();
        myMemberValues.iRatio = strlist.at(13).toDouble();
        myMemberValues.jRatio = strlist.at(14).toDouble();
        myMemberValues.wallOpenName = strlist.at(15);

        myMemberValues.iyBaneName = strlist.at(16);
        myMemberValues.izBaneName = strlist.at(17);
        myMemberValues.jyBaneName = strlist.at(18);
        myMemberValues.jzBaneName = strlist.at(19);

        myMemberValues.dataType = static_cast<DATATYPE>(dtype + static_cast<int>(DATATYPE::TPCOLUMN)); // enum
        myMemberValues.iFaceRule = static_cast<FACERULE>(ifrule);
        myMemberValues.jFaceRule = static_cast<FACERULE>(jfrule);
        myMemberValues.partingLength = static_cast<PARTINGLEN>(pltype);
        myMemberValues.partingRule = static_cast<PARTINGRULE>(prule);

        if (memberID.isNull())
            memberID = QUuid::createUuid();

        jointList = FactoryUnifiedData::getInstance(isOutput)->jointIDsToPointers(idlist);

        if (isClosedMember())
        {
            Q_FOREACH (JointData *jd, jointList)
                jd->appendComposeMember(this);
        }
        else
        {
            jointList.first()->appendComposeMember(this);
            jointList.last()->appendComposeMember(this);
            if (jointList.count() > 2)
            {
                for (int i = 1; i < jointList.count() - 1; i++)
                {
                    jointList.at(i)->appendInteriorMember(this);
                }
            }
        }
        resetLineShape(*FactoryUnifiedData::getInstance(isOutput));
        recalcLineLength();
        recalcLocalVector();
    }
    MemberData::~MemberData()
    {
        if (isClosedMember())
        {
            Q_FOREACH (JointData *jd, jointList)
                jd->removeComposeMember(this);
        }
        else if (!jointList.isEmpty())
        {
            jointList.first()->removeComposeMember(this);
            jointList.last()->removeComposeMember(this);
            if (jointList.count() > 2)
            {
                for (int i = 1; i < jointList.count() - 1; i++)
                {
                    jointList.at(i)->removeInteriorMember(this);
                }
            }
        }
    }

    void MemberData::writeP3dData(QDataStream &outfile) const
    {
        QList<QUuid> idlist;
        Q_FOREACH (JointData *jd, jointList)
            idlist.append(jd->getUuid());
        outfile << memberID << no << idlist << static_cast<int>(myMemberValues.dataType) - static_cast<int>(DATATYPE::TPCOLUMN)
                << myMemberValues.sectionName << myMemberValues.codeAngle
                << myMemberValues.shiftY << myMemberValues.shiftZ
                << static_cast<int>(myMemberValues.iFaceRule)
                << static_cast<int>(myMemberValues.jFaceRule)
                << myMemberValues.iFace << myMemberValues.jFace
                << static_cast<int>(myMemberValues.partingLength)
                << static_cast<int>(myMemberValues.partingRule)
                << myMemberValues.iLength << myMemberValues.jLength
                << myMemberValues.iRatio << myMemberValues.jRatio
                << myMemberValues.wallOpenName
                << myMemberValues.iyBaneName << myMemberValues.izBaneName
                << myMemberValues.jyBaneName << myMemberValues.jzBaneName;
    }

    void MemberData::writePsvData(QTextStream &outfile) const
    {
        const QString tab = "\t";

        QList<QUuid> idlist;
        Q_FOREACH (JointData *jd, jointList)
        {
            idlist.append(jd->getUuid());
        }

        outfile << memberID.toString() << tab
                << QString::number(no) << "\n";
        outfile << idlist.count() << tab;
        foreach (auto id, idlist)
        {
            outfile << id.toString() << tab;
        }
        outfile << "\n";
        outfile << static_cast<int>(myMemberValues.dataType) - static_cast<int>(DATATYPE::TPCOLUMN) << tab
                << myMemberValues.sectionName << tab
                << myMemberValues.codeAngle << tab
                << myMemberValues.shiftY << tab
                << myMemberValues.shiftZ << tab
                << static_cast<int>(myMemberValues.iFaceRule) << tab
                << static_cast<int>(myMemberValues.jFaceRule) << tab
                << myMemberValues.iFace << tab
                << myMemberValues.jFace << tab
                << static_cast<int>(myMemberValues.partingLength) << tab
                << static_cast<int>(myMemberValues.partingRule) << tab
                << myMemberValues.iLength << tab
                << myMemberValues.jLength << tab
                << myMemberValues.iRatio << tab
                << myMemberValues.jRatio << tab
                << myMemberValues.wallOpenName << tab
                << myMemberValues.iyBaneName << tab
                << myMemberValues.izBaneName << tab
                << myMemberValues.jyBaneName << tab
                << myMemberValues.jzBaneName << "\n";
    }

    QList<QString> MemberData::writeNsvData(QSharedPointer<N3DUuidTable> table, const DATATYPE &datatype) const
    {
        QList<QString> list;
        QList<JointData *> analysisJointList;
        for (int i = 0; i < jointList.count(); i++)
        {
            if (jointList.at(i)->isAnalysisJoint(*FactoryUnifiedData::getInstance(isOutput)))
                analysisJointList.append(jointList.at(i));
        }

        switch (datatype)
        {
        case DATATYPE::TPCOLUMN:
        case DATATYPE::TPGIRDER:
        case DATATYPE::TPBRACE:
            for (int i = 0; i < analysisJointList.count() - 1; i++)
            {
                QString text;
                text.append(table->GetOrCreateN3dMemberNumber(memberID, i)).append(",");
                text.append(table->GetOrCreateN3dNumber(analysisJointList.at(i)->getUuid(), DATATYPE::TPJOINT)).append(",");
                text.append(table->GetOrCreateN3dNumber(analysisJointList.at(i + 1)->getUuid(), DATATYPE::TPJOINT)).append(",");
                text.append(myMemberValues.sectionName).append(","); // S
                text.append(myMemberValues.sectionName).append(","); // C
                text.append(myMemberValues.sectionName).append(","); // E
                text.append(QString::number(myMemberValues.codeAngle)).append(",");
                text.append(QString::number(myMemberValues.shiftY)).append(",");
                text.append(QString::number(myMemberValues.shiftZ)).append(",");
                text.append(QString::number(myMemberValues.iFace)).append(",");
                text.append(QString::number(myMemberValues.jFace)).append(",");
                text.append(myMemberValues.iyBaneName).append(",");
                text.append(myMemberValues.izBaneName).append(",");
                text.append(myMemberValues.jyBaneName).append(",");
                text.append(myMemberValues.jzBaneName);
                list.append(text);
            }
            break;
        case DATATYPE::TPDAMPER:
        case DATATYPE::TPISO:
            for (int i = 0; i < analysisJointList.count() - 1; i++)
            {
                QString text;
                text.append(table->GetOrCreateN3dMemberNumber(memberID, i)).append(",");
                text.append(table->GetOrCreateN3dNumber(analysisJointList.at(i)->getUuid(), DATATYPE::TPJOINT)).append(",");
                text.append(table->GetOrCreateN3dNumber(analysisJointList.at(i + 1)->getUuid(), DATATYPE::TPJOINT)).append(",");
                text.append(myMemberValues.sectionName).append(",");
                text.append(QString::number(myMemberValues.codeAngle)).append(",");
                text.append(QString::number(myMemberValues.shiftY)).append(",");
                text.append(QString::number(myMemberValues.shiftZ)).append(",");
                text.append(QString::number(myMemberValues.iFace)).append(",");
                text.append(QString::number(myMemberValues.jFace));
                list.append(text);
            }
            break;
        case DATATYPE::TPSLAB:
        {
            QString text;
            text.append(table->GetOrCreateN3dNumber(memberID, datatype)).append(",");
            text.append(myMemberValues.sectionName).append(",");
            for (int i = 0; i < analysisJointList.count(); i++)
            {
                text.append(table->GetOrCreateN3dNumber(analysisJointList.at(i)->getUuid(), DATATYPE::TPJOINT));
                if (i != analysisJointList.count() - 1)
                    text.append(",");
            }
            list.append(text);
        }
        break;
        case DATATYPE::TPWALL:
        case DATATYPE::TPUWALL:
        {
            QString text;
            text.append(table->GetOrCreateN3dNumber(memberID, datatype)).append(",");
            text.append(myMemberValues.sectionName).append(",");
            text.append(myMemberValues.wallOpenName).append(",");
            for (int i = 0; i < analysisJointList.count(); i++)
            {
                text.append(table->GetOrCreateN3dNumber(analysisJointList.at(i)->getUuid(), DATATYPE::TPJOINT));
                if (i != analysisJointList.count() - 1)
                    text.append(",");
            }
            list.append(text);
        }
        break;
        default:
            throw;
        }

        return list;
    }

    // 節点とDATATYPE以外のコピー
    void MemberData::copyValuesFromMemberData(const UnifiedData &unified, MemberData *md)
    {
        myMemberValues = md->getMemberValues();
        resetLineShape(unified);
        recalcFaceLength();
        recalcPartition();
    }

    void MemberData::copyValues(const UnifiedData &unified, const MEMBERVALUES &values)
    {
        myMemberValues = values;
        resetLineShape(unified);
        recalcFaceLength();
        recalcPartition();
    }

    bool MemberData::isLineMember(bool includingLoad) const
    {
        DATATYPE dtype = myMemberValues.dataType;
        if (includingLoad && dtype == DATATYPE::TPMEMBERLOAD)
            return true;
        return (dtype == DATATYPE::TPCOLUMN || dtype == DATATYPE::TPGIRDER || dtype == DATATYPE::TPBRACE ||
                dtype == DATATYPE::TPBEAM || dtype == DATATYPE::TPDAMPER || dtype == DATATYPE::TPISO);
    }

    bool MemberData::isClosedMember(bool includingLoad) const
    {
        DATATYPE dtype = myMemberValues.dataType;
        if (includingLoad && dtype == DATATYPE::TPSLABLOAD)
            return true;
        return (dtype == DATATYPE::TPSLAB || dtype == DATATYPE::TPWALL || dtype == DATATYPE::TPUWALL);
    }

    bool MemberData::isJointMember() const
    {
        return (myMemberValues.dataType == DATATYPE::TPJOINTLOAD);
    }

    bool MemberData::isAnalysisLineMember() const
    {
        DATATYPE dtype = myMemberValues.dataType;
        return (dtype == DATATYPE::TPCOLUMN || dtype == DATATYPE::TPGIRDER || dtype == DATATYPE::TPBRACE || dtype == DATATYPE::TPISO || dtype == DATATYPE::TPDAMPER);
    }

    bool MemberData::isAnalysisClosedMember(const UnifiedData &unified) const
    {
        if (myMemberValues.dataType == DATATYPE::TPUWALL)
            return true;

        if (myMemberValues.dataType == DATATYPE::TPWALL)
        {
            return unified.isStructualSection(DATATYPE::TPWALL, myMemberValues.sectionName,
                                              getAttachedHeight(unified));
        }
        else if (myMemberValues.dataType == DATATYPE::TPSLAB)
        {
            return unified.isStructualSection(DATATYPE::TPSLAB, myMemberValues.sectionName);
        }
        return false;
    }

    bool MemberData::isAnalysisMember(const UnifiedData &unified) const
    {
        DATATYPE dtype = myMemberValues.dataType;
        if (dtype == DATATYPE::TPCOLUMN || dtype == DATATYPE::TPGIRDER || dtype == DATATYPE::TPBRACE || dtype == DATATYPE::TPISO || dtype == DATATYPE::TPDAMPER)
        {
            return true;
        }
        else if (dtype == DATATYPE::TPWALL)
        {
            return unified.isStructualSection(DATATYPE::TPWALL, myMemberValues.sectionName,
                                              getAttachedHeight(unified));
        }
        else if (dtype == DATATYPE::TPSLAB)
        {
            return unified.isStructualSection(DATATYPE::TPSLAB, myMemberValues.sectionName);
        }
        return false;
    }

    bool MemberData::isGirderType() const
    {
        DATATYPE dtype = myMemberValues.dataType;
        return (dtype == DATATYPE::TPGIRDER || dtype == DATATYPE::TPBRACE || dtype == DATATYPE::TPBEAM);
    }

    void MemberData::setSectionName(const UnifiedData &unified, const QString &name)
    {
        myMemberValues.sectionName = name;
        resetLineShape(unified);
    }

    void MemberData::setWallOpen(const QString &name)
    {
        myMemberValues.wallOpenName = name;
    }
    void MemberData::setFaceBaneName(const QString &name_iy, const QString &name_iz, const QString &name_jy, const QString &name_jz)
    {
        myMemberValues.iyBaneName = name_iy;
        myMemberValues.izBaneName = name_iz;
        myMemberValues.jyBaneName = name_jy;
        myMemberValues.jzBaneName = name_jz;
    }

    void MemberData::removeOneJoint(JointData *jd)
    {
        int idx = jointList.indexOf(jd);
        if (idx < 0)
            return;

        if (isClosedMember())
        {
            jd->removeComposeMember(this);
            jointList.removeAt(idx);
        }
        else
        {
            if (idx == 0)
            {
                if (jointList.count() > 2)
                    jointList.at(1)->interiorToCompose(this);
                jd->removeComposeMember(this);
                jointList.removeFirst();
            }
            else if (idx == jointList.count() - 1)
            {
                if (jointList.count() > 2)
                    jointList.at(idx - 1)->interiorToCompose(this);
                jd->removeComposeMember(this);
                jointList.removeLast();
            }
            else if (idx > 0)
            {
                jd->removeInteriorMember(this);
                jointList.removeAt(idx);
            }
        }
    }

    void MemberData::replaceJoint(JointData *a_jd, JointData *b_jd, bool recalc)
    {
        if (b_jd == nullptr)
            return;
        if (jointList.contains(a_jd))
        {
            int idx = jointList.indexOf(a_jd);
            if (idx < 0)
                return;
            if (isInteriorJoint(a_jd))
            {
                a_jd->removeInteriorMember(this);
                if (!this->getInteriorJointList().contains(b_jd))
                    b_jd->appendInteriorMember(this);
            }
            else if (isComposeJoint(a_jd))
            {
                a_jd->removeComposeMember(this);
                if (!this->getJointList().contains(b_jd))
                    b_jd->appendComposeMember(this);
            }

            jointList.replace(idx, b_jd);

            if (recalc)
            {
                recalcLineLength();
            }
        }
    }

    void MemberData::replaceJointList(const QList<JointData *> &new_list)
    {
        if (isClosedMember())
        {
            Q_FOREACH (JointData *jd, jointList)
                jd->removeComposeMember(this);
        }
        else if (!jointList.isEmpty())
        {
            jointList.first()->removeComposeMember(this);
            jointList.last()->removeComposeMember(this);
            if (jointList.count() > 2)
            {
                for (int i = 1; i < jointList.count() - 1; i++)
                {
                    jointList.at(i)->removeInteriorMember(this);
                }
            }
        }
        jointList = new_list;
        if (isClosedMember())
        {
            Q_FOREACH (JointData *jd, jointList)
                jd->appendComposeMember(this);
        }
        else
        {
            jointList.first()->appendComposeMember(this);
            jointList.last()->appendComposeMember(this);
            if (jointList.count() > 2)
            {
                for (int i = 1; i < jointList.count() - 1; i++)
                {
                    jointList.at(i)->appendInteriorMember(this);
                }
            }
        }
        recalcLineLength();
    }

    void MemberData::reverseJointIndex()
    {
        QList<JointData *> tmplist;
        Q_FOREACH (JointData *jd, jointList)
            tmplist.prepend(jd);
        jointList = tmplist;
        recalcLineLength();
    }

    void MemberData::changeJointList(const QList<JointData *> &jdlist)
    {
        for (int i = 0; i < jointList.count(); i++)
        {
            if (i == 0 || i == jointList.count() - 1)
            {
                jointList.at(i)->removeComposeMember(this);
            }
            else
            {
                jointList.at(i)->removeInteriorMember(this);
            }
        }
        jointList = jdlist;
        for (int i = 0; i < jointList.count(); i++)
        {
            if (i == 0 || i == jointList.count() - 1)
            {
                jointList.at(i)->appendComposeMember(this);
            }
            else
            {
                jointList.at(i)->appendInteriorMember(this);
            }
        }
        recalcLineLength();
    }

    void MemberData::insertComposeJoint(int idx, JointData *jd)
    {
        if (!isClosedMember())
            return;
        jd->appendComposeMember(this);
        jointList.insert(idx, jd);
    }

    void MemberData::insertInteriorJoint(int idx, JointData *jd)
    {
        if (isClosedMember())
            return;
        if (idx == 0 || idx > jointList.count() - 1)
            return;
        jd->appendInteriorMember(this);
        jointList.insert(idx, jd);
    }

    int MemberData::indexOfOnLineJoint(const JointData *jd) const
    {
        VEC3D v0(jd->xPos(), jd->yPos(), jd->zPos());

        bool detected = false;
        int idx;
        for (int i = 0; i < jointList.count(); i++)
        {

            if (i == jointList.count() - 1 && !isClosedMember())
                continue;

            JointData *jd1 = jointList.at(i);
            JointData *jd2 = (i == jointList.count() - 1) ? jointList.first() : jointList.at(i + 1);
            VEC3D v1(jd1->xPos(), jd1->yPos(), jd1->zPos());
            VEC3D v2(jd2->xPos(), jd2->yPos(), jd2->zPos());

            if (CalcVector3D::between(v1, v2, v0))
            {
                detected = true;
                idx = i + 1;
                break;
            }
        }

        return (detected) ? idx : -1;
    }

    QList<JointData *> MemberData::orderOnLineJoint(const QList<JointData *> &jdlist) const
    {
        QList<JointData *> copyed_jdlist = jointList;
        bool error = false;

        Q_FOREACH (JointData *jd, jdlist)
        {

            if (copyed_jdlist.contains(jd))
            {
                error = true;
                break;
            }

            VEC3D v0(jd->xPos(), jd->yPos(), jd->zPos());

            bool detected = false;
            int idx;
            for (int i = 0; i < copyed_jdlist.count(); i++)
            {

                if (i == copyed_jdlist.count() - 1 && !isClosedMember())
                    continue;

                JointData *jd1 = copyed_jdlist.at(i);
                JointData *jd2 = (i == copyed_jdlist.count() - 1) ? copyed_jdlist.first() : copyed_jdlist.at(i + 1);
                VEC3D v1(jd1->xPos(), jd1->yPos(), jd1->zPos());
                VEC3D v2(jd2->xPos(), jd2->yPos(), jd2->zPos());

                if (CalcVector3D::between(v1, v2, v0))
                {
                    detected = true;
                    idx = i + 1;
                    break;
                }
            }

            copyed_jdlist.insert(idx, jd);

            if (!detected)
            {
                error = true;
                break;
            }
        }

        return (error) ? QList<JointData *>() : copyed_jdlist;
    }

    /* 3次元上の点を部材軸上に投影し、部材長さ系の長さを返す 単位:m系 */
    qreal MemberData::perpLengthOfMember(const XYZ &gp) const
    {
        VEC3D v0(gp.x, gp.y, gp.z);

        qreal min_len = 1.0e+5;
        qreal xx = 0.0;

        for (int i = 0; i < jointList.count() - 1; i++)
        {
            JointData *jd1 = jointList.at(i);
            JointData *jd2 = jointList.at(i + 1);
            VEC3D v1(jd1->xPos(), jd1->yPos(), jd1->zPos());
            VEC3D v2(jd2->xPos(), jd2->yPos(), jd2->zPos());

            VEC3D vn = CalcVector3D::perpPointToSegment(v1, v2, v0);
            qreal l1 = CalcVector3D::norm(VEC3D(vn.x - v0.x, vn.y - v0.y, vn.z - v0.z));
            if (l1 < min_len)
            {
                min_len = l1;
                qreal l2 = CalcVector3D::norm(VEC3D(vn.x - v1.x, vn.y - v1.y, vn.z - v1.z));
                xx = lenList.at(i) / 1000.0 + l2;
            }
        }

        return xx;
    }

    void MemberData::appendIJoint(JointData *jd)
    {
        if (isClosedMember())
            return;
        jd->appendComposeMember(this);
        jointList.first()->composeToInterior(this);
        jointList.prepend(jd);
    }

    void MemberData::appendJJoint(JointData *jd)
    {
        if (isClosedMember())
            return;
        jd->appendComposeMember(this);
        jointList.last()->composeToInterior(this);
        jointList.append(jd);
    }

    JointData *GeneralMemberData::iJoint() const
    {
        return jointList.first();
    }

    JointData *GeneralMemberData::jJoint() const
    {
        return jointList.last();
    }

    bool GeneralMemberData::isIJoint(const JointData *jd) const
    {
        return (jd == jointList.first());
    }

    bool GeneralMemberData::isJJoint(const JointData *jd) const
    {
        return (jd == jointList.last());
    }

    JointData *MemberData::jointAt(int idx) const
    {
        if (idx < 0 || idx > jointList.count() - 1)
            return nullptr;
        return jointList.at(idx);
    }

    int MemberData::indexOfMemberJoint(const JointData *jd) const
    {
        return jointList.indexOf(const_cast<JointData *>(jd));
    }

    JointData *MemberData::previousJoint(const JointData *jd) const
    {
        int idx = jointList.indexOf(const_cast<JointData *>(jd));
        if (idx < 0 || isJointMember())
            return nullptr;
        if (isClosedMember())
        {
            return (idx == 0) ? jointList.last() : jointList.at(idx - 1);
        }
        else
        {
            return (idx == 0) ? nullptr : jointList.at(idx - 1);
        }
    }

    JointData *MemberData::nextJoint(const JointData *jd) const
    {
        int idx = jointList.indexOf(const_cast<JointData *>(jd));
        if (idx < 0 || isJointMember())
            return NULL;
        if (isClosedMember())
        {
            return (idx == jointList.count() - 1) ? jointList.first() : jointList.at(idx + 1);
        }
        else
        {
            return (idx == jointList.count() - 1) ? NULL : jointList.at(idx + 1);
        }
    }

    QList<JointData *> MemberData::aroundJoints(const JointData *jd) const
    {
        int idx = jointList.indexOf(const_cast<JointData *>(jd));
        if (idx < 0 || isJointMember())
            return QList<JointData *>();
        if (isClosedMember())
        {
            return (QList<JointData *>() << previousJoint(jd) << nextJoint(jd));
        }
        else
        {
            QList<JointData *> jdlist;
            JointData *jd1 = previousJoint(jd);
            JointData *jd2 = nextJoint(jd);
            if (jd1 != NULL)
                jdlist.append(jd1);
            if (jd2 != NULL)
                jdlist.append(jd2);
            return jdlist;
        }
    }

    QList<JointData *> MemberData::aroundAnalysisJoints(const UnifiedData &unified, const JointData *jd) const
    {
        if (!isAnalysisMember(unified))
            return QList<JointData *>();

        QList<JointData *> jdlist = getAnalysisJointList(unified);
        int idx = jdlist.indexOf(const_cast<JointData *>(jd));
        if (idx < 0 || jdlist.count() < 2)
            return QList<JointData *>();

        if (isClosedMember())
        {
            if (jdlist.count() < 3)
                return QList<JointData *>();
            if (idx == 0)
            {
                return QList<JointData *>() << jdlist.last() << jdlist.at(1);
            }
            else if (idx == jdlist.count() - 1)
            {
                return QList<JointData *>() << jdlist.at(idx - 1) << jdlist.first();
            }
        }
        else
        {
            if (idx == 0)
            {
                return QList<JointData *>() << jdlist.at(1);
            }
            else if (idx == jdlist.count() - 1)
            {
                return QList<JointData *>() << jdlist.at(idx - 1);
            }
        }

        return QList<JointData *>() << jdlist.at(idx - 1) << jdlist.at(idx + 1);
    }

    bool MemberData::isCriticalJoint(const JointData *jd) const
    {
        if (isJointMember())
        {
            return jointList.contains(const_cast<JointData *>(jd));
        }
        else if (isClosedMember())
        {
            return (jointList.contains(const_cast<JointData *>(jd)) && jointList.count() < 4);
        }
        else
        {
            return (jointList.contains(const_cast<JointData *>(jd)) && jointList.count() < 3);
        }
    }

    bool MemberData::isComposeJoint(const JointData *jd) const
    {
        if (isLineMember())
            return (jd == jointList.first() || jd == jointList.last());
        return true;
    }

    bool MemberData::isCornerJoint(const JointData *jd) const
    {
        if (isJointMember())
            return true;
        if (isLineMember())
        {
            if (jd == jointList.first() || jd == jointList.last())
                return true;
        }

        JointData *ij = previousJoint(jd);
        JointData *jj = nextJoint(jd);
        VEC3D v1(ij->xPos() - jd->xPos(), ij->yPos() - jd->yPos(), ij->zPos() - jd->zPos());
        v1 = CalcVector3D::normalize(v1);
        VEC3D v2(jj->xPos() - jd->xPos(), jj->yPos() - jd->yPos(), jj->zPos() - jd->zPos());
        v2 = CalcVector3D::normalize(v2);
        return (qAbs(CalcVector3D::dot(v1, v2)) < 0.99985); // 1°以上の角度差
    }

    XYZ GeneralMemberData::iXYZ() const
    {
        return jointList.first()->xyz();
    }

    XYZ GeneralMemberData::jXYZ() const
    {
        return jointList.last()->xyz();
    }

    bool GeneralMemberData::containsFloor(const QUuid &id) const
    {
        Q_FOREACH (JointData *jd, jointList)
        {
            if (!jd->containsFloor(id))
                return false;
        }
        return true;
    }

    bool GeneralMemberData::containsFloors(const QList<QUuid> &idlist) const
    {
        Q_FOREACH (JointData *jd, jointList)
        {
            if (!jd->containsFloor(idlist))
                return false;
        }
        return true;
    }

    bool GeneralMemberData::containsFrame(const QUuid &id) const
    {
        Q_FOREACH (JointData *jd, jointList)
        {
            if (!jd->containsFrame(id))
                return false;
        }
        return true;
    }

    bool GeneralMemberData::containsFrames(const QList<QUuid> &idlist) const
    {
        Q_FOREACH (JointData *jd, jointList)
        {
            if (!jd->containsFrame(idlist))
                return false;
        }
        return true;
    }

    bool GeneralMemberData::touchFloor(const QUuid &id) const
    {
        Q_FOREACH (JointData *jd, jointList)
        {
            if (jd->containsFloor(id))
                return true;
        }
        return false;
    }

    bool GeneralMemberData::touchFloors(const QList<QUuid> &idlist) const
    {
        Q_FOREACH (JointData *jd, jointList)
        {
            if (jd->containsFloor(idlist))
                return true;
        }
        return false;
    }

    bool GeneralMemberData::touchFrame(const QUuid &id) const
    {
        Q_FOREACH (JointData *jd, jointList)
        {
            if (jd->containsFrame(id))
                return true;
        }
        return false;
    }

    bool GeneralMemberData::touchFrames(const QList<QUuid> &idlist) const
    {
        Q_FOREACH (JointData *jd, jointList)
        {
            if (jd->containsFrame(idlist))
                return true;
        }
        return false;
    }
    QUuid GeneralMemberData::getAttachedFloor() const
    {
        return jointList.first()->getFirstFloor();
        /*
            QList<QUuid> fl_idlist;
            if( myMemberValues.dataType==TPJOINTLOAD ) return jointList.first()->getAttachedFloorList();
            Q_FOREACH( QUuid id, jointList.last()->getAttachedFloorList() ){
                if( containsFloor(id) ) fl_idlist.append(id);
            }
            return fl_idlist;
        */
    }

    QList<QUuid> MemberData::getAttachedFrameList() const
    {
        QList<QUuid> fr_idlist;
        if (myMemberValues.dataType == DATATYPE::TPJOINTLOAD)
            return jointList.first()->getAttachedFrameList();
        Q_FOREACH (QUuid id, jointList.last()->getAttachedFrameList())
        {
            if (containsFrame(id))
                fr_idlist.append(id);
        }
        return fr_idlist;
    }

    bool MemberData::isInteriorJoint(JointData *const jd) const
    {
        if (isClosedMember())
            return false;
        return (jointList.first() != jd && jointList.last() != jd && jointList.contains(jd));
    }

    QList<JointData *> MemberData::getInteriorJointList() const
    {
        if (isClosedMember() || jointList.count() < 3)
            return QList<JointData *>();
        return jointList.mid(1, jointList.count() - 2);
    }

    QList<JointData *> MemberData::getAnalysisJointList(const UnifiedData &unified) const
    {
        QList<JointData *> jdlist;

        if (isLineMember())
        {
            jdlist.append(jointList.first());
            for (int i = 1; i < jointList.count() - 1; i++)
            {
                JointData *jd = jointList.at(i);
                if (jd->isAnalysisJoint(unified))
                    jdlist.append(jd);
            }
            jdlist.append(jointList.last());
        }
        else if (isClosedMember())
        {
            for (int i = 0; i < jointList.count(); i++)
            {
                JointData *jd = jointList.at(i);
                if (jd->isAnalysisJoint(unified))
                    jdlist.append(jd);
            }
        }
        return jdlist;
    }

    QList<MemberData *> MemberData::getRelatedMembers() const
    {
        QList<MemberData *> related_mdlist;
        Q_FOREACH (JointData *jd, jointList)
        {
            QList<MemberData *> mdlist = jd->getRelatedMembers();
            Q_FOREACH (MemberData *md, mdlist)
            {
                if (!related_mdlist.contains(md))
                    related_mdlist.append(md);
            }
        }
        return related_mdlist;
    }

    QList<MemberData *> MemberData::getComposeMembers() const
    {
        if (!isLineMember())
            return QList<MemberData *>();
        QList<MemberData *> compose_mdlist;
        Q_FOREACH (MemberData *md, iJoint()->getComposeMembers())
        {
            if (!compose_mdlist.contains(md))
                compose_mdlist.append(md);
        }
        Q_FOREACH (MemberData *md, jJoint()->getComposeMembers())
        {
            if (!compose_mdlist.contains(md))
                compose_mdlist.append(md);
        }
        return compose_mdlist;
    }
    QList<MemberData *> MemberData::getLineMembersAroundPlane() const
    {

        qDebug() << "getLineMembersAroundPlane";
        if (isLineMember())
            return QList<MemberData *>();
        //    SectionValues sv;
        //    sv= FactoryUnifiedData::getInstance(isOutput)->getSectionValues(TPWALL,sectionName());
        //    qDebug()<<"sv"<<sv.plane_param.planeType;
        //    if(sv.plane_param.planeType!=PlaneType::WALL)return QList<MemberData*>();

        if (getPlaneModel() != PLANEMODEL::WALL_MODEL)
            return QList<MemberData *>();
        QList<MemberData *> around_mdlist;
        Q_FOREACH (JointData *jd, jointList)
        {
            QList<MemberData *> mdlist = jd->getRelatedMembers();
            Q_FOREACH (MemberData *md, mdlist)
            {
                if (!around_mdlist.contains(md) && md->isLineMember() && jointList.contains(md->iJoint()) && jointList.contains(md->jJoint()))
                {
                    around_mdlist.append(md);
                    qDebug() << "append getLineMembersAroundPlane";
                }
            }
        }
        return around_mdlist;
    }
    void MemberData::setFaceValues(bool ir_chg, int i_id, bool il_chg, qreal i_value,
                                   bool jr_chg, int j_id, bool jl_chg, qreal j_value)
    {
        qDebug() << "MemberData::setFaceValues in";
        if (ir_chg)
        {
            qDebug() << "MemberData::setFaceValues a";
            myMemberValues.iFaceRule = (i_id == 0) ? FACERULE::FACE_AUTO : FACERULE::FACE_VALUE;
            qDebug() << "MemberData::setFaceValues b";
        }
        if (il_chg)
        {
            myMemberValues.iFace = i_value;
        }
        if (jr_chg)
        {
            myMemberValues.jFaceRule = (j_id == 0) ? FACERULE::FACE_AUTO : FACERULE::FACE_VALUE;
        }
        if (jl_chg)
        {
            myMemberValues.jFace = j_value;
        }
        qDebug() << "MemberData::setFaceValues recalc";
        recalcLineLength();
        qDebug() << "MemberData::setFaceValues out";
    }

    void MemberData::setFaceValues(FACERULE irule, qreal ival, FACERULE jrule, qreal jval)
    {
        myMemberValues.iFaceRule = irule;
        myMemberValues.jFaceRule = jrule;
        myMemberValues.iFace = ival;
        myMemberValues.jFace = jval;
    }

    void MemberData::setPartingValues(PARTINGLEN plen, PARTINGRULE prule,
                                      qreal ilen, qreal jlen, qreal iratio, qreal jratio)
    {
        myMemberValues.partingLength = plen;
        myMemberValues.partingRule = prule;
        myMemberValues.iLength = ilen;
        myMemberValues.jLength = jlen;
        myMemberValues.iRatio = iratio;
        myMemberValues.jRatio = jratio;
    }

    void MemberData::resetLineShape(const UnifiedData &unified)
    {
        if (isLineMember(false))
        {

            qreal hh = getAttachedHeight(unified);

            LINESHAPE_IN lshape = unified.sectionNameToLineShape(myMemberValues.dataType,
                                                                 myMemberValues.sectionName, hh);
            myLineShape = HLINESHAPE(lshape.sectionName, lshape.renderType,
                                     lshape.leftSizeB, lshape.leftSizeH,
                                     lshape.centerSizeB, lshape.centerSizeH,
                                     lshape.rightSizeB, lshape.rightSizeH);

            myMemberValues.partingLength = lshape.partingLength;
            myMemberValues.partingRule = lshape.partingRule;
            myMemberValues.iLength = lshape.iLength;
            myMemberValues.jLength = (lshape.jLength > -1.0e-5) ? lshape.jLength : lenList.last() - lshape.iLength;
            myMemberValues.iRatio = lshape.iRatio;
            myMemberValues.jRatio = (lshape.jRatio > -1.0e-5) ? lshape.jRatio : 1.0 - lshape.iRatio;

            if (myMemberValues.dataType == DATATYPE::TPCOLUMN && !lshape.baseName.isEmpty())
            {
                BASEVALUES base = unified.getBaseValues(lshape.baseName);
                myLineShape.baseName = base.sectionName;
                myLineShape.baseSizeB = base.sizeB;
                myLineShape.baseSizeD = base.sizeD;
                myLineShape.baseSizeT = base.sizeT;
            }

            myLineShape.angle = myMemberValues.codeAngle;
            myLineShape.shiftY = myMemberValues.shiftY;
            myLineShape.shiftZ = myMemberValues.shiftZ;

            myLineShape.leftSizeB_T = lshape.leftSizeB_T;
            myLineShape.leftSizeH_T = lshape.leftSizeH_T;
            myLineShape.centerSizeB_T = lshape.centerSizeB_T;
            myLineShape.centerSizeH_T = lshape.centerSizeH_T;
            myLineShape.rightSizeB_T = lshape.rightSizeB_T;
            myLineShape.rightSizeH_T = lshape.rightSizeH_T;
        }
        else if (isClosedMember(false))
        {

            qreal hh = getAttachedHeight(unified);

            myPlaneValue = unified.sectionNameToPlaneProperty(myMemberValues.dataType,
                                                              myMemberValues.sectionName, hh);
        }
        /*
            qDebug() << "resetLineShape" << myMemberValues.sectionName;
            qDebug() << myMemberValues.partingLength << myMemberValues.partingRule;
            qDebug() << myMemberValues.iLength << myMemberValues.jLength;
            qDebug() << myMemberValues.iRatio << myMemberValues.jRatio;
        */
    }

    void MemberData::recalcLineLength()
    {
        //    qDebug() << "recalcLineLength";
        recalcJointLength();
        recalcFaceLength();
        recalcPartition();
    }

    void MemberData::recalcJointLength()
    {
        lenList.clear();
        qreal sumlen = 0.0;
        lenList.append(sumlen);

        for (int i = 0; i < jointList.count() - 1; i++)
        {

            JointData *jd0 = jointList.at(i);
            JointData *jd1 = jointList.at(i + 1);

            sumlen += qSqrt(qPow((jd1->xPos() - jd0->xPos()), 2.0) + qPow((jd1->yPos() - jd0->yPos()), 2.0) + qPow((jd1->zPos() - jd0->zPos()), 2.0)) * 1000.0;
            lenList.append(sumlen);
        }
    }

    void MemberData::recalcFaceLength()
    {
        if (!isGirderType())
        {
            if (myMemberValues.iFaceRule == FACERULE::FACE_AUTO)
                myMemberValues.iFace = 0.0;
            if (myMemberValues.jFaceRule == FACERULE::FACE_AUTO)
                myMemberValues.jFace = 0.0;
            return;
        }

        qreal eps = 1.0e-3;

        JointData *jd0 = iJoint();
        qreal ag0 = qAtan2(jointList.at(1)->yPos() - jd0->yPos(),
                           jointList.at(1)->xPos() - jd0->xPos());

        /* 楕円形からフェイス長さ算出 */
        /* A=cosT/a   B=cosT/b  a:X軸半径、b:Y軸半径、T:角度 */
        /* L^2=1/(A^2+B^2) */
        qreal maxfi = 0.0;
        Q_FOREACH (MemberData *md, jd0->getRelatedMembers())
        {
            if (md->dataType() == DATATYPE::TPCOLUMN)
            {
                qreal angle = ag0 - md->codeAngle();
                MEMBERSIZE size = md->getSizeOfJoint(jd0);
                if (size.sizeB < eps || size.sizeH < eps)
                    continue;
                qreal aa = 2.0 * qCos(angle) / size.sizeB;
                qreal bb = 2.0 * qSin(angle) / size.sizeH;
                maxfi = qMax(maxfi, 1.0 / qSqrt(aa * aa + bb * bb));
            }
            else if (md->dataType() == DATATYPE::TPCOLUMN)
            {
                qreal angle = ag0 - md->codeAngle();
                MEMBERSIZE size = md->getSizeOfJoint(jd0);
                if (size.sizeB < eps || size.sizeH < eps)
                    continue;
                qreal aa = 2.0 * qCos(angle) / size.sizeB;
                qreal bb = 2.0 * qSin(angle) / size.sizeH;
                maxfi = qMax(maxfi, 1.0 / qSqrt(aa * aa + bb * bb));
            }
        }
        // 梁に取り付く梁のフェイス長さ、中央断面の梁幅の1/2
        Q_FOREACH (MemberData *md, jd0->getInteriorMembers())
        {
            if (md->isGirderType())
                maxfi = qMax(maxfi, md->getCenterWidth() / 2.0);
        }
        if (myMemberValues.iFaceRule == FACERULE::FACE_AUTO)
            myMemberValues.iFace = maxfi;

        JointData *jd1 = jJoint();
        qreal ag1 = qAtan2(jointList.at(jointCount() - 2)->yPos() - jd1->yPos(),
                           jointList.at(jointCount() - 2)->xPos() - jd1->xPos());
        qreal maxfj = 0.0;
        Q_FOREACH (MemberData *md, jd1->getRelatedMembers())
        {
            if (md->dataType() == DATATYPE::TPCOLUMN)
            {
                qreal angle = ag1 - md->codeAngle();
                MEMBERSIZE size = md->getSizeOfJoint(jd1);
                if (size.sizeB < eps || size.sizeH < eps)
                    continue;
                qreal aa = 2.0 * qCos(angle) / size.sizeB;
                qreal bb = 2.0 * qSin(angle) / size.sizeH;
                maxfj = qMax(maxfj, 1.0 / qSqrt(aa * aa + bb * bb));
            }
        }
        // 梁に取り付く梁のフェイス長さ、中央断面の梁幅の1/2
        Q_FOREACH (MemberData *md, jd1->getInteriorMembers())
        {
            if (md->isGirderType())
                maxfj = qMax(maxfj, md->getCenterWidth() / 2.0);
        }
        if (myMemberValues.jFaceRule == FACERULE::FACE_AUTO)
            myMemberValues.jFace = maxfj;
    }

    void MemberData::recalcPartition()
    {
        if (!isLineMember())
            return;

        qreal fl_i = myMemberValues.iFace;
        qreal fl_j = myMemberValues.jFace;

        qreal len_i, len_j, len_fi, len_fj;
        qreal sumlen = lenList.last();
        len_fi = fl_i;
        len_fj = sumlen - fl_j;
        if (myMemberValues.partingLength == PARTINGLEN::PART_FULL)
        {
            if (myMemberValues.partingRule == PARTINGRULE::PART_VALUE)
            {
                len_i = myMemberValues.iLength;
                len_j = sumlen - myMemberValues.jLength;
            }
            else if (myMemberValues.partingRule == PARTINGRULE::PART_RATIO)
            {
                len_i = sumlen * myMemberValues.iRatio;
                len_j = sumlen - sumlen * myMemberValues.jRatio;
            }
        }
        else if (myMemberValues.partingLength == PARTINGLEN::PART_INTERNAL)
        {
            qreal in_len = sumlen - fl_i - fl_j;
            if (myMemberValues.partingRule == PARTINGRULE::PART_VALUE)
            {
                len_i = myMemberValues.iLength + fl_i;
                len_j = sumlen - myMemberValues.jLength - fl_j;
            }
            else if (myMemberValues.partingRule == PARTINGRULE::PART_RATIO)
            {
                len_i = in_len * myMemberValues.iRatio + fl_i;
                len_j = sumlen - in_len * myMemberValues.jRatio - fl_j;
            }
        }

        if (len_i > len_j)
        {
            qreal h_len = (len_i + len_j) / 2.0;
            len_i = h_len;
            len_j = h_len;
        }

        if (len_fi > len_i)
            len_i = len_fi;
        if (len_fj < len_j)
            len_j = len_fj;

        myPartition = MEMBERPARTITION(len_fi, len_fj, len_i, len_j, sumlen);
    }

    qreal MemberData::getJointLength(int idx) const
    {
        if (idx < 0 || lenList.count() - 1 < idx)
            return -1.0;
        return lenList.at(idx);
    }

    qreal MemberData::getJointLength(JointData *jd) const
    {
        int idx = jointList.indexOf(jd);
        return (idx < 0) ? -1.0 : lenList.at(idx);
    }

    qreal MemberData::getTotalLength() const
    {
        return lenList.last();
    }

    qreal MemberData::getInternalLength() const
    {
        return myPartition.lenFj - myPartition.lenFi;
    }

    HLINESHAPE MemberData::getHLineShape() const
    {
        return myLineShape;
    }

    MEMBERPARTITION MemberData::getMemberPartition() const
    {
        return myPartition;
    }

    QList<qreal> MemberData::getPartitionLenList() const
    {
        QList<qreal> lenlist;
        lenlist << 0.0 << myPartition.lenFi << myPartition.lenI
                << myPartition.lenJ << myPartition.lenFj << myPartition.lenSum;
        return lenlist;
    }

    VLINESHAPE MemberData::getVLineShape(qreal len) const
    {
        qreal eps = 1.0e-3;
        if (len < myPartition.lenFi - eps || len > myPartition.lenFj + eps)
        {
            return VLINESHAPE();
        }
        else if (len < myPartition.lenFi + eps)
        {
            return VLINESHAPE(myLineShape.sectionName,
                              myLineShape.renderType,
                              myLineShape.leftSizeB, myLineShape.leftSizeH,
                              myLineShape.angle, myLineShape.shiftY, myLineShape.shiftZ,
                              myLineShape.baseName, myLineShape.baseSizeB,
                              myLineShape.baseSizeD, myLineShape.leftSizeB_T, myLineShape.leftSizeH_T);
        }
        else if (len < myPartition.lenI + eps)
        {
            return VLINESHAPE(myLineShape.sectionName,
                              myLineShape.renderType,
                              myLineShape.leftSizeB, myLineShape.leftSizeH,
                              myLineShape.angle, myLineShape.shiftY, myLineShape.shiftZ,
                              myLineShape.baseName, myLineShape.baseSizeB,
                              myLineShape.baseSizeD, myLineShape.leftSizeB_T, myLineShape.leftSizeH_T);
        }
        else if (len < myPartition.lenJ)
        {
            return VLINESHAPE(myLineShape.sectionName,
                              myLineShape.renderType,
                              myLineShape.centerSizeB, myLineShape.centerSizeH,
                              myLineShape.angle, myLineShape.shiftY, myLineShape.shiftZ,
                              myLineShape.baseName, myLineShape.baseSizeB,
                              myLineShape.baseSizeD, myLineShape.centerSizeB_T, myLineShape.centerSizeH_T);
        }
        else
        {
            return VLINESHAPE(myLineShape.sectionName,
                              myLineShape.renderType,
                              myLineShape.rightSizeB, myLineShape.rightSizeH,
                              myLineShape.angle, myLineShape.shiftY, myLineShape.shiftZ,
                              myLineShape.baseName, myLineShape.baseSizeB,
                              myLineShape.baseSizeD, myLineShape.rightSizeB_T, myLineShape.rightSizeH_T);
        }
    }

    MEMBERSIZE MemberData::getSizeOfJoint(JointData *jd) const
    {
        int idx = jointList.indexOf(jd);
        if (idx < 0)
            return MEMBERSIZE();
        qreal len = getJointLength(idx);
        qreal eps = 1.0e-5;

        if (len < myPartition.lenFi - eps || len > myPartition.lenFj + eps)
        {
            return MEMBERSIZE();
        }
        else if (len < myPartition.lenI)
        {
            if (myLineShape.renderType == RENDERSHAPE::SHAPE_I || myLineShape.renderType == RENDERSHAPE::SHAPE_T2)
            {
                return MEMBERSIZE(myLineShape.leftSizeH, myLineShape.leftSizeB);
            }
            else if (myLineShape.renderType == RENDERSHAPE::SHAPE_CROSSH)
            {
                return MEMBERSIZE(myLineShape.leftSizeH_T, myLineShape.leftSizeH);
            }
            else
            {
                return MEMBERSIZE(myLineShape.leftSizeB, myLineShape.leftSizeH);
            }
        }
        else if (len > myPartition.lenJ)
        {
            if (myLineShape.renderType == RENDERSHAPE::SHAPE_I || myLineShape.renderType == RENDERSHAPE::SHAPE_T2)
            {
                return MEMBERSIZE(myLineShape.rightSizeH, myLineShape.rightSizeB);
            }
            else if (myLineShape.renderType == RENDERSHAPE::SHAPE_CROSSH)
            {
                return MEMBERSIZE(myLineShape.leftSizeH_T, myLineShape.leftSizeH);
            }
            else
            {
                return MEMBERSIZE(myLineShape.rightSizeB, myLineShape.rightSizeH);
            }
        }
        else
        {
            qreal BB = myLineShape.centerSizeB;
            qreal HH = myLineShape.centerSizeH;
            if (len < myPartition.lenI + eps)
            {
                if (myLineShape.leftSizeB > BB)
                    BB = myLineShape.leftSizeB;
                if (myLineShape.leftSizeH > HH)
                    HH = myLineShape.leftSizeH;
            }
            if (len > myPartition.lenJ - eps)
            {
                if (myLineShape.rightSizeB > BB)
                    BB = myLineShape.rightSizeB;
                if (myLineShape.rightSizeH > HH)
                    HH = myLineShape.rightSizeH;
            }
            if (myLineShape.renderType == RENDERSHAPE::SHAPE_I || myLineShape.renderType == RENDERSHAPE::SHAPE_T2)
            {
                return MEMBERSIZE(HH, BB);
            }
            else
            {
                return MEMBERSIZE(BB, HH);
            }
        }
    }

    LINEPROPERTY MemberData::getLineProperty(const UnifiedData &unified) const
    {
        if (!isLineMember(false))
        {
            qDebug() << "not a line member";
            return LINEPROPERTY();
        }
        if (this->sectionName().isEmpty())
        {
            qDebug() << "section name is Empty.";
            return LINEPROPERTY();
        }

        qreal hh = getAttachedHeight(unified);

        qDebug() << "MemberData::getLineProperty::sectionName" << (int)myMemberValues.dataType << myMemberValues.sectionName;
        return unified.sectionNameToLineProperty(myMemberValues.dataType,
                                                 myMemberValues.sectionName, hh);
    }
    PLANEPROPERTY MemberData::getPlaneProperty(const UnifiedData &unified) const
    {
        if (isLineMember(false))
        {
            qDebug() << "not a line member";
            return PLANEPROPERTY();
        }
        if (this->sectionName().isEmpty())
        {
            qDebug() << "section name is Empty.";
            return PLANEPROPERTY();
        }

        qreal hh = getAttachedHeight(unified);

        qDebug() << "MemberData::getLineProperty::sectionName" << (int)myMemberValues.dataType << myMemberValues.sectionName;
        return unified.sectionNameToPlaneProperty(myMemberValues.dataType,
                                                  myMemberValues.sectionName, hh);
    }

    void MemberData::recalcLocalVector()
    {
        if (jointList.count() < 2)
            return;

        JointData *j0 = jointList.at(0);
        JointData *j1 = jointList.at(1);

        VEC3D v0(j0->xPos(), j0->yPos(), j0->zPos());
        VEC3D v1(j1->xPos(), j1->yPos(), j1->zPos());

        if (dataType() == DATATYPE::TPSLAB || dataType() == DATATYPE::TPWALL ||
            dataType() == DATATYPE::TPUWALL || dataType() == DATATYPE::TPSLABLOAD)
        {

            QList<VEC3D> xyzlist;
            Q_FOREACH (JointData *jd, jointList)
                xyzlist.append(VEC3D(jd->xPos(), jd->yPos(), jd->zPos()));

            // 代表平面の法線ベクトル:snorm, 代表平面の原点:op1, 代表平面のu-v:areaUV
            VEC3D snorm = CalcVector3D::polygonNorm(xyzlist);
            VEC3D w_v = CalcVector3D::normalize(snorm);

            CalcVector3D::pointToPlane(w_v, v0, v1); // 第2点v1 は代表平面投影

            VEC3D u_v = VEC3D(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
            U_Vec = CalcVector3D::normalize(u_v);
            V_Vec = CalcVector3D::crossNormalized(w_v, U_Vec);
        }
        else if (dataType() == DATATYPE::TPCOLUMN)
        {

            VEC3D vv(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
            U_Vec = CalcVector3D::normalize(vv);

            VEC3D u_v = VEC3D(0.0, 0.0, 1.0);
            VEC3D v_v = VEC3D(1.0, 0.0, 0.0);

            QList<VEC3D> vlist;
            vlist << v_v;

            VEC3D vn;
            qreal theta;
            qreal angle = codeAngle();
            qreal cos1 = CalcVector3D::dot(u_v, U_Vec);
            if (cos1 < -0.9999)
            {
                theta = -M_PI;
                vn = VEC3D(qCos(angle), qSin(angle), 0.0);
            }
            else if (cos1 > 0.9999)
            {
                theta = 0.0;
                vn = VEC3D(0.0, 0.0, 0.0);
            }
            else
            {
                theta = qAcos(cos1);
                vn = CalcVector3D::crossNormalized(u_v, U_Vec);
            }

            if (qAbs(angle) > 1.0e-5)
                CalcVector3D::rotateToVR(vlist, angle, u_v);
            if (CalcVector3D::norm(vn) > 1.0e-3)
                CalcVector3D::rotateToVR(vlist, theta, vn);

            V_Vec = vlist.first();
        }
        else if (dataType() == DATATYPE::TPGIRDER || dataType() == DATATYPE::TPBRACE ||
                 dataType() == DATATYPE::TPBEAM || dataType() == DATATYPE::TPDAMPER || dataType() == DATATYPE::TPISO)
        {

            VEC3D vv(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
            U_Vec = CalcVector3D::normalize(vv);

            VEC3D a1(v1.x - v0.x, v1.y - v0.y, 0.0);
            a1 = CalcVector3D::normalize(a1);

            VEC3D u_v = VEC3D(1.0, 0.0, 0.0);
            VEC3D v_v = VEC3D(0.0, 1.0, 0.0);

            QList<VEC3D> vlist;
            vlist << v_v;

            qreal theta1, theta2;
            qreal angle = codeAngle();
            VEC3D vn1, vn2;

            if (CalcVector3D::norm(a1) > 1.0e-3)
            {
                qreal cos1 = CalcVector3D::dot(u_v, a1);
                if (cos1 < -0.9999)
                {
                    theta1 = M_PI;
                    vn1 = VEC3D(0.0, 0.0, 1.0);
                }
                else if (cos1 > 0.9999)
                {
                    theta1 = 0.0;
                    vn1 = VEC3D(0.0, 0.0, 0.0);
                }
                else
                {
                    theta1 = qAcos(cos1);
                    vn1 = CalcVector3D::crossNormalized(u_v, a1);
                }
                qreal cos2 = CalcVector3D::dot(a1, U_Vec);
                if (cos2 < -0.9999)
                {
                    theta2 = M_PI;
                    vn2 = VEC3D(-a1.y, a1.x, 0.0);
                }
                else if (cos2 > 0.9999)
                {
                    theta2 = 0.0;
                    vn2 = VEC3D(0.0, 0.0, 0.0);
                }
                else
                {
                    theta2 = qAcos(cos2);
                    vn2 = CalcVector3D::crossNormalized(a1, U_Vec);
                }
            }
            else
            {
                theta1 = 0.0;
                vn1 = VEC3D(0.0, 0.0, 0.0);
                theta2 = M_PI / 2.0;
                vn2 = VEC3D(0.0, -1.0, 0.0);
            }

            CalcVector3D::rotateToVR(vlist, angle, u_v);
            if (CalcVector3D::norm(vn1) > 1.0e-3)
                CalcVector3D::rotateToVR(vlist, theta1, vn1);
            if (CalcVector3D::norm(vn2) > 1.0e-3)
                CalcVector3D::rotateToVR(vlist, theta2, vn2);

            V_Vec = vlist.first();
        }
    }

    VEC3D GeneralMemberData::getUVector() const
    {
        return U_Vec;
    }

    VEC3D GeneralMemberData::getVVector() const
    {
        return V_Vec;
    }

    VEC3D GeneralMemberData::getWVector() const
    {
        return CalcVector3D::cross(U_Vec, V_Vec);
    }

    void MemberData::calcMidLocalVector(int idx, VEC3D &t_UV, VEC3D &t_VV, VEC3D &t_WV)
    {
        if (idx == 0 || idx > jointList.count() - 2 || this->isClosedMember())
        {
            t_UV = U_Vec;
            t_VV = V_Vec;
            t_WV = CalcVector3D::cross(U_Vec, V_Vec);
            return;
        }

        JointData *j0 = jointList.at(idx);
        JointData *j1 = jointList.at(idx + 1);

        VEC3D v0(j0->xPos(), j0->yPos(), j0->zPos());
        VEC3D v1(j1->xPos(), j1->yPos(), j1->zPos());

        if (dataType() == DATATYPE::TPCOLUMN)
        {

            VEC3D vv(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
            t_UV = CalcVector3D::normalize(vv);

            VEC3D u_v = VEC3D(0.0, 0.0, 1.0);
            VEC3D v_v = VEC3D(1.0, 0.0, 0.0);

            QList<VEC3D> vlist;
            vlist << v_v;

            VEC3D vn;
            qreal theta;
            qreal angle = codeAngle();
            qreal cos1 = CalcVector3D::dot(u_v, t_UV);
            if (cos1 < -0.9999)
            {
                theta = -M_PI;
                vn = VEC3D(qCos(angle), qSin(angle), 0.0);
            }
            else if (cos1 > 0.9999)
            {
                theta = 0.0;
                vn = VEC3D(0.0, 0.0, 0.0);
            }
            else
            {
                theta = qAcos(cos1);
                vn = CalcVector3D::crossNormalized(u_v, t_UV);
            }

            if (qAbs(angle) > 1.0e-5)
                CalcVector3D::rotateToVR(vlist, angle, u_v);
            if (CalcVector3D::norm(vn) > 1.0e-3)
                CalcVector3D::rotateToVR(vlist, theta, vn);

            t_VV = vlist.first();
        }
        else if (dataType() == DATATYPE::TPGIRDER || dataType() == DATATYPE::TPBRACE ||
                 dataType() == DATATYPE::TPBEAM || dataType() == DATATYPE::TPDAMPER || dataType() == DATATYPE::TPISO)
        {

            VEC3D vv(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
            t_UV = CalcVector3D::normalize(vv);

            VEC3D a1(v1.x - v0.x, v1.y - v0.y, 0.0);
            a1 = CalcVector3D::normalize(a1);

            VEC3D u_v = VEC3D(1.0, 0.0, 0.0);
            VEC3D v_v = VEC3D(0.0, 1.0, 0.0);

            QList<VEC3D> vlist;
            vlist << v_v;

            qreal theta1, theta2;
            qreal angle = codeAngle();
            VEC3D vn1, vn2;

            if (CalcVector3D::norm(a1) > 1.0e-3)
            {
                qreal cos1 = CalcVector3D::dot(u_v, a1);
                if (cos1 < -0.9999)
                {
                    theta1 = M_PI;
                    vn1 = VEC3D(0.0, 0.0, 1.0);
                }
                else if (cos1 > 0.9999)
                {
                    theta1 = 0.0;
                    vn1 = VEC3D(0.0, 0.0, 0.0);
                }
                else
                {
                    theta1 = qAcos(cos1);
                    vn1 = CalcVector3D::crossNormalized(u_v, a1);
                }
                qreal cos2 = CalcVector3D::dot(a1, t_UV);
                if (cos2 < -0.9999)
                {
                    theta2 = M_PI;
                    vn2 = VEC3D(-a1.y, a1.x, 0.0);
                }
                else if (cos2 > 0.9999)
                {
                    theta2 = 0.0;
                    vn2 = VEC3D(0.0, 0.0, 0.0);
                }
                else
                {
                    theta2 = qAcos(cos2);
                    vn2 = CalcVector3D::crossNormalized(a1, t_UV);
                }
            }
            else
            {
                theta1 = 0.0;
                vn1 = VEC3D(0.0, 0.0, 0.0);
                theta2 = M_PI / 2.0;
                vn2 = VEC3D(0.0, -1.0, 0.0);
            }

            CalcVector3D::rotateToVR(vlist, angle, u_v);
            if (CalcVector3D::norm(vn1) > 1.0e-3)
                CalcVector3D::rotateToVR(vlist, theta1, vn1);
            if (CalcVector3D::norm(vn2) > 1.0e-3)
                CalcVector3D::rotateToVR(vlist, theta2, vn2);

            t_VV = vlist.first();
        }

        t_WV = CalcVector3D::cross(t_UV, t_VV);
        return;
    }

    QList<qreal> MemberData::getSeparatedLength() const
    {
        qreal mband = 0.5; // 部材の分割幅　50cm
        qreal tlen = getTotalLength() / 1000.0;
        int mct = int((tlen + 1.0e-5) / mband);
        qreal medg = (tlen - qreal(mct) * mband) / 2.0;

        QList<qreal> slenlist;
        for (int i = 0; i < mct; i++)
        {
            if (i == 0)
            {
                slenlist.append((mband + medg) / 2.0);
            }
            else if (i == mct - 1)
            {
                slenlist.append(tlen - (mband + medg) / 2.0);
            }
            else
            {
                slenlist.append(mband / 2.0 + medg + qreal(i) * mband);
            }
        }
        return slenlist;
    }

    qreal MemberData::getAttachedHeight(const UnifiedData &unified) const
    {
        //    qreal hh = jointList.first()->zPos();
        qreal hh = 0.0;

        QUuid fid = jointList.first()->getFirstFloor();
        if (!fid.isNull())
            hh = unified.getFloorHeight(fid);

        if (myMemberValues.dataType == DATATYPE::TPCOLUMN || myMemberValues.dataType == DATATYPE::TPGIRDER)
        {
            QUuid fid = jointList.first()->getFirstFloor();
            if (!fid.isNull())
                hh = unified.getFloorHeight(fid);
        }
        else if (myMemberValues.dataType == DATATYPE::TPWALL)
        {
            Q_FOREACH (JointData *jd, jointList)
            {
                QUuid fid = jd->getFirstFloor();
                if (!fid.isNull())
                    hh = qMin(hh, unified.getFloorHeight(fid));
                //            hh = qMin(hh, jd->zPos());
            }
        }
        return hh;
    }
} // namespace post3dapp
