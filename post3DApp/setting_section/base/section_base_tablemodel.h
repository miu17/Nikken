#ifndef SECTION_BASE_TABLEMODEL_H
#define SECTION_BASE_TABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct BaseMemberSection;
struct SectionValues;
class SectionBaseTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionBaseTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionBaseTableModel(const SectionBaseTableModel &);
    SectionValues getMemberSection(int row) const override;
    bool isBlockedItem(const QModelIndex &) const override;
    QList<SectionValues> getSectionList() const override;
    BaseMemberSection CreateBaseMemberSection(const QList<QVariant>&, int) const;
};
} // namespace post3dapp
#endif // SECTION_BASE_TABLEMODEL_H
