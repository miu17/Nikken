#ifndef SECTIONBEAMTABLEMODEL_H
#define SECTIONBEAMTABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct LineMemberSection;
struct SectionValues;
class SectionBeamTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionBeamTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionBeamTableModel(const SectionBeamTableModel&);
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;
private:
    LineMemberSection CreateLineMemberSection(const QList<QVariant>&, int row) const;
};
} // namespace post3dapp
#endif // SECTIONBEAMTABLEMODEL_H
