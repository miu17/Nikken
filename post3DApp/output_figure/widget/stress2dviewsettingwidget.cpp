#include "stress2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
namespace post3dapp{

OutputFigureStress2DWidget::OutputFigureStress2DWidget(const QString& tabName, const QString& englishTabName, QWidget *parent)
    : OutputFigureCommon2DWidget(tabName, englishTabName, parent)
{
    // 応力図
    QLabel *stressLabel = new QLabel(u8"・応力図");
    QLabel *stressLineColorLabel = new QLabel(u8"応力線の色");
    stressColorCombobox = new TypicalColorComboBox();
    QLabel *stressLineScaleLabel = new QLabel(u8"スケール");
    stressScaleSpin = new QDoubleSpinBox();
    stressScaleSpin->setRange(0.01, 999.99);
    QLabel *stressLineScale2Label = new QLabel(u8" kN/m = 1.0m");

    QLabel *stressValueLabel = new QLabel(u8"値：");
    NValueCheck = new QCheckBox(u8"N値の表示");
    MValueCheck = new QCheckBox(u8"M値の表示");
    QValueCheck = new QCheckBox(u8"Q値の表示");

    QLabel *stressColummLabel = new QLabel(u8"柱：");
    columnValueCheck = new QCheckBox(u8"表示");
    stressColumnColorCombobox = new TypicalColorComboBox();

    QLabel *stressGirderLabel = new QLabel(u8"大梁：");
    girderValueCheck = new QCheckBox(u8"表示");
    stressGirderColorCombobox = new TypicalColorComboBox();

    QLabel *stressBraceLabel = new QLabel(u8"ブレース：");
    braceValueCheck = new QCheckBox(u8"表示");
    stressBraceColorCombobox = new TypicalColorComboBox();

    QLabel *stressWallLabel = new QLabel(u8"壁：");
    wallValueCheck = new QCheckBox(u8"表示");
    stressWallColorCombobox = new TypicalColorComboBox();

    QLabel *stressSlabLabel = new QLabel(u8"スラブ：");
    slabValueCheck = new QCheckBox(u8"表示");
    stressSlabColorCombobox = new TypicalColorComboBox();

    QLabel *stressPanelLabel = new QLabel(u8"パネル：");
    panelValueCheck = new QCheckBox(u8"表示");
    stressPanelColorCombobox = new TypicalColorComboBox();

    QLabel *stressSpringLabel = new QLabel(u8"ばね：");
    springValueCheck = new QCheckBox(u8"表示");
    stressSpringColorCombobox = new TypicalColorComboBox();

    QLabel *stressDamperLabel = new QLabel(u8"制振：");
    damperValueCheck = new QCheckBox(u8"表示");
    stressDamperColorCombobox = new TypicalColorComboBox();

    QLabel *stressIsoLabel = new QLabel(u8"免震：");
    isoValueCheck = new QCheckBox(u8"表示");
    stressIsoColorCombobox = new TypicalColorComboBox();


    QHBoxLayout *hlay_stress = new QHBoxLayout;
    hlay_stress->addWidget(stressLineColorLabel);
    hlay_stress->addWidget(stressColorCombobox);

    QHBoxLayout *hlay_stress2 = new QHBoxLayout;
    hlay_stress2->addWidget(stressLineScaleLabel);
    hlay_stress2->addWidget(stressScaleSpin);
    hlay_stress2->addWidget(stressLineScale2Label);

    QHBoxLayout *hlay_stress_value = new QHBoxLayout;
    hlay_stress_value->addWidget(stressValueLabel);
    hlay_stress_value->addWidget(NValueCheck);
    hlay_stress_value->addWidget(MValueCheck);
    hlay_stress_value->addWidget(QValueCheck);

    QHBoxLayout *hlay_stressColumn = new QHBoxLayout;
    hlay_stressColumn->addWidget(stressColummLabel);
    hlay_stressColumn->addWidget(columnValueCheck);
    hlay_stressColumn->addWidget(new QLabel(u8"カラー："));
    hlay_stressColumn->addWidget(stressColumnColorCombobox);

    QHBoxLayout *hlay_stressGirder = new QHBoxLayout;
    hlay_stressGirder->addWidget(stressGirderLabel);
    hlay_stressGirder->addWidget(girderValueCheck);
    hlay_stressGirder->addWidget(new QLabel(u8"カラー："));
    hlay_stressGirder->addWidget(stressGirderColorCombobox);

    QHBoxLayout *hlay_stressBrace = new QHBoxLayout;
    hlay_stressBrace->addWidget(stressBraceLabel);
    hlay_stressBrace->addWidget(braceValueCheck);
    hlay_stressBrace->addWidget(new QLabel(u8"カラー："));
    hlay_stressBrace->addWidget(stressBraceColorCombobox);

    QHBoxLayout *hlay_stressWall = new QHBoxLayout;
    hlay_stressWall->addWidget(stressWallLabel);
    hlay_stressWall->addWidget(wallValueCheck);
    hlay_stressWall->addWidget(new QLabel(u8"カラー："));
    hlay_stressWall->addWidget(stressWallColorCombobox);

    QHBoxLayout *hlay_stressSlab = new QHBoxLayout;
    hlay_stressSlab->addWidget(stressSlabLabel);
    hlay_stressSlab->addWidget(slabValueCheck);
    hlay_stressSlab->addWidget(new QLabel(u8"カラー："));
    hlay_stressSlab->addWidget(stressSlabColorCombobox);

    QHBoxLayout *hlay_stressPanel = new QHBoxLayout;
    hlay_stressPanel->addWidget(stressPanelLabel);
    hlay_stressPanel->addWidget(panelValueCheck);
    hlay_stressPanel->addWidget(new QLabel(u8"カラー："));
    hlay_stressPanel->addWidget(stressPanelColorCombobox);

    QHBoxLayout *hlay_stressSpring = new QHBoxLayout;
    hlay_stressSpring->addWidget(stressSpringLabel);
    hlay_stressSpring->addWidget(springValueCheck);
    hlay_stressSpring->addWidget(new QLabel(u8"カラー："));
    hlay_stressSpring->addWidget(stressSpringColorCombobox);

    QHBoxLayout *hlay_stressDamper = new QHBoxLayout;
    hlay_stressDamper->addWidget(stressDamperLabel);
    hlay_stressDamper->addWidget(damperValueCheck);
    hlay_stressDamper->addWidget(new QLabel(u8"カラー："));
    hlay_stressDamper->addWidget(stressDamperColorCombobox);

    QHBoxLayout *hlay_stressIso = new QHBoxLayout;
    hlay_stressIso->addWidget(stressIsoLabel);
    hlay_stressIso->addWidget(isoValueCheck);
    hlay_stressIso->addWidget(new QLabel(u8"カラー："));
    hlay_stressIso->addWidget(stressIsoColorCombobox);

    // 塑性記号
    QLabel *plasticLabel = new QLabel(u8"・塑性記号");
    QLabel *plasticSymbolColorLabel = new QLabel(u8"塑性記号カラー");
    plasticSymbolColorCombobox = new TypicalColorComboBox();
    QLabel *plasticSymbolSizeLabel = new QLabel(u8"塑性記号サイズ");
    plasticSymbolSizeCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_plastic = new QHBoxLayout;
    hlay_plastic->addWidget(plasticLabel);
    hlay_plastic->addWidget(plasticSymbolColorLabel);
    hlay_plastic->addWidget(plasticSymbolColorCombobox);
    hlay_plastic->addWidget(plasticSymbolSizeLabel);
    hlay_plastic->addWidget(plasticSymbolSizeCombobox);

    QVBoxLayout *vlay_stress = new QVBoxLayout;
    vlay_stress->addWidget(stressLabel);
    vlay_stress->addLayout(hlay_stress);
    vlay_stress->addLayout(hlay_stress2);
    vlay_stress->addLayout(hlay_stress_value);
    vlay_stress->addLayout(hlay_stressColumn);
    vlay_stress->addLayout(hlay_stressGirder);
    vlay_stress->addLayout(hlay_stressBrace);
    vlay_stress->addLayout(hlay_stressWall);
    vlay_stress->addLayout(hlay_stressSlab);
    vlay_stress->addLayout(hlay_stressPanel);
    vlay_stress->addLayout(hlay_stressSpring);
    vlay_stress->addLayout(hlay_stressDamper);
    vlay_stress->addLayout(hlay_stressIso);
    vlay_stress->addLayout(hlay_plastic);

    vlay->insertLayout(vlay->count() - 1, vlay_stress);

    initializeSettings();
}

void OutputFigureStress2DWidget::initializeSettings(){
    OutputFigureCommon2DWidget::initializeSettings();
    stressColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    stressScaleSpin->setValue(1.0);

    NValueCheck->setChecked(true);
    MValueCheck->setChecked(true);
    QValueCheck->setChecked(true);

    columnValueCheck->setChecked(true);
    stressColumnColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    girderValueCheck->setChecked(true);
    stressGirderColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    braceValueCheck->setChecked(true);
    stressBraceColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    wallValueCheck->setChecked(true);
    stressWallColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    slabValueCheck->setChecked(true);
    stressSlabColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    panelValueCheck->setChecked(true);
    stressPanelColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    springValueCheck->setChecked(true);
    stressSpringColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    damperValueCheck->setChecked(true);
    stressDamperColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    isoValueCheck->setChecked(true);
    stressIsoColorCombobox->setColor(TypicalColorTable::nameToColor("black"));

    plasticSymbolColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    plasticSymbolSizeCombobox->setSize(3);
}

void OutputFigureStress2DWidget::writeBinaryFile(QDataStream &outfile) const{
    OutputFigureCommon2DWidget::writeBinaryFile(outfile);
    outfile << stressColorCombobox->currentIndex();
    outfile << stressScaleSpin->value();

    outfile << NValueCheck->isChecked();
    outfile << MValueCheck->isChecked();
    outfile << QValueCheck->isChecked();

    outfile << columnValueCheck->isChecked();
    outfile << stressColumnColorCombobox->currentIndex();

    outfile << girderValueCheck->isChecked();
    outfile << stressGirderColorCombobox->currentIndex();

    outfile << braceValueCheck->isChecked();
    outfile << stressBraceColorCombobox->currentIndex();

    outfile << wallValueCheck->isChecked();
    outfile << stressWallColorCombobox->currentIndex();

    outfile << slabValueCheck->isChecked();
    outfile << stressSlabColorCombobox->currentIndex();

    outfile << panelValueCheck->isChecked();
    outfile << stressPanelColorCombobox->currentIndex();

    outfile << springValueCheck->isChecked();
    outfile << stressSpringColorCombobox->currentIndex();

    outfile << damperValueCheck->isChecked();
    outfile << stressDamperColorCombobox->currentIndex();

    outfile << isoValueCheck->isChecked();
    outfile << stressIsoColorCombobox->currentIndex();

    outfile << plasticSymbolColorCombobox->currentIndex();
    outfile << plasticSymbolSizeCombobox->currentIndex();
}
void OutputFigureStress2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    OutputFigureCommon2DWidget::readBinaryFile(infile, version);
    int tempInt;
    infile >> tempInt;
    stressColorCombobox->setCurrentIndex(tempInt);
    double tempDouble;
    infile >> tempDouble;
    stressScaleSpin->setValue(tempDouble);

    bool tempBool;
    infile >> tempBool;
    NValueCheck->setChecked(tempBool);
    infile >> tempBool;
    MValueCheck->setChecked(tempBool);
    infile >> tempBool;
    QValueCheck->setChecked(tempBool);

    infile >> tempBool;
    columnValueCheck->setChecked(tempBool);
    infile >> tempInt;
    stressColumnColorCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    girderValueCheck->setChecked(tempBool);
    infile >> tempInt;
    stressGirderColorCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    braceValueCheck->setChecked(tempBool);
    infile >> tempInt;
    stressBraceColorCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    wallValueCheck->setChecked(tempBool);
    infile >> tempInt;
    stressWallColorCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    slabValueCheck->setChecked(tempBool);
    infile >> tempInt;
    stressSlabColorCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    panelValueCheck->setChecked(tempBool);
    infile >> tempInt;
    stressPanelColorCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    springValueCheck->setChecked(tempBool);
    infile >> tempInt;
    stressSpringColorCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    damperValueCheck->setChecked(tempBool);
    infile >> tempInt;
    stressDamperColorCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    isoValueCheck->setChecked(tempBool);
    infile >> tempInt;
    stressIsoColorCombobox->setCurrentIndex(tempInt);

    infile >> tempInt;
    plasticSymbolColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    plasticSymbolSizeCombobox->setCurrentIndex(tempInt);
}

void OutputFigureStress2DWidget::saveSettings(QSettings& settings){
    OutputFigureCommon2DWidget::saveSettings(settings);
    settings.beginGroup(englishTabName);
    settings.setValue("stressColor", stressColorCombobox->name());
    settings.setValue("stressScale", stressScaleSpin->value());
    settings.setValue("NValueCheck", NValueCheck->isChecked());
    settings.setValue("MValueCheck", MValueCheck->isChecked());
    settings.setValue("QValueCheck", QValueCheck->isChecked());
    settings.setValue("columnValueCheck", columnValueCheck->isChecked());
    settings.setValue("stressColumnColor", stressColumnColorCombobox->name());
    settings.setValue("girderValueCheck", girderValueCheck->isChecked());
    settings.setValue("stressGirderColor", stressGirderColorCombobox->name());
    settings.setValue("braceValueCheck", braceValueCheck->isChecked());
    settings.setValue("stressBraceColor", stressBraceColorCombobox->name());
    settings.setValue("wallValueCheck", wallValueCheck->isChecked());
    settings.setValue("stressWallColor", stressWallColorCombobox->name());
    settings.setValue("slabValueCheck", slabValueCheck->isChecked());
    settings.setValue("stressSlabColor", stressSlabColorCombobox->name());
    settings.setValue("panelValueCheck", panelValueCheck->isChecked());
    settings.setValue("stressPanelColor", stressPanelColorCombobox->name());
    settings.setValue("springValueCheck", springValueCheck->isChecked());
    settings.setValue("stressSpringColor", stressSpringColorCombobox->name());
    settings.setValue("damperValueCheck", damperValueCheck->isChecked());
    settings.setValue("stressDamperColor", stressDamperColorCombobox->name());
    settings.setValue("isoValueCheck", isoValueCheck->isChecked());
    settings.setValue("stressIsoColor", stressIsoColorCombobox->name());
    settings.setValue("plasticSymbolColor", plasticSymbolColorCombobox->name());
    settings.setValue("plasticSymbolSize", plasticSymbolSizeCombobox->size());
    settings.endGroup();
}
void OutputFigureStress2DWidget::readSettings(QSettings& settings){
    OutputFigureCommon2DWidget::readSettings(settings);
    settings.beginGroup(englishTabName);
    stressColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressColor").toString()));
    stressScaleSpin->setValue(settings.value("stressScale").toDouble());
    NValueCheck->setChecked(settings.value("NValueCheck").toBool());
    MValueCheck->setChecked(settings.value("MValueCheck").toBool());
    QValueCheck->setChecked(settings.value("QValueCheck").toBool());
    columnValueCheck->setChecked(settings.value("columnValueCheck").toBool());
    stressColumnColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressColumnColor").toString()));
    girderValueCheck->setChecked(settings.value("girderValueCheck").toBool());
    stressGirderColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressGirderColor").toString()));
    braceValueCheck->setChecked(settings.value("braceValueCheck").toBool());
    stressBraceColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressBraceColor").toString()));
    wallValueCheck->setChecked(settings.value("wallValueCheck").toBool());
    stressWallColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressWallColor").toString()));
    slabValueCheck->setChecked(settings.value("slabValueCheck").toBool());
    stressSlabColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressSlabColor").toString()));
    panelValueCheck->setChecked(settings.value("panelValueCheck").toBool());
    stressPanelColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressPanelColor").toString()));
    springValueCheck->setChecked(settings.value("springValueCheck").toBool());
    stressSpringColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressSpringColor").toString()));
    damperValueCheck->setChecked(settings.value("damperValueCheck").toBool());
    stressDamperColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressDamperColor").toString()));
    isoValueCheck->setChecked(settings.value("isoValueCheck").toBool());
    stressIsoColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("stressIsoColor").toString()));
    plasticSymbolColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("plasticSymbolColor").toString()));
    plasticSymbolSizeCombobox->setCurrentIndex(settings.value("plasticSymbolSize").toInt());
    settings.endGroup();
}


} // namespace post3dapp
