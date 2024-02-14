#ifndef TEXTCALCLOADDIALOG_H
#define TEXTCALCLOADDIALOG_H

#include <QDialog>
#include <QTextEdit>

#include "unified_calcload.h"

namespace post3dapp{
class TextCalcLoadDialog : public QDialog
{
    Q_OBJECT

public:

    TextCalcLoadDialog( QWidget *parent = nullptr );
    void setEditorText(const QList<JointData*>&, const QList<MemberData*>&);

private:
    QTextEdit *textEditor;
    void createLayout();
    QString getLoadOutput(const QList<JointData*>&, const QList<MemberData*>&) const;
};
} // namespace post3dapp
#endif
