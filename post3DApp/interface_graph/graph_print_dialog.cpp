#include "graph_print_dialog.h"

#include <typeinfo>
#include <QtWidgets>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewWidget>
#include <QSizePolicy>
#include <QSpinBox>

#include "analysis2dsetting_dialog.h"
#include "calculationload2dsetting_dialog.h"
#include "custom_setting_dialog.h"
#include "define_draw_figure.h"
#include "draw_figure.h"
#include "elevationCI_grscene.h"
#include "elevationCL_grscene.h"
#include "elevationOT_grscene.h"
#include "elevationUI_grscene.h"
#include "fixed_data.h"
#include "graph_scene.h"
#include "input2dsetting_dialog.h"
#include "planCI_grscene.h"
#include "planCL_grscene.h"
#include "planOT_grscene.h"
#include "planUI_grscene.h"
#include "setting_widget_factory.h"
#include "unified_settings.h"

namespace post3dapp{

GraphPrintDialog::GraphPrintDialog(GraphScene *scene, QImage image, QDialog *parent):QDialog(parent),baseScene(scene),myImage(image)
{

    createLayout();
    //sceneのgraphオプション等をコピーして、それを基にNewして描画する
    previewButton->click();
}
GraphScene* GraphPrintDialog::CopySceneProperty()
{
    GraphScene* copy = new GraphScene(printer->logicalDpiX(),false);
    type =baseScene->graphTypeOptions();
//    size = baseScene->graphSizeOptions();
    prop = baseScene->graphProperty();
//    graph_per_row = baseScene->graphsPerRow();

    //graphsPerRow、
    return copy;

}
void GraphPrintDialog::createLayout()
{
    qDebug()<<"create Layout";

    paperSize = new QComboBox;
    paperSize->setMinimumWidth(100);
    paperSize->insertItems(0, QStringList{u8"A4タテ", u8"A4ヨコ", u8"A3タテ", u8"A3ヨコ", u8"A2タテ", u8"A2ヨコ", u8"A1タテ", u8"A1ヨコ"});

    previewButton = new QPushButton(tr(u8"プレビュー"));
    printButton = new QPushButton(tr(u8"出力"));

    QHBoxLayout* firstLine = new QHBoxLayout;
    firstLine->addWidget(new QLabel(tr(u8"用紙サイズ")));
    firstLine->addWidget(paperSize);
    firstLine->addStretch();
    firstLine->addWidget(previewButton);
    firstLine->addWidget(printButton);

    QHBoxLayout* secondLine = new QHBoxLayout;
//    vAlign = new QComboBox;
//    vAlign->insertItems(0, QStringList{u8"上寄せ", u8"中央", u8"下寄せ"});
//    vAlign->setCurrentIndex(1);
//    hAlign = new QComboBox;
//    hAlign->insertItems(0, QStringList{u8"左寄せ", u8"中央", u8"右寄せ"});
//    hAlign->setCurrentIndex(1);
    QLabel* label_margin = new QLabel(u8"マージン");
    margin = new QLineEdit();
    margin->setText(u8"2");
//    QLabel* label_padding = new QLabel(u8"パディング");
//    padding = new QLineEdit();
//    padding->setText(u8"2");
    QLabel* label_ngraphPerRow = new QLabel(u8"グラフ数/行");
    nGraphPerRow=new QSpinBox();
    nGraphPerRow->setValue(baseScene->graphsPerRow());
//    aspectFix = new QCheckBox(u8"アス比固定");
//    aspectFix->setChecked(true);
//    aspectRatio=new QLineEdit();
//    QLabel* label_aspect = new QLabel(u8"アスペクト比");
//    aspectRatio->setText(u8"1.00");
//    secondLine->addWidget(hAlign);
//    secondLine->addWidget(vAlign);
//    secondLine->addStretch();
    secondLine->addWidget(label_margin);
    secondLine->addWidget(margin);
    secondLine->addStretch();
//    secondLine->addWidget(label_padding);
//    secondLine->addWidget(padding);
//    secondLine->addStretch();
    secondLine->addWidget(label_ngraphPerRow);
    secondLine->addWidget(nGraphPerRow);
//    secondLine->addStretch();
//    secondLine->addWidget(aspectFix);
//    secondLine->addWidget(label_aspect);
//    secondLine->addWidget(aspectRatio);

//    QHBoxLayout* thirdLine = new QHBoxLayout;
//    QLabel* label_minmax = new QLabel(u8"xmin/xmax/ymin/ymax");
//    scaleAuto= new QCheckBox(u8"minmax自動");
//    scaleAuto->setChecked(true);
//    xMax =new QLineEdit();
//    xMin=new QLineEdit();
//    yMax=new QLineEdit();
//    yMin=new QLineEdit();
//    logScaleX= new QCheckBox(u8"X対数スケール");
//    logScaleY= new QCheckBox(u8"Y対数スケール");
//    thirdLine->addWidget(scaleAuto);
//    thirdLine->addWidget(label_minmax);
//    thirdLine->addWidget(xMin);
//    thirdLine->addWidget(xMax);
//    thirdLine->addWidget(yMin);
//    thirdLine->addWidget(yMax);
//    thirdLine->addStretch();
//    thirdLine->addWidget(logScaleX);
//    thirdLine->addWidget(logScaleY);


    check_header = new QCheckBox(tr(u8"ヘッダーを表示する"));
    check_footer = new QCheckBox(tr(u8"フッターを表示する"));
    drawWaku= new QCheckBox(u8"枠");
    drawLegend= new QCheckBox(u8"凡例");
    check_header->setChecked(true);
    check_footer->setChecked(true);
    drawWaku->setChecked(true);
    drawLegend->setChecked(true);

    QHBoxLayout* sita = new QHBoxLayout;
    sita->addWidget(check_header);
    sita->addWidget(check_footer);
    sita->addWidget(drawWaku);
    sita->addWidget(drawLegend);
    sita->addStretch();


    QHBoxLayout* status = new QHBoxLayout;
    status->addStretch();

    QVBoxLayout* vlay = new QVBoxLayout;
    vlay->addLayout(firstLine);
    vlay->addLayout(secondLine);
//    vlay->addLayout(thirdLine);
    vlay->addLayout(sita);
    vlay->addWidget(createPreviewWidget());
    vlay->addLayout(status);
    this->setLayout(vlay);

}
QWidget* GraphPrintDialog::createPreviewWidget()
{
    printer = new QPrinter;
    printer->setFullPage(true);
    myPrintPreview = new QPrintPreviewWidget(printer);
    myPrintPreview->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    myPrintPreview->setMinimumSize(600,600);
    connect(myPrintPreview, &QPrintPreviewWidget::paintRequested,
            this, &GraphPrintDialog::previewClicked);
    connect(previewButton, &QAbstractButton::clicked,
            myPrintPreview, &QPrintPreviewWidget::updatePreview);
    connect(printButton, &QAbstractButton::clicked,
            this, &GraphPrintDialog::setPrinter);




    return myPrintPreview;

}
void GraphPrintDialog::setPrinter(){
    QPrintDialog printDialog(printer,this);
    if(printDialog.exec() == QDialog::Accepted){
        printer = printDialog.printer();
        printer->setPageMargins(0,0,0,0,QPrinter::Millimeter);
        myPrintPreview->print();
    }
}


void GraphPrintDialog::previewClicked(QPrinter *newPrinter){
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

void GraphPrintDialog::drawPreview()
{
    qDebug()<<"drawPreview";
    QPainter* painter = new QPainter;
    painter->begin(printer);
    qDebug()<<"painter:"<<painter;

    auto copyScene = CopySceneProperty();
    prop.drawHeader = check_header->isChecked();
    prop.drawFooter = check_footer->isChecked();
    prop.drawWaku = drawWaku->isChecked();
    prop.unitPaper = PAPER(printer->pageSize(), printer->pageLayout().orientation());
//prop.sceneTitle = u8"タイトル：：ダイアログから印刷";
prop.drawLegend = drawLegend->isChecked();



    prop.paperMargin_left=margin->text().toDouble();
    prop.paperMargin_top=margin->text().toDouble();
    prop.paperMargin_right=margin->text().toDouble();
    prop.paperMargin_bottom=margin->text().toDouble();
    graph_per_row=nGraphPerRow->value();
    // ここで描画用のSceneを作成し、描画設定を割り当てる
    if(myImage != QImage()){
        prop.drawImage=true;
        copyScene->setImage(myImage);
    }

    copyScene->setValues(type,prop,graph_per_row,
                         UnifiedSettings::getInstance()->getOutput2DViewSettings());
    copyScene->drawForeground(painter,QRect());



    painter->end();
    return;

}


}
