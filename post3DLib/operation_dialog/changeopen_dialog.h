#ifndef CHANGEOPENDIALOG_H
#define CHANGEOPENDIALOG_H

#include <QtWidgets>
#include <QDialog>

class QComboBox;
namespace post3dapp{
class ChangeOpenDialog : public QDialog
{
    Q_OBJECT

public:

    ChangeOpenDialog( QWidget *parent = nullptr,bool isout = false );
    QString currentName() const;
    void slotChangeSectionComboBox();

private:

    void createLayout();
    QComboBox *sectionComboBox;
    const bool isOutput;

};
} // namespace post3dapp
#endif
