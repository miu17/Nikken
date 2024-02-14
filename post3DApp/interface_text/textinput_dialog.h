#ifndef TEXTINPUTDIALOG_H
#define TEXTINPUTDIALOG_H

#include <QDialog>

class QHBoxLayout;
namespace post3dapp{
enum class CBSTATUS;
class InputJointTableWidget;
class InputMemberTableWidget;
class InputLoadTableWidget;

class TextInputDialog : public QDialog
{
    Q_OBJECT

public:

    TextInputDialog( QWidget *parent = 0 );

    void clearData();

protected:

public slots:

    void slotTextFileOpen();
    void slotTextFileSave();

private slots:

    void slotDataImport();

    void slotJointUpload();
    void slotMemberUpload();
    void slotLoadUpload();
    void slotModelUpload();

signals:

    void statusChanged(CBSTATUS);

private:

    QHBoxLayout *buttonsLayout;

    InputJointTableWidget *jointTableWidget;
    InputMemberTableWidget *memberTableWidget;
    InputLoadTableWidget *loadTableWidget;

    void createButtons();
};
} // namespace post3dapp
#endif
