#include "textinput_dialog.h"

#include <QtGui>
#include <QBoxLayout>
#include <QDebug>
#include <QFileDialog>
#include <QPushButton>

#include "define_textinput.h"
#include "fileread_dialog.h"
#include "inputjoint_tablewidget.h"
#include "inputload_tablewidget.h"
#include "inputmember_tablewidget.h"
#include "menu_button.h"
#include "unified_data.h"
#include "unified_editingdata.h"
#include "unified_inputdata.h"
#include "utility.h"
#include "vhline.h"

namespace post3dapp{
TextInputDialog::TextInputDialog( QWidget *parent )
    : QDialog( parent )
{

    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint |Qt::WindowCloseButtonHint);
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));

    createButtons();
    jointTableWidget = new InputJointTableWidget(JointTableForm(), this);
    memberTableWidget = new InputMemberTableWidget(MemberTableForm(), this);
    loadTableWidget = new InputLoadTableWidget(LoadTableForm(), this);

    QTabWidget *tab_widget = new QTabWidget(this);
    tab_widget->addTab(jointTableWidget, u8"節点データ");
    tab_widget->addTab(memberTableWidget, u8"部材データ");
    tab_widget->addTab(loadTableWidget, u8"荷重データ");
    tab_widget->setIconSize(QSize(20, 20));

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(10, 10, 10, 10);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(tab_widget);
    this->setLayout(vlay);

    this->setWindowTitle(u8"テキスト入力");
}

void TextInputDialog::createButtons()
{

    QPushButton *open_button = new QPushButton(u8"ファイル読込", this);
    open_button->setFixedSize(90, 30);
    open_button->setAutoDefault(false);
    QPushButton *save_button = new QPushButton(u8"ファイル保存", this);
    save_button->setFixedSize(90, 30);
    save_button->setAutoDefault(false);

    QPushButton *import_button = new QPushButton(u8"データ取得", this);
    import_button->setFixedSize(90, 30);
    connect( import_button, &QAbstractButton::clicked, this, &TextInputDialog::slotDataImport );

    QPushButton *jup_button = new QPushButton(u8"節点登録", this);
    jup_button->setFixedSize(80, 30);
    jup_button->setAutoDefault(false);
    QPushButton *mup_button = new QPushButton(u8"部材登録", this);
    mup_button->setFixedSize(80, 30);
    mup_button->setAutoDefault(false);
    QPushButton *lup_button = new QPushButton(u8"荷重登録", this);
    lup_button->setFixedSize(80, 30);
    lup_button->setAutoDefault(false);

    QPushButton *allup_button = new QPushButton(u8"一括登録", this);
    allup_button->setFixedSize(80, 30);
    allup_button->setAutoDefault(false);

    connect ( open_button, &QAbstractButton::clicked, this, &TextInputDialog::slotTextFileOpen );
    connect ( save_button, &QAbstractButton::clicked, this, &TextInputDialog::slotTextFileSave );

    connect ( jup_button, &QAbstractButton::clicked, this, &TextInputDialog::slotJointUpload );
    connect ( mup_button, &QAbstractButton::clicked, this, &TextInputDialog::slotMemberUpload );
    connect ( lup_button, &QAbstractButton::clicked, this, &TextInputDialog::slotLoadUpload );
    connect ( allup_button, &QAbstractButton::clicked, this, &TextInputDialog::slotModelUpload );

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(5);
    buttonsLayout->addWidget(open_button);
    buttonsLayout->addWidget(save_button);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(import_button);
    buttonsLayout->addWidget(new VFLine(Qt::lightGray, this));
    buttonsLayout->addWidget(jup_button);
    buttonsLayout->addWidget(mup_button);
    buttonsLayout->addWidget(lup_button);
    buttonsLayout->addWidget(new VFLine(Qt::lightGray, this));
    buttonsLayout->addWidget(allup_button);
}

void TextInputDialog::clearData()
{
    jointTableWidget->clearData();
    memberTableWidget->clearData();
    loadTableWidget->clearData();
}

void TextInputDialog::slotTextFileOpen()
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
    bool ok = jointTableWidget->readTextFile(in, true);
    if ( ok ) memberTableWidget->readTextFile(in, true);
    if ( ok ) loadTableWidget->readTextFile(in, true);
    if ( !ok ) {
        Utility::messageFormatError(this);
        return;
    }
}

void TextInputDialog::slotTextFileSave()
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

    jointTableWidget->writeTextFile(outs);
    memberTableWidget->writeTextFile(outs);
    loadTableWidget->writeTextFile(outs);
}

void TextInputDialog::slotDataImport()
{
    jointTableWidget->importJointData( UnifiedInputData::getInInstance()->jointListOfAll() );
    QList<DATATYPE> mtypes, ltypes;
    mtypes << DATATYPE::TPCOLUMN << DATATYPE::TPGIRDER << DATATYPE::TPBRACE << DATATYPE::TPBEAM << DATATYPE::TPSLAB << DATATYPE::TPWALL << DATATYPE::TPUWALL
           << DATATYPE::TPDAMPER << DATATYPE::TPISO;
    memberTableWidget->importMemberData( UnifiedInputData::getInInstance()->memberListOfType(mtypes) );
    ltypes << DATATYPE::TPJOINTLOAD << DATATYPE::TPMEMBERLOAD << DATATYPE::TPSLABLOAD;
    loadTableWidget->importMemberData( UnifiedInputData::getInInstance()->memberListOfType(ltypes) );
}

void TextInputDialog::slotJointUpload()
{
    if ( Utility::informationUpdate(this) ) return;

    QString msg = jointTableWidget->checkJointData();
    if ( !msg.isEmpty() ) {
        Utility::showErrorMessage2(this, msg);
        return;
    }

    UnifiedEditingData::getInstance()->clearCopyBuffer();
    jointTableWidget->uploadJoints();

    UnifiedInputData::getInInstance()->signalElementCountChanged();
    UnifiedEditingData::getInstance()->clearUndoStack();
}

void TextInputDialog::slotMemberUpload()
{
    if ( Utility::informationUpdate(this) ) return;

    QString msg = memberTableWidget->checkMemberData( UnifiedInputData::getInInstance()->getDataCount(
                                                          DATATYPE::TPJOINT) + 1 );
    if ( !msg.isEmpty() ) {
        Utility::showErrorMessage2(this, msg);
        return;
    }

    UnifiedEditingData::getInstance()->clearCopyBuffer();
    memberTableWidget->uploadMembers();

    UnifiedInputData::getInInstance()->signalElementCountChanged();
    UnifiedEditingData::getInstance()->clearUndoStack();
}

void TextInputDialog::slotLoadUpload()
{
    if ( Utility::informationUpdate(this) ) return;

    QString msg = loadTableWidget->checkLoadData( UnifiedInputData::getInInstance()->getDataCount(
                                                      DATATYPE::TPJOINT) + 1 );
    if ( !msg.isEmpty() ) {
        Utility::showErrorMessage2(this, msg);
        return;
    }

    UnifiedEditingData::getInstance()->clearCopyBuffer();
    loadTableWidget->uploadLoads();

    UnifiedInputData::getInInstance()->signalElementCountChanged();
    UnifiedEditingData::getInstance()->clearUndoStack();
}

void TextInputDialog::slotModelUpload()
{
    if ( Utility::informationUpdate(this) ) return;

    QString msg = jointTableWidget->checkJointData();
    if ( !msg.isEmpty() ) {
        Utility::showErrorMessage2(this, msg);
        return;
    }
    int jcount = jointTableWidget->getJointCount();
    msg = memberTableWidget->checkMemberData( jcount + 1 );
    if ( !msg.isEmpty() ) {
        Utility::showErrorMessage2(this, msg);
        return;
    }
    msg = loadTableWidget->checkLoadData( jcount + 1 );
    if ( !msg.isEmpty() ) {
        Utility::showErrorMessage2(this, msg);
        return;
    }

    UnifiedEditingData::getInstance()->clearCopyBuffer();
    jointTableWidget->uploadJoints();
    memberTableWidget->uploadMembers();
    loadTableWidget->uploadLoads();

    UnifiedInputData::getInInstance()->signalElementCountChanged();
    UnifiedEditingData::getInstance()->clearUndoStack();
}

} // namespace post3dapp
