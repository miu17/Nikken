#include "mdisubwindow.h"

#include <QtGui>

#include "elevationCI_widget.h"
#include "elevationCL_widget.h"
#include "elevationOT_widget.h"
#include "elevationUI_widget.h"
#include "fixed_data.h"
#include "graph_widget.h"
#include "persCL_widget.h"
#include "persOT_widget.h"
#include "output_pers_widget.h"
#include "persUI_widget.h"
#include "planCI_widget.h"
#include "planCL_widget.h"
#include "planOT_widget.h"
#include "planUI_widget.h"
#include "unified_analysisdata.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"

namespace post3dapp{

MdiSubWindow::MdiSubWindow(const SUBWINSTATUS &status, QWidget *parent)
    : QMdiSubWindow(parent), mySubWinStatus(status)
{
    myInterfaceWidget = nullptr;
    uuid = QUuid();

    this->setStyle(new QProxyStyle);
    this->setWindowIcon(QIcon(QPixmap(":/icons/c2_yellow.png")));

    resetTitle();
    setInterfaceWidget();
    this->setAttribute(Qt::WA_DeleteOnClose);
}

MdiSubWindow::~MdiSubWindow()
{
    delete myInterfaceWidget;
}

void MdiSubWindow::setInterfaceWidget()
{
    qDebug()<<"MdiSubWindow::setInterfaceWidget";
    switch(mySubWinStatus.MDIType){
    case MDITYPE::MDIINPUT:
        switch(mySubWinStatus.WinType){
        case WINTYPE::WINPLAN:
            myInterfaceWidget = new PlanInterfaceWidget();
            connect(myInterfaceWidget, &InterfaceWidget::drawingFloorChanged,
                    this, &MdiSubWindow::changeCurrentID);
            break;
        case WINTYPE::WINELEVATION:
            myInterfaceWidget = new ElevationInterfaceWidget();
            connect(myInterfaceWidget, &InterfaceWidget::drawingFrameChanged,
                    this, &MdiSubWindow::changeCurrentID);
            break;
        case WINTYPE::WINPERS:
            myInterfaceWidget = new PersInterfaceWidget;
            break;
        default:
            throw;
        }
        this->setWidget(myInterfaceWidget);
        break;
    case MDITYPE::MDICOPY:
        switch(mySubWinStatus.WinType){
        case WINTYPE::WINPLAN:
            myInterfaceWidget = new PlanCopyInterfaceWidget(UnifiedFixedData::getInstance()->getInputData());
            connect(myInterfaceWidget, &InterfaceWidget::drawingFloorChanged,
                    this, &MdiSubWindow::changeCurrentID);
            break;
        case WINTYPE::WINELEVATION:
            myInterfaceWidget = new ElevationCopyInterfaceWidget(UnifiedFixedData::getInstance()->getInputData());
            connect(myInterfaceWidget, &InterfaceWidget::drawingFrameChanged,
                    this, &MdiSubWindow::changeCurrentID);
            break;
        case WINTYPE::WINPERS:
            myInterfaceWidget = new PersInterfaceWidget;
            break;
        default:
            throw;
        }
        this->setWidget(myInterfaceWidget);
        break;
    case MDITYPE::MDICALCLOAD:
        switch(mySubWinStatus.WinType){
        case WINTYPE::WINPLAN:
            myInterfaceWidget = new PlanCalcLoadWidget(UnifiedFixedData::getInstance()->getInputData());
            connect(myInterfaceWidget, &InterfaceWidget::drawingFloorChanged,
                    this, &MdiSubWindow::changeCurrentID);
            break;
        case WINTYPE::WINELEVATION:
            myInterfaceWidget = new ElevationCalcLoadWidget(UnifiedFixedData::getInstance()->getInputData());
            connect(myInterfaceWidget, &InterfaceWidget::drawingFrameChanged,
                    this, &MdiSubWindow::changeCurrentID);
            break;
        case WINTYPE::WINPERS:
            myInterfaceWidget = new PersCalcLoadWidget;
            break;
        default:
            throw;
        }
        this->setWidget(myInterfaceWidget);
        break;
    case MDITYPE::MDIOUTPUT:
        switch(mySubWinStatus.WinType){
        case WINTYPE::WINPLAN:
            myInterfaceWidget = new PlanOutputWidget();
            connect(myInterfaceWidget, &InterfaceWidget::drawingFloorChanged,
                    this, &MdiSubWindow::changeCurrentID);
            this->setWidget(myInterfaceWidget);
            break;
        case WINTYPE::WINELEVATION:
            myInterfaceWidget = new ElevationOutputWidget();
            connect(myInterfaceWidget, &InterfaceWidget::drawingFrameChanged,
                    this, &MdiSubWindow::changeCurrentID);
            this->setWidget(myInterfaceWidget);
            break;
        case WINTYPE::WINPERS:
            myInterfaceWidget = new OutputPersWidget();
//            myInterfaceWidget = new PersOutputWidget();
            this->setWidget(myInterfaceWidget);
            break;
        case WINTYPE::WINGRAPH:
            myInterfaceWidget = new GraphWidget();
            this->setWidget(myInterfaceWidget);
            break;
        case WINTYPE::WINTABLE:
            this->setWidget(new QWidget);
            break;
        }
        break;
    }
}

void MdiSubWindow::resetTitle()
{
    QString title;
    if(mySubWinStatus.MDIType == MDITYPE::MDIINPUT){
        title = u8"形状入力";
    }else if(mySubWinStatus.MDIType == MDITYPE::MDICOPY){
        title = u8"形状入力(解析結果)";
    }else if(mySubWinStatus.MDIType == MDITYPE::MDICALCLOAD){
        title = u8"荷重拾い結果";
    }else{
        title = u8"解析結果";
    }

    switch (mySubWinStatus.WinType) {
    case WINTYPE::WINPLAN:
        title += u8"伏図 -";
        if (mySubWinStatus.MDIType == MDITYPE::MDIINPUT){
            title += UnifiedInputData::getInInstance()->idToName(uuid, DATATYPE::TPFLOOR);
        }else if(mySubWinStatus.MDIType == MDITYPE::MDICOPY || mySubWinStatus.MDIType == MDITYPE::MDICALCLOAD){
            title += UnifiedFixedData::getInstance()->getInputData()->idToName(uuid, DATATYPE::TPFLOOR);
            title += UnifiedFixedData::getInstance()->getTime();
        }else{
            title += UnifiedFixedData::getInstance()->getAnalysisData()->idToName(uuid, DATATYPE::TPAFLOOR);
            title += UnifiedFixedData::getInstance()->getTime();
        }
        break;
    case WINTYPE::WINELEVATION:
        title += u8"軸組図 -";
        if (mySubWinStatus.MDIType == MDITYPE::MDIINPUT){
            title += UnifiedInputData::getInInstance()->idToName(uuid, DATATYPE::TPFRAMEP);
        }else if(mySubWinStatus.MDIType == MDITYPE::MDICOPY || mySubWinStatus.MDIType == MDITYPE::MDICALCLOAD){
            title += UnifiedFixedData::getInstance()->getInputData()->idToName(uuid, DATATYPE::TPFRAMEP);
            title += UnifiedFixedData::getInstance()->getTime();
        }else{
            title += UnifiedFixedData::getInstance()->getAnalysisData()->idToName(uuid, DATATYPE::TPFRAMEP);
            title += UnifiedFixedData::getInstance()->getTime();
        }
        break;
    case WINTYPE::WINPERS:
        title += u8"３Ｄパース";
        break;
    case WINTYPE::WINGRAPH:
        title += u8"グラフ表示";
        break;
    case WINTYPE::WINTABLE:
        title += u8"テキスト表示";
        break;
    default:
        break;
    }

    this->setWindowTitle(title);
}

void MdiSubWindow::changeCurrentID(const QString &str)
{
    if (mySubWinStatus.MDIType == MDITYPE::MDIINPUT){
        if (mySubWinStatus.WinType == WINTYPE::WINPLAN)
            uuid = UnifiedInputData::getInInstance()->nameToID(str, DATATYPE::TPFLOOR);
        else if (mySubWinStatus.WinType == WINTYPE::WINELEVATION)
            uuid = UnifiedInputData::getInInstance()->nameToID(str, DATATYPE::TPFRAMEP);
    }else if(mySubWinStatus.MDIType == MDITYPE::MDICOPY || mySubWinStatus.MDIType == MDITYPE::MDICALCLOAD){
        if (mySubWinStatus.WinType == WINTYPE::WINPLAN)
            uuid = UnifiedFixedData::getInstance()->getInputData()->nameToID(str, DATATYPE::TPFLOOR);
        else if (mySubWinStatus.WinType == WINTYPE::WINELEVATION)
            uuid = UnifiedFixedData::getInstance()->getInputData()->nameToID(str, DATATYPE::TPFRAMEP);
    }else{
        if (mySubWinStatus.WinType == WINTYPE::WINPLAN)
            uuid = UnifiedFixedData::getInstance()->getAnalysisData()->nameToID(str, DATATYPE::TPAFLOOR);
        else if (mySubWinStatus.WinType == WINTYPE::WINELEVATION)
            uuid = UnifiedFixedData::getInstance()->getAnalysisData()->nameToID(str, DATATYPE::TPFRAMEP);
    }
    resetTitle();
}

void MdiSubWindow::sendActiveStatusToChildWidget()
{
    if ( myInterfaceWidget == nullptr ) {        
        UnifiedEditingData::getInstance()->changeActiveWindowType(WINTYPE::NOWINTYPE);
        return;
    }

    UnifiedEditingData::getInstance()->changeActiveWindowType(mySubWinStatus.WinType);
    UnifiedEditingData::getInstance()->changeActiveMdiWindowType(mySubWinStatus.MDIType);

    if ( mySubWinStatus.WinType == WINTYPE::WINPLAN ||
         mySubWinStatus.WinType == WINTYPE::WINELEVATION ||
         mySubWinStatus.WinType == WINTYPE::WINPERS ) {
        myInterfaceWidget->sendActiveStatusToScene();
    }
}

} // namespace post3dapp
