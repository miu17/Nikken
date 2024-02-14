#ifndef CHANGESHIFTDIALOG_H
#define CHANGESHIFTDIALOG_H

#include <QtWidgets>
#include <QDialog>

class QLabel;
class QLineEdit;
namespace post3dapp{
class ChangeShiftDialog : public QDialog
{
    Q_OBJECT

public:

    ChangeShiftDialog( const QString &, const QString &, QWidget *parent = 0 );

    QString shiftYText() const;
    QString shiftZText() const;

private:

    void createLayout();

    QLineEdit *shiftYLineEdit;
    QLineEdit *shiftZLineEdit;

};
} // namespace post3dapp
#endif
