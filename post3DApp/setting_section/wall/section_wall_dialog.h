#ifndef SECTIONWALLDIALOG_H
#define SECTIONWALLDIALOG_H

#include "section_customdialog.h"
#include "section_wallsub_widget.h"

namespace post3dapp{
class SectionWallDialog : public SectionCustomDialog
{
    Q_OBJECT

public:

    SectionWallDialog( QWidget *parent = nullptr );

    void replaceMatchingWallName( const QString &str_a, const QString &str_b );
    void replaceMatchingSWallName( const QString &str_a, const QString &str_b );

    CustomTableModel* getRegularTableModel() const;
    CustomTableModel* getSteelTableModel() const;
protected:

public slots:

//    void slotModelUpload() override;

signals:

private:

    QTabWidget *tab_widget;

    SectionWallRegularWidget *reg_widget;
    SectionWallSteelWidget *steel_widget;

    void createTabWidget();

};
} // namespace post3dapp
#endif
