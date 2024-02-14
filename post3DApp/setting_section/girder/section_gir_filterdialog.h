#ifndef SECTIONGIRDERFILTERDIALOG_H
#define SECTIONGIRDERFILTERDIALOG_H

#include "section_filterdialog.h"

namespace post3dapp{
class SectionTableWidget;

class SectionGirderFilterDialog : public SectionFilterDialog
{
    Q_OBJECT
public:
    SectionGirderFilterDialog(const QList<SectionTableWidget *>&, QWidget * parent = 0);

protected:
    QVBoxLayout* createSwitchLayout() override;
};
} // namespace post3dapp
#endif // SECTIONGIRDERFILTERDIALOG_H
