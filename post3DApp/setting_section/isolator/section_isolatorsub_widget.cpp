#include "section_isolatorsub_widget.h"

#include <QtGui>
#include <QDebug>

#include "customtable_commands.h"
#include "customtable_view.h"
#include "define_section_struct.h"
#include "unified_inputdata.h"

namespace post3dapp{
void SectionIsolatorSubWidget::createViews()
{
    SectionIsolatorWidget::createViews();

    QHash<QString, int> coltag = mySectionType.getTaggedColumns();
    myTableView->setColumnWidth(coltag["PRODUCTTYPE"], 120);
    myTableView->setColumnWidth(coltag["PRODUCTNAME"], 150);
    myTableView->setColumnWidth(coltag["HYSTERESIS"], 120);
}

void SectionIsolatorSubWidget::reflectUnifiedData(){
    DoubleList temp(myTableModel->columnCount());
    int i = 0;
    Q_FOREACH(SectionValues sv, UnifiedInputData::getInInstance()->getSectionValueList(DATATYPE::TPISO)){
        QList<QVariant> data;
        data.append(QVariant(i));
        data.append(QVariant(sv.sectionName));
        data.append(QVariant(sv.isolator_param.productType));
        data.append(QVariant(sv.isolator_param.productName));
        data.append(QVariant()); // 復元力特性
        data.append(QVariant()); // K1
        data.append(QVariant()); // K2
        data.append(QVariant()); // K3
        data.append(QVariant()); // Fy1+
//        data.append(QVariant()); // Fy1-
        data.append(QVariant()); // Fy2+
//        data.append(QVariant()); // Fy2-
        data.append(QVariant()); // Fyv+
        data.append(QVariant(QString::number(sv.isolator_param.selfWeight)));
        data.append(QVariant(sv.isolator_param.renderShape));
        data.append(QVariant(QString::number(sv.isolator_param.sizeD))); //
        data.append(QVariant(QString::number(sv.isolator_param.sizeH))); //
        data.append(QVariant(QString::number(sv.isolator_param.settingLevel))); //
        temp.appendRowData(data);
        i++;
    }
    myTableModel->reflectUnifiedData(temp);
    emit sectionItemChanged();
    slotChangeCurrentRow(0);
    undoStack->clear();
}

QList<bool> SectionIsolatorSubWidget::getDefaultFilters() const{
    QList<bool> isolatorDefault;
    isolatorDefault << true; // NO
    isolatorDefault << true; // LABEL
    isolatorDefault << true; // PRODUCTTYPE
    isolatorDefault << true; // PRODUCTNAME
    isolatorDefault << true; // SHEARHYSTERESIS
    isolatorDefault << true; // hK1
    isolatorDefault << true; // hK2
    isolatorDefault << true; // hK3
    isolatorDefault << true; // hFY1_PLUS
//    isolatorDefault << true; // hFY1_MINUS
    isolatorDefault << true; // hFY2_PLUS
//    isolatorDefault << true; // hFY2_MINUS
    isolatorDefault << true; // AXIALHYSTERESIS
    isolatorDefault << true; // vK1
    isolatorDefault << true; // vK2
    isolatorDefault << true; // vK3
    isolatorDefault << true; // vFY2_PLUS
    isolatorDefault << true; // SELFWEIGHT
    isolatorDefault << true; // RENDERSHAPE
    isolatorDefault << true; // RENDERSIZED
    isolatorDefault << true; // RENDERSIZEH
    isolatorDefault << true; // SETLEVEL
    isolatorDefault << true; // DEPLACEMENT
    isolatorDefault << true; // SPRING
    isolatorDefault << true; // INFLACTION
    return isolatorDefault;
}
} // namespace post3dapp
