#ifndef CHANGEANGLEDIALOG_H
#define CHANGEANGLEDIALOG_H

#include <QDialog>

class QLineEdit;
namespace post3dapp{
class ChangeAngleDialog : public QDialog
{
    Q_OBJECT

public:

    ChangeAngleDialog( const QString &, QWidget *parent = 0 );
    QString valueText() const;

private:

    void createLayout();

    QLineEdit *angleLineEdit;

};
} // namespace post3dapp
#endif
