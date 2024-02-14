#ifndef JOINTLOADSETTINGDIALOG_H
#define JOINTLOADSETTINGDIALOG_H

#include <QDialog>
#include "define_unifieddata.h"

class QPushButton;
class QDataStream;
class QTextStream;

namespace post3dapp{
enum class CBSTATUS;
class AppVersion;
class CustomTableModel;
class JointLoadSettingTableWidget;

class JointLoadSettingDialog : public QDialog
{
    Q_OBJECT

public:

    JointLoadSettingDialog( DATATYPE cellType, QWidget *parent = nullptr );
    void reflectUnifiedData();

    bool checkBinaryFile ( QDataStream &infile, const AppVersion &ver) const;
    void readBinaryFile ( QDataStream &infile, const AppVersion &ver);
    void writeBinaryFile ( QDataStream &outfile);
    void readTextFile (QTextStream &outfile, const AppVersion &ver);
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
    JointLoadSettingTableWidget *myTableWidget;

    QPushButton *rewind_button;
    QPushButton *upload_button;

    void createButtons();
    void setDataModified(bool);

    void errorLoadTypes(const QStringList &);

};
} // namespace post3dapp

#endif
