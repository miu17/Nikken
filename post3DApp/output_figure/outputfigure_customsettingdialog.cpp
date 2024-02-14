#include "outputfigure_customsettingdialog.h"

#include "app_version.h"
#include "outputfigure_customsettingtabwidget.h"
#include "outputfigure_tabwidget.h"
namespace post3dapp{

OutputFigureCustomSettingDialog::OutputFigureCustomSettingDialog(QWidget *parent)
    : QDialog(parent)
{

    QPushButton *fileOpenButton = new QPushButton(u8"ファイル読込", this);
    QPushButton *fileSaveButton = new QPushButton(u8"ファイル保存", this);
    QPushButton *defaultButton = new QPushButton(u8"デフォルト値", this);
    fileOpenButton->setFixedWidth(90);
    fileSaveButton->setFixedWidth(90);
    defaultButton->setFixedWidth(90);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(fileOpenButton);
    hlay->addWidget(fileSaveButton);
    hlay->addWidget(defaultButton);
    hlay->addStretch();

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addLayout(hlay);
    customSettingTabWidget = new OutputFigureCustomSettingTabWidget();
    vlay->addWidget(customSettingTabWidget);
    this->setLayout(vlay);

    connect( fileSaveButton, &QPushButton::clicked, this, &OutputFigureCustomSettingDialog::slotFileSave);
    connect( fileOpenButton, &QPushButton::clicked, this, &OutputFigureCustomSettingDialog::slotFileOpen);
    connect( defaultButton, &QPushButton::clicked, customSettingTabWidget, &OutputFigureCustomSettingTabWidget::initializeSettings);
}

void OutputFigureCustomSettingDialog::writeBinaryFile(QDataStream &outfile) const{
    this->customSettingTabWidget->writeBinaryFile(outfile);
}
void OutputFigureCustomSettingDialog::readBinaryFile(QDataStream &infile, const AppVersion &version){
    this->customSettingTabWidget->readBinaryFile(infile, version);
}

void OutputFigureCustomSettingDialog::slotFileSave()
{
    QString filepath =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    saveSettings(filepath);
}

void OutputFigureCustomSettingDialog::slotFileOpen()
{
    QString filepath =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.cset)" );
    if ( filepath.isEmpty() ) return;
    readSettings(filepath);
}

void OutputFigureCustomSettingDialog::saveSettings(const QString &filepath) const{
    QSettings settings(filepath, QSettings::IniFormat);
    customSettingTabWidget->saveSettings(settings);
}
void OutputFigureCustomSettingDialog::readSettings(const QString &filepath){
    QSettings settings(filepath, QSettings::IniFormat);
    customSettingTabWidget->readSettings(settings);
}

} // namespace post3dapp
