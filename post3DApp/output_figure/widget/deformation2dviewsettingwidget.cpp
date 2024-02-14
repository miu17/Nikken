#include "deformation2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
namespace post3dapp{

OutputFigureDeformation2DWidget::OutputFigureDeformation2DWidget(const QString& tabName, const QString& englishTabName, QWidget *parent)
    : OutputFigureCommon2DWidget(tabName, englishTabName, parent)
{
    // その他(変形図)
    QLabel *deformationLabel = new QLabel(u8"・変形図");
    QLabel *deformation_beforeLabel = new QLabel(u8"変形前カラー：");
    beforeColorCombobox = new TypicalColorComboBox();
    QLabel *deformation_afterLabel = new QLabel(u8"変形後カラー：");
    afterColorCombobox = new TypicalColorComboBox();
    QHBoxLayout *hlay_deformation = new QHBoxLayout;
    hlay_deformation->addWidget(deformation_beforeLabel);
    hlay_deformation->addWidget(beforeColorCombobox);
    hlay_deformation->addWidget(deformation_afterLabel);
    hlay_deformation->addWidget(afterColorCombobox);

    vlay->insertWidget(vlay->count() - 1, deformationLabel);
    vlay->insertLayout(vlay->count() - 1, hlay_deformation);

    initializeSettings();
}

void OutputFigureDeformation2DWidget::initializeSettings(){
    OutputFigureCommon2DWidget::initializeSettings();
    beforeColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    afterColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
}

void OutputFigureDeformation2DWidget::writeBinaryFile(QDataStream &outfile) const{
    OutputFigureCommon2DWidget::writeBinaryFile(outfile);
    outfile << beforeColorCombobox->currentIndex();
    outfile << afterColorCombobox->currentIndex();
}
void OutputFigureDeformation2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    OutputFigureCommon2DWidget::readBinaryFile(infile, version);
    int tempInt;
    infile >> tempInt;
    beforeColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    afterColorCombobox->setCurrentIndex(tempInt);
}

void OutputFigureDeformation2DWidget::saveSettings(QSettings& settings){
    OutputFigureCommon2DWidget::saveSettings(settings);
    settings.beginGroup(englishTabName);
    settings.setValue("beforeColor", beforeColorCombobox->name());
    settings.setValue("afterColor", afterColorCombobox->name());
    settings.endGroup();
}
void OutputFigureDeformation2DWidget::readSettings(QSettings& settings){
    OutputFigureCommon2DWidget::readSettings(settings);
    settings.beginGroup(englishTabName);
    beforeColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("beforeColor").toString()));
    afterColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("afterColor").toString()));
    settings.endGroup();
}
} // namespace post3dapp

