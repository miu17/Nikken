#ifndef SECTIONSLABTABLEMODEL_H
#define SECTIONSLABTABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct PlaneMemberSection;
struct SectionValues;

class SectionSlabTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionSlabTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionSlabTableModel(const SectionSlabTableModel &);
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;

private:
    PlaneMemberSection CreatePlaneMemberSection(const QList<QVariant>&, int) const;
};
} // namespace post3dapp
#endif // SECTIONSLABTABLEMODEL_H
