#include "typicalcolor_style.h"

#include <QtGui>
#include <QComboBox>
#include <QStyleOptionComboBox>

namespace post3dapp{
TypicalColorStyle::TypicalColorStyle() : QProxyStyle () {}

void TypicalColorStyle::drawControl(ControlElement element, const QStyleOption *option,
                                    QPainter *painter, const QWidget *widget) const
{
    switch (element) {
    case CE_ComboBoxLabel:
        if (const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
            QRect editRect = proxy()->subControlRect(CC_ComboBox, cb, SC_ComboBoxEditField, widget);
            painter->save();
            painter->setClipRect(editRect);
            if (const QComboBox *combo = qobject_cast<const QComboBox *>(widget)) {
                QColor col = qvariant_cast<QColor>(combo->itemData(combo->currentIndex(), Qt::DecorationRole));
                QPixmap pixmap(cb->iconSize);
                pixmap.fill(col);
                QRect iconRect(editRect);
                iconRect.setWidth(cb->iconSize.width() + 4);
                iconRect = alignedRect(cb->direction,
                                       Qt::AlignLeft | Qt::AlignVCenter,
                                       iconRect.size(), editRect);
                if (cb->editable)
                    painter->fillRect(iconRect, option->palette.brush(QPalette::Base));
                proxy()->drawItemPixmap(painter, iconRect, Qt::AlignCenter, pixmap);

                if (cb->direction == Qt::RightToLeft)
                    editRect.translate(-4 - cb->iconSize.width(), 0);
                else
                    editRect.translate(cb->iconSize.width() + 4, 0);

            }
            if (!cb->currentText.isEmpty() && !cb->editable) {
                proxy()->drawItemText(painter, editRect.adjusted(1, 0, -1, 0),
                                      visualAlignment(cb->direction, Qt::AlignLeft | Qt::AlignVCenter),
                                      cb->palette, cb->state & State_Enabled, cb->currentText);
            }
            painter->restore();
        }
        break;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
        break;
    }
    return;
}
} // namespace post3dapp
