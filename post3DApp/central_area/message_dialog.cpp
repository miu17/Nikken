#include "message_dialog.h"

#include <QtGui>
#include <QtWidgets>

#include "unified_editingdata.h"
#include "utility.h"

namespace post3dapp{

MessageDialog::MessageDialog(QWidget *parent)
    : QDialog(parent)
{
    this->setFont(QFont(tr("Microsoft JhengHei"), 10));
    QLabel *title = new QLabel(u8"操作履歴", this);

    title->setFixedHeight(24);
    title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QPushButton *clearbutton = new QPushButton("Clear", this);
    QPushButton *savebutton = new QPushButton("Save Log", this);
    clearbutton->setFixedSize(80, 20);
    savebutton->setFixedSize(80, 20);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setContentsMargins(0, 0, 0, 0);
    hlay->addWidget(title);
    hlay->addStretch();
    hlay->addWidget(clearbutton);
    hlay->addSpacing(5);
    hlay->addWidget(savebutton);

    connect(clearbutton, &QAbstractButton::clicked, this, &MessageDialog::slotClearMessage);
    connect(savebutton, &QAbstractButton::clicked, this, &MessageDialog::slotSaveMessage);

    messageEditor = new QTextEdit(this);
    messageEditor->setFont(QFont(tr("Microsoft JhengHei"), 9));
    messageEditor->setReadOnly(true);
    messageEditor->setTextColor(QColor(30, 30, 30));
    messageEditor->setMinimumHeight(30);

    connect(UnifiedEditingData::getInstance(), &UnifiedEditingData::logMessageChanged,
            this, &MessageDialog::slotChangeMessage);

    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->setContentsMargins(0, 0, 0, 0);
    vlay->setSpacing(0);
    vlay->addLayout(hlay);
    vlay->addWidget(messageEditor);

    this->setLayout(vlay);

    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(Qt::white));
    this->setPalette(pal);
    this->setWindowTitle(u8"操作履歴");
    this->setWindowIcon(QIcon(QPixmap(":/icons/message.png")));
    this->setWindowFlags(Qt::Drawer);
    this->resize(QSize(650, 200));
}

void MessageDialog::closeEvent(QCloseEvent *event)
{
    emit windowClosed();
    QDialog::closeEvent(event);
}

void MessageDialog::slotChangeMessage(const QString &str)
{
    messageEditor->setPlainText(str);
    messageEditor->moveCursor(QTextCursor::End);
}

void MessageDialog::slotClearMessage()
{
    UnifiedEditingData::getInstance()->clearLogMessage();
}

void MessageDialog::slotSaveMessage()
{
    QString filename =
        QFileDialog::getSaveFileName( this, QString(), QString(), "File(*.log)" );
    if ( filename.isEmpty() ) return;
    QFile file(filename);
    if ( !file.open(QIODevice::WriteOnly) ) {
        Utility::messageFileSaveError(this);
        return;
    }

    QTextStream outs(&file);
    outs << UnifiedEditingData::getInstance()->getCurrentLogMessage();
    file.close();
}
} // namespace post3dapp
