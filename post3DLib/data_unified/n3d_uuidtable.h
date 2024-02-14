#ifndef N3DUUIDTABLE_H
#define N3DUUIDTABLE_H
#include <QHash>
#include <QObject>
#include <QUuid>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
enum class DATATYPE;
class POST3D_CLASS_DLLSPEC N3DUuidTable : public QObject
{
    Q_OBJECT

public:
    N3DUuidTable();
    QString GetOrCreateN3dNumber(const QUuid&, DATATYPE);
    QString GetOrCreateN3dMemberNumber(const QUuid&, int);
    QString GetN3dJoint(const QUuid& uuid) const{
        return QString::number(uuidTable.value(uuid));
    };
    QString GetN3dMember(const QUuid& uuid, int index) const{
        return QString::number(memberTable.key(QPair<QUuid, int>(uuid, index)));
    }
    QString write();

private:
    int maxSize;
    QHash<QUuid, int> uuidTable; // UUidとint。intも一つしかない
    QHash<int, QPair<QUuid, int>> memberTable; // key:n3d用int, <uuidTableのint, memberの順序番号：中間節点がある場合は1, 2>
};
}
#endif // N3DUUIDTABLE_H
