#ifndef STANDARDMATERIALDELEGATE_H
#define STANDARDMATERIALDELEGATE_H

#include <QStyledItemDelegate>
namespace post3dapp{
class StandardMaterialDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

    StandardMaterialDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:

};
} // namespace post3dapp
#endif
