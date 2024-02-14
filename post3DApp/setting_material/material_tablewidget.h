#ifndef MATERIAL_WIDGET_H
#define MATERIAL_WIDGET_H

#include "customtable_widget.h"
#include "define_materialtable.h"
namespace post3dapp{
struct MATERIALVALUES;
class MaterialTableView;

class MaterialTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    MaterialTableWidget( const MaterialDataType &type, QWidget *parent = 0 );

    void modelUpload() override;
    void modelRewind() override;

    bool readTextFile ( QTextStream &infile, bool flg) override;

    void reflectUnifiedData() override;

    MaterialDataType getDataType() const;
    QList<bool> getFilterColumns() const
    {
        return filter_columns;
    }
    int rowCount() const;
    QList<MATERIALVALUES> getMaterialList() const;

protected:

public slots:

    void slotManageTableData(const QModelIndex &, const QModelIndex &);

//    void slotTableViewReset();
    void changeCurrentView();
    void changeCurrentViewBack();

    void setFilterColumns(const QList<bool> &);
    void setTableViewBorderColumns(const MaterialDataType &, const QList<bool> &);


    void slotDeleteLine() override;
    void slotEditItem(const QModelIndex &, const QVariant &) override;
    void paste() override;
    void del() override;

signals:
    void filterColumnsChanged(const QList<bool> &);
//edit flag
//    void signalDataEditing();
    void sectionItemChanged();

protected:

    MaterialDataType myMaterialType;
    MaterialTableView *tableView_1;

    QList<bool> filter_columns;

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

    QWidget *createTableLabel( CustomTableView *, const QString & );

    void setDefaultValuePatch(int, int, const QVariant &);


    void pasteSectionData();
    void deleteSectionData();
    void editSectionData(const QModelIndex &, const QVariant &);

    void inputStandardValues(const QModelIndex &, QModelIndexList &, QList<QVariant> & );

};
} // namespace post3dapp
#endif
