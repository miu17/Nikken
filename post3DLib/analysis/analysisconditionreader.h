#ifndef ANALYSISCONDITIONREADER_H
#define ANALYSISCONDITIONREADER_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
enum class ANALYSISSTEP;
enum class LINESTATUSTYPE;
enum class MEMBERCATEGORY;
class LineCondition;
class GeneralLineCondition;

class POST3D_CLASS_DLLSPEC AnalysisConditionReader{
public:
    AnalysisConditionReader(){};
    static QSharedPointer<LineCondition> CheckLineCondition(const QString&, ANALYSISSTEP, LINESTATUSTYPE, bool*);
    static QSharedPointer<LineCondition> MatchLineCondition(const QString&, ANALYSISSTEP, LINESTATUSTYPE, bool*);
    static QString ConvertAnalysisCondition(const QString&, QTextStream& out);
    static bool hasFormatError(const QString&);
    static QMap<int, QString> ReadGeneralAnalysisCondition(const QStringList&);
    static QString DefaultGeneralAnalysisConditionText(); // UnifiedInputDataの初期値
    static QStringList DefaultGeneralAnalysisConditionLines(); // 1行ごと
    static QMap<int, QSharedPointer<GeneralLineCondition>> DefaultGeneralAnalysisConditionMap(); // 上記をMapにしたもの
private:
    static QStringList defaultGeneralAnalysisConditionLines;
    static QMap<int, QSharedPointer<GeneralLineCondition>> defaultGeneralAnalysisConditionMap;
    static QList<QSharedPointer<LineCondition>> Create(
            ANALYSISSTEP,
            LINESTATUSTYPE,
            const QList<MEMBERCATEGORY>&);
    static QSharedPointer<LineCondition> endCondition;
    static QList<QSharedPointer<LineCondition>> convConditions;
    static QList<QSharedPointer<LineCondition>> premConditions;
    static QList<QSharedPointer<LineCondition>> nminConditions;
    static QList<QSharedPointer<LineCondition>> presConditions;
    static QList<QSharedPointer<LineCondition>> preaConditions;
    static QList<QSharedPointer<LineCondition>> solsConditions;
    static QList<QSharedPointer<LineCondition>> stecConditions;
    static QList<QSharedPointer<LineCondition>> chckConditions;
    static QList<QSharedPointer<LineCondition>> qudsConditions;
    static QList<QSharedPointer<LineCondition>> qdcvConditions;
    static QList<QSharedPointer<LineCondition>> modeConditions;
    static QList<QSharedPointer<LineCondition>> conditions;
};
} // namespace post3dapp
#endif // ANALYSISCONDITIONREADER_H
