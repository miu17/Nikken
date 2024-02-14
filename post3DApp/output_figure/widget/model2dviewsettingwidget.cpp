#include "model2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "viewsetting_partswidget.h"

namespace post3dapp{

OutputFigureModel2DWidget::OutputFigureModel2DWidget(const QString& _tabName, const QString& _englishTabName, QWidget *parent)
    : Analysis2DSettingModelWidget(_tabName, _englishTabName, parent)
{

}

void OutputFigureModel2DWidget::writeBinaryFile(QDataStream &outfile) const{
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

    outfile << columnMarkCheck->isChecked();
    outfile << girderMarkCheck->isChecked();
    outfile << braceMarkCheck->isChecked();
    outfile << wallMarkCheck->isChecked();
    outfile << steelWallMarkCheck->isChecked();
    outfile << foundationWallMarkCheck->isChecked();
    outfile << slabMarkCheck->isChecked();
    outfile << damperMarkCheck->isChecked();
    outfile << isoMarkCheck->isChecked();

    outfile << markSizeCombobox->currentIndex();

    outfile << jointNumberCheck->isChecked();
    outfile << jointNumberColorCombobox->currentIndex();
    outfile << jointNumberSizeCombobox->currentIndex();

    outfile << boundaryConditionCheck->isChecked();
    outfile << boundaryConditionColorCombobox->currentIndex();
    outfile << boundaryConditionSizeCombobox->currentIndex();

    outfile << directionCheckbox->isChecked();

    outfile << zwallColorCombobox->currentIndex();
    outfile << rigidRangeColorCombobox->currentIndex();
    outfile << panelZoneColorCombobox->currentIndex();
}

void OutputFigureModel2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    bool tempBool;
    int tempInt;
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

    infile >> tempBool;
    columnMarkCheck->setChecked(tempBool);
    infile >> tempBool;
    girderMarkCheck->setChecked(tempBool);
    infile >> tempBool;
    braceMarkCheck->setChecked(tempBool);
    infile >> tempBool;
    wallMarkCheck->setChecked(tempBool);
    infile >> tempBool;
    steelWallMarkCheck->setChecked(tempBool);
    infile >> tempBool;
    foundationWallMarkCheck->setChecked(tempBool);
    infile >> tempBool;
    slabMarkCheck->setChecked(tempBool);
    infile >> tempBool;
    damperMarkCheck->setChecked(tempBool);
    infile >> tempBool;
    isoMarkCheck->setChecked(tempBool);

    infile >> tempInt;
    markSizeCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    jointNumberCheck->setChecked(tempBool);
    infile >> tempInt;
    jointNumberColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    jointNumberSizeCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    boundaryConditionCheck->setChecked(tempBool);
    infile >> tempInt;
    boundaryConditionColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    boundaryConditionSizeCombobox->setCurrentIndex(tempInt);

    infile >> tempBool;
    directionCheckbox->setChecked(tempBool);

    infile >> tempInt;
    zwallColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    rigidRangeColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    panelZoneColorCombobox->setCurrentIndex(tempInt);
}

} // namespace post3dapp
