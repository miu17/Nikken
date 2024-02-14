#ifndef ABSTRACT_READER_H
#define ABSTRACT_READER_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
struct STRESS;
class POST3D_CLASS_DLLSPEC AbstractReader
{
public:
    AbstractReader();
    virtual ~AbstractReader(){};

protected:
    QString MakeGroup(const QString&, const QString&, QHash<QString, QList<QString>>*) const;
    QList<QString> ParseNameType(const QString&, const QHash<QString, QList<QString>>&) const;
    QList<QString> ParseHyphenType(const QString&, QString&, const QHash<QString, QList<QString>>&) const;
    QString ParseCcsType(const QString&, const QString&, QHash<QString, QList<STRESS>>*) const;
    QString ParseCase(const QString&) const;

};
} // namespace post3dapp
#endif // ABSTRACT_READER_H
