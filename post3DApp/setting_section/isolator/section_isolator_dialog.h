#ifndef SECTIONnISOLATORDIALOG_H
#define SECTIONnISOLATORDIALOG_H

#include "section_customdialog.h"

namespace post3dapp{
class CustomTableModel;
class SectionIsolatorSubWidget;

class SectionIsolatorDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionIsolatorDialog( QWidget *parent = 0 );
    CustomTableModel* getTableModel() const;

signals:

private:

    SectionIsolatorSubWidget *isol_widget;

};
} // namespace post3dapp
#endif
