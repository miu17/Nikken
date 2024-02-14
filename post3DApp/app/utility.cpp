#include "utility.h"

#include <QMessageBox>

namespace post3dapp {

void Utility::showErrorMessage(QWidget* parent, const QString &msg)
{
    QMessageBox::critical( parent, "Error", msg );
}

void Utility::showErrorMessage2(QWidget* parent, const QString &msg)
{
    QMessageBox::critical( parent, "Error", msg, QMessageBox::NoButton, QMessageBox::Warning );
}


void Utility::showWarningMessage(QWidget* parent, const QString &msg)
{
    QMessageBox::warning( parent, "Warning", msg, QMessageBox::NoButton, QMessageBox::Warning );
}

bool Utility::showWarningMessage2(QWidget* parent, const QString &msg)
{
    int ret = QMessageBox::warning(parent, "Warning", msg,
                                   QMessageBox::Ok | QMessageBox::Cancel,
                                   QMessageBox::Cancel );
    return (ret == QMessageBox::Cancel);
}

void Utility::showInformationMessage(QWidget* parent, const QString &msg)
{
    QMessageBox::information( parent, "Information", msg );
}



bool Utility::showCheckDialog(const QString& msg, QWidget* parent){
    QMessageBox message(parent);
    message.setText(msg);
    message.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    if (QMessageBox::Ok != message.exec())
        return false;
    else
        return true;
}

bool Utility::informationUpdate(QWidget* parent){
    int ret = QMessageBox::information(parent, u8"確 認",
                                       u8"前回登録時のデータは失われます。\n現データを登録しますか？",
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Ok );
    return (ret == QMessageBox::Cancel);
}

bool Utility::informationRewind(QWidget* parent)
{
    int ret = QMessageBox::information(parent,
                                       u8"確 認",
                                       u8"編集中のデータは失われます。\n前回登録時の状態を復元しますか？",
                                       QMessageBox::Ok | QMessageBox::Cancel,
                                       QMessageBox::Ok );
    return (ret == QMessageBox::Cancel);

}

void Utility::messageFormatError(QWidget* parent)
{
    QMessageBox::warning( parent, "WARNING", u8"ファイルのフォーマットが間違っています。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}

void Utility::messageFileOpenError(QWidget* parent)
{
    QMessageBox::warning( parent, "WARNING", u8"ファイルを開けません。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}

void Utility::messageFileSaveError(QWidget* parent)
{
    QMessageBox::warning( parent, "WARNING", u8"指定のファイルには書き込めません。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}

void Utility::messageInvalidValue(QWidget* parent)
{
    QMessageBox::warning( parent, "WARNING",
                          u8"入力された値が不正です。",
                          QMessageBox::NoButton, QMessageBox::Warning );
}
} // namespace post3dapp
