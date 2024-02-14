#include "section_base_widget.h"

#include <QtGui>
#include <QDebug>

#include "section_base_linebuttons.h"
#include "section_base_linedraw.h"
#include "section_base_tablemodel.h"
#include "section_filterdialog.h"
#include "section_tableview.h"


namespace post3dapp{
SectionBaseWidget::SectionBaseWidget( const SectionDataType &type, QWidget *parent )
    : SectionTableWidget( type, parent )
{
}

void SectionBaseWidget::createModels(){
    myTableModel = new SectionBaseTableModel(mySectionType, this);
    SectionTableWidget::createModels();
}

void SectionBaseWidget::createViews()
{
    myTableView = new SectionTableView(this);
    tableView_1 = new SectionTableView(this);
    tableView_2 = new SectionTableView(this);
    if (mySectionType.getStructType() == STRUCTTYPE::STP_S) {
        tableView_3 = new SectionTableView(this);
    } else {
        tableView_3 = NULL;
    }

    SectionTableWidget::createViews();
    filter_columns = getDefaultFilters();
    ((SectionTableView *)myTableView)->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_1 != NULL)tableView_1->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_2 != NULL)tableView_2->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_3 != NULL)tableView_3->setTableViewBorderColumns(mySectionType, filter_columns);

}

void SectionBaseWidget::createWidgets()
{
    QStringList tablelabels;
    if (mySectionType.getStructType() == STRUCTTYPE::STP_S) {
        tablelabels << u8"一般" << u8"ベースプレート" << u8"アンカーボルト" << u8"基礎柱";
    } else if (mySectionType.getStructType() == STRUCTTYPE::STP_SRC) {
        tablelabels << u8"一般" << u8"ベースプレート" << u8"アンカーボルト";
    }
    QScrollBar *sbar = myTableView->verticalScrollBar();
    if (tableView_1 != NULL)tableView_1->setVerticalScrollBar(sbar);
    if (tableView_2 != NULL)tableView_2->setVerticalScrollBar(sbar);
    if (tableView_3 != NULL)tableView_3->setVerticalScrollBar(sbar);

    QWidget *tv1_wid = createTableLabel(myTableView, tablelabels.at(0));
    QWidget *tv2_wid;
    QWidget *tv3_wid;
    QWidget *tv4_wid;
    if (tableView_1 != NULL)tv2_wid = createTableLabel(tableView_1, tablelabels.at(1));
    if (tableView_2 != NULL)tv3_wid = createTableLabel(tableView_2, tablelabels.at(2));
    if (tableView_3 != NULL)tv4_wid = createTableLabel(tableView_3, tablelabels.at(3));

    QSplitter *h1_splitter = new QSplitter(Qt::Horizontal);
    h1_splitter->addWidget(tv1_wid);
    if (tableView_1 != NULL)h1_splitter->addWidget(tv2_wid);
    if (tableView_2 != NULL)h1_splitter->addWidget(tv3_wid);
    if (tableView_3 != NULL)h1_splitter->addWidget(tv4_wid);
    h1_splitter->setStretchFactor(0, 1);
    if (tableView_1 != NULL)h1_splitter->setStretchFactor(1, 1);
    if (tableView_2 != NULL)h1_splitter->setStretchFactor(2, 1);
    if (tableView_3 != NULL)h1_splitter->setStretchFactor(3, 1);

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

    QScrollArea *scrollbuttons = new QScrollArea;
    lineButtons = new SectionBaseLineButtons(mySectionType, filter_columns, scrollbuttons);
    connect (this, &SectionTableWidget::filterColumnsChanged, lineButtons, &SectionLineButtons::slotChangedFilter);

    scrollbuttons->setWidget(lineButtons);
    connect (lineButtons, &SectionLineButtons::lineButtonsDataChanged,
             this, &SectionTableWidget::setDataMyModel);
    connect (lineButtons, &SectionLineButtons::currentLineNumberChanged,
             this, &SectionTableWidget::slotChangeCurrentRow);

    lineCalc = NULL; //new SectionLineCalculate(mySectionType,tab_calc);
    lineDraw = new SectionBaseLineDraw(mySectionType, this);
    lineDraw->setMinimumWidth(200);
    lineDraw->setGeometry(0, 0, 200, 200);


    QSplitter *h2_splitter = new QSplitter(Qt::Horizontal);
    h2_splitter->addWidget(scrollbuttons);
    h2_splitter->addWidget(lineDraw);
    h2_splitter->setStretchFactor(0, 1);
    h2_splitter->setStretchFactor(1, 1);

    QSplitter *v_splitter = new QSplitter(Qt::Vertical);
    v_splitter->addWidget(view_frame);
    v_splitter->addWidget(h2_splitter);
    v_splitter->setStretchFactor(0, 1);
    v_splitter->setStretchFactor(1, 1);

    QHBoxLayout *hlay3 = new QHBoxLayout;
    hlay3->addWidget(v_splitter);

    this->setLayout(hlay3);

}
} // namespace post3dapp
