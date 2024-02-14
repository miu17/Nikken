#include "section_uwall_widget.h"

#include <QtGui>
#include <QDebug>

#include "section_filterdialog.h"
#include "unified_inputdata.h"
#include "section_linebuttons.h"
#include "section_linecalculate.h"
#include "section_tableview.h"
#include "section_uwall_tablemodel.h"

namespace post3dapp{
SectionUnderGroundWallWidget::SectionUnderGroundWallWidget( const SectionDataType &type, QWidget *parent )
    : SectionTableWidget( type, parent )
{
    createModels();
    createViews();
    createWidgets();

}

void SectionUnderGroundWallWidget::createModels(){
    myTableModel = new SectionUWallTableModel(mySectionType, this);
    SectionTableWidget::createModels();
}

void SectionUnderGroundWallWidget::createViews()
{
    myTableView = new SectionTableView(this);
    tableView_1 = new SectionTableView(this);
    if(mySectionType.getMemberType() == DATATYPE::TPUWALL) {
        tableView_2 = new SectionTableView(this);
        tableView_3 = nullptr;
    }

    SectionTableWidget::createViews();

    filter_columns = getDefaultFilters();
    ((SectionTableView *)myTableView)->setTableViewBorderColumns(mySectionType, filter_columns);
    tableView_1->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_2 != nullptr)tableView_2->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_3 != nullptr)tableView_3->setTableViewBorderColumns(mySectionType, filter_columns);


    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    Q_FOREACH (int tag, coltag) {
        if (coltag.key(tag).contains("BAR")) {
            tableView_1->setColumnWidth(tag, 75);
            tableView_2->setColumnWidth(tag, 75);
        }
    }
}

void SectionUnderGroundWallWidget::createWidgets()
{
    QStringList tablelabels;
    if (mySectionType.getMemberType() == DATATYPE::TPUWALL) {
        tablelabels << u8"一般" << u8"外側配筋" << u8"内側配筋";
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
void SectionUnderGroundWallWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPUWALL)){
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(sv.plane_param.concMaterial));
        data.append(QVariant(QString::number(sv.plane_param.concThickness)));
        data.append(QVariant(sv.plane_param.steelMaterial));
        data.append(QVariant(QString::number(sv.plane_param.steelThickness)));
        data.append(QVariant(QString::number(sv.plane_param.finishLoad)));
        data.append(QVariant()); // 標準径D
        data.append(QVariant()); // 標準ピッチ@
        data.append(QVariant(sv.plane_param.vBar_outerTop.phi));
        data.append(QVariant(QString::number(sv.plane_param.vBar_outerTop.pitch)));
        data.append(QVariant(sv.plane_param.vBar_outerMiddle.phi));
        data.append(QVariant(QString::number(sv.plane_param.vBar_outerMiddle.pitch)));
        data.append(QVariant(sv.plane_param.vBar_outerBottom.phi));
        data.append(QVariant(QString::number(sv.plane_param.vBar_outerBottom.pitch)));
        data.append(QVariant(sv.plane_param.vBar_innerTop.phi));
        data.append(QVariant(QString::number(sv.plane_param.vBar_innerTop.pitch)));
        data.append(QVariant(sv.plane_param.hBar_outerEdge.phi));
        data.append(QVariant(QString::number(sv.plane_param.hBar_outerEdge.pitch)));
        data.append(QVariant(sv.plane_param.hBar_outerCenter.phi));
        data.append(QVariant(QString::number(sv.plane_param.hBar_outerCenter.pitch)));
        data.append(QVariant(sv.plane_param.vBar_innerMiddle.phi));
        data.append(QVariant(QString::number(sv.plane_param.vBar_innerMiddle.pitch)));
        data.append(QVariant(sv.plane_param.vBar_innerBottom.phi));
        data.append(QVariant(QString::number(sv.plane_param.vBar_innerBottom.pitch)));
        data.append(QVariant(sv.plane_param.hBar_innerEdge.phi));
        data.append(QVariant(QString::number(sv.plane_param.hBar_innerEdge.pitch)));
        data.append(QVariant(sv.plane_param.hBar_innerCenter.phi));
        data.append(QVariant(QString::number(sv.plane_param.hBar_innerCenter.pitch)));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionUnderGroundWallWidget::getDefaultFilters() const
{
    QList<bool> uwallDefault;
    uwallDefault << true; // NO
    uwallDefault << true; // LABEL
    uwallDefault << true; // CONC
    uwallDefault << true; // CONCTHICKNESS
    uwallDefault << false; // STEEL
    uwallDefault << false; // STEELTHICKNESS
    uwallDefault << true; // FINISHLOAD
    uwallDefault << true; // STANDARDBARPHI
    uwallDefault << true; // STANDARDBARPITCH
    uwallDefault << true; // vOUTERTOP_BARPHI
    uwallDefault << true; // vOUTERTOP_BARPITCH
    uwallDefault << true; // vOUTERMIDDLE_BARPHI
    uwallDefault << true; // vOUTERMIDDLE_BARPITCH
    uwallDefault << true; // vOUTERBOTTOM_BARPHI
    uwallDefault << true; // vOUTERBOTTOM_BARPITCH
    uwallDefault << true; // hOUTEREDGE_BARPHI
    uwallDefault << true; // hOUTEREDGE_BARPITCH
    uwallDefault << true; // hOUTERCENTER_BARPHI
    uwallDefault << true; // hOUTERCENTER_BARPITCH
    uwallDefault << true; // vINNERTOP_BARPHI
    uwallDefault << true; // vINNERTOP_BARPITCH
    uwallDefault << true; // vINNERMIDDLE_BARPHI
    uwallDefault << true; // vINNERMIDDLE_BARPITCH
    uwallDefault << true; // vINNERBOTTOM_BARPHI
    uwallDefault << true; // vINNERBOTTOM_BARPITCH
    uwallDefault << true; // hINNEREDGE_BARPHI
    uwallDefault << true; // hINNEREDGE_BARPITCH
    uwallDefault << true; // hINNERCENTER_BARPHI
    uwallDefault << true; // hINNERCENTER_BARPITCH
    return uwallDefault;
};
} // namespace post3dapp
