#include "section_slab_widget.h"
#include <QtGui>
#include <QDebug>

#include "define_section_struct.h"
#include "section_filterdialog.h"
#include "section_linebuttons.h"
#include "section_linecalculate.h"
#include "section_slab_tablemodel.h"
#include "section_tableview.h"
#include "unified_inputdata.h"

namespace post3dapp{
void SectionSlabWidget::createModels(){
    myTableModel = new SectionSlabTableModel(mySectionType, this);
    SectionTableWidget::createModels();
}

void SectionSlabWidget::createViews()
{

    myTableView = new SectionTableView(this);
    tableView_1 = new SectionTableView(this);
    tableView_2 = new SectionTableView(this);
    tableView_3 = NULL;

    SectionTableWidget::createViews();

    filter_columns = getDefaultFilters();
    ((SectionTableView *)myTableView)->setTableViewBorderColumns(mySectionType, filter_columns);
    tableView_1->setTableViewBorderColumns(mySectionType, filter_columns);
    tableView_2->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_3 != NULL)tableView_3->setTableViewBorderColumns(mySectionType, filter_columns);

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    Q_FOREACH (int tag, coltag) {
        if (coltag.key(tag).contains("BAR") && !coltag.key(tag).contains("BARMATERIAL")) {
            myTableView->setColumnWidth(tag, 75);
            tableView_1->setColumnWidth(tag, 75);
            tableView_2->setColumnWidth(tag, 75);
            if (tableView_3 != NULL)tableView_3->setColumnWidth(tag, 75);
        }
    }

}

void SectionSlabWidget::createWidgets()
{
    QStringList tablelabels{u8"一般", u8"短辺方向配筋", u8"長辺方向配筋"};
    QScrollBar *sbar = myTableView->verticalScrollBar();
    tableView_1->setVerticalScrollBar(sbar);
    tableView_2->setVerticalScrollBar(sbar);
    if (tableView_3 != NULL)tableView_3->setVerticalScrollBar(sbar);

    QWidget *tv1_wid = createTableLabel(myTableView, tablelabels.at(0));
    QWidget *tv2_wid = createTableLabel(tableView_1, tablelabels.at(1));
    QWidget *tv3_wid = createTableLabel(tableView_2, tablelabels.at(2));
    QWidget *tv4_wid;
    if (tableView_3 != NULL)tv4_wid = createTableLabel(tableView_3, tablelabels.at(3));

    QSplitter *h1_splitter = new QSplitter(Qt::Horizontal);
    h1_splitter->addWidget(tv1_wid);
    h1_splitter->addWidget(tv2_wid);
    h1_splitter->addWidget(tv3_wid);
    if (tableView_3 != NULL)h1_splitter->addWidget(tv4_wid);
    h1_splitter->setStretchFactor(0, 1);
    h1_splitter->setStretchFactor(1, 1);
    h1_splitter->setStretchFactor(2, 1);
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


    lineButtons = new SectionLineButtons(mySectionType, filter_columns, this);
    lineCalc = new SectionLineCalculate(mySectionType, this);
    lineDraw = NULL;
    lineCalc->setGeometry(0, 0, 390, 200);
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
    v_splitter->setStretchFactor(0, 4);
    v_splitter->setStretchFactor(1, 2);

    QHBoxLayout *hlay3 = new QHBoxLayout;
    hlay3->addWidget(v_splitter);

    this->setLayout(hlay3);

}

void SectionSlabWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPSLAB)){
        if (sv.sectionType != STRUCTTYPE::STP_RC)
            continue;
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(PlaneType::enumToString(sv.plane_param.planeType)));
        data.append(QVariant(sv.plane_param.concMaterial));
        data.append(QVariant(QString::number(sv.plane_param.concThickness)));
        data.append(QVariant(sv.plane_param.steelMaterial));
        data.append(QVariant(QString::number(sv.plane_param.steelThickness)));
        data.append(QVariant(QString::number(sv.plane_param.finishLoad)));
        data.append(QVariant(QString::number(sv.plane_param.loadDir)));
        data.append(QVariant()); // 標準径D
        data.append(QVariant()); // 標準ピッチ@
        data.append(QVariant(sv.plane_param.sBar_upperEdgeA.phi));
        data.append(QVariant(QString::number(sv.plane_param.sBar_upperEdgeA.pitch)));
        data.append(QVariant(sv.plane_param.sBar_lowerEdgeA.phi));
        data.append(QVariant(QString::number(sv.plane_param.sBar_lowerEdgeA.pitch)));
        data.append(QVariant(sv.plane_param.sBar_upperCenterA.phi));
        data.append(QVariant(QString::number(sv.plane_param.sBar_upperCenterA.pitch)));
        data.append(QVariant(sv.plane_param.sBar_lowerCenterA.phi));
        data.append(QVariant(QString::number(sv.plane_param.sBar_lowerCenterA.pitch)));
        data.append(QVariant(sv.plane_param.sBar_upperEdgeCenterB.phi));
        data.append(QVariant(QString::number(sv.plane_param.sBar_upperEdgeCenterB.pitch)));
        data.append(QVariant(sv.plane_param.sBar_lowerEdgeCenterB.phi));
        data.append(QVariant(QString::number(sv.plane_param.sBar_lowerEdgeCenterB.pitch)));

        data.append(QVariant(sv.plane_param.lBar_upperEdgeA.phi));
        data.append(QVariant(QString::number(sv.plane_param.lBar_upperEdgeA.pitch)));
        data.append(QVariant(sv.plane_param.lBar_lowerEdgeA.phi));
        data.append(QVariant(QString::number(sv.plane_param.lBar_lowerEdgeA.pitch)));
        data.append(QVariant(sv.plane_param.lBar_upperCenterA.phi));
        data.append(QVariant(QString::number(sv.plane_param.lBar_upperCenterA.pitch)));
        data.append(QVariant(sv.plane_param.lBar_lowerCenterA.phi));
        data.append(QVariant(QString::number(sv.plane_param.lBar_lowerCenterA.pitch)));
        data.append(QVariant(sv.plane_param.lBar_upperEdgeCenterB.phi));
        data.append(QVariant(QString::number(sv.plane_param.lBar_upperEdgeCenterB.pitch)));
        data.append(QVariant(sv.plane_param.lBar_lowerEdgeCenterB.phi));
        data.append(QVariant(QString::number(sv.plane_param.lBar_lowerEdgeCenterB.pitch)));

        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionSlabWidget::getDefaultFilters() const{
    QList<bool> systemDefault;
    systemDefault << true; // NO
    systemDefault << true; // LABEL
    systemDefault << true; // SLABTYPE
    systemDefault << true; // CONC
    systemDefault << true; // CONCTHICKNESS
    systemDefault << false; // STEEL
    systemDefault << false; // STEELTHICKNESS
    systemDefault << true; // FINISHLOAD
    systemDefault << true; // LISTLOADDIRECTION
    systemDefault << true; // STANDARDBARPHI
    systemDefault << true; // STANDARDBARPITCH
    systemDefault << true; // sUPPEREDGE_A_BARPHI
    systemDefault << true; // sUPPEREDGE_A_BARPITCH
    systemDefault << true; // sLOWEREDGE_A_BARPHI
    systemDefault << true; // sLOWEREDGE_A_BARPITCH
    systemDefault << true; // sUPPERCENTER_A_BARPHI
    systemDefault << true; // sUPPERCENTER_A_BARPITCH
    systemDefault << true; // sLOWERCENTER_A_BARPHI
    systemDefault << true; // sLOWERCENTER_A_BARPITCH
    systemDefault << true; // sUPPER_B_BARPHI
    systemDefault << true; // sUPPER_B_BARPITCH
    systemDefault << true; // sLOWER_B_BARPHI
    systemDefault << true; // sLOWER_B_BARPITCH
    systemDefault << true; // lUPPEREDGE_A_BARPHI
    systemDefault << true; // lUPPEREDGE_A_BARPITCH
    systemDefault << true; // lLOWEREDGE_A_BARPHI
    systemDefault << true; // lLOWEREDGE_A_BARPITCH
    systemDefault << true; // lUPPERCENTER_A_BARPHI
    systemDefault << true; // lUPPERCENTER_A_BARPITCH
    systemDefault << true; // lLOWERCENTER_A_BARPHI
    systemDefault << true; // lLOWERCENTER_A_BARPITCH
    systemDefault << true; // lUPPER_B_BARPHI
    systemDefault << true; // lUPPER_B_BARPITCH
    systemDefault << true; // lLOWER_B_BARPHI
    systemDefault << true; // lLOWER_B_BARPITCH
    return systemDefault;
}
} // namespace post3dapp
