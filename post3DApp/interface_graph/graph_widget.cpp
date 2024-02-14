#include "graph_widget.h"

#include <QLineEdit>
#include <QLayout>

#include "fixed_data.h"
#include "graph_3d_scene.h"
#include "graph_3d_view.h"
#include "graph_factory.h"
#include "graph_print_dialog.h"
#include "graph_view.h"
#include "output_control.h"
#include "unified_analysisdata.h"
#include "unified_settings.h"

namespace post3dapp{

GraphWidget::GraphWidget(QWidget *parent)
    :InterfaceWidget(parent), setting(Analysis2DViewSettings(UnifiedSettings::getInstance()->getOutput2DViewSettings()))
{
//    size.hAlign=1;
//    size.vAlign=1;
    prop.drawFooter=false;
    prop.drawHeader=false;
    prop.drawWaku=false;

    createButtons();
    createLayout();
}

void GraphWidget::createButtons(){
    //シグナル繋ぐ
    //viewのnewでsceneもnewしている。そこにデータを流し込む。
    outTypeComboBox = new QComboBox(this);
    outTypeComboBox->setFixedWidth(400);
    outTypeComboBox->addItems( OutputControl::getInstance()->outputTypeStringList(OUTFIGCATEGORY::GRAPH) );

    connect(outTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GraphWidget::resetLayout);

    outRSTUComboBox = new QComboBox(this);
    QStringList qudslist = OutputControl::getInstance()->getOutfigStringList(LISTTYPE::QUDS);
    outRSTUComboBox->addItem( u8"(Case)" );
    outRSTUComboBox->addItems(qudslist);
    connect(outRSTUComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GraphWidget::updateGraph);

    outQUDSComboBox = new QComboBox(this);
    outQUDSComboBox->addItem( u8"(QUDS)" );
    outQUDSComboBox->addItem( u8"QU" );
    outQUDSComboBox->addItem( u8"DS" );
    connect(outQUDSComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GraphWidget::updateGraph);

    outFrameComboBox = new QComboBox(this);
    outFrameComboBox->addItem(u8"(Frame)");
    Q_FOREACH(auto frame, UnifiedFixedData::getInstance()->getAnalysisData()->getFramePointList()){
        outFrameComboBox->addItem(frame.frameName);
    }
    connect(outFrameComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GraphWidget::updateGraph);

    outFrameGroupComboBox = new QComboBox(this);
    outFrameGroupComboBox->addItem(u8"(Group)");
    Q_FOREACH(auto group, UnifiedFixedData::getInstance()->getAnalysisData()->getFrameGroupList()){
        outFrameGroupComboBox->addItem(group.groupName);
    }
    connect(outFrameGroupComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GraphWidget::updateGraph);

    outFloorComboBox = new QComboBox(this);
    outFloorComboBox->addItem(u8"(Floor)");
    Q_FOREACH(auto floor, UnifiedFixedData::getInstance()->getAnalysisData()->getFloorList()){
        outFloorComboBox->addItem(floor.floorName);
    }
    outFloorComboBox->removeItem(outFloorComboBox->count()-1);
    connect(outFloorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GraphWidget::updateGraph);



    printButton = new QToolButton(this);
    printButton->setAutoRaise(true);
    printButton->setIcon(QIcon(QPixmap(":/icons/print.png")));
    printButton->setFixedSize(30, 30);
    printButton->setIconSize(QSize(25, 25));
    connect(printButton, &QAbstractButton::clicked,
            this, &GraphWidget::slotClickedPrintButton);
    fitButton = new QToolButton(this);
    fitButton->setAutoRaise(true);
    fitButton->setIcon(QIcon(QPixmap(":/icons/fit.png")));
    fitButton->setFixedSize(30, 30);
    fitButton->setIconSize(QSize(25, 25));
    connect(fitButton, &QAbstractButton::clicked,
            this, &GraphWidget::updateGraph);

}
void GraphWidget::slotClickedPrintButton(){
    GraphPrintDialog* dialog = new GraphPrintDialog(graphView->getScene());
    if(dialog->exec()){

    }
    return;
}
void GraphWidget::createLayout(){
    QHBoxLayout *hlay1 = new QHBoxLayout;
    hlay1->addWidget(outTypeComboBox, 0);
    hlay1->addStretch();
    hlay1->addWidget(fitButton, 0);
    hlay1->addWidget(printButton, 0);
    qDebug()<<"createlayout add combo";

    QHBoxLayout *hlay2 = new QHBoxLayout;
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 5, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay1, 0);
        graphView = new GraphView(OUTFIGTYPE::UNDEFINED,true,this);
        vlay->addWidget(graphView);
        outRSTUComboBox->setVisible(false);
        outQUDSComboBox->setVisible(false);
        outFloorComboBox->setVisible(false);
        outFrameComboBox->setVisible(false);
        outFrameGroupComboBox->setVisible(false);
        hlay2->addWidget(outRSTUComboBox);
        hlay2->addWidget(outQUDSComboBox);
        hlay2->addWidget(outFloorComboBox);
        hlay2->addWidget(outFrameComboBox);
        hlay2->addWidget(outFrameGroupComboBox);
        vlay->addLayout(hlay2, 0);

    setLayout(vlay);
}


void GraphWidget::resetLayout(int index){
    if(index<=0){
        opt.type=OUTFIGTYPE::UNDEFINED;
    }else{
        auto type = OutputControl::getInstance()->outputTypeList(OUTFIGCATEGORY::GRAPH).at(index-1);
        opt.type=type;
    }
    switch(opt.type){
    case OUTFIGTYPE::UNDEFINED:
        outRSTUComboBox->setVisible(true);
        outQUDSComboBox->setVisible(false);
        outFloorComboBox->setVisible(false);
        outFrameComboBox->setVisible(false);
        outFrameGroupComboBox->setVisible(false);
        break;
    case OUTFIGTYPE::QDelta:
    case OUTFIGTYPE::QTheta:
        outRSTUComboBox->setVisible(true);
        outQUDSComboBox->setVisible(true);
        outFloorComboBox->setVisible(false);
        outFrameComboBox->setVisible(false);
        outFrameGroupComboBox->setVisible(false);
        break;
    case OUTFIGTYPE::QDeltaFrameFloors:
    case OUTFIGTYPE::QThetaFrameFloors:
        outRSTUComboBox->setVisible(true);
        outQUDSComboBox->setVisible(true);
        outFloorComboBox->setVisible(false);
        outFrameComboBox->setVisible(true);
        outFrameGroupComboBox->setVisible(false);
        break;
    case OUTFIGTYPE::QDeltaFloorFrames:
    case OUTFIGTYPE::QThetaFloorFrames:
        outRSTUComboBox->setVisible(true);
        outQUDSComboBox->setVisible(true);
        outFloorComboBox->setVisible(true);
        outFrameComboBox->setVisible(false);
        outFrameGroupComboBox->setVisible(true);
        break;
    case OUTFIGTYPE::QuNeedf:
        outRSTUComboBox->setVisible(true);
        outQUDSComboBox->setVisible(false);
        outFloorComboBox->setVisible(false);
        outFrameComboBox->setVisible(false);
        outFrameGroupComboBox->setVisible(false);
        break;
    default:
        break;
    }
    updateGraph();
}

void GraphWidget::updateGraph()
{
    if(outTypeComboBox->currentIndex()==0)return;
    opt.caseName=outRSTUComboBox->currentText();
    opt.floorName=outFloorComboBox->currentText();
    opt.frameName=outFrameComboBox->currentText();
    opt.frameGroupName=outFrameGroupComboBox->currentText();
    opt.qu_or_ds = outQUDSComboBox->currentText()==u8"QU" ? BASE_TYPE_PROPERTY::DSQU::QU : BASE_TYPE_PROPERTY::DSQU::DS;
    prop.unitPaper = PAPER(graphView->frameRect().size()/this->logicalDpiX()*DrawFigure::inchToMm);//pix -> mm;
    prop.legendType = opt.type;

        graphView->resetSceneValue(QList<GRAPH_SUBTYPE_PROPERTY>()<<opt,prop,1,setting);
        prop.sceneTitle=GraphFactory::makeGraphTitle(opt);
    return;
}


} // namespace post3dapp
