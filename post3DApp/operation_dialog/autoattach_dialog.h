#ifndef AUTOATTACHDIALOG_H
#define AUTOATTACHDIALOG_H

#include <QDialog>
class QButtonGroup;

namespace post3dapp{
class AutoAttachDialog : public QDialog
{
    Q_OBJECT

public:

    AutoAttachDialog( QWidget *parent = 0 );

    bool isOverwriteChecked() const
    {
        return ( overwriteGroup->checkedId() == 0 );
    }

private:

    void createLayout();
    QButtonGroup *overwriteGroup;

};
} // namespace post3dapp
#endif
