#ifndef SECTIONDAMPERTABLEMODEL_H
#define SECTIONDAMPERTABLEMODEL_H
#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct SectionValues;
struct DamperSection;

class SectionDamperTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionDamperTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionDamperTableModel(const SectionDamperTableModel &);
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;
private:
    DamperSection CreateDamperSection(const QList<QVariant>&, int) const;
};
} // namespace post3dapp

#endif // SECTIONDAMPERTABLEMODEL_H
