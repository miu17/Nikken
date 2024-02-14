#ifndef UNIFIEDANALYSISDATA_H
#define UNIFIEDANALYSISDATA_H

#include "calc_vector3d.h"
#include "unified_data.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
class QUndoStack;
class QUndoCommand;
class QProgressDialog;

namespace post3dapp{
class SpringData;
class ManageSpringData;

class POST3D_CLASS_DLLSPEC UnifiedAnalysisData : public UnifiedData
{
    Q_OBJECT

public:
    static UnifiedAnalysisData* Create(){return new UnifiedAnalysisData();}
    SpringData *createSpring(const QList<JointData *> &, B3D_DATATYPE, const QString &, const QUuid &uid ,
                             const VEC3D&, const VEC3D&, const QString &yBaneName, const QString &zBaneName, bool isISO);

    SpringData *springIDToPointer(const QUuid &) const;
    QList<QUuid> getIDList(DATATYPE) const override;


private:
    UnifiedAnalysisData();
    UnifiedAnalysisData(const UnifiedAnalysisData &);
    UnifiedAnalysisData &operator=(const UnifiedAnalysisData &);

    ManageSpringData *mySpringData;


};
} // namespace post3dapp

#endif // UNIFIEDANALYSISDATA_H
