#ifndef SECTIONSLABDIALOG_H
#define SECTIONSLABDIALOG_H

#include "section_customdialog.h"

namespace post3dapp{
class CustomTableModel;
class SectionSlabWidget;

class SectionSlabDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionSlabDialog( QWidget *parent = 0 );
    CustomTableModel* getTableModel() const;

signals:

private:

    SectionSlabWidget *slab_widget;

};
} // namespace post3dapp
#endif
