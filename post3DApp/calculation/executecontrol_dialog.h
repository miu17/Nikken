#ifndef EXECUTECONTROLDIALOG_H
#define EXECUTECONTROLDIALOG_H

#include <QDialog>

class QTextEdit;
class QLabel;
class QUuid;


namespace post3dapp{
enum class DATATYPE;

class ExecuteControlDialog : public QDialog
{
    Q_OBJECT

public:

    ExecuteControlDialog( QWidget *parent = 0 );

protected:

    void closeEvent(QCloseEvent *) override;

signals:

public slots:
    void slotCancel();

private:

    QTextEdit *messageEditor;

    bool isOkToClose();
    void makeIndexFile();
    void run();
    QString writeNsv() const;
    QString writeAnalysisDat() const;

    QList<QUuid> stringToFloorFrameRange(const QString &, DATATYPE);
};
} // namespace post3dapp

#endif

