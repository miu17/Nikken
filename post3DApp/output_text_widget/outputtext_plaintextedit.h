#ifndef OUTPUTTEXTPLAINTEXTEDIT_H
#define OUTPUTTEXTPLAINTEXTEDIT_H

#include "custom_plaintextedit.h"
namespace post3dapp{

class OutputTextPlainTextEdit : public CustomPlainTextEdit
{
    Q_OBJECT
public:
    OutputTextPlainTextEdit(QWidget *parent = nullptr);
    bool hasFormatError() const override;
    QString getStatusMessage() const override;

protected:
    bool eventFilter(QObject *obj,QEvent *event) override;

private:
    void decorateCells() const override;
};
} // namespace post3dapp

#endif // OUTPUTTEXTPLAINTEXTEDIT_H
