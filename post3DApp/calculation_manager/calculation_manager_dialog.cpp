#include "calculation_manager_dialog.h"

#include <QBoxLayout>
#include <QDesktopServices>
#include <QLineEdit>
#include <QPushButton>
#include <QUrl>

#include "app_version.h"
#include "calculation_tabwidget.h"
#include "calculationmaker.h"
#include "calculationpapersettingwidget.h"
#include "fixed_data.h"
#include "nposk.h"
#include "posk_plaintextedit.h"
#include "resorce_plaintextedit.h"
#include "unified_editingdata.h"
#include "unified_output.h"
#include "vhline.h"

namespace post3dapp{

CalculationManagerDialog::CalculationManagerDialog(QWidget *parent)
    :QDialog(parent)
{
    this->setWindowTitle(u8"計算書作成");
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    createButtons();

    QVBoxLayout *lay = new QVBoxLayout();
    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(upload_button);
    buttonsLayout->addWidget(new VFLine(Qt::lightGray, this));
    buttonsLayout->addWidget(rewind_button);
    lay->addLayout(buttonsLayout);
    tabWidget = new CalculationTabWidget();
    lay->addWidget(tabWidget);
    QHBoxLayout* runButtonsLayout = new QHBoxLayout();
    runButtonsLayout->setSpacing(5);
    runButtonsLayout->addWidget(pathLineEdit);
    runButtonsLayout->addWidget(reference_button);
    runButtonsLayout->addWidget(run_button);
    lay->addLayout(runButtonsLayout);
    this->setLayout(lay);
}

void CalculationManagerDialog::createButtons(){
    upload_button = new QPushButton(u8"登録");
    rewind_button = new QPushButton(u8"復元");
    pathLineEdit = new QLineEdit("default");
    reference_button = new QPushButton(u8"参照");
    run_button = new QPushButton(u8"実行");

    connect(upload_button, &QPushButton::clicked, this, &CalculationManagerDialog::slotModelUpload);
    connect(rewind_button, &QPushButton::clicked, this, &CalculationManagerDialog::slotModelRewind);
    connect(reference_button, &QPushButton::clicked, this, &CalculationManagerDialog::slotReference);
    connect(run_button, &QPushButton::clicked, this, &CalculationManagerDialog::slotRun);

    connect(UnifiedFixedData::getInstance()->getPstnData(), &UnifiedOutput::fileChanged, this, &CalculationManagerDialog::slotFilenameChanged);
}


void CalculationManagerDialog::slotModelUpload(){}
void CalculationManagerDialog::slotModelRewind(){}
void CalculationManagerDialog::slotReference(){}
void CalculationManagerDialog::slotRun(){
    CalculationMakerLib::CalculationMaker maker(tabWidget->paperWidget->getSetting());
    Nposk nposk;
    if (!nposk.parseAll(&maker, tabWidget->posk->toPlainText().split("\n"))){
        return;
    }
    tabWidget->resorce->setPlainText(nposk.getResorce());

    slotFilenameChanged(); // temporary

    maker.setSource(tabWidget->resorce->toPlainText().toUtf8());
    maker.setOutputFileName(pathLineEdit->text());
    maker.parse();

    QDesktopServices::openUrl(QUrl::fromLocalFile(pathLineEdit->text()));
}

void CalculationManagerDialog::slotFilenameChanged(){
    QString pdfPath = UnifiedEditingData::getInstance()->currentFileDir() + "/" + "out.pdf";
    pathLineEdit->setText(pdfPath);
}

void CalculationManagerDialog::writeBinaryFile(QDataStream &outfile) const{
    tabWidget->writeBinaryFile(outfile);
    outfile << pathLineEdit->text();
}
void CalculationManagerDialog::readBinaryFile(QDataStream &infile, const AppVersion &version){
    tabWidget->readBinaryFile(infile, version);
    QString temp;
    infile >> temp;
    pathLineEdit->setText(temp);
}

} // namespace post3dapp
