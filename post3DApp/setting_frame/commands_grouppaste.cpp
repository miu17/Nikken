#include "commands_grouppaste.h"

#include <QtGui>
#include <QDebug>
#include <QModelIndex>

#include "framesetting_subtablemodel.h"
#include "framesetting_subtablewidget.h"
#include "framesetting_tablemodel.h"

namespace post3dapp{
GroupPasteCommand::GroupPasteCommand(FrameSettingTableModel *modelG,
                                     FrameSettingSubTableModel *modelS,
                                     const QList<int> &rowlist, const QString &str, QUndoCommand *parent)
    : QUndoCommand(parent)
{
    frameGroupModel = modelG;
    frameSubModel = modelS;
    groupRowList = rowlist;
    clippedString = str;
}

void GroupPasteCommand::undo()
{
    QModelIndexList idxlist = f_orgData.keys();
    Q_FOREACH (QModelIndex idx, idxlist) {
        frameSubModel->setDataDirect(idx, f_orgData.value(idx), false);
    }
    idxlist = g_orgData.keys();
    Q_FOREACH (QModelIndex idx, idxlist) {
        frameGroupModel->setDataDirect(idx, g_orgData.value(idx), false);
    }
    Q_FOREACH ( int ig, groupRowList )
        frameGroupModel->signalDataChanged(ig);
}

void GroupPasteCommand::redo()
{

    QStringList str_line = clippedString.split('\n');
    int ii = 1;

    int col_fggid = frameGroupModel->getTaggedColumn("GID");
    int col_fguid = frameGroupModel->getTaggedColumn("UUID");
    int col_fcount = frameGroupModel->getTaggedColumn("FRAMESUM");
    int col_fpgid = frameSubModel->getTaggedColumn("GID");
    int col_fpuid = frameSubModel->getTaggedColumn("UUID");

    for ( int ig = 0; ig < groupRowList.count() ; ig++) {

        QStringList str_cols = str_line.at(ii).split('\t');
        if (str_cols.count() < frameGroupModel->columnCount()) return;

        str_cols.replace(col_fggid, "");
        str_cols.replace(col_fguid, "");

        if ( groupRowList.at(ig) > frameGroupModel->rowCount() - 1 ) {
            ii = ii + str_cols.at(col_fcount).toInt();
            continue;
        }

        for (int j = 0; j < frameGroupModel->columnCount(); j++) {
            QModelIndex idx = frameGroupModel->index(groupRowList.at(ig), j);
            g_orgData.insert( idx, frameGroupModel->getData( idx.row(), idx.column() ) );
            frameGroupModel->setDataDirect(idx, QVariant(str_cols.at(j)), false);
        }
        frameGroupModel->signalDataChanged(groupRowList.at(ig));

        ii++;

        QString gid = frameGroupModel->getGIDOfRow(groupRowList.at(ig));

        QList<int> frameRowList = frameSubModel->getRowsOfGID(gid);

        int fcount = str_cols.at(col_fcount).toInt(); // fcount

        for ( int j = 0; j < fcount ; j++ ) {

            str_cols = str_line.at(ii).split('\t');
            str_cols.replace(col_fpgid, ""); // gid framep
            str_cols.replace(col_fpuid, ""); // uid framep

            for (int k = 0; k < frameSubModel->columnCount(); k++) {
                QModelIndex idx = frameSubModel->index(frameRowList.at(j), k);
                f_orgData.insert( idx, frameSubModel->getData( idx.row(), idx.column() ) );
                frameSubModel->setDataDirect(idx, QVariant(str_cols.at(k)), false);
            }

            ii++;
        }

    }
}

} // namespace post3dapp
