#include <QtGui>
#include <QHeaderView>

#include "material_tableview.h"
#include "define_materialtable.h"
#include "define_material_struct.h"
namespace post3dapp{
MaterialTableView::MaterialTableView( QWidget *parent )
    : CustomTableView( parent )
{
    QFont font(tr("Meiryo UI"), 9);
    font.setStretch(98);
    this->horizontalHeader()->setFont(font);
}

void MaterialTableView::keyPressEvent ( QKeyEvent *event )
{
    QModelIndex index0 = currentIndex();
//tab or backtabでviewの左右端から逆側にフォーカスを移動させた後、さらにtabや矢印キーを用いて
//最後のviewまでフォーカスを移動させたとき、なぜか全体選択になる。
    if ( index0.column() == firstColumn) {
        if ( event->key() == Qt::Key_Left ) {
            setSelectionBehavior(QAbstractItemView::SelectRows);
        } else if ( event->key() == Qt::Key_Backtab ) {
            setSelectionBehavior(QAbstractItemView::SelectRows);
        } else if (middleViewLastColumns.contains(
                       index0.column()) //最初のがview1カラムのみの場合
                   && (event->key() == Qt::Key_Tab || event->key() == Qt::Key_Right)) {
            emit requestChangeViewNext();
            setSelectionBehavior(QAbstractItemView::SelectItems);
            return;
        } else {
            setSelectionBehavior(QAbstractItemView::SelectItems);
        }
    } else if ( index0.column() == 0 ) {
        if ( event->key() == Qt::Key_Right ) {
            setSelectionBehavior(QAbstractItemView::SelectItems);
        } else if ( event->key() == Qt::Key_Tab) {
            setSelectionBehavior(QAbstractItemView::SelectItems);
        } else if ( event->key() == Qt::Key_Backtab) {
            emit requestChangeViewBack();
            return;
            setSelectionBehavior(QAbstractItemView::SelectItems);
        } else {
            setSelectionBehavior(QAbstractItemView::SelectRows);
        }
    } else if ( index0.column() == lastColumn && event->key() == Qt::Key_Tab) {
        emit requestChangeViewNext();
        setSelectionBehavior(QAbstractItemView::SelectRows);
    } else if (middleViewLastColumns.contains(index0.column())
               && (event->key() == Qt::Key_Tab || event->key() == Qt::Key_Right)) {
        emit requestChangeViewNext();
        setSelectionBehavior(QAbstractItemView::SelectItems);
        return;

    } else if (middleViewFirstColumns.contains(index0.column())
               && (event->key() == Qt::Key_Backtab || event->key() == Qt::Key_Left)) {
        emit requestChangeViewBack();
        setSelectionBehavior(QAbstractItemView::SelectItems);
        return;

    } else {
        setSelectionBehavior(QAbstractItemView::SelectItems);
    }

//    emit selectionBehaviorChanged(selectionBehavior()==QAbstractItemView::SelectRows);

    if ( event->key() == Qt::Key_Delete ) {
        emit delKeyClicked();
        emit delKeyClicked(index0);
        return;
    }
    if ( event->key() == Qt::Key_Home ) {
        emit homeKeyClicked();
        return;
    }
    if ( event->key() == Qt::Key_End ) {
        emit endKeyClicked();
        return;
    }
    QTableView::keyPressEvent(event);

    if ( index0.row() != currentIndex().row() )emit currentRowChanged(currentIndex().row());


}

void MaterialTableView::setTableViewBorderColumns(const MaterialDataType &dataType,
                                                  const QList<bool> &filters)
{
    //firstcolumn,lastcolumnはviewと同じ数だけ欲しい。同じ値があっても良い。viewの中身が全部空のとき、0を指す。
    int sp1 = dataType.getSeparateColumn1();
    QList<int> myfirstcolumns;
    QList<int> mylastcolumns;
    QList<bool> myfilters;
    if (filters.count() != dataType.count()) {
        for (int i = 0; i < dataType.count(); ++i)myfilters << true;
    } else {
        myfilters = filters;
    }

//firstcolumn
    int nccounter = 0; //NoColumnCounter columnの無いviewを数える
    for (int i = 1; i < sp1; ++i) {
        if (myfilters.at(i) == true) {
            myfirstcolumns << i;
            break;
        }
        if (i == sp1 - 1)nccounter++;
    }
    if (myfirstcolumns.count() != 0) {
        for (int j = 0; j < nccounter + 1; ++j)myfirstcolumns << myfirstcolumns.at(0);
    } else {
        for (int j = 0; j < nccounter + 1; ++j)myfirstcolumns << 0;
    }

//lastcolumn
    int lasttrue = 0;
    for (int i = 1; i < dataType.count(); ++i) {
        if     (i == sp1) {
            mylastcolumns << lasttrue;   //前のviewの最後のtrueをlastcolumnに
        } else if (i == dataType.count()) {
            mylastcolumns << lasttrue;
        }
        if (myfilters.at(i) == true)lasttrue = i;
    }
    mylastcolumns << lasttrue; //最後のtrueをlastcolumn
    for (int i = 0; i < mylastcolumns.count(); ++i) {
        if (mylastcolumns.at(i) == 0)mylastcolumns[i] = lasttrue;
    }

    if (!myfirstcolumns.isEmpty()) {
        firstColumn = myfirstcolumns.first();
        myfirstcolumns.pop_front();
        if (!myfirstcolumns.isEmpty())middleViewFirstColumns = myfirstcolumns;
    }
    if (!mylastcolumns.isEmpty()) {
        lastColumn = mylastcolumns.last();
        mylastcolumns.pop_back();
        if (!mylastcolumns.isEmpty())middleViewLastColumns = mylastcolumns;
    }
}
} // namespace post3dapp
