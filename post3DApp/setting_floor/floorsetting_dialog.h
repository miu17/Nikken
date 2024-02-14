#ifndef FLOORSETTINGDIALOG_H
#define FLOORSETTINGDIALOG_H

#include <QDialog>

class QHBoxLayout;
class QPushButton;

namespace post3dapp{
enum class CBSTATUS;
class AppVersion;
class CustomTableModel;
class FloorSettingTableWidget;
class FloorSettingDrawWidget;

class FloorSettingDialog : public QDialog
{
    Q_OBJECT

public:

    FloorSettingDialog( QWidget *parent = nullptr );

    bool checkBinaryFile ( QDataStream &infile, const AppVersion &ver) const;
    void readBinaryFile ( QDataStream &infile, const AppVersion &ver);
    void writeBinaryFile ( QDataStream &outfile);
    void writeTextFile (QTextStream &outfile, bool);
    void readTextFile (QTextStream &infile, const AppVersion &var);
    void clearData();

    void reflectUnifiedData();
    CustomTableModel* getTableModel() const;

protected:

public slots:

    void slotTextFileOpen();
    void slotTextFileSave();
    void updateDrawWidget();

private slots:

    void slotModelUpload();
    void slotModelEdit();
    void slotModelRewind();

signals:

    void statusChanged(CBSTATUS);

private:

    QHBoxLayout *buttonsLayout;
    FloorSettingTableWidget *myTableWidget;
    FloorSettingDrawWidget *myDrawWidget;

    QPushButton *rewind_button;
    QPushButton *upload_button;
    QPushButton *edit_button;

    void createButtons();
    void setDataModified(bool);
};

} // namespace post3dapp

#endif
