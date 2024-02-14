#ifndef REFERENCECOMBOBOX_H
#define REFERENCECOMBOBOX_H

#include <QComboBox>
#include "define_unifieddata.h"

namespace post3dapp{
class  ReferenceCombobox : public QComboBox
{
    Q_OBJECT

public:

    ReferenceCombobox(DATATYPE type = DATATYPE::NODATATYPE, UnifiedDataType isout = UnifiedDataType::Input, QWidget *parent = nullptr );

    void setDefaultName(const QString &str);
    void setDefaultOn(bool ok);
    void setCurrentText(const QString &str);
    void changeDataType(DATATYPE type);

    QUuid currentUuid() const;

private slots:

    void saveCurrentItem(const QString &str);
    void updateItems(DATATYPE type);

private:

    QString defaultName;
    bool defaultOn;
    QUuid currentID;
    int currentIDX;
    QString currentName;
    DATATYPE myType;
    const UnifiedDataType isOutput;

};
} // namespace post3dapp
#endif
