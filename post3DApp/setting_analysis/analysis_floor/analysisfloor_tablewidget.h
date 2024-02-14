#ifndef ANALYSISFLOORTABLEWIDGET_H
#define ANALYSISFLOORTABLEWIDGET_H

#include "customtable_widget.h"
namespace post3dapp{
class AnalysisFloorTableModel;
class AnalysisFloorTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    AnalysisFloorTableWidget( const CustomTableDataType &type, QWidget *parent = 0 );

    void reflectUnifiedData() override;
    bool checkAnalysisFloor() const;
    QList<FLOORVALUES> getAnalysisFloorList() const;

    void modelUpload() override;
//    virtual void modelEdit();
    void modelRewind() override;

    void updateFloorTable();

protected:

public slots:

    void slotManageTableData(const QModelIndex &, const QModelIndex &);

    void paste() override;
    void del() override;

signals:

    void floorItemChanged();

protected:

    void createActions() override;
    void createModels() override;
    void createViews() override;
    void createWidgets() override;
};
} // namespace post3dapp
#endif
