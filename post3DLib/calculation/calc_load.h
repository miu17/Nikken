#ifndef CALCULATIONLOAD_H
#define CALCULATIONLOAD_H

#include <QObject>
#include "define_unifieddata.h"
class QPainterPath;
class QProgressDialog;
struct VEC3D;


#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
enum class DATATYPE;
enum class LOADDIR;
class JointData;
class MemberData;
struct MLOADSUM;
struct JLOADSUM;
struct WABLOAD;


enum class BOUNDARY { PIN_PIN = 0, FIX_FREE, FREE_FIX };


struct SLITJOINTS {
    explicit SLITJOINTS( const QList<JointData *> &_j1 = QList<JointData *>(),
                         const QList<JointData *> &_j2 = QList<JointData *>(),
                         const QList<JointData *> &_j3 = QList<JointData *>(),
                         const QList<JointData *> &_j4 = QList<JointData *>() )
        : topJoints(_j1), bottomJoints(_j2), leftJoints(_j3), rightJoints(_j4) { }

    QList<JointData *> topJoints;
    QList<JointData *> bottomJoints;
    QList<JointData *> leftJoints;
    QList<JointData *> rightJoints;
};

class POST3D_CLASS_DLLSPEC CalculationLoad : public QObject
{
    Q_OBJECT

public:

    CalculationLoad(QObject *parent = nullptr);
    QString executeCalculation(const QString&, QProgressDialog *dialog = nullptr);
    void deleteCashData();
    QString write(const QString&);

    bool isLoadedJoint(JointData *) const;
    bool isLoadeMember(MemberData *) const;
    QList<JLOADSUM *> loadOfJoint(JointData *) const;
    QList<MLOADSUM *> loadOfMember(MemberData *) const;
    void read(const QString&);

signals:

private:


    QString calcForMemberArea( DATATYPE, int &, const int &, QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads, QProgressDialog *dialog = nullptr);
    QString calcWallOpen( const QList<MemberData *> &,
                          QList<QPainterPath> &, QList<SLITJOINTS> & );

    QList<MemberData *> sharedLineMembers( JointData *, JointData * ) const;

    QString transmitBeamLoad( MemberData * );

    // sumUpLoad
    void appendMemberLoad( MemberData *, const QString &, LoadType::LOADTYPE, LOADDIR, const QList<qreal> &, QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads);
    // concentratedLoad
    void appendMemberLoad( MemberData *, const QString &, LoadType::LOADTYPE, LOADDIR, const QPointF & , QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads);
    // concentratedLoad
    void appendMemberLoad( MemberData *, const QString &, LoadType::LOADTYPE, LOADDIR, const QList<QPointF> & , QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads);
    // loadWAB
    void appendMemberLoad( MemberData *, const QString &, LoadType::LOADTYPE, LOADDIR, const WABLOAD & , QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads);

    void appendLocalMemberLoad( MemberData *, const QString &, LoadType::LOADTYPE, LOADDIR,
                                const QList<qreal> & );
    void appendLocalMemberLoad( MemberData *, const QString &, LoadType::LOADTYPE, LOADDIR,
                                const QList<QPointF> & );
    void appendLocalMemberLoad( MemberData *, const QString &, LoadType::LOADTYPE, LOADDIR, const WABLOAD & );

    void appendJointLoad( JointData *, const QString &, LoadType::LOADTYPE, LOADDIR, qreal );

    void appendSelfWeight( MemberData * , QMultiHash<MemberData *, MLOADSUM *> &globalMemberLoads);

    QString jointInfo(JointData *) const;
    QString memberInfo(MemberData *) const;

    //QMultiHash<MemberData *, MLOADSUM *> globalMemberLoads;
    QMultiHash<MemberData *, MLOADSUM *> localMemberLoads;
    QMultiHash<JointData *, JLOADSUM *> globalJointLoads;
};
} // namespace post3dapp
#endif
