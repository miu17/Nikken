#include<QtGui>
#include "material_filterdialog.h"
#include "define_material_struct.h"
#include "utility.h"
namespace post3dapp{
MaterialFilterDialog::MaterialFilterDialog(const MaterialDataType &type, QWidget *parent)
    : QDialog(parent), materialDataType(type)
{
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint );
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));

    setWindowTitle(u8"表示設定　材料");

    //メニュー
    QPushButton *pb_a = new QPushButton("OK", this);
    connect(pb_a, &QAbstractButton::clicked, this, &MaterialFilterDialog::slot_accept);
    QPushButton *pb_c = new QPushButton(u8"キャンセル", this);
    connect(pb_c, &QAbstractButton::clicked, this, &MaterialFilterDialog::slot_reject);
    QPushButton *pb_sd = new QPushButton(u8"設定保存", this);
    connect(pb_sd, &QAbstractButton::clicked, this, &MaterialFilterDialog::slot_saveDefault);
    QPushButton *pb_ad = new QPushButton(u8"設定読込", this);
    connect(pb_ad, &QAbstractButton::clicked, this, &MaterialFilterDialog::slot_loadDefault);

    QHBoxLayout *menubuttons = new QHBoxLayout;
    menubuttons->setSpacing(3);
    menubuttons->addWidget(pb_a);
    menubuttons->addWidget(pb_c);
    menubuttons->addStretch(1);
    menubuttons->addWidget(pb_sd);
    menubuttons->addWidget(pb_ad);

    //タブ
    QTabWidget *tabwidget = new QTabWidget(this);
    QScrollArea *tab = new QScrollArea;
    QVBoxLayout *selectrows = new QVBoxLayout;
    selectrows->setSpacing(0);
    selectrows->setContentsMargins(0, 0, 0, 0);
    QStringList header = materialDataType.getHeaderStrings();
    int header_count = header.count();

    QList<int> sp;
    sp << materialDataType.getSeparateColumn1();

    for (int i = 0; i < header_count; ++i) { //0番の要素（No.）はスキップ
        if (i == 0 || sp.contains(i)) {
            QString labeltext;
            if (i == 0) {
                labeltext = u8"一般";
            } else if (i == sp.at(0)) {
                labeltext = u8"強度";
            }
            QLabel *label = new QLabel("<span style=\"font-size:10pt\"><center>"
                                       + labeltext + "</center></span>");
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
        QList<QCheckBox *> chklist = allCheckBoxes.value("MATERIAL");
        chklist.append(checkbox);
        allCheckBoxes.insert("MATERIAL", chklist);
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
    tabwidget->addTab(tab, u8"材料");


    //一括切り替え
    QVBoxLayout *switchlayout = new QVBoxLayout;
    switchlayout->setSpacing(3);
    switchlayout->setContentsMargins(5, 5, 5, 5);

    QPushButton *all_on = new QPushButton(u8"全表示", this);
    connect(all_on, &QAbstractButton::clicked, this, &MaterialFilterDialog::slot_allOn);
    QPushButton *all_off = new QPushButton(u8"全非表示", this);
    connect(all_off, &QAbstractButton::clicked, this, &MaterialFilterDialog::slot_allOff);
    QHBoxLayout *allswitch = new QHBoxLayout;
    allswitch->setSpacing(3);
    allswitch->addWidget(new QLabel(u8"全項目："));
    allswitch->addStretch(1);
    allswitch->addWidget(all_on);
    allswitch->addWidget(all_off);
    switchlayout->addLayout(allswitch);

    QGroupBox *instantswitch = new QGroupBox(u8"一括切り替え");
    instantswitch->setLayout(switchlayout);
    instantswitch->setContentsMargins(5, 5, 5, 5);

    //トップレイアウト
    QVBoxLayout *toplayout = new QVBoxLayout;
    toplayout->addLayout(menubuttons);
    toplayout->addWidget(tabwidget);
    toplayout->addWidget(instantswitch);
    setLayout(toplayout);
}

void MaterialFilterDialog::resetFilterColumns(const QHash<QString, QList<bool> > &filters)
{
    setFilterColumns(filters);
    setAllCheckBoxes(filters);
}

void MaterialFilterDialog::setAllCheckBoxes(const QHash<QString, QList<bool> > &filters)
{
    for (int i = 0; i < filters["MATERIAL"].count(); i++) {
        if ( allCheckBoxes["MATERIAL"].count() < i + 1 ) continue;
        allCheckBoxes["MATERIAL"][i]->setChecked(filters["MATERIAL"][i]);
    }
}

QHash<QString, QList<bool> > MaterialFilterDialog::readAllCheckBoxes()
{
    QHash<QString, QList<bool> > filters;
    for (int i = 0; i < allCheckBoxes["MATERIAL"].count(); i++) {
        filters["MATERIAL"] << allCheckBoxes["MATERIAL"][i]->isChecked();
    }
    return filters;
}

void MaterialFilterDialog::slot_accept()
{
    setFilterColumns(readAllCheckBoxes());
    accept();
}

void MaterialFilterDialog::slot_reject()
{
    reject();
}

void MaterialFilterDialog::slot_saveDefault()
{
    setFilterColumns(readAllCheckBoxes());

    QFile file(":standard_material/defaultSectionHeaders.dat");
    if ( !file.open(QIODevice::ReadOnly) ) {
        Utility::messageFileOpenError(this);
        return;
    }
    QTextStream in(&file);
    QString outText;
    while ( !in.atEnd() ) {
        QString line = in.readLine();
        outText += line + "\n";
        if ( line != "MATERIAL" ) continue;
        for (int i = 0; i < myFilterColumns["MATERIAL"].count(); ++i) {
            outText += ( myFilterColumns["MATERIAL"][i] ) ? "1" : "0" ; //true=1,false=0
        }
        outText += "\n";
        in.readLine();
    }
    file.close();

    if ( !file.open(QIODevice::WriteOnly) ) {
        Utility::messageFileSaveError(this);
        return;
    }
    QTextStream out(&file);
    out << outText;
    file.close();
    defaultFilterColumns = myFilterColumns;

    return;
}

void MaterialFilterDialog::slot_loadDefault()
{
    if (!defaultFilterColumns.isEmpty()) {
        //デフォルトを読み込み済み → その値に設定、なし → load;
        setAllCheckBoxes(defaultFilterColumns);
        return;
    }

    QFile file(":standard_material/defaultSectionHeaders.dat");
    if ( !file.open(QIODevice::ReadOnly) ) {
        Utility::messageFileOpenError(this);
        return;
    }
    QTextStream in(&file);
    QHash<QString, QList<bool> > filters;
    while ( !in.atEnd() ) {
        QString type = in.readLine();
        if ( type != "MATERIAL" ) continue;
        QString line = in.readLine();

        if (materialDataType.getHeaderStrings().count() != line.size()) {
            Utility::messageFormatError(this);
            return;
        }
        for (int i = 0; i < line.size(); ++i) {
            if (line.at(i) == '1')filters["MATERIAL"] << true;
            if (line.at(i) == '0')filters["MATERIAL"] << false;
        }
    }
    setAllCheckBoxes(filters);
    defaultFilterColumns = filters;

    return;
}

void MaterialFilterDialog::slot_allOn()
{
    checkAllColumns(true);
}

void MaterialFilterDialog::slot_allOff()
{
    checkAllColumns(false);
}

void MaterialFilterDialog::checkAllColumns(bool on)
{
    QHash<QString, QList<bool> > filters = readAllCheckBoxes();
    QStringList headers = materialDataType.getHeaderStrings();
    for ( int i = 0; i < headers.count(); i++ ) {
        if ( filters["MATERIAL"].count() < i + 1 ) continue;
        filters["MATERIAL"][i] = on;
        if ( headers.at(i).contains(u8"符号") )filters["MATERIAL"][i] = true;
        if ( headers.at(i).contains("No.") )filters["MATERIAL"][i] = true;
    }
    setAllCheckBoxes(filters);
}

QList<bool> MaterialFilterDialog::loadDefaultFilters()
{
    QFile file(":standard_material/defaultSectionHeaders.dat");
    if ( !file.open(QIODevice::ReadOnly) )return QList<bool>();
    QTextStream in(&file);

    QList<bool> filters;
    while (!in.atEnd()) {
        QString str = in.readLine();
        if ( str != "MATERIAL" ) continue;
        QString line = in.readLine();
        for ( int i = 0; i < line.size(); i++ ) {
            if (line.at(i) == '1') {
                filters << true;
            } else if (line.at(i) == '0') {
                filters << false;
            }
        }
    }
    return filters;
}

QHash<QString, QList<bool> > MaterialFilterDialog::getFilterColumns() const
{
    return myFilterColumns;
}

void MaterialFilterDialog::setFilterColumns(const QHash<QString, QList<bool> > &filter)
{
    myFilterColumns = filter;
}
} // namespace post3dapp
