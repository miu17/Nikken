#ifndef ADDITIONALOPENDIALOG_H
#define ADDITIONALOPENDIALOG_H

#include <QDialog>

class QLabel;
class QButtonGroup;
class QCheckBox;

namespace post3dapp{
class ReferenceCombobox;
struct FILEREADPATTERN;

class AdditionalOpenDialog : public QDialog
{
    Q_OBJECT

public:

    AdditionalOpenDialog( QWidget *parent = 0 );
    FILEREADPATTERN getFileReadPattern() const;

private slots:

    void slotSetAddReadStatus(bool);
    void slotOpenFileA();
    void slotOpenFileB();
    void slotSelectAllFileA();
    void slotSelectAllFileB();
    void slotCheckButtonStatus();

signals:

private:

    QButtonGroup *group_read;

    QPushButton *button_fileA;
    QLabel *label_fileA;
    QPushButton *button_fileB;
    QLabel *label_fileB;

    QCheckBox *check_addFloor;
    ReferenceCombobox *combo_addFloor;

    QButtonGroup *overlap_list;
    QButtonGroup *overlap_jID;
    QButtonGroup *overlap_mID;
    QButtonGroup *overlap_jXYZ;
    QButtonGroup *overlap_mXYZ;

    void createLayout();

};

} // namespace post3dapp

#endif
