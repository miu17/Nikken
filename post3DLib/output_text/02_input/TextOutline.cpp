#include "abstracttext.h"

#include <io.h>
#include <stdio.h>
#include <QTextStream>
#include "fortranFunc.h"

namespace post3dapp{
QPair<QPair<QString, QString>, QString> TextOutline::write(const FixedData&) const{
    QString second;
    return QPair<QPair<QString, QString>, QString>{QPair<QString, QString>{getCommand(), getJapanese()}, second};
}
} // namespace post3dapp
