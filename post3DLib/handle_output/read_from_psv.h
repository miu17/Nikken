#ifndef READ_FROM_PSV_H
#define READ_FROM_PSV_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include <QVariant>

class QTextStream;

namespace post3dapp{
class InterpreterCsv;
struct REGULATION;

class POST3D_CLASS_DLLSPEC ReadFromPsv
{
public:
    ReadFromPsv();
    void ReadPsv(QFile*);
private:
    void readFloor(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readFrame(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readMaterial(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readRcColumn(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSColumn(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSrcColumn(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readCftColumn(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readRcGirder(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSGirder(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSrcGirder(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSBrace(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readRcBeam(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSBeam(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSrcBeam(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSlab(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readRcWall(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSWall(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readFoundationWall(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSBase(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSrcBase(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readOpening(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readDamper(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readIsolator(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readJointLoad(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readMemberLoad(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readSlabLoad(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readAnalysisFloor(QTextStream &, QSharedPointer<InterpreterCsv>);
    void readAnalysisCondition(QTextStream&, QSharedPointer<InterpreterCsv>);

    void arrange(QSharedPointer<InterpreterCsv>);

    QString parseString(const QString&, const QMap<QString, REGULATION>&, const QString&, const QVariant& = QVariant());
    double parseDouble(const QString&, const QMap<QString, REGULATION>&, const QString&, const QVariant& = QVariant());
    int parseInt(const QString&, const QMap<QString, REGULATION>&, const QString&, const QVariant& = QVariant());
};

} // namespace post3dapp
#endif // READ_FROM_PSV_H
