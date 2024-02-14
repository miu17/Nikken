#include "section_isolator_widget.h"

#include <QtGui>
#include <QDebug>

#include "define_sectiontable.h"
#include "section_filterdialog.h"
#include "section_isolator_linedraw.h"
#include "section_isolator_tablemodel.h"
#include "section_linecalculate.h"
#include "section_tableview.h"

namespace post3dapp{
SectionIsolatorWidget::SectionIsolatorWidget( const SectionDataType &type, QWidget *parent )
    : SectionTableWidget( type, parent )
{
}


void SectionIsolatorWidget::createModels(){
    myTableModel = new SectionIsolatorTableModel(mySectionType, this);
    SectionTableWidget::createModels();
}

void SectionIsolatorWidget::createViews()
{

    myTableView = new SectionTableView(this);
    tableView_1 = NULL;
    tableView_2 = NULL;
    tableView_3 = NULL;

    SectionTableWidget::createViews();

    filter_columns = getDefaultFilters();
    ((SectionTableView *)myTableView)->setTableViewBorderColumns(mySectionType,
                                                                 filter_columns); //型キャスト
    if (tableView_1 != NULL)tableView_1->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_2 != NULL)tableView_2->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_3 != NULL)tableView_3->setTableViewBorderColumns(mySectionType, filter_columns);
}

void SectionIsolatorWidget::createWidgets()
{
    QScrollBar *sbar = myTableView->verticalScrollBar();
    if (tableView_1 != NULL)tableView_1->setVerticalScrollBar(sbar);
    if (tableView_2 != NULL)tableView_2->setVerticalScrollBar(sbar);
    if (tableView_3 != NULL)tableView_3->setVerticalScrollBar(sbar);

    QWidget *tv1_wid, *tv2_wid, *tv3_wid, *tv4_wid;
    tv1_wid = createTableLabel(myTableView, u8"免震装置諸元");
    if (tableView_1 != NULL)tv2_wid = createTableLabel(tableView_1, "");
    if (tableView_2 != NULL)tv3_wid = createTableLabel(tableView_2, "");
    if (tableView_3 != NULL)tv4_wid = createTableLabel(tableView_3, "");

    QSplitter *h1_splitter = new QSplitter(Qt::Horizontal);
    h1_splitter->addWidget(tv1_wid);
    if (tableView_1 != NULL)h1_splitter->addWidget(tv2_wid);
    if (tableView_2 != NULL)h1_splitter->addWidget(tv3_wid);
    if (tableView_3 != NULL)h1_splitter->addWidget(tv4_wid);

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

    lineButtons = NULL;

    QScrollArea *scrollcalc = new QScrollArea;
    lineCalc = new SectionLineCalculate(mySectionType, scrollcalc);
    lineCalc->setGeometry(0, 0, 650, 400);
    scrollcalc->setWidget(lineCalc);

    lineDraw = new SectionIsolatorLineDraw(mySectionType, this);
    lineDraw->setMinimumWidth(200);

    QSplitter *h2_splitter = new QSplitter(Qt::Horizontal);
    h2_splitter->addWidget(scrollcalc);
    h2_splitter->addWidget(lineDraw);
    h2_splitter->setStretchFactor(0, 2);
    h2_splitter->setStretchFactor(1, 7);

    QSplitter *v_splitter = new QSplitter(Qt::Vertical);
    v_splitter->addWidget(view_frame);
    v_splitter->addWidget(h2_splitter);
    v_splitter->setStretchFactor(0, 2);
    v_splitter->setStretchFactor(1, 1);

    QHBoxLayout *hlay3 = new QHBoxLayout;
    hlay3->addWidget(v_splitter);

    this->setLayout(hlay3);

}

} // namespace post3dapp
