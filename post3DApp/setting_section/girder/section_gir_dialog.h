#ifndef SECTIONGIRDERDIALOG_H
#define SECTIONGIRDERDIALOG_H

#include "section_customdialog.h"

class QTabWidget;
namespace post3dapp{
class CustomTableModel;
class SectionGirderRCWidget;
class SectionGirderSRCWidget;
class SectionGirderSWidget;

class SectionGirderDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionGirderDialog( QWidget *parent = 0 );
    CustomTableModel* getRcTableModel() const;
    CustomTableModel* getSTableModel() const;
    CustomTableModel* getSrcTableModel() const;

signals:

private:

    QTabWidget *tab_widget;

    SectionGirderRCWidget *rc_widget;
    SectionGirderSWidget *s_widget;
    SectionGirderSRCWidget *src_widget;

    void createTabWidget();

};
} // namespace post3dapp
#endif
