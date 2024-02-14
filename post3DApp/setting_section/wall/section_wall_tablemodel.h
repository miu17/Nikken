#ifndef SECTIONWALLTABLEMODEL_H
#define SECTIONWALLTABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct PlaneMemberSection;
struct SectionValues;


class SectionWallTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionWallTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionWallTableModel(const SectionWallTableModel &);
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;

private:
    PlaneMemberSection CreatePlaneMemberSection(const QList<QVariant>&, int) const;
};
} // namespace post3dapp
#endif // SECTIONWALLTABLEMODEL_H
