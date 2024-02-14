#ifndef SECTIONBEAMDIALOG_H
#define SECTIONBEAMDIALOG_H

#include "section_customdialog.h"

class QTabWidget;
namespace post3dapp{
class CustomTableModel;
class SectionBeamRCWidget;
class SectionBeamSRCWidget;
class SectionBeamSWidget;

class SectionBeamDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionBeamDialog( QWidget *parent = 0 );
    CustomTableModel* getRcTableModel() const;
    CustomTableModel* getSTableModel() const;
    CustomTableModel* getSrcTableModel() const;
signals:

private:

    QTabWidget *tab_widget;

    SectionBeamRCWidget *rc_widget;
    SectionBeamSWidget *s_widget;
    SectionBeamSRCWidget *src_widget;

    void createTabWidget();

};
} // namespace post3dapp
#endif
