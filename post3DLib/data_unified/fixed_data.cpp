#include "fixed_data.h"

#include <QDateTime>
#include "define_section_struct.h"
#include "unified_inputdata.h"
#include "unified_output.h"
#include "unified_analysisdata.h"
#include "unified_calcload.h"

namespace post3dapp{

FixedData* FixedData::Create(){
    return new FixedData(false, QDateTime::currentDateTime().toString());
}

FixedData* FixedData::CreateDummy(){
    return new FixedData(true, u8"--");
}

FixedData::FixedData(bool _isDummy, const QString& _time)
    : time(_time)
{
    if (_isDummy){
        inputData = UnifiedInputData::Create();
    }else{
        inputData = UnifiedInputData::Copy(*UnifiedInputData::getInInstance());
    }
    analysisData = UnifiedAnalysisData::Create();
    pstnData = UnifiedOutput::Create();
    calcLoadData = UnifiedCalcLoad::Create();
    table = QSharedPointer<N3DUuidTable>(new N3DUuidTable());
}

FixedData* UnifiedFixedData::myInstance = nullptr;

QStringList FixedData::getGirderRcList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPGIRDER)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getGirderSList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPGIRDER)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getGirderSrcList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPGIRDER)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getColumnRcList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPCOLUMN)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getColumnSList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPCOLUMN)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getColumnSrcList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPCOLUMN)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getColumnCftList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPCOLUMN)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getBeamRcList() const{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPBEAM)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getBeamSList() const{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPBEAM)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getBeamSrcList() const{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPBEAM)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getBraceList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPBRACE)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getWallList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPWALL)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getSteelWallList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPWALL)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getFoundationWallList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPUWALL)){
        list.append(sec.sectionName);
    }
    return list;
}

QStringList FixedData::getSlabList() const{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPSLAB)){
        list.append(sec.sectionName);
    }
    return list;
}

QStringList FixedData::getOpenList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPWALLOPEN)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getBaseSList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPBASE)){
        if (sec.sectionType != STRUCTTYPE::STP_S)
            continue;
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getBaseSrcList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPBASE)){
        if (sec.sectionType != STRUCTTYPE::STP_SRC)
            continue;
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getDamperList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPDAMPER)){
        list.append(sec.sectionName);
    }
    return list;
}
QStringList FixedData::getIsoList() const
{
    QStringList list;
    Q_FOREACH(auto sec, getInputData()->getSectionValueList(DATATYPE::TPISO)){
        list.append(QString{sec.sectionName});
    }
    return list;
}
QStringList FixedData::getJointLoadList() const
{
    QStringList list;
    Q_FOREACH(auto load, getInputData()->getJointLoadList()){
        QHashIterator<LoadType::LOADTYPE, JOINTLOADDIM> i(load.loadTable);
        while (i.hasNext()){
            i.next();
            QString second;
            second.append(load.loadName.leftJustified(10));
            second.append(QString::number(i.key()).leftJustified(5));
            second.append(QString::number(i.value().fx).leftJustified(10));
            second.append(QString::number(i.value().fy).leftJustified(10));
            second.append(QString::number(i.value().fz).leftJustified(10));
            second.append(QString::number(i.value().mx).leftJustified(10));
            second.append(QString::number(i.value().my).leftJustified(10));
            second.append(QString::number(i.value().mz).leftJustified(10));
            second.append(i.value().notes.leftJustified(10));
            list.append(second);
        }
    }
    return list;
}
QStringList FixedData::getMemberLoadList() const
{
    QStringList list;
    Q_FOREACH(auto load, getInputData()->getMemberLoadList()){
        QHashIterator<LoadType::LOADTYPE, MEMBERLOADDIM> i(load.loadTable);
        while(i.hasNext()){
            i.next();
            QString second;
            second.append(load.loadName.leftJustified(10));
            second.append(QString::number(i.key()).leftJustified(5));
            if (i.value().corSytem == LCORSYSTEM::LCOR_GLOBAL)
                second.append(QString(u8"全体座標").leftJustified(5));
            else
                second.append(QString(u8"部材座標").leftJustified(5));
            switch(i.value().loadDir){
            case LOADDIR::LDIR_X:
                second.append(QString(u8"X").leftJustified(5));
                break;
            case LOADDIR::LDIR_Y:
                second.append(QString(u8"Y").leftJustified(5));
                break;
            case LOADDIR::LDIR_Z:
                second.append(QString(u8"Z").leftJustified(5));
                break;
            case LOADDIR::LDIR_MX:
                second.append(QString(u8"MX").leftJustified(5));
                break;
            case LOADDIR::LDIR_MY:
                second.append(QString(u8"MY").leftJustified(5));
                break;
            case LOADDIR::LDIR_MZ:
                second.append(QString(u8"MZ").leftJustified(5));
                break;
            }
            second.append(QString::number(i.value().wa).leftJustified(10));
            second.append(QString::number(i.value().wb).leftJustified(10));
            second.append(i.value().notes.leftJustified(30));
            list.append(second);
        }
    }
    return list;
}
QStringList FixedData::getSlabLoadList() const
{
    QStringList list;
    Q_FOREACH(auto load, getInputData()->getSlabLoadList()){
        QString second;
        second.append(load.loadName.leftJustified(10));
        second.append(QString::number(load.sDL).leftJustified(10));
        second.append(QString::number(load.sLL0).leftJustified(10));
        second.append(QString::number(load.sLL1).leftJustified(10));
        second.append(QString::number(load.sLL2).leftJustified(10));
        second.append(QString::number(load.sLL3).leftJustified(10));
        second.append(QString::number(load.sLL4).leftJustified(10));
        second.append(QString::number(load.sLLE).leftJustified(10));
        second.append(load.notes.leftJustified(10));
        list.append(second);
    }
    return list;
}

QString FixedData::getAnalysisJointName(const QUuid &uuid) const
{
    return table->GetN3dJoint(uuid);
}

QString FixedData::getAnalysisMemberName(const QUuid &uuid, int index) const
{
    return table->GetN3dMember(uuid, index);
}


} // namespace post3dapp
