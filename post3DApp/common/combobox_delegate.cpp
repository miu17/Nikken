#include <QtGui>
#include <QComboBox>

#include "combobox_delegate.h"
#include "unified_editingdata.h"
namespace post3dapp{
ComboBoxDelegate::ComboBoxDelegate(const QStringList &strlist, QObject *parent)
    : QStyledItemDelegate(parent)
{
    stringlist = strlist;
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &, const QModelIndex &) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    editor->addItems(stringlist);
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox *>(editor);
    combo->setCurrentIndex(combo->findText(index.data().toString()));
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QComboBox *combo = static_cast<QComboBox *>(editor);
    QString str = combo->currentText();
    model->setData(index, str, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor,
                                            const QStyleOptionViewItem &option, const QModelIndex &) const
{
    editor->setGeometry(option.rect);
}

void ComboBoxDelegate::updateStringList(const QStringList &strlist)
{
    stringlist = strlist;
}

} // namespace post3dapp
