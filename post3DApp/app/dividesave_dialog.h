#ifndef DIVIDESAVEDIALOG_H
#define DIVIDESAVEDIALOG_H

#include <QDialog>

class QLabel;
class QCheckBox;


namespace post3dapp{
struct FILESAVEPATTERN;
class DivideSaveDialog : public QDialog
{
    Q_OBJECT

public:

    DivideSaveDialog( QWidget *parent = 0 );
    FILESAVEPATTERN getFileSavePattern() const;

private slots:

    void slotShowRangeDialog();
    void slotSetOutRangeStatus(bool);
    void slotOpenFileA();
    void slotOpenFileB();
    void slotCheckButtonStatus();

signals:

private:

    QLabel *label_range;

    QPushButton *button_fileA;
    QLabel *label_fileA;
    QPushButton *button_fileB;
    QLabel *label_fileB;

    QCheckBox *check_outrange;

    void createLayout();

};

} // namespace post3dapp
#endif
