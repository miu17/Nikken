#include "section_wall_widget.h"

#include <QtGui>
#include <QDebug>

#include "section_filterdialog.h"
#include "section_linebuttons.h"
#include "section_linecalculate.h"
#include "section_tableview.h"
#include "section_wall_tablemodel.h"

namespace post3dapp{
SectionWallWidget::SectionWallWidget( const SectionDataType &type, QWidget *parent )
    : SectionTableWidget( type, parent )
{
}

void SectionWallWidget::createModels(){
    myTableModel = new SectionWallTableModel(mySectionType, this);
    SectionTableWidget::createModels();
}

void SectionWallWidget::createViews()
{
    myTableView = new SectionTableView(this);
    tableView_1 = new SectionTableView(this);
    if (mySectionType.getStructType()==STRUCTTYPE::STP_RC) {
        tableView_2 = new SectionTableView(this);
        tableView_3 = new SectionTableView(this);
    } else if(mySectionType.getStructType()==STRUCTTYPE::STP_S) {
        tableView_2 = nullptr;
        tableView_3 = nullptr;
    }

    SectionTableWidget::createViews();

    filter_columns = getDefaultFilters();
    ((SectionTableView *)myTableView)->setTableViewBorderColumns(mySectionType, filter_columns);
    tableView_1->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_2 != nullptr)tableView_2->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_3 != nullptr)tableView_3->setTableViewBorderColumns(mySectionType, filter_columns);

}

void SectionWallWidget::createWidgets()
{
    QStringList tablelabels;
    if (mySectionType.getStructType()==STRUCTTYPE::STP_RC) {
        tablelabels << u8"一般" << u8"配筋" << u8"開口補強筋（L,H≦1000）" << u8"開口補強筋（L,H＞1000）";
    } else if (mySectionType.getStructType()==STRUCTTYPE::STP_S) {
        tablelabels << u8"一般" << u8"リブ";
    }
    QScrollBar *sbar = myTableView->verticalScrollBar();
    tableView_1->setVerticalScrollBar(sbar);
    if (tableView_2 != nullptr){
        tableView_2->setVerticalScrollBar(sbar);
    }
    if (tableView_3 != nullptr)tableView_3->setVerticalScrollBar(sbar);

    QWidget *tv_wid = createTableLabel(myTableView, tablelabels.at(0));
    QWidget *tv1_wid = createTableLabel(tableView_1, tablelabels.at(1));
    QWidget *tv2_wid;
    if (tableView_2 != nullptr)tv2_wid = createTableLabel(tableView_2, tablelabels.at(2));
    QWidget *tv3_wid;
    if (tableView_3 != nullptr)tv3_wid = createTableLabel(tableView_3, tablelabels.at(3));

    QSplitter *h1_splitter = new QSplitter(Qt::Horizontal);
    h1_splitter->addWidget(tv_wid);
    h1_splitter->addWidget(tv1_wid);
    if (tableView_2 != nullptr)h1_splitter->addWidget(tv2_wid);
    if (tableView_3 != nullptr)h1_splitter->addWidget(tv3_wid);
    h1_splitter->setStretchFactor(0, 1);
    h1_splitter->setStretchFactor(1, 1);
    if (tableView_2 != nullptr)h1_splitter->setStretchFactor(2, 1);
    if (tableView_3 != nullptr)h1_splitter->setStretchFactor(3, 1);

    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->setSpacing(3);
    hlay1->addWidget(h1_splitter);
    hlay1->addWidget(sbar, 0);

    QVBoxLayout *vlay1 = new QVBoxLayout;
    vlay1->setContentsMargins(0, 0, 0, 0);
    vlay1->setSpacing(0);
    vlay1->addLayout(topLayout, 0);
    vlay1->addLayout(hlay1, 1);
    QFrame *view_frame = new QFrame(this);
    view_frame->setLayout(vlay1);


    lineButtons = new SectionLineButtons(mySectionType, filter_columns, this);
    lineCalc = new SectionLineCalculate(mySectionType, this);
    lineDraw = NULL;
    lineCalc->setGeometry(0, 0, 350, 400);
    connect (lineButtons, &SectionLineButtons::lineButtonsDataChanged,
             this, &SectionTableWidget::setDataMyModel);
    connect (lineButtons, &SectionLineButtons::currentLineNumberChanged,
             this, &SectionTableWidget::slotChangeCurrentRow);
    connect (this, &SectionTableWidget::filterColumnsChanged, lineButtons, &SectionLineButtons::slotChangedFilter);

    QScrollArea *scrollbuttons = new QScrollArea;
    QScrollArea *scrollcalc = new QScrollArea;
    scrollbuttons->setWidget(lineButtons);
    scrollcalc->setWidget(lineCalc);

    QSplitter *h2_splitter = new QSplitter(Qt::Horizontal);
    h2_splitter->addWidget(scrollbuttons);
    h2_splitter->addWidget(scrollcalc);
    h2_splitter->setStretchFactor(0, 3);
    h2_splitter->setStretchFactor(1, 2);

    QSplitter *v_splitter = new QSplitter(Qt::Vertical);
    v_splitter->addWidget(view_frame);
    v_splitter->addWidget(h2_splitter);
    v_splitter->setStretchFactor(0, 3);
    v_splitter->setStretchFactor(1, 2);

    QHBoxLayout *hlay3 = new QHBoxLayout;
    hlay3->addWidget(v_splitter);

    this->setLayout(hlay3);

}
} // namespace post3dapp
