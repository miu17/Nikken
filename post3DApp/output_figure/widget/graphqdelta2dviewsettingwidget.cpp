#include "graphqdelta2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
namespace post3dapp{

OutputFigureGraphQDelta2DWidget::OutputFigureGraphQDelta2DWidget(const QString& _tabName, const QString& _englishTabName, QWidget *parent)
    : BaseSettingWidget(parent), TabName(_tabName), englishTabName(_englishTabName)
{
    titleVisibleCheck = new QCheckBox(u8"タイトルを表示する");
    QLabel *plasticSymbolColorLabel = new QLabel(u8"塑性記号のカラー：");
    plasticSymbolColorCombobox = new TypicalColorComboBox();
    QLabel *plasticStepColorLabel = new QLabel(u8"塑性ステップのカラー：");
    plasticStepColorCombobox = new TypicalColorComboBox();
    QLabel *ruledLineColorLabel = new QLabel(u8"罫線のカラー：");
    ruledLineColorCombobox = new TypicalColorComboBox();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(titleVisibleCheck);
    QHBoxLayout *hlay_symbol = new QHBoxLayout;
    hlay_symbol->addWidget(plasticSymbolColorLabel);
    hlay_symbol->addWidget(plasticSymbolColorCombobox);
    vlay->addLayout(hlay_symbol);
    QHBoxLayout *hlay_step = new QHBoxLayout;
    hlay_step->addWidget(plasticStepColorLabel);
    hlay_step->addWidget(plasticStepColorCombobox);
    vlay->addLayout(hlay_step);
    QHBoxLayout *hlay_line = new QHBoxLayout;
    hlay_line->addWidget(ruledLineColorLabel);
    hlay_line->addWidget(ruledLineColorCombobox);
    vlay->addLayout(hlay_line);
    vlay->addStretch();
    this->setLayout(vlay);

    initializeSettings();
}

void OutputFigureGraphQDelta2DWidget::initializeSettings(){
    titleVisibleCheck->setChecked(true);
    plasticSymbolColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    plasticStepColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    ruledLineColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
}

void OutputFigureGraphQDelta2DWidget::writeBinaryFile(QDataStream &outfile) const{
    outfile << titleVisibleCheck->isChecked();
    outfile << plasticSymbolColorCombobox->currentIndex();
    outfile << plasticStepColorCombobox->currentIndex();
    outfile << ruledLineColorCombobox->currentIndex();
}
void OutputFigureGraphQDelta2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    bool tempBool;
    infile >> tempBool;
    titleVisibleCheck->setChecked(tempBool);
    int tempInt;
    infile >> tempInt;
    plasticSymbolColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    plasticStepColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    ruledLineColorCombobox->setCurrentIndex(tempInt);

}

void OutputFigureGraphQDelta2DWidget::saveSettings(QSettings& settings){
    settings.beginGroup(englishTabName);
    settings.setValue("titleVisibleCheck", titleVisibleCheck->isChecked());
    settings.setValue("plasticSymbolColor", plasticSymbolColorCombobox->name());
    settings.setValue("plastecStepColor", plasticStepColorCombobox->name());
    settings.setValue("ruledLineColor", ruledLineColorCombobox->name());
    settings.endGroup();
}
void OutputFigureGraphQDelta2DWidget::readSettings(QSettings& settings){
    settings.beginGroup(englishTabName);
    titleVisibleCheck->setChecked(settings.value("titleVisibleCheck").toBool());
    plasticSymbolColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("plasticSymbolColor").toString()));
    plasticStepColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("plastecStepColor").toString()));
    ruledLineColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("ruledLineColor").toString()));
    settings.endGroup();
}
} // namespace post3dapp
