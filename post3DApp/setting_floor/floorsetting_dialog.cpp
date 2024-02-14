#include "floorsetting_dialog.h"

#include <QtGui>
#include <QDebug>

#include "app_version.h"
#include "define_floorsettingtable.h"
#include "fileread_dialog.h"
#include "floorsetting_drawwidget.h"
#include "floorsetting_tablewidget.h"
#include "menu_button.h"
#include "unified_data.h"
#include "unified_inputdata.h"
#include "utility.h"
#include "vhline.h"

namespace post3dapp{

FloorSettingDialog::FloorSettingDialog( QWidget *parent )
    : QDialog( parent )
{

    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));

    createButtons();
    this->setDataModified(false);

    FloorDataType my_type;
    myTableWidget = new FloorSettingTableWidget(my_type, this);
    myDrawWidget = new FloorSettingDrawWidget(this);

    connect ( myTableWidget, &FloorSettingTableWidget::floorItemChanged,
              this, &FloorSettingDialog::updateDrawWidget);

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
}

void FloorSettingDialog::createButtons()
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

    connect ( open_button, &QAbstractButton::clicked, this, &FloorSettingDialog::slotTextFileOpen );
    connect ( save_button, &QAbstractButton::clicked, this, &FloorSettingDialog::slotTextFileSave );

    connect ( upload_button, &QAbstractButton::clicked, this, &FloorSettingDialog::slotModelUpload );
    connect ( rewind_button, &QAbstractButton::clicked, this, &FloorSettingDialog::slotModelRewind );
    connect ( edit_button, &QAbstractButton::clicked, this, &FloorSettingDialog::slotModelEdit );

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

bool FloorSettingDialog::checkBinaryFile ( QDataStream &infile, const AppVersion & ) const
{
    return myTableWidget->checkBinaryFile(infile);
}

void FloorSettingDialog::readBinaryFile ( QDataStream &infile, const AppVersion & )
{
    myTableWidget->readBinaryFile(infile);
    UnifiedInputData::getInInstance()->registerTempData(myTableWidget->getFloorList());
    myTableWidget->modelUpload();
    updateDrawWidget();
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
    setDataModified(false);
}
void FloorSettingDialog::readTextFile ( QTextStream &infile ,const AppVersion &var)
{
    myTableWidget->readTextFile(infile,true);
    UnifiedInputData::getInInstance()->registerTempData(myTableWidget->getFloorList());
    myTableWidget->modelUpload();
    updateDrawWidget();
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
    setDataModified(false);
}

void FloorSettingDialog::reflectUnifiedData(){
    myTableWidget->reflectUnifiedData();
    myTableWidget->modelUpload();
    updateDrawWidget();
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
    setDataModified(false);
}

void FloorSettingDialog::writeBinaryFile ( QDataStream &outfile)
{
    myTableWidget->writeBinaryFile(outfile);
}
void FloorSettingDialog::writeTextFile ( QTextStream &outfile, bool writeUuid)
{
    myTableWidget->writeTextFile(outfile, writeUuid);
}

void FloorSettingDialog::clearData()
{
    myTableWidget->clearData();
    myTableWidget->modelUpload();
    updateDrawWidget();
    rewind_button->setEnabled(false);
    edit_button->setEnabled(false);
    setDataModified(false);
}

void FloorSettingDialog::slotTextFileOpen()
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
    /*
        if( !myTableWidget->readTextFile(in,state) ){
            QMessageBox::warning( this, "WARNING",tr("ファイルのフォーマットが間違っています。"),
                                  QMessageBox::NoButton,QMessageBox::Warning );
            return;
        }
    */
    edit_button->setEnabled(false);
    setDataModified(true);
}

void FloorSettingDialog::slotTextFileSave()
{
    QString filename =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.txt)" );
    if ( filename.isEmpty() ) return;
    QFile file(filename);
    if ( !file.open(QIODevice::WriteOnly) ) {
        Utility::messageFileSaveError(this);
        return;
    }

    QTextStream outs(&file);

    myTableWidget->writeTextFile(outs, false);

}

void FloorSettingDialog::slotModelUpload()
{
    QList<FLOORVALUES> floorList = myTableWidget->getFloorList();

    if ( floorList.isEmpty() ) {
        Utility::showWarningMessage(this, u8"最低１つ以上の階を定義してください。");
        return;
    }

    if ( !UnifiedInputData::getInInstance()->checkTempData(this, floorList) ) return;

    if ( Utility::informationUpdate(this) ) {
        UnifiedInputData::getInInstance()->clearTempData(DATATYPE::TPFLOOR);
        return;
    }
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPFLOOR);
    myTableWidget->modelUpload();

    setDataModified(false);
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
}

void FloorSettingDialog::slotModelRewind()
{
    if ( Utility::informationRewind(this) ) return;

    myTableWidget->modelRewind();
    setDataModified(false);
    rewind_button->setEnabled(false);
    if ( myTableWidget->existResisterRow() ) edit_button->setEnabled(true);
}

void FloorSettingDialog::slotModelEdit()
{
    myTableWidget->modelEdit();

    rewind_button->setEnabled(true);
    edit_button->setEnabled(false);
    setDataModified(true);
}

void FloorSettingDialog::updateDrawWidget()
{
    setDataModified(true);
    QList<FLOORVALUES> floorList = myTableWidget->getFloorList(false);
    myDrawWidget->setDrawingData(floorList);
}

void FloorSettingDialog::setDataModified(bool onoff)
{
    if (onoff) {
        upload_button->setEnabled(true);
        rewind_button->setEnabled(true);
        this->setWindowTitle(u8"階の定義    *** 編集中 ***");
        this->setWindowIcon(QIcon(":/icons/c2_green.png"));
        emit statusChanged(CBSTATUS::CB_ACTIVE);
    } else {
        upload_button->setEnabled(false);
        this->setWindowTitle(u8"階の定義");
        this->setWindowIcon(QIcon(":/icons/c2_blue.png"));
        ( UnifiedInputData::getInInstance()->getDataCount(DATATYPE::TPFLOOR) > 0 ) ?
        emit statusChanged(CBSTATUS::CB_DEFINE) : emit statusChanged(CBSTATUS::CB_NORMAL) ;
    }
}

CustomTableModel* FloorSettingDialog::getTableModel() const{
    return myTableWidget->getTableModel();
}

} // namespace post3dapp
