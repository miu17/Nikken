#ifndef FRAMESETTINGDIALOG_H
#define FRAMESETTINGDIALOG_H

#include <QDialog>

class QDataStream;
class QHBoxLayout;
class QPushButton;
class QTextStream;

namespace post3dapp{
enum class CBSTATUS;
class AppVersion;
class CustomTableModel;
class FrameSettingTableWidget;
class FrameSettingDrawWidget;

class FrameSettingDialog : public QDialog
{
    Q_OBJECT

public:

    FrameSettingDialog( QWidget *parent = nullptr );

    bool checkBinaryFile ( QDataStream &infile, const AppVersion &ver) const;
    void readBinaryFile ( QDataStream &infile, const AppVersion &ver);
    void readTextFile (QTextStream &infile , const AppVersion &var);
    void writeBinaryFile ( QDataStream &outfile);
    void writeTextFile (QTextStream &outfile, bool);
    void clearData();

    void reflectUnifiedData();
    CustomTableModel* getTableModel() const;
    CustomTableModel* getSubTableModel() const;
protected:

public slots:

    void slotTextFileOpen();
    void slotTextFileSave();

    void updateDrawWidget();
    void changeCurrentGroupOfDrawWidget(const QString &);
    void changeDrawingStatus(bool);

private slots:

    void slotModelUpload();
    void slotModelEdit();
    void slotModelRewind();

signals:

    void statusChanged(CBSTATUS);

private:

    QHBoxLayout *buttonsLayout;
    FrameSettingTableWidget *myTableWidget;
    FrameSettingDrawWidget *myDrawWidget;

    QPushButton *rewind_button;
    QPushButton *upload_button;
    QPushButton *edit_button;

    bool drawLinkStatus;

    void createButtons();
    void setDataModified(bool);
};

} // namespace post3dapp




#endif
