#include "custom_print_dialog.h"

#include <typeinfo>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewWidget>
#include <QSizePolicy>
#include <QSpinBox>

#include "analysis2dsetting_dialog.h"
#include "calculationload2dsetting_dialog.h"
#include "custom_setting_dialog.h"
#include "define_analysis2dsettings.h"
#include "define_draw_figure.h"
#include "draw_figure.h"
#include "elevationCI_grscene.h"
#include "elevationCL_grscene.h"
#include "elevationOT_grscene.h"
#include "elevationUI_grscene.h"
#include "fixed_data.h"
#include "input2dsetting_dialog.h"
#include "planCI_grscene.h"
#include "planCL_grscene.h"
#include "planOT_grscene.h"
#include "planUI_grscene.h"
#include "setting_widget_factory.h"
#include "unified_settings.h"

namespace post3dapp{

CustomPrintDialog::CustomPrintDialog(CustomDrawGrScene *scene):baseScene(scene)
{
    createLayout();
    previewButton->click();
    policy = FIGURE_SIZE_POLICY{};
    setting = CustomPrintDialogSetting{};
}
CustomDrawGrScene* CustomPrintDialog::CopyScene() const
{
    if(typeid(*baseScene)==typeid(ElevationOutputGrScene)){
        auto scene = DrawFigure::createOutScene(baseScene->getFigureTypeProperty(), UnifiedSettings::getInstance()->getOutput2DViewSettings());
        scene->getViewSettings()->SetValueTextSize(setting.valueTextSize);
        scene->SetUseScaledFont(setting.use_scaled_font);
        return scene;
    }else if(typeid(*baseScene)==typeid(PlanOutputGrScene)){
        auto scene = DrawFigure::createOutScene(baseScene->getFigureTypeProperty(), UnifiedSettings::getInstance()->getOutput2DViewSettings());
        scene->getViewSettings()->SetValueTextSize(setting.valueTextSize);
        scene->SetUseScaledFont(setting.use_scaled_font);
        return scene;
    }else if(typeid(*baseScene)==typeid(ElevationInterfaceGrScene)){
        auto scene = DrawFigure::createInputScene(baseScene->getFigureTypeProperty(), UnifiedSettings::getInstance()->getInput2DViewSettings());
        scene->SetUseScaledFont(setting.use_scaled_font);
        return scene;
    }else if(typeid(*baseScene)==typeid(PlanInterfaceGrScene)){
        auto scene = DrawFigure::createInputScene(baseScene->getFigureTypeProperty(), UnifiedSettings::getInstance()->getInput2DViewSettings());
        scene->SetUseScaledFont(setting.use_scaled_font);
        return scene;
    }else if(typeid(*baseScene)==typeid(ElevationCalcLoadGrScene)){
        auto scene = DrawFigure::createLoadScene(baseScene->getFigureTypeProperty(), UnifiedSettings::getInstance()->getCalc2DViewSettings());
        scene->SetUseScaledFont(setting.use_scaled_font);
        return scene;
    }else if(typeid(*baseScene)==typeid(PlanCalcLoadGrScene)){
        auto scene = DrawFigure::createLoadScene(baseScene->getFigureTypeProperty(), UnifiedSettings::getInstance()->getCalc2DViewSettings());
        scene->SetUseScaledFont(setting.use_scaled_font);
        return scene;
    }else if(typeid(*baseScene)==typeid(ElevationCopyInterfaceGrScene)){
        auto scene = DrawFigure::createCopyScene(UnifiedFixedData::getInstance()->getInputData(), baseScene->getFigureTypeProperty(), UnifiedSettings::getInstance()->getInput2DViewSettings());
        scene->SetUseScaledFont(setting.use_scaled_font);
        return scene;
    }else if(typeid(*baseScene)==typeid(PlanCopyInterfaceGrScene)){
        auto scene = DrawFigure::createCopyScene(UnifiedFixedData::getInstance()->getInputData(), baseScene->getFigureTypeProperty(), UnifiedSettings::getInstance()->getInput2DViewSettings());
        scene->SetUseScaledFont(setting.use_scaled_font);
        return scene;
    }else{
        qDebug()<<"unknown scene type";
        throw;
    }


}
void CustomPrintDialog::createLayout()
{
    qDebug()<<"create Layout";

    paperSize = new QComboBox;
    paperSize->setMinimumWidth(100);
    paperSize->insertItems(0, QStringList{u8"A4タテ", u8"A4ヨコ", u8"A3タテ", u8"A3ヨコ", u8"A2タテ", u8"A2ヨコ", u8"A1タテ", u8"A1ヨコ"});

    tate = new QSpinBox();
    tate->setMinimum(1);
    tate->setValue(1);
    yoko = new QSpinBox();
    yoko->setMinimum(1);
    yoko->setValue(1);

    settingButton = new QPushButton(u8"詳細設定");
    previewButton = new QPushButton(tr(u8"プレビュー"));
    printButton = new QPushButton(tr(u8"出力"));

    QHBoxLayout* firstLine = new QHBoxLayout;
    firstLine->addWidget(new QLabel(tr(u8"用紙サイズ")));
    firstLine->addWidget(paperSize);
    firstLine->addWidget(new QLabel(u8"タテ"));
    firstLine->addWidget(tate);
    firstLine->addWidget(new QLabel(u8"xヨコ"));
    firstLine->addWidget(yoko);
    firstLine->addStretch();
    firstLine->addWidget(settingButton);
    firstLine->addWidget(previewButton);
    firstLine->addWidget(printButton);

    autoScaleRadioButton = new QRadioButton(u8"オートスケール");
    autoScaleRadioButton->setChecked(true);
    fixScaleRadioButton = new QRadioButton(u8"縮尺指定");
    scale = new QSpinBox;
    scale->setMinimumWidth(100);
    scale->setMinimum(10);
    scale->setMaximum(2000);
    scale->setValue(200);

    QHBoxLayout* secondLine = new QHBoxLayout;
    secondLine->addWidget(autoScaleRadioButton);
    secondLine->addWidget(fixScaleRadioButton);
    secondLine->addWidget(new QLabel(u8" 1/"));
    secondLine->addWidget(scale);
    secondLine->addStretch();

    check_header = new QCheckBox(tr(u8"ヘッダーを表示する"));
    check_footer = new QCheckBox(tr(u8"フッターを表示する"));

    QHBoxLayout* sita = new QHBoxLayout;
    sita->addWidget(check_header);
    sita->addWidget(check_footer);
    sita->addStretch();

    calcedScale = new QLabel();
    tate_now = new QSpinBox;
    tate_now->setValue(1);
    tate_max = new QLabel(tr(u8" / 1"));
    yoko_now = new QSpinBox;
    yoko_now->setValue(1);
    yoko_max = new QLabel(tr(u8" / 1"));

    QHBoxLayout* status = new QHBoxLayout;
    status->addWidget(calcedScale);
    status->addStretch();
    status->addWidget(new QLabel(tr(u8"縦：")));
    status->addWidget(tate_now);
    status->addWidget(tate_max);
    status->addSpacing(5);
    status->addWidget(new QLabel(tr(u8"横：")));
    status->addWidget(yoko_now);
    status->addWidget(yoko_max);

    QVBoxLayout* vlay = new QVBoxLayout;
    vlay->addLayout(firstLine);
    vlay->addLayout(secondLine);
    vlay->addLayout(sita);
    vlay->addWidget(createPreviewWidget());
    vlay->addLayout(status);
    this->setLayout(vlay);

    connect(settingButton, &QAbstractButton::clicked, this, &CustomPrintDialog::settingButtonClicked);
}
QWidget* CustomPrintDialog::createPreviewWidget()
{
    printer = new QPrinter;
    printer->setFullPage(true);
    myPrintPreview = new QPrintPreviewWidget(printer);
    myPrintPreview->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    myPrintPreview->setMinimumSize(600,600);
    connect(myPrintPreview, &QPrintPreviewWidget::paintRequested,
                this, &CustomPrintDialog::previewClicked);
    connect(previewButton, &QAbstractButton::clicked,
                myPrintPreview, &QPrintPreviewWidget::updatePreview);
    connect(printButton, &QAbstractButton::clicked,
                this, &CustomPrintDialog::setPrinter);

//    connect(settingButton,SIGNAL(clicked()),this,SLOT(changeSettings()));

    connect(yoko_now,QOverload<int>::of(&QSpinBox::valueChanged),this,QOverload<int>::of(&CustomPrintDialog::pageToggleChanged));
    connect(tate_now,QOverload<int>::of(&QSpinBox::valueChanged),this,QOverload<int>::of(&CustomPrintDialog::pageToggleChanged));
    connect(myPrintPreview,&QPrintPreviewWidget::previewChanged,this,&CustomPrintDialog::scrolled);


    return myPrintPreview;

}
void CustomPrintDialog::setPrinter(){
    QPrintDialog printDialog(printer,this);
    if(printDialog.exec() == QDialog::Accepted){
        printer = printDialog.printer();
        printer->setPageMargins(0,0,0,0,QPrinter::Millimeter);
        myPrintPreview->print();
    }
}

void CustomPrintDialog::pageToggleChanged(int value)
{
    //トグルを動かしたらページ移動
    int tate = tate_now->value();
    int yoko = yoko_now->value();
    int yokomax = results.divide.yoko;
    myPrintPreview->setCurrentPage(yoko + yokomax*(tate-1));
}
void CustomPrintDialog::scrolled(){
    //スクロールしたら現在のページ数を表示。ループになるのでシグナルは切っておく
    int current = myPrintPreview->currentPage();
    int yokomax = results.divide.yoko;
    int t = (current-1) / yokomax + 1;
    int y = (current-1) % yokomax + 1;
    tate_now->blockSignals(true);
    tate_now->setValue(t);
    tate_now->blockSignals(false);

    yoko_now->blockSignals(true);
    yoko_now->setValue(y);
    yoko_now->blockSignals(false);
}

void CustomPrintDialog::previewClicked(QPrinter *newPrinter){
    //紙サイズと方向をセット、draw
    printer = newPrinter;
    switch(paperSize->currentIndex()){
    case 0:
        printer->setPageSize(QPageSize(QPageSize::A4));
        printer->setPageOrientation(QPageLayout::Portrait);
        break;
    case 1:
        printer->setPageSize(QPageSize(QPageSize::A4));
        printer->setPageOrientation(QPageLayout::Landscape);
        break;
    case 2:
        printer->setPageSize(QPageSize(QPageSize::A3));
        printer->setPageOrientation(QPageLayout::Portrait);
        break;
    case 3:
        printer->setPageSize(QPageSize(QPageSize::A3));
        printer->setPageOrientation(QPageLayout::Landscape);
        break;
    case 4:
        printer->setPageSize(QPageSize(QPageSize::A2));
        printer->setPageOrientation(QPageLayout::Portrait);
        break;
    case 5:
        printer->setPageSize(QPageSize(QPageSize::A2));
        printer->setPageOrientation(QPageLayout::Landscape);
        break;
    case 6:
        printer->setPageSize(QPageSize(QPageSize::A1));
        printer->setPageOrientation(QPageLayout::Portrait);
        break;
    case 7:
        printer->setPageSize(QPageSize(QPageSize::A1));
        printer->setPageOrientation(QPageLayout::Landscape);
        break;
    default:
        printer->setPageSize(QPageSize(QPageSize::A4));
        printer->setPageOrientation(QPageLayout::Portrait);
        break;
    }
    drawPreview();
}

void CustomPrintDialog::drawPreview()
{
    qDebug()<<"drawPreview";
    QPainter* painter = new QPainter;
    painter->begin(printer);
    qDebug()<<"painter:"<<painter;

    policy.drawHeader = check_header->isChecked();
    policy.drawFooter = check_footer->isChecked();
    policy.initial_paper = PAPER(printer->pageSize(), printer->pageLayout().orientation());
    policy.initial_divide = PAPERDIVIDE(tate->value(), yoko->value());
    if (autoScaleRadioButton->isChecked()){
        policy.policy = DividePolicy::AutoScale;
    }else{
        policy.initial_scale = 1.0 / scale->value();
        policy.policy = DividePolicy::MANUAL;
    }

    // ここで描画用のSceneを作成し、描画設定を割り当てる
    auto copyScene = CopyScene();
    copyScene->resetItems();

    auto res = DrawFigure::drawForPrintDialog(painter,copyScene, policy);
    results = res.first;
    tate_now->setMaximum(results.divide.tate);
    tate_now->setMinimum(1);
    yoko_now->setMaximum(results.divide.yoko);
    yoko_now->setMinimum(1);
    tate_max->setText(u8" / " + QString::number(results.divide.tate));
    yoko_max->setText(u8" / " + QString::number(results.divide.yoko));
    calcedScale->setText(u8"描画縮尺　：　1 / "+ QString::number(1 / results.scale,'f',1));

    painter->end();
    return;

}

void CustomPrintDialog::settingButtonClicked(){
    CustomPrintDialogSettingDialog* dialog = new CustomPrintDialogSettingDialog(policy, setting);
    if (dialog->exec()){
        dialog->get_policy(&policy, &setting);
    }
}

}
