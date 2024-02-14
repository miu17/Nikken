#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

namespace post3dapp{
class ComboBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

    ComboBoxDelegate(const QStringList &strlist, QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void updateStringList(const QStringList &strlist);

private:

    QStringList stringlist;

};
} // namespace post3dapp
#endif
