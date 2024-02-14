#include "abstractlinedata.h"

#include "textdataunit.h"
namespace post3dapp{

bool AbstractLineData::hasFormatError(const QString &text) const{
    for(auto unit : dataUnits){
        QString word = text.mid(unit.startCol - 1, unit.endCol - unit.startCol + 1);
        if (!unit.match(word))
            return true;
    }
    if (text.trimmed().length() > dataUnits.last().endCol){
        // *Planと*PlanInputのようなもののためのチェック
        return true;
    }

    return false;
}

QString AbstractLineData::getStatusBar(int col) const{
    for (int i = 0; i < dataUnits.count(); i++){
        TextDataUnit unit = dataUnits.at(i);
        if (unit.startCol <= col && col <= unit.endCol){
            return "[" + QString::number(unit.startCol).rightJustified(3) + "-" + QString::number(unit.endCol).rightJustified(3) +  "]" + unit.statusBar;
        }
    }
    return QString();
}
} // namespace post3dapp
