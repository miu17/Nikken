#ifndef MANAGE_SPRINGDATA_H
#define MANAGE_SPRINGDATA_H
#include <QObject>
#include "define_unifieddata.h"
#include "manage_memberdata.h"
#include "calc_vector3d.h"
#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
class JointData;
class SpringData;

class POST3D_CLASS_DLLSPEC ManageSpringData : public QObject
{
    Q_OBJECT

public:

    ManageSpringData( QObject *parent = nullptr ,bool isOutput = false);

    //pstn、あるいは解析条件からの入力
    void clearData();
    SpringData *createSpring(const QList<JointData *> &, B3D_DATATYPE, const QString &, const QUuid &uid , const post3dapp::VEC3D &vecx, const post3dapp::VEC3D &vecy,
                             const QString &yBaneName, const QString &zBaneName, bool isISO);

    void removeSpring(SpringData * );

    SpringData *springIDToPointer(const QUuid &) const;
    QList<QUuid> getIDList() const;



public slots:

signals:

private:

    QList<SpringData *> springList;
    QHash<QUuid, SpringData *> uuidTable;

    void appendDataList(SpringData *sd);
    void removeDataList(SpringData *sd);
    bool isOutput;

};

class POST3D_CLASS_DLLSPEC SpringData : public GeneralMemberData//ばねの部材データ
{

public:

    SpringData(const QList<JointData *> &jlist, B3D_DATATYPE type, const QString &_name, const QUuid &id,
               const VEC3D& vecx,const VEC3D& vecy ,const QString& _yBaneName,const QString& _zBaneName,bool isISO); //  bool execRecalc=true);

    QString springName()const;

    B3D_DATATYPE springType()const{return spring_type;}
    bool isISO()const{return isISOFlag;}
    void setISOFlag(bool _isISO){isISOFlag=_isISO;}


private:

    bool isISOFlag;
    B3D_DATATYPE spring_type;
    QString name;

    //ばね名で登録、ばね断面はUnifiedDataに問い合わせを作成し、ばねの特性をget
    QString yBaneName;
    QString zBaneName;

};

} // namespace post3dapp
#endif // MANAGE_SPRINGDATA_H
