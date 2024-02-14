#ifndef DEFINE_BAR_ARRANGEMENT_H
#define DEFINE_BAR_ARRANGEMENT_H

#include <QList>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{
struct POST3D_CLASS_DLLSPEC BarArrangement { //鉄筋・寄せ筋（多段筋→List）
    explicit BarArrangement(
        const QList<int> &numsum_ = QList<int>(),
        const QList<int> &numleft_ = QList<int>(),
        const QList<int> &numright_ = QList<int>() )
        : numsum(numsum_), numleft(numleft_), numright(numright_) {}

    int size() const
    {
        return numsum.count();
    }

    QString text() const{
        if(numsum.count() == 0)
            return QString();

        QString text;
        for(int i = 0; i < numsum.count(); i++){
            if (i != 0)
                text += "/";
            text += QString::number(numsum.at(i));
            if ((numleft.count() == 0 || numleft.at(i) == 0) && (numright.count() == 0 || numright.at(i) == 0))
                continue;
            if (numleft.at(i) == 1 && numright.at(i) == 1)
                continue;
            text += "C" +  QString::number(numleft.at(i)) +  ":" + QString::number(numright.at(i));
        }
        return text;
    }

    QString reflectColumnText() const{
        if(numsum.count() == 0)
            return QString();

        QString text;
        for(int i = 0; i < numsum.count(); i++){
            if (i != 0){
                if (numsum.count() == 2 && numsum[0] == numsum[1] && numleft[0] && numleft[1] && numright[0] && numright[1])
                    return text;
                text += "/";
            }
            text += QString::number(numsum.at(i));
            if ((numleft.count() == 0 || numleft.at(i) == 0) && (numright.count() == 0 || numright.at(i) == 0))
                continue;
            if (numleft.at(i) == 1 && numright.at(i) == 1)
                continue;
            text += "C" +  QString::number(numleft.at(i)) +  ":" + QString::number(numright.at(i));
        }
        return text;
    }

    QList<int> numsum;
    QList<int> numleft;     //左寄せ　or 上寄せ
    QList<int> numright;    //右寄せ　or 下寄せ
};

class POST3D_CLASS_DLLSPEC ReadBarArrangement
{
private:
public:

    static bool readBarsColumnCirc(const QString &barstr, BarArrangement *retlists);
    static bool readBarsColumn(const QString &barstr, BarArrangement *retlists);
    static bool readBarsGirder(const QString &barstr, BarArrangement *retlists);
    static bool barErrorCheckColumn(const BarArrangement &bar1,
                                    const BarArrangement &bar2);
    static bool barErrorCheckGirder(const BarArrangement &bar1,
                                    const BarArrangement &bar2);
};
} // namespace post3dapp
#endif // DEFINE_BAR_ARRANGEMENT_H
