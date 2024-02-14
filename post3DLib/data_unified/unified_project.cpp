#include "unified_project.h"

#include "analysisconditionreader.h"
#include "version.h"


namespace post3dapp{
UnifiedProject *UnifiedProject::myInstance = nullptr;
UnifiedProject::UnifiedProject()
{
    stressCombiSettings = STRESSCOMBISETTINGS::GetDefault();
}

void UnifiedProject::writeP3dData(QDataStream &outfile) const
{
    outfile << generalAnalysiConditionText;
    outfile << editAnalysisConditionText;
    writeStressCombiData(outfile);
    writeGroupData(outfile);
}

void UnifiedProject::readP3dData(QDataStream &infile, const LibraryVersion &ver)
{
    infile >> generalAnalysiConditionText;
    infile >> editAnalysisConditionText;
    readStressCombiData(infile, ver);
    readGroupData(infile, ver);
}

void UnifiedProject::clearData()
{
    generalAnalysiConditionText.clear();
    editAnalysisConditionText.clear();
    clearStressCombiData();
    clearAnalysisGroup();
}

void UnifiedProject::registerTempAnalyisisData(const QString &general, const QString &edit)
{
    generalAnalysiConditionText = general;
    editAnalysisConditionText = edit;
}

QPair<QString, QString> UnifiedProject::getAnalysisCondition() const
{
    return QPair<QString, QString>(generalAnalysiConditionText, editAnalysisConditionText);
}

bool UnifiedProject::checkAnalysisCondition() const
{
    return AnalysisConditionReader::hasFormatError(editAnalysisConditionText);
}

QString UnifiedProject::parseAnalysisCondition(QTextStream &outfile) const
{
    QTextCodec* codec = QTextCodec::codecForName("Shift-JIS");
    outfile.setCodec( codec );
    outfile << generalAnalysiConditionText;

    QString errorMessage = AnalysisConditionReader::ConvertAnalysisCondition(editAnalysisConditionText, outfile);
    if (errorMessage != QString{})
        return u8"解析条件にエラーがあります。";

    return QString{};
}

void UnifiedProject::registerTempStressCombiData(const STRESSCOMBISETTINGS &set)
{
    stressCombiSettings = set;
    qDebug()<<"registerTempStressCombiData()"<<set.coef.value(u8"CaseDL");
    emit stressCombi_SettingChanged();
}

STRESSCOMBISETTINGS UnifiedProject::getStressCombinationSetting() const
{
    qDebug()<<"getStressCombi"<<stressCombiSettings.coef.value("CaseDL");
    return stressCombiSettings;
}

void UnifiedProject::readStressCombiData(QDataStream &infile, const LibraryVersion &ver)
{
    Q_UNUSED(ver);

    STRESSCOMBISETTINGS st;
    int count;
    infile >> count;
    for (int i = 0; i < count ; i++) {
        QString combi;
        infile >> combi;
        st.combi.append(combi);
        int step;
        infile >> step;
        st.step.append(step);
        int count2;
        infile >> count2;
        QList<qreal> tmplist1;
        QStringList tmplist2;
        for (int j = 0; j < count2 ; j++) {
            qreal list1;
            infile >> list1;
            tmplist1.append(list1);
            QString list2;
            infile >> list2;
            tmplist2.append(list2);
        }
        st.coef.insert(combi,tmplist1);
        st.stress.insert(combi,tmplist2);
    }
    registerTempStressCombiData(st);
}

void UnifiedProject::writeStressCombiData(QDataStream &outfile) const
{
    outfile << stressCombiSettings.combi.count();
    for(int i=0;i<stressCombiSettings.combi.count();i++){
        QString key = stressCombiSettings.combi.at(i);
        outfile << key << stressCombiSettings.step.at(i);
        outfile << stressCombiSettings.coef.value(key).count();
        for(int j=0;j<stressCombiSettings.coef.value(key).count();j++){
            outfile << stressCombiSettings.coef.value(key).at(j) << stressCombiSettings.stress.value(key).at(j);
        }
    }
}

void UnifiedProject::clearStressCombiData()
{
    stressCombiSettings = STRESSCOMBISETTINGS::GetDefault();
    emit stressCombi_SettingChanged();
}

void UnifiedProject::readGroupData(QDataStream &infile, const LibraryVersion &ver)
{
    int jCount;
    infile >> jCount;
    QList<GroupSetting> gList;
    for (int i = 0; i < jCount; i++){
        QString name;
        infile >> name;
        QList<QUuid> uuidList;
        infile >> uuidList;
        QString comment;
        infile >> comment;
        gList.append(GroupSetting(name, uuidList, comment));
    }

    int mCount;
    infile >> mCount;
    QList<GroupSetting> mList;
    for (int i = 0; i < mCount; i++){
        QString name;
        infile >> name;
        QList<QUuid> uuidList;
        infile >> uuidList;
        QString comment;
        infile >> comment;
        mList.append(GroupSetting(name, uuidList, comment));
    }
    registerTempAnalysisGroup(gList, mList);
}

void UnifiedProject::writeGroupData(QDataStream &outfile) const
{
    auto jGroup = getAnalyisisJointGroup();
    outfile << jGroup.count();
    foreach(auto j, jGroup){
        outfile << j.group << j.list << j.comment;
    }

    auto mGroup = getAnalyisisMemberGroup();
    outfile << mGroup.count();
    foreach(auto m, mGroup){
        outfile << m.group << m.list << m.comment;
    }
}

void UnifiedProject::registerTempAnalysisGroup(
        const QList<GroupSetting> &jGroup,
        const QList<GroupSetting> &mGroup)
{
    jointGroup = jGroup;
    memberGroup = mGroup;
}

void UnifiedProject::clearAnalysisGroup()
{
    jointGroup.clear();
    memberGroup.clear();
}

} // namespace post3dapp
