#ifndef SECTIONBRACEDIALOG_H
#define SECTIONBRACEDIALOG_H

#include "section_customdialog.h"

namespace post3dapp{
class CustomTableModel;
class SectionBraceSubWidget;

class SectionBraceDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionBraceDialog( QWidget *parent = 0 );
    CustomTableModel* getTableModel() const;
signals:

private:

    SectionBraceSubWidget *s_widget;

};
} // namespace post3dapp
#endif
