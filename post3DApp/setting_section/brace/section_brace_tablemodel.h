#ifndef SECTIONBRACETABLEMODEL_H
#define SECTIONBRACETABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct LineMemberSection;
struct SectionValues;

class SectionBraceTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionBraceTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionBraceTableModel(const SectionBraceTableModel &);
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;
    bool isBlockedItem(const QModelIndex &) const override;
private:
    LineMemberSection CreateLineMemberSection(const QList<QVariant>&, int row) const;
};
} // namespace post3dapp
#endif // SECTIONBRACETABLEMODEL_H
