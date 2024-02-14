#include <QtGui>
#include <QDebug>

#include "textinput_dialog.h"
#include "vhline.h"

#include "fileread_dialog.h"
#include "unified_data.h"

TextInputDialog::TextInputDialog( QWidget* parent )
    : QDialog( parent )
{

    this->setWindowFlags(Qt::Dialog
                         | Qt::WindowMaximizeButtonHint
                         | Qt::WindowMinimizeButtonHint );
    this->setFont(QFont(QString::fromLocal8Bit("Microsoft JhengHei"),10));

    createButtons();
    jointTableWidget=new InputJointTableWidget(JointTableForm(),this);
    memberTableWidget=new InputMemberTableWidget(MemberTableForm(),this);
    loadTableWidget=new InputLoadTableWidget(LoadTableForm(),this);

    QTabWidget* tab_widget=new QTabWidget(this);
    tab_widget->addTab(jointTableWidget,tr("Node data"));
    tab_widget->addTab(memberTableWidget,tr("Member data"));
    tab_widget->addTab(loadTableWidget,tr("Load data"));
    tab_widget->setIconSize(QSize(20,20));

    QVBoxLayout *vlay=new QVBoxLayout;
    vlay->setContentsMargins(10,10,10,10);
    vlay->setSpacing(5);
    vlay->addLayout(buttonsLayout);
    vlay->addWidget(tab_widget);
    this->setLayout(vlay);

    this->setWindowTitle(tr("Enter text"));
}

void TextInputDialog::createButtons()
{

    QPushButton *open_button = new QPushButton(tr("Read file"),this);
    open_button->setFixedSize(90,30);
    open_button->setAutoDefault(false);
    QPushButton *save_button = new QPushButton(tr("Save file"),this);
    save_button->setFixedSize(90,30);
    save_button->setAutoDefault(false);

    QPushButton *import_button = new QPushButton(tr("Data acquisition"),this);
    import_button->setFixedSize(90,30);
    connect( import_button, SIGNAL( clicked() ), this, SLOT( slotDataImport() ) );

    QPushButton *jup_button = new QPushButton(tr("Register node"),this);
    jup_button->setFixedSize(80,30);
    jup_button->setAutoDefault(false);
    QPushButton *mup_button = new QPushButton(tr("Member registration"),this);
    mup_button->setFixedSize(80,30);
    mup_button->setAutoDefault(false);
    QPushButton *lup_button = new QPushButton(tr("Load registration"),this);
    lup_button->setFixedSize(80,30);
    lup_button->setAutoDefault(false);

    QPushButton *allup_button = new QPushButton(tr("Bulk registration"),this);
    allup_button->setFixedSize(80,30);
    allup_button->setAutoDefault(false);

    connect ( open_button, SIGNAL( clicked() ), this, SLOT( slotTextFileOpen() ) );
    connect ( save_button, SIGNAL( clicked() ), this, SLOT( slotTextFileSave() ) );

    connect ( jup_button, SIGNAL( clicked() ), this, SLOT( slotJointUpload() ) );
    connect ( mup_button, SIGNAL( clicked() ), this, SLOT( slotMemberUpload() ) );
    connect ( lup_button, SIGNAL( clicked() ), this, SLOT( slotLoadUpload() ) );
    connect ( allup_button, SIGNAL( clicked() ), this, SLOT( slotModelUpload() ) );

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->setSpacing(5);
    buttonsLayout->addWidget(open_button);
    buttonsLayout->addWidget(save_button);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(import_button);
    buttonsLayout->addWidget(new VFLine(Qt::lightGray,this));
    buttonsLayout->addWidget(jup_button);
    buttonsLayout->addWidget(mup_button);
    buttonsLayout->addWidget(lup_button);
    buttonsLayout->addWidget(new VFLine(Qt::lightGray,this));
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
    if ( !file.open(QIODevice::ReadOnly) ){
        QMessageBox::warning( this, "WARNING",tr("I can not open the file."),
                              QMessageBox::NoButton,QMessageBox::Warning );
        return;
    }

    QTextStream in(&file);
    bool ok=jointTableWidget->readTextFile(in);
    if( ok ) memberTableWidget->readTextFile(in);
    if( ok ) loadTableWidget->readTextFile(in);
    if( !ok ){
        QMessageBox::warning( this, "WARNING",tr("The format of the file is incorrect."),
                              QMessageBox::NoButton,QMessageBox::Warning );
        return;
    }
}

void TextInputDialog::slotTextFileSave()
{
    QString filename =
            QFileDialog::getSaveFileName( this, QString(), QString(),"File(*.txt)" );
    if ( filename.isEmpty() ) return;
    QFile file(filename);
    if ( !file.open(QIODevice::WriteOnly) ){
        QMessageBox::warning( this, "WARNING",tr("You can not write to the specified file."),
                              QMessageBox::NoButton,QMessageBox::Warning );
        return;
    }

    QTextStream outs(&file);

    jointTableWidget->writeTextFile(outs);
    memberTableWidget->writeTextFile(outs);
    loadTableWidget->writeTextFile(outs);
}

void TextInputDialog::slotDataImport()
{
    jointTableWidget->importJointData( UnifiedData::getInstance()->jointListOfAll() );
    QList<DATATYPE> mtypes,ltypes;
    mtypes << TPCOLUMN << TPGIRDER << TPBRACE << TPBEAM << TPSLAB << TPWALL << TPUWALL
           << TPDAMPER << TPISO;
    memberTableWidget->importMemberData( UnifiedData::getInstance()->memberListOfType(mtypes) );
    ltypes << TPJOINTLOAD << TPMEMBERLOAD << TPSLABLOAD;
    loadTableWidget->importMemberData( UnifiedData::getInstance()->memberListOfType(ltypes) );
}

void TextInputDialog::slotJointUpload()
{
    if( informationUpdate() ) return;

    QString msg=jointTableWidget->checkJointData();
    if( !msg.isEmpty() ){
        QMessageBox::warning( this, "ERROR", msg, QMessageBox::NoButton, QMessageBox::Warning );
        return;
    }

    UnifiedData::getInstance()->clearCopyBuffer();
    jointTableWidget->uploadJoints();

    UnifiedData::getInstance()->signalElementCountChanged();
    UnifiedData::getInstance()->clearUndoStack();
}

void TextInputDialog::slotMemberUpload()
{
    if( informationUpdate() ) return;

    QString msg=memberTableWidget->checkMemberData( UnifiedData::getInstance()->getDataCount(TPJOINT)+1 );
    if( !msg.isEmpty() ){
        QMessageBox::warning( this, "ERROR", msg, QMessageBox::NoButton, QMessageBox::Warning );
        return;
    }

    UnifiedData::getInstance()->clearCopyBuffer();
    memberTableWidget->uploadMembers();

    UnifiedData::getInstance()->signalElementCountChanged();
    UnifiedData::getInstance()->clearUndoStack();
}

void TextInputDialog::slotLoadUpload()
{
    if( informationUpdate() ) return;

    QString msg=loadTableWidget->checkLoadData( UnifiedData::getInstance()->getDataCount(TPJOINT)+1 );
    if( !msg.isEmpty() ){
        QMessageBox::warning( this, "ERROR", msg, QMessageBox::NoButton, QMessageBox::Warning );
        return;
    }

    UnifiedData::getInstance()->clearCopyBuffer();
    loadTableWidget->uploadLoads();

    UnifiedData::getInstance()->signalElementCountChanged();
    UnifiedData::getInstance()->clearUndoStack();
}

void TextInputDialog::slotModelUpload()
{
    if( informationUpdate() ) return;

    QString msg=jointTableWidget->checkJointData();
    if( !msg.isEmpty() ){
        QMessageBox::warning( this, "ERROR", msg, QMessageBox::NoButton, QMessageBox::Warning );
        return;
    }
    int jcount=jointTableWidget->getJointCount();
    msg=memberTableWidget->checkMemberData( jcount+1 );
    if( !msg.isEmpty() ){
        QMessageBox::warning( this, "ERROR", msg, QMessageBox::NoButton, QMessageBox::Warning );
        return;
    }
    msg=loadTableWidget->checkLoadData( jcount+1 );
    if( !msg.isEmpty() ){
        QMessageBox::warning( this, "ERROR", msg, QMessageBox::NoButton, QMessageBox::Warning );
        return;
    }

    UnifiedData::getInstance()->clearCopyBuffer();
    jointTableWidget->uploadJoints();
    memberTableWidget->uploadMembers();
    loadTableWidget->uploadLoads();

    UnifiedData::getInstance()->signalElementCountChanged();
    UnifiedData::getInstance()->clearUndoStack();
}

bool TextInputDialog::informationUpdate()
{
    int ret = QMessageBox::information(this, tr("Confirmation"),
                                   tr("Registered data can not be restored.") + "\n" + tr("Continue processing?"),
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Ok );
    return (ret==QMessageBox::Cancel);

}

