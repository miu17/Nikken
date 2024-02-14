#include "calculation_tabwidget.h"

#include "app_version.h"
#include "buildinginformationwidget.h"
#include "calculationpapersettingwidget.h"
#include "posk_plaintextedit.h"
#include "resorce_plaintextedit.h"

namespace post3dapp {

CalculationTabWidget::CalculationTabWidget(QWidget *parent)
    :QTabWidget(parent)
{
    this->setFixedSize(900, 900);
    posk = new PoskPlainTextEdit();
    binfo = new BuildingInformationWidget();
    paperWidget = new CalculationPaperSettingWidget();

    resorce = new ResorcePlainTextEdit();

    this->addTab(posk, "posk");
    this->addTab(binfo, "building information");
    this->addTab(paperWidget, "paper setting");
    this->addTab(resorce, "convert");
}

void CalculationTabWidget::writeBinaryFile(QDataStream &outfile) const{
    outfile << posk->toPlainText();
    binfo->writeBinaryFile(outfile);
    paperWidget->writeBinaryFile(outfile);
}
void CalculationTabWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    QString temp;
    infile >> temp;
    posk->setPlainText(temp);
    binfo->readBinaryFile(infile, version);
    paperWidget->readBinaryFile(infile, version);
}

} // namespace post3dapp

