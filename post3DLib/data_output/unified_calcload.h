#ifndef UNIFIEDCALCLOAD_H
#define UNIFIEDCALCLOAD_H

#include <QObject>
#include <QDebug>
#include <QHash>
#include "calc_load.h"
#include "define_section_struct.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
class JointData;
class MemberData;
struct JLOADSUM;
struct MLOADSUM;

class POST3D_CLASS_DLLSPEC UnifiedCalcLoad : public QObject
{
    Q_OBJECT

public:
    static UnifiedCalcLoad* Create(){return new UnifiedCalcLoad();}
    QString executeCalculation(const QString&, QProgressDialog*, bool writeOutput);

    // 荷重拾い
    // Global -> Localへ変換, ポインタをUuidへ
//    void updateCalcLoadData( const QMultiHash<MemberData*,MLOADSUM*> &,
//                             const QMultiHash<JointData*,JLOADSUM*> & );

    QString getLoadFileDateTime(const QString&) const;
    void readDataOnMemory(const QString&);

    //荷重
    bool isLoadedJoint(JointData*) const;
    bool isLoadedMember(MemberData*) const;
    QList<JLOADSUM *> loadOfJoint(JointData*) const;
    QList<MLOADSUM *> loadOfMember(MemberData*) const;
    /*
        void appendMemberLoad( const QUuid &, const QString &, LOADTYPE, LOADDIR, const QList<qreal>& );
        void appendMemberLoad( const QUuid &, const QString &, LOADTYPE, LOADDIR, const QList<QPointF>& );
        void appendMemberLoad( const QUuid &, const QString &, LOADTYPE, LOADDIR, const WABLOAD& );
    */




    //file
    void readCalcLoadData(QDataStream &, const QString &ver = QString());
    void writeCalcLoadData(QDataStream &, const QString &ver = QString());
    static double getStandardArea(const QString&); // 単位mm2
    static SteelType::Type getStandardType(const QString&);
public slots:

signals:

    void dataUpdated();

private:
    UnifiedCalcLoad();
    UnifiedCalcLoad(const UnifiedCalcLoad &);
    UnifiedCalcLoad &operator=(const UnifiedCalcLoad &);
    CalculationLoad load;
    static QHash<QString, double> standardAreaList;
    static QHash<QString, SteelType::Type> standardTypeList;
};
} // namespace post3dapp
#endif
