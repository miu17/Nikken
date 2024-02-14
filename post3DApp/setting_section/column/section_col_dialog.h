#ifndef SECTIONCOLUMNDIALOG_H
#define SECTIONCOLUMNDIALOG_H

#include "section_customdialog.h"
#include "section_colsub_widget.h"

namespace post3dapp{
class SectionColumnDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionColumnDialog( QWidget *parent = 0 );
    void replaceMatchingBaseName( const QString &str_a, const QString &str_b );

    CustomTableModel* getRcTableModel() const;
    CustomTableModel* getSTableModel() const;
    CustomTableModel* getSrcTableModel() const;
    CustomTableModel* getCftTableModel() const;
signals:

private:

    QTabWidget *tab_widget;

    SectionColumnRCWidget *rc_widget;
    SectionColumnSWidget *s_widget;
    SectionColumnSRCWidget *src_widget;
    SectionColumnCFTWidget *cft_widget;

    void createTabWidget();

};
} // namespace post3dapp
#endif
