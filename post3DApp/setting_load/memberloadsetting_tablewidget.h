#ifndef MEMBERLOADSETTINGTABLEWIDGET_H
#define MEMBERLOADSETTINGTABLEWIDGET_H

#include "customtable_widget.h"

namespace post3dapp{
class CustomTableDataType;
class CustomTableModel;

class MemberLoadSettingTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    MemberLoadSettingTableWidget( const CustomTableDataType &type, QWidget *parent = 0 );

    void reflectUnifiedData() override;
    QList<MEMBERLOADVALUES> getValueList() const;
    QStringList checkLoadTypes() const;

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
