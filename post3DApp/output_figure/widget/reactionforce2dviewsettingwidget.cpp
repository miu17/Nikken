#include "reactionforce2dviewsettingwidget.h"

#include "app_version.h"
#include "typicalcolor_combobox.h"
#include "typicalcolor_table.h"
namespace post3dapp{

OutputFigureReactionForce2DWidget::OutputFigureReactionForce2DWidget(const QString& tabName, const QString& englishTabName, QWidget *parent)
    : OutputFigureCommon2DWidget(tabName, englishTabName, parent)
{
    // その他(反力図・反力マップ・組み合わせ反力マップ)
    QLabel *reactionLabel = new QLabel(u8"・反力マップ／組み合わせ反力マップ");
    QLabel *reactionMinusLabel = new QLabel(u8"正：");
    reactionMinusColorCombobox = new TypicalColorComboBox();
    QLabel *reactionPlusLabel = new QLabel(u8"負：");
    reactionPlusColorCombobox = new TypicalColorComboBox();
    QHBoxLayout *hlay_reaction = new QHBoxLayout;
    hlay_reaction->addWidget(reactionMinusLabel);
    hlay_reaction->addWidget(reactionMinusColorCombobox);
    hlay_reaction->addWidget(reactionPlusLabel);
    hlay_reaction->addWidget(reactionPlusColorCombobox);

    vlay->insertWidget(vlay->count() - 1, reactionLabel);
    vlay->insertLayout(vlay->count() - 1, hlay_reaction);

    initializeSettings();
}

void OutputFigureReactionForce2DWidget::initializeSettings(){
    OutputFigureCommon2DWidget::initializeSettings();
    reactionMinusColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
    reactionPlusColorCombobox->setColor(TypicalColorTable::nameToColor("black"));
}

void OutputFigureReactionForce2DWidget::writeBinaryFile(QDataStream &outfile) const{
    OutputFigureCommon2DWidget::writeBinaryFile(outfile);
    outfile << reactionMinusColorCombobox->currentIndex();
    outfile << reactionPlusColorCombobox->currentIndex();
}
void OutputFigureReactionForce2DWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    OutputFigureCommon2DWidget::readBinaryFile(infile, version);
    int tempInt;
    infile >> tempInt;
    reactionMinusColorCombobox->setCurrentIndex(tempInt);
    infile >> tempInt;
    reactionPlusColorCombobox->setCurrentIndex(tempInt);
}

void OutputFigureReactionForce2DWidget::saveSettings(QSettings& settings){
    OutputFigureCommon2DWidget::saveSettings(settings);
    settings.beginGroup(englishTabName);
    settings.setValue("reactionMinusColor", reactionMinusColorCombobox->name());
    settings.setValue("reactionPlusColor", reactionPlusColorCombobox->name());
    settings.endGroup();
}
void OutputFigureReactionForce2DWidget::readSettings(QSettings& settings){
    OutputFigureCommon2DWidget::readSettings(settings);
    settings.beginGroup(englishTabName);
    reactionMinusColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("reactionMinusColor").toString()));
    reactionPlusColorCombobox->setCurrentIndex(TypicalColorComboBox::nameToIndex(settings.value("reactionPlusColor").toString()));
    settings.endGroup();
}

} // namespace post3dapp
