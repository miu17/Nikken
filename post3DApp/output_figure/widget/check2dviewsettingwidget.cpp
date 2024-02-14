#include "check2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
namespace post3dapp{

OutputFigureCheck2DWidget::OutputFigureCheck2DWidget(const QString& tabName, const QString& englishTabName, QWidget *parent)
    : OutputFigureCommon2DWidget(tabName, englishTabName, parent)
{
    // その他(検定比図)
    QLabel *checkLabel = new QLabel(u8"・検定比図");
    QLabel *check_NGLabel = new QLabel(u8"NGカラー：");
    checkColorCombobox = new TypicalColorComboBox();
    gradationColorCheck = new QCheckBox(u8"グラデーションを利用する");
    QHBoxLayout *hlay_check = new QHBoxLayout;
    hlay_check->addWidget(check_NGLabel);
    hlay_check->addWidget(checkColorCombobox);
    hlay_check->addWidget(gradationColorCheck);

    vlay->insertWidget(vlay->count() - 1, checkLabel);
    vlay->insertLayout(vlay->count() - 1, hlay_check);

    initializeSettings();
}

void OutputFigureCheck2DWidget::initializeSettings(){
    OutputFigureCommon2DWidget::initializeSettings();
    checkColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    gradationColorCheck->setChecked(true);
}

void OutputFigureCheck2DWidget::writeBinaryFile(QDataStream &outfile) const{
    OutputFigureCommon2DWidget::writeBinaryFile(outfile);
    outfile << checkColorCombobox->currentIndex();
    outfile << gradationColorCheck->isChecked();

}
void OutputFigureCheck2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    OutputFigureCommon2DWidget::readBinaryFile(infile, version);
    int tempInt;
    infile >> tempInt;
    checkColorCombobox->setCurrentIndex(tempInt);
    bool tempBool;
    infile >> tempBool;
    gradationColorCheck->setChecked(tempBool);
}

void OutputFigureCheck2DWidget::saveSettings(QSettings& settings){
    OutputFigureCommon2DWidget::saveSettings(settings);
    settings.beginGroup(englishTabName);
    settings.setValue("checkColor", checkColorCombobox->name());
    settings.setValue("gradationColorCheck", gradationColorCheck->isChecked());
    settings.endGroup();
}
void OutputFigureCheck2DWidget::readSettings(QSettings& settings){
    OutputFigureCommon2DWidget::readSettings(settings);
    settings.beginGroup(englishTabName);
    checkColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("checkColor").toString()));
    gradationColorCheck->setChecked(settings.value("gradationColorCheck").toBool());
    settings.endGroup();
}

} // namespace post3dapp
