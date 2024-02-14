#ifndef CHANGEOPENDIALOG_H
#define CHANGEOPENDIALOG_H

#include <QDialog>
#include "define_unifieddata.h"

class QComboBox;
namespace post3dapp{
class ChangeOpenDialog : public QDialog
{
    Q_OBJECT

public:

    ChangeOpenDialog( QWidget *parent = nullptr,UnifiedDataType isout = UnifiedDataType::Input );
    QString currentName() const;
    void slotChangeSectionComboBox();

private:

    void createLayout();
    QComboBox *sectionComboBox;
    const UnifiedDataType isOutput;

};
} // namespace post3dapp
#endif
