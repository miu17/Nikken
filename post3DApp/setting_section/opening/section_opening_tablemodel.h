#ifndef SECTIONOPENINGTABLEMODEL_H
#define SECTIONOPENINGTABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct SectionValues;
struct WallOpenSection;

class SectionOpeningTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionOpeningTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionOpeningTableModel(const SectionOpeningTableModel &);
    bool isBlockedItem(const QModelIndex &) const override;
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;
private:
    WallOpenSection CreateWallOpenSection(const QList<QVariant>&, int) const;
};
} // namespace post3dapp
#endif // SECTIONOPENINGTABLEMODEL_H
