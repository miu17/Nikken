#include "eventoperation_abstract.h"

#include <QtGui>

#include "customdraw_grview.h"
#include "define_draw_figure.h"
#include "factory_unifieddata.h"
#include "input_grscene.h"
#include "operation_commands.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "unified_settings.h"

namespace post3dapp{
AbstractEventOperation::AbstractEventOperation(CustomDrawGrView *view)
    : QObject(view), myGrView(view)
{
    isOutput = view->askIsOutput();
    myGrScene = reinterpret_cast<InputGrScene *>(myGrView->scene());
    viewMode = true;
    mousePressed = false;
    onRubberHide = false;

    connect(myGrScene, &InputGrScene::itemSelectChanged, this, &AbstractEventOperation::slotChangeActionStatus);
}

void AbstractEventOperation::execDrawForeground(QPainter *painter, const QRect &vrect)
{
    if ( !isPositionSnapOn() ) return;

    painter->setTransform(QTransform(1, 0, 0, 1, 0, 0), false);

    painter->setFont(QFont("Consolas", 9));

    painter->setPen(QColor(235, 0, 0, 200));
    QPoint cp0 = myGrView->mapFromScene(currentGP.getElvQPointF() * 1000.);
    painter->drawLine(QPoint(cp0.x(), 0), QPoint(cp0.x(), 20));
    painter->drawLine(QPoint(0, cp0.y()), QPoint(20, cp0.y()));
    painter->drawLine(QPoint(cp0.x(), cp0.y() - 10), QPoint(cp0.x(), cp0.y() + 10));
    painter->drawLine(QPoint(cp0.x() - 10, cp0.y()), QPoint(cp0.x() + 10, cp0.y()));

    painter->setPen(QColor(200, 0, 0, 100));
    painter->drawLine(QPoint(cp0.x(), 0), QPoint(cp0.x(), vrect.height()));
    painter->drawLine(QPoint(0, cp0.y()), QPoint(vrect.width(), cp0.y()));

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(200, 200, 200, 50));
    painter->drawRect( QRect(30, vrect.height() - 80, 330, 65) );
    painter->setPen(QColor(204, 51, 0, 230));
    QRect txrect(35, vrect.height() - 75, vrect.width() - 40, 100);
    painter->drawText(txrect, Qt::AlignLeft | Qt::AlignTop, positionText);
}

void AbstractEventOperation::execDrawBackground(QPainter *painter, const QRect &)
{
    myGrScene->drawBackGround(painter);
}

void AbstractEventOperation::execMousePress(QMouseEvent * ) { }
void AbstractEventOperation::execMouseRelease(QMouseEvent * ) { }
void AbstractEventOperation::execMouseMove(QMouseEvent *) { }
void AbstractEventOperation::execKeyPress(QKeyEvent *) { }

void AbstractEventOperation::reloadSettings()
{
    myElementType = UnifiedEditingData::getInstance()->currentElement();
    currentAddEditMode = UnifiedEditingData::getInstance()->currentAddEditMode();
    if ( !isViewModeOn() ) initializeSettings();
}

void AbstractEventOperation::initializeSettings()
{
    myGrView->clearSelectedItems();
    myGrScene->clearCurrentAttach();
}

void AbstractEventOperation::setWaitingMode()
{
}

void AbstractEventOperation::setViewModeOn(bool ok)
{
    viewMode = ok;
    if (viewMode) myGrScene->clearCurrentAttach();
}

void AbstractEventOperation::toggleViewMode()
{
    viewMode = !viewMode;
}

void AbstractEventOperation::hideRubberBand(bool on)
{
    onRubberHide = on;
}

bool AbstractEventOperation::isHiddenRubberBand() const
{
    return onRubberHide;
}

bool AbstractEventOperation::isMousePressed() const
{
    return mousePressed;
}

bool AbstractEventOperation::isViewModeOn() const
{
    return viewMode;
}

bool AbstractEventOperation::isBatchInputMode() const
{
    return false;
}

bool AbstractEventOperation::isSelectItemMode() const
{
    return false;
}

bool AbstractEventOperation::isPositionSnapOn() const
{
    return false;
}

void AbstractEventOperation::mousePressOn(bool ok)
{
    mousePressed = ok;
}

void AbstractEventOperation::changePosition(const QPoint &cur_p)
{
    positionSnap(cur_p);
    changePositionText();
    myGrScene->setCurrentFrames(currentGP.getAttachedFrameList());
    if (myGrScene->sceneType() == SCENETYPE::ELEVATIONSCENE)
        myGrScene->setCurrentFloors(currentGP.getAttachedFloorList());
    myGrScene->update();
}

QPointF AbstractEventOperation::currentPosition() const
{
    return currentGP.getElvQPointF();
}

void AbstractEventOperation::positionSnap(const QPoint &cur_p, bool searchJoint)
{
    QPointF pp = myGrView->mapToScene( cur_p );

    SNAPSTATE snap_state = UnifiedEditingData::getInstance()->currentSnapState();
    GlobalPoint nearly_point;
    qreal min_len = 1.0e+10;
    qreal eps = 10.0;

    if (searchJoint) {
        myGrScene->searchNearerJointItem(pp, nearly_point, min_len);
    }

    min_len = min_len - eps;
    if (snap_state.crossIsChecked) {
        searchNearerCrossPoint(pp, nearly_point, min_len,snap_state.crossOffset);
    }

    min_len = min_len - eps;
    if (snap_state.defineIsChecked) {
        searchNearerDefinePoint(pp, nearly_point, min_len);
    }

    min_len = min_len - eps;
    if (snap_state.gridIsChecked) {
        searchNearerGridPoint(pp, snap_state.gridPitch, nearly_point, min_len);
    }

    min_len = min_len - eps;
    if (snap_state.divideIsChecked) {
        QPointF tmp_point;
        bool detected = myGrScene->searchDivideMemberPoint(pp, tmp_point, min_len,
                                                           snap_state.divideEquallyIsChecked, snap_state.divideParts,
                                                           snap_state.divideFromIJIsChecked, snap_state.divideFromI, snap_state.divideFromJ);
        if ( detected ) {
            nearly_point = GlobalPoint();
            nearly_point.setElvX(tmp_point.x() / 1000.0);
            nearly_point.setElvY(tmp_point.y() / 1000.0);
            nearly_point.setElvZ(0.0);
            myGrScene->completeGlobalXYZ(nearly_point);
        }
    }

    currentGP = nearly_point;
    qDebug()<<"current"<<currentGP.x()<<currentGP.y()<<currentGP.z();
}

void AbstractEventOperation::searchNearerCrossPoint(QPointF pp, GlobalPoint &nearly_point,
                                                    qreal &min_len,qreal offset)
{
    qDebug()<<"offset = "<<offset;
    qreal eps = 0.001;
    if(offset < eps){
        Q_FOREACH (GlobalPoint gp, myGrScene->getCrossPoints()) {
            qreal len = (pp - gp.getElvQPointF() * 1000.0).manhattanLength();
            if (len < min_len) {
                min_len = len;
                nearly_point = gp;
            }
        }
    }else{
        Q_FOREACH (GlobalPoint gp, myGrScene->getCrossPoints()) {
            GlobalPoint gpmx = gp;
            gpmx.setElvX(gp.elvX()-offset/1000.0);
            GlobalPoint gppx = gp;
            gppx.setElvX(gp.elvX()+offset/1000.0);
            GlobalPoint gpmy = gp;
            gpmy.setElvY(gp.elvY()-offset/1000.0);
            GlobalPoint gppy = gp;
            gppy.setElvY(gp.elvY()+offset/1000.0);
            GlobalPoint gpmz = gp;
            qreal len1 = (pp - gpmx.getElvQPointF() * 1000.0).manhattanLength();
            qreal len2 = (pp - gppx.getElvQPointF() * 1000.0).manhattanLength();
            qreal len3 = (pp - gpmy.getElvQPointF() * 1000.0).manhattanLength();
            qreal len4 = (pp - gppy.getElvQPointF() * 1000.0).manhattanLength();
            if (len1 < min_len) {
                min_len = len1;
                nearly_point = gpmx;
            }
            if (len2 < min_len) {
                min_len = len2;
                nearly_point = gppx;
            }
            if (len3 < min_len) {
                min_len = len3;
                nearly_point = gpmy;
            }
            if (len4 < min_len) {
                min_len = len4;
                nearly_point = gppy;
            }
            myGrScene->completeGlobalXYZ(nearly_point);
        }

    }
}

void AbstractEventOperation::searchNearerDefinePoint(QPointF pp, GlobalPoint &nearly_point,
                                                     qreal &min_len)
{
    Q_FOREACH (GlobalPoint gp, myGrScene->getDefinePoints()) {
        qreal len = (pp - gp.getElvQPointF() * 1000.0).manhattanLength();
        if (len < min_len) {
            min_len = len;
            nearly_point = gp;
        }
    }
}

void AbstractEventOperation::searchNearerGridPoint(QPointF pp, int g_pitch,
                                                   GlobalPoint &nearly_point, qreal &min_len )
{
    int rx = pp.x();
    int ry = pp.y();

    int modx = abs(rx) % g_pitch;
    if ( modx < g_pitch / 2 ) {
        ( rx >= 0 ) ? rx -= modx : rx += modx ;
    } else {
        ( rx >= 0 ) ? rx = rx - modx + g_pitch : rx = rx + modx - g_pitch ;
    }

    int mody = abs(ry) % g_pitch;
    if ( mody < g_pitch / 2 ) {
        ( ry >= 0 ) ? ry -= mody : ry += mody ;
    } else {
        ( ry >= 0 ) ? ry = ry - mody + g_pitch : ry = ry + mody - g_pitch ;
    }

    qreal len = (pp - QPointF(rx, ry)).manhattanLength();
    if ( len < min_len ) {
        nearly_point = GlobalPoint();
        nearly_point.setElvX(rx / 1000.);
        nearly_point.setElvY(ry / 1000.);
        nearly_point.setElvZ(0.0);
        myGrScene->completeGlobalXYZ(nearly_point);

        min_len = len;
    }
}

void AbstractEventOperation::changePositionText()
{
    positionText.clear();

    positionText += QString("Floor: ");
    QList<QUuid> idlist = currentGP.getAttachedFloorList();
    Q_FOREACH (QUuid id, idlist)
        positionText += FactoryUnifiedData::getInstance(isOutput)->idToName(id, DATATYPE::TPFLOOR) + ",";
    positionText += QString("\n");
    positionText += QString("Frame: ");
    idlist = currentGP.getAttachedFrameList();
    Q_FOREACH (QUuid id, idlist)
        positionText += FactoryUnifiedData::getInstance(isOutput)->idToName(id, DATATYPE::TPFRAMEP) + ",";
    positionText += QString("\n");

    positionText += QString("( X, Y, Z ) = (%1m,%2m,%3m )\n").arg(currentGP.x(), 8, 'f', 3)
                    .arg(currentGP.y(), 8, 'f', 3)
                    .arg(currentGP.z(), 8, 'f', 3);
    positionText += QString("( x, y, z ) = (%1m,%2m,%3m )\n").arg(currentGP.elvX(), 8, 'f', 3)
                    .arg(currentGP.elvY(), 8, 'f', 3)
                    .arg(currentGP.elvZ(), 8, 'f', 3);
}

QList<GlobalPoint> AbstractEventOperation::scenePointsToGlobalPoints(const QList<QPointF> &plist)
{
    QList<GlobalPoint> gplist;
    qreal eps = 1.0e-3;

    Q_FOREACH (QPointF pp, plist) {

        bool detected = false;
        GlobalPoint d_gp;

        // CrossPoint Search
        Q_FOREACH ( GlobalPoint gp, myGrScene->getCrossPoints() ) {
            if ( (pp - gp.getElvQPointF() * 1000.0).manhattanLength() < eps ) {
                d_gp = gp;
                detected = true;
                break;
            }
        }

        if (detected) {
            gplist.append(d_gp);
            continue;
        }

        // DefinePoint Search
        Q_FOREACH ( GlobalPoint gp, myGrScene->getDefinePoints() ) {
            if ( (pp - gp.getElvQPointF() * 1000.0).manhattanLength() < eps ) {
                d_gp = gp;
                detected = true;
                break;
            }
        }

        if (detected) {
            gplist.append(d_gp);
            continue;
        }

        // GridPoint or DividePoint

        d_gp = GlobalPoint();
        d_gp.setElvX(pp.x() / 1000.);
        d_gp.setElvY(pp.y() / 1000.);
        d_gp.setElvZ(0.0);
        myGrScene->completeGlobalXYZ(d_gp);

        gplist.append(d_gp);
    }

    return gplist;
}

DATATYPE AbstractEventOperation::currentDataType() const
{
    DATATYPE type;
    if (myElementType == ELJOINT) {
        type = DATATYPE::TPJOINT;
    } else if (myElementType == ELCOLUMN) {
        type = DATATYPE::TPCOLUMN;
    } else if (myElementType == ELGIRDER) {
        type = DATATYPE::TPGIRDER;
    } else if (myElementType == ELBRACE) {
        type = DATATYPE::TPBRACE;
    } else if (myElementType == ELBEAM) {
        type = DATATYPE::TPBEAM;
    } else if (myElementType == ELSLAB) {
        type = DATATYPE::TPSLAB;
    } else if (myElementType == ELWALL) {
        type = DATATYPE::TPWALL;
    } else if (myElementType == ELUWALL) {
        type = DATATYPE::TPUWALL;
    } else if (myElementType == ELDAMPER) {
        type = DATATYPE::TPDAMPER;
    } else if (myElementType == ELISO) {
        type = DATATYPE::TPISO;
    } else if (myElementType == ELSLABLOAD) {
        type = DATATYPE::TPSLABLOAD;
    } else if (myElementType == ELMEMBERLOAD) {
        type = DATATYPE::TPMEMBERLOAD;
    } else if (myElementType == ELJOINTLOAD) {
        type = DATATYPE::TPJOINTLOAD;
    } else {
        type = DATATYPE::NODATATYPE;
    }
    return type;
}

void AbstractEventOperation::divideMembersByJoints(const QList<JointData *> &jdlist,
                                                   ADDMOVEACTION am_act )
{
    if ( jdlist.isEmpty() ) return;
    if ( am_act != AMINTER && am_act != AMCOMPOSE ) return;

    QList<MemberData *> m_mdlist;
    QList<QList<JointData *> > m_jdlist;
    Q_FOREACH ( JointData *jd, jdlist ) {
        QList<MemberData *> mdlist = myGrScene->searchContainMembers(jd, true);
        Q_FOREACH (MemberData *md, mdlist) {
            if ( !m_mdlist.contains(md) ) {
                m_mdlist.append(md);
                m_jdlist.append( QList<JointData *>() << jd );
            } else {
                int idx = m_mdlist.indexOf(md);
                QList<JointData *> jlist = m_jdlist.at(idx);
                jlist.append(jd);
                m_jdlist.replace(idx, jlist);
            }
        }
    }
    if ( m_mdlist.isEmpty() ) return;

    if ( am_act == AMINTER ) {
        InteriorMembersCommand *im_com = new InteriorMembersCommand(m_mdlist, m_jdlist);
        UnifiedEditingData::getInstance()->pushUndoCommand(im_com);
    }
    if ( am_act == AMCOMPOSE ) {
        DivideMembersCommand *dv_com = new DivideMembersCommand(m_mdlist, m_jdlist);
        UnifiedEditingData::getInstance()->pushUndoCommand(dv_com);
    }
}


void AbstractEventOperation::showInformationMessage(const QString &msg)
{
    QMessageBox::information( myGrView, "Information", msg,
                              QMessageBox::NoButton, QMessageBox::Ok );
}

void AbstractEventOperation::showWarningMessage(const QString &msg)
{
    QMessageBox::warning( myGrView, "WARNING", msg,
                          QMessageBox::NoButton, QMessageBox::Warning );
}

void AbstractEventOperation::warningInvalidValue()
{
    QMessageBox::warning( myGrView, "WARNING", u8"入力された値が不正です。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}

bool AbstractEventOperation::warningJointBatchInput()
{
    int ret = QMessageBox::warning(myGrView, "WARNING",
                                   u8"選択された位置に節点を一括入力します。\nよろしいですか？",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel );
    return (ret == QMessageBox::Cancel);

}

bool AbstractEventOperation::warningMemberBatchInput()
{
    int ret = QMessageBox::warning(myGrView, "WARNING",
                                   u8"選択された位置に部材を一括入力します。\nよろしいですか？",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel );
    return (ret == QMessageBox::Cancel);

}

bool AbstractEventOperation::warningLoadBatchInput()
{
    int ret = QMessageBox::warning(myGrView, "WARNING",
                                   u8"選択された位置に荷重を一括入力します。\nよろしいですか？",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel );
    return (ret == QMessageBox::Cancel);

}

post3dapp::CustomDrawGrView *post3dapp::AbstractEventOperation::view() const
{
    return myGrView;
}

InputGrScene *AbstractEventOperation::scene() const
{
    return myGrScene;
}

} // namespace post3dapp
