#include "customtable_view.h"

#include <QtGui>
#include <QtWidgets>
#include <QMenu>

#include "unified_editingdata.h"
namespace post3dapp{
CustomTableView::CustomTableView( QWidget *parent )
    : QTableView( parent )
{
    this->setAlternatingRowColors(true);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setSortingEnabled(true);
    this->sortByColumn(0, Qt::AscendingOrder);

    this->horizontalHeader()->setMinimumHeight(35);
    this->horizontalHeader()->setStretchLastSection(true);

    this->verticalHeader()->setDefaultSectionSize(25);
    this->verticalHeader()->setVisible(false);

    this->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    this->horizontalHeader()->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    this->horizontalHeader()->setFont(QFont(tr("Meiryo UI"), 9));
    this->horizontalHeader()->setStretchLastSection(false);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QPalette pal = this->palette();
    QColor col = pal.color(QPalette::Active, QPalette::Highlight);
    pal.setColor(QPalette::Inactive, QPalette::Highlight, col);
    pal.setColor(QPalette::Inactive, QPalette::HighlightedText, Qt::white);
    this->setPalette(pal);

    setFirstColumn(1);
    setLastColumn(1);
}

void CustomTableView::keyPressEvent ( QKeyEvent *event )
{

    QModelIndex index0 = currentIndex();

    if ( index0.column() == firstColumn ) {
        if ( event->key() == Qt::Key_Left ) {
            setSelectionBehavior(QAbstractItemView::SelectRows);
        } else if ( event->key() == Qt::Key_Backtab ) {
            setSelectionBehavior(QAbstractItemView::SelectRows);
        } else {
            setSelectionBehavior(QAbstractItemView::SelectItems);
        }
    } else if ( index0.column() == 0 ) {
        if ( event->key() == Qt::Key_Right ) {
            setSelectionBehavior(QAbstractItemView::SelectItems);
        } else if ( event->key() == Qt::Key_Tab || event->key() == Qt::Key_Backtab ) {
            setSelectionBehavior(QAbstractItemView::SelectItems);
        } else {
            setSelectionBehavior(QAbstractItemView::SelectRows);
        }
    } else {
        setSelectionBehavior(QAbstractItemView::SelectItems);
    }

    if ( index0.column() == lastColumn && event->key() == Qt::Key_Tab)
        setSelectionBehavior(QAbstractItemView::SelectRows);

    emit selectionBehaviorChanged(selectionBehavior() == QAbstractItemView::SelectRows);

    if ( event->key() == Qt::Key_Delete ) {
        emit delKeyClicked();
        return;
    }
    QTableView::keyPressEvent(event);

    if ( index0.row() != currentIndex().row() ) emit currentRowChanged(currentIndex().row());
}

void CustomTableView::contextMenuEvent ( QContextMenuEvent *event )
{
    Q_UNUSED(event);
    menu->exec(QCursor::pos());
}

void CustomTableView::mousePressEvent( QMouseEvent *event )
{
    QModelIndex cur_index = currentIndex();
    QModelIndex index = indexAt(event->pos());
    if ( cur_index.row() != index.row() ) emit currentRowChanged(index.row());

    if ( index.column() == 0 && event->button() == Qt::LeftButton ) {
        setSelectionBehavior(QAbstractItemView::SelectRows);
    } else if ( event->button() == Qt::LeftButton ) {
        setSelectionBehavior(QAbstractItemView::SelectItems);
    }

    emit selectionBehaviorChanged(selectionBehavior() == QAbstractItemView::SelectRows);

    QTableView::mousePressEvent(event);
}
} // namespace post3dapp
