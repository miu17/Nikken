#include "custom_print_3d_dialog.h"
namespace post3dapp{

CustomPrint3dDialog::CustomPrint3dDialog(QGLViewer *glViewer, QWidget *parent)
    :QDialog(parent),viewer(glViewer)
{
    createLayout();
}

void CustomPrint3dDialog::previewClicked(QPrinter *newPrinter)
{
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

void CustomPrint3dDialog::settingButtonClicked()
{

}

void CustomPrint3dDialog::setPrinter()
{
    QPrintDialog printDialog(printer,this);
    if(printDialog.exec() == QDialog::Accepted){
        printer = printDialog.printer();
        printer->setPageMargins(0,0,0,0,QPrinter::Millimeter);
        myPrintPreview->print();
    }
}

void CustomPrint3dDialog::createLayout()
{
    paperSize = new QComboBox;
    paperSize->setMinimumWidth(100);
    paperSize->insertItems(0, QStringList{u8"A4タテ", u8"A4ヨコ", u8"A3タテ", u8"A3ヨコ", u8"A2タテ", u8"A2ヨコ", u8"A1タテ", u8"A1ヨコ"});

    settingButton = new QPushButton(u8"詳細設定");
    previewButton = new QPushButton(tr(u8"プレビュー"));
    printButton = new QPushButton(tr(u8"出力"));

    QHBoxLayout* firstLine = new QHBoxLayout;
    firstLine->addWidget(new QLabel(tr(u8"用紙サイズ")));
    firstLine->addWidget(paperSize);
    firstLine->addStretch();
    firstLine->addWidget(settingButton);
    firstLine->addWidget(previewButton);
    firstLine->addWidget(printButton);

    check_header = new QCheckBox(tr(u8"ヘッダーを表示する"));
    check_footer = new QCheckBox(tr(u8"フッターを表示する"));

    QHBoxLayout* sita = new QHBoxLayout;
    sita->addWidget(check_header);
    sita->addWidget(check_footer);
    sita->addStretch();

    QVBoxLayout* vlay = new QVBoxLayout;
    vlay->addLayout(firstLine);
    vlay->addLayout(sita);
    vlay->addWidget(createPreviewWidget());
    this->setLayout(vlay);

    connect(settingButton, &QAbstractButton::clicked, this, &CustomPrint3dDialog::settingButtonClicked);
}

QWidget *CustomPrint3dDialog::createPreviewWidget()
{
    printer = new QPrinter;
    printer->setFullPage(true);
    myPrintPreview = new QPrintPreviewWidget(printer);
    myPrintPreview->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);

    myPrintPreview->setMinimumSize(600,600);
    connect(myPrintPreview, &QPrintPreviewWidget::paintRequested,
                this, &CustomPrint3dDialog::previewClicked);
    connect(previewButton, &QAbstractButton::clicked,
                myPrintPreview, &QPrintPreviewWidget::updatePreview);
    connect(printButton, &QAbstractButton::clicked,
                this, &CustomPrint3dDialog::setPrinter);

    return myPrintPreview;
}

void CustomPrint3dDialog::drawPreview()
{
    qDebug()<<"drawPreview";
    QPainter* painter = new QPainter;
    painter->begin(printer);
    qDebug()<<"painter:"<<painter;

    FIGURE_SIZE_POLICY policy;
    policy.initial_paper = PAPER(printer->pageSize(), printer->pageLayout().orientation());
    policy.drawHeader = check_header->isChecked();
    policy.drawFooter = check_footer->isChecked();

    FIGURE_SIZE_RESULT result(policy);
    FIGURE_PROPERTY prop(policy);
    prop.divide.tate = 1;
    prop.divide.yoko = 1;
    FIGURE_TYPE_PROPERTY type;

    type.filename = "filename";
    drawOnePage(viewer, painter, result, prop, type);

    painter->end();
    return;
}

void CustomPrint3dDialog::drawOnePage(QGLViewer *viewer, QPainter *painter, const FIGURE_SIZE_RESULT &size, const FIGURE_PROPERTY &prop, const FIGURE_TYPE_PROPERTY& type)
{
    painter->save();
    CustomDrawGrScene::exportRenderPaperItems(painter, 1, 1, size, prop, type);

    // TODO partdrawの凡例描画が出てこない
    // ここで描画用のSceneを作成し、描画設定を割り当てる
    QImage previewPixmap = viewer->grabFramebuffer();
    auto target = size.getInnerPixcelRect(prop, painter->device()->logicalDpiX());
    qreal widthRatio = target.width() / previewPixmap.width();
    qreal heightRatio = target.height() / previewPixmap.height();
    QRectF targetRect;
    if (widthRatio < heightRatio){
        // widthをめいっぱい
        targetRect = QRectF(target.topLeft(), QSizeF(target.width(), previewPixmap.height() * widthRatio));
    }else{
        // heightをめいっぱい
        targetRect = QRectF(target.topLeft(), QSizeF(previewPixmap.width() * heightRatio, target.height()));
    }
    painter->drawImage(targetRect, previewPixmap, previewPixmap.rect());



    painter->restore();
}



} // namespace post3dapp
