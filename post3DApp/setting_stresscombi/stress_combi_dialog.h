#ifndef STRESSCOMBIWIDGET_H
#define STRESSCOMBIWIDGET_H

#include <QtWidgets>

class QPushButton;
namespace post3dapp{
class StressCombiDialog : public QDialog
{
    Q_OBJECT

public:
    StressCombiDialog( QWidget *parent = nullptr );

protected:

public slots:

private slots:
    void confirm();
    void cancel();

    void addCombination();
    void deleteCombination();
    void addStressCase();
    void deleteStressCase();

    void changeStackWidget(int row,int col);

    void checkCase(int,int);

signals:
    void statusChanged();

private:
    void removeSubWidget(int);
    void createLayout();
    void insertSubWidget(int index);
    bool existData(QString);
    QStringList defaultCases();
    void setData();
    void setOneData(QString,int,QList<qreal>,QStringList);

    QTableWidget* table1;
    QStackedLayout* stack;
    QList<QTableWidget*> combitables;
    QList<QComboBox*> stepcase;
    QStringList existCase;

};
} // namespace post3dapp

#endif // STRESSCOMBIWIDGET_H
