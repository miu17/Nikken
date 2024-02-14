#ifndef SECTIONUWALLDIALOG_H
#define SECTIONUWALLDIALOG_H

#include "section_customdialog.h"
#include "section_uwallsub_widget.h"

namespace post3dapp{
class SectionUWallDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionUWallDialog( QWidget *parent = nullptr );

    void replaceMatchingUWallName( const QString &str_a, const QString &str_b );

    CustomTableModel* getUnderGroundTableModel() const;
protected:

public slots:


signals:

private:

//    QTabWidget *tab_widget;

    SectionUnderGroundWallWidget *ug_widget;

//    void createTabWidget();

};
} // namespace post3dapp
#endif
