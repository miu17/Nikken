#include "nmint_dialog.h"

#include "fixed_data.h"
#include "graph_3d_view.h"
#include "graph_factory.h"
#include "graph_print_dialog.h"
#include "graph_view.h"
#include "unified_settings.h"
#include "unified_output.h"

namespace post3dapp{

NmintDialog::NmintDialog(QWidget* parent) : QDialog(parent), setting(Analysis2DViewSettings(UnifiedSettings::getInstance()->getOutput2DViewSettings()))
{
    prop.drawFooter=false;
    prop.drawHeader=false;
    prop.drawWaku=false;
    prop.legendType=OUTFIGTYPE::NMINT;
    syncSetting = true;

    createButtons();
    createLayout();

    updateGraph();
    update3dGraph();
}

void NmintDialog::createButtons(){
    auto chck = UnifiedFixedData::getInstance()->getPstnData()->getNbldChckData();
    outCaseNameComboBox = new QComboBox(this);
    outCaseNameComboBox->addItems(chck.namech);
    outCaseNameComboBox->setMinimumWidth(200);
    connect(outCaseNameComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NmintDialog::updateGraph);
    connect(outCaseNameComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NmintDialog::update3dGraph);

    outICJComboBox = new QComboBox(this);
    outICJComboBox->setMinimumWidth(200);
//    outICJComboBox->addItem( u8"(ICJ)" );
    outICJComboBox->addItem( u8"I端" );
    outICJComboBox->addItem( u8"中央" );
    outICJComboBox->addItem( u8"J端" );
    connect(outICJComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NmintDialog::updateGraph);
    connect(outICJComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NmintDialog::update3dGraph);
//    outICJComboBox->setCurrentIndex(1);

    printButton = new QToolButton(this);
    printButton->setAutoRaise(true);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));
    connect(printButton, &QAbstractButton::clicked,
            this, &NmintDialog::slotClickedPrintButton);

    fitButton = new QToolButton(this);
    fitButton->setAutoRaise(true);
    fitButton->setIcon(QIcon(QPixmap(":/icons/fit.png")));
    fitButton->setFixedSize(30, 30);
    fitButton->setIconSize(QSize(25, 25));
    connect(fitButton, &QAbstractButton::clicked,
            this, &NmintDialog::updateGraph);
    connect(fitButton, &QAbstractButton::clicked,
            this, &NmintDialog::update3dGraph);

}
void NmintDialog::slotClickedPrintButton(){
    //印刷向けに解像度が足りないため、リサイズ
    graph3dView->resize(graph3dView->width()*4,graph3dView->height()*4);
    //確実にフレームバッファを更新するため便宜的にwaitを挟んで2回再描画してから保存
    graph3dView->update();
    QEventLoop loop;
    QTimer::singleShot(10,&loop,SLOT(quit()));//10ms
    loop.exec();
    graph3dView->update();
    QTimer::singleShot(10,&loop,SLOT(quit()));
    loop.exec();

    QImage image=graph3dView->grabFramebuffer();
    GraphPrintDialog* dialog = new GraphPrintDialog(graphView->getScene(),image);
    graph3dView->resize(graph3dView->width()/4,graph3dView->height()/4);
    graph3dView->update();

    if(dialog->exec()){

    }
    return;
}
void NmintDialog::createLayout(){
    opt.type=OUTFIGTYPE::NMINT;
    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(outCaseNameComboBox);
    hlay1->addWidget(outICJComboBox);
    hlay1->addStretch();
    hlay1->addWidget(fitButton, 0);
    hlay1->addWidget(printButton, 0);
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay1, 0);
    //部材番号と検討ケース名（VLとか）
    graphView = new GraphView(opt.type,syncSetting,this);
    graphView->setSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding);
    graphView->setMinimumSize(QSize(450,150));
    graphView->resize(900,300);
    my3dGraphScene = new Graph3dScene();
    graph3dView=new Graph3dView(my3dGraphScene,this);
    QSplitter* sp=new QSplitter;
    sp->addWidget(graphView);
    sp->addWidget(graph3dView);
    vlay->addWidget(sp);
    setLayout(vlay);

}


void NmintDialog::updateGraph()
{
//    return;
    opt.icj=static_cast<GRAPH_SUBTYPE_PROPERTY::ICJ>(qMax(0,outICJComboBox->currentIndex()-1));
    opt.caseName=outCaseNameComboBox->currentText();

    this->setWindowTitle(GraphFactory::makeGraphTitle(opt));
    prop.sceneTitle=GraphFactory::makeGraphTitle(opt);
    prop.unitPaper = PAPER(graphView->frameRect().size()/this->logicalDpiX()*DrawFigure::inchToMm);//pix -> mm;
    prop.paperMargin_bottom=4;
    prop.paperMargin_top=4;
    prop.paperMargin_left=4;
    prop.paperMargin_right=4;
    QList<GRAPH_SUBTYPE_PROPERTY> opts;
    for(int i=0;i<3;i++){
        GRAPH_SUBTYPE_PROPERTY optcopy(opt);
        optcopy.nm_type=static_cast<GRAPH_SUBTYPE_PROPERTY::NMTYPE>(i);
        opts<<optcopy;
    }
    graphView->resetSceneValue(opts,prop,3,setting);

    return;
}
void NmintDialog::update3dGraph()
{
//    return;

    opt.icj=static_cast<GRAPH_SUBTYPE_PROPERTY::ICJ>(qMax(0,outICJComboBox->currentIndex()-1));
    GRAPH_SUBTYPE_PROPERTY optcopy(opt);
    optcopy.caseName=outCaseNameComboBox->currentText();
    GraphFactory::makeNmint3dGraph_CHCK(*static_cast<AnalysisGraphNmint2DViewSetting*>(setting.getSetting(OUTFIGTYPE::NMINT)),my3dGraphScene,optcopy);
    graph3dView->camera()->fitBoundingBox(my3dGraphScene->getMinVec(),
                                          my3dGraphScene->getMaxVec());

    graph3dView->update();

}
}
