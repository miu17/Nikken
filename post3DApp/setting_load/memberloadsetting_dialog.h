#ifndef MEMBERLOADSETTINGDIALOG_H
#define MEMBERLOADSETTINGDIALOG_H

#include <QDialog>
#include "define_unifieddata.h"

class QPushButton;
namespace post3dapp{
enum class CBSTATUS;
class AppVersion;
class CustomTableModel;
class MemberLoadSettingTableWidget;

class MemberLoadSettingDialog : public QDialog
{
    Q_OBJECT

public:

    MemberLoadSettingDialog( DATATYPE cellType, QWidget *parent = 0 );
    void reflectUnifiedData();

    bool checkBinaryFile ( QDataStream &infile, const AppVersion &ver) const;
    void readBinaryFile ( QDataStream &infile, const AppVersion &ver);
    void readTextFile (QTextStream &outfile, const AppVersion &ver);
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
    MemberLoadSettingTableWidget *myTableWidget;

    QPushButton *rewind_button;
    QPushButton *upload_button;

    void createButtons();
    void setDataModified(bool);

    void errorLoadTypes(const QStringList &);
};
} // namespace post3dapp

#endif
