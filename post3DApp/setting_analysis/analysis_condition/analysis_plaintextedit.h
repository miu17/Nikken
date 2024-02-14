#ifndef ANALYSISPLAINTEXTEDIT_H
#define ANALYSISPLAINTEXTEDIT_H

#include "custom_plaintextedit.h"

namespace post3dapp{

class AnalysisPlainTextEdit : public CustomPlainTextEdit
{
    Q_OBJECT
public:
    AnalysisPlainTextEdit(QWidget *parent = nullptr);
    bool hasFormatError() const override;
    QString getStatusMessage() const override;

protected:
    bool eventFilter(QObject *obj,QEvent *event) override;

private:
    void decorateCells() const override;

    QList<QUuid> joints;
    QList<QUuid> members;

};

} // namespace post3dapp
#endif // ANALYSISPLAINTEXTEDIT_H
