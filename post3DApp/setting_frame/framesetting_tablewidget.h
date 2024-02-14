#ifndef FRAMESETTINGTABLEWIDGET_H
#define FRAMESETTINGTABLEWIDGET_H

#include "customtable_widget.h"
namespace post3dapp{
class CustomTableDataType;
class FrameSettingSubTableWidget;
class FrameSettingTableModel;
class FrameSettingTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    FrameSettingTableWidget( const CustomTableDataType &type, QWidget *parent = 0 );

    bool checkBinaryFile ( QDataStream &infile) const override;
    void readBinaryFile ( QDataStream &infile) override;
    void writeBinaryFile ( QDataStream &outfile) override;
    void clearData() override;

    void reflectUnifiedData() override;

    bool checkTextFile ( QTextStream &infile ) const override;

    bool readTextFile ( QTextStream &infile, bool flg) override;

    void writeTextFile ( QTextStream &outfile, bool);

    QList<FRAMEGROUPVALUE> getFrameGroupList() const;
    QList<FRAMEPOINTSVALUE> getFramePointList() const;

    bool existResisterRow();

    void modelUpload() override;
    void modelEdit() override;
    void modelRewind() override;
    CustomTableModel* getSubTableModel() const;

public slots:

    void slotDeleteLine() override;
    void slotEditItem(const QModelIndex &, const QVariant &) override;
    void paste() override;
    void del() override;

    void slotRowActionEnable(bool) override;

    void groupCopy();
    void groupPaste();

    void slotManageTableData(const QModelIndex &, const QModelIndex &);
    void slotChangeSubView(int row);

    void slotChangeFrameCount(const QString &, int);
    void signalFrameItemChanged();

signals:

    void currentGroupChanged(const QString &);
    void frameItemChanged();


protected:

    FrameSettingSubTableWidget *subTablewidget;
    FrameSettingTableModel *frameGroupModel;

    QAction *groupCopyAction;
    QAction *groupPasteAction;

    bool isSignalBlocked;
    void setSingnalOn()
    {
        isSignalBlocked = false;
    }
    void setSingnalOff()
    {
        isSignalBlocked = true;
    }

    void createModels() override;
    void createViews() override;
    void createWidgets() override;

    void createMyActions();

    bool warningFrameCount();
    bool warningLineDelete();
    void warningGroupCopyPosition();
    void warningGroupCopyRange();

};
} // namespace post3dapp



#endif
