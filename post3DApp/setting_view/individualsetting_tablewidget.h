#ifndef INDIVIDUALSETTINGTABLEWIDGET_H
#define INDIVIDUALSETTINGTABLEWIDGET_H

#include "customtable_widget.h"
#include "customtable_model.h"

namespace post3dapp{
struct COLORBRUSHSETTING;
class IndividualSettingTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    IndividualSettingTableWidget( const CustomTableDataType &type, QWidget *parent = 0 );

    QList<QList<QVariant> > getIndividualData() const;
    QMap<QString, COLORBRUSHSETTING> getColorTableData() const;
    void setIndividualData( const QList<QList<QVariant> > & );

public slots:

signals:

protected:

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

};


class IndividualSettingTableModel : public CustomTableModel
{
    Q_OBJECT

public:

    IndividualSettingTableModel( const CustomTableDataType &type, QObject *parent = 0 )
        : CustomTableModel( type, parent ) { }

    QVariant data(const QModelIndex &index, int role) const override;

};
} // namespace post3dapp



#endif
