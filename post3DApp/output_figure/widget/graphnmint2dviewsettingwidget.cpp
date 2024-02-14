#include "graphnmint2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"

namespace post3dapp{

OutputFigureGraphNmint2DWidget::OutputFigureGraphNmint2DWidget(const QString& _tabName, const QString& _englishTabName, QWidget *parent)
    : BaseSettingWidget(parent), TabName(_tabName), englishTabName(_englishTabName)
{
    titleVisibleCheck = new QCheckBox(u8"タイトルを表示する");

    QLabel *axisColorLabel = new QLabel(u8"軸カラー：");
    axisColorCombobox = new TypicalColorComboBox();
    QLabel *axisTextColorLabel = new QLabel(u8"軸テキストカラー：");
    axisTextColorCombobox = new TypicalColorComboBox();
    QLabel *axisTextSizeLabel = new QLabel(u8"軸テキストサイズ：");
    axisTextSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_axis = new QHBoxLayout;
    hlay_axis->addWidget(axisColorLabel);
    hlay_axis->addWidget(axisColorCombobox);
    hlay_axis->addWidget(axisTextColorLabel);
    hlay_axis->addWidget(axisTextColorCombobox);
    hlay_axis->addWidget(axisTextSizeLabel);
    hlay_axis->addWidget(axisTextSizeCombobox);

    outputLVisibleCheck = new QCheckBox(u8"長期を表示する");
    outputSVisibleCheck = new QCheckBox(u8"短期を表示する");
    outputUVisibleCheck = new QCheckBox(u8"終局を表示する");
    QHBoxLayout *hlay_check = new QHBoxLayout;
    hlay_check->addWidget(outputLVisibleCheck);
    hlay_check->addWidget(outputSVisibleCheck);
    hlay_check->addWidget(outputUVisibleCheck);

    QLabel *symbolColorLabel = new QLabel(u8"記号カラー：");
    symbolColorCombobox = new TypicalColorComboBox();
    QLabel *symbolSizeLabel = new QLabel(u8"記号サイズ：");
    symbolSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_symbol = new QHBoxLayout;
    hlay_symbol->addWidget(symbolColorLabel);
    hlay_symbol->addWidget(symbolColorCombobox);
    hlay_symbol->addWidget(symbolSizeLabel);
    hlay_symbol->addWidget(symbolSizeCombobox);

    isAssignedNValueCheck = new QCheckBox(u8"N軸設定をする：");
    QLabel *minNValueLabel = new QLabel(u8"最小値：");
    minNValueSpinBox = new QDoubleSpinBox();
    QLabel *maxNValueLabel = new QLabel(u8"最大値：");
    maxNValueSpinBox = new QDoubleSpinBox();
    QLabel *pitchNValueLabel = new QLabel(u8"ピッチ：");
    pitchNValueSpinBox = new QDoubleSpinBox();
    QHBoxLayout *hlay_nvalue = new QHBoxLayout;
    hlay_nvalue->addWidget(minNValueLabel);
    hlay_nvalue->addWidget(minNValueSpinBox);
    hlay_nvalue->addWidget(maxNValueLabel);
    hlay_nvalue->addWidget(maxNValueSpinBox);
    hlay_nvalue->addWidget(pitchNValueLabel);
    hlay_nvalue->addWidget(pitchNValueSpinBox);

    isAssignedMyValueCheck = new QCheckBox(u8"My軸設定をする：");
    QLabel *minMyValueLabel = new QLabel(u8"最小値：");
    minMyValueSpinBox = new QDoubleSpinBox();
    QLabel *maxMyValueLabel = new QLabel(u8"最大値：");
    maxMyValueSpinBox = new QDoubleSpinBox();
    QLabel *pitchMyValueLabel = new QLabel(u8"ピッチ：");
    pitchMyValueSpinBox = new QDoubleSpinBox();
    QHBoxLayout *hlay_myvalue = new QHBoxLayout;
    hlay_myvalue->addWidget(minMyValueLabel);
    hlay_myvalue->addWidget(minMyValueSpinBox);
    hlay_myvalue->addWidget(maxMyValueLabel);
    hlay_myvalue->addWidget(maxMyValueSpinBox);
    hlay_myvalue->addWidget(pitchMyValueLabel);
    hlay_myvalue->addWidget(pitchMyValueSpinBox);

    isAssignedMzValueCheck = new QCheckBox(u8"Mz軸設定をする：");
    QLabel *minMzValueLabel = new QLabel(u8"最小値：");
    minMzValueSpinBox = new QDoubleSpinBox();
    QLabel *maxMzValueLabel = new QLabel(u8"最大値：");
    maxMzValueSpinBox = new QDoubleSpinBox();
    QLabel *pitchMzValueLabel = new QLabel(u8"ピッチ：");
    pitchMzValueSpinBox = new QDoubleSpinBox();
    QHBoxLayout *hlay_mzvalue = new QHBoxLayout;
    hlay_mzvalue->addWidget(minMzValueLabel);
    hlay_mzvalue->addWidget(minMzValueSpinBox);
    hlay_mzvalue->addWidget(maxMzValueLabel);
    hlay_mzvalue->addWidget(maxMzValueSpinBox);
    hlay_mzvalue->addWidget(pitchMzValueLabel);
    hlay_mzvalue->addWidget(pitchMzValueSpinBox);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(titleVisibleCheck);
    vlay->addLayout(hlay_axis);
    vlay->addLayout(hlay_check);
    vlay->addLayout(hlay_symbol);
    vlay->addWidget(isAssignedNValueCheck);
    vlay->addLayout(hlay_nvalue);
    vlay->addWidget(isAssignedMyValueCheck);
    vlay->addLayout(hlay_myvalue);
    vlay->addWidget(isAssignedMzValueCheck);
    vlay->addLayout(hlay_mzvalue);
    vlay->addStretch();

    this->setLayout(vlay);

    initializeSettings();
}

void OutputFigureGraphNmint2DWidget::initializeSettings(){
    titleVisibleCheck->setChecked(true);
    axisColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    axisTextColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    axisTextSizeCombobox->setSize(3);

    outputLVisibleCheck->setChecked(true);
    outputSVisibleCheck->setChecked(true);
    outputUVisibleCheck->setChecked(true);

    symbolColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    symbolSizeCombobox->setSize(3);

    isAssignedNValueCheck->setChecked(false);
    minNValueSpinBox->setRange(-100000, 100000);
    minNValueSpinBox->setValue(-500);
    maxNValueSpinBox->setRange(-100000, 100000);
    maxNValueSpinBox->setValue(500);
    pitchNValueSpinBox->setRange(0, 100000);
    pitchNValueSpinBox->setValue(100);

    isAssignedMyValueCheck->setChecked(false);
    minMyValueSpinBox->setRange(-100000, 100000);
    minMyValueSpinBox->setValue(-500);
    maxMyValueSpinBox->setRange(-100000, 100000);
    maxMyValueSpinBox->setValue(500);
    pitchMyValueSpinBox->setRange(0, 100000);
    pitchMyValueSpinBox->setValue(100);

    isAssignedMzValueCheck->setChecked(false);
    minMzValueSpinBox->setRange(-100000, 100000);
    minMzValueSpinBox->setValue(-500);
    maxMzValueSpinBox->setRange(-100000, 100000);
    maxMzValueSpinBox->setValue(500);
    pitchMzValueSpinBox->setRange(0, 100000);
    pitchMzValueSpinBox->setValue(100);
}

void OutputFigureGraphNmint2DWidget::writeBinaryFile(QDataStream &outfile) const{
    outfile << titleVisibleCheck->isChecked();
    outfile << axisColorCombobox->currentIndex();
    outfile << axisTextColorCombobox->currentIndex();
    outfile << axisTextSizeCombobox->currentIndex();

    outfile << outputLVisibleCheck->isChecked();
    outfile << outputSVisibleCheck->isChecked();
    outfile << outputUVisibleCheck->isChecked();

    outfile << symbolColorCombobox->currentIndex();
    outfile << symbolSizeCombobox->currentIndex();

    outfile << isAssignedNValueCheck->isChecked();
    outfile << minNValueSpinBox->value();
    outfile << maxNValueSpinBox->value();
    outfile << pitchNValueSpinBox->value();

    outfile << isAssignedMyValueCheck->isChecked();
    outfile << minMyValueSpinBox->value();
    outfile << maxMyValueSpinBox->value();
    outfile << pitchMyValueSpinBox->value();

    outfile << isAssignedMzValueCheck->isChecked();
    outfile << minMzValueSpinBox->value();
    outfile << maxMzValueSpinBox->value();
    outfile << pitchMzValueSpinBox->value();
}
void OutputFigureGraphNmint2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    bool tempBool;
    infile >> tempBool;
    titleVisibleCheck->setChecked(tempBool);
    int tempInt;
    infile >> tempInt;
    axisColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    axisTextColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    axisTextSizeCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    outputLVisibleCheck->setChecked(tempBool);
    infile >> tempBool;
    outputSVisibleCheck->setChecked(tempBool);
    infile >> tempBool;
    outputUVisibleCheck->setChecked(tempBool);

    infile >> tempInt;
    symbolColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    symbolSizeCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    isAssignedNValueCheck->setChecked(tempBool);
    double tempDouble;
    infile >> tempDouble;
    minNValueSpinBox->setValue(tempDouble);
    infile >> tempDouble;
    maxNValueSpinBox->setValue(tempDouble);
    infile >> tempDouble;
    pitchNValueSpinBox->setValue(tempDouble);

    infile >> tempBool;
    isAssignedMyValueCheck->setChecked(tempBool);
    infile >> tempDouble;
    minMyValueSpinBox->setValue(tempDouble);
    infile >> tempDouble;
    maxMyValueSpinBox->setValue(tempDouble);
    infile >> tempDouble;
    pitchMyValueSpinBox->setValue(tempDouble);

    infile >> tempBool;
    isAssignedMzValueCheck->setChecked(tempBool);
    infile >> tempDouble;
    minMzValueSpinBox->setValue(tempDouble);
    infile >> tempDouble;
    maxMzValueSpinBox->setValue(tempDouble);
    infile >> tempDouble;
    pitchMzValueSpinBox->setValue(tempDouble);
}

void OutputFigureGraphNmint2DWidget::saveSettings(QSettings& settings){
    settings.beginGroup(englishTabName);
    settings.setValue("titleVisibleCheck", titleVisibleCheck->isChecked());
    settings.setValue("axisColor", axisColorCombobox->name());
    settings.setValue("axisTextColor", axisTextColorCombobox->name());
    settings.setValue("axisTextSize", axisTextSizeCombobox->size());
    settings.setValue("outputLVisibleCheck", outputLVisibleCheck->isChecked());
    settings.setValue("outputSVisibleCheck", outputSVisibleCheck->isChecked());
    settings.setValue("outputUVisibleCheck", outputUVisibleCheck->isChecked());
    settings.setValue("symbolColor", symbolColorCombobox->name());
    settings.setValue("symbolSize", symbolSizeCombobox->size());
    settings.setValue("isAssignedNValueCheck", isAssignedNValueCheck->isChecked());
    settings.setValue("minNValue",  minNValueSpinBox->value());
    settings.setValue("maxNValue",  maxNValueSpinBox->value());
    settings.setValue("pitchNValue",  pitchNValueSpinBox->value());
    settings.setValue("isAssignedMyValueCheck", isAssignedMyValueCheck->isChecked());
    settings.setValue("minMyValue",  minMyValueSpinBox->value());
    settings.setValue("maxMyValue",  maxMyValueSpinBox->value());
    settings.setValue("pitchMyValue",  pitchMyValueSpinBox->value());
    settings.setValue("isAssignedMzValueCheck", isAssignedMzValueCheck->isChecked());
    settings.setValue("minMzValue",  minMzValueSpinBox->value());
    settings.setValue("maxMzValue",  maxMzValueSpinBox->value());
    settings.setValue("pitchMzValue",  pitchMzValueSpinBox->value());
    settings.endGroup();
}
void OutputFigureGraphNmint2DWidget::readSettings(QSettings& settings){
    settings.beginGroup(englishTabName);
    titleVisibleCheck->setChecked(settings.value("titleVisibleCheck").toBool());
    axisColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("axisColor").toString()));
    axisTextColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("axisTextColor").toString()));
    axisTextSizeCombobox->setCurrentIndex(settings.value("axisTextSize").toInt());
    outputLVisibleCheck->setChecked(settings.value("outputLVisibleCheck").toBool());
    outputSVisibleCheck->setChecked(settings.value("outputSVisibleCheck").toBool());
    outputUVisibleCheck->setChecked(settings.value("outputUVisibleCheck").toBool());
    symbolColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("symbolColor").toString()));
    symbolSizeCombobox->setCurrentIndex(settings.value("symbolSize").toInt());
    isAssignedNValueCheck->setChecked(settings.value("isAssignedNValueCheck").toBool());
    minNValueSpinBox->setValue(settings.value("minNValue").toDouble());
    maxNValueSpinBox->setValue(settings.value("maxNValue").toDouble());
    pitchNValueSpinBox->setValue(settings.value("pitchNValue").toDouble());
    isAssignedMyValueCheck->setChecked(settings.value("isAssignedMyValueCheck").toBool());
    minMyValueSpinBox->setValue(settings.value("minMyValue").toDouble());
    maxMyValueSpinBox->setValue(settings.value("maxMyValue").toDouble());
    pitchMyValueSpinBox->setValue(settings.value("pitchMyValue").toDouble());
    isAssignedMzValueCheck->setChecked(settings.value("isAssignedMzValueCheck").toBool());
    minMzValueSpinBox->setValue(settings.value("minMzValue").toDouble());
    maxMzValueSpinBox->setValue(settings.value("maxMzValue").toDouble());
    pitchMzValueSpinBox->setValue(settings.value("pitchMzValue").toDouble());
    settings.endGroup();
}

} // namespace post3dapp

