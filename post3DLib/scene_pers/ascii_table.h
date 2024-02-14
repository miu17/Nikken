#ifndef ASCIITABLE_H
#define ASCIITABLE_H

#include <QtGlobal>
#include <QHash>
#include <QList>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif
namespace post3dapp{
typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} IXYLINE;

typedef struct {
    qreal x1;
    qreal y1;
    qreal x2;
    qreal y2;
} RXYLINE;

class POST3D_CLASS_DLLSPEC AsciiTable
{

public:

    AsciiTable()
    {
        populateList();
    }
    ~AsciiTable(){}
    QList<RXYLINE> stringToXY(const QString &, qreal ww, qreal hh,
                              qreal o_x = 0.0, qreal o_y = 0.0, qreal angle = 0.0);

private:

    QHash< QChar, QList<IXYLINE> > mojiTable;

    void populateList();

};
} // namespace post3dapp
#endif
