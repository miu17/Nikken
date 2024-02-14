#ifndef SECTIONOPENINGDIALOG_H
#define SECTIONOPENINGDIALOG_H

#include "section_customdialog.h"

namespace post3dapp{
class CustomTableModel;
class SectionOpeningWidget;

class SectionOpeningDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionOpeningDialog( QWidget *parent = 0 );
    CustomTableModel* getTableModel() const;

signals:

private:

    SectionOpeningWidget *opening_widget;

};
} // namespace post3dapp
#endif
