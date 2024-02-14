#include "section_damper_widget.h"

#include <QtGui>
#include <QDebug>

#include "define_material_struct.h"
#include "define_section_struct.h"
#include "section_damper_tablemodel.h"
#include "section_filterdialog.h"
#include "section_tableview.h"
#include "unified_inputdata.h"

namespace post3dapp{
void SectionDamperWidget::createModels(){
    myTableModel = new SectionDamperTableModel(mySectionType, this);
    SectionTableWidget::createModels();
}

void SectionDamperWidget::createViews()
{

    myTableView = new SectionTableView(this);
    tableView_1 = nullptr;
    tableView_2 = nullptr;
    tableView_3 = nullptr;

    SectionTableWidget::createViews();

    filter_columns = getDefaultFilters();
    ((SectionTableView *)myTableView)->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_1 != nullptr)tableView_1->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_2 != nullptr)tableView_2->setTableViewBorderColumns(mySectionType, filter_columns);
    if (tableView_3 != nullptr)tableView_3->setTableViewBorderColumns(mySectionType, filter_columns);

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    Q_FOREACH (int tag, coltag) {
        if (coltag.key(tag).contains("VECTOR")) {
            myTableView->setColumnWidth(tag, 75);
        }
        if (coltag.key(tag).contains("V") && coltag.key(tag).length() <= 3) {
            myTableView->setColumnWidth(tag, 75);
        }
        if (coltag.key(tag).contains("PRODUCTTYPE")) {
            myTableView->setColumnWidth(tag, 220);
        }
    }
}

void SectionDamperWidget::createWidgets()
{
    QStringList tablelabels{u8"制振要素諸元"};
    QScrollBar *sbar = myTableView->verticalScrollBar();
    if (tableView_1 != nullptr)tableView_1->setVerticalScrollBar(sbar);
    if (tableView_2 != nullptr)tableView_2->setVerticalScrollBar(sbar);
    if (tableView_3 != nullptr)tableView_3->setVerticalScrollBar(sbar);

    QWidget *tv1_wid = createTableLabel(myTableView, tablelabels.at(0));
    QWidget *tv2_wid;
    QWidget *tv3_wid;
    QWidget *tv4_wid;
    if (tableView_1 != nullptr)tv2_wid = createTableLabel(tableView_1, tablelabels.at(1));
    if (tableView_2 != nullptr)tv3_wid = createTableLabel(tableView_2, tablelabels.at(2));
    if (tableView_3 != nullptr)tv4_wid = createTableLabel(tableView_3, tablelabels.at(3));

    QSplitter *h1_splitter = new QSplitter(Qt::Horizontal);
    h1_splitter->addWidget(tv1_wid);
    if (tableView_1 != nullptr)h1_splitter->addWidget(tv2_wid);
    if (tableView_2 != nullptr)h1_splitter->addWidget(tv3_wid);
    if (tableView_3 != nullptr)h1_splitter->addWidget(tv4_wid);
    h1_splitter->setStretchFactor(0, 1);
    if (tableView_1 != nullptr)h1_splitter->setStretchFactor(1, 1);
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

    lineButtons = nullptr;
    lineCalc = nullptr;
    lineDraw = nullptr;

    QHBoxLayout *hlay3 = new QHBoxLayout;
    hlay3->addWidget(view_frame);

    this->setLayout(hlay3);

}

void SectionDamperWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPDAMPER)){
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(QString::number(static_cast<int>(sv.damper_param.productType))));
        data.append(QVariant(QString::number(sv.damper_param.selfWeight)));
        data.append(QVariant(QString::number(sv.damper_param.vecX)));
        data.append(QVariant(QString::number(sv.damper_param.vecY)));
        data.append(QVariant(QString::number(sv.damper_param.vecZ)));
        data.append(QVariant(QString{}));
        QString p;
        Q_FOREACH(auto para, sv.damper_param.parameters){
            p.append(QString::number(para));
            p.append(",");
        }
        if (p != QString{})
            p.chop(1);
        data.append(QVariant(p));
        data.append(QVariant(QString::number(sv.damper_param.period)));
        data.append(QVariant(QString::number(sv.damper_param.maxDisp)));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionDamperWidget::getDefaultFilters() const{
    QList<bool> damperDefault;
    damperDefault << true; // NO
    damperDefault << true; // LABEL
    damperDefault << true; // PRODUCTTYPE
    damperDefault << true; // SELFWEIGHT
    damperDefault << true; // VECTOR_X
    damperDefault << true; // VECTOR_Y
    damperDefault << true; // VECTOR_Z
    damperDefault << true; // V1
    damperDefault << true; // V2
    damperDefault << true; // V3
    damperDefault << true; // V4
    damperDefault << true; // V5
    damperDefault << true; // V6
    damperDefault << true; // V7
    damperDefault << true; // NATURALPERIOD
    damperDefault << true; // MAXDISPLACEMENT
    damperDefault << true; // MODEL FLAG
    damperDefault << true; // RIGID LENGTH
    damperDefault << false; // DAMPER DIRECTION
    damperDefault << false; // T ANALYSIS
    damperDefault << false; // FACTOR
    damperDefault << false; // FRAME RIGIDITY
    return damperDefault;
}
} // namespace post3dapp
