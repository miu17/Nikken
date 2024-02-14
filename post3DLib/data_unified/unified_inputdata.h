#ifndef UNIFIEDINPUTDATA_H
#define UNIFIEDINPUTDATA_H

#include "unified_data.h"
#include "n3d_uuidtable.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
class QUndoStack;
class QUndoCommand;
class QProgressDialog;

namespace post3dapp{

class POST3D_CLASS_DLLSPEC UnifiedInputData : public UnifiedData
{
    Q_OBJECT

public:

    static UnifiedInputData *getInInstance()
    {
        if (myInstance == nullptr) myInstance = UnifiedInputData::Create();
        return myInstance;
    }

    static void deleteInInstance()
    {
        if (myInstance != nullptr) delete myInstance;
        myInstance = nullptr;
    }

    static UnifiedInputData* Create();
    static UnifiedInputData* CreateDummy();
    static UnifiedInputData* Copy(const UnifiedInputData&);
    void clearUnifiedData();
    void writeP3dData(QDataStream &) const;
    QString writePsvData(QTextStream &) const;
    QString writeNsvData(QTextStream &, QSharedPointer<N3DUuidTable>);
    void readUnifiedData(QDataStream &, const LibraryVersion &ver);

    void estimate(QTextStream &)const;

    /* 符号変更 */
    void executeChangeName(DATATYPE, const QString &, const QString &);



signals:
    void globalNameChanged(DATATYPE, const QString &, const QString &);

private:
    static UnifiedInputData *myInstance;

    UnifiedInputData();
    UnifiedInputData(const UnifiedInputData &);
    UnifiedInputData &operator=(const UnifiedInputData &);

    QString writeListNsvData(QSharedPointer<N3DUuidTable>);
};


} // namespace post3dapp
#endif
