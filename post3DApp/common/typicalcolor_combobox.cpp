#include "typicalcolor_combobox.h"

#include "typicalcolor_table.h"
#include "unified_inputdata.h"
#include "unified_editingdata.h"
namespace post3dapp{


TypicalColorComboBox::TypicalColorComboBox(QWidget *widget) : QComboBox(widget)
{
    populateList();
    setStyle( UnifiedEditingData::getInstance()->getTypicalColorStyle() );
}

QColor TypicalColorComboBox::color() const
{
    if ( currentIndex() == 0 ) return Qt::transparent;
    return itemData(currentIndex(), Qt::DecorationRole).value<QColor>();
}

QString TypicalColorComboBox::name() const
{
    if (currentIndex() == 0) return QString();
    return TypicalColorTable::tpColTable[currentIndex()].name;
}

void TypicalColorComboBox::setColor(QColor color)
{
    if ( color == Qt::transparent ) {
        setCurrentIndex(0);
        return;
    }
    if ( color == Qt::white ) {
        setCurrentIndex(9);
        return;
    }
    setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}

int TypicalColorComboBox::nameToIndex(const QString& str){
    for(int i = 0; i < TypicalColorTable::tpColTable_size; i++){
        if (TypicalColorTable::tpColTable[i].name == str)
            return i;
    }
    return 0;
}


int TypicalColorComboBox::colorToIndex(const QColor &color)
{
    // whiteとnoneが区別できない
    if (color == QColorConstants::Svg::white)
        return 9;

    for (int i = 0; i < TypicalColorTable::tpColTable_size ; i++ ){
        if (TypicalColorTable::tpColTable[i].rgb == color.rgb())
            return i;
    }
    return 0;
}

void TypicalColorComboBox::populateList()
{
    for ( int i = 0 ; i < TypicalColorTable::tpColTable_size ; i++ ) {
        insertItem(i, TypicalColorTable::tpColTable[i].name );
        setItemData(i, QColor(TypicalColorTable::tpColTable[i].rgb), Qt::DecorationRole);
    }
}
} // namespace post3dapp
