#include "slabloadsetting_dialog.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>

#include "app_version.h"
#include "define_loadsettingtable.h"
#include "menu_button.h"
#include "slabloadsetting_tablewidget.h"
#include "unified_data.h"
#include "unified_inputdata.h"
#include "utility.h"
#include "vhline.h"

namespace post3dapp{
SlabLoadSettingDialog::SlabLoadSettingDialog( DATATYPE dtype, QWidget *parent )
    : QDialog( parent ), myDataType(dtype)
{

    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint );
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));

    createButtons();
    this->setDataModified(false);

    SlabLoadType a_type;
    myTableWidget = new SlabLoadSettingTableWidget(a_type, this);
    connect ( myTableWidget, &SlabLoadSettingTableWidget::sectionItemChanged,
              this, &SlabLoadSettingDialog::slotModifiedStateOn );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(10, 10, 10, 10);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);

    vlay->addWidget(myTableWidget);
    this->setLayout(vlay);

}

void SlabLoadSettingDialog::createButtons()
{

    QPushButton *open_button = new QPushButton(u8"ファイル読込", this);
    open_button->setFixedSize(90, 30);
    open_button->setAutoDefault(false);
    QPushButton *save_button = new QPushButton(u8"ファイル保存", this);
    save_button->setFixedSize(90, 30);
    save_button->setAutoDefault(false);

    connect ( open_button, &QAbstractButton::clicked, this, &SlabLoadSettingDialog::slotTextFileOpen);
    connect ( save_button, &QAbstractButton::clicked, this, &SlabLoadSettingDialog::slotTextFileSave);

    upload_button = new QPushButton(u8"登　録", this);
    upload_button->setFixedSize(80, 30);
    upload_button->setAutoDefault(false);
    rewind_button = new QPushButton(u8"復　元", this);
    rewind_button->setFixedSize(80, 30);
    rewind_button->setAutoDefault(false);
    rewind_button->setEnabled(false);

    connect ( upload_button, &QAbstractButton::clicked, this, &SlabLoadSettingDialog::slotModelUpload);
    connect ( rewind_button, &QAbstractButton::clicked, this, &SlabLoadSettingDialog::slotModelRewind);

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(5);
    buttonsLayout->addWidget(open_button);
    buttonsLayout->addWidget(save_button);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(upload_button);
    buttonsLayout->addWidget(rewind_button);
}

bool SlabLoadSettingDialog::checkBinaryFile ( QDataStream &infile, const AppVersion & ) const
{
    return myTableWidget->checkBinaryFile(infile);
}

void SlabLoadSettingDialog::readBinaryFile ( QDataStream &infile, const AppVersion &)
{
    myTableWidget->readBinaryFile(infile);
    QList<SLABLOADVALUES> slist = myTableWidget->getValueList();
    UnifiedInputData::getInInstance()->registerTempData(slist);

    myTableWidget->modelUpload();
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void SlabLoadSettingDialog::readTextFile ( QTextStream &infile, const AppVersion &)
{
    myTableWidget->readTextFile(infile,true);
    QList<SLABLOADVALUES> slist = myTableWidget->getValueList();
    UnifiedInputData::getInInstance()->registerTempData(slist);

    myTableWidget->modelUpload();
    rewind_button->setEnabled(false);
    setDataModified(false);
}
void SlabLoadSettingDialog::writeBinaryFile ( QDataStream &outfile)
{
    myTableWidget->writeBinaryFile(outfile);
}
void SlabLoadSettingDialog::writeTextFile ( QTextStream &outfile)
{
    myTableWidget->writeTextFile(outfile);
}

void SlabLoadSettingDialog::clearData()
{
    myTableWidget->clearData();
    myTableWidget->modelUpload();
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void SlabLoadSettingDialog::slotTextFileOpen()
{
    QString filename =
        QFileDialog::getOpenFileName( this, "Open File", ".", "File(*.txt)" );
    if ( filename.isEmpty() ) return;

    QFile file(filename);
    if ( !file.open(QIODevice::ReadOnly) ) {
        Utility::messageFileOpenError(this);
        return;
    }

    QTextStream in(&file);

    if ( !myTableWidget->checkTextFile(in) ) {
        Utility::messageFormatError(this);
        return;
    }
    in.seek(0);
    myTableWidget->readTextFile(in, true);

    setDataModified(true);
}

void SlabLoadSettingDialog::slotTextFileSave()
{
    QString filename = QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.txt)" );
    if ( filename.isEmpty() ) return;
    QFile file(filename);
    if ( !file.open(QIODevice::WriteOnly) ) {
        Utility::messageFileSaveError(this);
        return;
    }

    QTextStream outs(&file);
    myTableWidget->writeTextFile(outs);
}

void SlabLoadSettingDialog::replaceMatchingName( const QString &str_a, const QString &str_b )
{
    myTableWidget->replaceMatchingName(str_a, str_b);
}

void SlabLoadSettingDialog::slotModelUpload()
{
    QList<SLABLOADVALUES> slist = myTableWidget->getValueList();
    if ( ! UnifiedInputData::getInInstance()->checkTempData(this, slist) ) return;
    if ( Utility::informationUpdate(this) ) {
        UnifiedInputData::getInInstance()->clearTempData(myDataType);
        return;
    }
    UnifiedInputData::getInInstance()->registerTempData(myDataType);

    myTableWidget->modelUpload();
    setDataModified(false);
    rewind_button->setEnabled(false);
}

void SlabLoadSettingDialog::slotModelRewind()
{
    myTableWidget->modelRewind();
    setDataModified(false);
    rewind_button->setEnabled(false);
}

void SlabLoadSettingDialog::setDataModified(bool onoff)
{
    if (onoff) {
        upload_button->setEnabled(true);
        rewind_button->setEnabled(true);
        this->setWindowTitle(u8"床荷重の定義    *** 編集中 ***");
        this->setWindowIcon(QIcon(":/icons/c2_green.png"));
        emit statusChanged(CBSTATUS::CB_ACTIVE);
    } else {
        upload_button->setEnabled(false);
        this->setWindowTitle(u8"床荷重の定義");
        this->setWindowIcon(QIcon(":/icons/c2_blue.png"));
        ( UnifiedInputData::getInInstance()->getDataCount(myDataType, true) > 0 ) ?
        emit statusChanged(CBSTATUS::CB_DEFINE) : emit statusChanged(CBSTATUS::CB_NORMAL) ;
    }
}

void SlabLoadSettingDialog::reflectUnifiedData(){
    myTableWidget->reflectUnifiedData();

    rewind_button->setEnabled(false);
    setDataModified(false);
}

CustomTableModel* SlabLoadSettingDialog::getTableModel() const{
    return myTableWidget->getTableModel();
}

} // namespace post3dapp
