#ifndef UNIFIEDPROJECT_H
#define UNIFIEDPROJECT_H


#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include "define_unifieddata.h"

namespace post3dapp{
class LibraryVersion;
class POST3D_CLASS_DLLSPEC UnifiedProject : public QObject
{
    Q_OBJECT
public:

    static UnifiedProject *getInstance()
    {
        if (myInstance == nullptr) myInstance = new UnifiedProject();
        return myInstance;
    }

    void writeP3dData(QDataStream &) const;
    void readP3dData(QDataStream &, const LibraryVersion &ver);
    void clearData();

    /* 解析条件 */
    void registerTempAnalyisisData(const QString&, const QString&);
    QPair<QString, QString> getAnalysisCondition() const;
    bool checkAnalysisCondition() const;
    QString parseAnalysisCondition(QTextStream &) const;

    /* 応力組み合わせ */
    void registerTempStressCombiData(const STRESSCOMBISETTINGS &);
    STRESSCOMBISETTINGS getStressCombinationSetting()const;

    /* 解析グループ定義 */
    void registerTempAnalysisGroup(
            const QList<GroupSetting>&,
            const QList<GroupSetting>&);
    QList<GroupSetting> getAnalyisisJointGroup() const{return jointGroup;}
    QList<GroupSetting> getAnalyisisMemberGroup() const{return memberGroup;}

signals:
    void stressCombi_SettingChanged();

private:
    UnifiedProject();
    static UnifiedProject *myInstance;

    /*解析条件*/
    QString generalAnalysiConditionText;
    QString editAnalysisConditionText;

    /* 応力組み合わせ設定 */
    STRESSCOMBISETTINGS stressCombiSettings;
    void readStressCombiData(QDataStream &infile, const LibraryVersion &ver);
    void writeStressCombiData(QDataStream &outfile)const;
    void clearStressCombiData();

    /* 解析用グループ定義 */
    QList<GroupSetting> jointGroup;
    QList<GroupSetting> memberGroup;
    void readGroupData(QDataStream &infile, const LibraryVersion &ver);
    void writeGroupData(QDataStream &outfile) const;
    void clearAnalysisGroup();

};
} // namespace post3dapp

#endif // UNIFIEDPROJECT_H
