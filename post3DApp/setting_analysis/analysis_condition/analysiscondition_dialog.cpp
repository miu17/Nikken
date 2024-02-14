#include "analysiscondition_dialog.h"

#include <QFileDialog>
#include <QLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>

#include "analysis_tabwidget.h"
#include "app_version.h"
#include "analysisconditionreader.h"
#include "menu_button.h"
#include "unified_inputdata.h"
#include "unified_project.h"
#include "utility.h"
#include "vhline.h"

namespace post3dapp {

AnalysisConditionDialog::AnalysisConditionDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle(u8"解析条件");
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    createButtons();
    tabWidget = new AnalysisTabWidget();

    QVBoxLayout *lay = new QVBoxLayout(this);
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(5);
    buttonsLayout->addWidget(open_button);
    buttonsLayout->addWidget(save_button);
    buttonsLayout->addWidget(convert_button);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(upload_button);
    buttonsLayout->addWidget(new VFLine(Qt::lightGray, this));
    buttonsLayout->addWidget(rewind_button);
    lay->addLayout(buttonsLayout);
    lay->addWidget(tabWidget);
    this->setLayout(lay);
    this->setDataModified(false);

    this->setWindowIcon(QIcon(":/icons/c2_blue.png"));

    connect(tabWidget, &AnalysisTabWidget::dataChanged, this, &AnalysisConditionDialog::slotDataChanged);

}

void AnalysisConditionDialog::createButtons()
{

    open_button = new QPushButton(u8"ファイル読込", this);
    open_button->setFixedSize(90, 30);
    open_button->setAutoDefault(false);
    save_button = new QPushButton(u8"ファイル保存", this);
    save_button->setFixedSize(90, 30);
    save_button->setAutoDefault(false);

    convert_button = new QPushButton(u8"変換", this);
    convert_button->setFixedSize(90, 30);
    convert_button->setAutoDefault(false);

    upload_button = new QPushButton(u8"登録", this);
    upload_button->setFixedSize(80, 30);
    upload_button->setAutoDefault(false);
    rewind_button = new QPushButton(u8"復元", this);
    rewind_button->setFixedSize(80, 30);
    rewind_button->setAutoDefault(false);

    rewind_button->setEnabled(false);

    connect ( open_button, &QAbstractButton::clicked, this, &AnalysisConditionDialog::slotTextFileOpen );
    connect ( save_button, &QAbstractButton::clicked, this, &AnalysisConditionDialog::slotTextFileSave );
    connect ( convert_button, &QAbstractButton::clicked, this, &AnalysisConditionDialog::slotTextFileConvert );

    connect ( upload_button, &QAbstractButton::clicked, this, &AnalysisConditionDialog::slotModelUpload );
    connect ( rewind_button, &QAbstractButton::clicked, this, &AnalysisConditionDialog::slotModelRewind );
}


void AnalysisConditionDialog::slotTextFileOpen(){
    QString filename =
        QFileDialog::getOpenFileName( this, u8"ファイルを開く", ".", "File(*.dat)" );
    if ( filename.isEmpty() ) return;

    QFile file(filename);
    if ( !file.open(QIODevice::ReadOnly) ) {
        Utility::messageFileOpenError(this);
        return;
    }

    QTextStream in(&file);
    tabWidget->read(in);
    setDataModified(false);
}
void AnalysisConditionDialog::slotTextFileSave(){
    QString filename = QFileDialog::getSaveFileName(this, u8"ファイル保存", ".", "File(*.dat)");
    if (filename.isEmpty()) return;
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return;

    if (this->tabWidget->hasFormatError()){
        Utility::messageFormatError(this);
    }

    QTextStream outfile(&file);
    outfile << this->tabWidget->write();
    file.close();
}

void AnalysisConditionDialog::slotTextFileConvert(){
    slotModelUpload();
    if (UnifiedProject::getInstance()->checkAnalysisCondition()){
        Utility::showErrorMessage(this, u8"解析条件のフォーマットがおかしい");
        return;
    }

    QFile file("D:\\Users\\03551\\Desktop\\panalysis_slover.dat");
    if (!file.open(QIODevice::WriteOnly))
        return;

    QTextStream outfile(&file);
    QString errorMessage = UnifiedProject::getInstance()->parseAnalysisCondition(outfile);
    if (errorMessage != QString{})
        Utility::showErrorMessage(this, errorMessage);
    file.close();
}

void AnalysisConditionDialog::setDataModified(bool onoff){
    if (onoff) {
        upload_button->setEnabled(true);
        rewind_button->setEnabled(true);
        this->setWindowTitle(u8"解析条件 *** 編集中 ***");
        this->setWindowIcon(QIcon(":/icons/c2_green.png"));
        emit statusChanged(CBSTATUS::CB_ACTIVE);
    } else {
        upload_button->setEnabled(false);
        tabWidget->compare(QPair<QString, QString>(AnalysisConditionReader::DefaultGeneralAnalysisConditionText(), QString{}))
                ? emit statusChanged(CBSTATUS::CB_NORMAL) : emit statusChanged(CBSTATUS::CB_DEFINE);

    }
}

void AnalysisConditionDialog::slotModelUpload(){
    tabWidget->modelUpload();
    setDataModified(false);
    rewind_button->setEnabled(false);
}

void AnalysisConditionDialog::slotModelRewind(){
    tabWidget->modelRewind();
    setDataModified(false);
    rewind_button->setEnabled(false);
}

void AnalysisConditionDialog::slotDataChanged(){
    bool isChanged = !this->tabWidget->compare(UnifiedProject::getInstance()->getAnalysisCondition());
    setDataModified(isChanged);
}

void AnalysisConditionDialog::clearData()
{
    tabWidget->clearData();
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void AnalysisConditionDialog::readBinaryFile(QDataStream& infile, const AppVersion &ver){
    QString str1;
    infile >> str1;
    this->tabWidget->read(str1);
    setDataModified(false);
}

void AnalysisConditionDialog::readTextFile(QTextStream &infile, const AppVersion &ver)
{
    this->tabWidget->read(infile);
    setDataModified(false);
}

void AnalysisConditionDialog::writeBinaryFile(QDataStream &outfile){
    outfile << this->tabWidget->write();
}

void AnalysisConditionDialog::writeTextFile(QTextStream &outfile)
{
    outfile << this->tabWidget->write();
}

} // namespace post3dapp
