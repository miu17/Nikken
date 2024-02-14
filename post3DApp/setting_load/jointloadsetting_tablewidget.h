#ifndef JOINTLOADSETTINGTABLEWIDGET_H
#define JOINTLOADSETTINGTABLEWIDGET_H

#include "customtable_widget.h"

namespace post3dapp{
struct JOINTLOADVALUES;
class CustomTableDataType;
class CustomTableModel;

class JointLoadSettingTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    JointLoadSettingTableWidget( const CustomTableDataType &type, QWidget *parent = 0 );

    void reflectUnifiedData() override;
    QList<JOINTLOADVALUES> getValueList() const;
    QStringList checkLoadTypes() const;

    void modelUpload() override;
    void modelRewind() override;


    bool readTextFile ( QTextStream &infile, bool flg) override;

    QStringList getSectionNameList() const;

protected:

public slots:

    void slotDeleteLine() override;
    void slotManageTableData(const QModelIndex &, const QModelIndex &);

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
