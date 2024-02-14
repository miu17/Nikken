#include "reference_combobox.h"

#include <QDebug>

#include "factory_unifieddata.h"
#include "unified_data.h"
#include "unified_editingdata.h"
namespace post3dapp{
ReferenceCombobox::ReferenceCombobox(DATATYPE type,UnifiedDataType isout, QWidget *parent)
    : QComboBox(parent), myType(type), isOutput(isout)
{
    setStyle( UnifiedEditingData::getInstance()->getWindowXPStyle() );

    currentID = QUuid();
    currentIDX = 0;
    currentName = QString();

    if (myType == DATATYPE::TPFLOOR || myType == DATATYPE::TPAFLOOR) {
        defaultName = u8"（全階）";
    } else if (myType == DATATYPE::TPFRAMEG) {
        defaultName = u8"（全通りグループ）";
    } else if (myType == DATATYPE::TPFRAMEP) {
        defaultName = u8"（全通り）";
    } else if (myType == DATATYPE::TPMACONC || myType == DATATYPE::TPMASTEEL || myType == DATATYPE::TPMAREIN
               || myType == DATATYPE::TPMAREIND ) {
        defaultName = QString();
    } else if (myType == DATATYPE::TPCOLUMN || myType == DATATYPE::TPGIRDER || myType == DATATYPE::TPBRACE || myType == DATATYPE::TPBEAM ||
               myType == DATATYPE::TPSLAB || myType == DATATYPE::TPWALL || myType == DATATYPE::TPUWALL ||
               myType == DATATYPE::TPDAMPER || myType == DATATYPE::TPISO ||
               myType == DATATYPE::TPSLABLOAD || myType == DATATYPE::TPMEMBERLOAD || myType == DATATYPE::TPJOINTLOAD ) {
        defaultName = "(dummy)";
    } else if (myType == DATATYPE::TPBASE || myType == DATATYPE::TPWALLOPEN) {
        defaultName = QString();
    }

    defaultOn = true;
    updateItems(myType);

    connect ( FactoryUnifiedData::getInstance(isOutput), &UnifiedData::dataUpdated,
              this, &ReferenceCombobox::updateItems );
    connect ( this, QOverload<const QString&>::of(&QComboBox::currentIndexChanged),
              this, QOverload<const QString&>::of(&ReferenceCombobox::saveCurrentItem));
}

void ReferenceCombobox::setDefaultName(const QString &str)
{
    defaultName = str;
    updateItems(myType);
}

void ReferenceCombobox::setDefaultOn(bool ok)
{
    defaultOn = ok;
    updateItems(myType);
}

void ReferenceCombobox::setCurrentText(const QString &str)
{
    int idx = findText(str);
    if ( idx > -1 ) setCurrentIndex(idx);
}

QUuid ReferenceCombobox::currentUuid() const
{
    if ( myType == DATATYPE::TPFLOOR || myType == DATATYPE::TPFRAMEG || myType == DATATYPE::TPFRAMEP
            || myType == DATATYPE::TPAFLOOR ) return currentID;
    return QUuid();
}

void ReferenceCombobox::changeDataType(DATATYPE type)
{
    myType = type;
    updateItems(myType);
}

void ReferenceCombobox::saveCurrentItem(const QString &str)
{
    if ( myType == DATATYPE::TPFLOOR || myType == DATATYPE::TPFRAMEG || myType == DATATYPE::TPFRAMEP || myType == DATATYPE::TPAFLOOR ) {
        currentID = FactoryUnifiedData::getInstance(isOutput)->nameToID(str, myType);
    } else {
        currentName = currentText();
    }
}

void ReferenceCombobox::updateItems(DATATYPE type)
{
    if (type != myType) return;

    QStringList nameList = FactoryUnifiedData::getInstance(isOutput)->getNameList(myType);
    if (defaultOn) nameList.prepend(defaultName);

    int index;
    if ( myType == DATATYPE::TPFLOOR || myType == DATATYPE::TPFRAMEG || myType == DATATYPE::TPFRAMEP || myType == DATATYPE::TPAFLOOR ) {
        QString c_str = FactoryUnifiedData::getInstance(isOutput)->idToName(currentID, myType);
        index = ( nameList.contains(c_str) ) ? nameList.indexOf(c_str) : 0 ;
    } else {
        index = ( nameList.contains(currentName) ) ?
                nameList.indexOf(currentName) : 0 ;
    }

    this->clear();
    this->addItems(nameList);
    this->setCurrentIndex(index);
}
} // namespace post3dapp
