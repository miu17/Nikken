#include "standard_section.h"
#include <QDebug>
namespace post3dapp{
QStringList StandardSectionTable::getSectionFileList(listType type) const
{
    switch (type) {
    case listType::STEEL:
        return steelFileList;
    case listType::COLUMNSRCSTEEL:
        return columnSrcSteelFileList;
    case listType::COLUMNSRCSTEELT:
        return columnSrcSteelTFileList;
    case listType::CFT:
        return cftSteelFileList;
    case listType::BASE:
        return baseFileList;
    case listType::ISOLATOR:
        return isolatorFileList;
    default:
        return steelFileList;
    }
}

QStringList StandardSectionTable::getSectionNameList(const QString &filename) const
{
    // 鉄骨規格断面は表示の際"-R"部分を削除
    if ( steelFileList.contains(filename) ) {
        QStringList strlist;
        Q_FOREACH ( QString str, this->value(filename) ) {
            QStringList words = str.split('-');
            if ( words.count() < 3 ) {
                strlist << QString();
                continue;
            }
            strlist << words.at(0) + "-" + words.at(1);
        }
        return strlist;
    }
    return this->value(filename);
}

bool  StandardSectionTable::existsSectionName( const QString &sectionname ) const
{
    return valueList.contains(sectionname);
}

QString StandardSectionTable::getSteelSectionValue(const QString &nm, SteelFormat fm) const
{
    if ( !valueList.contains(nm) ) return QString();
    int ii = static_cast<int>(fm);
    QStringList line = valueList.value(nm);
    return ( ii < line.count() ) ? line.at(ii) : QString();
}

QString StandardSectionTable::getBaseSectionValue(const QString &nm, BaseFormat fm) const
{
    if ( !valueList.contains(nm) ) return QString();
    int ii = static_cast<int>(fm);
    QStringList line = valueList.value(nm);
    return ( ii < line.count() ) ? line.at(ii) : QString();
}

QString StandardSectionTable::getIsolatorSectionValue(const QString &nm, IsolatorFormat fm) const
{
    if ( !valueList.contains(nm) ) return QString();
    int ii = static_cast<int>(fm);
    QStringList line = valueList.value(nm);
    return ( ii < line.count() ) ? line.at(ii) : QString();
}

QString StandardSectionTable::steelSectionName( const QString &bname, const QString &sname ) const
{
    if ( !contains(bname) ) return QString();
    QStringList slist = value(bname);
    Q_FOREACH ( QString str, slist ) {
        if ( str.contains(sname) ) return str;
    }
    return QString();
}

void StandardSectionTable::polulateList()
{
    Q_INIT_RESOURCE(resource);
    QDir dir(":/post3DLib/include_files/standard_steel");
    QFileInfoList ilist = dir.entryInfoList(QDir::Files);
    for (int i = 0; i < ilist.count() ; i++) {
        if ( ilist.at(i).suffix() != "csv" ) continue;
        QString filename = ilist.at(i).filePath();
        QFile file(filename);
        if ( !file.open(QIODevice::ReadOnly) ) continue;
        QTextStream in(&file);
        QStringList words, sclist;
        sclist << "";
        bool hasCftSteel = false;
        bool hasColumnSrcSteelT = false;
        while ( !in.atEnd() ) {
            words = in.readLine().split(',', QString::KeepEmptyParts);
            if ( words.count() < 3 ) continue;
            QString startCharacter = words.at(0);
            if ( startCharacter == "#" || startCharacter.isEmpty() ) continue;
            QString str = startCharacter + QString("-") + words.at(1) + QString("-") + words.at(2);
            valueList.insert(str, words);
            sclist.append(str);
            if (startCharacter == "BX" || startCharacter == "P"){
                hasCftSteel = true;
            }
            if (startCharacter == "H"){
                hasColumnSrcSteelT = true;
            }

        }
        insert(ilist.at(i).baseName(), sclist);
        steelFileList.append(ilist.at(i).baseName());
        columnSrcSteelFileList.append(ilist.at(i).baseName());
        if (hasCftSteel){
            cftSteelFileList.append(ilist.at(i).baseName());
        }
        if (hasColumnSrcSteelT){
            columnSrcSteelTFileList.append(ilist.at(i).baseName());
        }
    }

    dir = QDir(":/standard_base");
    ilist = dir.entryInfoList(QDir::Files);
    for (int i = 0; i < ilist.count() ; i++) {
        if ( ilist.at(i).suffix() != "csv" ) continue;
        QString filename = ilist.at(i).filePath();
        QFile file(filename);
        if ( !file.open(QIODevice::ReadOnly) ) continue;
        QTextStream in(&file);
        QStringList words, sclist;
        sclist << "";
        while ( !in.atEnd() ) {
            words = in.readLine().split(',');
            if ( words.count() < 1 ) continue;
            if ( words.at(0) == "#" || words.at(0).isEmpty() ) continue;
            valueList.insert(words.at(0), words);
            sclist.append(words.at(0));
        }
        insert(ilist.at(i).baseName(), sclist);
        baseFileList.append(ilist.at(i).baseName());
    }

    dir = QDir(":/standard_isolator");
    ilist = QDir(dir).entryInfoList(QDir::Files);
    for (int i = 0; i < ilist.count() ; i++) {
        if ( ilist.at(i).suffix() != "csv" ) continue;
        QString filename = ilist.at(i).filePath();
        QFile file(filename);
        if ( !file.open(QIODevice::ReadOnly) ) continue;
        QTextStream in(&file);
        QStringList words, sclist;
        sclist << "";
        while ( !in.atEnd() ) {
            words = in.readLine().split(',');
            if ( words.count() < 1 ) continue;
            if ( words.at(0) == "#" || words.at(0).isEmpty() ) continue;
            valueList.insert(words.at(0), words);
            sclist.append(words.at(0));
        }
        insert(ilist.at(i).baseName(), sclist);
        isolatorFileList.append(ilist.at(i).baseName());
    }

}
} // namespace post3dapp
