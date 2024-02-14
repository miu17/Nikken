#ifndef ANALYSISRANGETABLEWIDGET_H
#define ANALYSISRANGETABLEWIDGET_H

#include "customtable_widget.h"
#include "define_analysisrangetable.h"
#include "analysisrange_tablemodel.h"

#include "combobox_delegate.h"
namespace post3dapp{
class JointData;
class MemberData;
class AnalysisRangeTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:
    AnalysisRangeTableWidget( const CustomTableDataType &type, QWidget* parent=0 );
    int currentRow() const;
    QList<GroupSetting> getData() const;
    virtual bool checkData() const = 0;
protected:

public slots:

    void slotDeleteLine() override;
    void slotManageTableData(const QModelIndex &, const QModelIndex &, const QVector<int> &roles = QVector<int>());
    void slotCurrentRowChanged(int);

signals:

    void signalTableItemChanged();
    void signalCurrentRowChanged(int);

protected:
    void createWidgets() override;

    QAction *allocateAction;
    QAction *additionallyAllocateAction;
    QAction *confirmAction;
};

class AnalysisJointRangeTableWidget : public AnalysisRangeTableWidget
{
    Q_OBJECT
public:
    AnalysisJointRangeTableWidget( const CustomTableDataType &type, const QList<GroupSetting>&, QWidget* parent=0 );

    void setSelectedItems( const QList<JointData*>&);
    QList<JointData*> getSelectedJoints() const;
    bool checkData() const override;
    void modelRewind(const QList<GroupSetting>&);

signals:
    void signalAllocateCurrentJoints();
    void signalAdditionallyAllocateCurrentJoints();
    void signalConfirmTableJoints();

protected:
    void createModels() override;
    void createViews() override;
    void createActions() override;
};

class AnalysisMemberRangeTableWidget : public AnalysisRangeTableWidget
{
    Q_OBJECT
public:
    AnalysisMemberRangeTableWidget( const CustomTableDataType &type, const QList<GroupSetting>&, QWidget* parent=0 );
    void setSelectedItems(const QList<MemberData*>& );
    QList<MemberData*> getSelectedMembers() const;
    bool checkData() const override;
    void modelRewind(const QList<GroupSetting>&);

signals:
    void signalAllocateCurrentMembers();
    void signalAdditionallyAllocateCurrentMembers();
    void signalConfirmTableMembers();

protected:
    void createModels() override;
    void createViews() override;
    void createActions() override;
};

} // namespace post3dapp

#endif
