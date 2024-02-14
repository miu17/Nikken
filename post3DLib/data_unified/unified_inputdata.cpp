#include "unified_inputdata.h"

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QProgressDialog>
#include <QStringList>
#include <QUndoStack>

#include "calc_estimate.h"
#include "calc_vector2d.h"
#include "manage_floordata.h"
#include "manage_framedata.h"
#include "manage_jointdata.h"
#include "manage_materialdata.h"
#include "manage_memberdata.h"
#include "manage_loaddata.h"
#include "manage_sectiondata.h"
#include "unified_calcload.h"
#include "unified_output.h"
#include "version.h"

namespace post3dapp{
UnifiedInputData *UnifiedInputData::myInstance = nullptr;


UnifiedInputData::UnifiedInputData()
    :UnifiedData(UnifiedDataType::Input)
{
}

UnifiedInputData* UnifiedInputData::Create(){
    UnifiedInputData* input = new UnifiedInputData();
    connect(input->mySectionData, &ManageSectionData::sectionDeleted, input, &UnifiedInputData::updateDeletedSection);
    connect(input->mySectionData, &ManageSectionData::sectionChanged,
            input, &UnifiedData::updateChangedSection);

    connect(input->myFloorData, &ManageFloorData::floorDeleted,
            input, &UnifiedData::adjustJointAttachment);
    connect(input->myFrameData, &ManageFrameData::frameDeleted,
            input, &UnifiedData::adjustJointAttachment);
    return input;
}

UnifiedInputData* UnifiedInputData::CreateDummy(){
    UnifiedInputData* input = new UnifiedInputData();
    return input;
}


UnifiedInputData* UnifiedInputData::Copy(const UnifiedInputData& input) {
    UnifiedInputData* copy = new UnifiedInputData();
    copy->myFloorData = new ManageFloorData(*input.myFloorData);
    copy->myFrameData = new ManageFrameData(*input.myFrameData, UnifiedDataType::FixedInput);
    copy->myMaterialData = new ManageMaterialData(*input.myMaterialData);
    copy->mySectionData = new ManageSectionData(*input.mySectionData, UnifiedDataType::FixedInput);
    copy->myJointData = new ManageJointData(*input.myJointData, UnifiedDataType::FixedInput);
    copy->myMemberData = new ManageMemberData(*input.myMemberData, *copy->myJointData, *copy);
    copy->myLoadData = new ManageLoadData(*input.myLoadData);
    copy->filename = input.filename;
    return copy;
}

void UnifiedInputData::readUnifiedData( QDataStream &infile, const LibraryVersion &ver)
{
    myJointData->readData(infile, ver);
    myMemberData->readData(infile, ver);

    emit dataReaded();
    emit elementCountChanged();
}

void UnifiedInputData::clearUnifiedData()
{
    emit dataCleared();
    myMemberData->clearData();
    myJointData->clearData();
    emit clearCopyBuffer();
    emit elementCountChanged();
}


void UnifiedInputData::writeP3dData( QDataStream &outfile) const
{
    myJointData->writeP3dData(outfile);
    myMemberData->writeP3dData(outfile);
}
QString UnifiedInputData::writePsvData( QTextStream &outfile) const
{
    try{
        outfile << u8"**JOINT\n";
        myJointData->writePsvData(outfile);
        myMemberData->writePsvData(outfile);
    }
    catch(...){
        return u8"形状データにエラーがあります。";
    }

    return QString{};
}

QString UnifiedInputData::writeNsvData( QTextStream &outfile, QSharedPointer<N3DUuidTable> table)
{
    QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
    outfile.setCodec( codec );
    try{
        outfile << LibraryVersion::Current().text() << "\n";
        outfile << writeListNsvData(table);
        outfile << myJointData->writeNsvData(table);
        outfile << myMemberData->writeNsvData(table);

        outfile << u8"*WVOLUME" << "\n";
        QString text = u8"#階, 柱(m2), 耐力壁X(m2), 耐力壁Y(m2), 雑壁X(m2), 雑壁Y(m2), 床施工面積(m2), コンクリート材種名\n";
        outfile << text;
        foreach(auto f, myFloorData->getFloorList()){
            QString temp;
            temp.append(table->GetOrCreateN3dNumber(f.UUID, DATATYPE::TPFLOOR)).append(",");
            temp.append(u8"100.0, 200.0, 300.0, 400.0, 500.0, Fc21");
            outfile << temp << "\n";
        }
    }
    catch(...){
        return u8"形状データにエラーがあります。";
    }

    return QString{};
}

QString UnifiedInputData::writeListNsvData(QSharedPointer<N3DUuidTable> table){
    QString text;
    text.append(myFloorData->writeData(table));
    text.append(myFrameData->writeData(table));
    text.append(myMaterialData->writeData());
    text.append(mySectionData->writeNsvData(table));
    return text;
}


void UnifiedInputData::executeChangeName(DATATYPE dtype, const QString &str_a, const QString &str_b )
{
    emit globalNameChanged(dtype, str_a, str_b);

    if ( dtype == DATATYPE::TPJOINTLOAD || dtype == DATATYPE::TPMEMBERLOAD || dtype == DATATYPE::TPSLABLOAD ) {
        myLoadData->replaceLoadName(dtype, str_a, str_b);
    } else {
        mySectionData->replaceSectionName(dtype, str_a, str_b);
    }
    QList<MemberData *> mdlist = myMemberData->replaceSectionName(*this, dtype, str_a, str_b);

    emit dataUpdated(dtype);
    emit memberChanged(mdlist);
}

void UnifiedInputData::estimate(QTextStream &outfile)const{
    QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
    outfile.setCodec( codec );
    outfile << LibraryVersion::Current().text() << "\n";

    //
    CalculationEstimate calc;
    outfile << calc.exec();

}

} // namespace post3dapp
