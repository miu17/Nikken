#include "persUI_widget.h"

#include <QtGui>

#include "autoattach_dialog.h"
#include "changeangle_dialog.h"
#include "changeface_dialog.h"
#include "changeglobaljoint_dialog.h"
#include "changeopen_dialog.h"
#include "changesection_dialog.h"
#include "changeshift_dialog.h"
#include "finditem_dialog.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "manage_persactions.h"
#include "operation_commands.h"
#include "persUI_glview.h"
#include "persUI_scene.h"
#include "persUI_settingdialog.h"
#include "unified_inputdata.h"
#include "unified_editingdata.h"
#include "unified_settings.h"
#include "vhline.h"

namespace post3dapp{
PersInterfaceWidget::PersInterfaceWidget(QWidget *parent)
    : InterfaceWidget(parent)
{
    controlDialog = new PersInterfaceSettingDialog(this);
    connect( controlDialog, &PersInterfaceSettingDialog::viewSettingChanged, this, &PersInterfaceWidget::slotChangeViewSettings );

    createSceneView();
    createButtons();



    manageActions = new ManagePersActions(myGLView);

    connect(myGLView, &PersInterfaceGLViewer::signalShowAutoAttachDialog, this, &PersInterfaceWidget::slotShowAutoAttachDialog);
    connect(myGLView, &PersInterfaceGLViewer::signalShowChangeAngleDialog, this, &PersInterfaceWidget::slotShowChangeAngleDialog);
    connect(myGLView, &PersInterfaceGLViewer::signalShowChangeFaceDialog, this, &PersInterfaceWidget::slotShowChangeFaceDialog);
    connect(myGLView, &PersInterfaceGLViewer::signalShowChangeJointDialog, this, &PersInterfaceWidget::slotShowChangeJointDialog);
    connect(myGLView, &PersInterfaceGLViewer::signalChangeWallOpen, this, &PersInterfaceWidget::slotChangeWallOpen);
    connect(myGLView, &PersInterfaceGLViewer::signalChangeSectionDialog, this, &PersInterfaceWidget::slotChangeSectionDialog);
    connect(myGLView, &PersInterfaceGLViewer::signalShowChangeShiftDialog, this, &PersInterfaceWidget::slotShowChangeShiftDialog);
    connect(myGLView, &PersInterfaceGLViewer::signalFindItems, this, &PersInterfaceWidget::slotFindItems);
    connect(myGLView, &PersInterfaceGLViewer::signalDeleteItems, this, &PersInterfaceWidget::slotDeleteItems);
    connect(myGLView, &PersInterfaceGLViewer::signalDeleteIndependentJoints, this, &PersInterfaceWidget::slotDeleteIndependentJoints);
    connect(myGLView, &PersInterfaceGLViewer::signalReverseMemberJoint, this, &PersInterfaceWidget::slotReverseMemberJoint);
    connect(myGLView, &PersInterfaceGLViewer::signalDeleteWallOpen, this, &PersInterfaceWidget::slotDeleteWallOpen);

    connect(myGLView, &PersInterfaceGLViewer::signalSetActionStatus, this, &PersInterfaceWidget::slotSetActionStatus);
    connect(myGLView, &PersInterfaceGLViewer::signalSendSelectedItems, this, &PersInterfaceWidget::slotSendSelectedItems);
    connect(myGLView, &PersInterfaceGLViewer::signalSelectPoints, this, &PersInterfaceWidget::slotSelectPoints);
    connect(myGLView, &PersInterfaceGLViewer::signalAppendSelectedMemberID, this, &PersInterfaceWidget::slotAppendSelectedMemberID);

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::currentStatusChanged,
            this, &PersInterfaceWidget::slotImportCurrentStatus);
    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::selectTermChanged,
            myGLView->getScene(), &PersInterfaceScene::initializeSelectItems);
}

PersInterfaceWidget::~PersInterfaceWidget()
{
    delete myGLView;
}

void PersInterfaceWidget::sendActiveStatusToScene()
{
    PersInterfaceScene *scene = UnifiedEditingData::getInstance()->getSingularPersScene();
    scene->updateViewSettings();
    slotSendSelectedItems();
}

bool PersInterfaceWidget::changeJointsGlobalXYZ(bool ok_x, bool ok_y, bool ok_z,
                                                qreal gx, qreal gy, qreal gz)
{
    if ( ok_x && ok_y && ok_z )
        return false;
    if ( myGLView->getScene()->getSelectedJoints().isEmpty() )
        return true;

    QList<GlobalPoint> gplist;
    Q_FOREACH (JointData *jd, myGLView->getScene()->getSelectedJoints()) {
        GlobalPoint gp;
        qreal xx = ( ok_x ) ? gx : jd->xPos();
        qreal yy = ( ok_y ) ? gy : jd->yPos();
        qreal zz = ( ok_z ) ? gz : jd->zPos();
        gp.setX(xx);
        gp.setY(yy);
        gp.setZ(zz);
        gp.appendAttachedFloor(jd->getAttachedFloorList());
        gp.appendAttachedFrame(jd->getAttachedFrameList());
        gplist.append(gp);
    }
//    divideMembersByJoints(jdlist,viewSettings.currentAddMoveAction());
    QList<JointData *> select_joints = myGLView->getScene()->getSelectedJoints();
    myGLView->getScene()->clearSelectedItems();

//    return  FactoryUnifiedData::getInstance(isOutput)->changeJoints(select_joints,gplist);

    ChangeJointsCommand *cj_com = new ChangeJointsCommand(select_joints, gplist);
    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
    return cj_com->isSuccessfullyChanged();
}

bool PersInterfaceWidget::changJointsGlobalDelta(bool ok_x, bool ok_y, bool ok_z,
                                                 qreal dx, qreal dy, qreal dz)
{
    if ( ok_x && ok_y && ok_z )
        return false;
    if ( myGLView->getScene()->getSelectedJoints().isEmpty() )
        return true;

    QList<GlobalPoint> gplist;
    Q_FOREACH (JointData *jd, myGLView->getScene()->getSelectedJoints()) {
        GlobalPoint gp;
        qreal xx = ( ok_x ) ? jd->xPos() + dx : jd->xPos();
        qreal yy = ( ok_y ) ? jd->yPos() + dy : jd->yPos();
        qreal zz = ( ok_z ) ? jd->zPos() + dz : jd->zPos();
        gp.setX(xx);
        gp.setY(yy);
        gp.setZ(zz);
        gp.appendAttachedFloor(jd->getAttachedFloorList());
        gp.appendAttachedFrame(jd->getAttachedFrameList());
        gplist.append(gp);
    }
//    divideMembersByJoints(jdlist,viewSettings.currentAddMoveAction());
    QList<JointData *> select_joints = myGLView->getScene()->getSelectedJoints();
    myGLView->getScene()->clearSelectedItems();

//    return  FactoryUnifiedData::getInstance(isOutput)->changeJoints(select_joints,gplist);

    ChangeJointsCommand *cj_com = new ChangeJointsCommand(select_joints, gplist);
    UnifiedEditingData::getInstance()->pushUndoCommand(cj_com);
    return cj_com->isSuccessfullyChanged();
}

void PersInterfaceWidget::clearFloorAttachment()
{
    if ( myGLView->getScene()->getSelectedJoints().isEmpty() )
        return;
    QList<JointData *> select_joints = myGLView->getScene()->getSelectedJoints();
    myGLView->getScene()->clearSelectedItems();
    UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeJointsAttachmentCommand(select_joints, QList<QUuid>(), DATATYPE::TPFLOOR));
}

bool PersInterfaceWidget::appendFloorAttachment(const QString &floors)
{
    if ( myGLView->getScene()->getSelectedJoints().isEmpty() )
        return true;

    QList<QUuid> fl_idlist;
    QStringList strlist = floors.split(",");
    if ( strlist.last().isEmpty() ) strlist.removeLast();
    Q_FOREACH ( QString str, strlist ) {
        QUuid id =  UnifiedInputData::getInInstance()->nameToID(str, DATATYPE::TPFLOOR);
        if ( id.isNull() ) return false;
        if ( !fl_idlist.contains(id) ) fl_idlist.append(id);
    }
    QList<JointData *> select_joints = myGLView->getScene()->getSelectedJoints();
    myGLView->getScene()->clearSelectedItems();
    UnifiedEditingData::getInstance()->pushUndoCommand(new AppendJointsAttachmentCommand(select_joints, fl_idlist, DATATYPE::TPFLOOR));
    return true;
}

void PersInterfaceWidget::clearFrameAttachment()
{
    if ( myGLView->getScene()->getSelectedJoints().isEmpty() )
        return;
    QList<JointData *> select_joints = myGLView->getScene()->getSelectedJoints();
    myGLView->getScene()->clearSelectedItems();
    UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeJointsAttachmentCommand(select_joints, QList<QUuid>(), DATATYPE::TPFRAMEP));
}

bool PersInterfaceWidget::appendFrameAttachment(const QString &frames)
{
    if ( myGLView->getScene()->getSelectedJoints().isEmpty() )
        return true;

    QList<QUuid> fr_idlist;
    QStringList strlist = frames.split(",");
    if ( strlist.last().isEmpty() ) strlist.removeLast();
    Q_FOREACH ( QString str, strlist ) {
        QUuid id =  UnifiedInputData::getInInstance()->nameToID(str, DATATYPE::TPFRAMEP);
        if ( id.isNull() ) return false;
        if ( !fr_idlist.contains(id) ) fr_idlist.append(id);
    }
    QList<JointData *> select_joints = myGLView->getScene()->getSelectedJoints();
    myGLView->getScene()->clearSelectedItems();
    UnifiedEditingData::getInstance()->pushUndoCommand(new AppendJointsAttachmentCommand(select_joints, fr_idlist, DATATYPE::TPFRAMEP));
    return true;
}

void PersInterfaceWidget::createSceneView()
{
    PersInterfaceScene *scene = UnifiedEditingData::getInstance()->getSingularPersScene();
    QGLWidget *share = UnifiedEditingData::getInstance()->getSingularGLWidget();
    myGLView = new PersInterfaceGLViewer(scene, this, share);

    if ( scene->isNotInitialized() )
        QTimer::singleShot(1000, scene, &PersInterfaceScene::initDisplayList);
}

void PersInterfaceWidget::createButtons()
{
    QToolButton *pngButton = new QToolButton(this);
    pngButton->setToolTip(u8"画像ファイル保存");
    pngButton->setAutoRaise(true);
    pngButton->setIcon(QIcon(QPixmap(":/icons/png.png")));
    pngButton->setIconSize(QSize(30, 30));
    pngButton->setFixedSize(30, 30);

    connect( pngButton, &QAbstractButton::clicked, this, &PersInterfaceWidget::slotSavePixmap );

    QToolButton *zoomInButton = new QToolButton(this);
    zoomInButton->setToolTip(u8"表示拡大");
    zoomInButton->setAutoRaise(true);
    zoomInButton->setIcon(QIcon(QPixmap(":/icons/zoomin.png")));
    zoomInButton->setIconSize(QSize(25, 25));
    zoomInButton->setFixedSize(30, 30);
    zoomInButton->setAutoRepeat(true);
    zoomInButton->setAutoRepeatInterval(33);
    zoomInButton->setAutoRepeatDelay(0);

    QToolButton *zoomOutButton = new QToolButton(this);
    zoomOutButton->setToolTip(u8"表示縮小");
    zoomOutButton->setAutoRaise(true);
    zoomOutButton->setIcon(QIcon(QPixmap(":/icons/zoomout.png")));
    zoomOutButton->setIconSize(QSize(25, 25));
    zoomOutButton->setFixedSize(30, 30);
    zoomOutButton->setAutoRepeat(true);
    zoomOutButton->setAutoRepeatInterval(33);
    zoomOutButton->setAutoRepeatDelay(0);

    connect ( zoomInButton, &QAbstractButton::clicked, myGLView, &PersInterfaceGLViewer::zoomIn );
    connect ( zoomOutButton, &QAbstractButton::clicked, myGLView, &PersInterfaceGLViewer::zoomOut );

    QToolButton *fitButton = new QToolButton(this);
    fitButton->setToolTip(u8"画面フィッティング");
    fitButton->setAutoRaise(true);
    fitButton->setIcon(QIcon(QPixmap(":/icons/fit.png")));
    fitButton->setFixedSize(30, 30);
    fitButton->setIconSize(QSize(25, 25));

    connect ( fitButton, &QAbstractButton::clicked, myGLView, &PersInterfaceGLViewer::fitWindow );

    dirCombobox = new QComboBox(this);
    QStringList strlist1{
        u8"Y軸 正方向", u8"Y軸 負方向", u8"X軸 正方向", u8"X軸 負方向", u8"Z軸 正方向", u8"Z軸 負方向"
    };
    dirCombobox->setStyle(UnifiedEditingData::getInstance()->getWindowXPStyle() );
    dirCombobox->addItems(strlist1);
    dirCombobox->setFixedWidth(100);
    connect (dirCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged), myGLView, &PersInterfaceGLViewer::resetViewDir);

    controlButton = new QToolButton(this);
    controlButton->setAutoRaise(true);
    controlButton->setIcon(QIcon(QPixmap(":/icons/control.png")));
    controlButton->setToolTip(u8"個別ウィンドウの表示設定");
    controlButton->setIconSize(QSize(30, 30));
    controlButton->setFixedSize(30, 30);

    controlButton->setCheckable(true);
    controlButton->setChecked(false);

    connect(controlButton, &QAbstractButton::toggled, this, &PersInterfaceWidget::slotShowControlDialog);
    connect(controlDialog, &PersInterfaceSettingDialog::windowClosed, this, &PersInterfaceWidget::slotControlButtonUnChecked);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setSpacing(5);
    hlay->addWidget(pngButton);
    hlay->addWidget(new VFLine(Qt::lightGray, this));
    hlay->addWidget(zoomOutButton);
    hlay->addWidget(zoomInButton);
    hlay->addWidget(fitButton);
    hlay->addWidget(new VFLine(Qt::lightGray, this));
    hlay->addWidget(new QLabel(u8"視点:", this));
    hlay->addWidget(dirCombobox);
    hlay->addWidget(new VFLine(Qt::lightGray, this));
    hlay->addStretch();
    hlay->addWidget(controlButton);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 5, 0, 0);
    vlay->setSpacing(3);
    vlay->addLayout(hlay, 0);
    vlay->addWidget(myGLView, 1);

    setLayout(vlay);
}

void PersInterfaceWidget::warningInvalidValue()
{
    QMessageBox::warning( this,
                          "WARNING",
                          u8"入力された値が不正です。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}

void PersInterfaceWidget::showInformationMessage(const QString &msg)
{
    QMessageBox::information( this,
                              "Information",
                              msg,
                              QMessageBox::NoButton, QMessageBox::Ok );

}

QString PersInterfaceWidget::changeSelectedMembersSection(DATATYPE dtype, const QString &str)
{
    QList<MemberData *> select_members;
    if ( dtype != DATATYPE::TPWALLOPEN ) {
        Q_FOREACH ( MemberData *md, myGLView->getScene()->getSelectedMembers() ) {
            if ( md->dataType() == dtype ) select_members.append(md);
        }
    } else {
        Q_FOREACH ( MemberData *md, myGLView->getScene()->getSelectedMembers() ) {
            if ( md->dataType() == DATATYPE::TPWALL || md->dataType() == DATATYPE::TPUWALL ) select_members.append(md);
        }
    }
    if ( select_members.isEmpty() ) return u8"対象の部材が選択されていません。";
    myGLView->getScene()->clearSelectedItems();

    QString msg;
    if ( dtype == DATATYPE::TPWALLOPEN ) {
        msg = ( str.isEmpty() )
                ? QString(u8"%1個の壁について開口を削除しました。").arg(select_members.count())
                : QString(u8"%1個の壁について開口を変更しました。").arg(select_members.count());
    } else {
        msg = QString(u8"%1個の部材について符号を変更しました。").arg(select_members.count());
    }

    if ( dtype != DATATYPE::TPWALLOPEN ) {
        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeMemberSectionCommand(select_members, str));
    } else {
        UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeWallOpenCommand(select_members, str));
    }
    return msg;
}

void PersInterfaceWidget::removeSelectedJoint(JointData *jd)
{
    auto myElementType = UnifiedEditingData::getInstance()->currentElement();
    if ( myElementType != ALLELEMENT && myElementType != ELJOINT ) return;
    myGLView->getScene()->removeSelectedJoint(jd);
}

void PersInterfaceWidget::appendSelectedJoint(JointData *jd)
{
    auto myElementType = UnifiedEditingData::getInstance()->currentElement();
    if ( myElementType != ALLELEMENT && myElementType != ELJOINT ) return;
    if ( myElementType == ALLELEMENT && !isJointOkToSelectLimited(jd) ) return;
    myGLView->getScene()->appendSelectedJoint(jd);
}

void PersInterfaceWidget::appendSelectedMember(MemberData *md)
{
    auto myElementType = UnifiedEditingData::getInstance()->currentElement();
    if ( myElementType != ALLELEMENT && myElementType != dataTypeToElementType(md->dataType()) ) return;
    if ( myElementType == ALLELEMENT && !isMemberOkToSelectLimited(md) ) return;
    myGLView->getScene()->appendSelectedMember(md);
}

bool PersInterfaceWidget::isJointOkToSelectLimited(JointData *jd) const
{
    if ( !UnifiedEditingData::getInstance()->isSelectLimited() ) return true;
    if ( UnifiedEditingData::getInstance()->isSelectLimitedName() ) return false;

    LIMITSELECTTERM selectTerm =  UnifiedEditingData::getInstance()->currentLimitSelectTerm();

    if ( selectTerm.isLimitDataType ) {
        if ( !selectTerm.limitedDataTypes.contains(DATATYPE::TPJOINT) ) return false;
    }

    if ( selectTerm.isLimitFloor ) {
        bool ok = false;
        Q_FOREACH ( QUuid id, jd->getAttachedFloorList() ) {
            if ( selectTerm.limitedFloors.contains(id) ) {
                ok = true;
                break;
            }
        }
        if ( !ok ) return false;
    }
    if ( selectTerm.isLimitFrame ) {
        bool ok = false;
        Q_FOREACH ( QUuid id, jd->getAttachedFrameList() ) {
            if ( selectTerm.limitedFrames.contains(id) ) {
                ok = true;
                break;
            }
        }
        if ( !ok ) return false;
    }
    return true;
}

bool PersInterfaceWidget::isMemberOkToSelectLimited(MemberData *md) const
{
    if ( ! UnifiedEditingData::getInstance()->isSelectLimited() ) return true;

    LIMITSELECTTERM selectTerm =  UnifiedEditingData::getInstance()->currentLimitSelectTerm();

    if ( selectTerm.isLimitDataType ) {
        if ( !selectTerm.limitedDataTypes.contains( md->dataType() ) ) return false;
    }
    if ( selectTerm.isLimitName ) {
        if ( !selectTerm.limitedNames.contains( md->sectionName() ) ) return false;
    }
    if ( selectTerm.isLimitFloor ) {
        if ( selectTerm.isFloorContained ) {
            if ( !md->containsFloors(selectTerm.limitedFloors) ) return false;
        } else {
            if ( !md->touchFloors(selectTerm.limitedFloors) ) return false;
        }
    }
    if ( selectTerm.isLimitFrame ) {
        if ( selectTerm.isFrameContained ) {
            if ( !md->containsFrames(selectTerm.limitedFrames) ) return false;
        } else {
            if ( !md->touchFrames(selectTerm.limitedFrames) ) return false;
        }
    }
    return true;
}

void PersInterfaceWidget::slotAppendSelectedMemberID(GLuint id)
{
    auto currentAddEditMode = UnifiedEditingData::getInstance()->currentAddEditMode();
    if ( currentAddEditMode != EDITMODE ) return;
    int idx = myGLView->getScene()->getMemberDisplayList().indexOf(id);
    if ( idx < 0 ) return;
    appendSelectedMember( myGLView->getScene()->getSceneMembers().at(idx) );
}

ELEMENTTYPE PersInterfaceWidget::dataTypeToElementType(DATATYPE dtype) const
{
    ELEMENTTYPE etype;
    if (dtype == DATATYPE::TPJOINT) {
        etype = ELJOINT;
    } else if (dtype == DATATYPE::TPCOLUMN) {
        etype = ELCOLUMN;
    } else if (dtype == DATATYPE::TPGIRDER) {
        etype = ELGIRDER;
    } else if (dtype == DATATYPE::TPBRACE) {
        etype = ELBRACE;
    } else if (dtype == DATATYPE::TPBEAM) {
        etype = ELBEAM;
    } else if (dtype == DATATYPE::TPSLAB) {
        etype = ELSLAB;
    } else if (dtype == DATATYPE::TPWALL) {
        etype = ELWALL;
    } else if (dtype == DATATYPE::TPUWALL) {
        etype = ELUWALL;
    } else if (dtype == DATATYPE::TPDAMPER) {
        etype = ELDAMPER;
    } else if (dtype == DATATYPE::TPISO) {
        etype = ELISO;
    } else if (dtype == DATATYPE::TPSLABLOAD) {
        etype = ELSLABLOAD;
    } else if (dtype == DATATYPE::TPMEMBERLOAD) {
        etype = ELMEMBERLOAD;
    } else if (dtype == DATATYPE::TPJOINTLOAD) {
        etype = ELJOINTLOAD;
    }
    return etype;
}

void PersInterfaceWidget::slotSavePixmap()
{
    QString filePath = QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.png)" );
    if ( filePath.isEmpty() ) return;

    QSize sz = myGLView->size();
    qreal ratio = qreal(sz.height()) / qreal(sz.width());

    myGLView->setRenderMode(true);

    QMessageBox msg(QMessageBox::Information, "Information", u8"画像ファイルを作成中です。",
                    QMessageBox::NoButton, this);
    msg.show();
    qApp->setOverrideCursor( QCursor(Qt::WaitCursor) );
//    QPixmap pix = myGLView->renderPixmap(5000, qRound(5000.0 * ratio));
//    pix.save(filePath);
    QImage image = myGLView->grabFramebuffer();
    image.save(filePath);
    qApp->restoreOverrideCursor();
    msg.close();
    myGLView->setRenderMode(false);
}

void PersInterfaceWidget::slotShowControlDialog(bool onoff)
{
    if (!onoff) {
        controlDialog->hide();
        return;
    }
    controlDialog->setCenterPosition( myGLView->camera()->revolveAroundPoint().x,
                                      myGLView->camera()->revolveAroundPoint().y,
                                      myGLView->camera()->revolveAroundPoint().z );
    controlDialog->setCurrentValues();
    controlDialog->show();
}

void PersInterfaceWidget::slotChangeViewSettings()
{
    PERSSETTINGVALUES settings = controlDialog->currentViewSettings();
    myGLView->changeViewSettings(settings);
}

void PersInterfaceWidget::slotControlButtonUnChecked()
{
    controlButton->setChecked(false);
}

void PersInterfaceWidget::slotShowAutoAttachDialog()
{
    AutoAttachDialog dialog(this);
    if ( dialog.exec() != QDialog::Accepted )
        return;

    if ( myGLView->getScene()->getSelectedJoints().isEmpty() ){
        showInformationMessage(u8"節点が選択されていません。");
        return;
    }
    QList<JointData *> select_joints = myGLView->getScene()->getSelectedJoints();
    myGLView->getScene()->clearSelectedItems();
    UnifiedEditingData::getInstance()->pushUndoCommand(new CompleteJointsAttachmentCommand(select_joints, dialog.isOverwriteChecked()));
    showInformationMessage(QString(u8"%1個の節点に属性が付加されました。").arg(select_joints.count()));

}

void PersInterfaceWidget::slotShowChangeAngleDialog()
{
    QString str_angle = myGLView->getScene()->selectedMembersCodeAngle();
    if ( str_angle.isEmpty() ) {
        showInformationMessage(u8"部材が選択されていません。");
        return;
    }
    ChangeAngleDialog dialog(str_angle, this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString value_str = dialog.valueText();
    bool ok = true;
    qreal ag = value_str.toDouble(&ok);
    if ( value_str == "***" ) {
        showInformationMessage(u8"コードアングルは変更されませんでした。");
        return;
    }
    if ( !ok ) {
        warningInvalidValue();
        return;
    }

    qreal eps = 1.0e-5;
    bool inRange = ( ag > -180.0 - eps && ag < 180.0 + eps );
    while ( !inRange ) {
        if ( ag < -180.0 ) ag += 360.0;
        if ( ag > 180.0 ) ag -= 360.0;
        inRange = ( ag > -180.0 - eps && ag < 180.0 + eps );
    }

    QList<MemberData *> select_members;
    Q_FOREACH ( MemberData *md, myGLView->getScene()->getSelectedMembers() ) {
        if ( !md->isLineMember(false) ) continue;
        if ( md->dataType() == DATATYPE::TPDAMPER || md->dataType() == DATATYPE::TPISO ) continue;
        select_members.append(md);
    }
    if ( select_members.isEmpty() ){
        showInformationMessage(u8"対象の部材が選択されていません。");
        return;
    }
    myGLView->getScene()->clearSelectedItems();
    UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeCodeAngleCommand(select_members, ag * M_PI / 180.0));
    showInformationMessage(QString(u8"%1個の部材についてコードアングルを変更しました。").arg(select_members.count()));
}

void PersInterfaceWidget::slotShowChangeFaceDialog()
{
    QStringList strlist = myGLView->getScene()->selectedMembersFaceValue();
    if ( strlist.count() < 4 ) {
        showInformationMessage(u8"部材が選択されていません。");
        return;
    }
    int iid = strlist.at(0).toInt();
    int jid = strlist.at(2).toInt();
    ChangeFaceDialog dialog(iid, strlist.at(1), jid, strlist.at(3), this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    bool ir_chg = true, il_chg = true, jr_chg = true, jl_chg = true;
    qreal i_value, j_value;

    int i_id = dialog.ifaceRuleCheckedID();
    QString i_str = dialog.ifaceValueText();
    if ( i_id == 1 ) {
        bool ok = true;
        i_value = i_str.toDouble(&ok);
        if ( i_str == "***" ) {
            il_chg = false;
        } else if ( !ok ) {
            warningInvalidValue();
            return;
        }
    } else if ( i_id == 2 ) {
        ir_chg = false;
        il_chg = false;
    } else if ( i_id == 1 ) {
        il_chg = false;
    }

    int j_id = dialog.jfaceRuleCheckedID();
    QString j_str = dialog.jfaceValueText();
    if ( j_id == 1 ) {
        bool ok = true;
        j_value = j_str.toDouble(&ok);
        if ( j_str == "***" ) {
            jl_chg = false;
        } else if ( !ok ) {
            warningInvalidValue();
            return;
        }
    } else if ( j_id == 2 ) {
        jr_chg = false;
        jl_chg = false;
    } else if ( j_id == 1 ) {
        jl_chg = false;
    }

    QList<MemberData *> select_members;
    Q_FOREACH ( MemberData *md, myGLView->getScene()->getSelectedMembers() ) {
        if ( md->dataType() == DATATYPE::TPSLAB || md->dataType() == DATATYPE::TPWALL || md->dataType() == DATATYPE::TPUWALL ) continue;
        select_members.append(md);
    }
    if ( select_members.isEmpty() ){
        showInformationMessage(u8"対象の部材が選択されていません。");
        return;
    }

    myGLView->getScene()->clearSelectedItems();
    UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeFaceCommand(select_members, ir_chg, i_id, il_chg, i_value, jr_chg, j_id, jl_chg, j_value));
    showInformationMessage(QString(u8"%1個の部材についてフェイス長さを変更しました。").arg(select_members.count()));
}

void PersInterfaceWidget::slotShowChangeJointDialog()
{
    QStringList xyz_str = myGLView->getScene()->selectedJointsXYZStrings();
    int jcount = xyz_str.first().toInt();
    if ( jcount == 0 ) {
        showInformationMessage(u8"節点が選択されていません。");
        return;
    }
    ChangeGlobalJointDialog dialog( xyz_str.at(1), xyz_str.at(2), xyz_str.at(3), this );
    if ( dialog.exec() == QDialog::Accepted ) {
        showInformationMessage( QString("%1").arg(jcount) + u8"個の節点を変更しました。" );
    }
}

void PersInterfaceWidget::slotChangeWallOpen()
{
    ChangeOpenDialog dialog(this,UnifiedDataType::Input);
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString str = dialog.currentName();
    QString msg = changeSelectedMembersSection(DATATYPE::TPWALLOPEN, str);
    showInformationMessage(msg);
}

void PersInterfaceWidget::slotChangeSectionDialog()
{
    ChangeSectionDialog dialog(this,UnifiedDataType::Input);
    if ( dialog.exec() == QDialog::Accepted ) {
        DATATYPE dtype = dialog.currentDataType();
        QString str = dialog.currentName();
        QString msg = changeSelectedMembersSection(dtype, str);
        showInformationMessage(msg);
    }
}

void PersInterfaceWidget::slotShowChangeShiftDialog()
{
    QStringList strlist = myGLView->getScene()->selectedMembersShiftValue();
    if ( strlist.count() < 2 ) {
        showInformationMessage(u8"部材が選択されていません。");
        return;
    }
    ChangeShiftDialog dialog(strlist.at(0), strlist.at(1), this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    QString value_shy = dialog.shiftYText();
    QString value_shz = dialog.shiftZText();

    bool y_chg = true, z_chg = true, ok = true;
    qreal shy = value_shy.toDouble(&ok);
    if ( value_shy == "***" ) {
        y_chg = false;
        ok = true;
    }
    if ( !ok ) {
        warningInvalidValue();
        return;
    }
    qreal shz = value_shz.toDouble(&ok);
    if ( value_shz == "***" ) {
        z_chg = false;
        ok = true;
    }
    if ( !ok ) {
        warningInvalidValue();
        return;
    }

    QList<MemberData *> select_members;
    Q_FOREACH ( MemberData *md, myGLView->getScene()->getSelectedMembers() ) {
        if ( md->dataType() == DATATYPE::TPDAMPER || md->dataType() == DATATYPE::TPISO ) continue;
        select_members.append(md);
    }
    if ( select_members.isEmpty() ){
        showInformationMessage(u8"対象の部材が選択されていません。");
        return;
    }

    myGLView->getScene()->clearSelectedItems();
    UnifiedEditingData::getInstance()->pushUndoCommand(new ChangeShiftValueCommand(select_members, y_chg, shy, z_chg, shz));
    showInformationMessage(QString(u8"%1個の部材について寄り長さを変更しました。").arg(select_members.count()));
}

void PersInterfaceWidget::slotFindItems()
{
    if ( UnifiedEditingData::getInstance()->currentAddEditMode() != EDITMODE ) return;

    FindItemDialog dialog( myGLView->getScene()->currentDataType(), this ,UnifiedDataType::Input);

    if ( dialog.exec() == QDialog::Accepted ) {

        myGLView->getScene()->clearSelectedItems();
        slotSendSelectedItems();

        QString msg = dialog.checkDialogInput();
        if ( !msg.isEmpty() ) {
            showInformationMessage(msg);
            return;
        }

        FINDDIALOGSTATUS fst = dialog.dialogStatus();
        QList<JointData *> jdlist;
        QList<MemberData *> mdlist;
        DATATYPE dtype = myGLView->getScene()->currentDataType();

        // 節点のサーチ
        if ( fst.isJflChecked || fst.isJfrChecked || fst.isJnoChecked ) {

            if ( dtype != DATATYPE::TPJOINT && dtype != DATATYPE::NODATATYPE ) {
                showInformationMessage(u8"現在の編集モードでは節点を選択できません。");
                return;
            }

            jdlist = myGLView->getScene()->getSceneJoints();

            for ( int i = jdlist.count() - 1; i >= 0; i-- ) {
                JointData *jd = jdlist.at(i);
                if ( fst.isJflChecked && !jd->containsFloor( fst.jFloors ) ) {
                    jdlist.removeAt(i);
                    continue;
                }
                if ( fst.isJfrChecked && !jd->containsFrame( fst.jFrames ) ) {
                    jdlist.removeAt(i);
                    continue;
                }
                if ( fst.isJnoChecked ) {
                    int idx =  UnifiedInputData::getInInstance()->indexOfJoint(jd) + 1;
                    if ( idx < fst.jnoFrom || idx > fst.jnoTo ) {
                        jdlist.removeAt(i);
                        continue;
                    }
                }
            }

        }

        // 部材のサーチ
        if ( fst.isMtpChecked || fst.isMstChecked || fst.isMflChecked || fst.isMfrChecked ) {

            if ( dtype == DATATYPE::TPJOINT ) {
                showInformationMessage(u8"現在の編集モードでは部材・荷重を選択できません。");
                return;
            }

            if ( dtype != DATATYPE::NODATATYPE && fst.isMtpChecked && dtype != fst.dataType ) {
                showInformationMessage(u8"現在の編集モードでは指定の要素を選択できません。");
                return;
            }

            mdlist = myGLView->getScene()->getSceneMembers();

            for ( int i = mdlist.count() - 1; i >= 0; i-- ) {
                MemberData *md = mdlist.at(i);
                if ( fst.isMtpChecked && md->dataType() != fst.dataType ) {
                    mdlist.removeAt(i);
                    continue;
                }
                if ( fst.isMstChecked ) {
                    if ( fst.mstMatch && md->sectionName() != fst.memberName ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                    if ( !fst.mstMatch && !md->sectionName().contains( fst.memberName ) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                }
                if ( fst.isMflChecked ) {
                    if ( fst.mflMatch && !md->containsFloors(fst.mFloors) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                    if ( !fst.mflMatch && !md->touchFloors(fst.mFloors) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                }
                if ( fst.isMfrChecked ) {
                    if ( fst.mfrMatch && !md->containsFrames(fst.mFrames) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                    if ( !fst.mfrMatch && !md->touchFrames(fst.mFrames) ) {
                        mdlist.removeAt(i);
                        continue;
                    }
                }
            }
        }

        if ( jdlist.isEmpty() && mdlist.isEmpty() ) {
            showInformationMessage(u8"指定の要素は見つかりませんでした。");
            return;
        }

        myGLView->getScene()->setJointSelect( jdlist );
        myGLView->getScene()->setMemberSelect( mdlist );
        slotSendSelectedItems();
    }
}

void PersInterfaceWidget::slotDeleteItems()
{
    if ( myGLView->getScene()->getSelectedMembers().isEmpty() && myGLView->getScene()->getSelectedJoints().isEmpty() ){
        showInformationMessage(u8"節点または部材が選択されていません。");
        return;
    }
    QList<MemberData *> select_members = myGLView->getScene()->getSelectedMembers();
    QList<JointData *> select_joints = myGLView->getScene()->getSelectedJoints();

    int j_count = 0;
    int m_count = 0;
    j_count += select_joints.count();
    m_count += select_members.count();

    myGLView->getScene()->clearSelectedItems();

    UnifiedEditingData::getInstance()->beginUndoStackMacro("delete Joints And Members");

    if ( UnifiedEditingData::getInstance()->currentDeleteAction() == REMAINJOINT ) {
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( select_joints ));

        for ( int i = select_members.count() - 1; i >= 0; i-- ) {
            MemberData *md = select_members.at(i);
            if ( ! UnifiedInputData::getInInstance()->isExistedMember(md) ) select_members.removeAt(i);
        }
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveMembersCommand( select_members ));

    } else {
        QList<JointData *> relateJoints;
        Q_FOREACH ( JointData *jd, select_joints ) {
            Q_FOREACH ( JointData *jjd, jd->linkedJoints() ) {
                if ( !select_joints.contains(jjd) && !relateJoints.contains(jjd) )
                    relateJoints.append(jjd);
            }
        }
        Q_FOREACH ( MemberData *md, select_members ) {
            Q_FOREACH ( JointData *jjd, md->getJointList() ) {
                if ( !select_joints.contains(jjd) && !relateJoints.contains(jjd) )
                    relateJoints.append(jjd);
            }
        }

        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( select_joints ));
        for ( int i = select_members.count() - 1; i >= 0; i-- ) {
            MemberData *md = select_members.at(i);
            if ( ! UnifiedInputData::getInInstance()->isExistedMember(md) ) select_members.removeAt(i);
        }
        UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveMembersCommand(select_members));

        QList<JointData *> delJoints;
        Q_FOREACH ( JointData *jd, relateJoints ) {
            if ( jd->isIndependentJoint() ) delJoints.append(jd);
        }
        if ( !delJoints.isEmpty() ) {
            j_count += delJoints.count();
            UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand(delJoints));
        }
    }

    UnifiedEditingData::getInstance()->endUndoStackMacro();
    showInformationMessage(QString(u8"%1個の節点、%2個の部材を削除しました。").arg(j_count).arg(m_count));
}

void PersInterfaceWidget::slotDeleteIndependentJoints()
{
    QList<JointData *> delJoints;
    Q_FOREACH ( JointData *jd, myGLView->getScene()->getSelectedJoints()) {
        if ( jd->isIndependentJoint() ) delJoints.append(jd);
    }
    myGLView->getScene()->clearSelectedItems();

    if ( delJoints.isEmpty() ){
        showInformationMessage(u8"選択された節点に独立節点は見つかりませんでした。");
        return;
    }

    UnifiedEditingData::getInstance()->pushUndoCommand(new RemoveJointsCommand( delJoints ));
    showInformationMessage(QString(u8"%1個の独立節点を削除しました。").arg(delJoints.count()));
}

void PersInterfaceWidget::slotReverseMemberJoint()
{
    QList<MemberData *> select_members = myGLView->getScene()->getSelectedMembers();
    if ( select_members.isEmpty() ) {
        showInformationMessage(u8"部材（線材）が選択されていません。");
        return;
    }
    myGLView->getScene()->clearSelectedItems();

    UnifiedEditingData::getInstance()->pushUndoCommand(new ReverseMemberJointCommand(select_members));
    showInformationMessage(QString(u8"%1個の部材についてI,J端が入れ替わりました。").arg(select_members.count()));
}

void PersInterfaceWidget::slotDeleteWallOpen()
{
    QString msg = changeSelectedMembersSection(DATATYPE::TPWALLOPEN, QString());
    showInformationMessage(msg);
}

void PersInterfaceWidget::slotSetActionStatus(const QList<DATATYPE>& dType)
{
    if ( UnifiedEditingData::getInstance()->currentAddEditMode() != EDITMODE )
        return;

    manageActions->setActionStatus( dType );
    manageActions->showContextMenu();
}

void PersInterfaceWidget::slotImportCurrentStatus()
{
    ELEMENTTYPE el_type = UnifiedEditingData::getInstance()->currentElement();
    ADDEDITMODE ad_mode = UnifiedEditingData::getInstance()->currentAddEditMode();
    myGLView->getScene()->importCurrentStatus(el_type, ad_mode);
}

void PersInterfaceWidget::slotSendSelectedItems()
{
    auto selectedJoints = myGLView->getScene()->getSelectedJoints();
    auto selectedMembers = myGLView->getScene()->getSelectedMembers();
    if ( ( selectedJoints.isEmpty() && selectedMembers.isEmpty() )
            &&  UnifiedEditingData::getInstance()->isNoActiveElements() ) return;

    UnifiedEditingData::getInstance()->changeActiveElements( selectedJoints, selectedMembers );
}

void PersInterfaceWidget::slotSelectPoints(qglviewer::Camera *camera, const QRect &rect, PERSSELECTMODE mode)
{
    if ( UnifiedEditingData::getInstance()->currentAddEditMode() != EDITMODE ) return;

    QList<JointData *> tmp_jdlist;
    QList<MemberData *> tmp_mdlist;

    Q_FOREACH ( JointData *jd, myGLView->getScene()->getSceneJoints() ) {
        qglviewer::Vec sp = camera->projectedCoordinatesOf(
                     qglviewer::Vec( jd->xPos() * 1000.0, jd->yPos() * 1000.0, jd->zPos() * 1000.0 ) );
        QPoint pp(qRound(sp.x), qRound(sp.y));
        if ( rect.contains(pp) ) {
            if ( mode == PERSSELECTMODE::PERS_ADDSELECT )
                appendSelectedJoint(jd);
            if ( mode == PERSSELECTMODE::PERS_REMOVESELECT )
                removeSelectedJoint(jd);
            if ( !tmp_jdlist.contains(jd) ) tmp_jdlist.append(jd);
            QList<MemberData *> mdlist = jd->getRelatedMembers();
            Q_FOREACH ( MemberData *md, mdlist ) {
                if ( md->dataType() == DATATYPE::TPJOINTLOAD || md->dataType() == DATATYPE::TPMEMBERLOAD
                        || md->dataType() == DATATYPE::TPSLABLOAD ) continue;
                if ( !tmp_mdlist.contains(md) ) tmp_mdlist.append(md);
            }
        }
    }

    Q_FOREACH ( MemberData *md, tmp_mdlist) {
        QList<JointData *> jdlist = md->getJointList();
        bool isContain = true;
        Q_FOREACH ( JointData *m_jd, jdlist ) {
            if ( !tmp_jdlist.contains(m_jd) ) {
                isContain = false;
                break;
            }
        }
        if ( isContain ) {
            if ( mode == PERSSELECTMODE::PERS_ADDSELECT ) appendSelectedMember(md);
            if ( mode == PERSSELECTMODE::PERS_REMOVESELECT ) myGLView->getScene()->removeSelectedMember(md);
        }
    }

    emit myGLView->signalSendSelectedItems();
}

} // namespace post3dapp
