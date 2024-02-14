#include "standard_section_delegate.h"

#include <QtGui>

#include "define_sectiontable.h"
#include "unified_editingdata.h"

namespace post3dapp{
StandardSectionDelegate::StandardSectionDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *StandardSectionDelegate::createEditor(QWidget *parent,
                                               const QStyleOptionViewItem &option, const QModelIndex &idx) const
{
    Q_UNUSED(option);
    QComboBox *editor = new QComboBox(parent);
    editor->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    const QAbstractItemModel *model = idx.model();
    // 隣の名前から検索する
    QModelIndex idx_b = model->index(idx.row(), idx.column() - 1);
    editor->addItems(SectionDataType::standardSectionNameList(idx_b.data().toString()));
    return editor;
}

void StandardSectionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox *>(editor);
    combo->setCurrentIndex(combo->findText(index.data().toString()));
}

void StandardSectionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                           const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox *>(editor);
    model->setData(index, combo->currentText(), Qt::EditRole);
}

void StandardSectionDelegate::updateEditorGeometry(QWidget *editor,
                                                   const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
} // namespace post3dapp
