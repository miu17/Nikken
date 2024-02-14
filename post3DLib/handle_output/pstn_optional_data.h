#ifndef PSTNOPTIONALDATA_H
#define PSTNOPTIONALDATA_H

#include <QBrush>
#include <QFont>
#include <QDebug>
#include <QList>
#include <QObject>
#include <QStringList>
#include "qmath.h"

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{

class POST3D_CLASS_DLLSPEC Floor
{
public:
    explicit Floor(QString _name = QString(), qreal _height = 0.0, QUuid _uuid = QUuid()): name(_name),
        height(_height), UUID(_uuid) {}
    QString name;
    qreal height;
    QUuid UUID;
    bool operator==(const Floor &other) const
    {
        return (name == other.name && height == other.height);
    }
    bool operator<(const Floor &other) const
    {
        return (height < other.height);
    }
};
class POST3D_CLASS_DLLSPEC Frame
{
    //Frameに属するデータとFrameGroupに属するデータが混在しているので別ける？
public:
    explicit Frame(QString _name = QString(), qreal _coordinate = 0.0, int _gridgroup = 1,
                   QUuid _uuid = QUuid(), QUuid _gid = QUuid())
        : name(_name), coordinate(_coordinate), gridgroup(_gridgroup), UUID(_uuid), GID(_gid) {}
    QString name;
    qreal coordinate;
    int gridgroup;
    QUuid UUID;
    QUuid GID;

    bool operator==(const Frame &other) const
    {
        if (name == other.name && gridgroup == other.gridgroup && coordinate == other.coordinate) {
            return true;
        } else {
            return false;
        }
    }
    bool operator<(const Frame &other) const
    {
        if (gridgroup < other.gridgroup) {
            return true;
        } else if (gridgroup > other.gridgroup) {
            return false;
        } else if (coordinate < other.coordinate) {
            return true;
        } else {
            return false;
        }
    }
};

/* 部材データ */

//class JointData;
//class MemberStress;
class POST3D_CLASS_DLLSPEC OptionalMemberData
{
public:
    enum class IWALL_OR_HWALL {IWALL = 0, HWALL};
    enum class YIELD_STATE {NOT = 0, YIELD, HINGE, SHEAR_CRACK, SHEAR_YIELD, AXIAL_YIELD};
    OptionalMemberData( int _openingNo = -1, bool _isStructuralWall = false,
                        IWALL_OR_HWALL _walldirection = IWALL_OR_HWALL::IWALL)
        : wallDirection(_walldirection), isStructuralWall(_isStructuralWall), openingNo(_openingNo) {}
    IWALL_OR_HWALL wallDirection;
    bool isStructuralWall;
    int openingNo;
    QHash<QString, QList<QList<YIELD_STATE> > > yieldstate; //key=stresscase,list=[ijc][step]
};


/*開口データ*/
class POST3D_CLASS_DLLSPEC Opening
{
public:
    // enum OPENINGTYPE{SHAPE=0,ANALYTICAL};->3Dのどこで定義しているか確認
    enum class LEFT_OR_RIGHT {WFULL = 0, LEFT, RIGHT, CENTER};
    enum class TOP_OR_BOTTOM {HFULL = 0, BOTTOM, TOP, MIDDLE};
    // OPENINGTYPE type;
    Opening() {}
    Opening(LEFT_OR_RIGHT _lr, TOP_OR_BOTTOM _tb, qreal _px, qreal _py, qreal _w, qreal _h)
        : lr(_lr), tb(_tb), positionx(_px), positiony(_py), width(_w), height(_h) {}
    LEFT_OR_RIGHT lr;
    TOP_OR_BOTTOM tb;
    qreal positionx;
    qreal positiony;
    qreal width;
    qreal height;
};
struct POST3D_CLASS_DLLSPEC
    OpeningSet { //OpeningList.at(int)でlabelと開口の組を得られるようにしたい
public:
    OpeningSet() {}
    OpeningSet(QString _label, const QList<Opening *> &_openings): label(_label), openings(_openings) {}
    QString label;
    QList<Opening *> openings;
};

} // namespace post3dapp
#endif
