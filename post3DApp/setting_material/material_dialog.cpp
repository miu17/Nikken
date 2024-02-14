#include <QtGui>
#include <QDebug>

#include "app_version.h"
#include "menu_button.h"
#include "material_dialog.h"
#include "material_tablewidget.h"
#include "material_filterdialog.h"
#include "define_material_struct.h"
#include "utility.h"
#include "unified_inputdata.h"
namespace post3dapp{
MaterialSettingDialog::MaterialSettingDialog( QWidget *parent )
    : QDialog( parent )
{
    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    this->setWindowTitle(u8"材料定義");

    createButtons();

    MaterialTable dtype;
    materialWidget = new MaterialTableWidget(dtype);
    connect ( materialWidget, &MaterialTableWidget::sectionItemChanged,
              this, &MaterialSettingDialog::slotModifiedStateOn );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(5, 5, 5, 5);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(materialWidget);
    this->setLayout(vlay);

    filterDialog = new MaterialFilterDialog(dtype, this);
}

void MaterialSettingDialog::slotOpenFilterDialog()
{
    QHash<QString, QList<bool> > filtercolumns;
    filtercolumns["MATERIAL"]   = materialWidget->getFilterColumns();
    filterDialog->resetFilterColumns(filtercolumns);
    if ( filterDialog->exec() ) { //trueが帰ってきたらセット
        QHash<QString, QList<bool> > filters = filterDialog->getFilterColumns();
        materialWidget->setFilterColumns(filters["MATERIAL"]);
    }
}

void MaterialSettingDialog::createButtons()
{
    QPushButton *open_button = new QPushButton(u8"ファイル読込");
    open_button->setFixedSize(90, 30);
    open_button->setAutoDefault(false);
    QPushButton *save_button = new QPushButton(u8"ファイル保存");
    save_button->setFixedSize(90, 30);
    save_button->setAutoDefault(false);
    /*
    QPushButton *config_button = new QPushButton(tr("Display setting"));
    config_button->setFixedSize(90, 30);
    config_button->setAutoDefault(false);
    */

    connect ( open_button, &QAbstractButton::clicked, this, &MaterialSettingDialog::slotTextFileOpen);
    connect ( save_button, &QAbstractButton::clicked, this, &MaterialSettingDialog::slotTextFileSave);
    //connect ( config_button, SIGNAL( clicked() ), this, SLOT( slotOpenFilterDialog() ));

    upload_button = new QPushButton(u8"登　録", this);
    upload_button->setFixedSize(80, 30);
    upload_button->setAutoDefault(false);
    rewind_button = new QPushButton(u8"復　元", this);
    rewind_button->setFixedSize(80, 30);
    rewind_button->setAutoDefault(false);
    rewind_button->setEnabled(false);

    connect ( upload_button, &QAbstractButton::clicked, this, &MaterialSettingDialog::slotModelUpload);
    connect ( rewind_button, &QAbstractButton::clicked, this, &MaterialSettingDialog::slotModelRewind);

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(5);
    buttonsLayout->addWidget(open_button);
    buttonsLayout->addWidget(save_button);
    //buttonsLayout->addWidget(config_button);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(upload_button);
    buttonsLayout->addWidget(rewind_button);
}

bool MaterialSettingDialog::checkBinaryFile ( QDataStream &infile, const AppVersion & ) const
{
    return materialWidget->checkBinaryFile(infile);
}

void MaterialSettingDialog::readBinaryFile ( QDataStream &infile, const AppVersion &)
{
    materialWidget->readBinaryFile(infile);
    QList<MATERIALVALUES> mvlist = materialWidget->getMaterialList();
    UnifiedInputData::getInInstance()->registerTempData(mvlist);

    materialWidget->modelUpload();
    rewind_button->setEnabled(false);
    setDataModified(false);
}
void MaterialSettingDialog::readTextFile ( QTextStream &infile, const AppVersion &var)
{
    materialWidget->readTextFile(infile,true);
    QList<MATERIALVALUES> mvlist = materialWidget->getMaterialList();
    UnifiedInputData::getInInstance()->registerTempData(mvlist);

    materialWidget->modelUpload();
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void MaterialSettingDialog::reflectUnifiedData(){
    materialWidget->reflectUnifiedData();
    materialWidget->modelUpload();
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void MaterialSettingDialog::writeBinaryFile ( QDataStream &outfile)
{
    materialWidget->writeBinaryFile(outfile);
}
void MaterialSettingDialog::writeTextFile ( QTextStream &outfile)
{
    materialWidget->writeTextFile(outfile);
}

void MaterialSettingDialog::clearData()
{
    materialWidget->clearData();
    materialWidget->modelUpload();
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void MaterialSettingDialog::slotTextFileOpen()
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
    if ( !materialWidget->checkTextFile(in) ) {
        Utility::messageFormatError(this);
        return;
    }
    in.seek(0);
    materialWidget->readTextFile(in, true);

    setDataModified(true);
}

void MaterialSettingDialog::slotTextFileSave()
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
    materialWidget->writeTextFile(outs);
}

void MaterialSettingDialog::slotModelUpload()
{
    QList<MATERIALVALUES> mvlist = materialWidget->getMaterialList();
    if ( !UnifiedInputData::getInInstance()->checkTempData(this, mvlist) ) return;
    if ( Utility::informationUpdate(this) ) {
        UnifiedInputData::getInInstance()->clearTempData(DATATYPE::TPMACONC); // Materialのうちいずれか
        return;
    }
    UnifiedInputData::getInInstance()->registerTempData(DATATYPE::TPMACONC); // Materialのうちいずれか

    materialWidget->modelUpload();
    setDataModified(false);
    rewind_button->setEnabled(false);
}

void MaterialSettingDialog::slotModelRewind()
{
    materialWidget->modelRewind();
    setDataModified(false);
    rewind_button->setEnabled(false);
}

void MaterialSettingDialog::setDataModified(bool onoff)
{
    if ( onoff ) {
        upload_button->setEnabled(true);
        rewind_button->setEnabled(true);
        this->setWindowTitle(u8"材料 定義    *** 編集中 ***");
        this->setWindowIcon(QIcon(":/icons/c2_green.png"));
        emit statusChanged(CBSTATUS::CB_ACTIVE);
    } else {
        upload_button->setEnabled(false);
        this->setWindowTitle(u8"材料定義");
        this->setWindowIcon(QIcon(":/icons/c2_blue.png"));
        int ict = UnifiedInputData::getInInstance()->getDataCount(DATATYPE::TPMACONC);
        ict += UnifiedInputData::getInInstance()->getDataCount(DATATYPE::TPMASTEEL);
        ict += UnifiedInputData::getInInstance()->getDataCount(DATATYPE::TPMAREIN);
        ( ict > 0 ) ? emit statusChanged(CBSTATUS::CB_DEFINE) : emit statusChanged(CBSTATUS::CB_NORMAL) ;
    }
}

CustomTableModel* MaterialSettingDialog::getTableModel() const{
    return materialWidget->getTableModel();
}

} // namespace post3dapp
