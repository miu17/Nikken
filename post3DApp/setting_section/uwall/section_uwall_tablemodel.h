#ifndef SECTIONUWALLTABLEMODEL_H
#define SECTIONUWALLTABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct PlaneMemberSection;
struct SectionValues;


class SectionUWallTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionUWallTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionUWallTableModel(const SectionUWallTableModel &);
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;

private:
    PlaneMemberSection CreatePlaneMemberSection(const QList<QVariant>&, int) const;
};
} // namespace post3dapp
#endif // SECTIONWALLTABLEMODEL_H
