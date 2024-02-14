#ifndef REFERENCECOMBOBOXDELEGATE_H
#define REFERENCECOMBOBOXDELEGATE_H

#include <QDebug>
#include "combobox_delegate.h"
#include "unified_inputdata.h"
namespace post3dapp{
class ReferenceComboBoxDelegate : public ComboBoxDelegate
{
    Q_OBJECT

public:

    ReferenceComboBoxDelegate(DATATYPE type = DATATYPE::NODATATYPE, QObject *parent = nullptr)
        : ComboBoxDelegate( QStringList(), parent ), myType(type)
    {
        updateStringList(QStringList{""});

        connect ( UnifiedInputData::getInInstance(), &UnifiedData::dataUpdated,
                  this, &ReferenceComboBoxDelegate::updateItems );
    }

private slots:

    void updateItems(DATATYPE type){

        if(type!=myType) return;

        QStringList nameList{""};
        nameList.append(UnifiedInputData::getInInstance()->getNameList(myType));
        updateStringList(nameList);
    }

private:

    DATATYPE myType;
};
} // namespace post3dapp
#endif
