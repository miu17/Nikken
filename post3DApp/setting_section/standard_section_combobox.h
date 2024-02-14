#ifndef STANDARDSECTIONCOMBOBOX_H
#define STANDARDSECTIONCOMBOBOX_H

#include <QComboBox>
#include "define_sectiontable.h"

namespace post3dapp{
class StandardSectionComboBox : public QComboBox
{
    Q_OBJECT

public:

    StandardSectionComboBox(QWidget *parent = 0)
        : QComboBox(parent) {}

public slots:

    void updateItemList(const QString &str)
    {
        clear();
        addItems(SectionDataType::standardSectionNameList(str));
    }

private:

};
} // namespace post3dapp

#endif
