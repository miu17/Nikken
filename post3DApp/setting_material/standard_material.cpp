#include "standard_material.h"
#include <QDebug>
#include <QResource>
namespace post3dapp{
QStringList StandardMaterialTable::getMaterialNameList(const QString &mtype) const
{
    return this->value(mtype);
}

bool  StandardMaterialTable::existsMaterialName( const QString &mname ) const
{
    return valueList.contains(mname);
}

QString StandardMaterialTable::getMaterialValue(const QString &nm, format fm) const
{
    if ( !valueList.contains(nm) ) return QString();
    int ii = static_cast<int>(fm);
    QStringList line = valueList.value(nm);
    return ( ii < line.count() ) ? line.at(ii) : QString();
}

void StandardMaterialTable::polulateList()
{
    Q_INIT_RESOURCE(resource);
    QFile file(":/standard_material/standard_material.csv");
    if ( !file.open(QIODevice::ReadOnly) ) return;
    QTextStream in(&file);
    QStringList words, clist, slist, rlist;
    while ( !in.atEnd() ) {
        words = in.readLine().split(',');
        if ( words.count() < 2 ) continue;
        if ( words.at(1).isEmpty() ) continue;
        if ( words.at(0) == "CONCRETE" ) {
            clist.append( words.at(1) );
        } else if ( words.at(0) == "STEEL" ) {
            slist.append( words.at(1) );
        } else if ( words.at(0) == "REINFORCE" ) {
            rlist.append( words.at(1) );
        } else {
            continue;
        }
        valueList.insert( words.at(1), words );
    }
    insert("CONCRETE", clist);
    insert("STEEL", slist);
    insert("REINFORCE", rlist);
}
} // namespace post3dapp
