#include "section_opening_widget.h"

#include <QtGui>
#include <QDebug>

#include "define_section_struct.h"
#include "section_filterdialog.h"
#include "section_opening_tablemodel.h"
#include "section_tableview.h"
#include "unified_inputdata.h"

namespace post3dapp{
void SectionOpeningWidget::createModels(){
    myTableModel = new SectionOpeningTableModel(mySectionType, this);
    SectionTableWidget::createModels();
}

void SectionOpeningWidget::createViews()
{

    myTableView = new SectionTableView(this);
    tableView_1 = nullptr;
    tableView_2 = nullptr;
    tableView_3 = nullptr;

    SectionTableWidget::createViews();
    filter_columns = getDefaultFilters();
    ((SectionTableView *)myTableView)->setTableViewBorderColumns(mySectionType, filter_columns);
}

void SectionOpeningWidget::createWidgets()
{
    QStringList tablelabels{u8"開口諸元"};
    QScrollBar *sbar = myTableView->verticalScrollBar();

    QWidget *tv1_wid = createTableLabel(myTableView, tablelabels.at(0));

    QSplitter *h1_splitter = new QSplitter(Qt::Horizontal);
    h1_splitter->addWidget(tv1_wid);

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

void SectionOpeningWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPWALLOPEN)){
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(QString::number(static_cast<int>(sv.wopen_param.openType))));
        data.append(QVariant(QString::number(sv.wopen_param.isHorizontalFace)));
        data.append(QVariant(QString::number(static_cast<int>(sv.wopen_param.hPosition))));
        data.append(QVariant(QString::number(sv.wopen_param.isVerticalFace)));
        data.append(QVariant(QString::number(static_cast<int>(sv.wopen_param.vPosition))));
        data.append(QVariant(QString::number(sv.wopen_param.ox)));
        data.append(QVariant(QString::number(sv.wopen_param.oy)));
        data.append(QVariant(QString::number(sv.wopen_param.width)));
        data.append(QVariant(QString::number(sv.wopen_param.height)));
        data.append(QVariant(QString::number(sv.wopen_param.slitL)));
        data.append(QVariant(QString::number(sv.wopen_param.slitR)));
        data.append(QVariant(QString::number(sv.wopen_param.slitB)));
        data.append(QVariant(QString::number(sv.wopen_param.slitT)));
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionOpeningWidget::getDefaultFilters() const{
    QList<bool> openingDefault;
    openingDefault << true; // NO
    openingDefault << true; // LABEL
    openingDefault << true; // OPENINGTYPE
    openingDefault << true; // LRBASE
    openingDefault << true; // LR
    openingDefault << true; // UDBASE
    openingDefault << true; // UD
    openingDefault << true; // POSITIONX
    openingDefault << true; // POSITIONY
    openingDefault << true; // WIDTH
    openingDefault << true; // HEIGHT
    openingDefault << true; // SLITL
    openingDefault << true; // SLITR
    openingDefault << true; // SLITB
    openingDefault << true; // SLITT
    openingDefault << true; // COMMENT
    return openingDefault;
}
} // namespace post3dapp
