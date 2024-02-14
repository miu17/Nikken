#ifndef TEXTOUTPUTDIALOG_H
#define TEXTOUTPUTDIALOG_H

#include <QDialog>

class QTextEdit;

namespace post3dapp{
struct FIGURE_TYPE_PROPERTY;

class TextOutputDialog : public QDialog
{
    Q_OBJECT

public:

    TextOutputDialog( QWidget *parent = nullptr );

    void setEditorText(const FIGURE_TYPE_PROPERTY&, const QList<QUuid> &, const QList<QUuid> & , const QUuid &id);

public slots:

signals:

private:

    QTextEdit *textEditor;

    void createLayout();
};
} // namespace post3dapp
#endif
