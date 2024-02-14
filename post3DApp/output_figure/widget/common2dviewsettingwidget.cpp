#include "common2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
namespace post3dapp{

OutputFigureCommon2DWidget::OutputFigureCommon2DWidget(const QString& _tabName, const QString& _englishTabName, QWidget* parent)
    : BaseSettingWidget(parent), TabName(_tabName), englishTabName(_englishTabName)
{
    // 階・通りのライン
    QLabel *scaleLineLabel = new QLabel(u8"階／通りスケールの表示カラー:");
    scaleColorCombobox = new TypicalColorComboBox();
    QHBoxLayout *hlay_scale = new QHBoxLayout();
    hlay_scale->setContentsMargins(0, 0, 0, 0);
    hlay_scale->addWidget(scaleLineLabel);
    hlay_scale->addSpacing(30);
    hlay_scale->addWidget(scaleColorCombobox);
    hlay_scale->addStretch();

    QHBoxLayout *hlay_gridfloor = new QHBoxLayout();
    QLabel *scaleStringLabel = new QLabel(u8"階／通りスケールの文字サイズ");
    scaleSizeCombobox = new FiveSizeComboBox();
    hlay_gridfloor->setContentsMargins(0, 0, 0, 0);
    hlay_gridfloor->addWidget(scaleStringLabel);
    hlay_gridfloor->addSpacing(30);
    hlay_gridfloor->addWidget(scaleSizeCombobox);
    hlay_gridfloor->addSpacing(50);

    gridStringCheck = new QCheckBox(u8"階の文字表示");
    floorStringCheck = new QCheckBox(u8"通りの文字表示");
    hlay_gridfloor->addWidget(gridStringCheck);
    hlay_gridfloor->addWidget(floorStringCheck);
    hlay_gridfloor->addStretch();

    // 解析節点
    QLabel *jointLabel = new QLabel(u8"・解析節点");
    jointLabel->setFixedWidth(120);
    QLabel *jointSizeLabel = new QLabel(u8"節点サイズ：");
    jointSizeCombobox = new FiveSizeComboBox();
    jointColorCombobox = new TypicalColorComboBox();
    QHBoxLayout *hlay_joint = new QHBoxLayout;
    hlay_joint->setContentsMargins(0, 0, 0, 0);
    hlay_joint->addWidget(jointLabel);
    hlay_joint->addWidget(new QLabel(u8"色："));
    hlay_joint->addWidget(jointColorCombobox);
    hlay_joint->addSpacing(30);
    hlay_joint->addWidget(jointSizeLabel);
    hlay_joint->addWidget(jointSizeCombobox);
    hlay_joint->addStretch();

    // 解析部材（線）
    QLabel *lineLabel = new QLabel(u8"・解析部材（線材）");
    lineLabel->setFixedWidth(120);
    QLabel *lineRCColorLabel = new QLabel(u8"色(RC)：");
    lineRCColorCombobox = new TypicalColorComboBox();
    QLabel *lineSRCColorLabel = new QLabel(u8"色(SRC)：");
    lineSRCColorCombobox = new TypicalColorComboBox();
    QLabel *lineSColorLabel = new QLabel(u8"色(S)：");
    lineSColorCombobox = new TypicalColorComboBox();
    QLabel *lineCFTColorLabel = new QLabel(u8"色(CFT)：");
    lineCFTColorCombobox = new TypicalColorComboBox();
    QLabel *lineDamperColorLabel = new QLabel(u8"色(制振)：");
    lineDamperColorCombobox = new TypicalColorComboBox();
    QLabel *lineIsoColorLabel = new QLabel(u8"色(免震)：");
    lineIsoColorCombobox = new TypicalColorComboBox();

    QHBoxLayout *hlay_line = new QHBoxLayout;
    hlay_line->setContentsMargins(0, 0, 0, 0);
    hlay_line->addWidget(lineLabel);
    hlay_line->addWidget(lineRCColorLabel);
    hlay_line->addWidget(lineRCColorCombobox);
    hlay_line->addWidget(lineSRCColorLabel);
    hlay_line->addWidget(lineSRCColorCombobox);
    hlay_line->addWidget(lineSColorLabel);
    hlay_line->addWidget(lineSColorCombobox);

    QHBoxLayout *hlay_line2 = new QHBoxLayout;
    hlay_line2->setContentsMargins(0, 0, 0, 0);
    hlay_line2->addWidget(lineLabel);
    hlay_line2->addWidget(lineCFTColorLabel);
    hlay_line2->addWidget(lineCFTColorCombobox);
    hlay_line2->addWidget(lineDamperColorLabel);
    hlay_line2->addWidget(lineDamperColorCombobox);
    hlay_line2->addWidget(lineIsoColorLabel);
    hlay_line2->addWidget(lineIsoColorCombobox);

    // 解析部材（面）
    QLabel *planeLabel = new QLabel(u8"・解析部材（面材）");
    planeLabel->setFixedWidth(120);
    QLabel *planeRCColorLabel = new QLabel(u8"色(RC・SRC)：");
    planeRCColorCombobox = new TypicalColorComboBox();
    QLabel *planeSColorLabel = new QLabel(u8"色(S)：");
    planeSColorCombobox = new TypicalColorComboBox();
    QLabel *planeDamperColorLabel = new QLabel(u8"色(制振)：");
    planeDamperColorCombobox = new TypicalColorComboBox();
    QHBoxLayout *hlay_plane = new QHBoxLayout;
    hlay_plane->setContentsMargins(0, 0, 0, 0);
    hlay_plane->addWidget(planeLabel);
    hlay_plane->addWidget(planeRCColorLabel);
    hlay_plane->addWidget(planeRCColorCombobox);
    hlay_plane->addWidget(planeSColorLabel);
    hlay_plane->addWidget(planeSColorCombobox);
    hlay_plane->addWidget(planeDamperColorLabel);
    hlay_plane->addWidget(planeDamperColorCombobox);

    // ばね・支点
    QLabel *supportLabel = new QLabel(u8"・ばね／支点");
    supportLabel->setFixedWidth(120);
    QLabel *supportColorLabel = new QLabel(u8"支点カラー");
    supportColorCombobox = new TypicalColorComboBox();
    QHBoxLayout *hlay_support = new QHBoxLayout;
    hlay_support->setContentsMargins(0, 0, 0, 0);
    hlay_support->addWidget(supportLabel);
    hlay_support->addWidget(supportColorLabel);
    hlay_support->addWidget(supportColorCombobox);
    hlay_support->addStretch();

    //値の共通文字サイズ・色
    QLabel *valueLabel = new QLabel(u8"値:");
    valueLabel->setFixedWidth(120);
    QLabel *valueColorLabel = new QLabel(u8"文字カラー:");
    valueColorCombobox = new TypicalColorComboBox();
    QLabel *valueSizeLabel = new QLabel(u8"文字サイズ：");
    valueSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_value = new QHBoxLayout;
    hlay_value->setContentsMargins(0, 0, 0, 0);
    hlay_value->addWidget(valueLabel);
    hlay_value->addWidget(valueColorLabel);
    hlay_value->addWidget(valueColorCombobox);
    hlay_value->addSpacing(30);
    hlay_value->addWidget(valueSizeLabel);
    hlay_value->addWidget(valueSizeCombobox);
    hlay_value->addStretch();

    //　選択時の色
    QLabel *selectColorLabel = new QLabel(u8"選択カラー:");
    selectColorLabel->setFixedWidth(120);
    selectColorCombobox = new TypicalColorComboBox();
    QHBoxLayout *hlay_select = new QHBoxLayout;
    hlay_select->setContentsMargins(0, 0, 0, 0);
    hlay_select->addWidget(selectColorLabel);
    hlay_select->addWidget(selectColorCombobox);
    hlay_select->addStretch();

    //注記の共通文字サイズ・色
    QLabel *noteLabel = new QLabel(u8"注記:");
    valueLabel->setFixedWidth(120);
    QLabel *noteColorLabel = new QLabel(u8"注記カラー:");
    noteColorCombobox = new TypicalColorComboBox();
    QLabel *noteSizeLabel = new QLabel(u8"注記サイズ：");
    noteSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_note = new QHBoxLayout;
    hlay_note->setContentsMargins(0, 0, 0, 0);
    hlay_note->addWidget(noteLabel);
    hlay_note->addWidget(noteColorLabel);
    hlay_note->addWidget(noteColorCombobox);
    hlay_note->addSpacing(30);
    hlay_note->addWidget(noteSizeLabel);
    hlay_note->addWidget(noteSizeCombobox);
    hlay_note->addStretch();

    vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 10, 5, 10);
    vlay->setSpacing(3);
    vlay->addLayout(hlay_scale);
    vlay->addLayout(hlay_gridfloor);
    vlay->addLayout(hlay_joint);
    vlay->addLayout(hlay_line);
    vlay->addLayout(hlay_line2);
    vlay->addLayout(hlay_plane);
    vlay->addLayout(hlay_support);
    vlay->addLayout(hlay_value);
    vlay->addLayout(hlay_select);
    vlay->addLayout(hlay_note);
    vlay->addStretch();
    this->setLayout(vlay);

    initializeSettings();
}

void OutputFigureCommon2DWidget::initializeSettings(){
    scaleColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    scaleSizeCombobox->setSize(3);
    gridStringCheck->setChecked(true);
    floorStringCheck->setChecked(true);

    jointColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    jointSizeCombobox->setSize(3);

    lineRCColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    lineSRCColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    lineSColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    lineCFTColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    lineDamperColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    lineIsoColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    planeRCColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    planeSColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    planeDamperColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    supportColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    valueColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    valueSizeCombobox->setSize(3);

    selectColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    noteColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    noteSizeCombobox->setSize(3);

}

void OutputFigureCommon2DWidget::writeBinaryFile(QDataStream &outfile) const{
    outfile << scaleColorCombobox->currentIndex();
    outfile << scaleSizeCombobox->currentIndex();
    outfile << gridStringCheck->isChecked();
    outfile << floorStringCheck->isChecked();

    outfile << jointColorCombobox->currentIndex();
    outfile << jointSizeCombobox->currentIndex();

    outfile << lineRCColorCombobox->currentIndex();
    outfile << lineSRCColorCombobox->currentIndex();
    outfile << lineSColorCombobox->currentIndex();
    outfile << lineCFTColorCombobox->currentIndex();
    outfile << lineDamperColorCombobox->currentIndex();
    outfile << lineIsoColorCombobox->currentIndex();

    outfile << planeRCColorCombobox->currentIndex();
    outfile << planeSColorCombobox->currentIndex();
    outfile << planeDamperColorCombobox->currentIndex();

    outfile << supportColorCombobox->currentIndex();

    outfile << valueColorCombobox->currentIndex();
    outfile << valueSizeCombobox->currentIndex();

    outfile << selectColorCombobox->currentIndex();

    outfile << noteColorCombobox->currentIndex();
    outfile << noteSizeCombobox->currentIndex();
}
void OutputFigureCommon2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    int tempInt;
    bool tempBool;

    infile >> tempInt;
    scaleColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    scaleSizeCombobox->setCurrentIndex(tempInt);
    infile >> tempBool;
    gridStringCheck->setChecked(tempBool);
    infile >> tempBool;
    floorStringCheck->setChecked(tempBool);

    infile >> tempInt;
    jointColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    jointSizeCombobox->setCurrentIndex(tempInt);

    infile >> tempInt;
    lineRCColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    lineSRCColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    lineSColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    lineCFTColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    lineDamperColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    lineIsoColorCombobox->setCurrentIndex(tempInt);

    infile >> tempInt;
    planeRCColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    planeSColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    planeDamperColorCombobox->setCurrentIndex(tempInt);

    infile >> tempInt;
    supportColorCombobox->setCurrentIndex(tempInt);

    infile >> tempInt;
    valueColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    valueSizeCombobox->setCurrentIndex(tempInt);

    infile >> tempInt;
    selectColorCombobox->setCurrentIndex(tempInt);

    infile >> tempInt;
    noteColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    noteSizeCombobox->setCurrentIndex(tempInt);
}

void OutputFigureCommon2DWidget::saveSettings(QSettings& settings){
    settings.beginGroup(englishTabName);
    settings.setValue("scaleColor", scaleColorCombobox->name());
    settings.setValue("scaleSize", scaleSizeCombobox->size());
    settings.setValue("gridStringCheck", gridStringCheck->isChecked());
    settings.setValue("floorStringCheck", floorStringCheck->isChecked());
    settings.setValue("jointColor", jointColorCombobox->name());
    settings.setValue("jointSize", jointSizeCombobox->size());
    settings.setValue("lineRCColor", lineRCColorCombobox->name());
    settings.setValue("lineSRCColor", lineSRCColorCombobox->name());
    settings.setValue("lineSColor", lineSColorCombobox->name());
    settings.setValue("lineCFTColor", lineCFTColorCombobox->name());
    settings.setValue("lineDamperColor", lineDamperColorCombobox->name());
    settings.setValue("lineIsoColor", lineIsoColorCombobox->name());
    settings.setValue("planeRCColor", planeRCColorCombobox->name());
    settings.setValue("planeSColor", planeSColorCombobox->name());
    settings.setValue("planeDamperColor", planeDamperColorCombobox->name());
    settings.setValue("supportColor", supportColorCombobox->name());
    settings.setValue("valueColor", valueColorCombobox->name());
    settings.setValue("valueSize", valueSizeCombobox->size());
    settings.setValue("selectColor", selectColorCombobox->name());
    settings.setValue("noteColor", noteColorCombobox->name());
    settings.setValue("noteSize", noteSizeCombobox->size());
    settings.endGroup();
}
void OutputFigureCommon2DWidget::readSettings(QSettings& settings){
    settings.beginGroup(englishTabName);
    scaleColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("scaleColor").toString()));
    scaleSizeCombobox->setCurrentIndex(settings.value("scaleSize").toInt());
    gridStringCheck->setChecked(settings.value("gridStringCheck").toBool());
    floorStringCheck->setChecked(settings.value("floorStringCheck").toBool());
    jointColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("jointColor").toString()));
    jointSizeCombobox->setCurrentIndex(settings.value("jointSize").toInt());
    lineRCColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("lineRCColor").toString()));
    lineSRCColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("lineSRCColor").toString()));
    lineSColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("lineSColor").toString()));
    lineCFTColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("lineCFTColor").toString()));
    lineDamperColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("lineDamperColor").toString()));
    lineIsoColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("lineIsoColor").toString()));
    planeRCColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("planeRCColor").toString()));
    planeSColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("planeSColor").toString()));
    planeDamperColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("planeDamperColor").toString()));
    supportColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("supportColor").toString()));
    valueColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("valueColor").toString()));
    valueSizeCombobox->setCurrentIndex(settings.value("valueSize").toInt());
    selectColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("selectColor").toString()));
    noteColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("noteColor").toString()));
    noteSizeCombobox->setCurrentIndex(settings.value("noteSize").toInt());
    settings.endGroup();
}

} // namespace post3dapp
