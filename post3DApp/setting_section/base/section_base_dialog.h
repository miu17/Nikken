#ifndef SECTIONBASEDIALOG_H
#define SECTIONBASEDIALOG_H

#include "section_customdialog.h"

namespace post3dapp {
class CustomTableModel;
class SectionBaseSRCWidget;
class SectionBaseSWidget;

class SectionBaseDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionBaseDialog( QWidget *parent = 0 );
    CustomTableModel* getSTableModel() const;
    CustomTableModel* getSrcTableModel() const;
protected:

signals:

private:

    QTabWidget *tab_widget;

    SectionBaseSWidget *s_widget;
    SectionBaseSRCWidget *src_widget;

    void createTabWidget();

};
} // namespace post3dapp
#endif
