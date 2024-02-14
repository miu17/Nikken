#ifndef FLOORSETTINGTABLEWIDGET_H
#define FLOORSETTINGTABLEWIDGET_H

#include "customtable_widget.h"

class QTextStream;

namespace post3dapp{
struct FLOORVALUES;
class CustomTableDataType;

class FloorSettingTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    FloorSettingTableWidget( const CustomTableDataType &type, QWidget *parent = nullptr );

    void reflectUnifiedData() override;

    bool readTextFile ( QTextStream &infile, bool flg) override;

    void writeTextFile ( QTextStream &outfile, bool writeUuid) ;

    QList<FLOORVALUES> getFloorList(bool isAll = true) const;
    bool existResisterRow();

    void modelUpload() override;
    void modelEdit() override;
    void modelRewind() override;

protected:

public slots:

    void slotDeleteLine() override;
    void slotManageTableData(const QModelIndex &, const QModelIndex &);

signals:

    void floorItemChanged();

protected:

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

    bool warningLineDelete();

};
} // namespace post3dapp
#endif
