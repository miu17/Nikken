#ifndef ANALYSISGROUPDIALOG_H
#define ANALYSISGROUPDIALOG_H

#include <QDialog>

#include "analysisrange_tablewidget.h"
#include "analysisgroup_glview.h"
#include "menu_button.h"

class QPushButton;
namespace post3dapp{


class AnalysisGroupDialog : public QDialog
{
    Q_OBJECT

public:
    AnalysisGroupDialog( QWidget* parent=0 );
    void clearData();
protected:

public slots:
    void reject() override;
private slots:
    void slotChangeCurrentElement(const QString &);
    void slotAllocateJointTable();
    void slotAdditionallyAllocateJointTable();
    void sendJointItemsTableToDraw();
    void slotAllocateMemberTable();
    void slotAdditionallyAllocateMemberTable();
    void sendMemberItemsTableToDraw();

    void slotTableItemChanged();
    void slotModelUpload();
    void slotModelRewind();

signals:

    void statusChanged(CBSTATUS);

private:

    QHBoxLayout *buttonsLayout;
    AnalysisGroupGLViewer* myViewWidget;
    AnalysisJointRangeTableWidget *rangeJointTableWidget;
    AnalysisMemberRangeTableWidget *rangeMemberTableWidget;

    QPushButton *rewind_button;
    QPushButton *upload_button;

    void createButtons();
    void createLayouts();
    void setDataModified(bool);

    bool informationUpdate();
    bool informationRewind();

    bool checkData();

    bool checkJointChanged() const;
    bool checkMemberChanged() const;
};

} // namespace post3dapp

#endif
