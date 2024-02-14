#ifndef SECTIONCOLTABLEMODEL_H
#define SECTIONCOLTABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
struct SectionValues;
struct LineMemberSection;

class SectionColumnTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionColumnTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionColumnTableModel(const SectionColumnTableModel &);
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;
private:
    LineMemberSection CreateLineMemberSection(const QList<QVariant>&, int row) const;
};
} // namespace post3dapp
#endif // SECTIONCOLUMNTABLEMODEL_H
