#ifndef TYPICALCOLOR_TABLE_H
#define TYPICALCOLOR_TABLE_H

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

#include <QColor>
namespace post3dapp{
class POST3D_CLASS_DLLSPEC TypicalColorTable
{
public:
    QRgb rgb;
    QString name;

    static QColor nameToColor(const QString &);
    static QString colorToName(const QColor &);
    static const int tpColTable_size = 141;
    static const TypicalColorTable tpColTable[tpColTable_size];
};
} // namespace post3dapp
#endif // TYPICALCOLOR_TABLE_H
