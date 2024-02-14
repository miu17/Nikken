#include "horizontalcapacity2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
#include "typicalcolor_table.h"
namespace post3dapp{

OutputFigureGraphHorizontalCapacity2dWidget::OutputFigureGraphHorizontalCapacity2dWidget(const QString& _tabName, const QString& _englishTabName, QWidget *parent)
    : BaseSettingWidget(parent), TabName(_tabName), englishTabName(_englishTabName)
{
    titleVisibleCheck = new QCheckBox(u8"タイトルを表示する");
    QLabel *plasticSymbolColorLabel = new QLabel(u8"必要保有水平耐力のカラー：");
    requiredHorizontalCapacityColorCombobox = new TypicalColorComboBox();
    QLabel *plasticStepColorLabel = new QLabel(u8"保有水平耐力のカラー：");
    horizontalCapacityColorCombobox = new TypicalColorComboBox();
    QLabel *ruledLineColorLabel = new QLabel(u8"罫線のカラー：");
    ruledLineColorCombobox = new TypicalColorComboBox();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(titleVisibleCheck);
    QHBoxLayout *hlay_required = new QHBoxLayout;
    hlay_required->addWidget(plasticSymbolColorLabel);
    hlay_required->addWidget(requiredHorizontalCapacityColorCombobox);
    vlay->addLayout(hlay_required);
    QHBoxLayout *hlay_capacity = new QHBoxLayout;
    hlay_capacity->addWidget(plasticStepColorLabel);
    hlay_capacity->addWidget(horizontalCapacityColorCombobox);
    vlay->addLayout(hlay_capacity);
    QHBoxLayout *hlay_line = new QHBoxLayout;
    hlay_line->addWidget(ruledLineColorLabel);
    hlay_line->addWidget(ruledLineColorCombobox);
    vlay->addLayout(hlay_line);
    vlay->addStretch();
    this->setLayout(vlay);

    initializeSettings();
}

void OutputFigureGraphHorizontalCapacity2dWidget::initializeSettings(){
    titleVisibleCheck->setChecked(true);
    requiredHorizontalCapacityColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    horizontalCapacityColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    ruledLineColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
}

void OutputFigureGraphHorizontalCapacity2dWidget::writeBinaryFile(QDataStream &outfile) const{
    outfile << titleVisibleCheck->isChecked();
    outfile << requiredHorizontalCapacityColorCombobox->currentIndex();
    outfile << horizontalCapacityColorCombobox->currentIndex();
    outfile << ruledLineColorCombobox->currentIndex();
}
void OutputFigureGraphHorizontalCapacity2dWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    bool tempBool;
    infile >> tempBool;
    titleVisibleCheck->setChecked(tempBool);
    int tempInt;
    infile >> tempInt;
    requiredHorizontalCapacityColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    horizontalCapacityColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    ruledLineColorCombobox->setCurrentIndex(tempInt);
}

void OutputFigureGraphHorizontalCapacity2dWidget::saveSettings(QSettings& settings){
    settings.beginGroup(englishTabName);
    settings.setValue("titleVisibleCheck", titleVisibleCheck->isChecked());
    settings.setValue("requiredHorizontalCapacityColor", requiredHorizontalCapacityColorCombobox->name());
    settings.setValue("horizontalCapacityColor", horizontalCapacityColorCombobox->name());
    settings.setValue("ruledLineColor", ruledLineColorCombobox->name());
    settings.endGroup();
}
void OutputFigureGraphHorizontalCapacity2dWidget::readSettings(QSettings& settings){
    settings.beginGroup(englishTabName);
    titleVisibleCheck->setChecked(settings.value("titleVisibleCheck").toBool());
    requiredHorizontalCapacityColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("requiredHorizontalCapacityColor").toString()));
    horizontalCapacityColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("horizontalCapacityColor").toString()));
    ruledLineColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("ruledLineColor").toString()));
    settings.endGroup();
}

} // namespace post3dapp
