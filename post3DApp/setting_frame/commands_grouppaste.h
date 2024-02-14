#ifndef GROUPPASTECOMMAND_H
#define GROUPPASTECOMMAND_H

#include <QUndoCommand>
class QModelIndex;
class QVariant;

namespace post3dapp{
class FrameSettingTableModel;
class FrameSettingSubTableModel;

class GroupPasteCommand : public QUndoCommand
{

public:

    GroupPasteCommand(FrameSettingTableModel *modelG, FrameSettingSubTableModel *modelS,
                      const QList<int> &rowlist, const QString &str, QUndoCommand *parent = 0);

    void undo();
    void redo();

private:

    FrameSettingTableModel *frameGroupModel;
    FrameSettingSubTableModel *frameSubModel;

    QList<int> groupRowList;
    QString clippedString;

    QHash<QModelIndex, QVariant> g_orgData;
    QHash<QModelIndex, QVariant> f_orgData;

};
} // namespace post3dapp
#endif
