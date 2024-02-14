﻿#include "section_gir_widget.h"

#include <QtGui>
#include <QDebug>

#include "section_filterdialog.h"
#include "section_gir_tablemodel.h"
#include "section_gir_linedraw.h"
#include "section_linebuttons.h"
#include "section_linecalculate.h"
#include "section_tableview.h"


namespace post3dapp{
SectionGirderWidget::SectionGirderWidget( const SectionDataType &type, QWidget *parent )
    : SectionTableWidget( type, parent )
{
}

void SectionGirderWidget::createModels(){
    myTableModel = new SectionGirderTableModel(mySectionType, this);
    SectionTableWidget::createModels();
}

void SectionGirderWidget::createViews()
{

    myTableView = new SectionTableView(this);
    tableView_1 = new SectionTableView(this);
    tableView_2 = new SectionTableView(this);
    tableView_3 = new SectionTableView(this);

    SectionTableWidget::createViews();

    filter_columns = getDefaultFilters();
    ((SectionTableView *)myTableView)->setTableViewBorderColumns(mySectionType, filter_columns);
    tableView_1->setTableViewBorderColumns(mySectionType, filter_columns);
    tableView_2->setTableViewBorderColumns(mySectionType, filter_columns);
    tableView_3->setTableViewBorderColumns(mySectionType, filter_columns);
}

void SectionGirderWidget::createWidgets()
{
    QScrollBar *sbar = myTableView->verticalScrollBar();
    tableView_1->setVerticalScrollBar(sbar);
    tableView_2->setVerticalScrollBar(sbar);
    tableView_3->setVerticalScrollBar(sbar);

    QWidget *tv1_wid = createTableLabel(myTableView, u8"一般");
    QWidget *tv2_wid = createTableLabel(tableView_1, u8"左端（i）");
    QWidget *tv3_wid = createTableLabel(tableView_2, u8"中央（c）");
    QWidget *tv4_wid = createTableLabel(tableView_3, u8"右端（j）");

    QSplitter *h1_splitter = new QSplitter(Qt::Horizontal);
    h1_splitter->addWidget(tv1_wid);
    h1_splitter->addWidget(tv2_wid);
    h1_splitter->addWidget(tv3_wid);
    h1_splitter->addWidget(tv4_wid);
    h1_splitter->setStretchFactor(0, 1);
    h1_splitter->setStretchFactor(1, 1);
    h1_splitter->setStretchFactor(2, 1);
    h1_splitter->setStretchFactor(3, 1);

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

    QScrollArea *tab_input = new QScrollArea;
    lineButtons = new SectionLineButtons(mySectionType, filter_columns, tab_input);
    tab_input->setWidget(lineButtons);
    connect (lineButtons, &SectionLineButtons::lineButtonsDataChanged,
             this, &SectionTableWidget::setDataMyModel);
    connect (lineButtons, &SectionLineButtons::currentLineNumberChanged,
             this, &SectionTableWidget::slotChangeCurrentRow);
    connect (this, &SectionTableWidget::filterColumnsChanged, lineButtons, &SectionLineButtons::slotChangedFilter);

    QScrollArea *tab_calc = new QScrollArea;
    lineCalc = new SectionLineCalculate(mySectionType, tab_calc);
    lineCalc->setGeometry(0, 0, 800, 400);
    tab_calc->setWidget(lineCalc);

    QTabWidget *tabs = new QTabWidget(this);
    tabs->setTabPosition(QTabWidget::West);
    tabs->addTab(tab_input, u8"個別断面入力");
    tabs->addTab(tab_calc, u8"断面データ");

    lineDraw = new SectionGirderLineDraw(mySectionType, this);
    lineDraw->setMinimumWidth(200);

    QSplitter *h2_splitter = new QSplitter(Qt::Horizontal);
    h2_splitter->addWidget(tabs);
    h2_splitter->addWidget(lineDraw);
    h2_splitter->setStretchFactor(0, 1);
    h2_splitter->setStretchFactor(1, 2);

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