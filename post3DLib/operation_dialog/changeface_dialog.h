#ifndef CHANGEFACEDIALOG_H
#define CHANGEFACEDIALOG_H

#include <QtWidgets>
#include <QDialog>

class QLineEdit;
class QButtonGroup;
namespace post3dapp{
class ChangeFaceDialog : public QDialog
{
    Q_OBJECT

public:

    ChangeFaceDialog( int, const QString &,
                      int, const QString &, QWidget *parent = 0 );

    int ifaceRuleCheckedID() const;
    int jfaceRuleCheckedID() const;
    QString ifaceValueText() const;
    QString jfaceValueText() const;

private slots:

    void enableIFaceLineEdit(int);
    void enableJFaceLineEdit(int);

signals:

private:

    void createLayout();

    QLineEdit *angleLineEdit;

    QButtonGroup *ifaceRuleGroup;
    QLineEdit *ifaceLine;
    QButtonGroup *jfaceRuleGroup;
    QLineEdit *jfaceLine;

};
} // namespace post3dapp
#endif

