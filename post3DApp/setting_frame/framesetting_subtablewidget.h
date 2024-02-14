#ifndef FRAMESETTINGSUBTABLEWIDGET_H
#define FRAMESETTINGSUBTABLEWIDGET_H

#include "customtable_widget.h"
class QUndoStack;
class QTextStream;
namespace post3dapp{
struct FRAMEPOINTSVALUE;
class FrameSettingTableModel;
class FrameSettingSubTableModel;
class FrameSettingSubTableWidget : public CustomTableWidget
{
    Q_OBJECT

public:

    FrameSettingSubTableWidget( const CustomTableDataType &,
                                FrameSettingTableModel *,
                                QUndoStack *,
                                QWidget *parent = 0 );

    void clearData() override;
    void reflectUnifiedData() override;

    bool readTextFile (QTextStream &infile, bool flg) override;

    void writeTextFile ( QTextStream &outfile, bool);

    void modelUpload() override;
    void modelEdit() override;
    void modelRewind() override;

    QList<FRAMEPOINTSVALUE> getFramePointList() const;

    void setEditOn(bool);
    void changeCurrentGroup(const QString &);
    void updateSubTable(const QString &);

    void appendClipBoad(QString &, const QString &);
    void setGroupPasteCommand( const QList<int> &rowlist, const QString &str  );

protected:

public slots:

    void slotDeleteLine() override;

    void slotManageRowCount(const QString &gid);
    void slotSignalRowCount(const QString &gid);

    void removeGidRows(const QString &);
    void slotShowXYDialog(const QModelIndex &);

    void signalDataChanged(const QModelIndex &, const QModelIndex &);

signals:

    void frameCountChanged(const QString &, int);
    void frameItemChanged();

protected:

    FrameSettingSubTableModel *frameSubModel;
    FrameSettingTableModel *frameGroupModel;

    QHash<QString, int> minRowList;
    QHash<QString, int> importRowList;

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

    bool warningFrameCount();
    bool warningDeleteLine();


};
} // namespace post3dapp



#endif
