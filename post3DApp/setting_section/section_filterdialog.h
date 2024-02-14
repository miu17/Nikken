#ifndef FILTER_DIALOG_H
#define FILTER_DIALOG_H

#include <QDialog>

class QTabWidget;
class QVBoxLayout;
namespace post3dapp{
class SectionDataType;
class SectionTableWidget;
class SectionFilterDialog : public QDialog
{
    Q_OBJECT

public:

    SectionFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);

    void resetFilterColumns(const QHash<QString, QList<bool> > &);

    QHash<QString, QList<bool> > getFilterColumns() const
    {
        return myTempFilter;
    }

protected:
    virtual void createLayout();
    virtual QTabWidget* createTab();
    virtual QVBoxLayout* createSwitchLayout();
    void setFilterColumns(const QHash<QString, QList<bool> > &filter)
    {
        myTempFilter = filter;
    }

private:
    QHash<QString, QList<bool> > myTempFilter; // originalは各TabWidgetがもつ
    QHash<QString, QList<bool> > myTempDefaultFilter;
    QHash<QString, QList<QCheckBox *> > allCheckBoxes;
    QList<SectionDataType> sectionTypeList;
    QStringList filterKeys;


    void setAllCheckBoxes(const QHash<QString, QList<bool> > &filter);
    QHash<QString, QList<bool> > readAllCheckBoxes();

public slots:

    void slot_accept();
    void slot_reject();

    void slot_allOn();
    void slot_allOff();
    void slot_axialBarOn();
    void slot_axialBarOff();
    void slot_rkryOn();
    void slot_rkryOff();
    void slot_steelPositionOn();
    void slot_steelPositionOff();

protected:

    void checkAllColumns(bool);
    void checkAxialBarColumns(bool);
    void checkRkRyColumns(bool);
    void checkSteelPositionColumns(bool);

};
} // namespace post3dapp
#endif // FILTER_DIALOG_H
