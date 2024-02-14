#ifndef OUTPUTFIGUREREADER_H
#define OUTPUTFIGUREREADER_H

#include <QSharedPointer>
#include "abstract_reader.h"
#include "line_outputfigure.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class OutputPaperSetting;
struct BASE_TYPE_PROPERTY;
struct FIGURE_TYPE_PROPERTY;
struct STRESS;
class POST3D_CLASS_DLLSPEC OutputFigureReader : public AbstractReader
{
public:
    OutputFigureReader():AbstractReader(){}
    static bool hasFormatError(const QString&);
    QList<BASE_TYPE_PROPERTY*> fastParse(const QStringList&, QString *errorMessage) const; // データ構造の中身までみないテキストチェック。
    QList<BASE_TYPE_PROPERTY*> cliParse(const QStringList&, QString *errorMessage, bool hasInput, bool hasInputCopy, bool hasCalc, bool hasOutput) const; // CLIはInputだけの出力の場合もある
    QList<BASE_TYPE_PROPERTY*> appParse(const QStringList&, QString *errorMessage) const; // App側全てのデータを持っている前提
    static QSharedPointer<LineOutputFigure> CheckLineCondition(const QString&, OutputFigureStatus, QString*);
    static QSharedPointer<LineOutputFigure> MatchLineCondition(const QString&, OutputFigureStatus, QString*);
private:
    static QList<QSharedPointer<LineOutputFigure>> conditions;
    void ParseQudsType(const QString&, const QString&, const QHash<QString, QList<QString>>&, QHash<QString, QList<QString>>*) const;
    QString ParseCrezType(const QString&, const QString&, const QHash<QString, QList<STRESS>>&, QHash<QString, QList<QString>>&, const FIGURE_TYPE_PROPERTY*, QList<BASE_TYPE_PROPERTY*>*) const;
    QString checkProperties(const QList<BASE_TYPE_PROPERTY*>&, bool, bool, bool, bool) const;
    QList<BASE_TYPE_PROPERTY*> parse(const QStringList&, QString *errorMessage, bool readAll, bool hasInput = true, bool hasInputCopy = true, bool hasCalc = true, bool hasOutput = true) const; // readAll = false はCLIで解析データ読み込みが必要かの判定用
};
} // namespace post3dapp

#endif // OUTPUTFIGUREREADER_H
