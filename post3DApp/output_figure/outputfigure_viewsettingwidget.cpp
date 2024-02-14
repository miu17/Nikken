#include "outputfigure_viewsettingwidget.h"

#include <QLayout>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QDialog>

#include "app_version.h"
#include "define_calculationload2dsettings.h"
#include "define_draw_figure.h"
#include "outputfigure_customsettingdialog.h"
#include "outputfigure_customsettingtabwidget.h"
#include "outputfigure_defaultsettingtabwidget.h"
#include "outputfigure_displaysettingtabwidget.h"
#include "unified_settings.h"


namespace post3dapp{
OutputFigureViewSettingWidget::OutputFigureViewSettingWidget(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* hlay1 = new QHBoxLayout();
    defaultPaperRadioButton = new QRadioButton("default");
    defaultPaperRadioButton->setChecked(true);
    defaultPaperLineEdit = new QLineEdit("400");
    hlay1->addSpacing(50);
    hlay1->addWidget(defaultPaperRadioButton);
    hlay1->addWidget(new QLabel("scale"));
    hlay1->addWidget(defaultPaperLineEdit);

    QHBoxLayout* hlay2 = new QHBoxLayout();
    a4RadioButton = new QRadioButton("A4 Portrait");
    hlay2->addSpacing(50);
    hlay2->addWidget(a4RadioButton);

    QHBoxLayout* hlay3 = new QHBoxLayout();
    scaleRadioButton = new QRadioButton("Other");
    scaleLineEdit = new QLineEdit("400");
    hlay3->addSpacing(50);
    hlay3->addWidget(scaleRadioButton);
    hlay3->addWidget(new QLabel("scale"));
    hlay3->addWidget(scaleLineEdit);

    QVBoxLayout* vlay = new QVBoxLayout();
    vlay->addWidget(new QLabel("paper setting"));
    vlay->addLayout(hlay1);
    vlay->addLayout(hlay2);
    vlay->addLayout(hlay3);

    QGroupBox* groupBox1 = new QGroupBox();
    groupBox1->setLayout(vlay);

    QHBoxLayout* hlay4 = new QHBoxLayout();
    defaultRadioButton = new QRadioButton("default");
    defaultRadioButton->setChecked(true);
    QPushButton* defaultPushButton = new QPushButton();
    connect(defaultPushButton, &QPushButton::clicked, this, &OutputFigureViewSettingWidget::slotDefaultSettingShow);
    hlay4->addSpacing(50);
    hlay4->addWidget(defaultRadioButton);
    hlay4->addWidget(defaultPushButton);

    QHBoxLayout* hlay5 = new QHBoxLayout();
    displayRadioButton = new QRadioButton("display");
    QPushButton* displayPushButton = new QPushButton();
    connect(displayPushButton, &QPushButton::clicked, this, &OutputFigureViewSettingWidget::slotDisplaySettingShow);
    hlay5->addSpacing(50);
    hlay5->addWidget(displayRadioButton);
    hlay5->addWidget(displayPushButton);

    QHBoxLayout* hlay6 = new QHBoxLayout();
    customRadioButton = new QRadioButton("custom");
    QPushButton* customPushuButton = new QPushButton();
    connect(customPushuButton, &QPushButton::clicked, this, &OutputFigureViewSettingWidget::slotCustomSettingShow);
    hlay6->addSpacing(50);
    hlay6->addWidget(customRadioButton);
    hlay6->addWidget(customPushuButton);

    QVBoxLayout* vlay2 = new QVBoxLayout();
    vlay2->addWidget(new QLabel("view setting"));
    vlay2->addLayout(hlay4);
    vlay2->addLayout(hlay5);
    vlay2->addLayout(hlay6);
    QGroupBox* groupBox2 = new QGroupBox();
    groupBox2->setLayout(vlay2);

    customSettingDialog = new OutputFigureCustomSettingDialog();
    customSettingDialog->setVisible(false);

    QVBoxLayout* vlay3 = new QVBoxLayout();
    vlay3->addWidget(groupBox1);
    vlay3->addWidget(groupBox2);
    vlay3->addStretch();


    this->setLayout(vlay3);
}

void OutputFigureViewSettingWidget::slotDefaultSettingShow(){
    QDialog dialog(this);
    OutputFigureDefalutSettingWidget *wt = new OutputFigureDefalutSettingWidget();
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(wt);
    dialog.setLayout(vlay);
    if (dialog.exec() == QDialog::Accepted){
    }
}
void OutputFigureViewSettingWidget::slotDisplaySettingShow(){
    QDialog dialog(this);
    OutputFigureDisplaySettingTabWidget *wt = new OutputFigureDisplaySettingTabWidget();
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(wt);
    dialog.setLayout(vlay);
    if (dialog.exec() == QDialog::Accepted){
    }
}
void OutputFigureViewSettingWidget::slotCustomSettingShow(){
    // copy
    if (customSettingDialog->exec() == QDialog::Accepted){
    }
    else{
        // reset
    }
}

void OutputFigureViewSettingWidget::writeBinaryFile(QDataStream &outfile) const{
    int paper;
    if (defaultPaperRadioButton->isChecked())
        paper = 0;
    else if (a4RadioButton->isChecked())
        paper = 1;
    else
        paper = 2;

    outfile << paper;
    outfile << defaultPaperLineEdit->text();
    outfile << scaleLineEdit->text();

    int setting;
    if (defaultRadioButton->isChecked())
        setting = 0;
    else if (displayRadioButton->isChecked())
        setting = 1;
    else
        setting = 2;
    outfile << setting;

    customSettingDialog->writeBinaryFile(outfile);
}
void OutputFigureViewSettingWidget::readBinaryFile(QDataStream &infile, const AppVersion &version){
    int paper;
    infile >> paper;
    if (paper == 0){
        defaultPaperRadioButton->setChecked(true);
    }else if (paper == 1){
        a4RadioButton->setChecked(true);
    }else{
        scaleRadioButton->setChecked(true);
    }

    QString paperText;
    infile >> paperText;
    defaultPaperLineEdit->setText(paperText);
    QString scale;
    infile >> scale;
    scaleLineEdit->setText(scale);

    int setting;
    infile >> setting;
    if (setting == 0)
        defaultRadioButton->setChecked(true);
    else if (setting == 1)
        displayRadioButton->setChecked(true);
    else
        customRadioButton->setChecked(true);

    customSettingDialog->readBinaryFile(infile, version);
}

OutputPaperSetting OutputFigureViewSettingWidget::GetPaperSetting() const
{
    Input2DViewSettings input = Input2DViewSettings::getDefault();
    //Analysis2DViewSettings output;
    CalculationLoad2DViewSettings load = CalculationLoad2DViewSettings::getDefault();
    FIGURE_PROPERTY property;
    FIGURE_SIZE_POLICY policy;
    if (defaultPaperRadioButton->isChecked()){
        // A4タテに収まるようなオートスケール。オートスケールが小さすぎる場合はA3ヨコでオートスケール。
        // A3ヨコのオートスケールが小さすぎる場合はA3ヨコに収まるようにオートスケールだが警告。
        // 伏図と軸組図で縮尺が変更になる。
        policy.policy = DividePolicy::A4A3Auto;
        policy.initial_divide = PAPERDIVIDE(1,1);
        policy.initial_scale = 1/ 200.0;
        policy.initial_paper = PAPER(QPagedPaintDevice::A4, QPageLayout::Portrait);
        policy.upperBound_divide = PAPERDIVIDE(4,4);
        policy.upperBound_scale = 1 / 200.0;
        policy.upperBound_paperSize = QList<QSize>{PAPER::getPaperSize(QPagedPaintDevice::A3, QPageLayout::Landscape)};
    }else if (a4RadioButton->isChecked()){
        // 必ずA4タテ。
        // A4タテのオートスケールが小さすぎる場合は警告。
        // 伏図と軸組図で縮尺が変更になる。
        policy.policy = DividePolicy::PAPERSIZE_DIVIDE_SCALE;
        policy.initial_divide = PAPERDIVIDE(1,1);
        policy.initial_paper = PAPER(QPagedPaintDevice::A4, QPageLayout::Portrait);
        policy.upperBound_scale = 1 / 800.0;
    }else{
        // 指定の縮尺でA4タテ。A4タテに収まらない場合はA3ヨコ。A3ヨコに収まらない場合は
        // 伏図と軸組図で縮尺同じ。
        policy.policy = DividePolicy::A4A3Scale;

    }
    return OutputPaperSetting(
                UnifiedSettings::getInstance()->getInput2DViewSettings(),
                UnifiedSettings::getInstance()->getInput3DViewSettings(),
                UnifiedSettings::getInstance()->getCalc2DViewSettings(),
                UnifiedSettings::getInstance()->getCalc3DViewSettings(),
                UnifiedSettings::getInstance()->getOutput2DViewSettings(),
                UnifiedSettings::getInstance()->getOutput3DViewSettings(),
                policy);
}


} // namespace post3dapp
