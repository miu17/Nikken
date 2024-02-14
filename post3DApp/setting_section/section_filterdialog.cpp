#include "section_filterdialog.h"

#include <QtGui>
#include "define_sectiontable.h"
#include "section_tablewidget.h"

namespace post3dapp{
SectionFilterDialog::SectionFilterDialog(const QList<SectionTableWidget *>& tabelWidgets, QWidget * parent)
    : QDialog(parent)
{
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));

    Q_FOREACH(SectionTableWidget* tableWidget, tabelWidgets){
        sectionTypeList.append(tableWidget->getSectionDataType());
        myTempDefaultFilter.insert(tableWidget->getName(), tableWidget->getDefaultFilters());
        myTempFilter.insert(tableWidget->getName(), tableWidget->getDefaultFilters());
        filterKeys.append(tableWidget->getName());
    }
}


void SectionFilterDialog::createLayout()
{
    QPushButton *okButton = new QPushButton("OK", this);
    connect(okButton, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_accept);
    QPushButton *clickedButton = new QPushButton(u8"キャンセル", this);
    connect(clickedButton, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_reject);

    QHBoxLayout *menubuttons = new QHBoxLayout;
    menubuttons->setSpacing(3);
    menubuttons->addWidget(okButton);
    menubuttons->addWidget(clickedButton);
    menubuttons->addStretch(1);

    QGroupBox *instantswitch = new QGroupBox(u8"一括切り替え");
    instantswitch->setLayout(createSwitchLayout());
    instantswitch->setContentsMargins(5, 5, 5, 5);

    //トップレイアウト
    QVBoxLayout *toplayout = new QVBoxLayout;
    toplayout->addLayout(menubuttons);
    toplayout->addWidget(createTab());
    toplayout->addWidget(instantswitch);
    setLayout(toplayout);
}

QTabWidget* SectionFilterDialog::createTab(){
    QTabWidget *tabwidget = new QTabWidget(this);
    for (int itp = 0; itp < sectionTypeList.count(); ++itp) {
        SectionDataType i_dtype = sectionTypeList.at(itp);
        DATATYPE i_mtype = i_dtype.getMemberType();
        STRUCTTYPE i_stype = i_dtype.getStructType();

        QScrollArea *tab = new QScrollArea;
        QVBoxLayout *selectrows = new QVBoxLayout;
        selectrows->setSpacing(0);
        selectrows->setContentsMargins(0, 0, 0, 0);
        QStringList header = i_dtype.getHeaderStrings();
        QList<int> sp;
        sp << i_dtype.getSeparateColumn1()
           << i_dtype.getSeparateColumn2()
           << i_dtype.getSeparateColumn3();
        for (int i = 0; i < i_dtype.count(); ++i) { //0番の要素（No.）はスキップ
            if (i == 0 || sp.contains(i)) {
                QString labeltext;
                if (i == 0) {
                    labeltext = u8"一般";
                } else if ( i_mtype == DATATYPE::TPCOLUMN ) {
                    if (i == sp.at(0)) {
                        labeltext = u8"柱脚（i）";
                    } else if (i == sp.at(1)) {
                        labeltext = u8"柱脚（j）";
                    }
                } else if ( i_mtype == DATATYPE::TPGIRDER || i_mtype == DATATYPE::TPBEAM || i_mtype == DATATYPE::TPBRACE ) {
                    if (i == sp.at(0)) {
                        labeltext = u8"左端（i）";
                    } else if (i == sp.at(1)) {
                        labeltext = u8"中央（c）";
                    } else if (i == sp.at(2)) {
                        labeltext = u8"右端（j）";
                    }
                } else if ( i_mtype == DATATYPE::TPWALL&& i_dtype.getStructType() == STRUCTTYPE::STP_RC ) {
                    if (i == sp.at(0)) {
                        labeltext = u8"配筋";
                    } else if (i == sp.at(1)) {
                        labeltext = u8"開口補強筋（L,H≦1000）";
                    } else if (i == sp.at(2)) {
                        labeltext = u8"開口補強筋（L,H＞1000）";
                    }
                } else if ( i_mtype == DATATYPE::TPWALL && i_dtype.getStructType() == STRUCTTYPE::STP_S) {
                    if (i == sp.at(0)) {
                        labeltext = u8"配筋";
                    } else if (i == sp.at(1)) {
                        labeltext = u8"開口補強筋（L,H≦1000）";
                    }
                } else if ( i_mtype == DATATYPE::TPUWALL ) {
                    if (i == sp.at(0)) {
                        labeltext = u8"外側配筋";
                    } else if (i == sp.at(1)) {
                        labeltext = u8"内側配筋";
                    }
                } else if ( i_mtype == DATATYPE::TPSLAB ) {
                    if (i == sp.at(0)) {
                        labeltext = u8"短辺方向配筋";
                    } else if (i == sp.at(1)) {
                        labeltext = u8"長辺方向配筋";
                    }
                } else if ( i_mtype == DATATYPE::TPBASE ) {
                    if (i == sp.at(0)) {
                        labeltext = u8"ベースプレート";
                    } else if (i == sp.at(1)) {
                        labeltext = u8"アンカーボルト";
                    } else if (i == sp.at(2) && i_dtype.getStructType() == STRUCTTYPE::STP_S) {
                        labeltext = u8"基礎柱";
                    }
                }

                QLabel *label = new QLabel("<span style=\"font-size:10pt\"><center>" + labeltext +
                                           "</center></span>");
                QPalette pal;
                pal.setColor(QPalette::Background, QColor(200, 220, 200));
                label->setAutoFillBackground(true);
                label->setPalette(pal);
                label->setContentsMargins(0, 0, 0, 0);
                label->setMinimumHeight(20);
                selectrows->addWidget(label);
            }
            QLabel *label = new QLabel(header[i].replace("\n", " "));
            QCheckBox *checkbox = new QCheckBox(u8"表示");
            checkbox->setChecked(true);
            if (i == 0 || i == 1) {
                checkbox->setEnabled(false);
            }
            QString kword = filterKeys.at(itp);
            QList<QCheckBox *> chklist = allCheckBoxes.value(kword);
            chklist.append(checkbox);
            allCheckBoxes.insert(kword, chklist);
            QHBoxLayout *linelayout = new QHBoxLayout;
            linelayout->addSpacing(10);
            linelayout->addWidget(label, 1);
            linelayout->addWidget(checkbox);
            linelayout->addSpacing(20);
            linelayout->setContentsMargins(0, 0, 0, 0);
            QFrame *lineframe = new QFrame;
            QPalette pal;
            if (i % 2 == 0) {
                pal.setColor(QPalette::Background, QColor(240, 240, 240));
            } else {
                pal.setColor(QPalette::Background, QColor(220, 220, 220));
            }
            lineframe->setAutoFillBackground(true);
            lineframe->setPalette(pal);
            lineframe->setLayout(linelayout);
            lineframe->setContentsMargins(0, 0, 0, 0);

            selectrows->addWidget(lineframe);
        }
        QFrame *frame = new QFrame;
        frame->setLayout(selectrows);
        frame->setMinimumHeight(20);
        frame->setMinimumWidth(350);
        tab->setWidget(frame);
        tab->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        if (i_mtype == DATATYPE::TPWALL && i_stype == STRUCTTYPE::STP_RC) {
            tabwidget->addTab(tab, u8"壁");
        } else if (i_mtype == DATATYPE::TPWALL && i_stype == STRUCTTYPE::STP_S) {
            tabwidget->addTab(tab, u8"鋼板壁");
        }else if (i_stype == STRUCTTYPE::STP_RC) {
            tabwidget->addTab(tab, "RC");
        } else if (i_stype == STRUCTTYPE::STP_S)  {
            tabwidget->addTab(tab, "S");
        } else if (i_stype == STRUCTTYPE::STP_SRC) {
            tabwidget->addTab(tab, "SRC");
        } else if (i_stype == STRUCTTYPE::STP_CFT) {
            tabwidget->addTab(tab, "CFT");
        } else if (i_mtype == DATATYPE::TPUWALL) {
            tabwidget->addTab(tab, u8"地下外壁");
        } else if (i_mtype == DATATYPE::TPSLAB) {
            tabwidget->addTab(tab, u8"スラブ");
        } else if (i_mtype == DATATYPE::TPDAMPER) {
            tabwidget->addTab(tab, u8"制振要素");
        } else if (i_mtype == DATATYPE::TPISO) {
            tabwidget->addTab(tab, u8"免震要素");
        } else if (i_mtype == DATATYPE::TPWALLOPEN) {
            tabwidget->addTab(tab, u8"開口");
        }
    }


    return tabwidget;
}

QVBoxLayout* SectionFilterDialog::createSwitchLayout()
{
    QVBoxLayout *switchlayout = new QVBoxLayout;
    switchlayout->setSpacing(3);
    switchlayout->setContentsMargins(5, 5, 5, 5);

    QLabel *all_label = new QLabel(u8"全項目：");
    QPushButton *all_on = new QPushButton(u8"全表示", this);
    connect(all_on, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_allOn);
    QPushButton *all_off = new QPushButton(u8"全非表示", this);
    connect(all_off, &QAbstractButton::clicked, this, &SectionFilterDialog::slot_allOff);
    QHBoxLayout *allswitch = new QHBoxLayout;
    allswitch->setSpacing(3);
    allswitch->addWidget(all_label);
    allswitch->addStretch(1);
    allswitch->addWidget(all_on);
    allswitch->addWidget(all_off);
    switchlayout->addLayout(allswitch);
    return switchlayout;
}


void SectionFilterDialog::resetFilterColumns(const QHash<QString, QList<bool> > &filters)
{
    setFilterColumns(filters);
    setAllCheckBoxes(filters);
}

void SectionFilterDialog::setAllCheckBoxes(const QHash<QString, QList<bool> > &filters)
{
    Q_FOREACH(QString key, filterKeys){
        for (int j = 0; j < filters[key].count(); j++) {
            if ( allCheckBoxes[key].count() < j + 1 ) continue;
            allCheckBoxes[key][j]->setChecked(filters[key][j]);
        }
    }
}

QHash<QString, QList<bool> > SectionFilterDialog::readAllCheckBoxes()
{
    QHash<QString, QList<bool> > filters;
    for ( int i = 0; i < sectionTypeList.count(); i++ ) {
        QString kword = filterKeys.at(i);
        for (int j = 0; j < allCheckBoxes[kword].count(); j++) {
            filters[kword] << allCheckBoxes[kword][j]->isChecked();
        }
    }
    return filters;
}

void SectionFilterDialog::slot_accept()
{
    setFilterColumns(readAllCheckBoxes());
    accept();
}

void SectionFilterDialog::slot_reject()
{
    reject();
}

void SectionFilterDialog::slot_allOn()
{
    checkAllColumns(true);
}

void SectionFilterDialog::slot_allOff()
{
    checkAllColumns(false);
}

void SectionFilterDialog::checkAllColumns(bool on)
{
    QHash<QString, QList<bool> > filters = readAllCheckBoxes();
    for ( int i = 0; i < sectionTypeList.count(); i++ ) {
        QString kword = filterKeys.at(i);
        QStringList headers = sectionTypeList.at(i).getHeaderStrings();
        for ( int j = 0; j < headers.count(); j++ ) {
            if ( filters[kword].count() < j + 1 ) continue;
            filters[kword][j] = on;
            if ( headers.at(j).contains(u8"符号") )filters[kword][j] = true;
            if ( headers.at(j).contains("No.") )filters[kword][j] = true;
        }
    }
    setAllCheckBoxes(filters);
}

void SectionFilterDialog::slot_axialBarOn()
{
    checkAxialBarColumns(true);
}

void SectionFilterDialog::slot_axialBarOff()
{
    checkAxialBarColumns(false);
}

void SectionFilterDialog::checkAxialBarColumns(bool on)
{
    QHash<QString, QList<bool> > filters = readAllCheckBoxes();
    for ( int i = 0; i < sectionTypeList.count(); i++ ) {
        STRUCTTYPE stype = sectionTypeList.at(i).getStructType();
        if ( stype != STRUCTTYPE::STP_RC && stype != STRUCTTYPE::STP_SRC ) continue;
        QStringList headers = sectionTypeList.at(i).getHeaderStrings();
        QString kword = filterKeys.at(i);
        for ( int j = 0; j < headers.count(); j++ ) {
            if ( filters[kword].count() < j + 1 ) continue;
            if ( headers.at(j).contains(u8"芯鉄筋") )filters[kword][j] = on;
        }
    }
    setAllCheckBoxes(filters);
}

void SectionFilterDialog::slot_rkryOn()
{
    checkRkRyColumns(true);
}

void SectionFilterDialog::slot_rkryOff()
{
    checkRkRyColumns(false);
}

void SectionFilterDialog::checkRkRyColumns(bool on)
{
    QHash<QString, QList<bool> > filters = readAllCheckBoxes();
    for ( int i = 0; i < sectionTypeList.count(); i++ ) {
        STRUCTTYPE stype = sectionTypeList.at(i).getStructType();
        if ( stype != STRUCTTYPE::STP_RC && stype != STRUCTTYPE::STP_SRC ) continue;
        QStringList headers = sectionTypeList.at(i).getHeaderStrings();
        QString kword = filterKeys.at(i);
        for ( int j = 0; j < headers.count(); j++ ) {
            if ( filters[kword].count() < j + 1 ) continue;
            if ( headers[j].contains(u8"主筋距離")|| headers[j].contains(u8"主筋間隔") )
                filters[kword][j] = on;
        }
    }
    setAllCheckBoxes(filters);
}

void SectionFilterDialog::slot_steelPositionOn()
{
    checkSteelPositionColumns(true);
}

void SectionFilterDialog::slot_steelPositionOff()
{
    checkSteelPositionColumns(false);
}

void SectionFilterDialog::checkSteelPositionColumns(bool on)
{
    QHash<QString, QList<bool> > filters = readAllCheckBoxes();
    for ( int i = 0; i < sectionTypeList.count(); i++ ) {
        STRUCTTYPE stype = sectionTypeList.at(i).getStructType();
        if ( stype != STRUCTTYPE::STP_SRC ) continue;
        QStringList headers = sectionTypeList.at(i).getHeaderStrings();
        QString kword = filterKeys.at(i);
        for ( int j = 0; j < headers.count(); j++ ) {
            if ( filters[kword].count() < j + 1 ) continue;
            if ( headers.at(j).contains(u8"鉄骨位置") )
                filters[kword][j] = on;
        }
    }
    setAllCheckBoxes(filters);
}
} // namespace post3dapp
