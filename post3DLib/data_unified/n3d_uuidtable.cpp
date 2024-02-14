#include "n3d_uuidtable.h"

#include "define_unifieddata.h"
#include "fixed_data.h"
#include "unified_inputdata.h"

namespace post3dapp{

N3DUuidTable::N3DUuidTable()
{
    uuidTable.insert(QUuid(), 0);
    maxSize = 0;
}

QString N3DUuidTable::GetOrCreateN3dNumber(const QUuid& uuid, DATATYPE type){
    if (!uuidTable.contains(uuid)){
        maxSize++;
        uuidTable.insert(uuid, maxSize);
        return QString::number(maxSize);
    }
    else{
        if (uuid == QUuid())
            return "0";
        else if (type == DATATYPE::TPJOINT)
            return QString::number(uuidTable.value(uuid));
        else
            return UnifiedFixedData::getInstance()->getInputData()->idToName(uuid, type);
    }
}

QString N3DUuidTable::GetOrCreateN3dMemberNumber(const QUuid& uuid, int index){
    if (!uuidTable.contains(uuid)){
        maxSize++;
        uuidTable.insert(uuid, maxSize);
    }

    if (index != 0){
        maxSize++;
    }
    memberTable.insert(maxSize, QPair<QUuid, int>{uuid, index});
    return QString::number(maxSize);
}

QString N3DUuidTable::write()
{
    QString text;
    text.append("*UUID").append("\n");
    foreach(auto t, uuidTable.keys()){
        text.append(t.toString()).append(",").append(QString::number(uuidTable.value(t))).append("\n");
    }
    return text;
}

} // namespace post3dapp
