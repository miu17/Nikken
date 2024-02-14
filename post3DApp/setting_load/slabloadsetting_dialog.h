#ifndef SLABLOADSETTINGDIALOG_H
#define SLABLOADSETTINGDIALOG_H

#include <QDialog>
#include "define_unifieddata.h"

class QPushButton;
namespace post3dapp{
enum class CBSTATUS;
class AppVersion;
class CustomTableModel;
class SlabLoadSettingTableWidget;

class SlabLoadSettingDialog : public QDialog
{
    Q_OBJECT

public:

    SlabLoadSettingDialog( DATATYPE cellType, QWidget *parent = 0 );
    void reflectUnifiedData();

    bool checkBinaryFile ( QDataStream &infile, const AppVersion&) const;
    void readBinaryFile ( QDataStream &infile, const AppVersion&);
    void readTextFile (QTextStream &outfile, const AppVersion&);
    void writeBinaryFile ( QDataStream &outfile);
    void writeTextFile (QTextStream &outfile);
    void clearData();

    void replaceMatchingName( const QString &str_a, const QString &str_b );
    CustomTableModel* getTableModel() const;
protected:

public slots:

private slots:

    void slotTextFileOpen();
    void slotTextFileSave();

    void slotModelUpload();
    void slotModelRewind();

    void slotModifiedStateOn()
    {
        setDataModified(true);
    }

signals:

    void statusChanged(CBSTATUS);

private:

    DATATYPE myDataType;

    QHBoxLayout *buttonsLayout;
    SlabLoadSettingTableWidget *myTableWidget;

    QPushButton *rewind_button;
    QPushButton *upload_button;

    void createButtons();
    void setDataModified(bool);

};

} // namespace post3dapp
#endif
