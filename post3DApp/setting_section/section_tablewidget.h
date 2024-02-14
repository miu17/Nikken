#ifndef SECTION_TABLEWIDGET_H
#define SECTION_TABLEWIDGET_H

#include "customtable_widget.h"
#include "section_tablemodel.h"
namespace post3dapp{
struct SectionValues;
class SectionDataType;
class SectionLineButtons;
class SectionLineDraw;
class SectionLineCalculate;
class SectionTableView;

class SectionTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    SectionTableWidget( const SectionDataType &type, QWidget *parent = 0 );

    virtual QString getName() const{ return QString();};
    void modelUpload() override;
    void modelRewind() override;

    bool readTextFile ( QTextStream &infile, bool flg) override
    {
        if ( !myTableModel->readListTextData(infile, flg) ) return false;

        emit sectionItemChanged();
        slotChangeCurrentRow(0);
        undoStack->clear();
        return true;
    }
    virtual void recalcDefaultValue() {};

    SectionDataType getSectionDataType() const
    {
        return mySectionType;
    }
    QList<bool> getFilterColumns() const
    {
        return filter_columns;
    }

    bool checkBinaryFile(QDataStream &) const override;
    void readBinaryFile(QDataStream &) override;
    void writeBinaryFile(QDataStream &) override;

    SectionValues getMemberSection(int row) const;
    QList<SectionValues> getSectionList() const;

    virtual QList<bool> getDefaultFilters() const = 0;

protected:

public slots:

    void slotManageTableData(const QModelIndex &, const QModelIndex &);
    void slotLineViewChange(int);
    void setDataMyModel(int, int, const QString &);
    void slotChangeCurrentRow(int);
    void changeCurrentView();
    void changeCurrentViewBack();

    void slotChangeTableRowCount(int) override;
    void slotChangeSpinBoxValue(int) override;
    void setFilterColumns(const QList<bool> &);
    void setTableViewBorderColumns(const SectionDataType &, const QList<bool> &);

    void slotDeleteLine() override;
    void slotEditItem(const QModelIndex &, const QVariant &) override;
    void paste() override;
    void del() override;

signals:

    void filterColumnsChanged(const QList<bool> &);
    void sectionItemChanged();

protected:

    SectionDataType mySectionType;

    SectionLineButtons *lineButtons;
    SectionLineDraw *lineDraw;
    SectionLineCalculate *lineCalc;

    SectionTableView *tableView_1;
    SectionTableView *tableView_2;
    SectionTableView *tableView_3;

    QList<bool> filter_columns;
    virtual void createModels() override;
    virtual void createViews() override;
    virtual void createWidgets() override {}

    QWidget *createTableLabel( CustomTableView *, const QString & );

    void setLineButtonsSpinBox();
    void setDefaultValuePatch(int, int, const QVariant &);


    void pasteSectionData(bool, bool, bool, bool);
    void deleteSectionData(bool, bool, bool, bool);
    void editSectionData(const QModelIndex &, const QVariant &, bool, bool, bool, bool);

    void inputStandardValuesS(const QModelIndex &, QModelIndexList &, QList<QVariant> & );
    void inputStandardValuesRC(const QModelIndex &);
    void inputStandardValuesBASE(const QModelIndex &, QModelIndexList &, QList<QVariant> & );
    void inputStandardValuesISO(const QModelIndex &, QModelIndexList &, QList<QVariant> & );


};
} // namespace post3dapp
#endif // SECTION_WIDGET_H
