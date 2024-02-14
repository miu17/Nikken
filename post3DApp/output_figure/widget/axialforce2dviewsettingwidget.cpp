#include "axialforce2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"

namespace post3dapp{

OutputFigureAxialForce2DWidget::OutputFigureAxialForce2DWidget(const QString& tabName, const QString& englishTabName, QWidget *parent)
    : OutputFigureCommon2DWidget(tabName, englishTabName, parent)
{
    // その他(軸力図)
    QLabel *axialLabel = new QLabel(u8"・軸力図");
    QLabel *axialMinusLabel = new QLabel(u8"圧縮カラー：");
    axialMinusColorCombobox = new TypicalColorComboBox();
    QLabel *axialPlusLabel = new QLabel(u8"引張カラー：");
    axialPlusColorCombobox = new TypicalColorComboBox();
    QHBoxLayout *hlay_axial = new QHBoxLayout;
    hlay_axial->addWidget(axialMinusLabel);
    hlay_axial->addWidget(axialMinusColorCombobox);
    hlay_axial->addWidget(axialPlusLabel);
    hlay_axial->addWidget(axialPlusColorCombobox);

    // lastがStretch
    vlay->insertWidget(vlay->count() - 1, axialLabel);
    vlay->insertLayout(vlay->count() - 1, hlay_axial);

    initializeSettings();
}

void OutputFigureAxialForce2DWidget::initializeSettings(){
    OutputFigureCommon2DWidget::initializeSettings();
    axialMinusColorCombobox->setColor(TypicalColorTable::nameToColor("blue"));
    axialPlusColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
}

void OutputFigureAxialForce2DWidget::writeBinaryFile(QDataStream &outfile) const{
    OutputFigureCommon2DWidget::writeBinaryFile(outfile);
    outfile << axialMinusColorCombobox->currentIndex();
    outfile << axialPlusColorCombobox->currentIndex();
}
void OutputFigureAxialForce2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    OutputFigureCommon2DWidget::readBinaryFile(infile, version);
    int tempInt;
    infile >> tempInt;
    axialMinusColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    axialPlusColorCombobox->setCurrentIndex(tempInt);
}

void OutputFigureAxialForce2DWidget::saveSettings(QSettings& settings){
    OutputFigureCommon2DWidget::saveSettings(settings);
    settings.beginGroup(englishTabName);
    settings.setValue("axialMinusColor", axialMinusColorCombobox->name());
    settings.setValue("axialPlusColor", axialPlusColorCombobox->name());
    settings.endGroup();
}
void OutputFigureAxialForce2DWidget::readSettings(QSettings& settings){
    OutputFigureCommon2DWidget::readSettings(settings);
    settings.beginGroup(englishTabName);
    axialMinusColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("axialMinusColor").toString()));
    axialPlusColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("axialPlusColor").toString()));
    settings.endGroup();
}


} // namespace post3dapp
