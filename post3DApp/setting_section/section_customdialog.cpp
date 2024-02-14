#include "section_customdialog.h"

#include <QtGui>
#include <QDebug>

#include "app_version.h"
#include "define_section_struct.h"
#include "menu_button.h"
#include "section_filterdialog.h"
#include "section_tablewidget.h"
#include "unified_inputdata.h"
#include "utility.h"

namespace post3dapp{
SectionCustomDialog::SectionCustomDialog(QWidget *parent )
    : QDialog( parent )
{
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
}

void SectionCustomDialog::createButtons(bool hasFilter)
{
    QPushButton *open_button = new QPushButton(u8"ファイル読込", this);
    open_button->setFixedSize(90, 30);
    open_button->setAutoDefault(false);
    QPushButton *save_button = new QPushButton(u8"ファイル保存", this);
    save_button->setFixedSize(90, 30);
    save_button->setAutoDefault(false);

    connect ( open_button, &QAbstractButton::clicked, this, &SectionCustomDialog::slotTextFileOpen);
    connect ( save_button, &QAbstractButton::clicked, this, &SectionCustomDialog::slotTextFileSave);

    upload_button = new QPushButton(u8"登　録", this);
    upload_button->setFixedSize(80, 30);
    upload_button->setAutoDefault(false);
    rewind_button = new QPushButton(u8"復　元", this);
    rewind_button->setFixedSize(80, 30);
    rewind_button->setAutoDefault(false);
    rewind_button->setEnabled(false);

    connect ( upload_button, &QAbstractButton::clicked, this, &SectionCustomDialog::slotModelUpload);
    connect ( rewind_button, &QAbstractButton::clicked, this, &SectionCustomDialog::slotModelRewind);

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(5);
    buttonsLayout->addWidget(open_button);
    buttonsLayout->addWidget(save_button);
    if (hasFilter){
        QPushButton *config_button = new QPushButton(u8"表示設定読込");
        config_button->setFixedSize(90, 30);
        config_button->setAutoDefault(false);
        connect ( config_button, &QAbstractButton::clicked, this, &SectionCustomDialog::slotOpenFilterDialog);
        buttonsLayout->addWidget(config_button);
    }

    buttonsLayout->addStretch();
    buttonsLayout->addWidget(upload_button);
    buttonsLayout->addWidget(rewind_button);
}

bool SectionCustomDialog::checkBinaryFile ( QDataStream &infile, const AppVersion & ) const
{
    bool isOk = true;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        isOk = wid->checkBinaryFile(infile);
        if ( !isOk ) return false;
    }
    return true;
}

void SectionCustomDialog::readBinaryFile ( QDataStream &infile, const AppVersion & )
{
    // 壁はdtypeが異なる
    QHash<int, QList<SectionValues>> sectionHash;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        QList<SectionValues> slist;
        DATATYPE dtype = wid->getSectionDataType().dataType();
        wid->readBinaryFile(infile);
        slist.append( wid->getSectionList() );
        if (sectionHash.contains(static_cast<int>(dtype))){
            QList<SectionValues> temp = sectionHash.value(static_cast<int>(dtype));
            temp.append(slist);
            sectionHash.insert(static_cast<int>(dtype), temp);
        }
        else{
            sectionHash.insert(static_cast<int>(dtype), slist);
        }
    }
    QHashIterator<int, QList<SectionValues>> iter(sectionHash);
    while (iter.hasNext()){
        iter.next();
        UnifiedInputData::getInInstance()->registerTempData(static_cast<DATATYPE>(iter.key()), iter.value());
    }

    Q_FOREACH ( SectionTableWidget *wid, childWidgets )
        wid->modelUpload();
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void SectionCustomDialog::readTextFile ( QTextStream &infile, const AppVersion &)
{
    QList<SectionValues> slist;
    DATATYPE dtype = childWidgets.first()->getSectionDataType().dataType();

    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        wid->readTextFile(infile,true);
        slist.append( wid->getSectionList() );
    }
    UnifiedInputData::getInInstance()->registerTempData(dtype, slist);
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) wid->modelUpload();

    rewind_button->setEnabled(false);
    setDataModified(false);
}



void SectionCustomDialog::writeBinaryFile ( QDataStream &outfile) const
{
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        wid->writeBinaryFile(outfile);
    }
}
void SectionCustomDialog::writeTextFile ( QTextStream &outfile) const
{
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        wid->writeTextFile(outfile);
    }
}

void SectionCustomDialog::clearData()
{
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        wid->clearData();
        wid->modelUpload();
    }
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void SectionCustomDialog::replaceMatchingName( const QString &str_a, const QString &str_b )
{
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        wid->replaceMatchingName(str_a, str_b);
    }
}

void SectionCustomDialog::slotModelUpload()
{
    // 壁ダイアログの場合のみ、TPWALL TPUWALLの2種のDATATYPEを扱う→　分離した

    QList<SectionValues> slist;
    DATATYPE dtype = childWidgets.first()->getSectionDataType().dataType();

    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        slist.append( wid->getSectionList() );
    }
    if ( !UnifiedInputData::getInInstance()->checkTempData(this, dtype, slist) ) return;
    if ( Utility::informationUpdate(this) ) {
        UnifiedInputData::getInInstance()->clearTempData(dtype);
        return;
    }
    UnifiedInputData::getInInstance()->registerTempData(dtype);
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) wid->modelUpload();

    setDataModified(false);
    rewind_button->setEnabled(false);
}

void SectionCustomDialog::slotModelRewind()
{
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) wid->modelRewind();

    setDataModified(false);
    rewind_button->setEnabled(false);
}

void SectionCustomDialog::setDataModified(bool onoff)
{
    if ( onoff ) {
        upload_button->setEnabled(true);
        rewind_button->setEnabled(true);
        this->setWindowTitle(sectionWindowTitle + u8"    *** 編集中 ***");
        this->setWindowIcon(QIcon(":/icons/c2_green.png"));
        emit statusChanged(CBSTATUS::CB_ACTIVE);
    } else {
        upload_button->setEnabled(false);
        this->setWindowTitle(sectionWindowTitle);
        this->setWindowIcon(QIcon(":/icons/c2_blue.png"));
        DATATYPE dtype = childWidgets.first()->getSectionDataType().dataType();
//        if ( dtype != DATATYPE::TPWALL ) {
            ( UnifiedInputData::getInInstance()->getDataCount(dtype, true) > 0 ) ?
            emit statusChanged(CBSTATUS::CB_DEFINE) : emit statusChanged(CBSTATUS::CB_NORMAL) ;
//        } else {
//            int cnt = UnifiedInputData::getInInstance()->getDataCount(DATATYPE::TPWALL, true);
//            cnt += UnifiedInputData::getInInstance()->getDataCount(DATATYPE::TPSWALL, true);
//            cnt += UnifiedInputData::getInInstance()->getDataCount(DATATYPE::TPUWALL, true);
//            ( cnt > 0 ) ? emit statusChanged(CBSTATUS::CB_DEFINE) : emit statusChanged(CBSTATUS::CB_NORMAL) ;
//        }
    }
}

void SectionCustomDialog::slotTextFileOpen()
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
    bool isOk = true;
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        if (isOk) isOk = wid->checkTextFile(in);
    }
    if ( !isOk ) {
        Utility::messageFormatError(this);
        return;
    }
    in.seek(0);

    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        wid->readTextFile(in, true);
        wid->recalcDefaultValue();
    }

    setDataModified(true);
}

void SectionCustomDialog::slotTextFileSave()
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
    Q_FOREACH ( SectionTableWidget *wid, childWidgets ) {
        wid->writeTextFile(outs);
    }
}

void SectionCustomDialog::reflectUnifiedData(){
    Q_FOREACH(SectionTableWidget* tableWidget, childWidgets){
        tableWidget->reflectUnifiedData();
    }
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void SectionCustomDialog::slotOpenFilterDialog(){
    QHash<QString, QList<bool> > filtercolumns;
    Q_FOREACH(SectionTableWidget* tableWidgets, childWidgets){
        filtercolumns[tableWidgets->getName()]   = tableWidgets->getFilterColumns();
    }
    filterDialog->resetFilterColumns(filtercolumns);
    if (filterDialog->exec()) { //trueが帰ってきたらセット
        QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
        Q_FOREACH(SectionTableWidget* tableWidgets, childWidgets){
            tableWidgets->setFilterColumns(filters[tableWidgets->getName()]);
            tableWidgets->setTableViewBorderColumns(tableWidgets->getSectionDataType(), filters[tableWidgets->getName()]);
            emit tableWidgets->filterColumnsChanged(filters[tableWidgets->getName()]);
        }
    }
}
} // namespace post3dapp
