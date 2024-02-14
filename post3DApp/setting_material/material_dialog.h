#ifndef MATERIAL_MAINWIDGET_H
#define MATERIAL_MAINWIDGET_H

#include <QDialog>

class QHBoxLayout;
class QPushButton;
class QTextStream;
namespace post3dapp{
enum class CBSTATUS;
class AppVersion;
class CustomTableModel;
class MaterialFilterDialog;
class MaterialTableWidget;

class MaterialSettingDialog : public QDialog
{
    Q_OBJECT

public:

    MaterialSettingDialog( QWidget *parent = nullptr );

    bool checkBinaryFile ( QDataStream &infile, const AppVersion &ver) const;
    void readBinaryFile ( QDataStream &infile, const AppVersion &ver);
    void writeBinaryFile ( QDataStream &outfile);
    void writeTextFile (QTextStream &outfile);
    void readTextFile (QTextStream &outfile, const AppVersion &var);
    void clearData();

    void reflectUnifiedData();
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

//    virtual void slotOpenFilterDialog(){ }
    void slotOpenFilterDialog();

signals:

    void statusChanged(CBSTATUS);

protected:

    QHBoxLayout *buttonsLayout;
    QPushButton *rewind_button;
    QPushButton *upload_button;

    MaterialTableWidget *materialWidget;
    MaterialFilterDialog *filterDialog;

    void createButtons();
    void setDataModified(bool);

};
} // namespace post3dapp
#endif
