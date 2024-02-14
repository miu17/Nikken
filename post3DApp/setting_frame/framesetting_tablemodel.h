#ifndef FRAMESETTINGTABLEMODEL_H
#define FRAMESETTINGTABLEMODEL_H

#include "customtable_model.h"
namespace post3dapp{
class FrameSettingTableModel : public CustomTableModel
{
    Q_OBJECT

public:

    FrameSettingTableModel(const CustomTableDataType &type, QObject *parent = 0);
    FrameSettingTableModel(const FrameSettingTableModel&);

    QVariant data(const QModelIndex &index, int role) const override;
    void setDataDirect(const QModelIndex &index, const QVariant &value, bool signalOn = true) override;
    void setRowDataDirect(int row, const QList<QVariant> &list, bool signal = true) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void removeItems(const QModelIndexList &idxlist) override;
    bool removeRows(int row, int count, const QModelIndex &index = QModelIndex()) override;

    bool isBlockedItem(const QModelIndex &) const override;
    bool isRegisterRow(int row) const override;


    bool isGIDRow(int row)
    {
        return ( allItems.at(row, myType.getTaggedColumn("GID")).isValid() );
    }

    int getTaggedColumn(const QString &tag) const
    {
        return myType.getTaggedColumn(tag);
    }

    QList<QVariant> getRowDataOfGID(const QString &gid );
    QStringList getGIDList() const;
    QString getGIDOfRow( int row ) const;
    int getRowOfGID( const QString &gid ) const;
    FRAMEGROUPVALUE getFrameGroupItemOfGID( const QString &gid ) const;
    void setMinValueOfFrameCount(const QString &gid, int value);
    bool hasUUID( const QString &gid )
    {
        return isRegisterRow(getRowOfGID(gid)) ;
    }

//    virtual bool readTextData(QTextStream&, bool overwriteFlag=true);

public slots:

signals:

    void gidRemoved(const QString &);

protected:

    int columnTYPE;
    QHash<QString, int> minValueList;
private:
    bool isInvalidColumn(int, int) const;

};
} // namespace post3dapp
#endif
