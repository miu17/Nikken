#ifndef ANALYISISCONDITIONGENERALWIDGET_H
#define ANALYISISCONDITIONGENERALWIDGET_H

#include <QMap>
#include <QWidget>

namespace post3dapp{
class GeneralLineCondition;

class AnalyisisConditionGeneralWidget : public QWidget
{
    Q_OBJECT
public:
    AnalyisisConditionGeneralWidget(QWidget *parent = 0);
    QString write() const;
    void read(const QMap<int, QString>&);
    void setDefault();
private:
    QMap<int, QSharedPointer<GeneralLineCondition>> layoutMap;
    int createLayout(const int, const double, QSharedPointer<GeneralLineCondition>);
signals:
    void dataChanged();
};

} // namespace post3dapp

#endif // ANALYISISCONDITIONGENERALWIDGET_H
