#ifndef GENERALLINECONDITION_H
#define GENERALLINECONDITION_H

#include <QList>
#include <QSharedPointer>
#include <QString>

#ifdef POST3D_LIB
#define POST3D_CLASS_DLLSPEC Q_DECL_EXPORT
#else
#define POST3D_CLASS_DLLSPEC Q_DECL_IMPORT
#endif

namespace post3dapp{

class POST3D_CLASS_DLLSPEC GeneralLineCondition{
public:
    GeneralLineCondition(int _number, QString _keyword, bool _isChoice, int _defaultChoice,
              double _defaultFactor, const QString& _comment, const QList<QString>& _choices = QList<QString>())
        :number(_number), keyword(_keyword), isChoice(_isChoice), defaultChoice(_defaultChoice),
        defaultFactor(_defaultFactor), comment(_comment), choices(_choices), selectedNumber(_defaultChoice), factor(_defaultFactor){}

    const int number;
    const QString keyword;
    bool isChoice;
    const int defaultChoice;
    const double defaultFactor;
    const QString comment;
    const QList<QString> choices;
    void setFactor(const double _factor){factor = _factor;}
    void setSelectedNumber(const int _number){selectedNumber = _number;}
    QString write() const;
    static QSharedPointer<GeneralLineCondition> Create(const QString &);

private:
    int selectedNumber;
    double factor;
};

}


#endif // GENERALLINECONDITION_H
