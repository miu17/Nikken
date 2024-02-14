#include "unified_analysisdata.h"

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QProgressDialog>
#include <QStringList>
#include <QUndoStack>

#include "calc_vector2d.h"
#include "manage_springdata.h"
#include "unified_calcload.h"
#include "unified_output.h"
namespace post3dapp{
UnifiedAnalysisData::UnifiedAnalysisData()
    :UnifiedData(UnifiedDataType::Pstn)
{
    mySpringData = new ManageSpringData();

}

SpringData* UnifiedAnalysisData::createSpring(const QList<JointData *> &jlist, B3D_DATATYPE type,
                                      const QString &name,const QUuid &uid,const VEC3D& vecx,const VEC3D &vecy,
                                      const  QString &yBaneName,const  QString &zBaneName,bool isISO)
{
    return mySpringData->createSpring(jlist, type, name, uid,vecx,vecy,yBaneName,zBaneName,isISO);
}

SpringData* UnifiedAnalysisData::springIDToPointer(const QUuid &id) const
{
    return mySpringData->springIDToPointer(id);
}

QList<QUuid> UnifiedAnalysisData::getIDList(DATATYPE type) const
{
    if ( type == DATATYPE::TPSPRING ) return mySpringData->getIDList();
    return UnifiedData::getIDList(type);
}
} // namespace post3dapp
