#ifndef ABSTRACTLINEDATA_H
#define ABSTRACTLINEDATA_H

#include <QString>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class TextDataUnit;

class POST3D_CLASS_DLLSPEC AbstractLineData
{
public:
    AbstractLineData(const QString& _key,
                     const QList<TextDataUnit>& _units)
        :key(_key), dataUnits(_units){};
    virtual ~AbstractLineData(){};
    QList<TextDataUnit> getUnits() const{return dataUnits;}
    virtual bool hasFormatError(const QString&) const;
    QString getStatusBar(int) const;
protected:
    QString key;
    //int secondKeywordIndex;
    QList<TextDataUnit> dataUnits;

};

} // namespace post3dapp

#endif // ABSTRACTLINEDATA_H
