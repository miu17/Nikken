#include "framesetting_dialog.h"

#include <QtGui>
#include <QDebug>

#include "app_version.h"
#include "define_framesettingtable.h"
#include "fileread_dialog.h"
#include "framesetting_drawwidget.h"
#include "framesetting_tablewidget.h"
#include "menu_button.h"
#include "unified_data.h"
#include "unified_inputdata.h"
#include "utility.h"
#include "vhline.h"


namespace post3dapp{

FrameSettingDialog::FrameSettingDialog( QWidget *parent )
    : QDialog( parent )
{
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont( QFont(tr("Microsoft JhengHei"), 10) );

//    QPalette pal = this->palette();
//    pal.setBrush(QPalette::Window,QBrush(QColor(240,240,255)));
//    this->setPalette(pal);

    createButtons();
    this->setDataModified(false);

    FrameDataType my_type;
    myTableWidget = new FrameSettingTableWidget(my_type, this);
    myDrawWidget = new FrameSettingDrawWidget(this);
    drawLinkStatus = true;

    connect ( myTableWidget, &FrameSettingTableWidget::frameItemChanged,
              this, &FrameSettingDialog::updateDrawWidget );
    connect ( myTableWidget, &FrameSettingTableWidget::currentGroupChanged,
              this, &FrameSettingDialog::changeCurrentGroupOfDrawWidget );
    connect ( myDrawWidget, &FrameSettingDrawWidget::linkStatusChanged,
              this, &FrameSettingDialog::changeDrawingStatus );

    QSplitter *splh = new QSplitter(Qt::Horizontal, this);
    splh->addWidget(myTableWidget);
    splh->addWidget(myDrawWidget);
    splh->setStretchFactor(0, 2);
    splh->setStretchFactor(1, 1);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(10, 10, 10, 10);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(splh);
    this->setLayout(vlay);

    myTableWidget->setMinimumWidth(120);
    myDrawWidget->setMinimumWidth(500);
}

void FrameSettingDialog::createButtons()
{
    QPushButton *open_button = new QPushButton(u8"ファイル読込", this);
    open_button->setFixedSize(90, 30);
    open_button->setAutoDefault(false);
    QPushButton *save_button = new QPushButton(u8"ファイル保存", this);
    save_button->setFixedSize(90, 30);
    save_button->setAutoDefault(false);

    upload_button = new QPushButton(u8"登　録", this);
    upload_button->setFixedSize(80, 30);
    upload_button->setAutoDefault(false);
    rewind_button = new QPushButton(u8"復　元", this);
    rewind_button->setFixedSize(80, 30);
    rewind_button->setAutoDefault(false);
    edit_button = new QPushButton(u8"編 集", this);
    edit_button->setFixedSize(80, 30);
    edit_button->setAutoDefault(false);

    rewind_button->setEnabled(false);
    edit_button->setEnabled(false);

    connect ( open_button, &QAbstractButton::clicked, this, &FrameSettingDialog::slotTextFileOpen );
    connect ( save_button, &QAbstractButton::clicked, this, &FrameSettingDialog::slotTextFileSave );

    connect ( upload_button, &QAbstractButton::clicked, this, &FrameSettingDialog::slotModelUpload );
    connect ( rewind_button, &QAbstractButton::clicked, this, &FrameSettingDialog::slotModelRewind );
    connect ( edit_button, &QAbstractButton::clicked, this, &FrameSettingDialog::slotModelEdit );

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(5);
    buttonsLayout->addWidget(open_button);
    buttonsLayout->addWidget(save_button);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(upload_button);
    buttonsLayout->addWidget(new VFLine(Qt::lightGray, this));
    buttonsLayout->addWidget(rewind_button);
    buttonsLayout->addWidget(edit_button);

}

bool FrameSettingDialog::checkBinaryFile ( QDataStream &infile, const AppVersion & ) const
{
    return myTableWidget->checkBinaryFile(infile);
}

void FrameSettingDialog::readBinaryFile ( QDataStream &infile, const AppVersion & )
{
    myTableWidget->readBinaryFile(infile);
    UnifiedInputData::getInInstance()->registerTempData(myTableWidget->getFrameGroupList(),
                                                 myTableWidget->getFramePointList());
    myTableWidget->modelUpload();
    updateDrawWidget();
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
    setDataModified(false);
}
void FrameSettingDialog::readTextFile ( QTextStream &infile, const AppVersion &var)
{
    myTableWidget->readTextFile(infile,true);
    UnifiedInputData::getInInstance()->registerTempData(myTableWidget->getFrameGroupList(),
                                                 myTableWidget->getFramePointList());
    myTableWidget->modelUpload();
    updateDrawWidget();
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
    setDataModified(false);
}

void FrameSettingDialog::reflectUnifiedData(){
    myTableWidget->reflectUnifiedData();
    myTableWidget->modelUpload();

    updateDrawWidget();
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
    setDataModified(false);
}

void FrameSettingDialog::writeBinaryFile ( QDataStream &outfile)
{
    myTableWidget->writeBinaryFile(outfile);
}
void FrameSettingDialog::writeTextFile ( QTextStream &outfile, bool writeUuid)
{
    myTableWidget->writeTextFile(outfile, writeUuid);
}

void FrameSettingDialog::clearData()
{
    myTableWidget->clearData();
    myTableWidget->modelUpload();
    updateDrawWidget();
    rewind_button->setEnabled(false);
    edit_button->setEnabled(false);
    setDataModified(false);
}

void FrameSettingDialog::slotTextFileOpen()
{
    QString filename =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.txt)" );
    if ( filename.isEmpty() ) return;

    QFile file(filename);
    if ( !file.open(QIODevice::ReadOnly) ) {
        Utility::messageFileOpenError(this);
        return;
    }

    FileReadDialog dialog(this);
    if (!dialog.exec()) return;
    bool state = dialog.overWriteButtonState();

    QTextStream in(&file);

    bool ok1 = myTableWidget->checkTextFile(in);
    if ( !ok1 ) {
        Utility::messageFormatError(this);
        return;
    }
    in.seek(0);
    myTableWidget->readTextFile(in, state);
    edit_button->setEnabled(false);
    setDataModified(true);
}

void FrameSettingDialog::slotTextFileSave()
{
    QString filename =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.txt)" );
    if ( filename.isEmpty() ) return;
    QFile file(filename);
    if ( !file.open(QIODevice::WriteOnly) ) {
        Utility::messageFormatError(this);
        return;
    }

    QTextStream outs(&file);

    myTableWidget->writeTextFile(outs, false);
}

void FrameSettingDialog::slotModelUpload()
{
    QList<FRAMEGROUPVALUE> groupList = myTableWidget->getFrameGroupList();
    QList<FRAMEPOINTSVALUE> frameList = myTableWidget->getFramePointList();
    if ( !UnifiedInputData::getInInstance()->checkTempData(this, groupList, frameList) ) return;

    if ( Utility::informationUpdate(this) ) {
        UnifiedInputData::getInInstance()->clearTempData(DATATYPE::TPFRAMEG);
        return;
    }
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPFRAMEG);
    myTableWidget->modelUpload();

    setDataModified(false);
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
}

void FrameSettingDialog::slotModelRewind()
{
    if ( Utility::informationRewind(this) ) return;

    myTableWidget->modelRewind();
    setDataModified(false);
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
}

void FrameSettingDialog::slotModelEdit()
{
    myTableWidget->modelEdit();

    rewind_button->setEnabled(true);
    edit_button->setEnabled(false);
    setDataModified(true);
}


void FrameSettingDialog::updateDrawWidget()
{
    setDataModified(true);
    if (!drawLinkStatus) return;

    QList<FRAMEGROUPVALUE> groupList = myTableWidget->getFrameGroupList();
    QList<FRAMEPOINTSVALUE> frameList = myTableWidget->getFramePointList();

    myDrawWidget->setDrawingData(groupList, frameList);
}

void FrameSettingDialog::changeCurrentGroupOfDrawWidget(const QString &gid)
{
    myDrawWidget->setCurrentGID(QUuid(gid));
}

void FrameSettingDialog::changeDrawingStatus(bool st)
{
    drawLinkStatus = st;

    if (drawLinkStatus) {
        QList<FRAMEGROUPVALUE> groupList = myTableWidget->getFrameGroupList();
        QList<FRAMEPOINTSVALUE> frameList = myTableWidget->getFramePointList();
        myDrawWidget->setDrawingData(groupList, frameList);
    }
}

void FrameSettingDialog::setDataModified(bool onoff)
{
    if (onoff) {
        upload_button->setEnabled(true);
        rewind_button->setEnabled(true);
        this->setWindowTitle(u8"通りの定義    *** 編集中 ***");
        this->setWindowIcon(QIcon(":/icons/c2_green.png"));
        emit statusChanged(CBSTATUS::CB_ACTIVE);
    } else {
        upload_button->setEnabled(false);
        this->setWindowTitle(u8"通りの定義");
        this->setWindowIcon(QIcon(":/icons/c2_blue.png"));
        ( UnifiedInputData::getInInstance()->getDataCount(DATATYPE::TPFRAMEG) > 0 ) ?
        emit statusChanged(CBSTATUS::CB_DEFINE) : emit statusChanged(CBSTATUS::CB_NORMAL) ;
    }
}

CustomTableModel* FrameSettingDialog::getTableModel() const{
    return myTableWidget->getTableModel();
}
CustomTableModel* FrameSettingDialog::getSubTableModel() const{
    return myTableWidget->getSubTableModel();
}

} // namespace post3dapp
