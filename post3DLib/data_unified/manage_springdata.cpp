#include "manage_springdata.h"

#include <QDebug>

#include "manage_jointdata.h"
#include "unified_data.h"

namespace post3dapp{
ManageSpringData::ManageSpringData(QObject *parent, bool isout)
    : QObject(parent)
{
    isOutput = isout;
}

void ManageSpringData::clearData()
{
    Q_FOREACH (SpringData *sd, springList) delete sd;
    springList.clear();
    uuidTable.clear();
}
QList<QUuid> ManageSpringData::getIDList() const
{
    QList<QUuid> idlist;
    Q_FOREACH (SpringData *sd, springList) {
        idlist << sd->getUuid();
    }
    return idlist;
}

SpringData *ManageSpringData::createSpring(const QList<JointData *> &jlist,  B3D_DATATYPE type,
                                           const QString &name, const QUuid &id, const VEC3D& vecx, const VEC3D& vecy,
                                           const QString &yBaneName, const QString &zBaneName,bool isISO)
{
    if ( jlist.count() != 2 ) return nullptr;


    SpringData *sd = new SpringData(jlist,type, name, id,vecx,vecy,yBaneName,zBaneName,isISO);
    appendDataList(sd);
    return sd;
}

void ManageSpringData::removeSpring(SpringData *sd)
{
    removeDataList(sd);
    delete sd;
}




SpringData *ManageSpringData::springIDToPointer(const QUuid &id) const
{
    if ( id.isNull() ) return nullptr;
    return ( uuidTable.contains(id) ) ? uuidTable.value(id) : nullptr ;
}


void ManageSpringData::appendDataList(SpringData *sd)
{
    springList.append(sd);
    uuidTable.insert(sd->getUuid(), sd);
}

void ManageSpringData::removeDataList(SpringData *sd)
{
    springList.removeOne(sd);

    QUuid id = uuidTable.key(sd);
    if ( !id.isNull() ) uuidTable.remove(id);
}




SpringData::SpringData(const QList<JointData *> &jdlist,  B3D_DATATYPE type, const QString &_name, const QUuid &id , const VEC3D &_vecx, const VEC3D &_vecy,
                       const QString &_yBaneName, const QString &_zBaneName,bool isISO)
{
    memberID = ( id.isNull() ) ? QUuid::createUuid() : id ;

    jointList = jdlist;

    spring_type = type;
    name = _name;
    yBaneName=_yBaneName;
    zBaneName=_zBaneName;
    isISOFlag = isISO;

    U_Vec = _vecx;
    V_Vec = _vecy;

}

QString SpringData::springName() const
{
    return name;
}




} // namespace post3dapp
