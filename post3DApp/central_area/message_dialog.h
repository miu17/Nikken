#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>

class QLabel;
class QTextEdit;

namespace post3dapp{
class MessageDialog : public QDialog
{
    Q_OBJECT

public:

    MessageDialog(QWidget *parent = nullptr );

protected:

    void closeEvent(QCloseEvent *);
    void keyPressEvent ( QKeyEvent * ) { }

signals:

    void windowClosed();

public slots:

    void slotChangeMessage(const QString &);
    void slotClearMessage();
    void slotSaveMessage();

private:

    QTextEdit *messageEditor;

};
} // namespace post3dapp

#endif

