#ifndef SECTIONDAMPERDIALOG_H
#define SECTIONDAMPERDIALOG_H

#include "section_customdialog.h"

namespace post3dapp{
class CustomTableModel;
class SectionDamperWidget;

class SectionDamperDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionDamperDialog( QWidget *parent = 0 );
    CustomTableModel* getTableModel() const;
protected:

signals:

private:

    SectionDamperWidget *damper_widget;

};
} // namespace post3dapp
#endif
