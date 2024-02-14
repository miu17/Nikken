#ifndef TEXTINPUTDIALOG_H
#define TEXTINPUTDIALOG_H

#include <QDialog>

#include "inputjoint_tablewidget.h"
#include "inputmember_tablewidget.h"
#include "inputload_tablewidget.h"
#include "menu_button.h"

class QPushButton;

class TextInputDialog : public QDialog
{
    Q_OBJECT

public:

    TextInputDialog( QWidget* parent=0 );

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

    bool informationUpdate();

};

#endif
