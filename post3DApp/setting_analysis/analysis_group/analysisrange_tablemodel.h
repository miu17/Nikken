#ifndef ANALYSISRANGETABLEMODEL_H
#define ANALYSISRANGETABLEMODEL_H

#include "customtable_model.h"
namespace post3dapp{

class AnalysisRangeTableModel : public CustomTableModel{
    Q_OBJECT
public:
    AnalysisRangeTableModel(const CustomTableDataType &type, QObject *parent = 0);
    virtual QList<GroupSetting> getData() const = 0;
    virtual void modelRewind(const QList<GroupSetting>&) = 0;
};

class AnalysisJointRangeTableModel : public AnalysisRangeTableModel
{
    Q_OBJECT

public:

    AnalysisJointRangeTableModel(const CustomTableDataType &type, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const override;
    bool isDisableItem(const QModelIndex&) const override;
    QList<GroupSetting> getData() const override;
    void modelRewind(const QList<GroupSetting>&) override;
private:
    QString getJointNumbersString(const QList<QVariant>&) const;
};

class AnalysisMemberRangeTableModel : public AnalysisRangeTableModel
{
    Q_OBJECT

public:
    AnalysisMemberRangeTableModel(const CustomTableDataType &type, QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const override;
    bool isDisableItem(const QModelIndex&) const override;
    QList<GroupSetting> getData() const override;
    void modelRewind(const QList<GroupSetting>&) override;
private:
    QString getDataTypeString(const QList<QVariant>&) const;
};

} // namespace post3dapp
#endif
