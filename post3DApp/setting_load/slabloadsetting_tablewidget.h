#ifndef SLABLOADSETTINGTABLEWIDGET_H
#define SLABLOADSETTINGTABLEWIDGET_H

#include "customtable_widget.h"
namespace post3dapp{
struct SLABLOADVALUES;
class CustomTableDataType;
class CustomTableModel;

class SlabLoadSettingTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    SlabLoadSettingTableWidget( const CustomTableDataType &type, QWidget *parent = nullptr );

    void reflectUnifiedData() override;
    QList<SLABLOADVALUES> getValueList() const;
    QStringList checkOverlapNames() const;

    void modelUpload() override;
    void modelRewind() override;


    bool readTextFile ( QTextStream &infile, bool flg) override;

    QStringList getSectionNameList() const;

protected:

public slots:

    void slotDeleteLine() override;
    void slotManageTableData(const QModelIndex &, const QModelIndex &);

//    void paste();

signals:

    void sectionItemChanged();

protected:

    CustomTableModel *localModel;

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};

} // namespace post3dapp
#endif
