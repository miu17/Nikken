#include "define_material_struct.h"

#include <QtGui>

namespace post3dapp{
MaterialType::Type MaterialType::stringToEnum(const QString &str)
{
    Type retenum = Type::UNDEFINED;
    if ( str == u8"コンクリート" ) {
        retenum = Type::CONCRETE;
    } else if ( str == u8"鉄骨" ) {
        retenum = Type::STEEL;
    } else if ( str == u8"鉄筋" ) {
        retenum = Type::REINFORCE;
    }
    return retenum;
}

QString MaterialType::enumToString(const MaterialType::Type &type){
    if (type == Type::CONCRETE)
        return u8"コンクリート";
    if (type == Type::STEEL)
        return u8"鉄骨";
    if (type == Type::REINFORCE)
        return u8"鉄筋";
    return QString();
}

QString MATERIALVALUES::writeData() const{
    QString text;
    text.append(materialName).append(",");
    switch(materialType){
    case MaterialType::Type::CONCRETE:
        text.append(QString::number(E)).append(",");
        text.append(QString::number(G)).append(",");
        text.append(QString::number(unitW1)).append(",");
        text.append(QString::number(unitW2)).append(",");
        text.append(QString::number(modularRatio)).append(",");
        text.append(QString::number(F)).append(",");
        break;
    case MaterialType::Type::STEEL:
        text.append(QString::number(E)).append(",");
        text.append(QString::number(G)).append(",");
        text.append(QString::number(unitW1)).append(",");
        text.append(QString::number(F)).append(",");
        break;
    case MaterialType::Type::REINFORCE:
        text.append(symbol).append(",");
        text.append(QString::number(barMin)).append(",");
        text.append(QString::number(barMax)).append(",");
        text.append(QString::number(E)).append(",");
        break;
    }
    text.append(QString::number(Ft_L)).append(",");
    text.append(QString::number(Ft_S)).append(",");
    text.append(QString::number(Ft_U)).append(",");
    text.append(QString::number(Fc_L)).append(",");
    text.append(QString::number(Fc_S)).append(",");
    text.append(QString::number(Fc_U)).append(",");
    text.append(QString::number(Fs_L)).append(",");
    text.append(QString::number(Fs_S)).append(",");
    text.append(QString::number(Fs_U));

    if (materialType == MaterialType::Type::CONCRETE){
        text.append(",");
        text.append(QString::number(Fa_L)).append(",");
        text.append(QString::number(Fa_S)).append(",");
        text.append(QString::number(Fa_U)).append(",");
        text.append(QString::number(Fa_Lu)).append(",");
        text.append(QString::number(Fa_Su));
    }
    return text;
}
} // namespace post3dapp
