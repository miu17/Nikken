#include "section_tableview.h"

#include <QtGui>
#include <QHeaderView>

#include "define_section_struct.h"
#include "define_sectiontable.h"

namespace post3dapp{
SectionTableView::SectionTableView( QWidget *parent )
    : CustomTableView( parent )
{
    QFont font(tr("Meiryo UI"), 9);
    font.setStretch(98);
    this->horizontalHeader()->setFont(font);
}

void SectionTableView::keyPressEvent ( QKeyEvent *event )
{
    QModelIndex index0 = currentIndex();
//tab or backtabでviewの左右端から逆側にフォーカスを移動させた後、さらにtabや矢印キーを用いて
//最後のviewまでフォーカスを移動させたとき、なぜか全体選択になる。

    if ( index0.column() == firstColumn ) {
        if ( event->key() == Qt::Key_Left || event->key() == Qt::Key_Backtab ) {
            setSelectionBehavior(QAbstractItemView::SelectRows);
        } else if ( middleViewLastColumns.contains(
                        index0.column()) //最初のがview1カラムのみの場合
                    && ( event->key() == Qt::Key_Tab || event->key() == Qt::Key_Right ) ) {
            emit requestChangeViewNext();
//            setSelectionBehavior(QAbstractItemView::SelectItems);
            return;
        } else {
            setSelectionBehavior(QAbstractItemView::SelectItems);
        }
    } else if ( index0.column() == 0 ) {
        if ( event->key() == Qt::Key_Right || event->key() == Qt::Key_Tab ) {
            setSelectionBehavior(QAbstractItemView::SelectItems);
        } else if ( event->key() == Qt::Key_Backtab) {
            emit requestChangeViewBack();
            return;
//            setSelectionBehavior(QAbstractItemView::SelectItems);
        } else {
            setSelectionBehavior(QAbstractItemView::SelectRows);
        }
    } else if ( index0.column() == lastColumn && event->key() == Qt::Key_Tab ) {
        emit requestChangeViewNext();
        return;

    } else if ( middleViewLastColumns.contains(index0.column())
                && (event->key() == Qt::Key_Tab || event->key() == Qt::Key_Right) ) {
        emit requestChangeViewNext();
//            setSelectionBehavior(QAbstractItemView::SelectItems);
        return;

    } else if ( middleViewFirstColumns.contains(index0.column())
                && (event->key() == Qt::Key_Backtab || event->key() == Qt::Key_Left) ) {
        emit requestChangeViewBack();
//            setSelectionBehavior(QAbstractItemView::SelectItems);
        return;

    } else {
        setSelectionBehavior(QAbstractItemView::SelectItems);
    }
    /*
        if( index0.column() == lastColumn && event->key() == Qt::Key_Tab ){
                emit requestChangeViewNext();
                setSelectionBehavior(QAbstractItemView::SelectRows);
        }
    */

    emit selectionBehaviorChanged( selectionBehavior() == QAbstractItemView::SelectRows );

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

void SectionTableView::setTableViewBorderColumns(const SectionDataType &sectiondatatype,
                                                 const QList<bool> &filters)
{
    //firstcolumn,lastcolumnはviewと同じ数だけ欲しい。同じ値があっても良い。viewの中身が全部空のとき、0を指す。
    int sp1 = sectiondatatype.getSeparateColumn1();
    int sp2 = sectiondatatype.getSeparateColumn2();
    int sp3 = sectiondatatype.getSeparateColumn3();
    int viewcount = 0;
    if     (sp1 < 0) {
        sp1 = sectiondatatype.count();
        viewcount = 1;
    } else if (sp2 < 0) {
        sp2 = sectiondatatype.count();
        viewcount = 2;
    } else if (sp3 < 0) {
        sp3 = sectiondatatype.count();
        viewcount = 3;
    } else {
        viewcount = 4;
    }
    QList<int> myfirstcolumns;
    QList<int> mylastcolumns;
    QList<bool> myfilters;
    if (filters.count() != sectiondatatype.count()) {
        for (int i = 0; i < sectiondatatype.count(); ++i)myfilters << true;
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
    if (viewcount == 1) goto endcountfirstcolumn;
    for (int i = sp1; i < sp2; ++i) {
        if (myfilters.at(i) == true) {
            for (int j = 0; j < nccounter + 1; ++j)myfirstcolumns << i;
            nccounter = 0;
            break;
        }
        if (i == sp2 - 1)nccounter++;
    }
    if (viewcount == 2) goto endcountfirstcolumn;
    for (int i = sp2; i < sp3; ++i) {
        if (myfilters.at(i) == true) {
            for (int j = 0; j < nccounter + 1; ++j)myfirstcolumns << i;
            nccounter = 0;
            break;
        }
        if (i == sp3 - 1)nccounter++;
    }
    if (viewcount == 3) goto endcountfirstcolumn;
    for (int i = sp3; i < sectiondatatype.count(); ++i) {
        if (myfilters.at(i) == true) {
            for (int j = 0; j < nccounter + 1; ++j)myfirstcolumns << i;
            nccounter = 0;
            break;
        }
        if (i == sectiondatatype.count() - 1)nccounter++;
    }
endcountfirstcolumn:
    if (myfirstcolumns.count() != 0) {
        for (int j = 0; j < nccounter + 1; ++j)myfirstcolumns << myfirstcolumns.at(0);
    } else {
        for (int j = 0; j < nccounter + 1; ++j)myfirstcolumns << 0;
    }

//lastcolumn
    int lasttrue = 0;
    for (int i = 1; i < sectiondatatype.count(); ++i) {
        if     (i == sp1) {
            mylastcolumns << lasttrue;   //前のviewの最後のtrueをlastcolumnに
        } else if (i == sp2) {
            mylastcolumns << lasttrue;
        } else if (i == sp3) {
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
