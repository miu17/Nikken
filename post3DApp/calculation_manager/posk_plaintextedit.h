#ifndef POSKPLAINTEXTEDIT_H
#define POSKPLAINTEXTEDIT_H

#include "custom_plaintextedit.h"

namespace post3dapp{

class PoskPlainTextEdit : public CustomPlainTextEdit
{
    Q_OBJECT
public:
    PoskPlainTextEdit(QWidget *parent = nullptr);
    bool hasFormatError() const override;
    QString getStatusMessage() const override;

protected:
    bool eventFilter(QObject *obj,QEvent *event) override;

private:
    void decorateCells() const override;
};

} // namespace post3dapp

#endif // POSKPLAINTEXTEDIT_H
