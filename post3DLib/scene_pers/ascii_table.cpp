
#include <QtGui>
#include "qmath.h"

#include "ascii_table.h"
namespace post3dapp{
void AsciiTable::populateList()
{
    QString filename(":/text/ascii_xy.txt");
    QFile file(filename);
    file.open(QIODevice::ReadOnly);

    QTextStream in(&file);
    QString line;

    while (!in.atEnd()) {
        line = in.readLine();
        QChar chr = line.at(0);
        QStringList strlist = line.mid(3).split(',');
        QList<int> nlist;
        Q_FOREACH (QString str, strlist) nlist.append(str.toInt());

        QList<IXYLINE> linelist;
        for (int i = 0; i < nlist.count() ; i++) {
            int inum = nlist.at(i);
            int ix = (inum % 10000) / 100;
            int iy = inum % 100;
            if ( inum > 10000 ) {
                linelist.last().x2 = ix;
                linelist.last().y2 = iy;
            } else if (i != 0) {
                linelist.removeLast();
            }
            IXYLINE xy = {ix, iy, 0, 0};
            if ( i != nlist.count() - 1 ) linelist.append(xy);
        }

        mojiTable.insert(chr, linelist);
    }
}

QList<RXYLINE> AsciiTable::stringToXY(const QString &str, qreal ww, qreal hh,
                                      qreal o_x, qreal o_y, qreal angle)
{

    qreal cc = qCos(angle / 180.*M_PI);
    qreal ss = qSin(angle / 180.*M_PI);
    qreal shift_x, shift_y;

    QList<RXYLINE> rlinelist;

    for ( int i = 0 ; i < str.count() ; i++ ) {
        QChar chr = str.at(i);
        shift_x = o_x + ww * cc * qreal(i);
        shift_y = o_y + hh * ss * qreal(i);

        if ( mojiTable.contains(chr) ) {
            QList<IXYLINE> linelist = mojiTable.value(chr);
            Q_FOREACH (IXYLINE line, linelist) {
                qreal xx1 = qreal(line.x1) / 90.*ww;
                qreal yy1 = qreal(line.y1) / 90.*hh;
                qreal xx2 = qreal(line.x2) / 90.*ww;
                qreal yy2 = qreal(line.y2) / 90.*hh;
                RXYLINE rline;
                rline.x1 = (cc * xx1 - ss * yy1) + shift_x;
                rline.y1 = (ss * xx1 + cc * yy1) + shift_y;
                rline.x2 = (cc * xx2 - ss * yy2) + shift_x;
                rline.y2 = (ss * xx2 + cc * yy2) + shift_y;
                rlinelist.append(rline);
            }
        }
    }

    return rlinelist;
}
} // namespace post3dapp
