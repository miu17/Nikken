#ifndef CALCANYPOINTSTRESS_H
#define CALCANYPOINTSTRESS_H
#include <QtCore>

namespace post3dapp{

class calcAnyPointStress
{
public:
    calcAnyPointStress();
    static QVector<double> calcMemberForce(const QUuid &member_id, const int &iPos,const qreal &xPos, const QString &combi, const int &step);


    static void vecToArray(double*,QVector<qreal>);
    static void vecToArray(double*,QVector<QVector<qreal>>);
    static void vecToArray(double*,QVector<QVector<QVector<qreal>>>);
    static void vecToArray(double*,QVector<QVector<QVector<QVector<qreal>>>>);
    static void vecToArray(int*,QVector<int>);
    static void vecToArray(int*,QVector<QVector<int>>);
    static void vecToArray(int*,QVector<QVector<QVector<int>>>);
    static void vecToArray(int*,QVector<QVector<QVector<QVector<int>>>>);

private:
    static void readData(QString modelname);
    static void deleteData();
    static QString currentfilename;
//    static QString currentmodelname;
    static QHash<QString,int*> nlhmmList ;
    static QHash<QString,int*> ilhmmList ;
    static QHash<QString,double*> dlhmmList;
    static QHash<QString,int*> lndlhList ;
    static QHash<QString,int*> nlemmList ;
    static QHash<QString,int*> ilemmList ;
    static QHash<QString,double*> dlemmList ;
    static QHash<QString,int*> lndleList ;
    static QHash<QString,int*> ifpmmList ;
    static QHash<QString,double*> debmmList ;
    static QHash<QString,int*> indmList ;
    static QHash<QString,double*> evmemList;
};
}
#endif // CALCANYPOINTSTRESS_H
