#include "centerofrigidity2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
namespace post3dapp{

OutputFigureCenterOfRigidity2DWidget::OutputFigureCenterOfRigidity2DWidget(const QString& tabName, const QString& englishTabName, QWidget *parent)
    : OutputFigureCommon2DWidget(tabName, englishTabName, parent)
{
    // その他(層せん断力中心・剛心伏図)
    QLabel *rigidityLable = new QLabel(u8"・層せん断力中心・剛心伏図");
    QLabel *shearForceLabel = new QLabel(u8"せん断力中心カラー：");
    shearForceColorCombobox = new TypicalColorComboBox();
    QLabel *rigidityLabel = new QLabel(u8"剛心カラー:");
    rigidityColorCombobox = new TypicalColorComboBox();
    QLabel *rigidityCircleSizeLabel = new QLabel(u8"表示円サイズ:");
    rigidityCombobox = new FiveSizeComboBox();
    QHBoxLayout *hlay_rigidity = new QHBoxLayout;
    hlay_rigidity->addWidget(shearForceLabel);
    hlay_rigidity->addWidget(shearForceColorCombobox);
    hlay_rigidity->addWidget(rigidityLabel);
    hlay_rigidity->addWidget(rigidityColorCombobox);
    QHBoxLayout *hlay_rigidityCircle = new QHBoxLayout;
    hlay_rigidityCircle->addWidget(rigidityCircleSizeLabel);
    hlay_rigidityCircle->addWidget(rigidityCombobox);

    vlay->insertWidget(vlay->count() - 1, rigidityLable);
    vlay->insertLayout(vlay->count() - 1, hlay_rigidity);

    initializeSettings();
}

void OutputFigureCenterOfRigidity2DWidget::initializeSettings(){
    OutputFigureCommon2DWidget::initializeSettings();
    shearForceColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    rigidityColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    rigidityCombobox->setSize(3);
}

void OutputFigureCenterOfRigidity2DWidget::writeBinaryFile(QDataStream &outfile) const{
    OutputFigureCommon2DWidget::writeBinaryFile(outfile);
    outfile << shearForceColorCombobox->currentIndex();
    outfile << rigidityColorCombobox->currentIndex();
    outfile << rigidityCombobox->currentIndex();
}
void OutputFigureCenterOfRigidity2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    OutputFigureCommon2DWidget::readBinaryFile(infile, version);
    int tempInt;
    infile >> tempInt;
    shearForceColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    rigidityColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    rigidityCombobox->setCurrentIndex(tempInt);
}

void OutputFigureCenterOfRigidity2DWidget::saveSettings(QSettings& settings){
    OutputFigureCommon2DWidget::saveSettings(settings);
    settings.beginGroup(englishTabName);
    settings.setValue("shearForceColor", shearForceColorCombobox->name());
    settings.setValue("rigidityColor", rigidityColorCombobox->name());
    settings.setValue("rigidity", rigidityCombobox->size());
    settings.endGroup();
}
void OutputFigureCenterOfRigidity2DWidget::readSettings(QSettings& settings){
    OutputFigureCommon2DWidget::readSettings(settings);
    settings.beginGroup(englishTabName);
    shearForceColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("shearForceColor").toString()));
    rigidityColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("rigidityColor").toString()));
    rigidityCombobox->setCurrentIndex(settings.value("rigidity").toInt());
    settings.endGroup();
}


} // namespace post3dapp
