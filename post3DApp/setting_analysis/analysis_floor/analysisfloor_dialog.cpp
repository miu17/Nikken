#include "analysisfloor_dialog.h"

#include <QtGui>
#include <QDebug>
#include <QLabel>
#include <QPushButton>

#include "analysisfloor_tablewidget.h"
#include "app_version.h"
#include "define_analysisfloor.h"
#include "fileread_dialog.h"
#include "menu_button.h"
#include "reference_combobox.h"
#include "unified_data.h"
#include "unified_inputdata.h"
#include "utility.h"
#include "vhline.h"

namespace post3dapp{
AnalysisFloorDialog::AnalysisFloorDialog( QWidget *parent )
    : QDialog( parent )
{

    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));

    createButtons();
    this->setDataModified(false);

    QLabel *b_label = new QLabel(u8"・基準レベル階：", this);
    b_label->setFixedWidth(120);
    QLabel *r_label = new QLabel(u8"・屋上階：", this);
    r_label->setFixedWidth(120);
    baseFloorCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::Input, this);
    baseFloorCombobox->setFixedWidth(120);
    baseFloorCombobox->setDefaultName(u8"（階名）");
    topFloorCombobox = new ReferenceCombobox(DATATYPE::TPFLOOR, UnifiedDataType::Input, this);
    topFloorCombobox->setFixedWidth(120);
    topFloorCombobox->setDefaultName(u8"（階名）");
    QHBoxLayout *b_lay = new QHBoxLayout;
    b_lay->addWidget(b_label);
    b_lay->addWidget(baseFloorCombobox);
    b_lay->addStretch();
    QHBoxLayout *r_lay = new QHBoxLayout;
    r_lay->addWidget(r_label);
    r_lay->addWidget(topFloorCombobox);
    r_lay->addStretch();

    connect ( baseFloorCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
              this, &AnalysisFloorDialog::slotModifiedStateOn );
    connect ( topFloorCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged),
              this, &AnalysisFloorDialog::slotModifiedStateOn );


    AnalysisFloorDataType my_type;
    tableWidget = new AnalysisFloorTableWidget( my_type, this );
    connect ( tableWidget, &AnalysisFloorTableWidget::floorItemChanged,
              this, &AnalysisFloorDialog::slotModifiedStateOn );

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(10, 10, 10, 10);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addSpacing(5);
    vlay->addLayout(b_lay);
    vlay->addLayout(r_lay);
    vlay->addSpacing(5);
    vlay->addWidget(tableWidget);
    this->setLayout(vlay);

    connect( UnifiedInputData::getInInstance(), &UnifiedData::dataUpdated,
             this, &AnalysisFloorDialog::slotUpdateFloorTable );
}

void AnalysisFloorDialog::createButtons()
{
    upload_button = new QPushButton(u8"登　録", this);
    upload_button->setFixedSize(80, 30);
    upload_button->setAutoDefault(false);
    rewind_button = new QPushButton(u8"復　元", this);
    rewind_button->setFixedSize(80, 30);
    rewind_button->setAutoDefault(false);
    rewind_button->setEnabled(false);

    connect ( upload_button, &QAbstractButton::clicked, this, &AnalysisFloorDialog::slotModelUpload);
    connect ( rewind_button, &QAbstractButton::clicked, this, &AnalysisFloorDialog::slotModelRewind);

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(5);
    buttonsLayout->addWidget(upload_button);
    buttonsLayout->addWidget(rewind_button);
    buttonsLayout->addStretch();

}

bool AnalysisFloorDialog::checkBinaryFile ( QDataStream &infile, const AppVersion & ) const
{
    if ( infile.atEnd() ) return false;
    QString str1, str2;
    infile >> str1 >> str2;
    return tableWidget->checkBinaryFile(infile);
}

void AnalysisFloorDialog::readBinaryFile ( QDataStream &infile, const AppVersion & )
{

    QString str1, str2;
    infile >> str1 >> str2;
    baseFloorCombobox->setCurrentText(str1);
    topFloorCombobox->setCurrentText(str2);

    tableWidget->readBinaryFile(infile);
    QList<FLOORVALUES> floorList = tableWidget->getAnalysisFloorList();
    UnifiedInputData::getInInstance()->registerAnalysisFloorData(floorList,
                                                          baseFloorCombobox->currentUuid(),
                                                          topFloorCombobox->currentUuid() );
    tableWidget->modelUpload();
    setDataModified(false);
    rewind_button->setEnabled(false);

}
void AnalysisFloorDialog::readTextFile ( QTextStream &infile, const AppVersion &)
{

    QString str1, str2;
    infile >> str1 >> str2;
    baseFloorCombobox->setCurrentText(str1);
    topFloorCombobox->setCurrentText(str2);

    tableWidget->readTextFile(infile, true);
    QList<FLOORVALUES> floorList = tableWidget->getAnalysisFloorList();
    UnifiedInputData::getInInstance()->registerAnalysisFloorData(floorList,
                                                          baseFloorCombobox->currentUuid(),
                                                          topFloorCombobox->currentUuid() );
    tableWidget->modelUpload();
    setDataModified(false);
    rewind_button->setEnabled(false);

}

void AnalysisFloorDialog::writeBinaryFile ( QDataStream &outfile)
{
    outfile << baseFloorCombobox->currentText()  << topFloorCombobox->currentText();
    tableWidget->writeBinaryFile(outfile);
}
void AnalysisFloorDialog::writeTextFile ( QTextStream &outfile)
{
    outfile << baseFloorCombobox->currentText()  <<","<< topFloorCombobox->currentText()<< "\n";
    tableWidget->writeTextFile(outfile);
}

void AnalysisFloorDialog::clearData()
{
    tableWidget->clearData();
    tableWidget->modelUpload();
    rewind_button->setEnabled(false);
    setDataModified(false);
}

void AnalysisFloorDialog::slotModelUpload()
{
    /*    エラーチェック　    */

    if ( !tableWidget->checkAnalysisFloor() ) {
        Utility::showWarningMessage(this, u8"解析階名が重複しています。");
        return;
    }

    if ( Utility::informationUpdate(this) ) return;

    QList<FLOORVALUES> floorList = tableWidget->getAnalysisFloorList();
    UnifiedInputData::getInInstance()->registerAnalysisFloorData(floorList,
                                                          baseFloorCombobox->currentUuid(),
                                                          topFloorCombobox->currentUuid() );
    tableWidget->modelUpload();
    setDataModified(false);
    rewind_button->setEnabled(false);
}

void AnalysisFloorDialog::slotModelRewind()
{
    if ( Utility::informationRewind(this) ) return;

    tableWidget->modelRewind();
    setDataModified(false);
    rewind_button->setEnabled(false);
}

void AnalysisFloorDialog::slotUpdateFloorTable(DATATYPE dtype)
{
    if ( dtype != DATATYPE::TPFLOOR ) return;

    tableWidget->updateFloorTable();
    UnifiedInputData::getInInstance()->registerAnalysisFloorData( tableWidget->getAnalysisFloorList(),
                                                           baseFloorCombobox->currentUuid(),
                                                           topFloorCombobox->currentUuid() );
}


void AnalysisFloorDialog::setDataModified(bool onoff)
{
    if (onoff) {
        upload_button->setEnabled(true);
        rewind_button->setEnabled(true);
        this->setWindowTitle(u8"解析階の定義    *** 編集中 ***");
        this->setWindowIcon(QIcon(":/icons/c2_green.png"));
        emit statusChanged(CBSTATUS::CB_ACTIVE);
    } else {
        upload_button->setEnabled(false);
        this->setWindowTitle(u8"解析階の定義");
        this->setWindowIcon(QIcon(":/icons/c2_blue.png"));
        ( UnifiedInputData::getInInstance()->getDataCount(DATATYPE::TPFLOOR)>0 ) ?
           emit statusChanged(CBSTATUS::CB_DEFINE) : emit statusChanged(CBSTATUS::CB_NORMAL) ;
    }
}

void AnalysisFloorDialog::reflectUnifiedData(){
    tableWidget->reflectUnifiedData();
    baseFloorCombobox->setCurrentText(UnifiedInputData::getInInstance()->idToName(UnifiedInputData::getInInstance()->getBaseLevelFloor(), DATATYPE::TPFLOOR));
    topFloorCombobox->setCurrentText(UnifiedInputData::getInInstance()->idToName(UnifiedInputData::getInInstance()->getRoofTopFloor(), DATATYPE::TPFLOOR));
    rewind_button->setEnabled(false);
    setDataModified(false);
}

CustomTableModel* AnalysisFloorDialog::getTableModel() const{
    return tableWidget->getTableModel();
}

} // namespace post3dapp
