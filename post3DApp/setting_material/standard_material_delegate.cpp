#include <QtGui>

#include "standard_material_delegate.h"
#include "define_materialtable.h"
namespace post3dapp{
StandardMaterialDelegate::StandardMaterialDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *StandardMaterialDelegate::createEditor(QWidget *parent,
                                                const QStyleOptionViewItem &option, const QModelIndex &idx) const
{
    Q_UNUSED(option);
    const QAbstractItemModel *model = idx.model();
    QModelIndex idx_b = model->index(idx.row(), idx.column() - 1);
    QString str = idx_b.data().toString();
    QStringList list = MaterialDataType::standardMaterialNameList(str);
    QComboBox *editor = new QComboBox(parent);
    editor->addItems(list);
    editor->setEditable(true);
    return editor;
}

void StandardMaterialDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox *>(editor);
    int idx = combo->findText(index.data().toString());
    if (idx >= 0) combo->setCurrentIndex(idx);
}

void StandardMaterialDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                            const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox *>(editor);
    QString str = combo->currentText();
    model->setData(index, str, Qt::EditRole);
}

void StandardMaterialDelegate::updateEditorGeometry(QWidget *editor,
                                                    const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}
} // namespace post3dapp
