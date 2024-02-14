#include "centerofrigidity_map_plan_template.h"

#include <typeinfo>

#include "define_analysis2dsettings.h"
#include "elevationOT_grscene.h"
#include "fixed_data.h"
#include "input_grscene.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "output_grscene.h"
#include "output_linesection_item_map.h"
#include "output_lineplane_item_map.h"
#include "output_planeitem.h"
#include "output_point_item.h"
#include "output_text_item.h"
#include "planOT_grscene.h"
#include "unified_analysisdata.h"
#include "unified_output.h"

namespace post3dapp{

CenterOfRigidityPlanTemplate::CenterOfRigidityPlanTemplate(const Analysis2DViewSettings& _setting, const FIGURE_TYPE_PROPERTY& type, QUuid uuid)
    : NumericPlanTemplate(_setting, type, uuid)
{
}

void CenterOfRigidityPlanTemplate::createPointItems(OutputGrScene *scene)
{
    switch(getFigureTypeProperty().type){
    case OUTFIGTYPE::CenterOfRigidity:
        centerOfRigidity(scene);

        break;
    default:
        return;
    }
}
void CenterOfRigidityPlanTemplate::centerOfRigidity(OutputGrScene *scene)
{
    qDebug()<<"centerOfRigidity";

    DataNBLD3D_STEC stec = UnifiedFixedData::getInstance()->getPstnData()->getNbldStecData();
    DataNBLD3D_CMDL model = UnifiedFixedData::getInstance()->getPstnData()->getCmdlData();
    qDebug()<<"centerOfRigidity2";

//簡易法のあとで清算法を必ず通る
//ifecst,kzwall,numec
//ifecst == 0 独立
//000 1->4
//001 1->3->4->6
//010 1->2->4->5
//011 1->2->3->4->5->6
//ifecst == 1　連成
//100 4
//101 4->6
//110 4->5
//111 4->5->6

//ifecst == 0 独立
//架構外雑壁剛性（簡易計算） kzwall > 0
//1:階全体（簡易計算法：雑壁非考慮） outs1::27,28,1,2
//2:階全体（簡易計算法：雑壁考慮） kzwall > 0 outs1::27,28,11,12
//3:指定領域（簡易計算法：雑壁非考慮） numec != 0 -> oute1::19,20,1,2
//ifecst == 1　連成
//架構外雑壁剛性（清算法） kzwall > 0
//4:階全体（清算法：雑壁非考慮） outs1::27,28,1,2
//5:階全体（清算法：雑壁考慮）    kzwall != 0 && numec == 0 Xg,Yg,Xs,Ys = outs1::27,28,11,12
//6:指定領域（清算法：雑壁非考慮） numec != 0 -> Xg,Yg,Xs,Ys = oute1::19,20,1,2

    int ig = 0;
    int il = UnifiedFixedData::getInstance()->getPstnData()->uuidToB3dIndex(getId());
//for(int is = 0;is< stec.nset;is++){
    int is = 0;
    qDebug()<<"centerOfRigidity is=" << is;

    QPointF G1 = QPointF(1000.0*stec.outs1[is][ig][il][26],1000.0*stec.outs1[is][ig][il][27]);
//    QPointF G2 = QPointF(1000.0*stec.oute1[is][il][18],1000.0*stec.oute1[is][il][19]);
    QPointF S1 = QPointF(1000.0*stec.outs1[is][ig][il][0],1000.0*stec.outs1[is][ig][il][1]);
//    QPointF S2 = QPointF(1000.0*stec.outs1[is][ig][il][10],1000.0*stec.outs1[is][ig][il][11]);
//    QPointF S3 = QPointF(1000.0*stec.oute1[is][il][0],1000.0*stec.oute1[is][il][1]);

    //せん断力中心・剛心。架構外雑壁を考慮するか否か、範囲指定をするか否かで分岐。
    qreal eps = 0.000001;
    QColor color1 = getViewSettings()->getShearForceColor();
    QColor color2 = getViewSettings()->getRigidityColor();
    int size = getViewSettings()->getRigiditySize();
    if(qAbs(G1.x()) > eps && qAbs(G1.y()) > eps)createPointItem(scene,G1,QPointF(1700,1000),u8"せん断力中心：階全体",color1,size);
//    createPointItem(scene,G2,u8"せん断力中心：指定領域");
    if(qAbs(S1.x()) > eps && qAbs(S1.y()) > eps)createPointItem(scene,S1,QPointF(1700,-1000),u8"剛心:階全体・雑壁非考慮",color2,size);
//    createPointItem(scene,S2,u8"剛心:階全体・雑壁考慮");
//    createPointItem(scene,S3,u8"剛心:指定領域・雑壁非考慮");

//}
}

void CenterOfRigidityPlanTemplate::createPointItem(OutputGrScene *scene,QPointF point,QPointF offset,QString str,QColor color,int size){
    qDebug()<<"MapPlanTemplate::createPointItem init ";
    OutputGraphicsItem *item;
    item = new OutputPointItem(this,DATATYPE::NODATATYPE,offset,str,color,size);
    item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    item->setPos(point);
    item->setZValue(-300.0);
    item->setEnabled(true);
    item->setSelected(false);

    scene->addItem(item);
    qDebug()<<"MapPlanTemplate::createPointItem done";

}


} // namespace post3dapp
