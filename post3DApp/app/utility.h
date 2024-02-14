#ifndef UTILITY_H
#define UTILITY_H

#include <QString>
#include <QWidget>

namespace post3dapp {

class Utility
{
public:
    static void showErrorMessage(QWidget* parent, const QString &);
    static void showErrorMessage2(QWidget* parent, const QString &);
    static void showWarningMessage(QWidget* parent, const QString &);
    static bool showWarningMessage2(QWidget* parent, const QString &);
    static void showInformationMessage(QWidget* parent, const QString &);
    static bool showCheckDialog(const QString&, QWidget* parent = nullptr);
    static bool informationUpdate(QWidget* parent);
    static bool informationRewind(QWidget* parent);
    static void messageFormatError(QWidget* parent);
    static void messageFileOpenError(QWidget* parent);
    static void messageFileSaveError(QWidget* parent);
    static void messageInvalidValue(QWidget* parent);
};

}

#endif // UTILITY_H
