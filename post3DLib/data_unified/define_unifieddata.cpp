#include "define_unifieddata.h"

#include "define_defaultvalue.h"
#include "n3d_uuidtable.h"
namespace post3dapp{



QString FLOORVALUES::writeData(QSharedPointer<N3DUuidTable> table) const{
    QString pID = table == nullptr ? parentID.toString() : table->GetOrCreateN3dNumber(parentID, DATATYPE::TPFLOOR);
    QString uuid = table == nullptr ? UUID.toString() : table->GetOrCreateN3dNumber(UUID, DATATYPE::TPAFLOOR);
    return  uuid
            + "," + floorName
            + "," + QString::number(floorHeight)
            + "," + QString::number(displaysFloorScale)
            + "," + beamConcrete
            + "," + slabConcrete
            + "," + QString::number(adjustedLevel)
            + "," + QString::number(isChildFloor)
            + "," + pID;
}

QString FRAMEGROUPVALUE::writeData(QSharedPointer<N3DUuidTable> table) const{
    QString text;
    text.append(table == nullptr ? GID.toString() : table->GetOrCreateN3dNumber(GID, DATATYPE::TPFRAMEG)).append(",");
    text.append(groupName).append(",");
    text.append(QString::number(count)).append(",");
    switch(frameType){
    case FrameType::FRAMETYPE::STRAIGHT:
        text.append(QString::number(basePoint.x())).append(",");
        text.append(QString::number(basePoint.y())).append(",");
        text.append(QString::number(startAngle)).append(",");
        text.append(QString::number(plusLength)).append(",");
        text.append(QString::number(minusLength)).append(",");
        break;
    case FrameType::FRAMETYPE::CIRCLE:
        text.append(QString::number(basePoint.x())).append(",");
        text.append(QString::number(basePoint.y())).append(",");
        text.append(QString::number(startAngle)).append(",");
        text.append(QString::number(endAngle)).append(",");
        text.append(QString::number(division)).append(",");
        break;
    case FrameType::FRAMETYPE::RADIUS:
        text.append(QString::number(basePoint.x())).append(",");
        text.append(QString::number(basePoint.y())).append(",");
        text.append(QString::number(startAngle)).append(",");
        text.append(QString::number(plusLength)).append(",");
        break;
    case FrameType::FRAMETYPE::DIAMETER:
        text.append(QString::number(basePoint.x())).append(",");
        text.append(QString::number(basePoint.y())).append(",");
        text.append(QString::number(startAngle)).append(",");
        text.append(QString::number(plusLength)).append(",");
        text.append(QString::number(minusLength)).append(",");
        break;
    case FrameType::FRAMETYPE::VARIABLE:
        break;
    case FrameType::FRAMETYPE::CLCIRCLE:
        text.append(QString::number(basePoint.x())).append(",");
        text.append(QString::number(basePoint.y())).append(",");
        text.append(QString::number(division)).append(",");
        break;
    case FrameType::FRAMETYPE::CLVARIABLE:
        break;
    default:
        break;
    }
    text.append(table == nullptr ? startFloorID.toString() : table->GetOrCreateN3dNumber(startFloorID, DATATYPE::TPFLOOR)).append(",");
    text.append(table == nullptr ? endFloorID.toString() : table->GetOrCreateN3dNumber(endFloorID, DATATYPE::TPFLOOR));
    return text;
}

QString FRAMEPOINTSVALUE::writeData(QSharedPointer<N3DUuidTable> table) const{
    QString text;
    text.append(table == nullptr ? UUID.toString() : table->GetOrCreateN3dNumber(UUID, DATATYPE::TPFRAMEP)).append(",");
    text.append(frameName).append(",");
    switch (frameType) {
    case FrameType::FRAMETYPE::STRAIGHT:
    case FrameType::FRAMETYPE::CIRCLE:
    case FrameType::FRAMETYPE::RADIUS:
    case FrameType::FRAMETYPE::DIAMETER:
        text.append(straight.writeData(frameType));
        break;
    case FrameType::FRAMETYPE::VARIABLE:
    case FrameType::FRAMETYPE::CLCIRCLE:
    case FrameType::FRAMETYPE::CLVARIABLE:
        text.append(QString::number(pointList.count())).append(",");
        for (int i = 0; i < pointList.count(); i++){
            text.append(QString::number(pointList.at(i).x())).append("/");
            text.append(QString::number(pointList.at(i).y()));
            if (i != pointList.count() - 1)
                text.append(",");
        }
    default:
        break;
    }

    return text;
}

FrameType::FRAMETYPE FrameType::stringToEnum(const QString &str)
{
    if (str == REGULATION::defaultFrameGroupFrameType.at(0))
        return FRAMETYPE::STRAIGHT;
    if (str == REGULATION::defaultFrameGroupFrameType.at(1))
        return FRAMETYPE::CIRCLE;
    if (str == REGULATION::defaultFrameGroupFrameType.at(2))
        return FRAMETYPE::RADIUS;
    if (str == REGULATION::defaultFrameGroupFrameType.at(3))
        return FRAMETYPE::DIAMETER;
    if (str == REGULATION::defaultFrameGroupFrameType.at(4))
        return FRAMETYPE::VARIABLE;
    if (str == REGULATION::defaultFrameGroupFrameType.at(5))
        return FRAMETYPE::CLCIRCLE;
    if (str == REGULATION::defaultFrameGroupFrameType.at(6))
        return FRAMETYPE::CLVARIABLE;

    return FRAMETYPE::NOFRAMETYPE;
}

QString FrameType::enumToString(const FrameType::FRAMETYPE &type)
{
    switch(type){
    case FRAMETYPE::STRAIGHT:
        return REGULATION::defaultFrameGroupFrameType.at(0);
    case FRAMETYPE::CIRCLE:
        return REGULATION::defaultFrameGroupFrameType.at(1);
    case FRAMETYPE::RADIUS:
        return REGULATION::defaultFrameGroupFrameType.at(2);
    case FRAMETYPE::DIAMETER:
        return REGULATION::defaultFrameGroupFrameType.at(3);
    case FRAMETYPE::VARIABLE:
        return REGULATION::defaultFrameGroupFrameType.at(4);
    case FRAMETYPE::CLCIRCLE:
        return REGULATION::defaultFrameGroupFrameType.at(5);
    case FRAMETYPE::CLVARIABLE:
        return REGULATION::defaultFrameGroupFrameType.at(6);
        break;
    default:
        throw;
    }
}

LoadType::LOADTYPE LoadType::stringToEnum(const QString &str)
{
    if (str == u8"DL")
        return LOADTYPE::LTYPE_DL;
    if (str == u8"LL0")
        return LOADTYPE::LTYPE_LL0;
    if (str == u8"LL1")
        return LOADTYPE::LTYPE_LL1;
    if (str == u8"LL2")
        return LOADTYPE::LTYPE_LL2;
    if (str == u8"LL3")
        return LOADTYPE::LTYPE_LL3;
    if (str == u8"LL4")
        return LOADTYPE::LTYPE_LL4;
    if (str == u8"LLE")
        return LOADTYPE::LTYPE_LLE;

    return LOADTYPE::LTYPE_LL0;
}

QString LoadType::enumToString(const LoadType::LOADTYPE &type)
{
    switch(type){
    case LOADTYPE::LTYPE_DL:
        return u8"DL";
    case LOADTYPE::LTYPE_LL0:
        return u8"LL0";
    case LOADTYPE::LTYPE_LL1:
        return u8"LL1";
    case LOADTYPE::LTYPE_LL2:
        return u8"LL2";
    case LOADTYPE::LTYPE_LL3:
        return u8"LL3";
    case LOADTYPE::LTYPE_LL4:
        return u8"LL4";
    case LOADTYPE::LTYPE_LLE:
        return u8"LLE";

    default:
        qFatal("");
    }
}

} // namespace post3dapp
