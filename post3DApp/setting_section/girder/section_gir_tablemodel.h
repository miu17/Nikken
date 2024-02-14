#ifndef SECTIONGIRDERTABLEMODEL_H
#define SECTIONGIRDERTABLEMODEL_H

#include "section_tablemodel.h"
namespace post3dapp{
class SectionDataType;
struct LineMemberSection;
struct SectionValues;

class SectionGirderTableModel : public SectionTableModel
{
    Q_OBJECT
public:
    SectionGirderTableModel(const SectionDataType &type, QObject *parent = 0);
    SectionGirderTableModel(const SectionGirderTableModel &);
    SectionValues getMemberSection(int row) const override;
    QList<SectionValues> getSectionList() const override;

private:
    LineMemberSection CreateLineMemberSection(const QList<QVariant>&, int row) const;
};
} // namespace post3dapp
#endif // SECTIONGIRDERTABLEMODEL_H
