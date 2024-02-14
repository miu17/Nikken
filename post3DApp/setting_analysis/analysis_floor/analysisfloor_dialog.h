#ifndef ANALYSISFLOORDIALOG_H
#define ANALYSISFLOORDIALOG_H

#include <QDialog>

namespace post3dapp{
enum class CBSTATUS;
enum class DATATYPE;
class AnalysisFloorTableWidget;
class AppVersion;
class CustomTableModel;
class ReferenceCombobox;

class AnalysisFloorDialog : public QDialog
{
    Q_OBJECT

public:

    AnalysisFloorDialog( QWidget *parent = 0 );
    void reflectUnifiedData();

    bool checkBinaryFile ( QDataStream &, const AppVersion &) const;
    void readBinaryFile ( QDataStream &, const AppVersion &);
    void readTextFile (QTextStream &, const AppVersion &);
    void writeBinaryFile ( QDataStream &);
    void writeTextFile (QTextStream &);
    void clearData();
    CustomTableModel* getTableModel() const;

protected:

public slots:

//    void slotFileOpen();
//    void slotFileSave();
//    void updateDrawWidget();

private slots:

    void slotModelUpload();
    void slotModelRewind();

    void slotModifiedStateOn()
    {
        setDataModified(true);
    }

    void slotUpdateFloorTable(DATATYPE);

signals:

    void statusChanged(CBSTATUS);

private:

    QHBoxLayout *buttonsLayout;
    AnalysisFloorTableWidget *tableWidget;

    QPushButton *rewind_button;
    QPushButton *upload_button;

    ReferenceCombobox *baseFloorCombobox;
    ReferenceCombobox *topFloorCombobox;

    void createButtons();
    void setDataModified(bool);

};
} // namespace post3dapp





#endif
