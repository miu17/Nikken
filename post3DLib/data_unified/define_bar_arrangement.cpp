#include "define_bar_arrangement.h"

#include <QtGui>
namespace post3dapp{
/////配筋読み込み/////

bool ReadBarArrangement::readBarsColumnCirc(const QString &barstr, BarArrangement *retlists)
{
    retlists->numsum.clear();
    retlists->numleft.clear();
    retlists->numright.clear();
    if (QRegExp("[0-9]*").exactMatch(barstr)) { //正規表現
        retlists->numsum << barstr.toInt();
        retlists->numleft << 0;
        retlists->numright << 0;
    } else {
        return false;
    }
    return true;
}

bool ReadBarArrangement::readBarsColumn(const QString &barstr, BarArrangement *retlists)
{
//5C1:0,1C0:0のような入力も許容。x、y方向ともに1C10/0と入れた場合、左上に１本の入力。両方1C00と入れた場合各辺の中央に１本ずつの入力。
//xが1C10/0、yが0だとエラー。barErrorCheck。
    retlists->numsum.clear();
    retlists->numleft.clear();
    retlists->numright.clear();
    QStringList lines = barstr.split("/");
    if     (lines.size() == 1) {
        lines << lines[0];   //1列分の入力→逆側も同じと解釈
    } else if (lines.size() != 2) {
        return false;   //列数が１でも２でもない場合はエラー：：柱の場合
    }
    for (int i = 0; i < lines.size(); ++i) {
        if (QRegExp("[1-9][0-9]*C[1-9][0-9]*:[1-9][0-9]*").exactMatch(lines[i])) { //正規表現１
            QStringList tmp = lines[i].split("C");
            retlists->numsum << tmp[0].toInt();
            QStringList rightLeft = tmp[1].split(":");
            retlists->numleft << (rightLeft[0]).toInt();
            retlists->numright << (rightLeft[1]).toInt();
            if (retlists->numsum[i] < retlists->numleft[i] + retlists->numright[i]) {
                return false;   //全鉄筋数より寄せ筋のほうが多い場合エラー
            }
        } else if (QRegExp("[1-9][0-9]*").exactMatch(lines[i]) || lines[i] == "0") { //正規表現２
            retlists->numsum << lines[i].toInt();
            if (retlists->numsum[i] >= 2) {
                retlists->numleft << 1;
                retlists->numright << 1;
            } else {
                retlists->numleft << 0;
                retlists->numright << 0;
            }
        } else {
            return false;
        }
    }
    return true;
}

bool ReadBarArrangement::readBarsGirder(const QString &barstr, BarArrangement *retlists)
{
//    QList<int> numsum,numleft,numright;

    retlists->numsum.clear();
    retlists->numleft.clear();
    retlists->numright.clear();
    QStringList lines = barstr.split("/");
    for (int i = 0; i < lines.size(); ++i) {
        if (QRegExp("[1-9][0-9]*C[1-9][0-9]*:[1-9][0-9]*").exactMatch(lines[i])) { //正規表現１
            QStringList tmp = lines[i].split("C");
            retlists->numsum << tmp[0].toInt();
            QStringList rightLeft = tmp[1].split(":");
            retlists->numleft << (rightLeft[0]).toInt();
            retlists->numright << (rightLeft[1]).toInt();
            if (retlists->numsum[i] < retlists->numleft[i] + retlists->numright[i]) {
                return false;   //全鉄筋数より寄せ筋のほうが多い場合エラー
            }
        } else if (QRegExp("[1-9][0-9]*").exactMatch(lines[i]) || lines[i] == "0") { //正規表現２
            retlists->numsum << lines[i].toInt();
            if (retlists->numsum[i] >= 2) {
                retlists->numleft << 1;
                retlists->numright << 1;
            } else {
                retlists->numleft << 0;
                retlists->numright << 0;
            }
        } else {
            return false;
        }
    }
    return true;
}

bool ReadBarArrangement::barErrorCheckColumn(const BarArrangement &bar1,
                                             const BarArrangement &bar2)
{
    //リストサイズのチェック
    if (bar1.numsum.size() != 2 || bar1.numleft.size() != 2 || bar1.numright.size() != 2) {
        return false;
    }
    if (bar2.numsum.size() != 2 || bar2.numleft.size() != 2 || bar2.numright.size() != 2) {
        return false;
    }

    //各列の配筋数チェック
    if (bar1.numsum[0] < bar1.numleft[0] + bar1.numright[0]) {
        return false;
    }
    if (bar1.numsum[1] < bar1.numleft[1] + bar1.numright[1]) {
        return false;
    }
    if (bar2.numsum[0] < bar2.numleft[0] + bar2.numright[0]) {
        return false;
    }
    if (bar2.numsum[1] < bar2.numleft[1] + bar2.numright[1]) {
        return false;
    }

    //四隅のチェック
    if (bar1.numleft[0] == 0  && bar2.numleft[0] != 0 ) {
        return false;
    }
    if (bar1.numright[0] == 0 && bar2.numleft[1] != 0 ) {
        return false;
    }
    if (bar1.numleft[1] == 0  && bar2.numright[0] != 0) {
        return false;
    }
    if (bar1.numright[1] == 0 && bar2.numright[1] != 0) {
        return false;
    }
    if (bar1.numleft[0] != 0  && bar2.numleft[0] == 0 ) {
        return false;
    }
    if (bar1.numright[0] != 0 && bar2.numleft[1] == 0 ) {
        return false;
    }
    if (bar1.numleft[1] != 0  && bar2.numright[0] == 0) {
        return false;
    }
    if (bar1.numright[1] != 0 && bar2.numright[1] == 0) {
        return false;
    }

    return true;
}

bool ReadBarArrangement::barErrorCheckGirder(const BarArrangement &bar1,
                                             const BarArrangement &bar2)
{
    //リストサイズのチェック
//    if(bar1.numsum.size()==0 || bar1.numleft.size()==0 || bar1.numright.size()==0){return -1;}
//    if(bar2.numsum.size()==0 || bar2.numleft.size()==0 || bar2.numright.size()==0){return -1;}
    //各列のチェック
    for (int i = 0; i < bar1.size(); ++i) {
        if (bar1.numsum[i] < bar1.numleft[i] + bar1.numright[i]) {
            return false;
        }
    }
    for (int i = 0; i < bar2.size(); ++i) {
        if (bar2.numsum[i] < bar2.numleft[i] + bar2.numright[i]) {
            return false;
        }
    }
    return true;
}
} // namespace post3dapp
