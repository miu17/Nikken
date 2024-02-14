#ifndef STANDARDSECTIONDELEGATE_H
#define STANDARDSECTIONDELEGATE_H

#include <QStyledItemDelegate>

class QWidget;
namespace post3dapp{
class StandardSectionDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:

    StandardSectionDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:

};
} // namespace post3dapp
#endif
