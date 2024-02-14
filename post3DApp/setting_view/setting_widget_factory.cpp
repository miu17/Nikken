#include "setting_widget_factory.h"

#include "axialforce2dviewsettingwidget.h"
#include "centerofrigidity2dviewsettingwidget.h"
#include "check2dviewsettingwidget.h"
#include "common2dviewsettingwidget.h"
#include "custom_setting_dialog.h"
#include "deformation2dviewsettingwidget.h"
#include "graphnmint2dviewsettingwidget.h"
#include "graphqdelta2dviewsettingwidget.h"
#include "horizontalcapacity2dviewsettingwidget.h"
#include "model2dviewsettingwidget.h"
#include "output_control.h"
#include "reactionforce2dviewsettingwidget.h"
#include "stress2dviewsettingwidget.h"

namespace post3dapp {

BaseSettingWidget* SettingWidgetFactory::createSettingWidget(const SETTINGDIALOGSETTINGS& type)
{
    //　　2d　/ 3dがある。viewタイプmdioutputじゃなくて、2DOutputみたいなやつを持つ？
    //inputも全体と個別がある。タブの1頁を返すか、タブウィジェット全体で返すか。セーブロード含めて一元化するなら階層構造は変えないほうがいい。
    //タブとかStackedとかに必要な数詰めてそれを返す？inputだと全体・個別、outputのディスプレイ用だと共通・モデル、outputのシングルだとそれだけ、outputのマルチだと全部、とか。
    //であれば、BaseSettingWidgetは不要でQWidgetで十分？共通機能がある？
    QStackedWidget* stack= new QStackedWidget;
    //if(type.dialogMode==DIALOGMODE::DIALOG_SINGLE){}

//    if(type.modelViewType==INPUT2DMODEL){
//        return new InputFigure2DWidget();
//        QWidget *planAllWidget;
//        IndividualSettingTableWidget *planIndividualWidget;
//    }
    //out
    if(type.modelViewType==OUTPUT2DMODEL){
        QString typeStr = OutputControl::outfigStrings.value(type.type).japanese();
        QString typeKey = OutputControl::outfigStrings.value(type.type).english();
        switch(type.type){
        case OUTFIGTYPE::MODEL:
            return new OutputFigureModel2DWidget(typeStr,typeKey);
        case OUTFIGTYPE::CMQ:
        case OUTFIGTYPE::NSTRESS:
        case OUTFIGTYPE::FSTRESS:
        case OUTFIGTYPE::TORSION:
        case OUTFIGTYPE::ALLOW:
        case OUTFIGTYPE::RANK:
            return new OutputFigureStress2DWidget(typeStr,typeKey);
        case OUTFIGTYPE::REACTMAP:
        case OUTFIGTYPE::COMBIREACTMAP:
            return new OutputFigureReactionForce2DWidget(typeStr,typeKey);
        case OUTFIGTYPE::AXIAL:
            return new OutputFigureAxialForce2DWidget(typeStr,typeKey);
        case OUTFIGTYPE::DEFORM:
            return new OutputFigureDeformation2DWidget(typeStr,typeKey);
        case OUTFIGTYPE::CHECKRATIO:
        case OUTFIGTYPE::ALLOWRATIOE:
        case OUTFIGTYPE::ALLOWRATIOU:
            return new OutputFigureCheck2DWidget(typeStr,typeKey);
        case OUTFIGTYPE::CenterOfRigidity:
            return new OutputFigureCenterOfRigidity2DWidget(typeStr,typeKey);
        case OUTFIGTYPE::QDelta:
        case OUTFIGTYPE::QDeltaFloorFrames:
        case OUTFIGTYPE::QDeltaFrameFloors:
        case OUTFIGTYPE::QTheta:
        case OUTFIGTYPE::QThetaFloorFrames:
        case OUTFIGTYPE::QThetaFrameFloors:
            return new OutputFigureGraphQDelta2DWidget(typeStr,typeKey);
        case OUTFIGTYPE::QuNeedf:
            return new OutputFigureGraphHorizontalCapacity2dWidget(typeStr,typeKey);
        case OUTFIGTYPE::NMINT:
            return new OutputFigureGraphNmint2DWidget(typeStr,typeKey);


            //...
        default:
            return new OutputFigureCommon2DWidget(typeStr, typeKey);
        }
        //calc

    }
    return new BaseSettingWidget();
}
}
