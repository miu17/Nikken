#include "typicalcolor_delegate.h"

#include <QtGui>
#include "typicalcolor_combobox.h"
namespace post3dapp{
TypicalColorDelegate::TypicalColorDelegate( QObject *parent )
    : QStyledItemDelegate(parent)
{
}

QWidget *TypicalColorDelegate::createEditor(QWidget *parent,
                                            const QStyleOptionViewItem &, const QModelIndex &) const
{
    TypicalColorComboBox *editor = new TypicalColorComboBox(parent);
    return editor;
}

void TypicalColorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    TypicalColorComboBox *combo = static_cast<TypicalColorComboBox *>(editor);
    combo->setCurrentIndex(combo->findText(index.data().toString()));
}

void TypicalColorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                        const QModelIndex &index) const
{
    TypicalColorComboBox *combo = static_cast<TypicalColorComboBox *>(editor);
    QString str = combo->currentText();
    model->setData(index, str, Qt::EditRole);
}

void TypicalColorDelegate::updateEditorGeometry(QWidget *editor,
                                                const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

} // namespace post3dapp
