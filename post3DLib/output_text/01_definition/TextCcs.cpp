#include "abstracttext.h"

#include <io.h>
#include <stdio.h>
#include <QTextStream>

#include "fortranFunc.h"

namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextCcs::write(const FixedData&) const{
    QString second;
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
/*
TextCcs::TextCcs(const QStringList& list)
    :AbstractTextCallFortran(list){}

const QString TextCcs::Command = u8"*CCS";
const QString TextCcs::Japanese = u8"応力解析ケース";

void TextCcs::CallFortran(const UnifiedOutput&) const{
    int* IPRTYPE = new int;
    int* KSECT = new int;
    char CPRCON[2*30] = "                               dadfas";
    *IPRTYPE = 1;
    *KSECT = 2;

    PR000(IPRTYPE, KSECT, CPRCON);
    delete IPRTYPE;
    delete KSECT;
}

*/

} // namespace post3dapp
