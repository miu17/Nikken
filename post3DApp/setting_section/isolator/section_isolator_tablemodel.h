#ifndef SECTIONISOLATORTABLEMODEL_H
#define SECTIONISOLATORTABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct IsolatorSection;
struct SectionValues;

class SectionIsolatorTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionIsolatorTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionIsolatorTableModel(const SectionIsolatorTableModel &);
    bool isBlockedItem(const QModelIndex &) const override;
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;
private:
    IsolatorSection CreateIsolatorSection(const QList<QVariant>&, int) const;
};
} // namespace post3dapp
#endif // SECTIONISOLATORTABLEMODEL_H
