#include <QtGui>

#include "central_widget.h"
#include "main_widget.h"
#include "manage_jointdata.h"
#include "manage_memberdata.h"
#include "operation_area.h"
#include "topmenu_widget.h"
#include "unified_inputdata.h"
#include "unified_editingdata.h"

namespace post3dapp{

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    this->setWindowIcon(QIcon(QPixmap(":/icons/c2_yellow.png")));
    this->setWindowTitle("Post3D MAIN");
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, Qt::white);
    this->setPalette(pal);

    topWidget = new TopMenuWidget(this);
    centralWidget = new CentralWidget(this);

    OperationArea *operationArea = new OperationArea(this);
    operationArea->setFont(QFont(tr("Microsoft JhengHei"), 9));

    QSplitter *spl = new QSplitter(Qt::Horizontal, this);
    spl->addWidget(operationArea);
    spl->addWidget(centralWidget);
    spl->setStretchFactor(0, 0);
    spl->setStretchFactor(1, 1);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(topWidget);
    vlay->addWidget(spl, 1);
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);

    this->setLayout(vlay);

    isModified = false;
    connect(UnifiedInputData::getInInstance(), &UnifiedData::dataUpdated, this, &MainWidget::setModified);
    connect(UnifiedInputData::getInInstance(), &UnifiedData::jointAppended, this,
            &MainWidget::setModified);
    connect(UnifiedInputData::getInInstance(), &UnifiedData::jointChanged, this,
            &MainWidget::setModified);
    connect(UnifiedInputData::getInInstance(), &UnifiedData::jointRemoved, this,
            &MainWidget::setModified);
    connect(UnifiedInputData::getInInstance(), &UnifiedData::memberAppended, this,
            &MainWidget::setModified);
    connect(UnifiedInputData::getInInstance(), &UnifiedData::memberChanged, this,
            &MainWidget::setModified);
    connect(UnifiedInputData::getInInstance(), &UnifiedData::memberRemoved,
            this, &MainWidget::setModified);

    connect(topWidget, &TopMenuWidget::saveFileChanged, this, &MainWidget::slotChangeTitle);
}

//Miu 使えるかも　データが書き換わったらの処理
void MainWidget::setModified()
{
    isModified = true;
    QString str = QString("Post3D MAIN  - [ ") + currentFileName + (".p3d* ]");
    this->setWindowTitle(str);
    if ( !currentFileName.isEmpty() ) topWidget->enableSaveAsButton(true);
}

void MainWidget::slotChangeTitle()
{
    currentFileName = UnifiedEditingData::getInstance()->currentFileBaseName();
    QString str = QString("Post3D MAIN  - [ ") + currentFileName + (".p3d ]");
    this->setWindowTitle(str);
    topWidget->enableSaveAsButton(false);
    topWidget->enableAutoSaveButton(true); //miu 自動保存ボタンを有効にする
    isModified = false;
}

void MainWidget::closeEvent ( QCloseEvent *event )
{
    if ( isModified ) {
        QMessageBox msg(this);
        msg.setWindowTitle(u8"Post3D MAIN の終了確認");
        msg.setText(u8"保存されていないデータがあります。\n編集中のデータを保存しますか？");
        msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Save);
        msg.button( QMessageBox::Save )->setText(u8"保存して戻る");
        msg.button( QMessageBox::Discard )->setText(u8"保存せずに終了");
        msg.button( QMessageBox::Cancel )->setText(u8"キャンセル");
        int ret = msg.exec();
        if ( ret == QMessageBox::Cancel ) {
            event->ignore();
            return;
        }
        if ( ret == QMessageBox::Save ) {
            topWidget->slotFileSave();
            event->ignore();
            return;
        }
        delete centralWidget;
        event->accept();
    } else {
        QMessageBox msg(this);
        msg.setWindowTitle(u8"Post3D MAIN の終了確認");
        msg.setText(u8"終了してよろしいですか？");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Yes);
        msg.button( QMessageBox::Yes )->setText(u8"終了");
        msg.button( QMessageBox::Cancel )->setText(u8"キャンセル");
        int ret = msg.exec();
        if ( ret == QMessageBox::Cancel ) {
            event->ignore();
            return;
        }
        delete centralWidget;
        event->accept();
    }
}

} // namespace post3dapp
