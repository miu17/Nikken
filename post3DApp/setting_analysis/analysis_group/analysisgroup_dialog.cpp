#include <QtGui>
#include <QDebug>

#include <QPushButton>

#include "analysisgroup_dialog.h"
#include "vhline.h"

#include "fileread_dialog.h"
#include "unified_data.h"
#include "unified_inputdata.h"
#include "unified_project.h"
#include "unified_settings.h"

#include "reference_combobox.h"



namespace post3dapp{

AnalysisGroupDialog::AnalysisGroupDialog( QWidget* parent )
    : QDialog( parent )
{

    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint
                         | Qt::WindowCloseButtonHint);
    this->setFont(QFont(QString::fromLocal8Bit("Microsoft JhengHei"),10));

    createButtons();
    createLayouts();
    connect(rangeJointTableWidget, &AnalysisJointRangeTableWidget::signalAllocateCurrentJoints,
            this, &AnalysisGroupDialog::slotAllocateJointTable);
    connect(rangeJointTableWidget, &AnalysisJointRangeTableWidget::signalAdditionallyAllocateCurrentJoints,
            this, &AnalysisGroupDialog::slotAdditionallyAllocateJointTable);
    connect(rangeJointTableWidget, &AnalysisJointRangeTableWidget::signalConfirmTableJoints,
            this, &AnalysisGroupDialog::sendJointItemsTableToDraw);

    connect(rangeMemberTableWidget, &AnalysisMemberRangeTableWidget::signalAllocateCurrentMembers,
            this, &AnalysisGroupDialog::slotAllocateMemberTable);
    connect(rangeMemberTableWidget, &AnalysisMemberRangeTableWidget::signalAdditionallyAllocateCurrentMembers,
            this, &AnalysisGroupDialog::slotAdditionallyAllocateMemberTable);
    connect(rangeMemberTableWidget, &AnalysisMemberRangeTableWidget::signalConfirmTableMembers,
            this, &AnalysisGroupDialog::sendMemberItemsTableToDraw);


    connect(rangeJointTableWidget, &AnalysisRangeTableWidget::signalTableItemChanged,
            this, &AnalysisGroupDialog::slotTableItemChanged);
    connect(rangeMemberTableWidget, &AnalysisRangeTableWidget::signalTableItemChanged,
            this, &AnalysisGroupDialog::slotTableItemChanged);
    // TODO:片方が選択されるともう片方の選択は外したい



    this->setDataModified(false);
}

void AnalysisGroupDialog::createButtons()
{
    upload_button = new QPushButton(u8"登　録",this);
    upload_button->setFixedSize(80,30);
    upload_button->setAutoDefault(false);
    connect(upload_button, &QPushButton::clicked, this, &AnalysisGroupDialog::slotModelUpload);

    rewind_button = new QPushButton(u8"復　元",this);
    rewind_button->setFixedSize(80,30);
    rewind_button->setAutoDefault(false);
    rewind_button->setEnabled(false);
    connect (rewind_button, &QPushButton::clicked, this, &AnalysisGroupDialog::slotModelRewind);

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(upload_button);
    buttonsLayout->addWidget(rewind_button);
}

void AnalysisGroupDialog::createLayouts()
{
    myViewWidget=new AnalysisGroupGLViewer(new AnalysisGroupGLScene(), this);

    rangeJointTableWidget=new AnalysisJointRangeTableWidget(
                AnalysisJointRangeDataType(),
                UnifiedProject::getInstance()->getAnalyisisJointGroup(),
                this);

    rangeMemberTableWidget = new AnalysisMemberRangeTableWidget(
                AnalysisMemberRangeDataType(),
                UnifiedProject::getInstance()->getAnalyisisMemberGroup(),
                this);

    /* TODO?　行選択された場合に選択する
    connect(rangeJointTableWidget, &AnalysisRangeTableWidget::signalCurrentRowChanged,
            this, &AnalysisGroupDialog::sendJointItemsTableToDraw);
    connect(rangeMemberTableWidget, &AnalysisRangeTableWidget::signalCurrentRowChanged,
            this, &AnalysisGroupDialog::sendMemberItemsTableToDraw);
    */

    ReferenceCombobox* floorCombobox=new ReferenceCombobox(DATATYPE::TPFLOOR);
    floorCombobox->setDefaultName(u8"（階の選択）");
    floorCombobox->setFixedWidth(110);
    connect(floorCombobox, QOverload<const QString &>::of(&QComboBox::currentTextChanged), myViewWidget, &AnalysisGroupGLViewer::slotChangeLimitedFloor);

    ReferenceCombobox* frameCombobox=new ReferenceCombobox(DATATYPE::TPFRAMEP);
    frameCombobox->setDefaultName(u8"（通りの選択）");
    frameCombobox->setFixedWidth(110);
    connect(frameCombobox, QOverload<const QString &>::of(&QComboBox::currentTextChanged), myViewWidget, &AnalysisGroupGLViewer::slotChangeLimitedFrame);

    QComboBox* elementCombobox=new QComboBox(this);
    elementCombobox->setFixedWidth(110);

    QStringList strlist1;
    strlist1 << u8"（選択要素）" << u8"節点" << u8"柱" << u8"大梁"
             << u8"ブレース" << u8"壁" << u8"制振要素" << u8"免震要素";
    elementCombobox->addItems(strlist1);
    connect(elementCombobox, QOverload<const QString &>::of(&QComboBox::currentTextChanged),
            this, &AnalysisGroupDialog::slotChangeCurrentElement);

    QToolButton* zoomInButton=new QToolButton(this);
    zoomInButton->setToolTip(u8"表示拡大");
    zoomInButton->setAutoRaise(true);
    zoomInButton->setIcon(QIcon(QPixmap(":/icons/zoomin.png")));
    zoomInButton->setIconSize(QSize(25,25));
    zoomInButton->setFixedSize(30,30);
    zoomInButton->setAutoRepeat(true);
    zoomInButton->setAutoRepeatInterval(33);
    zoomInButton->setAutoRepeatDelay(0);
    connect (zoomInButton, &QToolButton::clicked, myViewWidget, &AnalysisGroupGLViewer::zoomIn);

    QToolButton* zoomOutButton=new QToolButton(this);
    zoomOutButton->setToolTip(u8"表示縮小");
    zoomOutButton->setAutoRaise(true);
    zoomOutButton->setIcon(QIcon(QPixmap(":/icons/zoomout.png")));
    zoomOutButton->setIconSize(QSize(25,25));
    zoomOutButton->setFixedSize(30,30);
    zoomOutButton->setAutoRepeat(true);
    zoomOutButton->setAutoRepeatInterval(33);
    zoomOutButton->setAutoRepeatDelay(0);
    connect (zoomOutButton, &QToolButton::clicked, myViewWidget, &AnalysisGroupGLViewer::zoomOut);

    QToolButton* fitButton=new QToolButton(this);
    fitButton->setToolTip(u8"画面フィッティング");
    fitButton->setAutoRaise(true);
    fitButton->setIcon(QIcon(QPixmap(":/icons/fit.png")));
    fitButton->setFixedSize(30,30);
    fitButton->setIconSize(QSize(25,25));
    connect (fitButton, &QToolButton::clicked, myViewWidget, &AnalysisGroupGLViewer::fitWindow);

    QToolButton* controlButton=new QToolButton(this);
    controlButton->setAutoRaise(true);
    controlButton->setIcon(QIcon(QPixmap(":/icons/control.png")));
    controlButton->setToolTip(u8"表示設定");
    controlButton->setIconSize(QSize(30,30));
    controlButton->setFixedSize(30,30);
    controlButton->setCheckable(true);
    controlButton->setChecked(false);
    // TODO

    QHBoxLayout *hlay1=new QHBoxLayout;
    hlay1->addWidget(floorCombobox);
    hlay1->addWidget(frameCombobox);
    hlay1->addWidget(elementCombobox);
    hlay1->addStretch();
    hlay1->addWidget(zoomInButton);
    hlay1->addWidget(zoomOutButton);
    hlay1->addWidget(fitButton);
    hlay1->addWidget(new VFLine(Qt::lightGray,this));
    hlay1->addWidget(controlButton);

    QVBoxLayout* vlay1=new QVBoxLayout(this);
    vlay1->setContentsMargins(0,0,0,0);
    vlay1->setSpacing(5);
    vlay1->addWidget(myViewWidget,1);
    vlay1->addLayout(hlay1,0);

    QWidget *wid=new QWidget(this);
    wid->setLayout(vlay1);

    QSplitter *splv=new QSplitter(Qt::Vertical,this);
    splv->addWidget(rangeJointTableWidget);
    splv->addWidget(rangeMemberTableWidget);
    splv->setStretchFactor(0,1);
    splv->setStretchFactor(1,2);

    QSplitter *splh=new QSplitter(Qt::Horizontal,this);
    splh->addWidget(splv);
    splh->addWidget(wid);
    splh->setStretchFactor(0,2);
    splh->setStretchFactor(1,1);

    QVBoxLayout *vlay=new QVBoxLayout(this);
    vlay->setContentsMargins(10,10,10,10);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(splh);
    this->setLayout(vlay);
}

void AnalysisGroupDialog::clearData()
{
    setDataModified(false);
}

void AnalysisGroupDialog::reject()
{
    ( UnifiedProject::getInstance()->getAnalyisisJointGroup().count() > 0 ||
      UnifiedProject::getInstance()->getAnalyisisMemberGroup().count() > 0) ?
       emit statusChanged(CBSTATUS::CB_DEFINE) : emit statusChanged(CBSTATUS::CB_NORMAL) ;
    QDialog::reject();
}

void AnalysisGroupDialog::slotModelUpload()
{
    if (!checkData()){
        // Message
        emit statusChanged(CBSTATUS::CB_ACTIVE);
        return;
    }

    UnifiedProject::getInstance()->registerTempAnalysisGroup(
                rangeJointTableWidget->getData(),
                rangeMemberTableWidget->getData());

    setDataModified(false);
}

void AnalysisGroupDialog::slotModelRewind()
{
    if( informationRewind() ) return;
    rangeJointTableWidget->modelRewind(UnifiedProject::getInstance()->getAnalyisisJointGroup());
    rangeMemberTableWidget->modelRewind(UnifiedProject::getInstance()->getAnalyisisMemberGroup());
    setDataModified(false);
}

void AnalysisGroupDialog::slotChangeCurrentElement(const QString & str)
{
    ELEMENTTYPE my_el=ALLELEMENT;
    if( str==u8"節点" ){ my_el=ELJOINT; }
    else if( str==u8"柱" ){ my_el=ELCOLUMN; }
    else if( str==u8"大梁" ){ my_el=ELGIRDER; }
    else if( str==u8"ブレース" ){ my_el=ELBRACE; }
    else if( str==u8"壁" ){ my_el=ELWALL; }
    else if( str==u8"制振要素" ){ my_el=ELDAMPER; }
    else if( str==u8"免震要素" ){ my_el=ELISO; }
    myViewWidget->setLimitedElement(my_el);
}

void AnalysisGroupDialog::sendJointItemsTableToDraw()
{
    QList<JointData*> jdlist=rangeJointTableWidget->getSelectedJoints();
    myViewWidget->setSelectedJointItems(jdlist);
}

void AnalysisGroupDialog::sendMemberItemsTableToDraw()
{
    QList<MemberData*> mdlist=rangeMemberTableWidget->getSelectedMembers();
    myViewWidget->setSelectedMemberItems(mdlist);
    myViewWidget->raise();
}

void AnalysisGroupDialog::slotTableItemChanged()
{
    // 空行の場合は変更なし
    if (checkJointChanged() || checkMemberChanged()){
        setDataModified(true);
    }
    else{
        setDataModified(false);
    }
}

void AnalysisGroupDialog::slotAllocateJointTable()
{
    QList<JointData*> jdlist=rangeJointTableWidget->getSelectedJoints();
    auto joints = myViewWidget->currentSelectedJoints();
    rangeJointTableWidget->setSelectedItems(joints);
    sendJointItemsTableToDraw();
}

void AnalysisGroupDialog::slotAdditionallyAllocateJointTable()
{
    auto holdJoints = rangeJointTableWidget->getSelectedJoints();
    auto joints = myViewWidget->currentSelectedJoints();
    QList<JointData*> ret(holdJoints);
    foreach(auto j, joints){
        if (holdJoints.contains(j))
            continue;
        ret.append(j);
    }
    rangeJointTableWidget->setSelectedItems(ret);
    sendJointItemsTableToDraw();
}

void AnalysisGroupDialog::slotAllocateMemberTable()
{
    auto members = myViewWidget->currentSelectedMembers();
    rangeMemberTableWidget->setSelectedItems(members);
    sendMemberItemsTableToDraw();
}

void AnalysisGroupDialog::slotAdditionallyAllocateMemberTable()
{
    auto holdMembers = rangeMemberTableWidget->getSelectedMembers();
    auto members = myViewWidget->currentSelectedMembers();
    QList<MemberData*> ret(holdMembers);
    foreach(auto m, members){
        if (holdMembers.contains(m))
            continue;
        ret.append(m);
    }
    rangeMemberTableWidget->setSelectedItems(ret);
    sendMemberItemsTableToDraw();
}

void AnalysisGroupDialog::setDataModified(bool onoff)
{
    if(onoff){
        upload_button->setEnabled(true);
        rewind_button->setEnabled(true);
        this->setWindowTitle(u8"解析用グループ定義    *** 編集中 ***");
        this->setWindowIcon(QIcon(":/icons/c2_green.png"));
        emit statusChanged(CBSTATUS::CB_ACTIVE);
    }else{
        upload_button->setEnabled(false);
        rewind_button->setEnabled(false);
        this->setWindowTitle(u8"解析用グループ定義");
        this->setWindowIcon(QIcon(":/icons/c2_blue.png"));
        ( UnifiedProject::getInstance()->getAnalyisisJointGroup().count() > 0
       || UnifiedProject::getInstance()->getAnalyisisMemberGroup().count() > 0) ?
           emit statusChanged(CBSTATUS::CB_DEFINE) : emit statusChanged(CBSTATUS::CB_NORMAL) ;
    }
}

bool AnalysisGroupDialog::informationUpdate()
{
    int ret = QMessageBox::information(this, u8"CONFIRM",
                                   u8"前回登録時のデータは失われます。\n現データを登録しますか？",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Ok );
    return (ret==QMessageBox::Cancel);
}

bool AnalysisGroupDialog::informationRewind()
{
    int ret = QMessageBox::information(this, u8"CONFIRM",
                                   u8"編集中のデータは失われます。\n前回登録時の状態を復元しますか？",
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Ok );
    return (ret==QMessageBox::Cancel);
}

bool AnalysisGroupDialog::checkData()
{
    if (!rangeJointTableWidget->checkData()){
        return false;
    }

    if (!rangeMemberTableWidget->checkData()){
        return false;
    }

    return true;
}

bool AnalysisGroupDialog::checkJointChanged() const
{
    auto preJoints = UnifiedProject::getInstance()->getAnalyisisJointGroup();
    auto joints = rangeJointTableWidget->getData();
    if (preJoints.count() != joints.count()){
        return true;
    }else{
        for (int i = 0; i < preJoints.count(); i++){
            if (!preJoints.at(i).isSame(joints.at(i)))
                return true;
        }
    }

    return false;
}

bool AnalysisGroupDialog::checkMemberChanged() const
{
    auto preMembers = UnifiedProject::getInstance()->getAnalyisisMemberGroup();
    auto members = rangeMemberTableWidget->getData();
    if (preMembers.count() != members.count()){
        return true;
    }else{
        for (int i = 0; i < preMembers.count(); i++){
            if (!preMembers.at(i).isSame(members.at(i)))
                return true;
        }
    }

    return false;
}

} // namespace post3dapp
