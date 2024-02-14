#ifndef OUTPUTTEXTREADER_H
#define OUTPUTTEXTREADER_H

#include "abstract_reader.h"
#include "line_outputtext.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class AbstractText;

class POST3D_CLASS_DLLSPEC OutputTextReader : public AbstractReader
{
public:
    OutputTextReader():AbstractReader(){};
    static bool hasFormatError(const QString&);
    QList<AbstractText*> fastParse(const QStringList&, QString *errorMessage) const; // データ構造の中身まで見ないテキストチェック
    QList<AbstractText*> cliParse(const QStringList&, QString *errorMessage, bool hasInput, bool hasOutput) const;
    QList<AbstractText*> appParse(const QStringList&, QString *errorMessage) const;
    static LineOutputText* CheckLineCondition(const QString&, OutputTextStatus, QString*);
    static LineOutputText* MatchLineCondition(const QString&, OutputTextStatus, QString*);
private:
    AbstractText* Create(OutputTextStatus) const;
    QList<AbstractText*> parse(const QStringList&, QString *errorMessage, bool readAll, bool hasInput = true, bool hasOutput = true) const;
    QString checkText(const QList<AbstractText*>&, bool, bool) const;

    static QList<LineOutputText*> conditions;
};
} // namespace post3dapp

#endif // OUTPUTTEXTREADER_H
